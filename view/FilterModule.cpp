//
// Created by Andreas Lagerstrom on 2018-02-17.
//

#include "FilterModule.h"

void FilterModule::draw(IGraphics *iGraphics, IBitmap *knob) {
    iGraphics->AttachControl(new IKnobMultiControl(iPlug, x, y, cutoffParam, knob));
    iGraphics->AttachControl(new IKnobMultiControl(iPlug, x + spacing, y, resonanceParam, knob));
    iGraphics->AttachControl(new IKnobMultiControl(iPlug, x + 2 * spacing, y, modeParam, knob));
}

void FilterModule::handleParamChange(int paramIndex) {
    if (paramIndex == this->cutoffParam) {
        filter.setCutoff(iPlug->GetParam(paramIndex)->Value());
    } else if (paramIndex == this->resonanceParam) {
        filter.setResonance(iPlug->GetParam(paramIndex)->Value());
    } else if (paramIndex == this->modeParam) {
        filter.setFilterMode(static_cast<Filter::FilterMode>(iPlug->GetParam(paramIndex)->Int()));
    }
}

void FilterModule::advance() {

}

double FilterModule::getSample() {
    filter.setModulation(modulator->getSample());
    double value = 0;
    for (unsigned long i = 0; i < inputModules.size(); i++){
        AbstractModule *module = inputModules.at(i);
        value += module->getSample() / inputModules.size();
    }
    return filter.process(value);
}

FilterModule::FilterModule(IPlug *iPlug, int x, int y, int spacing, int cutoffParam, int resonanceParam, int modeParam)
        : iPlug(iPlug), cutoffParam(cutoffParam), resonanceParam(resonanceParam), modeParam(modeParam), spacing(spacing), x(x), y(y) {
}

void FilterModule::initializeParameters(const char *cutoffName, const char *resonanceName, const char *modeName) {
    iPlug->GetParam(this->modeParam)->InitEnum(modeName, Filter::FILTER_MODE_LOWPASS, Filter::numberOfFilterModes);
    iPlug->GetParam(this->cutoffParam)->InitDouble(cutoffName, 0.99, 0.01, 0.99, 0.001);
    iPlug->GetParam(this->cutoffParam)->SetShape(2);
    iPlug->GetParam(this->resonanceParam)->InitDouble(resonanceName, 0.01, 0.01, 1.0, 0.001);

}

void FilterModule::addInputModule(AbstractModule * module) {
    inputModules.push_back(module);
}

void FilterModule::setModulator(AbstractModule *module) {
    this->modulator = module;
}
