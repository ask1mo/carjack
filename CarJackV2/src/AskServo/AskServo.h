#ifndef ASKSERVO_H
#define ASKSERVO_H

#include <Arduino.h>
#include <setup.h>

#define SERVOEDGEHIT_NONE 0
#define SERVOEDGEHIT_LOWER 1
#define SERVOEDGEHIT_UPPER 2

#define SERVO_SPEED_NEGATIVE -15
#define SERVO_SPEED_NEGATIVE_MIN -45
#define SERVO_SPEED_POSITIVE 15
#define SERVO_SPEED_POSITIVE_MAX 45

#define INTERVALMILLIS_SERVO 1000


class AskServo
{
private:
    int     velocity;
    uint8_t pin_servo;
    uint8_t pin_edge_lower;
    uint8_t pin_edge_upper;
    uint8_t pin_override;
    uint8_t edgeHit = SERVOEDGEHIT_NONE;
    uint32_t lastMillis_Servo = 0;
    int temperatureDifference = 0;
    

public:
    AskServo(uint8_t pin, uint8_t lowerEdgePin, uint8_t upperEdgePin, uint8_t overridePin);
    void    tick();
    void    setVelocity(int velocity);
    int     getVelocity();
    void    setTemperatureDifference(int temperatureDifference);
};

#endif