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
          ampEnvelopeModule(this, 100, 400, 100, attackParameter, decayParameter, sustainParameter, releaseParameter, ampEnvelopeAmountParameter),
          filterEnvelopeModule(this, 100, 500, 100, filterAttackParameter, filterDecayParameter, filterSustainParameter, filterReleaseParameter, filterEnvelopeAmountParameter),
          filterModule(this, 100, 600, 100, filterCutoffParameter, filterResonanceParameter, filterModeParameter),
          lfo1(this, 100, 700, 100, lfoFreqParam, lfoModeParam, lfoAmountParam),
          lfo2(this, 400, 700, 100, secondLfoFreqParam, secondLfoModeParam, secondLfoAmountParam)
{
    TRACE;
    IGraphics *pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT);
//    pGraphics->AttachBackground(PANEL_ID, PANEL_FN);
    IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 128);

    GetParam(portamentoParam)->InitDouble("Portamento", 0.01, 0.000001, 0.01, 0.00001, "%");
    GetParam(portamentoParam)->SetShape(3.);
    pGraphics->AttachControl(new IKnobMultiControl(this, 600, 100, portamentoParam, &knob));


    oscillator1.initializeParameters("Osc 1 Frequency", "Osc 1 Waveform", "Osc 1 Octave", "Osc 1 Semitone");
    oscillator1.draw(pGraphics, &knob);

    oscillator2.initializeParameters("Osc 2 Frequency", "Osc 2 Waveform", "Osc 2 Octave", "Osc 2 Semitone");
    oscillator2.draw(pGraphics, &knob);

    oscillator3.initializeParameters("Osc 3 Frequency", "Osc 3 Waveform", "Osc 3 Octave", "Osc 3 Semitone");
    oscillator3.draw(pGraphics, &knob);

    ampEnvelopeModule.initializeParameters("Amp Attack", "Amp Decay", "Amp Sustain", "Amp Release", "Amp Envelope Amount");
    ampEnvelopeModule.draw(pGraphics, &knob);

    filterEnvelopeModule.initializeParameters("Filter Attack", "Filter Decay", "Filter Sustain", "Filter Release", "Filter Envelope Amount");
    filterEnvelopeModule.draw(pGraphics, &knob);

    lfo1.initializeParameters("LFO 1 Frequency", "LFO 1 Mode", "LFO 1 Level");
    lfo1.draw(pGraphics, &knob);

    lfo2.initializeParameters("LFO 2 Frequency", "LFO 2 Mode", "LFO 2 Level");
    lfo2.draw(pGraphics, &knob);

    filterModule.initializeParameters("Cutoff", "Resonance", "Filter Mode");
    filterModule.draw(pGraphics, &knob);
    filterModule.addInputModule(&oscillator1);
    filterModule.addInputModule(&oscillator2);
    filterModule.addInputModule(&oscillator3);
    filterModule.addModulator(&lfo1);
    filterModule.addModulator(&lfo2);
    filterModule.addModulator(&filterEnvelopeModule);

    midiReceiver.noteOn.Connect(this, &SynthOne::onNoteOn);
    midiReceiver.noteOff.Connect(this, &SynthOne::onNoteOff);

    AttachGraphics(pGraphics);
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
        oscillator1.advance();
        oscillator2.advance();
        oscillator3.advance();
        oscillator1.setPortamento(portamento);
        oscillator2.setPortamento(portamento);
        oscillator3.setPortamento(portamento);
        filterEnvelopeModule.advance();
        ampEnvelopeModule.advance();
        lfo1.advance();
        lfo2.advance();
        leftOutput[i] = rightOutput[i] = filterModule.getSample() * ampEnvelopeModule.getSample();
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
    filterModule.handleParamChange(paramIdx);
    lfo1.handleParamChange(paramIdx);
    lfo2.handleParamChange(paramIdx);
    if (paramIdx == portamentoParam){
        portamento = GetParam(portamentoParam)->Value();
    }

}

void SynthOne::ProcessMidiMsg(IMidiMsg *pMsg) {
    midiReceiver.onMessageReceived(pMsg);
}

