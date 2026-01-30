#include "AskServo.h"

//Consructor
AskServo::AskServo(uint8_t pin, uint8_t lowerEdgePin, uint8_t upperEdgePin, uint8_t overridePin)
{
    Serial.println(F("Servo: Starting"));
    
    velocity    = 0;        
    this->pin_servo = pin;
    this->pin_edge_lower = lowerEdgePin;
    this->pin_edge_upper = upperEdgePin;
    this->pin_override = overridePin;
    
    ledcSetup(PWM_CHANNEL_SERVO, 50, 8);
    ledcAttachPin(pin, PWM_CHANNEL_SERVO);

    pinMode(pin_edge_lower, INPUT_PULLUP);
    pinMode(pin_edge_upper, INPUT_PULLUP);
    pinMode(pin_override, INPUT);
    


    Serial.println(F("Servo: Started"));
}

//Public
void    AskServo::tick()
{
    //OVERIDE SWITCH CUTOFF
    if (!digitalRead(pin_override))
    {
        ledcWrite(PWM_CHANNEL_SERVO, 0);
        return;
    }


    //EDGE HIT CUTOFF
    bool lowerHit = !digitalRead(pin_edge_lower);
    bool upperHit = !digitalRead(pin_edge_upper);
    edgeHit = SERVOEDGEHIT_NONE;
    if (lowerHit)
    {
        edgeHit = SERVOEDGEHIT_LOWER;
    }
    if (upperHit)
    {
        edgeHit = SERVOEDGEHIT_UPPER;
    }
    if (lowerHit && upperHit)
    {
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

    //WHINE NOISE REMOVAL
    if(abs(dingusVelocity) < 6)
    {
        dingusVelocity = 0;  //If veloctity (abs) is less than 6, set to 0 (Removes the whining noise while motor is not really moving)
    }

    //VERY OUT OF RANGE SPEED BOOST
    if (abs(temperatureDifference) > 7) // If the temperature difference is greater than 15 degrees, move the servo at full speed
    {
        if (temperatureDifference > 0) dingusVelocity = SERVO_SPEED_NEGATIVE_MIN; // If the temperature difference is positive (Too warm), move the servo to the right
        else dingusVelocity = SERVO_SPEED_POSITIVE_MAX; // If the temperature difference is negative (Too cold), move the servo to the left
    }

    
    // Map velocity to PWM duty cycle       180 middle
    const int minDutyCycle = 130; //60 min, 130 best
    const int maxDutyCycle = 255; //330 max, 250 best
    int pwmValue = map(dingusVelocity, SERVO_SPEED_NEGATIVE_MIN, SERVO_SPEED_POSITIVE_MAX, maxDutyCycle, minDutyCycle);
    pwmValue = pwmValue / 10;


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
void AskServo::setTemperatureDifference(int temperatureDifference)
{
    this->temperatureDifference = temperatureDifference;
}