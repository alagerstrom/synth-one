#include "SynthOne.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"
#include "Filter.h"

const int numberOfPresets = 1;

SynthOne::SynthOne(IPlugInstanceInfo instanceInfo)
        : IPLUG_CTOR(numberOfParameters, numberOfPresets, instanceInfo),
          oscillator1(this, 100, 100, 100, osc1WaveParam, osc1FreqParam, osc1OctaveParam, osc1SemiParam),
          oscillator2(this, 100, 200, 100, osc2WaveParam, osc2FreqParam, osc2OctaveParam, osc2SemiParam),
          oscillator3(this, 100, 300, 100, osc3WaveParam, osc3FreqParam, osc3OctaveParam, osc3SemiParam),
          ampEnvelopeModule(this, 100, 400, 100, attackParameter, decayParameter, sustainParameter, releaseParameter),
          filterEnvelopeModule(this, 100, 600, 100, filterAttackParameter, filterDecayParameter, filterSustainParameter, filterReleaseParameter) {
    TRACE;

    //arguments are: name, defaultVal, minVal, maxVal, step, label



    GetParam(filterModeParameter)->InitEnum("Filter mode", Filter::FILTER_MODE_LOWPASS, Filter::numberOfFilterModes);
    GetParam(filterCutoffParameter)->InitDouble("Cutoff", 0.99, 0.01, 0.99, 0.001);
    GetParam(filterCutoffParameter)->SetShape(2);
    GetParam(filterResonanceParameter)->InitDouble("Resonance", 0.01, 0.01, 1.0, 0.001);
    GetParam(filterAttackParameter)->InitDouble("Filter Env Attack", 0.01, 0.01, 10.0, 0.001);
    GetParam(filterAttackParameter)->SetShape(3);
    GetParam(filterDecayParameter)->InitDouble("Filter Env Decay", 0.5, 0.01, 15.0, 0.001);
    GetParam(filterDecayParameter)->SetShape(3);
    GetParam(filterSustainParameter)->InitDouble("Filter Env Sustain", 0.1, 0.001, 1.0, 0.001);
    GetParam(filterSustainParameter)->SetShape(2);
    GetParam(filterReleaseParameter)->InitDouble("Filter Env Release", 1.0, 0.001, 15.0, 0.001);
    GetParam(filterReleaseParameter)->SetShape(3);
    GetParam(filterEnvelopeAmountParameter)->InitDouble("Filter Env Amount", 0.0, -1.0, 1.0, 0.001);


    IGraphics *pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT);
    pGraphics->AttachBackground(PANEL_ID, PANEL_FN);
    IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 128);

    oscillator1.initializeParameters("Osc 1 Frequency", "Osc 1 Waveform", "Osc 1 Octave", "Osc 1 Semitone");
    oscillator1.draw(pGraphics, &knob);

    oscillator2.initializeParameters("Osc 2 Frequency", "Osc 2 Waveform", "Osc 2 Octave", "Osc 2 Semitone");
    oscillator2.draw(pGraphics, &knob);

    oscillator3.initializeParameters("Osc 3 Frequency", "Osc 3 Waveform", "Osc 3 Octave", "Osc 3 Semitone");
    oscillator3.draw(pGraphics, &knob);

    ampEnvelopeModule.initializeParameters("Amp Attack", "Amp Decay", "Amp Sustain", "Amp Release");
    ampEnvelopeModule.draw(pGraphics, &knob);

    filterEnvelopeModule.initializeParameters("Filter Attack", "Filter Decay", "Filter Sustain", "Filter Release");
    filterEnvelopeModule.draw(pGraphics, &knob);


    int filterY = 500;
    int filterDistance = 100;
    int firstFilterX = 100;

    createKnob(firstFilterX, filterY, pGraphics, filterModeParameter);
    createKnob(firstFilterX + filterDistance, filterY, pGraphics, filterCutoffParameter);
    createKnob(firstFilterX + 2 * filterDistance, filterY, pGraphics, filterResonanceParameter);

    createKnob(500, 600, pGraphics, filterEnvelopeAmountParameter);


    AttachGraphics(pGraphics);

    midiReceiver.noteOn.Connect(this, &SynthOne::onNoteOn);
    midiReceiver.noteOff.Connect(this, &SynthOne::onNoteOff);

    MakeDefaultPreset("clean", numberOfParameters);
}

SynthOne::~SynthOne() {
}

void SynthOne::ProcessDoubleReplacing(double **inputs, double **outputs, int nFrames) {
    double *leftOutput = outputs[0];
    double *rightOutput = outputs[1];

    for (int i = 0; i < nFrames; ++i) {
        midiReceiver.advance();
        oscillator1.setPlayedNote(midiReceiver.getLastNoteNumber());
        oscillator2.setPlayedNote(midiReceiver.getLastNoteNumber());
        oscillator3.setPlayedNote(midiReceiver.getLastNoteNumber());
        filter.setModulation(filterEnvelopeModule.nextSample() * filterEnvelopeAmount);
        leftOutput[i] = rightOutput[i] = filter.process((oscillator1.nextSample() + oscillator2.nextSample() + oscillator3.nextSample()) / 3 * ampEnvelopeModule.nextSample());
    }
    midiReceiver.flush(nFrames);
}

void SynthOne::Reset() {
    TRACE;
    IMutexLock lock(this);
    double newSampleRate = this->GetSampleRate();
    oscillator1.setSampleRate(newSampleRate);
    oscillator2.setSampleRate(newSampleRate);
    oscillator3.setSampleRate(newSampleRate);
    ampEnvelopeModule.setSampleRate(newSampleRate);
    filterEnvelopeModule.setSampleRate(this->GetSampleRate());
}

void SynthOne::OnParamChange(int paramIdx) {
    IMutexLock lock(this);
    oscillator1.handleParamChange(paramIdx);
    oscillator2.handleParamChange(paramIdx);
    oscillator3.handleParamChange(paramIdx);
    ampEnvelopeModule.handleParamChange(paramIdx);
    filterEnvelopeModule.handleParamChange(paramIdx);
    switch (paramIdx) {
        case filterEnvelopeAmountParameter:
            this->filterEnvelopeAmount = GetParam(filterEnvelopeAmountParameter)->Value();
            break;
        case filterCutoffParameter:
            filter.setCutoff(GetParam(paramIdx)->Value());
            break;
        case filterResonanceParameter:
            filter.setResonance(GetParam(paramIdx)->Value());
            break;
        case filterModeParameter:
            filter.setFilterMode(static_cast<Filter::FilterMode>(GetParam(paramIdx)->Int()));
            break;
        default:
            break;
    }
}

void SynthOne::ProcessMidiMsg(IMidiMsg *pMsg) {
    midiReceiver.onMessageReceived(pMsg);
}

void SynthOne::createKnob(int x, int y, IGraphics *pGraphics, int param) {
    IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 128);
    pGraphics->AttachControl(new IKnobMultiControl(this, x, y, param, &knob));
}
