#include "main.h"


void switchOperatingMode()
{
  if (operatingMode == CCMODE)
  {
    operatingMode = MENUMODE;
    Serial.println("Switched to MENUMODE");
  }
  else if (operatingMode == MENUMODE)
  {
    operatingMode = CCMODE;
    Serial.println("Switched to CCMODE");
  }
}

void pollRotaryEncoder()
{
  //CC: Left/right to decrease/increase speed
  //Menu: Left/right to navigate menu
  if (buffered_encoder != 0)
  {
    if (operatingMode == CCMODE)
    {
      switch (cruiseControl.getRotaryMode())
      {
        case CCRotaryMode::SPEED:
          //Cruise Control Mode - Adjust Speed
          Serial.print("Adjusting target speed by ");
          Serial.println(buffered_encoder);
          cruiseControl.addTargetSpeed(buffered_encoder);
        break;
        case CCRotaryMode::SETTINGS:
        {
          if (buffered_encoder > 0) cruiseControl.cycleResolution();
          else if (buffered_encoder < 0) cruiseControl.cycleAcceleration();
        }
        break;
        default:
          //Locked - do nothing
          Serial.println("Wheel locked - no action");
        break;
      }
    }
    else if (operatingMode == MENUMODE)
    {
      //Menu Mode
      if (buffered_encoder > 0) dataLink.transmit_MenuInteraction(MenuInteraction::RIGHT);
      else if (buffered_encoder < 0) dataLink.transmit_MenuInteraction(MenuInteraction::LEFT);
    }
    buffered_encoder = 0;
  }
}
  

void pollButton()
{
  //Button
    switch (button->getCommand())
    {
      case BTN_TPD: //Short press: Enable or disable cruise control
      {
        Serial.println("BTN_TPD");
        if (operatingMode != MENUMODE) cruiseControl.cycleOperatingMode();
        dataLink.transmit_MenuInteraction(MenuInteraction::ENTER);
      }
      break;

      
      case BTN_HLD_1S:
      {
        Serial.println("BTN_HLD_1S");
        if (operatingMode == CCMODE) cruiseControl.loadMemSpeed();
        else if (operatingMode == MENUMODE) dataLink.transmit_MenuInteraction(MenuInteraction::EXIT);
      }
      break;

      case BTN_HLD_3S:
      {
        Serial.println("BTN_HLD_3S");
        
        if (operatingMode == CCMODE) cruiseControl.setMemSpeed(vehicle.getData().vehicleSpeed);
        // No action for MENUMODE
      }
      break;

      case BTN_HLD_5S:
      {
        Serial.println("BTN_HLD_5S");
        switchOperatingMode();
      }
    }
}










/*=== === === SYSTEM === === ===*/
void setup()
{
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
void loop() //Deprecated, replaced by tasks
{
  //Serial.print("Error: main.loop got called");
}

/*=== === === MAIN === === ===*/
void task_alpha( void *pvParameters ) //Multicore replacement for "loop()"
{
  //Setup
  Serial.println("----- ===== Task Alpha Setup Started ===== -----");

  //Encoder
  //encoder.attachSingleEdge(PIN_ENCODER_B, PIN_ENCODER_A);
  //encoder.clearCount();
  button = new AskButton(PIN_BUTTON, 400);




  Serial.println("----- ===== Task Alpha Setup Complete ===== -----");

  
  //Loop
  while (1)
  {
    uint64_t currentMillis = millis();

    //Encoder
    //buffered_encoder_in += encoder.getCount();

    if (buffered_encoder_in != 0)
    {
      buffered_encoder += buffered_encoder_in;
      buffered_encoder_in = 0;

      if (buffered_encoder > MAXENCODERSTEPS) buffered_encoder = MAXENCODERSTEPS;
      if (buffered_encoder < -MAXENCODERSTEPS) buffered_encoder = -MAXENCODERSTEPS;
    }
    pollRotaryEncoder();
    pollButton();

    if (!OBD2MutexTaken)
    {
      OBD2MutexTaken = true;
      cruiseControl.updateVehicleData(obdData);
      OBD2MutexTaken = false;
    }

    cruiseControl.loop();
    dataLink.transmit_Data(cruiseControl.getDataFrame());
  

    vTaskDelay(1);
  }
}
void task_beta( void *pvParameters)
{

  Serial.println("Connected to ELM327");

  while (1)
  {
    vehicle.loop();

    if(!OBD2MutexTaken)
    {
      OBD2MutexTaken = true;
      obdData = vehicle.getData();
      OBD2MutexTaken = false;
    }


    vTaskDelay(100);
  }
}

