#ifndef PID_H
#define PID_H

#include <Arduino.h>

class PID
{
    private:
    uint8_t *Kp;
    uint8_t *Ki;
    uint8_t *Kd;

    uint8_t min;
    uint8_t max;
    float setpoint;
    float lastError;
    float lastTime;
    float lastOutput;

    public:
    PID(uint8_t *Kp, uint8_t *Ki, uint8_t *Kd, uint8_t min, uint8_t max);
    int8_t calculate(float input);
    void setSetpoint(float setpoint);
};

#endif
