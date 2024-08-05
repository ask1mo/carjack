#include "Trinity.h"



//Constructor
Trinity::Trinity(uint8_t maxFramerate)
{
  Serial.println(F("Trinity MK3 - Ask Blommaert"));

  if(DEBUGLEVEL >= DEBUG_OPERATIONS)
  {
    Serial.print(F("Creating Trinity at adress "));
    Serial.println((int)this, DEC);
  }

  ledManager        = new LedManager(); //Todo: Made LedManager accept const ints

  setupComplete = false;

  

  setBrightness(BRIGHTNESS_3_MAX, true);
  prevFrameMillis = 0;
  frameTime = 1000/maxFramerate;

  Serial.print(F("...Trinity Initialised with frameRate of "));
  Serial.print(maxFramerate);
  Serial.print(F(" and frameTime of "));
  Serial.println(frameTime);

  Serial.println(F("...Trinity Started"));
}
void Trinity::addPanel(Panel *panel)
{
  ledManager->addPanel(panel);
}
void Trinity::begin()
{
  ledManager->begin();
  setupComplete = true;
  Serial.println(F("...Trinity Setup Finalised"));
}
//Private

//Public
void Trinity::tick()
{
  if (!setupComplete)
  {
    if (DEBUGLEVEL >= DEBUG_ERRORS)Serial.println(F("Trinity not setup yet"));
    return;
  }
  

  //Frame pushing
  uint64_t currentMillis = millis();
  if(currentMillis >= (prevFrameMillis+frameTime))
  {
    prevFrameMillis = currentMillis;

    
    ledManager->tick();
    ledManager->print();
  }
}
void Trinity::forceTick(uint16_t ticks, bool keepPrinting, uint16_t delayTime)
{
  for (uint16_t i = 0; i < ticks; i++)
  {
    if(DEBUGLEVEL >=DEBUG_DAYISRUINED) Serial.print(F("T"));
    ledManager->tick();

    if (keepPrinting)
    {
      if(DEBUGLEVEL >=DEBUG_DAYISRUINED) Serial.print(("P"));
      ledManager->print();

      if(delayTime > 0 )
      {
        delay(delayTime);
      }
    }
  }
}


void Trinity::setSpeed(uint8_t speed)
{
  ledManager->setSpeed(speed);
}
uint16_t Trinity::getDiodeAmount()
{
  return ledManager->getDiodeAmount();
}

void Trinity::setPanelVfx(uint8_t panelNumber, VFXData vfxData)
{
  ledManager->setPanelVfx(panelNumber, vfxData);
}
uint16_t Trinity::getPanelDiodeAmount(uint8_t panelNumber)
{
  return ledManager->getPanelDiodeAmount(panelNumber);
}
void Trinity::setPanelDiodeVfx(uint8_t panelNumber, uint16_t diodeNumber, VFXData vfxData)
{
  ledManager->setPanelDiodeVfx(panelNumber, diodeNumber, vfxData);
}
void Trinity::setPanelBrightness(uint8_t panelNumber, uint8_t brightness, bool smoothEnabled)
{
  ledManager->setPanelBrightness(panelNumber, brightness, smoothEnabled);
}

bool Trinity::getPanelEffectFinished(uint8_t panelNumber)
{
  return ledManager->getPanelEffectFinished(panelNumber);
}

void Trinity::setCustomPaletteColours(uint8_t slot, uint8_t colourRGBNumber, ColourRGB colourRGB)
{
  ledManager->setCustomPaletteColours(slot, colourRGBNumber, colourRGB);
}
void Trinity::setCustomPaletteAvailableColours(uint8_t slot, uint8_t avalaibleColours)
{
  ledManager->setCustomPaletteAvailableColours(slot, avalaibleColours);
}

uint8_t Trinity::getPanelAmount()
{
  return ledManager->getPanelAmount();
}

void Trinity::toggleFilter()
{
  ledManager->toggleFilter();
}

void Trinity::setAnimation_Atos_0() //Special Synth
{
  setSpeed(1);

  for (uint16_t i = 0; i < getPanelAmount(); i++)
  {
    setPanelVfx(i, (VFXData){EFFECT_STOCK_STATIC, COLOUR_BLACK, 0, 1, true});
  }
  forceTick(1, true, 0);

  uint8_t multiplier = 10;
  uint8_t diodesAmount;
  uint16_t offset;
  uint8_t speed = 3;


  //Right panel
  diodesAmount = getPanelDiodeAmount(0);
  offset = diodesAmount*multiplier;
  setPanelVfx(0, (VFXData){EFFECT_SPECIAL_SYNTH, COLOUR_BLACK, 100, 1, true});
  for (uint16_t j = 0; j < getPanelDiodeAmount(0); j++)
  {
    if (j < diodesAmount/2) offset -= multiplier;
    else offset += multiplier;
    setPanelDiodeVfx(0, j, (VFXData){EFFECT_SPECIAL_SYNTH, COLOUR_BLACK, offset, speed, true});
  }
  


  //Middle panel
  diodesAmount = getPanelDiodeAmount(1);
  offset = diodesAmount*multiplier;
  setPanelVfx(1, (VFXData){EFFECT_SPECIAL_SYNTH, COLOUR_BLACK, 0, 1, true});
  for (uint16_t j = 0; j < getPanelDiodeAmount(1); j++)
  {
    if (j < diodesAmount/2) offset -= multiplier;
    else offset += multiplier;
    setPanelDiodeVfx(1, j, (VFXData){EFFECT_SPECIAL_SYNTH, COLOUR_BLACK, offset, speed, true});
  }
  

  //Left panel
  diodesAmount = getPanelDiodeAmount(2);
  offset = diodesAmount*multiplier;
  setPanelVfx(2, (VFXData){EFFECT_SPECIAL_SYNTH, COLOUR_BLACK, 100, 1, true});
  for (uint16_t j = 0; j < getPanelDiodeAmount(2); j++)
  {
    if (j < diodesAmount/2) offset -= multiplier;
    else offset += multiplier;
    setPanelDiodeVfx(2, j, (VFXData){EFFECT_SPECIAL_SYNTH, COLOUR_BLACK, offset, speed, true});
  }

  forceTick(1000, false, 0);

  
  setPanelBrightness(0, 0, false);
  setPanelBrightness(0, BRIGHTNESS_3_MAX_VAL, true);
  setPanelBrightness(1, 0, false);
  setPanelBrightness(1, BRIGHTNESS_3_MAX_VAL, true);
  setPanelBrightness(2, 0, false);
  setPanelBrightness(2, BRIGHTNESS_3_MAX_VAL, true);


}

