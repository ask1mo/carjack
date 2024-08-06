#ifndef ASKSERVO_H
#define ASKSERVO_H

#include <Arduino.h>
#include <setup.h>

#define SERVOEDGEHIT_NONE 0
#define SERVOEDGEHIT_LOWER 1
#define SERVOEDGEHIT_UPPER 2

#define SERVO_MIN -45
#define SERVO_MAX 45


class AskServo
{
private:
    int     velocity;
    uint8_t pin;
    uint8_t edgeHit = SERVOEDGEHIT_NONE;
    bool    enabled = true;

public:
    AskServo(uint8_t pin, uint8_t lowerEdgePin, uint8_t upperEdgePin, int minSpeed, int maxSpeed);
    void    tick();
    void    setVelocity(int velocity);
    int     getVelocity();
    void    setEnabled(bool enabled);
};

#endif