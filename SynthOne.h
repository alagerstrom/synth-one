#ifndef __SYNTHONE__
#define __SYNTHONE__

#include "IPlug_include_in_plug_hdr.h"
#include "OscillatorModule.h"
#include "OscillatorController.h"
#include "Filter.h"
#include "EnvelopeGenerator.h"
#include "MidiReceiver.h"
#include "EnvelopeModule.h"
#include "FilterModule.h"
#include "LFOModule.h"

class SynthOne : public IPlug {
public:
    SynthOne(IPlugInstanceInfo instanceInfo);

    ~SynthOne();

    void Reset();

    void OnParamChange(int paramIdx);

    void ProcessDoubleReplacing(double **inputs, double **outputs, int nFrames);

    void ProcessMidiMsg(IMidiMsg *pMsg);



private:
    enum ParameterEnum {
        osc1FreqParam = 0,
        osc1WaveParam = 1,
        osc1OctaveParam,
        osc1SemiParam,
        osc2FreqParam,
        osc2WaveParam,
        osc2OctaveParam,
        osc2SemiParam,
        osc3FreqParam,
        osc3WaveParam,
        osc3OctaveParam,
        osc3SemiParam,
        attackParameter,
        decayParameter,
        sustainParameter,
        releaseParameter,
        ampEnvelopeAmountParameter,
        filterModeParameter,
        filterCutoffParameter,
        filterResonanceParameter,
        filterAttackParameter,
        filterDecayParameter,
        filterSustainParameter,
        filterReleaseParameter,
        filterEnvelopeAmountParameter,
        lfoFreqParam,
        lfoModeParam,
        lfoAmountParam,
        numberOfParameters
    };
    MidiReceiver midiReceiver;

    void onNoteOn(int noteNumber, int velocity) {
        ampEnvelopeModule.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
        filterEnvelopeModule.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
    };

    void onNoteOff(int noteNumber, int velocity) {
        ampEnvelopeModule.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
        filterEnvelopeModule.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
    };
    OscillatorModule oscillator1;
    OscillatorModule oscillator2;
    OscillatorModule oscillator3;
    EnvelopeModule ampEnvelopeModule;
    EnvelopeModule filterEnvelopeModule;
    FilterModule filterModule;
    LFOModule lfoModule;
    void createKnob(int x, int y, IGraphics *pGraphics, int param);
};

#endif
