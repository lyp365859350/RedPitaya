#include "main.h"

#include <math.h>
#include <stdio.h>
#include "version.h"


/* -------------------------  debug parameter  --------------------------------- */
CIntParameter signalPeriiod("DEBUG_SIGNAL_PERIOD", CBaseParameter::RW, 100, 0, 0, 10000);
CIntParameter parameterPeriiod("DEBUG_PARAM_PERIOD", CBaseParameter::RW, 200, 0, 0, 10000);
CBooleanParameter digitalLoop("DIGITAL_LOOP", CBaseParameter::RW, false, 0);



/***************************************************************************************
*                                     OSCILLOSCOPE                                     *
****************************************************************************************/

/* --------------------------------  OUT SIGNALS  ------------------------------ */
CFloatSignal ch1("ch1", CH_SIGNAL_SIZE_DEFAULT, 0.0f);
CFloatSignal ch2("ch2", CH_SIGNAL_SIZE_DEFAULT, 0.0f);
CFloatSignal math("math", CH_SIGNAL_SIZE_DEFAULT, 0.0f);

/* ------------------------------- DATA PARAMETERS ------------------------------ */
CIntParameter dataSize("OSC_DATA_SIZE", CBaseParameter::RW, CH_SIGNAL_SIZE_DEFAULT, 0, 1, 16*1024);
CFloatParameter viewPortion("OSC_VIEV_PART", CBaseParameter::RO, 0.1, 0, 0, 1);
CIntParameter samplingRate("OSC_SAMPL_RATE", CBaseParameter::RW, RP_SMP_125M, 0, RP_SMP_125M, RP_SMP_1_907K);

/* --------------------------------  OUT PARAMETERS  ------------------------------ */
CBooleanParameter in1Show("CH1_SHOW", CBaseParameter::RW, true, 0);
CBooleanParameter in2Show("CH2_SHOW", CBaseParameter::RW, true, 0);

CBooleanParameter in1InvShow("CH1_SHOW_INVERTED", CBaseParameter::RW, false, 0);
CBooleanParameter in2InvShow("CH2_SHOW_INVERTED", CBaseParameter::RW, false, 0);
CBooleanParameter mathInvShow("MATH_SHOW_INVERTED", CBaseParameter::RW, false, 0);

CBooleanParameter inReset("OSC_RST", CBaseParameter::RW, false, 0);
CBooleanParameter inRun("OSC_RUN", CBaseParameter::RW, true, 0);
CBooleanParameter inAutoscale("OSC_AUTOSCALE", CBaseParameter::RW, false, 0);
CBooleanParameter inSingle("OSC_SINGLE", CBaseParameter::RW, false, 0);

CFloatParameter in1Offset("OSC_CH1_OFFSET", CBaseParameter::RWSA, 0, 0, -40, 40);
CFloatParameter in2Offset("OSC_CH2_OFFSET", CBaseParameter::RWSA, 0, 0, -40, 40);
CFloatParameter inMathOffset("OSC_MATH_OFFSET", CBaseParameter::RW, 0, 0, -40, 40);
CFloatParameter in1Scale("OSC_CH1_SCALE", CBaseParameter::RW, 1, 0, 0.00005, 1000);
CFloatParameter in2Scale("OSC_CH2_SCALE", CBaseParameter::RW, 1, 0, 0.00005, 1000);
CFloatParameter out1Scale("OSC_OUTPUT1_SCALE", CBaseParameter::RWSA, 1, 0, 0.00005, 1000);
CFloatParameter out2Scale("OSC_OUTPUT2_SCALE", CBaseParameter::RWSA, 1, 0, 0.00005, 1000);
CFloatParameter inMathScale("OSC_MATH_SCALE", CBaseParameter::RW, 1, 0, 0.00005, 1000);
CFloatParameter in1Probe("OSC_CH1_PROBE", CBaseParameter::RW, 1, 0, 0, 1000);
CFloatParameter in2Probe("OSC_CH2_PROBE", CBaseParameter::RW, 1, 0, 0, 1000);
CFloatParameter inTimeOffset("OSC_TIME_OFFSET", CBaseParameter::RW, 0, 0, -100000, 100000);
CFloatParameter inTimeScale("OSC_TIME_SCALE", CBaseParameter::RW, 1, 0, 0.00005, 50000);

CIntParameter in1Gain("OSC_CH1_IN_GAIN", CBaseParameter::RW, 0, 0, 0, 1);
CIntParameter in2Gain("OSC_CH2_IN_GAIN", CBaseParameter::RW, 0, 0, 0, 1);

