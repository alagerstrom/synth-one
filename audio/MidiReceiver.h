//
// Created by Andreas Lagerstrom on 2018-01-28.
//

#ifndef SYNTHONE_MIDIRECEIVER_H
#define SYNTHONE_MIDIRECEIVER_H

#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "IPlugStructs.h"
#include "GallantSignal.h"

using Gallant::Signal2;

class MidiReceiver {

private:
    IMidiQueue midiQueue;
    static const int NUMBER_OF_KEYS = 128;
    int numberOfPlayedKeys;
    bool keyStatus[NUMBER_OF_KEYS];
    int lastNoteNumber;
    double lastFrequency;
    int lastVelocity;
    int offset;

//    double noteNumberToFrequency(int noteNumber);

public:

    MidiReceiver() {
        numberOfPlayedKeys = 0;
        lastNoteNumber = -1;
        lastFrequency = -1.0;
        lastVelocity = 0;
        offset = 0;
        for (int i = 0; i < NUMBER_OF_KEYS; i++)
            keyStatus[i] = false;
    }

    Signal2<int, int> noteOn;
    Signal2<int, int> noteOff;

    bool getKeyStatus(int index);

    int getNumberOfPlayedKeys();

    int getLastNoteNumber() const;

//    double getLastFrequency() const;

    int getLastVelocity() const;

    void advance();

    void onMessageReceived(IMidiMsg *midiMessage);

    void flush(int numberOfFrames);

    void resize(int blockSize);

    int findHighestPlayedNote();
};


#endif //SYNTHONE_MIDIRECEIVER_H
