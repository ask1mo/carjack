#include "AskServo.h"

//Consructor
AskServo::AskServo(uint8_t pin, uint16_t lowerEdge, uint16_t upperEdge, uint16_t edgeToEdgeMovementDuration)
{
    Serial.println(F("Servo: Starting"));
    
    this->edgeToEdgeMovementDuration = edgeToEdgeMovementDuration;
    //Goals
    goalEdge    = LOCATION_LOWEREDGE;
    goalCoords  = 90;
    velocity    = 0;
    //Limits
    this->lowerEdge = lowerEdge;
    this->upperEdge = upperEdge;                
    //States
    movementMode = MOVEMENTMODE_IDLE;
    movementState = MOVEMENTSTATE_IDLE;
    //Time
    lastActivationTime = 0;

    servo1Pin = pin;
    
    servo1.setPeriodHertz(50); // standard 50 hz servo

    setEnabled(true);

    servo1.write(goalCoords);

    Serial.println(F("Servo: Started"));
}

//Public
void    AskServo::tick()
{
    uint64_t currentMillis = millis();

    switch (movementMode)
    {
        case MOVEMENTMODE_IDLE:
        {

        }
        break;
        case MOVEMENTMODE_EDGE:
        {
            if (currentMillis >= (lastActivationTime + edgeToEdgeMovementDuration))
            {
                if(movementState == MOVEMENTSTATE_EDGE_MOVINGTOLOWEREDGE)
                {
                    movementState = MOVEMENTSTATE_EDGE_ATLOWEREDGE;
                }
                else if(movementState == MOVEMENTSTATE_EDGE_MOVINGTOUPPERREDGE)
                {
                    movementState = MOVEMENTSTATE_EDGE_ATUPPEREDGE;
                }
                else
                {
                    movementState = MOVEMENTSTATE_ERROR;
                    Serial.println(F("ERROR DETECTED IN SERVO STATE BEHAVIOUR"));
                }
            }
        }
        break;
        case MOVEMENTMODE_COORDS:
        {
            
        }
        break;
        case MOVEMENTMODE_VELOCITY:
        {
            //Ey ask make sure the code isn't going too fast.
            int result = goalCoords+velocity;
            
            if(result < (int)lowerEdge)
            {
                goalCoords = lowerEdge;
            }
            else if(result > (int)upperEdge)
            {
                goalCoords = upperEdge;
            }
            else
            {
                goalCoords += velocity;
            }

            servo1.write(goalCoords);
        }
        break;
        case MOVEMENTMODE_PATROL:
        {
            uint8_t lowerEdgePatrol;
            uint8_t upperEdgePatrol;


            if((lowerEdge+patrolAngle) > centerCoords) //If the patrolangle will exceed the lower egde (if it will Go into negative coords)
            {
                lowerEdgePatrol = lowerEdge;
            }
            else
            {
                lowerEdgePatrol = centerCoords-patrolAngle;
            }

            if((upperEdge-patrolAngle) < patrolAngle) //If the patrolangle will exceed the upper egde (if it will Go into too far positive coords)
            {
                lowerEdgePatrol = upperEdge;
            }
            else
            {
                upperEdgePatrol = centerCoords+patrolAngle;
            }



            //Ey ask make sure the code isn't going too fast.
            int result = goalCoords+velocity;
            
            if(result < (int)lowerEdgePatrol)
            {
                goalCoords = lowerEdgePatrol;
                velocity = -velocity;
            }
            else if(result > (int)upperEdgePatrol)
            {
                goalCoords = upperEdgePatrol;
                velocity = -velocity;
            }
            else
            {
                goalCoords += velocity;
            }

            servo1.write(goalCoords);
        }
        break;
    }
}

void    AskServo::setEnabled(bool enabled)
{
    if(enabled)
    {
        Serial.println(F("Servo: Enabled"));
        //servo1.attach(servo1Pin, 1000, 2000);
        servo1.write(goalCoords);
    }
    else
    {
        Serial.println(F("Servo: Disabled"));
        //servo1.detach();
    }
}

