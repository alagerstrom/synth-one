//
// Created by Andreas Lagerstrom on 2018-02-17.
//

#ifndef SYNTHONE_ENVELOPEMODULE_H
#define SYNTHONE_ENVELOPEMODULE_H


#include "AbstractModule.h"
#include "IPlug_include_in_plug_hdr.h"
#include "EnvelopeGenerator.h"

class EnvelopeModule : public AbstractModule {
private:
    IPlug *iPlug;
    int x;
    int y;
    int spacing;
    int attackParameter;
    int decayParameter;
    int sustainParameter;
    int releaseParameter;
    int amountParameter;
    EnvelopeGenerator envelopeGenerator;
    double amountValue = 0;


public:
    EnvelopeModule(
            IPlug *iPlug,
            int x,
            int y,
            int spacing,
            int attackParameter,
            int decayParameter,
            int sustainParameter,
            int releaseParameter,
            int amountParameter
    );

public:
    void draw(IGraphics *iGraphics, IBitmap *knob) override;

    void handleParamChange(int paramIndex) override;

    void initializeParameters(const char *attackName, const char *decayName, const char *sustainName, const char *releaseName, const char* amountName);

    void enterStage(EnvelopeGenerator::EnvelopeStage stage);

    void setSampleRate(double rate);

    void advance() override;

    double getSample() override;
};


#endif //SYNTHONE_ENVELOPEMODULE_H
