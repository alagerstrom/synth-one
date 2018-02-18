//
// Created by Andreas Lagerstrom on 2018-02-17.
//

#ifndef SYNTHONE_LFOMODULE_H
#define SYNTHONE_LFOMODULE_H


#include "AbstractModule.h"
#include "Oscillator.h"
#include "IPlug_include_in_plug_hdr.h"

class LFOModule : public AbstractModule {

private:
    IPlug * iPlug;
    int x;
    int y;
    int spacing;
    int frequencyParam;
    int modeParam;
    int amountParam;
    Oscillator oscillator;
    double amount = 0;

public:
    LFOModule(IPlug *iPlug, int x, int y, int spacing, int frequencyParam, int modeParam, int amountParam);

    void draw(IGraphics *iGraphics, IBitmap *knob) override;

    void handleParamChange(int paramIndex) override;

    void advance() override;

    double getSample() override;

    void initializeParameters(
            const char* frequencyName,
            const char* modeName,
            const char* amountName
    );

};


#endif //SYNTHONE_LFOMODULE_H
