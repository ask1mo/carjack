#ifndef MAIN_H
#define MAIN_H

#include "AskButton/AskButton.h"
#include <Arduino.h>
#include <Wire.h>
#include <ESP32Encoder.h>
#include <vector>
#include "setup.h"
#include "Vehicle/vehicle.h"
#include "cruiseControl/CruiseControl.h"
#include "DataLink/DataLink.h"


#define MAXENCODERSTEPS 1


DataLink dataLink;
CruiseControl cruiseControl; //Pin, longPressDuration   
Vehicle vehicle;
AskButton *button;
ObdDataFrame obdData;
OperatingMode operatingMode = CCMODE; // Cruise control controls, or menu mode controls?
int8_t buffered_encoder_in = 0; //Used to buffer encoder input
uint8_t buffered_encoder = 0; //0-255, used to buffer encoder input


/*=== === === MUTEX === === ===*/
//Audio
volatile bool OBD2MutexTaken = false;
ObdDataFrame mutex_obdData;



//Tasks
TaskHandle_t            Task1;
TaskHandle_t            Task2;

void task_alpha(void *pvParameters); // Task for main loop, rotary encoder+button, cruise control relay, Communication to head unit.
void task_beta (void *pvParameters); // Task for bluetooth OBD2 reading

#endif