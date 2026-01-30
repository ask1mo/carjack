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
#define SOUND_EASCANADA         9

#define SOUND_ATOS_SCROLL       10
#define SOUND_ATOS_SELECT       11
#define SOUND_ATOS_RETURN       12
#define SOUND_ATOS_BOOTUP       13
#define SOUND_ATOS_SHUTDOWN     14
#define SOUND_ATOS_NOBATTERY    15

#define SOUND_TEMPDEBUG_TOGGLE  255 //Workaround for debugging to turn on/off the buzzer. NEVER used inside buzzer code.

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
    uint16_t getFrequency_EASCANADA();
    
    uint16_t getFrequency_ATOS_SCROLL();
    uint16_t getFrequency_ATOS_SELECT();
    uint16_t getFrequency_ATOS_RETURN();
    uint16_t getFrequency_ATOS_BOOTUP();
    uint16_t getFrequency_ATOS_SHUTDOWN();
    uint16_t getFrequency_ATOS_NOBATTERY();

    public:
    AskBuzzer(byte pin);
    void toggleEnabled();
    bool getEnabled();
    void setEnabled(bool enabled);
    uint8_t getCurrentlyPlayingSound();
    void tick();
    void playSound(uint8_t soundCode);

};

#endif