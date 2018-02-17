//
// Created by Andreas Lagerstrom on 2018-02-05.
//

#include "OscillatorController.h"

double OscillatorController::nextSample() {
    return oscillator.nextSample();
}

void OscillatorController::setSampleRate(double sampleRate) {
    oscillator.setSampleRate(sampleRate);
}

void OscillatorController::setMode(OscillatorMode mode) {
    oscillator.setMode(mode);
}

void OscillatorController::setOctave(int octave) {
    this->octave = octave;
    setPlayedNote(this->playedNote);
}


double OscillatorController::noteNumberToFrequency(int noteNumber) {
    return 440.0 * pow(2.0, (noteNumber - 69.0) / 12.0);
}

void OscillatorController::setPlayedNote(int noteNumber) {
    if (noteNumber != -1){
        this->playedNote = noteNumber;
        int noteNumberWithOctaveAndSemitone = noteNumber + 12 * octave + semitone;
        oscillator.setFrequency(tune * noteNumberToFrequency(noteNumberWithOctaveAndSemitone));
    }
}

void OscillatorController::setSemitone(int semitone) {
    this->semitone = semitone;
    setPlayedNote(this->playedNote);
}

void OscillatorController::setTune(double tune) {
    this->tune = tune;
    setPlayedNote(playedNote);
}

void OscillatorController::handleParamChange(int paramIndex) {
    if (paramIndex == freqParam){
        setTune(iPlug->GetParam(freqParam)->Value());
    }else if (paramIndex == waveParam){
        setMode(static_cast<OscillatorMode>(static_cast<int>(iPlug->GetParam(waveParam)->Value())));
    }else if (paramIndex == semiParam){
        int semitone = static_cast<int>(iPlug->GetParam(semiParam)->Value());
        setSemitone(semitone);
    }else if (paramIndex == octParam){
        int octave = static_cast<int>(iPlug->GetParam(octParam)->Value());
        setOctave(octave);
    }

}