/* --------------------------------  TRIGGER PARAMETERS --------------------------- */
CFloatParameter inTriggLevel("OSC_TRIG_LEVEL", CBaseParameter::RW, 0, 0, -20, 20);
CFloatParameter inTriggLimit("OSC_TRIG_LIMIT", CBaseParameter::RO, 0, 0, -20, 20);
CIntParameter inTrigSweep("OSC_TRIG_SWEEP", CBaseParameter::RW, RPAPP_OSC_TRIG_AUTO, 0, RPAPP_OSC_TRIG_AUTO, RPAPP_OSC_TRIG_SINGLE);
CIntParameter inTrigSource("OSC_TRIG_SOURCE", CBaseParameter::RW, RPAPP_OSC_TRIG_SRC_CH1, 0, RPAPP_OSC_TRIG_SRC_CH1, RPAPP_OSC_TRIG_SRC_EXTERNAL);
CIntParameter inTrigSlope("OSC_TRIG_SLOPE", CBaseParameter::RW, RPAPP_OSC_TRIG_SLOPE_PE, 0, RPAPP_OSC_TRIG_SLOPE_NE, RPAPP_OSC_TRIG_SLOPE_PE);

/* --------------------------------  MEASURE  ------------------------------ */
CIntParameter measureSelect1("OSC_MEAS_SEL1", CBaseParameter::RW, -1, 0, -1, 23);
CIntParameter measureSelect2("OSC_MEAS_SEL2", CBaseParameter::RW, -1, 0, -1, 23);
CIntParameter measureSelect3("OSC_MEAS_SEL3", CBaseParameter::RW, -1, 0, -1, 23);
CIntParameter measureSelect4("OSC_MEAS_SEL4", CBaseParameter::RW, -1, 0, -1, 23);

CFloatParameter measureValue1("OSC_MEAS_VAL1", CBaseParameter::RWSA, 0, 0, -1000000, 1000000);
CFloatParameter measureValue2("OSC_MEAS_VAL2", CBaseParameter::RWSA, 0, 0, -1000000, 1000000);
CFloatParameter measureValue3("OSC_MEAS_VAL3", CBaseParameter::RWSA, 0, 0, -1000000, 1000000);
CFloatParameter measureValue4("OSC_MEAS_VAL4", CBaseParameter::RWSA, 0, 0, -1000000, 1000000);

/* --------------------------------  CURSORS  ------------------------------ */
CBooleanParameter cursorx1("OSC_CURSOR_X1", CBaseParameter::RW, false, 0);
CBooleanParameter cursorx2("OSC_CURSOR_X2", CBaseParameter::RW, false, 0);
CBooleanParameter cursory1("OSC_CURSOR_Y1", CBaseParameter::RW, false, 0);
CBooleanParameter cursory2("OSC_CURSOR_Y2", CBaseParameter::RW, false, 0);
CIntParameter cursorSrc("OSC_CURSOR_SRC", CBaseParameter::RW, RPAPP_OSC_SOUR_CH1, 0, RPAPP_OSC_SOUR_CH1, RPAPP_OSC_SOUR_MATH);

CFloatParameter cursor1V("OSC_CUR1_V", CBaseParameter::RW, -1, 0, -1000, 1000);
CFloatParameter cursor2V("OSC_CUR2_V", CBaseParameter::RW, -1, 0, -1000, 1000);
CFloatParameter cursor1T("OSC_CUR1_T", CBaseParameter::RW, -1, 0, -1000, 1000);
CFloatParameter cursor2T("OSC_CUR2_T", CBaseParameter::RW, -1, 0, -1000, 1000);

/* ----------------------------------  MATH  -------------------------------- */
CIntParameter mathOperation("OSC_MATH_OP", CBaseParameter::RW, RPAPP_OSC_MATH_NONE, 0, RPAPP_OSC_MATH_NONE, RPAPP_OSC_MATH_INT);
CIntParameter mathSource1("OSC_MATH_SRC1", CBaseParameter::RW, RP_CH_1, 0, RP_CH_1, RP_CH_2);
CIntParameter mathSource2("OSC_MATH_SRC2", CBaseParameter::RW, RP_CH_2, 0, RP_CH_1, RP_CH_2);



/***************************************************************************************
*                                      GENERATE                                        *
****************************************************************************************/

/* ------------------------------  OUT ARBITRARY SIGNAL ------------------------------ */
//CCustomParameter<void> arbitrarySignal("arbitrarySignal", CBaseParameter::RW,  NULL, 0, NULL, NULL);
//CIntParameter arbitraryChannel("SOUR_TRAC_CH", CBaseParameter::RW, RP_CH_1, 0, RP_CH_1, RP_CH_2);

/* --------------------------------  OUTOUT PARAMETERS  ------------------------------ */
CFloatSignal out1Signal("output1", CH_SIGNAL_SIZE_DEFAULT, 0.0f);
CFloatSignal out2Signal("output2", CH_SIGNAL_SIZE_DEFAULT, 0.0f);
bool updateOutCh1 = true;
bool updateOutCh2 = true;

CBooleanParameter out1Show("OUTPUT1_SHOW", CBaseParameter::RW, true, 0);
CBooleanParameter out2Show("OUTPUT2_SHOW", CBaseParameter::RW, true, 0);

