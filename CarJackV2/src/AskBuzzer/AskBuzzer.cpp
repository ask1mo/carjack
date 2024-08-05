#include "AskBuzzer.h"

AskBuzzer::AskBuzzer(byte pin)
{
    if(DEBUGLEVEL >= DEBUG_OPERATIONS)
    {
        Serial.print(F("Creating Buzzer at adress "));
        Serial.println((int)this, DEC);
    }
    this->pin = pin;
    currentPlayingSound = SOUND_NONE;
    soundTime = 0;
    enabled = true;


    pinMode(this->pin, INPUT_PULLUP);
}

uint16_t AskBuzzer::getFrequency_NONE()
{
    uint16_t playdurationTime = 0;

    playdurationTime += 1000;
    if (soundTime <=playdurationTime) return 1500;

    return 0;
}
uint16_t AskBuzzer::getFrequency_BOOTUP()
{
    uint16_t playdurationTime = 0;

    playdurationTime += 30;
    if (soundTime <=playdurationTime) return 1500;
    playdurationTime += 60;
    if (soundTime <=playdurationTime) return 0;

    playdurationTime += 30;
    if (soundTime <=playdurationTime) return 1000;
    playdurationTime += 60;
    if (soundTime <=playdurationTime) return 0;

    playdurationTime += 30;
    if (soundTime <=playdurationTime) return 2000;
    playdurationTime += 60;
    if (soundTime <=playdurationTime) 
    {
        currentPlayingSound = SOUND_NONE;
        return 0;
    }
    return 0;

}
uint16_t AskBuzzer::getFrequency_SHUTDOWN()
{
  uint16_t playdurationTime = 0;

    playdurationTime += 200;
    if (soundTime <=playdurationTime) return 200;
    playdurationTime += 200;
    if (soundTime <=playdurationTime) return 100;

    playdurationTime += 1000;
    if (soundTime <=playdurationTime) 
    {
        currentPlayingSound = SOUND_NONE;
        return 0;
    }
    return 0;
}
uint16_t AskBuzzer::getFrequency_BUTTON_CLICK()
{
    uint16_t playdurationTime = 0;

    playdurationTime += 30;
    if (soundTime <=playdurationTime) return 1000;
    playdurationTime += 50;
    if (soundTime <=playdurationTime) return 0;

    playdurationTime += 30;
    if (soundTime <=playdurationTime) return 1000;
    playdurationTime += 125;
    if (soundTime <=playdurationTime) 
    {
        currentPlayingSound = SOUND_NONE;
        return 0;
    }
    return 0;
}
uint16_t AskBuzzer::getFrequency_BUTTON_FORWARD()
{
    uint16_t playdurationTime = 0;

    playdurationTime += 30;
    if (soundTime <=playdurationTime) return 1000;
    playdurationTime += 50;
    if (soundTime <=playdurationTime) return 0;

    playdurationTime += 30;
    if (soundTime <=playdurationTime) return 1500;
    playdurationTime += 125;
    if (soundTime <=playdurationTime) 
    {
        currentPlayingSound = SOUND_NONE;
        return 0;
    }
    return 0;
}
uint16_t AskBuzzer::getFrequency_BUTTON_BACKWARD()
{
    uint16_t playdurationTime = 0;

    playdurationTime += 30;
    if (soundTime <=playdurationTime) return 1500;
    playdurationTime += 50;
    if (soundTime <=playdurationTime) return 0;

    playdurationTime += 30;
    if (soundTime <=playdurationTime) return 1000;
    playdurationTime += 125;
    if (soundTime <=playdurationTime) 
    {
        currentPlayingSound = SOUND_NONE;
        return 0;
    }
    return 0;
}
uint16_t AskBuzzer::getFrequency_SMOKE_COUNTDOWN()
{
    uint16_t playdurationTime = 0;

    playdurationTime += 30;
    if (soundTime <=playdurationTime) return 5000;
    playdurationTime += 125;
    if (soundTime <=playdurationTime)
    {
        return 0;
        currentPlayingSound = SOUND_NONE;
    }
    return 0;
}
uint16_t AskBuzzer::getFrequency_SMOKE_ARM()
{
    uint16_t playdurationTime = 0;

    playdurationTime += 30;
    if (soundTime <=playdurationTime) return 10000;
    playdurationTime += 125;
    if (soundTime <=playdurationTime) return 0;

    playdurationTime += 30;
    if (soundTime <=playdurationTime) return 10000;
    playdurationTime += 125;
    if (soundTime <=playdurationTime) return 0;

    playdurationTime += 30;
    if (soundTime <=playdurationTime) return 10000;
    playdurationTime += 125;
    if (soundTime <=playdurationTime)
    {
        return 0;
        currentPlayingSound = SOUND_NONE;
    }
}
uint16_t AskBuzzer::getFrequency_SMOKE_DISARM()
{

}
uint16_t AskBuzzer::getFrequency_EASCANADA()
{
    uint16_t playdurationTime = 0;

    playdurationTime += 500;
    if (soundTime <=playdurationTime) return 1000;
    
    playdurationTime += 500;
    if (soundTime <=playdurationTime) return 500;
        
    
    playdurationTime += 500;
    if (soundTime <=playdurationTime)
    {
        playSound(SOUND_EASCANADA);
        return 500;
    }

    currentPlayingSound = SOUND_NONE;
    return 0;
}

