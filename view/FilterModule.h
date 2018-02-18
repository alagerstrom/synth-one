//
// Created by Andreas Lagerstrom on 2018-02-17.
//

#ifndef SYNTHONE_FILTERMODULE_H
#define SYNTHONE_FILTERMODULE_H


#include <vector>
#include "AbstractModule.h"
#include "IPlug_include_in_plug_hdr.h"
#include "Filter.h"

class FilterModule : public AbstractModule {
private:
    IPlug * iPlug;
    int x;
    int y;
    int spacing;
    int cutoffParam;
    int resonanceParam;
    int modeParam;
    Filter filter;
    std::vector<AbstractModule *> modulators;
    std::vector<AbstractModule *> inputModules;

    double sumModulation();

public:

    FilterModule(IPlug *iPlug, int x, int y, int spacing, int cutoffParam, int resonanceParam, int modeParam);

    void addInputModule(AbstractModule * module);

    void initializeParameters(
            const char* cutoffName,
            const char* resonanceName,
            const char* modeName
    );

    void draw(IGraphics *iGraphics, IBitmap *knob) override;

    void handleParamChange(int paramIndex) override;

    void advance() override;

    double getSample() override;

    void addModulator(AbstractModule *module);
};


#endif //SYNTHONE_FILTERMODULE_H