CBooleanParameter out1State("OUTPUT1_STATE", CBaseParameter::RW, false, 0);
CBooleanParameter out2State("OUTPUT2_STATE", CBaseParameter::RW, false, 0);
CFloatParameter out1Amplitude("SOUR1_VOLT", CBaseParameter::RW, 0.9, 0, -1, 1);
CFloatParameter out2Amplitude("SOUR2_VOLT", CBaseParameter::RW, 0.9, 0, -1, 1);
CFloatParameter out1Offset("SOUR1_VOLT_OFFS", CBaseParameter::RW, 0, 0, -1, 1);
CFloatParameter out2Offset("SOUR2_VOLT_OFFS", CBaseParameter::RW, 0, 0, -1, 1);
CFloatParameter out1Frequancy("SOUR1_FREQ_FIX", CBaseParameter::RW, 1000, 0, 0.00005, 62.5e6);
CFloatParameter out2Frequancy("SOUR2_FREQ_FIX", CBaseParameter::RW, 1000, 0, 0.00005, 62.5e6);

CFloatParameter out1Phase("SOUR1_PHAS", CBaseParameter::RW, 0, 0, -360, 360);
CFloatParameter out2Phase("SOUR2_PHAS", CBaseParameter::RW, 0, 0, -360, 360);
CFloatParameter out1DCYC("SOUR1_DCYC", CBaseParameter::RW, 0.5, 0, 0, 100);
CFloatParameter out2DCYC("SOUR2_DCYC", CBaseParameter::RW, 0.5, 0, 0, 100);

CIntParameter out1WAveform("SOUR1_FUNC", CBaseParameter::RW, RP_WAVEFORM_SINE, 0, RP_WAVEFORM_SINE, RP_WAVEFORM_ARBITRARY);
CIntParameter out2WAveform("SOUR2_FUNC", CBaseParameter::RW, RP_WAVEFORM_SINE, 0, RP_WAVEFORM_SINE, RP_WAVEFORM_ARBITRARY);

CIntParameter out1Burst("SOUR1_BURS_STAT", CBaseParameter::RW, 0, 0, 0, 1);
CIntParameter out2Burst("SOUR2_BURS_STAT", CBaseParameter::RW, 0, 0, 0, 1);

CIntParameter out1TriggerSource("SOUR1_TRIG_SOUR", CBaseParameter::RW, RP_GEN_TRIG_SRC_INTERNAL, 0, RP_GEN_TRIG_SRC_INTERNAL, RP_GEN_TRIG_GATED_BURST);
CIntParameter out2TriggerSource("SOUR2_TRIG_SOUR", CBaseParameter::RW, RP_GEN_TRIG_SRC_INTERNAL, 0, RP_GEN_TRIG_SRC_INTERNAL, RP_GEN_TRIG_GATED_BURST);

CFloatParameter out1ShowOffset("OUTPUT1_SHOW_OFF", CBaseParameter::RW, 0, 0, -40, 40);
CFloatParameter out2ShowOffset("OUTPUT2_SHOW_OFF", CBaseParameter::RW, 0, 0, -40, 40);

/***************************************************************************************
*                                      CALIBATE                                        *
****************************************************************************************/

// 0-nothing		1-commant from web		-1-response OK
// 1V - TP16
CIntParameter calibrateFrontEndOffset("CLAIB_FE_OFF", CBaseParameter::RW, 0, 0, -1, 1);
CIntParameter calibrateFrontEndScaleLV("CLAIB_FE_SCALE_LV", CBaseParameter::RW, 0, 0, -1, 1);
CIntParameter calibrateFrontEndScaleHV("CLAIB_FE_SCALE_HV", CBaseParameter::RW, 0, 0, -1, 1);
CIntParameter calibrateBackEndOffset("CLAIB_BE_OFF", CBaseParameter::RW, 0, 0, -1, 1);
CIntParameter calibrateBackEndScale("CLAIB_BE_SCALE", CBaseParameter::RW, 0, 0, -1, 1);




const char *rp_app_desc(void) {
    return (const char *)"Red Pitaya osciloscope application.\n";
}

int rp_app_init(void) {
    fprintf(stderr, "Loading scope version %s-%s.\n", VERSION_STR, REVISION_STR);
    CDataManager::GetInstance()->SetParamInterval(parameterPeriiod.Value());
    CDataManager::GetInstance()->SetSignalInterval(signalPeriiod.Value());

    rpApp_Init();
    rpApp_OscRun();
    return 0;
}

int rp_app_exit(void) {
    fprintf(stderr, "Unloading scope version %s-%s.\n", VERSION_STR, REVISION_STR);
    rpApp_Release();
    return 0;
}

int rp_set_params(rp_app_params_t *p, int len) {
    return 0;
}

