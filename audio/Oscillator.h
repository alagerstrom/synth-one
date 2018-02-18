//
// Created by Andreas Lagerstrom on 2018-01-28.
//

#ifndef SYNTHONE_OSCILLATOR_H
#define SYNTHONE_OSCILLATOR_H

#include <cmath>

enum OscillatorMode {
    SINE = 0,
    SAW = 1,
    SQUARE = 2,
    TRIANGLE = 3
};

class Oscillator {
protected:
    OscillatorMode oscillatorMode;
    const double pi;
    const double twoPi;
    double frequency;
    double phase;
    double sampleRate;
    double phaseIncrement;

    void incrementPhase();

    void updateIncrement();


public:
    void setMode(OscillatorMode mode);

    void setFrequency(double frequency);

    void setSampleRate(double sampleRate);

    Oscillator() :
            oscillatorMode(SINE),
            pi(2 * acos(0)),
            twoPi(2 * pi),
            frequency(440.0),
            phase(0.0),
            sampleRate(44100.0) {
        updateIncrement();
    }

    virtual double getSample();
    virtual void advance();

    double naiveWaveformForMode(OscillatorMode mode);

};


#endif //SYNTHONE_OSCILLATOR_H
