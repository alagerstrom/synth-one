//
// Created by Andreas Lagerstrom on 2018-02-17.
//

#ifndef SYNTHONE_ENVELOPEMODULE_H
#define SYNTHONE_ENVELOPEMODULE_H


#include "AbstractModule.h"
#include "IPlug_include_in_plug_hdr.h"
#include "EnvelopeGenerator.h"

class EnvelopeModule : public AbstractModule{
private:
    IPlug * iPlug;
    int x;
    int y;
    int spacing;
    int attackParameter;
    int decayParameter;
    int sustainParameter;
    int releaseParameter;
    EnvelopeGenerator envelopeGenerator;


public:
    EnvelopeModule(IPlug *iPlug, int x, int y, int spacing, int attackParameter, int decayParameter, int sustainParameter, int releaseParameter);

public:
    void draw(IGraphics *iGraphics, IBitmap *knob) override;

    void handleParamChange(int paramIndex) override;

    void initializeParameters(const char * attackName, const char * decayName, const char * sustainName, const char * releaseName);

    void enterStage(EnvelopeGenerator::EnvelopeStage stage);

    double nextSample();

    void setSampleRate(double rate);
};


#endif //SYNTHONE_ENVELOPEMODULE_H