int rp_get_params(rp_app_params_t **p) {
    return 0;
}

int rp_get_signals(float ***s, int *sig_num, int *sig_len) {
    return 0;
}


void UpdateParams(void) {
    CDataManager::GetInstance()->SetParamInterval(parameterPeriiod.Value());

    if (measureSelect1.Value() != -1) {
        measureValue1.Value() = getMeasureValue(measureSelect1.Value());
	}
    if (measureSelect2.Value() != -1) {
        measureValue2.Value() = getMeasureValue(measureSelect2.Value());
	}
    if (measureSelect3.Value() != -1) {
        measureValue3.Value() = getMeasureValue(measureSelect3.Value());
	}
    if (measureSelect4.Value() != -1) {
        measureValue4.Value() = getMeasureValue(measureSelect4.Value());
	}

    float portion;
    rpApp_OscGetViewPart(&portion);
    viewPortion.Value() = portion;
	
	float trigg_limit;
	rp_channel_t channel = (rp_channel_t) inTrigSource.Value();
	rp_AcqGetGainV(channel, &trigg_limit);
	inTriggLimit.Value() = trigg_limit;
	
    rp_acq_sampling_rate_t sampling_rate;
    rp_AcqGetSamplingRate(&sampling_rate);
    samplingRate.Value() = sampling_rate;

    bool running;
    rpApp_OscIsRunning(&running);
    inRun.Value() = running;

    rp_EnableDigitalLoop(digitalLoop.Value() || IsDemoParam.Value());

    rpApp_OscGetAmplitudeOffset(RPAPP_OSC_SOUR_CH1, &in1Offset.Value());
    rpApp_OscGetAmplitudeOffset(RPAPP_OSC_SOUR_CH2, &in2Offset.Value());
    rpApp_OscGetAmplitudeOffset(RPAPP_OSC_SOUR_MATH, &inMathOffset.Value());

	
    float value;
    rpApp_OscGetAmplitudeScale(RPAPP_OSC_SOUR_CH1, &value);
    in1Scale.Value() = value;
    rpApp_OscGetAmplitudeScale(RPAPP_OSC_SOUR_CH2, &value);
    in2Scale.Value() = value;
    rpApp_OscGetAmplitudeOffset(RPAPP_OSC_SOUR_CH1, &value);
    in1Offset.Value() = value;
    rpApp_OscGetAmplitudeOffset(RPAPP_OSC_SOUR_CH2, &value);
    in1Offset.Value() = value;
	
    rpApp_OscGetTimeOffset(&value);
    inTimeOffset.Value() = value;
    rpApp_OscGetTimeScale(&value);
    inTimeScale.Value() = value;

	if(in1Scale.IsValueChanged() || in2Scale.IsValueChanged() || in1Offset.IsValueChanged() || in1Offset.IsValueChanged()
	   || inTimeOffset.IsValueChanged() || inTimeScale.IsValueChanged()) {
		
		CDataManager::GetInstance()->SendAllParams();
		updateOutCh1 = true;
		updateOutCh1 = true;
	}
}

float getMeasureValue(int measure) {
    float value;
    switch (measure) {
        case 0:
        case 1:
        case 2:
            rpApp_OscMeasureVpp((rpApp_osc_source) (measure % 3), &value);
			value = fabs(value);
            break;
        case 3:
        case 4:
        case 5:
            rpApp_OscMeasureMeanVoltage((rpApp_osc_source) (measure % 3), &value);
            break;
        case 6:
        case 7:
        case 8:
            rpApp_OscMeasureAmplitudeMax((rpApp_osc_source) (measure % 3), &value);
            break;
        case 9:
        case 10:
        case 11:
            rpApp_OscMeasureAmplitudeMin((rpApp_osc_source) (measure % 3), &value);
            break;
        case 12:
        case 13:
        case 14:
            rpApp_OscMeasureDutyCycle((rpApp_osc_source) (measure % 3), &value);
            break;
        case 15:
        case 16:
        case 17:
            rpApp_OscMeasurePeriod((rpApp_osc_source) (measure % 3), &value);
            break;
        case 18:
        case 19:
        case 20:
            rpApp_OscMeasureFrequency((rpApp_osc_source) (measure % 3), &value);
            break;
        case 21:
        case 22:
        case 23:
            rpApp_OscMeasureRootMeanSquare((rpApp_osc_source) (measure % 3), &value);
            break;
        default:
            value = 0;
    }
    return value;
}

