#include "AskButton/AskButton.h"
#include <Arduino.h>
#include <Wire.h>
#include <ESP32Encoder.h>
#include "setup.h"
#include "MenuDisplay/MenuDisplay.h"
#include "AskServo/AskServo.h"
#include "PID/PID.h"
#include "DHT20.h"
#include "Trinity/Trinity.h"
#include "AskBuzzer/AskBuzzer.h"

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


#define SERVO_MIN 0
#define SERVO_MAX 180
#define MAXSERVOFRAMERATE 1

//Temp
uint8_t servoAngle = 90;
bool servoPolarity = 0;

//Variables
MenuDisplay   *menuDisplay;
DisplayData   *displayData;
AskButton     *button;
PID           *pid;
//AskServo      *askServo;
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


void cycleLedEffect()
{
  ledEffect++;
  if (ledEffect > 4) ledEffect = 0;

  switch (ledEffect)
  {
    case 0:
    {
      trinity->setAnimation_Atos_0();
    }
    break;
    case 1:
    {
      trinity->setAnimation_Atos_1();
    }
    break;
    case 2:
    {
      trinity->setAnimation_Atos_2();
    }
    break;
    case 3:
    {
      trinity->setAnimation_Atos_3();
    }
    break;
    case 4:
    {
      trinity->setAnimation_Atos_4();
    }
    break;
  }
}
void toggleAutoClimate() 
{
  autoClimate = !autoClimate;
  //askServo->setEnabled(autoClimate);
}
void toggleSound()
{
  sound = !sound;
}
void cycleLEDEffect()
{
  ledEffect++;
}
void engageScreenSaver()
{
  menuDisplay->startScreensaver();
}
void toggleNightScreen()
{
  nightScreenOff = !nightScreenOff;
}
void toggleScreenOff()
{
  screenOff = !screenOff;
}
void setGoalTemperature(uint8_t temperature)
{
  goalTemperature = temperature;
  pid->setSetpoint(goalTemperature);
}
void setBrightness(uint64_t brightness)
{
  ledBrightness = brightness;
  trinity->setBrightness(map(brightness, 0, 100, 0, 255), false);
}
void enterMode_Menu()
{
  Serial.println(F("Mode switch: Menu"));
  systemState = SYSTEMSTATE_MENU;
  menuDisplay->doMenuInteraction(MENUINTERACTION_MENU_OPEN);
  askBuzzer->playSound(SOUND_BUTTON_CLICK);
}
void enterMode_VarEdit_Temperature_Yes()
{
  Serial.println(F("Mode switch: Leave Temperature (YES)"));

  setGoalTemperature((uint8_t)goalTemperatureNew);

  enterMode_Menu();
}
void enterMode_VarEdit_Temperature_No()
{
  Serial.println(F("Mode switch: Leave game time (No)"));

  enterMode_Menu();
}
void enterMode_VarEdit_Temperature()
{
  Serial.println(F("Mode switch: Temperature Edit"));

  goalTemperatureNew = (uint64_t)goalTemperature;

  menuDisplay->openDialogBox_Variable(false, "Temperature", &goalTemperatureNew, &enterMode_VarEdit_Temperature_Yes, &enterMode_VarEdit_Temperature_No);

  
  systemState = SYSTEMSTATE_TEMPERATUREEDIT;
}
void enterMode_VarEdit_PID_Yes()
{
  Serial.println(F("Mode switch: Leave PID (YES)"));

  switch (pidCurrent)
  {
    case 'P':
    kProportional = (uint8_t)pidNew;
    break;

    case 'I':
    kIntegral = (uint8_t)pidNew;
    break;

    case 'D':
    kDerivative = (uint8_t)pidNew;
    break;
  }

  kProportional_display  =  (uint64_t) kProportional;
  kIntegral_display      =  (uint64_t) kIntegral;
  kDerivative_display    =  (uint64_t) kDerivative;
  menuDisplay->forceTick();

  enterMode_Menu();
}
void enterMode_VarEdit_PID_No()
{
  Serial.println(F("Mode switch: Leave game time (No)"));

  enterMode_Menu();
}
void enterMode_VarEdit_PID_KP()
{
  Serial.println(F("Mode switch: KP Edit"));

  pidCurrent = 'P';
  pidNew = (uint64_t)kProportional;

  menuDisplay->openDialogBox_Variable(false, "Proportional", &pidNew, &enterMode_VarEdit_PID_Yes, &enterMode_VarEdit_PID_No);

  
  systemState = SYSTEMSTATE_VAREDIT_PID;
}
void enterMode_VarEdit_PID_KI()
{
  Serial.println(F("Mode switch: KI Edit"));

  pidCurrent = 'I';
  pidNew = (uint64_t)kIntegral;

  menuDisplay->openDialogBox_Variable(false, "Integral", &pidNew, &enterMode_VarEdit_PID_Yes, &enterMode_VarEdit_PID_No);

  
  systemState = SYSTEMSTATE_VAREDIT_PID;
}
void enterMode_VarEdit_PID_KD()
{
  Serial.println(F("Mode switch: KD Edit"));

  pidCurrent = 'D';
  pidNew = (uint64_t)kDerivative;

  menuDisplay->openDialogBox_Variable(false, "Derivative", &pidNew, &enterMode_VarEdit_PID_Yes, &enterMode_VarEdit_PID_No);

  
  systemState = SYSTEMSTATE_VAREDIT_PID;
}
void enterMode_VarEdit_Brightness_Yes()
{
  Serial.println(F("Mode switch: Leave Brightness (YES)"));

  enterMode_Menu();
}
void enterMode_VarEdit_Brightness_No()
{
  Serial.println(F("Mode switch: Leave Brightness (No)"));

  ledBrightness = ledBrightnessOld;
  trinity->setBrightness(ledBrightness, false);

  enterMode_Menu();
}

