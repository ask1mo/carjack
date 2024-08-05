#ifndef ASKSERVO_H
#define ASKSERVO_H

#include <Arduino.h>
#include <ESP32Servo.h>

#define MOVEMENTMODE_IDLE       0
#define MOVEMENTMODE_EDGE       1
#define MOVEMENTMODE_COORDS     2
#define MOVEMENTMODE_VELOCITY   3
#define MOVEMENTMODE_PATROL     4

#define MOVEMENTSTATE_ERROR                     0
#define MOVEMENTSTATE_IDLE                      1
#define MOVEMENTSTATE_EDGE_MOVINGTOLOWEREDGE    2
#define MOVEMENTSTATE_EDGE_ATLOWEREDGE          3
#define MOVEMENTSTATE_EDGE_MOVINGTOUPPERREDGE   4
#define MOVEMENTSTATE_EDGE_ATUPPEREDGE          5
#define MOVEMENTSTATE_COORDS_ATCOORDS           6
#define MOVEMENTSTATE_COORDS_MOVINGTOCOORDS     7
#define MOVEMENTSTATE_VELOCITY                  8

#define LOCATION_LOWEREDGE 0
#define LOCATION_UPPEREDGE 1

class AskServo
{
    private:
    //Setup
    uint16_t    edgeToEdgeMovementDuration;
    //Goals
    bool        goalEdge;
    uint8_t     goalCoords;
    int16_t     velocity;
    uint8_t     centerCoords; //Central point for patrol mode to move around
    uint8_t     patrolAngle; //The angle around the central point that needs to be patrolled
    //Limits
    uint16_t    lowerEdge;
    uint16_t    upperEdge;
    //States
    uint8_t     movementMode;
    uint8_t     movementState;
    //Time
    uint64_t    lastActivationTime;
    //Blah
    Servo servoN1;
    Servo servoN2;
    Servo servo1;
    Servo servo2;
    uint8_t servo1Pin;

    public:
    AskServo(uint8_t pin, uint16_t coordsClosed, uint16_t coordsOpened, uint16_t movementDuration);
    void    tick();

    void    setEnabled(bool enabled); 

    void    setGoalEdge(bool goalEdge);
    void    setVelocity(int velocity);
    void    setGoalCoords(uint8_t goalCoords);
    void    setRelativeGoalCoords(int goalCoords);
    void    setPatrolTarget(uint8_t centerCoords, uint8_t patrolAngle, int16_t velocity);
    
    uint8_t getMovementMode(); 
    uint8_t getMovementState();
    uint8_t getCurrentCoords();
    int16_t getVelocity();
};

#endif