void UpdateSignals(void) {
    CDataManager::GetInstance()->SetSignalInterval(signalPeriiod.Value());

    float data[dataSize.Value()];
    if (in1Show.Value()) {
        rpApp_OscGetViewData(RPAPP_OSC_SOUR_CH1, data, (uint32_t) dataSize.Value());

        if (ch1.GetSize() != dataSize.Value())
            ch1.Resize(dataSize.Value());
        for (int i = 0; i < dataSize.Value(); i++)
            ch1[i] = data[i];
    } else {
        ch1.Resize(0);
    }

    if (in2Show.Value()) {
        rpApp_OscGetViewData(RPAPP_OSC_SOUR_CH2, data, (uint32_t) dataSize.Value());

        if (ch2.GetSize() != dataSize.Value())
            ch2.Resize(dataSize.Value());
        for (int i = 0; i < dataSize.Value(); i++)
            ch2[i] = data[i];
    } else {
        ch2.Resize(0);
    }

    if (mathOperation.Value() != RPAPP_OSC_MATH_NONE) {
        rpApp_OscGetViewData(RPAPP_OSC_SOUR_MATH, data, (uint32_t) dataSize.Value());

        if (math.GetSize() != dataSize.Value())
            math.Resize(dataSize.Value());
        for (int i = 0; i < dataSize.Value(); i++)
            math[i] = data[i];
    } else {
        math.Resize(0);
    }


/* ------ UPDATE OUT SIGNALS ------*/
    if (out1Show.Value() && out1State.Value() && out1Burst.Value() == 0) {
        if (out1Signal.GetSize() != dataSize.Value()) {
            out1Signal.Resize(dataSize.Value());
            generate(RP_CH_1);
        }
        if (updateOutCh1) {
            generate(RP_CH_1);
            updateOutCh1 = false;
        }
    } else {
        out1Signal.Resize(0);
    }

    if (out2Show.Value() && out2State.Value() && out2Burst.Value() == 0) {
        if (out2Signal.GetSize() != dataSize.Value()) {
            out2Signal.Resize(dataSize.Value());
            generate(RP_CH_2);
        }
        if (updateOutCh2) {
            generate(RP_CH_2);
            updateOutCh2 = false;
        }
    } else {
        out2Signal.Resize(0);
    }
}