void enterMode_VarEdit_Brightness()
{
  Serial.println(F("Mode switch: Brightness Edit"));

  menuDisplay->openDialogBox_Variable(false, "Brightness", &ledBrightness, &enterMode_VarEdit_Brightness_Yes, &enterMode_VarEdit_Brightness_No);

  
  systemState = SYSTEMSTATE_VAREDIT_BRIGHTNESS;
}

void handleMode_Menu()
{
  if (encoderValue > 0)
  {
    for (int i = 0; i < encoderValue; i++)
    {
      menuDisplay->doMenuInteraction(MENUINTERACTION_PREVIOUS);
      askBuzzer->playSound(SOUND_BUTTON_BACKWARD);
      menuDisplay->tick(); 
    }
  }
  if (encoderValue < 0)
  {
    for (int i = 0; i < abs(encoderValue); i++)
    {
      menuDisplay->doMenuInteraction(MENUINTERACTION_NEXT);
      askBuzzer->playSound(SOUND_BUTTON_FORWARD);
      menuDisplay->tick(); 
    }
  }
  

  switch (button->getCommand())
  {
    case BUTTON_TAPPED:
      menuDisplay->doMenuInteraction(MENUINTERACTION_ENTER);
      askBuzzer->playSound(SOUND_BUTTON_CLICK);
      Serial.println("Tapped");
      menuDisplay->tick();
      break;
    case BUTTON_HELD:
      menuDisplay->doMenuInteraction(MENUINTERACTION_EXIT);
      askBuzzer->playSound(SOUND_BUTTON_CLICK);
      Serial.println("Held");
      menuDisplay->tick();
      break;
  } 
}

void handleMode_VariableEdit_PID()
{
  if (encoderValue > 0)
  {
    if (pidNew < 20) pidNew++;
    menuDisplay->keepAwake();
    menuDisplay->forceTick();
  }
  if (encoderValue < 0)
  {
    if (pidNew > 0) pidNew--;
    menuDisplay->keepAwake();
    menuDisplay->forceTick();
  }

  encoderValue = 0;
  handleMode_Menu();
}
void handleMode_TemperatureEdit()
{
  if (encoderValue > 0)
  {
    if (systemState != SYSTEMSTATE_TEMPERATUREEDIT) //Special screensaver case
    {
      if (goalTemperature < TEMPERATURE_MAX) setGoalTemperature((goalTemperature+=1));
    }
    else 
    {
      if (goalTemperatureNew < TEMPERATURE_MAX) goalTemperatureNew++;
      menuDisplay->keepAwake();
    }
    menuDisplay->forceTick();
  }
  if (encoderValue < 0)
  {
    if (systemState != SYSTEMSTATE_TEMPERATUREEDIT) //Special screensaver case
    {
      if (goalTemperature > TEMPERATURE_MIN) setGoalTemperature((goalTemperature-=1));
    }
    else
    {
      if (goalTemperatureNew > TEMPERATURE_MIN) goalTemperatureNew--;
      menuDisplay->keepAwake();
    }
    menuDisplay->forceTick();
  }

  encoderValue = 0;
  handleMode_Menu();
}
void handleMode_VariableEdit_Brightness()
{
  if (encoderValue > 0)
  {
    if (ledBrightness < 100) setBrightness(ledBrightness+5);
    menuDisplay->keepAwake();
    menuDisplay->forceTick();
    
  }
  if (encoderValue < 0)
  {
    if (ledBrightness > 0) setBrightness(ledBrightness-5);
    menuDisplay->keepAwake();
    menuDisplay->forceTick();
  }

  encoderValue = 0;
  handleMode_Menu();
}