void AskBuzzer::tick()
{
    soundTime = millis() - soundStartTime;
    uint16_t frequency;

    switch (currentPlayingSound)
    {
        case SOUND_NONE:
        {
            frequency = 0;
        }
        break;

        case SOUND_BOOTUP:
        {
            frequency = getFrequency_BOOTUP();
        }
        break;

        case SOUND_SHUTDOWN:
        {
            frequency = getFrequency_SHUTDOWN();
        }
        break;

        case SOUND_BUTTON_CLICK:
        {
            frequency = getFrequency_BUTTON_CLICK();
        }
        break;

        case SOUND_BUTTON_FORWARD:
        {
            frequency = getFrequency_BUTTON_FORWARD();
        }
        break;

        case SOUND_BUTTON_BACKWARD:
        {
            frequency = getFrequency_BUTTON_BACKWARD();
        }
        break;

        case SOUND_SMOKE_COUNTDOWN:
        {
            frequency = getFrequency_SMOKE_COUNTDOWN();
        }
        break;

        case SOUND_SMOKE_ARM:
        {
            frequency = getFrequency_SMOKE_ARM();
        }
        break;

        case SOUND_SMOKE_DISARM:
        {
            frequency = getFrequency_SMOKE_DISARM();
        }
        break;

        case SOUND_EASCANADA:
        {
            frequency = getFrequency_EASCANADA();
        }
        break;
    }

    //Serial.print("Frequency: ");
    //Serial.println(frequency);
    if (frequency == 0 || !enabled)
    {
        noTone(pin);
    }
    else
    {
        tone(pin, frequency);
    }
}
void AskBuzzer::playSound(uint8_t soundCode)
{
    currentPlayingSound = soundCode;
    noTone(pin);
    soundStartTime = millis();
}
void AskBuzzer::playTone(uint8_t toneCode)
{
    switch (toneCode)
    {
        case TONE_NONE:
        {
            noTone(pin);
        }
        break;
        
        case TONE_SMOKE_FIRE:
        {
            tone(pin, 1000);
        }
        break;
    }
}
void AskBuzzer::stopTone()
{
    noTone(pin);
}
void AskBuzzer::toggleEnabled()
{
    if(enabled)enabled = false;
    else enabled = true;
}
bool AskBuzzer::getEnabled()
{
    return enabled;
}
uint8_t AskBuzzer::getCurrentlyPlayingSound()
{
    return currentPlayingSound;
}
void AskBuzzer::setEnabled(bool enabled)
{
    this->enabled = enabled;
}