void OnNewParams(void) {
/* ---- UPDATE INTERLAN SIGNAL GENERATION ----- */
/* ------ SEND GENERATE PARAMETERS TO API ------*/
    if (IS_NEW(out1State) || IS_NEW(out1Amplitude) || IS_NEW(out1Offset) || IS_NEW(out1Frequancy) || IS_NEW(out1Phase)
        || IS_NEW(out1DCYC) || IS_NEW(out1WAveform) || IS_NEW(out1Burst) || IS_NEW(out1TriggerSource)) {
    
        updateOutCh1 = true;
        IF_VALUE_CHANGED_BOOL(out1State, rp_GenOutEnable(RP_CH_1), rp_GenOutDisable(RP_CH_1));

        rp_GenAmp(RP_CH_1, out1Amplitude.NewValue());
        out1Amplitude.Update();

        rp_GenOffset(RP_CH_1, out1Offset.NewValue());
        out1Offset.Update();

        rp_GenFreq(RP_CH_1, out1Frequancy.NewValue());
        out1Frequancy.Update();

        rp_GenPhase(RP_CH_1, out1Phase.NewValue());
        out1Phase.Update();

        rp_GenDutyCycle(RP_CH_1, out1DCYC.NewValue());
        out1DCYC.Update();

        rp_GenWaveform(RP_CH_1, (rp_waveform_t) out1WAveform.NewValue());
        out1WAveform.Update();

        rp_GenMode(RP_CH_1, out1Burst.NewValue() == 0 ? RP_GEN_MODE_CONTINUOUS : RP_GEN_MODE_BURST);
        out1Burst.Update();

        rp_GenTriggerSource(RP_CH_1, (rp_trig_src_t) out1TriggerSource.NewValue());
        out1TriggerSource.Update();
    }

    if (IS_NEW(out2State) || IS_NEW(out2Amplitude) || IS_NEW(out2Offset) || IS_NEW(out2Frequancy) || IS_NEW(out2Phase)
        || IS_NEW(out2DCYC) || IS_NEW(out2WAveform) || IS_NEW(out2Burst) || IS_NEW(out2TriggerSource)) {

        updateOutCh2 = true;
        IF_VALUE_CHANGED_BOOL(out2State, rp_GenOutEnable(RP_CH_2), rp_GenOutDisable(RP_CH_2));

        rp_GenAmp(RP_CH_2, out2Amplitude.NewValue());
        out2Amplitude.Update();

        rp_GenOffset(RP_CH_2, out2Offset.NewValue());
        out2Offset.Update();

        rp_GenFreq(RP_CH_2, out2Frequancy.NewValue());
        out2Frequancy.Update();

        rp_GenPhase(RP_CH_2, out2Phase.NewValue());
        out2Phase.Update();

        rp_GenDutyCycle(RP_CH_2, out2DCYC.NewValue());
        out2DCYC.Update();

        rp_GenWaveform(RP_CH_2, (rp_waveform_t) out2WAveform.NewValue());
        out2WAveform.Update();

        rp_GenMode(RP_CH_2, out2Burst.NewValue() == 0 ? RP_GEN_MODE_CONTINUOUS : RP_GEN_MODE_BURST);
        out2Burst.Update();

        rp_GenTriggerSource(RP_CH_2, (rp_trig_src_t) out2TriggerSource.NewValue());
        out2TriggerSource.Update();
    }

    updateOutCh1 = updateOutCh1 || IS_NEW(inTimeScale) || IS_NEW(inAutoscale) || IS_NEW(out1ShowOffset);
    updateOutCh2 = updateOutCh2 || IS_NEW(inTimeScale) || IS_NEW(inAutoscale) || IS_NEW(out2ShowOffset);


/* ------ UPDATE OSCILLOSCOPE LOCAL PARAMETERS ------*/
    in1Show.Update();
    in2Show.Update();
    dataSize.Update();
    measureSelect1.Update();
    measureSelect2.Update();
    measureSelect3.Update();
    measureSelect4.Update();

    cursorx1.Update();
    cursorx2.Update();
    cursory1.Update();
    cursory2.Update();
    cursorSrc.Update();
    cursor1V.Update();
    cursor2V.Update();
    cursor1T.Update();
    cursor2T.Update();

	if (out1Scale.IsNewValue())
	{
		out1Scale.Update();
		generate(RP_CH_1);
	}
	if (out2Scale.IsNewValue())
	{
		out2Scale.Update();
		generate(RP_CH_2);
	}

/* ------ SEND OSCILLOSCOPE PARAMETERS TO API ------*/
    IF_VALUE_CHANGED_BOOL(inRun, rpApp_OscRun(), rpApp_OscStop())

    if (inReset.NewValue()) {
        rpApp_OscReset();
        inReset.Update();
        inReset.Value() = false;
    }
    if (inSingle.NewValue()) {
        rpApp_OscSingle();
        inSingle.Update();
        inSingle.Value() = false;
        rpApp_osc_trig_sweep_t sweep;
        rpApp_OscGetTriggerSweep(&sweep);
        inTrigSweep.Value() = sweep;
    }

    if (inAutoscale.NewValue()) {
        rpApp_OscAutoScale();
        float value;
        rpApp_OscGetAmplitudeScale(RPAPP_OSC_SOUR_CH1, &value);
        in1Scale.Value() = value;
        rpApp_OscGetAmplitudeScale(RPAPP_OSC_SOUR_CH2, &value);
        in2Scale.Value() = value;
        rpApp_OscGetAmplitudeOffset(RPAPP_OSC_SOUR_CH1, &value);
        in1Offset.Value() = value;
        rpApp_OscGetAmplitudeOffset(RPAPP_OSC_SOUR_CH2, &value);
        in1Offset.Value() = value;
        rpApp_OscGetTimeOffset(&value);
        inTimeOffset.Value() = value;
        rpApp_OscGetTimeScale(&value);
        inTimeScale.Value() = value;
        inAutoscale.Update();
        inAutoscale.Value() = false;

        rpApp_osc_trig_sweep_t sweep;
        rpApp_OscGetTriggerSweep(&sweep);
        inTrigSweep.Value() = sweep;
    }

    IF_VALUE_CHANGED(in1Offset,    rpApp_OscSetAmplitudeOffset(RPAPP_OSC_SOUR_CH1,  in1Offset.NewValue()))
    IF_VALUE_CHANGED(in2Offset,    rpApp_OscSetAmplitudeOffset(RPAPP_OSC_SOUR_CH2,  in2Offset.NewValue()))
    IF_VALUE_CHANGED(inMathOffset, rpApp_OscSetAmplitudeOffset(RPAPP_OSC_SOUR_MATH, inMathOffset.NewValue()))
    IF_VALUE_CHANGED(in1Scale,    rpApp_OscSetAmplitudeScale(RPAPP_OSC_SOUR_CH1,  in1Scale.NewValue()))
    IF_VALUE_CHANGED(in2Scale,    rpApp_OscSetAmplitudeScale(RPAPP_OSC_SOUR_CH2,  in2Scale.NewValue()))
    IF_VALUE_CHANGED(inMathScale, rpApp_OscSetAmplitudeScale(RPAPP_OSC_SOUR_MATH, inMathScale.NewValue()))
    IF_VALUE_CHANGED(in1Probe, rpApp_OscSetProbeAtt(RP_CH_1, in1Probe.NewValue()))
    IF_VALUE_CHANGED(in2Probe, rpApp_OscSetProbeAtt(RP_CH_2, in2Probe.NewValue()))
    IF_VALUE_CHANGED(in1Gain, rpApp_OscSetInputGain(RP_CH_1, (rpApp_osc_in_gain_t)in1Gain.NewValue()))
    IF_VALUE_CHANGED(in2Gain, rpApp_OscSetInputGain(RP_CH_2, (rpApp_osc_in_gain_t)in2Gain.NewValue()))
    IF_VALUE_CHANGED(inTimeOffset, rpApp_OscSetTimeOffset(inTimeOffset.NewValue()))
    IF_VALUE_CHANGED(inTimeScale, rpApp_OscSetTimeScale(inTimeScale.NewValue()))
    IF_VALUE_CHANGED(inTrigSweep, rpApp_OscSetTriggerSweep((rpApp_osc_trig_sweep_t) inTrigSweep.NewValue()))
    IF_VALUE_CHANGED(inTrigSource, rpApp_OscSetTriggerSource((rpApp_osc_trig_source_t)inTrigSource.NewValue()))
    IF_VALUE_CHANGED(inTrigSlope, rpApp_OscSetTriggerSlope((rpApp_osc_trig_slope_t) inTrigSlope.NewValue()))
    IF_VALUE_CHANGED(inTriggLevel, rpApp_OscSetTriggerLevel(inTriggLevel.NewValue()))
    IF_VALUE_CHANGED(in1InvShow, rpApp_OscSetInverted(RPAPP_OSC_SOUR_CH1, in1InvShow.NewValue()))
    IF_VALUE_CHANGED(in2InvShow, rpApp_OscSetInverted(RPAPP_OSC_SOUR_CH2, in2InvShow.NewValue()))
    IF_VALUE_CHANGED(mathInvShow, rpApp_OscSetInverted(RPAPP_OSC_SOUR_MATH, mathInvShow.NewValue()))

    IF_VALUE_CHANGED(mathOperation, rpApp_OscSetMathOperation((rpApp_osc_math_oper_t) mathOperation.NewValue()))
    if (mathSource1.Value() != mathSource1.NewValue() || mathSource2.Value() != mathSource2.NewValue()) {
        if (rpApp_OscSetMathSources((rp_channel_t) mathSource1.NewValue(), (rp_channel_t) mathSource2.NewValue())) {
            mathSource1.Update();
            mathSource2.Update();
        }
    }

/* ------ UPDATE GENERATE LOCAL PARAMETERS ------*/
    out1Show.Update();
    out2Show.Update();
    out1ShowOffset.Update();
    out2ShowOffset.Update();

/* ------ HANDLE CALIBRATE ------*/
    if (calibrateBackEndOffset.NewValue() == 1) {
        if (rp_CalibrateBackEndOffset(RP_CH_1) && rp_CalibrateBackEndOffset(RP_CH_2)) {
            calibrateBackEndOffset.Value() = -1;
        }
    }
    if (calibrateBackEndScale.NewValue() == 1) {
        if (rp_CalibrateBackEndScale(RP_CH_1) && rp_CalibrateBackEndOffset(RP_CH_2)) {
            calibrateBackEndOffset.Value() = -1;
        }
    }
    if (calibrateFrontEndOffset.NewValue() == 1) {
        if (rp_CalibrateFrontEndOffset(RP_CH_1) && rp_CalibrateFrontEndOffset(RP_CH_2)) {
            calibrateBackEndOffset.Value() = -1;
        }
    }
    if (calibrateFrontEndScaleHV.NewValue() == 1) {
        if (rp_CalibrateFrontEndScaleHV(RP_CH_1, CALIB_FE_HV_REF_V) && rp_CalibrateFrontEndScaleHV(RP_CH_2, CALIB_FE_HV_REF_V)) {
            calibrateBackEndOffset.Value() = -1;
        }
    }
    if (calibrateFrontEndScaleLV.NewValue() == 1) {
        if (rp_CalibrateFrontEndScaleLV(RP_CH_1, CALIB_FE_LV_REF_V) && rp_CalibrateFrontEndScaleLV(RP_CH_2, CALIB_FE_LV_REF_V)) {
            calibrateBackEndOffset.Value() = -1;
        }
    }

/* ------ UPDATE DEBUG PARAMETERS ------*/
    signalPeriiod.Update();
    parameterPeriiod.Update();
    digitalLoop.Update();
}





