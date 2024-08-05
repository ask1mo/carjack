#include "PID.h"

PID::PID(uint8_t *Kp, uint8_t *Ki, uint8_t *Kd, uint8_t min, uint8_t max)
{
    Serial.println(F("PID: Starting"));
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    this->min = min;
    this->max = max;
    lastError = 0;
    lastTime = 0;
    lastOutput = 0;
    setpoint = 0;
}

int8_t PID::calculate(float input)
{
    Serial.print(F("PID: Input: "));
    Serial.print(input);
    Serial.print(F(" Setpoint: "));
    Serial.print(setpoint);

    float currentTime = millis();
    float elapsedTime = (currentTime - lastTime) / 1000.0; // Convert to seconds
    float error = setpoint - input;
    float derivative = (error - lastError) / elapsedTime;
    float integral = lastOutput + error * elapsedTime;

    // Clamp integral to prevent windup
    if (integral > max) integral = max;
    if (integral < min) integral = min;

    float output = *Kp * error + *Ki * integral + *Kd * derivative;

    Serial.print(F(" Real Output: "));
    Serial.print(output);

    // Clamp output to min and max
    if (output > max) output = max;
    if (output < min) output = min;

    Serial.print(F(" Effective Output: "));
    Serial.print(output);

    // Save for next calculation
    lastError = error;
    lastTime = currentTime;
    lastOutput = integral;

    return static_cast<int8_t>(output);
}

void PID::setSetpoint(float setpoint)
{
    this->setpoint = setpoint;
}

