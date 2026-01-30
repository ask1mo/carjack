#include "CruiseControl.h"

//Constructor
CruiseControl::CruiseControl()
{

    Serial.print(F("Creating CruiseControl at adress "));
    Serial.println((int)this, DEC);

    pinMode(PIN_RELAY_PLUS, OUTPUT);
    pinMode(PIN_RELAY_MIN, OUTPUT);
    pinMode(PIN_RELAY_SET, OUTPUT);

}

//Private
void CruiseControl::actuateRelays()
{
  //Actuate relays if needed. Do this by applying the stored pending actions one by one
  // If an actuator pulse is currently active, check if it should be turned off now
  if (actuatorActive) //Turning off an active pulse
  {
    if (currentMillis >= actuatorDeactivateTime)
    {
      digitalWrite(activeActuatorPin, LOW);
      actuatorActive = false;
      // Update expected speed after actuation when the pulse completes
      speed_vehicle_expectedAfterActuation += (acceleration * ((activeActuatorActionValue + acceleration - 1) / acceleration)); //Round up division
      activeActuatorActionValue = 0;
    }
  }
  // If no pulse is active, start the next pending action (if timing allows)
  else if (pendingActuatorActions.size() != 0) // Turning on a new pulse
  {
    if ((currentMillis - lastRelayActionPerformed) > acceleration*ONE_SECOND) // spacing between sequences
    {
      int action = pendingActuatorActions.front(); //Get the first action in the queue
      pendingActuatorActions.erase(pendingActuatorActions.begin()); //Remove it from the queue
      int pinToActuate = 0;
      if (action > 0) pinToActuate = PIN_RELAY_PLUS; //Need to speed up
      else if (action < 0) pinToActuate = PIN_RELAY_MIN; //Need to slow down
      

      Serial.println("Cruise Control: Actuating pin "+String(pinToActuate)+" for action "+String(action));
      digitalWrite(pinToActuate, HIGH);
      // Start a non-blocking pulse: schedule when to turn it LOW
      activeActuatorPin = pinToActuate;
      actuatorActive = true;
      actuatorDeactivateTime = currentMillis + 500;
      activeActuatorActionValue = action;

      lastRelayActionPerformed = currentMillis;
    }
  }
  else if (actuatorSequenceInProgress && pendingActuatorActions.size() == 0) //Detecting end of actuation sequence
  {
    actuatorSequenceInProgress = false;
    actuatorSequenceCompletionTime = currentMillis;
    Serial.println("Cruise Control: Actuation sequence complete");
  }
}
void CruiseControl::setTargetSpeed(uint8_t speed)
{
  if (speed < 30 && speed != 0)
  {
    speed_target = 30;
  }
  else
  {
    speed_target = speed;
  }

  Serial.print("Set ccSpeed to ");
  Serial.println(speed_target);


  //Calculate the speed difference from current speed and the speed the cc will prolly be in after all actuations
  int speedDif = speed_target - speed_vehicle_expectedAfterActuation;
  //Add actuator actions to the queue
  pendingActuatorActions.push_back(speedDif);
  actuatorSequenceInProgress = true;


}
bool CruiseControl::isVehicleAtTargetSpeed()
{
  //Check if the vehicle speed is at the target speed within the precision margin
  if (abs((int)speed_vehicle - (int)speed_target) <= precision) return true;
  return false;
}

//Public
void CruiseControl::loop()
{
  if (digitalRead(PIN_BRAKESENSOR)) //If brakes are applied, disable cruise control
  {
    enabled = false;

  }


  if (!enabled)  //If the CC switch is disabled
  {
    digitalWrite(PIN_RELAY_SET, LOW); //Ensure the SET relay is off
    digitalWrite(PIN_RELAY_PLUS, LOW); //Ensure the PLUS relay is off
    digitalWrite(PIN_RELAY_MIN, LOW); //Ensure the MINUS relay is off
    return;
  }

  currentMillis = millis();



  actuateRelays();

  //if more than 5 seconds have passed since the actuators were finished, and the vehicle speed is not at the target speed, re-issue actuator commands
  if (!actuatorSequenceInProgress && (currentMillis - actuatorSequenceCompletionTime) > CC_CORRECTION_COOLDOWN && isVehicleAtTargetSpeed() == false)
  {
    int speedDif = speed_target - speed_vehicle;
    if (speedDif == 1) runPrecisionCorrection();
    else setTargetSpeed(speed_target);
  }

}

