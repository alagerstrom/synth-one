//
// Created by Andreas Lagerstrom on 2018-02-05.
//

#ifndef SYNTHONE_OSCILLATORCONTROLLER_H
#define SYNTHONE_OSCILLATORCONTROLLER_H


#include "PolyBLEPOscillator.h"
#include "IPlug_include_in_plug_hdr.h"

class OscillatorController {

private:
    PolyBLEPOscillator oscillator;
    IPlug *iPlug;
    int octave;
    int semitone;
    int playedNote;
    double tune;
    const int freqParam;
    const int waveParam;
    const int octParam;
    const int semiParam;

    double noteNumberToFrequency(int noteNumber);

public:
    OscillatorController(IPlug *iPlug, int freqParam, int waveParam, int octParam, int semiParam) :
            iPlug(iPlug),
            octave(3),
            semitone(0),
            playedNote(-1),
            tune(0),
            freqParam(freqParam),
            waveParam(waveParam),
            octParam(octParam),
            semiParam(semiParam) {

    };

    void setPlayedNote(int noteNumber);

    double nextSample();

    void setSampleRate(double sampleRate);

    void setMode(OscillatorMode mode);

    void setOctave(int octave);

    void setSemitone(int semitone);

    void setTune(double tune);

    void handleParamChange(int paramNumber);
};


#endif //SYNTHONE_OSCILLATORCONTROLLER_H
