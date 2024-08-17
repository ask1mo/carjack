#include "main.h"

/*=== === === ALPHA === === ===*/
//Display functions
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
  askServo->setEnabled(autoClimate);
}
void toggleSound()
{
  sound = !sound;
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
  //playSound(SOUND_BUTTON_FORWARD); //Click sound
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
//Display Handling
void handleMode_Menu()
{
  if (encoderValue > 0)
  {
    for (int i = 0; i < encoderValue; i++)
    {
      menuDisplay->doMenuInteraction(MENUINTERACTION_PREVIOUS);
      playSound(SOUND_BUTTON_CLICK); //Scroll previous sound
      menuDisplay->tick(); 
    }
  }
  if (encoderValue < 0)
  {
    for (int i = 0; i < abs(encoderValue); i++)
    {
      menuDisplay->doMenuInteraction(MENUINTERACTION_NEXT);
      playSound(SOUND_BUTTON_CLICK); //Scroll next sound
      menuDisplay->tick(); 
    }
  }

  
  

  switch (button->getCommand())
  {
    case BUTTON_TAPPED:
      menuDisplay->doMenuInteraction(MENUINTERACTION_ENTER);
      playSound(SOUND_BUTTON_FORWARD);
      Serial.println("Tapped");
      menuDisplay->tick();
      break;
    case BUTTON_HELD:
      menuDisplay->doMenuInteraction(MENUINTERACTION_EXIT);
      playSound(SOUND_BUTTON_BACKWARD);
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
//Functions
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

/*=== === === MUTEX === === ===*/
void playSound(uint8_t sound)
{
  buffered_sound_in = sound;
}
void checkNew_playSound_in()
{
  if (soundMutexTaken) return;
  
  soundMutexTaken = true;
  if (buffered_sound_in != 0)
  {
    buffered_sound_mutex = buffered_sound_in;
    buffered_sound_in = 0;
  }
  soundMutexTaken = false;
  
}
void checkNew_playSound_out()
{
  if (soundMutexTaken) return;
  
  soundMutexTaken = true;
  if (buffered_sound_mutex != 0)
  {
    askBuzzer->playSound(buffered_sound_mutex);
    buffered_sound_mutex = 0;
  }
  soundMutexTaken = false;  
}

void bufferEncoder(int8_t encoderVal)
{
  buffered_encoder_in += encoderVal;
}
void checkNew_encoder_in()
{
  if (encoderMutexTaken) return;
  
  encoderMutexTaken = true;
  if (buffered_encoder_in != 0)
  {
    buffered_encoder_mutex += buffered_encoder_in;
    buffered_encoder_in = 0;

    if (buffered_encoder_mutex > MAXENCODERSTEPS) buffered_encoder_mutex = MAXENCODERSTEPS;
    if (buffered_encoder_mutex < -MAXENCODERSTEPS) buffered_encoder_mutex = -MAXENCODERSTEPS;
  }
  encoderMutexTaken = false;
}
void checkNew_encoder_out()
{
  if (encoderMutexTaken) return;
  
  encoderMutexTaken = true;
  if (buffered_encoder_mutex != 0)
  {
    encoderValue = buffered_encoder_mutex;
    buffered_encoder_mutex = 0;
  }
  encoderMutexTaken = false;
}

/*=== === === BETA === === ===*/

void setup()
{
  pinMode(PIN_RELAY, INPUT_PULLUP); //Quick pin high for relay

  Serial.begin(115200);
  Serial.println(F("----- ===== Setup started ===== -----"));

  xTaskCreatePinnedToCore
  (
    task_alpha,       /* Task function. */
    "task_main",         /* name of task. */
    10000,           /* Stack size of task */
    NULL,            /* parameter of the task */
    1,               /* priority of the task */
    &Task1,          /* Task handle to keep track of created task */
    0                /* pin task to core 0 */
  );              

  xTaskCreatePinnedToCore
  (
    task_beta,       /* Task function. */
    "task_leds",         /* name of task. */
    10000,           /* Stack size of task */
    NULL,            /* parameter of the task */
    1,               /* priority of the task */
    &Task2,          /* Task handle to keep track of created task */
    1               /* pin task to core 1 */
  ); 
}
void loop()
{
  

}
void task_alpha( void *pvParameters ) //Multicore replacement for "loop()"
{
  //Setup
  Serial.println("----- ===== Task Alpha Setup Started ===== -----");
  //Battery backup relay
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, HIGH);
  //Voltage sensor
  pinMode(PIN_VOLTAGESENSOR, INPUT);
  
  
  button = new AskButton(PIN_BUTTON, 400);
  //Leds
  pinMode(PIN_LEDS, OUTPUT);
  //PWM servo
  servoTime = 1000/MAXSERVOFRAMERATE;
  askServo = new AskServo(PIN_SERVO, PIN_ENDSTOP_MIN, PIN_ENDSTOP_MAX, SERVO_MIN, SERVO_MAX);
  pinMode(PIN_ENDSTOP_MIN, INPUT_PULLUP);
  pinMode(PIN_ENDSTOP_MAX, INPUT_PULLUP);
  pinMode(PIN_MANUALOVERIDE_SERVO, INPUT_PULLUP);
  //PID
  pid = new PID(&kProportional, &kIntegral, &kDerivative, SERVO_MIN, SERVO_MAX);
  pid->setSetpoint(21);
  //DHT
  Wire.begin(PIN_SDA, PIN_SCL);
  uint8_t returnvak = DHT.begin();
  Serial.print(F("DHT20: "));
  Serial.println(returnvak);
  //LDR
  pinMode(PIN_LDR, INPUT);





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

  
  Serial.println("----- ===== Task Alpha Setup Complete ===== -----");
  
  //Loop
  while (1)
  {
    uint64_t currentMillis = millis();

    //Encoder
    encoderValue = 0;
    checkNew_encoder_out();
    

    //Auto shutoff
    bool carPoweredNew = digitalRead(PIN_VOLTAGESENSOR);
    if (carPowered)
    {
      if (carPoweredNew)
      {
        powerLossTimer = 11;
        digitalWrite(PIN_RELAY, HIGH);
        //playSound(SOUND_BUTTON_CLICK);
        menuDisplay->forceTick();
      }
      else
      {
        playSound(SOUND_BUTTON_CLICK);
      }
    }
    else
    {
      //Millis 10 second countdown
      if (millis() - lastMillis_PowerLoss > 1000)
      {
        menuDisplay->forceTick();

        if (powerLossTimer > 0)
        {
          powerLossTimer--;
          lastMillis_PowerLoss = millis();
          playSound(SOUND_SHUTDOWN);
        }
        if (powerLossTimer == 0)
        {
          digitalWrite(PIN_RELAY, LOW);
        }
      }
    }
    carPowered = carPoweredNew;

    //Display
    doDisplayShit();


    //Thermometer, PID & Servo

    
    if (digitalRead(PIN_ENDSTOP_MIN) == LOW) Serial.println("Endstop min");
    if (digitalRead(PIN_ENDSTOP_MAX) == LOW) Serial.println("Endstop max");
    if (digitalRead(PIN_MANUALOVERIDE_SERVO) == LOW) Serial.println("Manual override");

    if(currentMillis >= (prevServoMillis+servoTime))
    {
      prevServoMillis = currentMillis;

      int status = DHT.read();
      currentTemperature = DHT.getTemperature();
      currentHumidity = DHT.getHumidity();

      

      askServo->setVelocity(pid->calculate(currentTemperature));


      askServo->tick();
    }

    //Leds
    trinity->tick();

    //Sound
    checkNew_playSound_in(); //Put new sounds in buffer for other core
    
    //LDR
    float dingus = analogRead(PIN_LDR);
    //Serial.print(F("LDR: "));
    //Serial.println(dingus);

    vTaskDelay(1);
  }
}
void task_beta( void *pvParameters )
{
  //Setup
  Serial.println("----- ===== Task Beta Setup Started ===== -----");

  //Encoder
  encoder.attachSingleEdge(PIN_ENCODER_B, PIN_ENCODER_A);
  encoder.clearCount();

  askBuzzer               = new AskBuzzer(PIN_BUZZER);
  playSound(SOUND_BOOTUP);

  Serial.println("----- ===== Task Beta Setup Complete ===== -----");
  
  //Loop
  while (1)
  {
    //Encoder
    bufferEncoder(encoder.getCount());
    encoder.setCount(0); //Reset ncoder
    checkNew_encoder_in(); //Put new encoder values in buffer for other core



    //Sound
    checkNew_playSound_out(); //Play sounds from buffer
    askBuzzer->tick();
    vTaskDelay(1);
  }
}