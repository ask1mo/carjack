#ifndef ASKBUZZER_H
#define ASKBUZZER_H

#include <Arduino.h>
#include "setup.h"

#define SOUND_NONE              0
#define SOUND_BOOTUP            1
#define SOUND_SHUTDOWN          2
#define SOUND_BUTTON_CLICK      3
#define SOUND_BUTTON_FORWARD    4
#define SOUND_BUTTON_BACKWARD   5
#define SOUND_SMOKE_COUNTDOWN   6
#define SOUND_SMOKE_ARM         7
#define SOUND_SMOKE_DISARM      8
#define SOUND_EASCANADA         9

#define TONE_NONE               0
#define TONE_SMOKE_FIRE         1

class AskBuzzer
{
    private:
    byte pin;
    bool enabled;
    uint8_t currentPlayingSound;
    uint64_t soundStartTime;
    uint16_t soundTime;
    uint16_t getFrequency_NONE();
    uint16_t getFrequency_BOOTUP();
    uint16_t getFrequency_SHUTDOWN();
    uint16_t getFrequency_BUTTON_CLICK();
    uint16_t getFrequency_BUTTON_FORWARD();
    uint16_t getFrequency_BUTTON_BACKWARD();
    uint16_t getFrequency_SMOKE_COUNTDOWN();
    uint16_t getFrequency_SMOKE_ARM();
    uint16_t getFrequency_SMOKE_DISARM();
    uint16_t getFrequency_EASCANADA();

    public:
    AskBuzzer(byte pin);
    void toggleEnabled();
    bool getEnabled();
    void setEnabled(bool enabled);
    uint8_t getCurrentlyPlayingSound();
    void tick();
    void playSound(uint8_t soundCode);
    void playTone(uint8_t toneCode);
    void stopTone();
};

#endif