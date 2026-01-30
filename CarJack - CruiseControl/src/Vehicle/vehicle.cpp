#include "vehicle.h"

Vehicle::Vehicle()
{
    Serial.print(F("Creating Vehicle at adress "));
    Serial.println((int)this, DEC);
    // start bluetooth stack and schedule non-blocking connection attempts
  
  

    Serial.print(F("Starting Bluetooth as: "));
    Serial.println(BLUETOOTH_NAME);
    // Start Bluetooth in discoverable mode; do not block on connect
    SerialBT.begin(BLUETOOTH_NAME, true);
    // set last attempt so tryConnect will attempt immediately
    lastConnectAttempt = 0;
    connected = false;
}

void Vehicle::loop()
{
  // maintain connection state and attempt reconnects if needed
  if (!connected)
  {
    tryConnect();
    return;
  }
  else if (!SerialBT.connected())   // If connected but Bluetooth dropped, mark disconnected so reconnect attempts begin
  {
    Serial.println(F("OBD Bluetooth disconnected."));
    connected = false;
    // schedule immediate reconnect attempt
    lastConnectAttempt = 0;
    return;
  }


    pollData();

}





void Vehicle::tryConnect()
{
  unsigned long now = millis();
  if (now - lastConnectAttempt < connectIntervalMs && lastConnectAttempt != 0)
    return;

  lastConnectAttempt = now;
  Serial.print(F("Attempting OBD connect to '"));
  Serial.print(OBD_DEVICE_NAME);
  Serial.println(F("'..."));

  if (!SerialBT.connect(OBD_DEVICE_NAME))
  {
    Serial.println(F("OBD BT connect failed."));
    return;
  }

  // SerialBT reports connected; initialize ELM327 (non-blocking init if supported)
  Serial.println(F("Bluetooth connected, initializing ELM327..."));
  // Call begin with non-blocking flag (library dependent). Keep a short timeout.
  if (!myELM327.begin(SerialBT, true, 2000))
  {
    Serial.println(F("ELM327 init failed."));
    SerialBT.disconnect();
    return;
  }

  Serial.println(F("Connected to ELM327"));
  connected = true;
}

void Vehicle::pollData()
{
  // Read vehicle speed (kph)
  float spd = myELM327.kph();
  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    data.vehicleSpeed = spd;
    dataMillis_speed = millis();
  }

  // Read RPM
  float r = myELM327.rpm();
  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    data.rpm = r;
  }

  // Read throttle position
  float t = myELM327.throttle();
  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    data.throttlePosition = t;
  }
}

ObdDataFrame Vehicle::getData()
{
    return data;
}


bool Vehicle::isDataTimedOut()
{
    //Check if speed data is older than 2 seconds, or if bluetooth/ELM is disconnected
    if (!connected) return true;
    if (millis() - dataMillis_speed > 2000) return true;
    return false; 
}