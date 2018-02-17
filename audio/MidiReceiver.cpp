//
// Created by Andreas Lagerstrom on 2018-01-28.
//

#include <cmath>
#include "MidiReceiver.h"



bool MidiReceiver::getKeyStatus(int index) {
    return keyStatus[index];
}

int MidiReceiver::getNumberOfPlayedKeys() {
    return numberOfPlayedKeys;
}

int MidiReceiver::getLastNoteNumber() const {
    return lastNoteNumber;
}

//double MidiReceiver::getLastFrequency() const {
//    return lastFrequency;
//}

int MidiReceiver::getLastVelocity() const {
    return lastVelocity;
}

void MidiReceiver::advance() {
    while (!midiQueue.Empty()) {
        IMidiMsg *midiMessage = midiQueue.Peek();
        if (midiMessage->mOffset > offset)
            break;
        IMidiMsg::EStatusMsg statusMessage = midiMessage->StatusMsg();
        int noteNumber = midiMessage->NoteNumber();
        int velocity = midiMessage->Velocity();
        if (statusMessage == IMidiMsg::kNoteOn && velocity) {
            if (!keyStatus[noteNumber]) {
                keyStatus[noteNumber] = true;
                numberOfPlayedKeys++;
            }

            if (noteNumber != lastNoteNumber) {
                lastNoteNumber = noteNumber;
                lastVelocity = velocity;
                noteOn(noteNumber, velocity);
            }
        } else {
            if (keyStatus[noteNumber]) {
                keyStatus[noteNumber] = false;
                numberOfPlayedKeys--;
                lastNoteNumber = findHighestPlayedNote();
                if (numberOfPlayedKeys == 0)
                    noteOff(noteNumber, lastVelocity);
            }
        }
        midiQueue.Remove();
    }
    offset++;
}

int MidiReceiver::findHighestPlayedNote(){
    for (int i = NUMBER_OF_KEYS - 1; i >= 0; i--){
        if (keyStatus[i])
            return i;
    }
    return -1;
}

void MidiReceiver::onMessageReceived(IMidiMsg *midiMessage) {
    IMidiMsg::EStatusMsg statusMsg = midiMessage->StatusMsg();
    if (statusMsg == IMidiMsg::kNoteOn || statusMsg == IMidiMsg::kNoteOff) {
        midiQueue.Add(midiMessage);
    }
}

void MidiReceiver::flush(int numberOfFrames) {
    midiQueue.Flush(numberOfFrames);
    offset = 0;
}

void MidiReceiver::resize(int blockSize) {
    midiQueue.Resize(blockSize);
}