void Trinity::setAnimation_Atos_1() //Super Rainbow
{

  setSpeed(4);

  uint8_t multiplier = 10;
  uint8_t diodesAmount;
  uint16_t offset;
  uint8_t speed = 15;


  //Right panel
  diodesAmount = getPanelDiodeAmount(0);
  offset = diodesAmount*multiplier;
  setPanelVfx(0, (VFXData){EFFECT_SPECIAL_RAINBOW, COLOUR_BLACK, 50, 1, true});
  for (uint16_t j = 0; j < getPanelDiodeAmount(0); j++)
  {
    if (j < diodesAmount/2) offset -= multiplier;
    else offset += multiplier;
    setPanelDiodeVfx(0, j, (VFXData){EFFECT_SPECIAL_RAINBOW, COLOUR_BLACK, offset, speed, true});
  }

  //Middle panel
  diodesAmount = getPanelDiodeAmount(1);
  offset = diodesAmount*multiplier;
  setPanelVfx(1, (VFXData){EFFECT_SPECIAL_RAINBOW, COLOUR_BLACK, 0, 1, true});
  for (uint16_t j = 0; j < getPanelDiodeAmount(1); j++)
  {
    if (j < diodesAmount/2) offset -= multiplier;
    else offset += multiplier;
    setPanelDiodeVfx(1, j, (VFXData){EFFECT_SPECIAL_RAINBOW, COLOUR_BLACK, offset, speed, true});
  }

  //Left panel
  diodesAmount = getPanelDiodeAmount(2);
  offset = diodesAmount*multiplier;
  setPanelVfx(2, (VFXData){EFFECT_SPECIAL_RAINBOW, COLOUR_BLACK, 50, 1, true});
  for (uint16_t j = 0; j < getPanelDiodeAmount(2); j++)
  {
    if (j < diodesAmount/2) offset -= multiplier;
    else offset += multiplier;
    setPanelDiodeVfx(2, j, (VFXData){EFFECT_SPECIAL_RAINBOW, COLOUR_BLACK, offset, speed, true});
  }
}
void Trinity::setAnimation_Atos_2() //Red Decode
{
  setSpeed(10);
  uint8_t multiplier = 10;
  uint8_t diodesAmount;
  uint16_t offset;
  uint8_t speed = 1;


  //Right panel
  diodesAmount = getPanelDiodeAmount(0);
  offset = diodesAmount*multiplier;
  setPanelVfx(0, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, 10, 1, true});
  for (uint16_t j = 0; j < getPanelDiodeAmount(0); j++)
  {
    offset -= multiplier;
    setPanelDiodeVfx(0, j, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, offset, speed, true});
  }

  //Middle panel
  diodesAmount = getPanelDiodeAmount(1);
  offset = diodesAmount*multiplier;
  setPanelVfx(1, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, 0, 1, true});
  for (uint16_t j = 0; j < getPanelDiodeAmount(1); j++)
  {
    if (j < diodesAmount/2) offset -= multiplier;
    else offset += multiplier;
    setPanelDiodeVfx(1, j, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, offset, speed, true});
  }

  //Left panel
  diodesAmount = getPanelDiodeAmount(2);
  offset = 0;
  setPanelVfx(2, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, 10, 1, true});
  for (uint16_t j = 0; j < getPanelDiodeAmount(2); j++)
  {
    offset += multiplier;
    setPanelDiodeVfx(2, j, (VFXData){EFFECT_STOCK_DECODE, COLOUR_RED, offset, speed, true});
  }
}
void Trinity::setAnimation_Atos_3() //Static Cool
{
  setSpeed(1);

  setPanelVfx(0, (VFXData){EFFECT_STOCK_STATIC, COLOUR_RED,  0, 1, true});
  setPanelVfx(1, (VFXData){EFFECT_STOCK_STATIC, COLOUR_BLUE, 0, 1, true});
  setPanelVfx(2, (VFXData){EFFECT_STOCK_STATIC, COLOUR_RED,  0, 1, true});

}
void Trinity::setAnimation_Atos_4() //Static white
{
  setSpeed(1);

  for (uint16_t i = 0; i < getPanelAmount(); i++)
  {
    setPanelVfx(i, (VFXData){EFFECT_STOCK_STATIC, COLOUR_WHITE, 0, 1, true});
  }
}
void Trinity::setBrightness(uint8_t brightness, bool smoothEnabled)
{
  ledManager->setGoalBrightness(brightness, smoothEnabled);
}