//
// Created by Andreas Lagerstrom on 2018-01-28.
//

#include "Oscillator.h"

void Oscillator::updateIncrement() {
    this->phaseIncrement = frequency * 2 * pi / sampleRate;
}

void Oscillator::setMode(OscillatorMode mode) {
    this->oscillatorMode = mode;
}

void Oscillator::setFrequency(double frequency) {
    this->frequency = frequency;
    updateIncrement();
}

void Oscillator::setSampleRate(double sampleRate) {
    this->sampleRate = sampleRate;
}

void Oscillator::incrementPhase() {
    phase += phaseIncrement;
    while (phase > twoPi) {
        phase -= twoPi;
    }
}

double Oscillator::naiveWaveformForMode(OscillatorMode mode) {
    double value = 0.0;
    switch (mode) {
        case SINE:
            value = sin(phase);
            break;
        case SAW:
            value = (2.0 * phase / twoPi) - 1.0;
            break;
        case SQUARE:
            if (phase < pi) {
                value = 1.0;
            } else {
                value = -1.0;
            }
            break;
        case TRIANGLE:
            value = -1.0 + (2.0 * phase / twoPi);
            value = 2.0 * (fabs(value) - 0.5);
            break;
        default:
            break;
    }
    return value;
}

double Oscillator::getSample() {
    return naiveWaveformForMode(oscillatorMode);
}

void Oscillator::advance() {
    incrementPhase();
}