/***************************************************************************************
*                            SIGNAL GENERATING TEMPORARY                                *
****************************************************************************************/

void generate(rp_channel_t channel) {
    CFloatSignal *signal;
    rp_waveform_t waveform;
    float frequency, phase, amplitude, offset, showOff, duty_cycle;

    if (channel == RP_CH_1) {
        signal = &out1Signal;
        waveform = (rp_waveform_t) out1WAveform.Value();
        frequency = out1Frequancy.Value();
        phase = (float) (out1Phase.Value() / 180.0f * M_PI);
        amplitude = out1Amplitude.Value()/out1Scale.Value();
        offset = out1Offset.Value();
        showOff = out1ShowOffset.Value();
		duty_cycle = out1DCYC.Value();
    }
    else {
        signal = &out2Signal;
        waveform = (rp_waveform_t) out2WAveform.Value();
        frequency = out2Frequancy.Value();
        phase = (float) (out2Phase.Value() / 180.0f * M_PI);
        amplitude = out2Amplitude.Value()/out2Scale.Value();
        offset = out2Offset.Value();
        showOff = out2ShowOffset.Value();
		duty_cycle = out2DCYC.Value();
    }

    switch (waveform) {
        case RP_WAVEFORM_SINE:
            synthesis_sin(signal, frequency, phase, amplitude, offset, showOff);
            break;
        case RP_WAVEFORM_TRIANGLE:
            synthesis_triangle(signal, frequency, phase, amplitude, offset, showOff);
            break;
        case RP_WAVEFORM_SQUARE:
            synthesis_square(signal, frequency, phase, amplitude, offset, showOff);
            break;

        case RP_WAVEFORM_RAMP_UP:
            synthesis_rampUp(signal, frequency, phase, amplitude, offset, showOff);
            break;
        case RP_WAVEFORM_RAMP_DOWN:
            synthesis_rampDown(signal, frequency, phase, amplitude, offset, showOff);
            break;
        case RP_WAVEFORM_DC:
            synthesis_DC(signal, frequency, phase, amplitude, offset, showOff);
            break;
        case RP_WAVEFORM_PWM:
            synthesis_PWM(signal, frequency, phase, amplitude, offset, showOff, duty_cycle);
            break;
        default:
            break;
    }
}


