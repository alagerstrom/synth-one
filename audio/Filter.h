//
// Created by Andreas Lagerstrom on 2018-02-04.
//

#ifndef SYNTHONE_FILTER_H
#define SYNTHONE_FILTER_H


#include <cmath>

class Filter {
public:
    enum FilterMode {
        FILTER_MODE_LOWPASS = 0,
        FILTER_MODE_HIGHPASS,
        FILTER_MODE_BANDPASS,
        numberOfFilterModes
    };

    Filter() :
            cutoff(0.99),
            resonance(0.0),
            mode(FILTER_MODE_LOWPASS),
            buf0(0.0),
            buf1(0.0),
            buf2(0.0),
            buf3(0.0),
            modulation(0.0) {
        calculateFeedbackAmount();
    };

    double process(double inputValue);

    inline void setCutoff(double newCutoff) {
        cutoff = newCutoff;
        calculateFeedbackAmount();
    };

    inline void setResonance(double newResonance) {
        resonance = newResonance;
        calculateFeedbackAmount();
    };

    inline void setFilterMode(FilterMode newMode) {
        mode = newMode;
    }

    inline void setModulation(double newCutoffMod) {
        modulation = newCutoffMod;
        calculateFeedbackAmount();
    }

private:
    double modulation;
    double cutoff;
    double resonance;
    FilterMode mode;
    double feedbackAmount;
    double buf0;
    double buf1;
    double buf2;
    double buf3;

    inline double getCalculatedCutoff() const {
        return fmax(fmin(cutoff + modulation, 0.99), 0.01);
    };

    inline void calculateFeedbackAmount() {
        feedbackAmount = resonance + resonance / (1.0 - getCalculatedCutoff());
    }

};


#endif //SYNTHONE_FILTER_H