void CruiseControl::runPrecisionCorrection() //Decrease the speed by less than 1 km/h by quickly turning the cruise control on and off
{
  Serial.println("Cruise Control: Running precision correction");
  //Quickly turn off and on the cruise control to reduce speed by less than 1 km/h
  digitalWrite(PIN_RELAY_SET, HIGH);
  //Mark details to trigger pin release after pulse duration
  activeActuatorPin = PIN_RELAY_SET;
  actuatorActive = true;
  actuatorDeactivateTime = currentMillis + CC_RELAY_PRECISION_PULSE_DURATION;
}

void CruiseControl::cycleResolution()
{
  switch (resolution)
  {
    case 1:
    Serial.println("Resolution set to 5 km/h");
    resolution = 5;
    break;

    case 5:
    Serial.println("Resolution set to 10 km/h");
    resolution = 10;
    break;

    case 10:
    Serial.println("Resolution set to 1 km/h");
    resolution = 1;
    break;

    default:
    Serial.println("Unknown Resolution: set to 1 km/h");
    resolution = 1;
    break;
  }
}
void CruiseControl::cycleAcceleration()
{
  switch (acceleration)
  {
    case 1:
    Serial.println("Acceleration set to 2 km/h/s per step");
    acceleration = 2;
    break;

    case 2:
    Serial.println("Acceleration set to 3 km/h/s per step");
    acceleration = 3;
    break;

    case 3:
    Serial.println("Acceleration set to 1 km/h/s per step");
    acceleration = 1;
    break;

    default:
    Serial.println("Unknown Acceleration: set to 1 km/h/s per step");
    acceleration = 1;
    break;
  }
}
void CruiseControl::loadMemSpeed()
{
  if (speed_memory > 0)
  {
    setTargetSpeed(speed_memory);
  }
  else
  {
    Serial.println("No memory speed to resume from");
  }
}
void CruiseControl::setMemSpeed(uint8_t speed)
{
  speed_memory = speed;
  Serial.print("Set memory speed to ");
  Serial.println(speed_memory);
}
void CruiseControl::addTargetSpeed(int8_t delta)
{
  int16_t newSpeed = speed_target + (delta * resolution);
  if (newSpeed < CC_MINSPEED) newSpeed = CC_MINSPEED;
  setTargetSpeed((uint8_t)newSpeed);
}
CCRotaryMode CruiseControl::getRotaryMode()
{
  return rotaryMode;
}
void CruiseControl::updateVehicleData(ObdDataFrame obdData)
{
  if (obdData.dataTimedOut)
  {
    speed_vehicle = speed_vehicle_expectedAfterActuation; //If data timed out, assume the vehicle is at the expected speed after actuation
  }
  else
  {
    speed_vehicle = (uint8_t)obdData.vehicleSpeed;
  }
}

void CruiseControl::cycleOperatingMode()
{
  if (!enabled)
  {
    enabled = true;
    Serial.println("Cruise Control: Enabled");

    setTargetSpeed(speed_vehicle); //Set target speed to current vehicle speed when enabling

    switch (rotaryMode)
    {
      case CCRotaryMode::LOCKED:
      rotaryMode = CCRotaryMode::SPEED;
      Serial.println("Cruise Control: Rotary mode set to SPEED");
      break;

      case CCRotaryMode::SPEED:
      rotaryMode = CCRotaryMode::SETTINGS;
      Serial.println("Cruise Control: Rotary mode set to SETTINGS");
      break;

      case CCRotaryMode::SETTINGS:
      rotaryMode = CCRotaryMode::LOCKED;
      Serial.println("Cruise Control: Rotary mode set to LOCKED");
      break;



      default:
      rotaryMode = CCRotaryMode::LOCKED;
      Serial.println("Cruise Control: Unknown rotary mode, set to LOCKED");
      break;
    }

  }
  else
  {
    enabled = false;
    Serial.println("Cruise Control: Disabled");
  }
}