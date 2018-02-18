//
// Created by Andreas Lagerstrom on 2018-02-17.
//

#include "EnvelopeModule.h"

void EnvelopeModule::draw(IGraphics *iGraphics, IBitmap *knob) {
    iGraphics->AttachControl(new IKnobMultiControl(iPlug, x, y, this->attackParameter, knob));
    iGraphics->AttachControl(new IKnobMultiControl(iPlug, x + spacing, y, this->decayParameter, knob));
    iGraphics->AttachControl(new IKnobMultiControl(iPlug, x + 2 * spacing, y, this->sustainParameter, knob));
    iGraphics->AttachControl(new IKnobMultiControl(iPlug, x + 3 * spacing, y, this->releaseParameter, knob));
    iGraphics->AttachControl(new IKnobMultiControl(iPlug, x + 4 * spacing, y, this->amountParameter, knob));

}

void EnvelopeModule::handleParamChange(int paramIndex) {
    if (paramIndex == this->attackParameter) {
        envelopeGenerator.setStageValue(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK, iPlug->GetParam(paramIndex)->Value());
    } else if (paramIndex == this->decayParameter) {
        envelopeGenerator.setStageValue(EnvelopeGenerator::ENVELOPE_STAGE_DECAY, iPlug->GetParam(paramIndex)->Value());
    } else if (paramIndex == this->sustainParameter) {
        envelopeGenerator.setStageValue(EnvelopeGenerator::ENVELOPE_STAGE_SUSTAIN, iPlug->GetParam(paramIndex)->Value());
    } else if (paramIndex == this->releaseParameter) {
        envelopeGenerator.setStageValue(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE, iPlug->GetParam(paramIndex)->Value());
    } else if (paramIndex == this->amountParameter){
        amountValue = iPlug->GetParam(this->amountParameter)->Value();
    }
}

EnvelopeModule::EnvelopeModule(IPlug *iPlug, int x, int y, int spacing, int attackParameter, int decayParameter, int sustainParameter, int releaseParameter, int amountParameter)
        : iPlug(iPlug),
          x(x),
          y(y),
          spacing(spacing),
          attackParameter(attackParameter),
          decayParameter(decayParameter),
          sustainParameter(sustainParameter),
          releaseParameter(releaseParameter),
          amountParameter(amountParameter) {
}

void EnvelopeModule::initializeParameters(const char *attackName, const char *decayName, const char *sustainName, const char *releaseName, const char *amountName) {
    iPlug->GetParam(this->attackParameter)->InitDouble(attackName, 0.01, 0.01, 10.0, 0.001);
    iPlug->GetParam(this->attackParameter)->SetShape(3);
    iPlug->GetParam(this->decayParameter)->InitDouble(decayName, 0.5, 0.01, 15.0, 0.001);
    iPlug->GetParam(this->decayParameter)->SetShape(3);
    iPlug->GetParam(this->sustainParameter)->InitDouble(sustainName, 0.1, 0.001, 1.0, 0.001);
    iPlug->GetParam(this->sustainParameter)->SetShape(2);
    iPlug->GetParam(this->releaseParameter)->InitDouble(releaseName, 1.0, 0.001, 15.0, 0.001);
    iPlug->GetParam(this->releaseParameter)->SetShape(3);
    iPlug->GetParam(this->amountParameter)->InitDouble(amountName, 0.0, -1.0, 1.0, 0.001);


}

void EnvelopeModule::enterStage(EnvelopeGenerator::EnvelopeStage stage) {
    envelopeGenerator.enterStage(stage);

}

void EnvelopeModule::setSampleRate(double rate) {
    envelopeGenerator.setSampleRate(rate);
}

void EnvelopeModule::advance() {
    envelopeGenerator.advance();

}

double EnvelopeModule::getSample() {
    return envelopeGenerator.getSample() * amountValue;
}