void    AskServo::setGoalEdge(bool goalEdge)
{
    movementMode = (MOVEMENTMODE_EDGE);

    uint64_t currentMillis = millis();

    this->goalEdge = goalEdge;
    
    switch (movementState)
    {
        case MOVEMENTSTATE_EDGE_MOVINGTOLOWEREDGE:
        {
            if(this->goalEdge == LOCATION_UPPEREDGE)
            {
                servo1.write(upperEdge);
                lastActivationTime = currentMillis;
                movementState = MOVEMENTSTATE_EDGE_MOVINGTOUPPERREDGE;
            }
        }
        break;
        case MOVEMENTSTATE_EDGE_ATLOWEREDGE:
        {
            if(goalEdge == LOCATION_UPPEREDGE)
            {
                servo1.write(upperEdge);
                lastActivationTime = currentMillis;
                movementState = MOVEMENTSTATE_EDGE_MOVINGTOUPPERREDGE;
            }
        }
        break;
        case MOVEMENTSTATE_EDGE_MOVINGTOUPPERREDGE:
        {
            if(goalEdge == LOCATION_LOWEREDGE)
            {
                servo1.write(lowerEdge);
                lastActivationTime = currentMillis;
                movementState = MOVEMENTSTATE_EDGE_MOVINGTOLOWEREDGE;
            }
        }
        break;
        case MOVEMENTSTATE_EDGE_ATUPPEREDGE:
        {
            if(goalEdge == LOCATION_LOWEREDGE)
            {
                servo1.write(lowerEdge);
                lastActivationTime = currentMillis;
                movementState = MOVEMENTSTATE_EDGE_MOVINGTOLOWEREDGE;
            }
        }
        break;
        default:
        {
            if(goalEdge == LOCATION_LOWEREDGE)
            {
                servo1.write(lowerEdge);
                movementState = MOVEMENTSTATE_EDGE_MOVINGTOLOWEREDGE;
            }
            if(goalEdge == LOCATION_UPPEREDGE)
            {
                servo1.write(upperEdge);
                movementState = MOVEMENTSTATE_EDGE_MOVINGTOUPPERREDGE;
            }
            lastActivationTime = currentMillis;
        }
        break;
    }
}
void    AskServo::setVelocity(int velocity)
{
    movementMode = MOVEMENTMODE_VELOCITY;

    this->velocity = velocity;

    movementState = MOVEMENTSTATE_VELOCITY;
} 
void    AskServo::setGoalCoords(uint8_t goalCoords)
{
    movementMode = MOVEMENTMODE_COORDS;
    this->goalCoords = goalCoords;
    servo1.write(goalCoords);
    movementState = MOVEMENTSTATE_COORDS_ATCOORDS;
}
void    AskServo::setRelativeGoalCoords(int goalCoords)
{
    movementMode = MOVEMENTMODE_COORDS;

    int result = this->goalCoords+goalCoords;

    Serial.print("Current: ");
    Serial.println(this->goalCoords);

    Serial.print("Change: ");
    Serial.println(goalCoords);

    Serial.print("Result: ");
    Serial.println(result);
            
    if(result < (int)lowerEdge)
    {
        this->goalCoords = lowerEdge;
    }
    else if(result > (int)upperEdge)
    {
        this->goalCoords = upperEdge;
    }
    else
    {
        this->goalCoords = result;
    }

    Serial.print("GoalCoords: ");
    Serial.println(this->goalCoords);
    servo1.write(this->goalCoords);
}
void    AskServo::setPatrolTarget(uint8_t centerCoords, uint8_t patrolAngle, int16_t velocity)
{
    movementMode = MOVEMENTMODE_PATROL;
    this->centerCoords = centerCoords;
    this->patrolAngle = (uint8_t)(patrolAngle/2);
    if(goalCoords == upperEdge || this->velocity < 0)
    {
        this->velocity = -velocity;
    }
    else
    {
        this->velocity = velocity;
    }
}

uint8_t AskServo::getMovementMode()
{
    return movementMode;
}
uint8_t AskServo::getMovementState()
{
    return movementState;
}
uint8_t AskServo::getCurrentCoords()
{
    return goalCoords;
}

int16_t AskServo::getVelocity()
{
    if(movementMode != MOVEMENTMODE_VELOCITY) return 0;

    return velocity;
}