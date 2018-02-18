//
// Created by Andreas Lagerstrom on 2018-02-17.
//

#include "LFOModule.h"

void LFOModule::draw(IGraphics *iGraphics, IBitmap *knob) {
    iGraphics->AttachControl(new IKnobMultiControl(iPlug, x, y, frequencyParam, knob));
    iGraphics->AttachControl(new IKnobMultiControl(iPlug, x + spacing, y, modeParam, knob));
    iGraphics->AttachControl(new IKnobMultiControl(iPlug, x + 2* spacing, y, amountParam, knob));
}

void LFOModule::handleParamChange(int paramIndex) {
    if (paramIndex == this->frequencyParam){
        oscillator.setFrequency(iPlug->GetParam(frequencyParam)->Value());
    }else if (paramIndex == this->modeParam){
        oscillator.setMode(static_cast<OscillatorMode>(static_cast<int>(iPlug->GetParam(this->modeParam)->Value())));
    }else if (paramIndex == this->amountParam){
        amount = iPlug->GetParam(amountParam)->Value();
    }

}

void LFOModule::advance() {
    oscillator.advance();
}

double LFOModule::getSample() {
    return oscillator.getSample() * amount;
}

LFOModule::LFOModule(IPlug *iPlug, int x, int y, int spacing, int frequencyParam, int modeParam, int amountParam)
        : iPlug(iPlug), x(x), y(y), spacing(spacing), frequencyParam(frequencyParam), modeParam(modeParam), amountParam(amountParam) {
}

void LFOModule::initializeParameters(const char *frequencyName, const char *modeName, const char* amountName) {
    iPlug->GetParam(frequencyParam)->InitDouble(frequencyName, 5.0, 0.05, 50.0, 0.00001, "%");
    iPlug->GetParam(frequencyParam)->SetShape(2.);
    iPlug->GetParam(modeParam)->InitEnum(modeName, 0, 4);
    iPlug->GetParam(amountParam)->InitDouble(amountName, 0.0, 0.0, 1.0, 0.0001, "%");
    iPlug->GetParam(amountParam)->SetShape(2.0);
}
