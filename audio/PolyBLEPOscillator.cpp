//
// Created by Andreas Lagerstrom on 2018-02-13.
//

#include "PolyBLEPOscillator.h"

// PolyBLEP by Tale
// (slightly modified)
// http://www.kvraudio.com/forum/viewtopic.php?t=375517
double PolyBLEPOscillator::polyBlep(double t) {
    double dt = phaseIncrement / twoPi;
    // 0 <= t < 1
    if (t < dt) {
        t /= dt;
        return t + t - t * t - 1.0;
    }
        // -1 < t < 0
    else if (t > 1.0 - dt) {
        t = (t - 1.0) / dt;
        return t * t + t + t + 1.0;
    }
        // 0 otherwise
    else return 0.0;
}

double PolyBLEPOscillator::nextSample() {
    double value = 0.0;
    double t = phase / twoPi;

    if (oscillatorMode == SINE) {
        value = naiveWaveformForMode(SINE);
    } else if (oscillatorMode == SAW) {
        value = naiveWaveformForMode(SAW);
        value -= polyBlep(t);
    }
    else {
        value = naiveWaveformForMode(SQUARE);
        value += polyBlep(t);
        value -= polyBlep(fmod(t + 0.5, 1.0));
        if (oscillatorMode == TRIANGLE) {
            // Leaky integrator: y[n] = A * x[n] + (1 - A) * y[n-1]
            value = phaseIncrement * value + (1 - phaseIncrement) * lastOutput;
            lastOutput = value;
        }
    }
    incrementPhase();
    return value;
}