void synthesis_sin(CFloatSignal *signal, float freq, float phase, float amp, float off, float showOff) {
    for(int i = 0; i < (*signal).GetSize(); i++) {
        (*signal)[i] = (float) (sin(2 * M_PI * (float) i / (float) (*signal).GetSize() * (freq * inTimeScale.Value()/1000) * 10 + phase) * amp + off + showOff);
    }
}

void synthesis_triangle(CFloatSignal *signal, float freq, float phase, float amp, float off, float showOff) {
    for(int i = 0; i < (*signal).GetSize(); i++) {
        (*signal)[i] = (float) ((asin(sin(2 * M_PI * (float) i / (float) (*signal).GetSize() * (freq * inTimeScale.Value()/1000) * 10 + phase))) / M_PI * 2  * amp + off + showOff);
    }
}

void synthesis_square(CFloatSignal *signal, float freq, float phase, float amp, float off, float showOff) {
    for(int i = 0; i < (*signal).GetSize(); i++) {
        (*signal)[i] = sin(2 * M_PI * (float) i / (float) (*signal).GetSize() * (freq * inTimeScale.Value()/1000) * 10 + phase) > 0 ? amp+off + showOff : -amp+off + showOff;
    }
}

int synthesis_rampUp(CFloatSignal *signal, float freq, float phase, float amp, float off, float showOff) {
    float shift = 0;
    for(int unsigned i = 0; i < signal->GetSize() - 1; i++) {
        float angle = M_PI * (float) i / (float) (*signal).GetSize() * (freq * inTimeScale.Value()/1000) * 10 + phase - shift;
        if(angle > M_PI) {
            angle -= M_PI;
            shift += M_PI;
        }
        (*signal)[signal->GetSize() - i-2] = (float) (-1.0 * (acos(cos(angle)) / M_PI - 1));
    }
    return RP_OK;
}

int synthesis_rampDown(CFloatSignal *signal, float freq, float phase, float amp, float off, float showOff) {
    float shift = 0;
    for(int unsigned i = 0; i < signal->GetSize() - 1; i++) {
        float angle = M_PI * (float) i / (float) (*signal).GetSize() * (freq * inTimeScale.Value()/1000) * 10 + phase - shift;
        if(angle > M_PI) {
            angle -= M_PI;
            shift += M_PI;
        }
        (*signal)[i] = (float) (-1.0 * (acos(cos(angle)) / M_PI - 1));
    }
    return RP_OK;
}

int synthesis_DC(CFloatSignal *signal, float freq, float phase, float amp, float off, float showOff) {
    for(int i = 0; i < signal->GetSize(); i++) {
        (*signal)[i] = 1.0*amp + off + showOff;
    }
    return RP_OK;
}

int synthesis_PWM(CFloatSignal *signal, float freq, float phase, float amp, float off, float showOff, float ratio /*duty cycle*/) {
    float period = (float) (*signal).GetSize() / (freq * inTimeScale.Value() * 10.f / 1000.f);
    float duty = period * ratio;
    float fphase = period * phase / (2.f * M_PI);

    float shift = 0;
    for(int i = 0; i < (*signal).GetSize(); i++) {
        float value = (float)i + fphase - shift;
        if(value > period) {
            value -= period;
            shift += period;
        }
        (*signal)[i] = (value > duty) ? (-amp + off + showOff) : (amp + off + showOff);
    }
    return RP_OK;
}

void OnNewSignals(void)
{
	// do something
	//CDataManager::GetInstance()->UpdateAllSignals();
}
