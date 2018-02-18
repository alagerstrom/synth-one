//
// Created by Andreas Lagerstrom on 2018-02-13.
//

#ifndef SYNTHONE_OSCILLATORVIEW_H
#define SYNTHONE_OSCILLATORVIEW_H


#include "IPlug_include_in_plug_hdr.h"
#include "OscillatorController.h"
#include "AbstractModule.h"


class OscillatorModule : public AbstractModule {
private:
    IPlug * iPlug;
    int x;
    int y;
    int spacing;
    int modeParameter;
    int tuneParameter;
    int octaveParameter;
    int semitoneParameter;
    OscillatorController oscillatorController;

public:


    void draw(IGraphics *iGraphics, IBitmap *knob) override;

    void handleParamChange(int paramIndex) override;

    OscillatorModule(IPlug *iPlug, int x, int y, int spacing, int modeParameter, int tuneParameter, int octaveParameter, int semitoneParameter);

    void initializeParameters(const char *frequencyParameterName, const char *waveformParameterName, const char *octaveParameterName, const char *semitoneParameterName);

    void setPlayedNote(int noteNumber);

    void setSampleRate(double rate);

    void advance() override;

    double getSample() override;
};


#endif //SYNTHONE_OSCILLATORVIEW_H
