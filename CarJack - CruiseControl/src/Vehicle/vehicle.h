#ifndef vehicle_H
#define vehicle_H

#include <Arduino.h>
#include "BluetoothSerial.h"
#include "ELMduino.h"
#include "../setup.h"

#define BLUETOOTH_NAME "CarJack"
#define OBD_DEVICE_NAME "OBDII"

class Vehicle
{
    private:
    bool connected = false;
    BluetoothSerial SerialBT;
    ELM327 myELM327;
    ObdDataFrame data;
    // connection configuration/state
    unsigned long lastConnectAttempt = 0;
    unsigned long connectIntervalMs = 5000; // try every 5s
    uint32_t dataMillis_speed = 0;
    

    void pollData();
    void tryConnect();

    public:
    Vehicle();
    void loop();
    void beginOBD();
    ObdDataFrame getData();
    bool isDataTimedOut();
};




#endif