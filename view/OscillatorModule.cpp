//
// Created by Andreas Lagerstrom on 2018-02-13.
//

#include "OscillatorModule.h"

OscillatorModule::OscillatorModule(IPlug *iPlug, int x, int y, int spacing, int modeParameter, int tuneParameter, int octaveParameter, int semitoneParameter)
        : iPlug(iPlug),
          x(x),
          y(y),
          spacing(spacing),
          modeParameter(modeParameter),
          tuneParameter(tuneParameter),
          octaveParameter(octaveParameter),
          semitoneParameter(semitoneParameter),
          oscillatorController(iPlug, tuneParameter, modeParameter, octaveParameter, semitoneParameter) {
}

void OscillatorModule::initializeParameters(const char *frequencyParameterName, const char *waveformParameterName, const char *octaveParameterName, const char *semitoneParameterName) {
    iPlug->GetParam(tuneParameter)->InitDouble(frequencyParameterName, 1.0, (double) 11 / 12, (double) 13 / 12, 0.00001, "%");
    iPlug->GetParam(tuneParameter)->SetShape(1.);
    iPlug->GetParam(modeParameter)->InitEnum(waveformParameterName, 0, 4);
    iPlug->GetParam(octaveParameter)->InitInt(octaveParameterName, 3, 0, 6);
    iPlug->GetParam(semitoneParameter)->InitInt(semitoneParameterName, 0, -12, 12);
}

void OscillatorModule::draw(IGraphics *iGraphics, IBitmap *knob) {
    iGraphics->AttachControl(new IKnobMultiControl(iPlug, x, y, octaveParameter, knob));
    iGraphics->AttachControl(new IKnobMultiControl(iPlug, x + spacing, y, semitoneParameter, knob));
    iGraphics->AttachControl(new IKnobMultiControl(iPlug, x + 2 * spacing, y, tuneParameter, knob));
    iGraphics->AttachControl(new IKnobMultiControl(iPlug, x + 3 * spacing, y, modeParameter, knob));
}

void OscillatorModule::setPlayedNote(int noteNumber) {
    oscillatorController.setPlayedNote(noteNumber);

}

void OscillatorModule::setSampleRate(double rate) {
    oscillatorController.setSampleRate(rate);
}

void OscillatorModule::handleParamChange(int paramIndex) {
    oscillatorController.handleParamChange(paramIndex);
}

double OscillatorModule::nextSample() {
    return oscillatorController.nextSample();
}