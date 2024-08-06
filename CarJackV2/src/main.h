#ifndef MAIN_H
#define MAIN_H

#include "AskButton/AskButton.h"
#include <Arduino.h>
#include <Wire.h>
#include <ESP32Encoder.h>
#include <vector>
#include "setup.h"
#include "MenuDisplay/MenuDisplay.h"
#include "AskServo/AskServo.h"
#include "PID/PID.h"
#include "DHT20.h"
#include "Trinity/Trinity.h"
#include "askBuzzer/askBuzzer.h"

#define SYSTEMSTATE_MENU                        1
#define SYSTEMSTATE_MANUALFLYING                2
#define SYSTEMSTATE_AUTOMATICFLYING             3
#define SYSTEMSTATE_SETUPAUTOMATIC              4
#define SYSTEMSTATE_EMERGENCY                   5
#define SYSTEMSTATE_TEST                        6
#define SYSTEMSTATE_VAREDIT_SELECTPOINTTOEDIT   7
#define SYSTEMSTATE_VAREDIT_GAMETIME            8
#define SYSTEMSTATE_VAREDIT_DRONESPEED          9
#define SYSTEMSTATE_POINTEDIT                   10
#define SYSTEMSTATE_VAREDIT_PID                 11
#define SYSTEMSTATE_TEMPERATUREEDIT             14
#define SYSTEMSTATE_VAREDIT_BRIGHTNESS          15

#define TEMPERATURE_MIN 15
#define TEMPERATURE_MAX 35

#define MAXENCODERSTEPS 1


#define MAXSERVOFRAMERATE 1

//Temp
uint8_t servoAngle = 90;
bool servoPolarity = 0;

//Variables
MenuDisplay   *menuDisplay;
DisplayData   *displayData;
AskButton     *button;
PID           *pid;
AskServo      *askServo;
Trinity       *trinity;
AskBuzzer     *askBuzzer;
int16_t       servoTime;
uint64_t      prevServoMillis;
uint8_t       displayMode;
ESP32Encoder  encoder;
int8_t        encoderValue;
uint8_t       powerLossTimer = 11;
uint32_t      lastMillis_PowerLoss = 0;
DHT20         DHT;
uint8_t       systemState = SYSTEMSTATE_MENU;
uint64_t      pidNew;
char          pidCurrent;
uint64_t      ledBrightnessOld;


//Settings
uint64_t ledEffect = 0;
uint64_t ledBrightness = 100;
bool autoClimate = true;
uint64_t goalTemperature = 21;
uint64_t goalTemperatureNew;
bool screenOff = false;
bool nightScreenOff = false;
bool sound = false;
uint8_t kProportional = 5;
uint8_t kIntegral = 1;
uint8_t kDerivative = 1;
uint64_t kProportional_display = 5;
uint64_t kIntegral_display = 1;
uint64_t kDerivative_display = 1;

//Sensors
float currentTemperature;
float currentHumidity;
bool dayBrightness;
bool carPowered;

/*=== === === MUTEX === === ===*/
//Audio
bool soundMutexTaken = false;
uint8_t buffered_sound_in;
uint8_t buffered_sound_mutex;
void playSound(uint8_t sound);
//Encoder
bool encoderMutexTaken = false;
int8_t buffered_encoder_in;
int8_t buffered_encoder_mutex;
void bufferEncoder(int8_t encoderValue);

//Tasks
TaskHandle_t            Task1;
TaskHandle_t            Task2;

void task_alpha(void *pvParameters);
void task_beta (void *pvParameters);

#endif