//
// Created by Andreas Lagerstrom on 2018-02-13.
//

#ifndef SYNTHONE_POLYBLEPOSCILLATOR_H
#define SYNTHONE_POLYBLEPOSCILLATOR_H


#include "Oscillator.h"

class PolyBLEPOscillator : public Oscillator{
public:
    PolyBLEPOscillator() : lastOutput(0.0){
        updateIncrement();
    };
    double nextSample();

private:
    double polyBlep(double t);
    double lastOutput;
};


#endif //SYNTHONE_POLYBLEPOSCILLATOR_H
