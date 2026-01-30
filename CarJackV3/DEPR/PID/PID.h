#ifndef PID_H
#define PID_H

#include <Arduino.h>

class PID
{
    private:
    uint8_t *Kp;
    uint8_t *Ki;
    uint8_t *Kd;

    int16_t min;
    int16_t max;
    float setpoint;
    float lastError;
    float lastTime;
    float lastOutput;

    public:
    PID(uint8_t *Kp, uint8_t *Ki, uint8_t *Kd, int16_t min, int16_t max);
    int16_t calculate(float input);
    void setSetpoint(float setpoint);
};

#endif
