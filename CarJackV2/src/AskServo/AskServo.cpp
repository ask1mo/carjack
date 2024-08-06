#include "AskServo.h"

//Consructor
AskServo::AskServo(uint8_t pin, uint8_t lowerEdgePin, uint8_t upperEdgePin, int minSpeed, int maxSpeed)
{
    Serial.println(F("Servo: Starting"));
    
    velocity    = 0;        
    this->pin = pin;
    
    ledcSetup(PWM_CHANNEL_SERVO, 50, 8);
    ledcAttachPin(pin, PWM_CHANNEL_SERVO);
    


    Serial.println(F("Servo: Started"));
}

//Public
void    AskServo::tick()
{
    if (!enabled)
    {
        Serial.println(F("Servo: Disabled"));
        ledcWrite(PWM_CHANNEL_SERVO, 0);
        return;
    }

    int dingusVelocity = velocity;

    switch (edgeHit)
    {
        case SERVOEDGEHIT_LOWER:
        {
            if(velocity < 0)
            {
                dingusVelocity = 0;
            }
        }
        break;
        case SERVOEDGEHIT_UPPER:
        {
            if(velocity > 0)
            {
                dingusVelocity = 0;
            }
        }
        break;
    }

    Serial.print(F("Servo: velocity: "));
    Serial.print(dingusVelocity);
    
    
    // Map velocity to PWM duty cycle       180 middle
    const int minDutyCycle = 130; //60 min, 130 best
    const int maxDutyCycle = 255; //330 max, 250 best
    int pwmValue = map(dingusVelocity, SERVO_MIN, SERVO_MAX, maxDutyCycle, minDutyCycle);
    pwmValue = pwmValue / 10;

    Serial.print(F(" PWM: "));
    Serial.println(pwmValue);

    // Write the PWM value
    ledcWrite(PWM_CHANNEL_SERVO, pwmValue);
}
void    AskServo::setVelocity(int velocity)
{
    this->velocity = velocity;
} 
int AskServo::getVelocity()
{
    return velocity;
}
void    AskServo::setEnabled(bool enabled)
{
    this->enabled = enabled;
}