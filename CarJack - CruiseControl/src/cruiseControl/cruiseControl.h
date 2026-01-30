#ifndef CruiseControl_H
#define CruiseControl_H

#include <Arduino.h>
#include "../setup.h"
#include <vector>

#define CC_MINSPEED 30 //Minimum speed to activate cruise control
#define CC_RELAY_ACTUATION_PULSE_DURATION 200 //Milliseconds to hold relay HIGH for
#define CC_RELAY_PRECISION_PULSE_DURATION 1000 //Milliseconds to wait between actuator actions in a sequence
#define CC_CORRECTION_COOLDOWN 5000 //Milliseconds to wait after an actuation sequence before allowing another

class CruiseControl
{
    private:
    uint32_t currentMillis;

    //States
    CCRotaryMode rotaryMode = CCRotaryMode::LOCKED;

    //Settings
    int precision = 0;
    int resolution = 1;
    int acceleration = 1;

    //Enabled States
    bool enabled = false;
    bool underSpeedPaused = false; //If the cruise control is paused due to being under the minimum speed of 30 km/h

    //Speeds
    uint8_t speed_target = 0; //The speed that the cruise control should be set to, in km/h
    uint8_t speed_cccurrent = 0; //The current speed that the cruise control is set to, in km/h
    uint8_t speed_vehicle_expectedAfterActuation = 0; //The speed that the cruise control will expect to be at after all actuations, in km/h
    uint8_t speed_memory = 69; //A memory of the last speed that the cruise control was manually set to, in km/h
    uint8_t speed_vehicle = 0; // The current speed of the vehicle, in km/h

    //Actuator management
    uint32_t lastRelayActionPerformed = 0;
    bool actuatorSequenceInProgress = false;
    uint32_t actuatorSequenceCompletionTime = 0;
    std::vector<int> pendingActuatorActions;
    // Non-blocking actuator state
    int activeActuatorPin = 0; // pin currently driven HIGH for actuation
    bool actuatorActive = false; // whether an actuator pulse is in progress
    uint32_t actuatorDeactivateTime = 0; // when to turn the actuator pin LOW
    int activeActuatorActionValue = 0; // the action value associated with the active pulse

    
    void setTargetSpeed(uint8_t speed);
    void actuateRelays();
    void runPrecisionCorrection();
    bool isVehicleAtTargetSpeed();


    public:
    CruiseControl();
    void loop();
    void cycleOperatingMode();
    void addTargetSpeed(int8_t delta);
    void loadMemSpeed();
    void setMemSpeed(uint8_t speed);
    void cycleResolution();
    void cycleAcceleration();
    CarJackDataFrame getDataFrame();
    CCRotaryMode getRotaryMode();
    void setVehicleSpeed(uint8_t speed);
    void updateVehicleData(ObdDataFrame obdData);

    
};



#endif