void doDisplayShit()
{
  if (menuDisplay->isDialogBoxOpen())
  {
    switch (systemState)
    {
      case SYSTEMSTATE_VAREDIT_PID:
      {
        handleMode_VariableEdit_PID();
      }
      break;
      case SYSTEMSTATE_TEMPERATUREEDIT:
      {
        handleMode_TemperatureEdit();
      }
      break;
      case SYSTEMSTATE_VAREDIT_BRIGHTNESS:
      {
        handleMode_VariableEdit_Brightness();
      }
      break;

      default:
      {
        handleMode_Menu();
      }
      break;
    }
    menuDisplay->tick();
    return;
  }
  switch (systemState)
  {
    case SYSTEMSTATE_MENU:
    {
      handleMode_Menu();
    }
    break;
  }

  if (menuDisplay->isScreensaverActive())
  {
    handleMode_TemperatureEdit();
  }

  menuDisplay->tick();
}

void setup()
{
  Serial.begin(115200);
  //Battery backup relay
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, HIGH);
  //Voltage sensor
  pinMode(PIN_VOLTAGESENSOR, INPUT);
  //Encoder
  encoder.attachSingleEdge(PIN_ENCODER_B, PIN_ENCODER_A);
  encoder.clearCount();
  
  button = new AskButton(PIN_BUTTON, 300);
  //Leds
  pinMode(PIN_LEDS, OUTPUT);
  //PWM servo
  servoTime = 1000/MAXSERVOFRAMERATE;
  //askServo = new AskServo(PIN_SERVO, SERVO_MIN, SERVO_MAX, 1000);
  //askServo->setGoalCoords(0);
  //PID
  pid = new PID(&kProportional, &kIntegral, &kDerivative, SERVO_MIN, SERVO_MAX);
  pid->setSetpoint(21);
  //DHT
  Wire.begin(PIN_SCL, PIN_SDA);
  uint8_t returnvak = DHT.begin();
  Serial.print(F("DHT20: "));
  Serial.println(returnvak);





  //Menu display
  MenuItem_Folder *menuItemFolder_Root_Main            =new MenuItem_Folder ("Root_Main",       sprite40_warning,         NULL, NULL,                               "", ""              );
    MenuItem_Folder *menuItemFolder_LEDSettings        =new MenuItem_Folder ("Lights",          sprite40_diode,           NULL, NULL,                               "", ""              );
    menuItemFolder_Root_Main              ->addMenuItem(menuItemFolder_LEDSettings                                                                                                      );
      menuItemFolder_LEDSettings          ->addMenuItem(new MenuItem_U64    ("Effect",          sprite40_brightness_vfx,  &cycleLedEffect,                          "", &ledEffect      ));
      menuItemFolder_LEDSettings          ->addMenuItem(new MenuItem_U64    ("Brightness",      sprite40_brightness,      &enterMode_VarEdit_Brightness,            "", &ledBrightness  ));
    MenuItem_Folder *menuItemFilder_Climate             =new MenuItem_Folder("Climate",         sprite40_thermometer,     NULL, NULL,                               "", ""              );
    menuItemFolder_Root_Main              ->addMenuItem(menuItemFilder_Climate                                                                                                          );
      menuItemFilder_Climate              ->addMenuItem(new MenuItem_U64    ("Temperature",     sprite40_airco,           &enterMode_VarEdit_Temperature,           "", &goalTemperature));
      menuItemFilder_Climate              ->addMenuItem(new MenuItem_Bool   ("Auto Climate",    sprite40_fans,            &toggleAutoClimate,                       "", &autoClimate    ));
      MenuItem_Folder *menuItemFolder_PIDSettings      =new MenuItem_Folder ("PID",             sprite40_gear,            NULL, NULL,                               "", ""              );
      menuItemFilder_Climate              ->addMenuItem(menuItemFolder_PIDSettings                                                                                                      );
        menuItemFolder_PIDSettings        ->addMenuItem(new MenuItem_U64    ("KP",              sprite40_gear,            &enterMode_VarEdit_PID_KP,                "", &kProportional_display  ));
        menuItemFolder_PIDSettings        ->addMenuItem(new MenuItem_U64    ("KI",              sprite40_gear,            &enterMode_VarEdit_PID_KI,                "", &kIntegral_display    ));
        menuItemFolder_PIDSettings        ->addMenuItem(new MenuItem_U64    ("KD",              sprite40_gear,            &enterMode_VarEdit_PID_KD,                "", &kDerivative_display    ));
    MenuItem_Folder *menuItemFolder_DisplaySettings    =new MenuItem_Folder ("Display",         sprite40_tvScreen,        NULL, NULL,                               "", ""              );
    menuItemFolder_Root_Main              ->addMenuItem(menuItemFolder_DisplaySettings                                                                                                  );
      menuItemFolder_DisplaySettings      ->addMenuItem(new MenuItem_Bool   ("Screen Off",      sprite40_onOff,           &toggleScreenOff,                         "", &screenOff      ));
      menuItemFolder_DisplaySettings      ->addMenuItem(new MenuItem_Bool   ("Night Screen Off",sprite40_moon,            &toggleNightScreen,                       "", &nightScreenOff ));
    menuItemFolder_Root_Main              ->addMenuItem(new MenuItem_Bool   ("Sound",           sprite40_sounds,          &toggleSound,                             "", &sound          ));
    
  displayData = new struct DisplayData();
  menuDisplay = new MenuDisplay(menuItemFolder_Root_Main, GRAPHICSTYPE_SPRITES, displayData);

  displayData->ledEffect          = &ledEffect;
  displayData->ledBrightness      = &ledBrightness;
  displayData->autoClimate        = &autoClimate;
  displayData->goalTemperature    = &goalTemperature;
  displayData->screenOff          = &screenOff;
  displayData->nightScreenOff     = &nightScreenOff;
  displayData->sound              = &sound;
  displayData->currentTemperature = &currentTemperature;
  displayData->currentHumidity    = &currentHumidity;
  displayData->dayBrightness      = &dayBrightness;
  displayData->carPowered         = &carPowered;
  displayData->powerLossTimer     = &powerLossTimer;

  menuDisplay->tick();


  //Leds
  trinity = new Trinity(30);

  trinity->addPanel(new Panel(0, 0, 0, CLOCK_COUNTERWISE, COMPASS_WEST, 15));
  trinity->addPanel(new Panel(0, 0, 0, CLOCK_COUNTERWISE, COMPASS_WEST, 8));
  trinity->addPanel(new Panel(0, 0, 0, CLOCK_COUNTERWISE, COMPASS_WEST, 15));

  trinity->begin(); 
  trinity->setAnimation_Atos_0();

  setBrightness(100);

  askBuzzer               = new AskBuzzer(PIN_BUZZER);
  askBuzzer->playSound(SOUND_BOOTUP);
}
void loop()
{
  uint64_t currentMillis = millis();

  //Encoder
  encoderValue = encoder.getCount();
  encoder.setCount(0); //Reset encoder

  //Auto shutoff
  carPowered = digitalRead(PIN_VOLTAGESENSOR);
  if (carPowered)
  {
    powerLossTimer = 11;
    digitalWrite(PIN_RELAY, HIGH);
  }
  else
  {
    //Millis 10 second countdown
    if (millis() - lastMillis_PowerLoss > 1000)
    {
      if (powerLossTimer > 0)
      {
        powerLossTimer--;
        lastMillis_PowerLoss = millis();
      }
      if (powerLossTimer == 0)
      {
        digitalWrite(PIN_RELAY, LOW);
      }
    }
  }

  //Display
  doDisplayShit();


  //Thermometer, PID & Servo
  if(currentMillis >= (prevServoMillis+servoTime))
  {
    prevServoMillis = currentMillis;

    int status = DHT.read();
    currentTemperature = DHT.getTemperature();
    currentHumidity = DHT.getHumidity();

    

    //askServo->setGoalCoords(pid->calculate(currentTemperature));


    //askServo->tick();
     //Serial.print(F(" Servo is at: "));
    //Serial.println(askServo->getCurrentCoords());
  }

  //Leds
  trinity->tick();

  //Soound
  askBuzzer->tick();

}