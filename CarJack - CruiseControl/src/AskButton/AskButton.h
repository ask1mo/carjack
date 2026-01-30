#ifndef ASKBUTTON_H
#define AKSBUTTON_H

#include <Arduino.h>

#define BUTTON_RELEASED             0
#define BUTTON_TAPPED               1
#define BUTTON_HELD                 2
#define BUTTON_ERROR                3 
#define BUTTON_PRESSSTATE_HIGH      1 //This one is only for getPressState()

#define BTN_REL    0
#define BTN_TPD    1
#define BTN_HLD_1S 2
#define BTN_HLD_3S 3
#define BTN_HLD_5S 4

class AskButton
{
    private:
    byte pin;
    bool state;
    bool prevState;
    bool holdStateIsKnown;
    unsigned long myMillis;
    unsigned long pressStartTime;
    unsigned long lastActionTime;
    uint32_t longPressDuration;

    uint8_t lastCommand;

    public:
    AskButton(byte pin, uint32_t longPressDuration);
    byte getCommand();
    uint8_t getLastCommand(bool removeAfterRead);
    bool getPressState();
    unsigned long getLastTimeDelta();
};



#endif