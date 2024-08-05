#include "MenuDisplay.h"

MenuDisplay::MenuDisplay(MenuItem_Folder *rootFolder, uint8_t graphicsType, DisplayData *displayData) // Should also contain tiles per screen
{
  Serial.println(F("Constructing new menu."));
  this->rootFolder    = rootFolder;
  this->graphicsType  = graphicsType;
  this->displayData   = displayData;
  currentFolder = rootFolder;
  currentMenuItem = currentFolder->getSubItem(0);
  screensaverEnabled = true;
  activeScreen = SCREEN_SCREENSAVER;
  lastActiveScreen = SCREEN_MENU;
  currentIndex = 0;
  indexOffset = 0;
  enabled = true;
  lastScreenupdateTime = 0;
  continousFramerateMode = false;
  allowScreensaver = true;
  lastInteractionTime = 0;

  dialogBoxOpen = false;
  dialogBoxText = "DialogBox";
  dialogBoxIndex = 0;

  bootMessageLineIndex = 0;

  Serial.println(F("MenuDisplay Started, Display Starting..."));

  //display = new U8G2_SSD1309_128X64_NONAME0_F_4W_HW_SPI(U8G2_R0, /* cs=*/ PIN_SPI_CS, /* dc=*/ PIN_SPI_DC, /* reset=*/ PIN_SPI_RST);
  display = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 32, /* data=*/ 33);
  display->begin();
  display->setFont(u8g2_font_6x10_tf);
  display->setFontRefHeightExtendedText();
  display->setDrawColor(1);
  display->setFontPosTop();
  display->setFontDirection(0);

  printScreen_Boot();

  display->sendBuffer();
  
  Serial.println(F("Display Started"));
}

//Private
void MenuDisplay::navigateNext()
{
    if(currentIndex == 0 )
    {
      currentIndex = currentFolder->getMenuItemsLength();
      indexOffset = (currentFolder->getMenuItemsLength() - MENU_MAXINDEX);
      if (currentFolder->getMenuItemsLength() < MENU_MAXINDEX)
      {
        indexOffset = 0;
      }
    }
    currentIndex--;

    if (currentIndex < indexOffset)
    {
      indexOffset--;
    }
    currentMenuItem = currentFolder->getSubItem(currentIndex);
}
void MenuDisplay::navigatePrevious()
{

    currentIndex++;
    if(currentIndex == currentFolder->getMenuItemsLength())
    {
      currentIndex = 0;
      indexOffset = 0;
    }
    
    if (currentIndex == (indexOffset+MENU_MAXINDEX))
    {
      indexOffset++;
    }
    currentMenuItem = currentFolder->getSubItem(currentIndex);
  
}
void MenuDisplay::navigateEnter()
{

    if (currentFolder->clickContainedItem(currentIndex) == SIGNATURE_FOLDER)
    {
      Serial.print(F("Folder detected."));
      currentFolder->setLastIndex(currentIndex);
      currentFolder->setLastIndexOffset(indexOffset);
      MenuItem_Folder *newFolder = currentFolder->getSubFolder(currentIndex);
      newFolder->setParentFolder(currentFolder);
      currentFolder = newFolder;
      indexOffset = 0;
      currentIndex = 0;
      //Serial.println(F("Folder Opened"));
      currentMenuItem = currentFolder->getSubItem(currentIndex);
    }
    //Files dont need any code
    Serial.println(F("File detected."));
  
}
void MenuDisplay::navigateExit()
{

    Serial.println(F("MenuDisplay: Exiting folder."));
    Serial.println(F("MenuDisplay: Checking if root folder."));

    if (currentFolder == rootFolder)
    {
      startScreensaver();
    }
    else
    {
      Serial.println(F("MenuDisplay: Not root folder, exiting."));
      currentFolder->notifyDeselcted();
      Serial.println(F("MenuDisplay: Notifying parent folder."));
      currentFolder = currentFolder->getParentFolder();
      Serial.println(F("MenuDisplay: Setting current folder."));
      currentIndex = currentFolder->getLastIndex();
      Serial.println(F("MenuDisplay: Setting current index."));
      indexOffset = currentFolder->getLastIndexOffset();
      Serial.println(F("MenuDisplay: Setting index offset."));
    }
    //currentIndex = 0;
    //indexOffset = 0;
    currentMenuItem = currentFolder->getSubItem(currentIndex);
  
  Serial.println(F("MenuDisplay: Folder exited."));
}


void MenuDisplay::resolveDialogBox_yes()
{
  Serial.println(F("MenuDisplay: Dialog box yes selected."));
  dialogBoxOpen = false;

  switch (dialogBoxType)
  {
    case DIALOGTYPE_INTERNAL:
    if (dialogFunction_InternalYes != NULL) (this->*dialogFunction_InternalYes)();
    break;

    case DIALOGTYPE_EXTERNAL:
    if (dialogFunctionYes != NULL) dialogFunctionYes();
    break;

    case DIALOGTYPE_MESSAGE:
    if (dialogFunctionYes != NULL) dialogFunctionYes();
    break;
  }
}
void MenuDisplay::resolveDialogBox_no()
{
  Serial.println(F("MenuDisplay: Dialog box no selected."));
  dialogBoxOpen = false;

  switch (dialogBoxType)
  {
    case DIALOGTYPE_INTERNAL:
    if (dialogFunction_InternalNo != NULL) (this->*dialogFunction_InternalNo)();
    break;

    case DIALOGTYPE_EXTERNAL:
    if (dialogFunctionNo != NULL) dialogFunctionNo();
    break;

    case DIALOGTYPE_MESSAGE:
    if (dialogFunctionNo != NULL) dialogFunctionNo();
    break;
  }

  currentFolder->setSubItemSelected(currentIndex, false);
}

void MenuDisplay::print()
{
  display->clearBuffer();
  switch (activeScreen)
  {
    case SCREEN_SCREENSAVER:
    printScreen_Screensaver();
    break;

    case SCREEN_REBOOT:
    printScreen_Reboot();
    break;
  
    case SCREEN_MENU:
    printScreen_Menu();
    break;
    
    case SCREEN_VAREDIT_SELECTPOINTTOEDIT:
    display->setCursor(0,0);
    display->println(F("EMPTY: Select point to edit"));
    break; //Empty black screen

    case SCREEN_VAREDIT_GAMETIME:
    printScreen_Varedit_GameTime();
    break; //Empty black screen

    case SCREEN_VAREDIT_DRONESPEED:
    display->setCursor(0,0);
    display->println(F("EMPTY: Drone speed]"));
    break; //Empty black screen

    case SCREEN_POINTEDIT:
    printScreen_pointEdit();
    break;

    default:
    display->setCursor(0,0);
    display->println(F("ERROR: Unknown screen."));
    break;
  }
  if (dialogBoxOpen && !screensaverEnabled)printDialogBox();

  if(!*displayData->carPowered)printEmergencyOverheat();
  /*
  if (showWarning)
  {
    if (!displayData->droneConnected)printWifiDisconnected(108, 44);
  }
  if (displayData->emergencyPressed) printEstopEmergency();
  */

  Serial.println(F("."));
  display->sendBuffer();
}
void MenuDisplay::printTestraster()
{
  display->drawXBMP(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, sprite_screensaverBackground);
  /*
  display->drawLine((SCREEN_WIDTH/2), 0, (SCREEN_WIDTH/2), SCREEN_HEIGHT, WHITE); // |
  display->drawLine(0, (SCREEN_HEIGHT/2), SCREEN_WIDTH, (SCREEN_HEIGHT/2), WHITE); // -
  display->drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  */
}
void MenuDisplay::printDialogBox()
{
  uint8_t dialogBoxWidth  = dialogBoxText.length() * 6 + 10; //6=CharacterWidth, 10=Extra width
    uint8_t dialogBoxHeight = 40;
    uint8_t dialogBoxX      = (SCREEN_WIDTH - dialogBoxWidth)/2;
    uint8_t dialogBoxY      = (SCREEN_HEIGHT - dialogBoxHeight)/2;

    // draw the dialog box
    display->setDrawColor(0);
    display->drawBox(dialogBoxX, dialogBoxY, dialogBoxWidth, dialogBoxHeight);
    display->setDrawColor(1);
    display->drawFrame(dialogBoxX, dialogBoxY, dialogBoxWidth, dialogBoxHeight);

    // print the text in the middle of the dialog box
    display->setCursor(dialogBoxX + (dialogBoxWidth - dialogBoxText.length()*6) / 2, dialogBoxY + 10);
    display->print(dialogBoxText);

    if (dialogBoxType == DIALOGTYPE_MESSAGE)return;
    
    // print the buttons on the bottom of the dialog box
    uint8_t buttonWidth = 20;
    uint8_t buttonHeight = 10;
    uint8_t buttonAX = (SCREEN_WIDTH/2) - (buttonWidth/2) - 25;
    uint8_t buttonBX = (SCREEN_WIDTH/2) - (buttonWidth/2) + 25;




    if (dialogBoxVariableEditMode)
    {
      display->setCursor(60, 40);
      display->print(*dialogBoxEditableVariable);
    }

    if(!dialogBoxHasButtons) return;

    if (dialogBoxIndex == 0)
    {
      display->setDrawColor(1);      
      display->drawBox    (buttonAX,    dialogBoxY+20, buttonWidth, buttonHeight);
      display->setDrawColor(0);
      display->drawFrame  (buttonAX,    dialogBoxY+20, buttonWidth, buttonHeight);
      display->setCursor  (buttonAX+2,  dialogBoxY+20);
      display->print("No");

      display->setDrawColor(1);
      display->drawFrame  (buttonBX,    dialogBoxY+20, buttonWidth, buttonHeight);
      display->setCursor  (buttonBX+2,  dialogBoxY+20);
      display->print("Yes");
    }
    else
    {
      display->setDrawColor(1);
      display->drawFrame  (buttonAX,    dialogBoxY+20, buttonWidth, buttonHeight);
      display->setCursor  (buttonAX+2,  dialogBoxY+20);
      display->print("No");

      display->setDrawColor(1);      
      display->drawBox    (buttonBX,    dialogBoxY+20, buttonWidth, buttonHeight);
      display->setDrawColor(0);
      display->drawFrame  (buttonBX,    dialogBoxY+20, buttonWidth, buttonHeight);
      display->setCursor  (buttonBX+2,  dialogBoxY+20);
      display->print("Yes");
    }
    display->setDrawColor(1);

    
    
}
void MenuDisplay::printWifiDisconnected(uint8_t x, uint8_t y)
{
  // Black out background
    display->setDrawColor(0);
    display->drawBox(x, y, SPRITETYPE_SMALL+4, SPRITETYPE_SMALL+4);
    display->setDrawColor(1);

    // Draw warning sprite
    display->drawXBMP(x+2, y+2, SPRITETYPE_SMALL, SPRITETYPE_SMALL, sprite_wifiDisconnected);

    // Draw Frame
    display->drawFrame(x, y, SPRITETYPE_SMALL+4, SPRITETYPE_SMALL+4);
}
void MenuDisplay::printEstopEmergency()
{
  // Black out background
    display->setDrawColor(0);
    display->drawBox(36, 0, 56, 60); // Adjusted for text width and height
    display->setDrawColor(1);

    // Draw warning sprite
    display->drawXBMP(44, 2, SPRITETYPE_HUGE, SPRITETYPE_HUGE, sprite40_warning); // Adjusted for sprite size

    // Draw text
    display->setCursor(46, 44); // Adjusted x value for "E-STOP"
    display->print("E-STOP");
    display->setCursor(43, 53); // Adjusted x value for "PRESSED"
    display->print("PRESSED");

    // Draw Frame
    display->drawFrame(36, 0, 56, 64); // Adjusted for text width and height
}
void MenuDisplay::printEmergencyOverheat()
{
  // Black out background
    display->setDrawColor(0);
    display->drawBox(15, 7, 100, 50);
    display->setDrawColor(1);

    // Draw warning sprite
    display->drawXBMP(20, 12, SPRITETYPE_HUGE, SPRITETYPE_HUGE, sprite40_warning);

    // Draw text
    display->setCursor(50, 15);
    display->print("Power Loss");

    display->setCursor(55, 33);
    display->print(" Shutdown");

    display->setCursor(70, 43);
    if (*displayData->powerLossTimer == 0) display->print("Failed");
    else if (*displayData->powerLossTimer > 10) display->print("10");
    else display->print(*displayData->powerLossTimer);

    // Draw Frame
    display->drawFrame(15, 7, 100, 50);
}
void MenuDisplay::printScreen_Screensaver()
{
  if (screensaverStyle == SCREENSAVER_GRAPHICAL)
  {
    /*
    display->drawXBMP(32, 0, SPRITE_64x64, SPRITE_64x64, sprite_loopyDrone);
    display->setCursor(30,0);
    display->println(F("FutureMindz"));

    display->setCursor(25, 50);
    display->println(F("Drone Control"));
    */
    
 
    display->setCursor(0, 20);
    display->setFont(u8g2_font_10x20_tf);
    display->print(*displayData->currentTemperature, 1);
    display->print(F("/"));
    display->print(*displayData->goalTemperature);
    display->setFont(u8g2_font_6x10_tf);

    display->setCursor(0, 40);
    display->print(*displayData->currentHumidity, 0);
    display->print(F("%"));
  }
  else
  {
    display->clearBuffer();
  }

}
void MenuDisplay::printScreen_Menu()
{
  switch (graphicsType)
  {
    case GRAPHICSTYPE_TEXT:
    {
      display->setCursor(0,0);
      for (uint8_t i = 0; i < MENU_MAXINDEX; i++)
      {
        String text = currentFolder->getSubItemText(i+indexOffset);
        display->println(text);
      }
      display->drawFrame(0, (currentIndex-indexOffset)*8, 85, 8);
    }
    break;
    case GRAPHICSTYPE_SPRITES:
    {
      //Draw scrollbar
      if( currentFolder->getMenuItemsLength() > MENU_MAXINDEX)
      {
        uint8_t handleWidth = SCREEN_WIDTH / currentFolder->getMenuItemsLength();
        uint8_t handlePosition = handleWidth * currentIndex;
       display->drawHLine(0, SCREEN_HEIGHT - SCROLLBARHEIGHT, SCREEN_WIDTH);
        display->drawBox(handlePosition, SCREEN_HEIGHT - SCROLLBARHEIGHT, handleWidth, SCROLLBARHEIGHT);
      }
      

      //Draw title of selected item
      display->setCursor((SCREEN_WIDTH - (currentMenuItem->getTitle().length())*CHARACTERWIDTH)/2, 0);
      display->println(currentMenuItem->getTitle());

      for (uint8_t i = 0; i < MENU_MAXINDEX; i++)
      {
        //Check if the index is out of bounds
        if (i+indexOffset >= currentFolder->getMenuItemsLength()) break;


        //Drawing sprite
        const unsigned char *spriteToPrint = currentFolder->getSubItemSprite(i+indexOffset);
        display->drawXBMP( 44*i, CHARACTERHEIGTH+2, SPRITETYPE_HUGE, SPRITETYPE_HUGE, spriteToPrint);

        //Drawing data type sprite
        switch (currentFolder->getSubItem(i+indexOffset)->getSignature())
        {
          case SIGNATURE_U64:
          {
            MenuItem_U64 *menuItem_U64 = (MenuItem_U64*)currentFolder->getSubItem(i+indexOffset);
            int textWidth = String(menuItem_U64->getData()).length() * CHARACTERWIDTH;
            int startPoint = (44*i + 20) - (textWidth / 2);
            display->setDrawColor(0);
            display->drawBox(startPoint, 48, textWidth, CHARACTERHEIGTH);
            display->setDrawColor(1);
            display->setCursor(startPoint, 48);
            display->print(menuItem_U64->getData());
          }
          break;
          case SIGNATURE_FLOAT:
          {
            MenuItem_Float *menuItem_Float = (MenuItem_Float*)currentFolder->getSubItem(i+indexOffset);
            int textWidth = String(menuItem_Float->getData()).length() * CHARACTERWIDTH;
            int startPoint = (44*i + 20) - (textWidth / 2);
            display->setDrawColor(0);
            display->drawBox(startPoint, 48, textWidth, CHARACTERHEIGTH);
            display->setDrawColor(1);
            display->setCursor(startPoint, 48);
            display->print(menuItem_Float->getData());
          }
          break;
        
          default:
          {
            if (ENABLEPRINTGRAPHICSTYPES || currentFolder->getSubItem(i+indexOffset)->getSignature() == SIGNATURE_BOOL) //Bad temporary fix for still showing bools
            {
              const unsigned char *dataSprite = currentFolder->getSubItemDataSprite(i+indexOffset);
              display->setDrawColor(0);
              display->drawBox( (44*i)+12, 43, SPRITETYPE_SMALL, SPRITETYPE_SMALL);
              display->setDrawColor(1);
              display->drawXBMP( (44*i)+12, 43, SPRITETYPE_SMALL, SPRITETYPE_SMALL, dataSprite); 
            }   
          }
          break;
        }
      }

      //Drawing index frame
      display->drawFrame(44*(currentIndex-indexOffset), CHARACTERHEIGTH+1, SPRITETYPE_HUGE, SCREEN_HEIGHT-SCROLLBARHEIGHT-CHARACTERHEIGTH);
    }
    break;
  }
}
void MenuDisplay::printScreen_Boot()
{
  int coordinateX = 84;
  display->drawXBMP(coordinateX, 10, SPRITETYPE_HUGE, SPRITETYPE_HUGE, sprite40_vegaHUGE);
  display->setCursor(coordinateX+8, 50);
  display->print(F("Vega"));
}
void MenuDisplay::printScreen_Varedit_GameTime()
{
  display->setCursor(30,56);
  display->print(F("Temperature: "));
  display->print(*displayData->goalTemperature);
}
void MenuDisplay::printScreen_pointEdit()
{
  display->setCursor(0, 0);
  display->print(F("Effect: "));
  display->println(*displayData->ledEffect);
}


void MenuDisplay::printScreen_Reboot()
{
  int coordinateX = 32;
  display->drawXBMP(coordinateX, 0, SPRITE_64x64, SPRITE_64x64, sprite64_dots);
  display->setCursor(coordinateX+16, 0);
  display->print(F("Reboot"));
}

byte MenuDisplay::relativeIndex()
{
  return (currentIndex-indexOffset);
}
void MenuDisplay::openDialogBox_Internal(String text, void (MenuDisplay::*dialogFunction_InternalYes)(), void (MenuDisplay::*dialogFunction_InternalNo)())
{
  Serial.println(F("MenuDisplay: Opening INTERNAL dialog box."));
  dialogBoxType = DIALOGTYPE_INTERNAL;
  dialogBoxOpen = true;
  dialogBoxIndex = 1;
  dialogBoxText = text;
  this->dialogFunction_InternalYes  = dialogFunction_InternalYes;
  this->dialogFunction_InternalNo   = dialogFunction_InternalNo;
  dialogBoxVariableEditMode = false;

  print();
}

//Public
void MenuDisplay::tick()
{
  uint64_t currentTime = millis();

  if (!allowScreensaver)
  {
    screensaverEnabled = false;
    showWarning = false;
    print();
  }
  else
  {
    //If a button hasn't been pressed in the last 250 millis. Also checks if no interactions were made in the first seconds
    if ((currentTime - lastInteractionTime) > SCREENSAVERTIME || (currentTime < SCREENSAVERTIME && lastInteractionTime == 0))
    {
      //If the screensaver was still off
      if(!screensaverEnabled && lastActiveScreen == SCREEN_MENU)
      {
        lastActiveScreen = activeScreen;
        activeScreen = SCREEN_SCREENSAVER;
        screensaverEnabled = true;
        Serial.println(F("Screensaver Enabled"));
        print();
      }
    }
    else if (screensaverEnabled)
    {
      activeScreen = lastActiveScreen;
      screensaverEnabled = false;
      Serial.println(F("Screensaver Disabled"));
      print();
    }

    //If time for a refresh (Screensaver on only)
    if (screensaverEnabled)
    {
      if(currentTime - lastScreenupdateTime >= SCREENUPDATETIME)
      {
        lastScreenupdateTime = currentTime;
        print();
      }
    }


    if(currentTime - lastShowWarningMillis >= SHOWWARNINGMILLIS)
    {
      lastShowWarningMillis = currentTime;
      showWarning = !showWarning;
    }
  }
  
  //If continous framerate is required (Turret fire only)
  if(continousFramerateMode)
  {
    Serial.print("!");
    print();
  }
}
void MenuDisplay::forceTick()
{
  print();
}
void MenuDisplay::loadMenuStructure(MenuItem_Folder *rootFolder)
{
  this->rootFolder->setLastIndex(currentIndex);
  this->rootFolder->setLastIndexOffset(indexOffset);
  this->rootFolder = rootFolder;
  currentFolder = rootFolder;
  currentIndex = this->rootFolder->getLastIndex();
  indexOffset = this->rootFolder->getLastIndexOffset();
}
void MenuDisplay::openMessageBox(String text, void (*dialogFunction)())
{
  Serial.println(F("MenuDisplay: Opening message box."));
  dialogBoxType = DIALOGTYPE_MESSAGE;
  dialogBoxOpen = true;
  dialogBoxIndex = 0;
  dialogBoxText = text;
  this->dialogFunctionYes = dialogFunction;
  this->dialogFunctionNo = dialogFunction; 
  dialogBoxVariableEditMode = false;

  print();
}
void MenuDisplay::openDialogBox(String text, void (*dialogFunctionYes)(), void (*dialogFunctionNo)())
{
  Serial.println(F("MenuDisplay: Opening dialog box."));
  dialogBoxType = DIALOGTYPE_EXTERNAL;
  dialogBoxOpen = true;
  dialogBoxIndex = 1;
  dialogBoxText = text;
  this->dialogFunctionYes = dialogFunctionYes;
  this->dialogFunctionNo = dialogFunctionNo; 
  dialogBoxVariableEditMode = false;

  print();
}
void MenuDisplay::openDialogBox_Variable(bool withButtons, String text, uint64_t *variableToEdit, void (*dialogFunctionYes)(), void (*dialogFunctionNo)())
{
  Serial.print(F("MenuDisplay: Opening dialog box with variable: "));
  openDialogBox(text, dialogFunctionYes, dialogFunctionNo);
  dialogBoxVariableEditMode = true;
  dialogBoxEditableVariable = variableToEdit;
  dialogBoxHasButtons = withButtons;
  dialogBoxIndex = 1;
  print();
}
void MenuDisplay::doMenuInteraction(uint8_t interaction)
{
  if (interaction == MENUINTERACTION_NONE) return;



  if (screensaverEnabled)
  {
    if(interaction == MENUINTERACTION_EXIT)
    {
      screensaverStyle = !screensaverStyle; //Toggles screen black/graphical for screensavers
    }
    else if (interaction == MENUINTERACTION_ENTER) //Only allow single taps to wake from screensaver
    {
      lastInteractionTime = millis();
      Serial.println(F("MenuDisplay: Screensaver active, Not allowing menu input and disabling screensaver."));
    }

    return;
  }
  

  Serial.println(F("MenuDisplay: Interaction"));

  lastInteractionTime = millis();
  
  if (dialogBoxOpen)
  {
    Serial.println(F("MenuDisplay: Dialog box open, Not allowing menu input."));
    switch (interaction)
    {
      case MENUINTERACTION_PREVIOUS:
      if (dialogBoxHasButtons &&  dialogBoxIndex < 1) dialogBoxIndex++;
      break;

      case MENUINTERACTION_NEXT:
      if (dialogBoxHasButtons && dialogBoxIndex > 0) dialogBoxIndex--;
      break;

      case MENUINTERACTION_EXIT:
      resolveDialogBox_no();
      break;

      case MENUINTERACTION_ENTER:
      if(dialogBoxIndex) resolveDialogBox_yes();
      else resolveDialogBox_no();
      break;
    }
    tick();
    print();
    return;
  }
  


  if (interaction == MENUINTERACTION_MENU_OPEN)
  {
    activeScreen = SCREEN_MENU;
    lastActiveScreen = SCREEN_MENU;
  }

  if (lastActiveScreen != SCREEN_MENU)
  {
    Serial.println(F("MenuDisplay: Not in menu, Not allowing menu input."));
    tick();
    return;
  }
  

  switch (interaction)
  {
    case MENUINTERACTION_NEXT:
    navigateNext();
    break;

    case MENUINTERACTION_PREVIOUS:
    navigatePrevious();
    break;

    case MENUINTERACTION_ENTER:
    {
      if (currentMenuItem->getEntryDialogString() != "")
      {
        openDialogBox(currentMenuItem->getEntryDialogString(), currentMenuItem->getEntryFunction(), NULL);
      }
      else 
      {
        navigateEnter();
      }
    }
    break;

    case MENUINTERACTION_EXIT:
    Serial.println(F("MenuDisplay: Exit selected."));
    if (currentFolder->getExitDialogString() != "")openDialogBox_Internal(currentFolder->getExitDialogString(), &MenuDisplay::navigateExit, NULL);
    else navigateExit();
    break;
  }
 
  tick();
  print();

}
void MenuDisplay::forceMenuInteraction(uint8_t interaction)
{
  if (interaction == MENUINTERACTION_NONE) return;

  lastInteractionTime = millis();
  
  if (dialogBoxOpen)
  {
    Serial.println(F("MenuDisplay: Dialog box open, Not allowing menu input."));
    switch (interaction)
    {
      case MENUINTERACTION_PREVIOUS:
      if (dialogBoxIndex < 1) dialogBoxIndex++;
      break;

      case MENUINTERACTION_NEXT:
      if (dialogBoxIndex > 0) dialogBoxIndex--;
      break;

      case MENUINTERACTION_EXIT:
      resolveDialogBox_no();
      break;

      case MENUINTERACTION_ENTER:
      if(dialogBoxIndex) resolveDialogBox_yes();
      else resolveDialogBox_no();
      break;
    }
    tick();
    print();
    return;
  }
  


  if (interaction == MENUINTERACTION_MENU_OPEN)
  {
    activeScreen = SCREEN_MENU;
    lastActiveScreen = SCREEN_MENU;
  }

  if (lastActiveScreen != SCREEN_MENU)
  {
    Serial.println(F("MenuDisplay: Not in menu, Not allowing menu input."));
    tick();
    return;
  }
  

  switch (interaction)
  {
    case MENUINTERACTION_NEXT:
    navigateNext();
    break;

    case MENUINTERACTION_PREVIOUS:
    navigatePrevious();
    break;

    case MENUINTERACTION_ENTER:
    if (currentMenuItem->getEntryDialogString() != "")
    {
      openDialogBox_Internal(currentMenuItem->getEntryDialogString(), &MenuDisplay::navigateEnter, NULL);
    }
    else 
    {
      navigateEnter();
    }
    break;

    case MENUINTERACTION_EXIT:
    Serial.println(F("MenuDisplay: Exit selected."));
    if (currentFolder->getExitDialogString() != "")openDialogBox_Internal(currentFolder->getExitDialogString(), &MenuDisplay::navigateExit, NULL);
    else navigateExit();
    break;
  }

  
  tick();
  print();
}
void MenuDisplay::setActiveScreen(uint8_t activeScreen)
{
  this->activeScreen = activeScreen;
  this->lastActiveScreen = activeScreen;
}
void MenuDisplay::keepAwake()
{
  lastInteractionTime = millis();
}
bool MenuDisplay::isScreensaverActive()
{
  return screensaverEnabled;
}
void MenuDisplay::setAllowScreensaver(bool allowScreensaver)
{
  this->allowScreensaver = allowScreensaver;
}
void MenuDisplay::printFoldersAndFiles()
{
  Serial.println(F("MenuDisplay: Listing all folders and files. (NOTE: Printing adresses for debug)"));
  rootFolder->printTitleAdress();
}
void MenuDisplay::printCurrentFolder()
{
  Serial.print(F("MenuDisplay's current folder: "));
  String text = currentFolder->toText();
  Serial.println(text);
}
void MenuDisplay::setContinousFramerateMode(bool continousFramerateMode)
{
  this->continousFramerateMode = continousFramerateMode;
}
void MenuDisplay::startScreensaver()
{
  Serial.println(F("Forcing screensaver to start."));
  lastInteractionTime -= SCREENSAVERTIME; //Trick the screensaver into enabling
}
bool MenuDisplay::isDialogBoxOpen()
{
  return dialogBoxOpen;
}

void MenuDisplay::printBootMessage(String text)
{
  display->setDrawColor(0);
  display->drawBox(0, bootMessageLineIndex*9, text.length()*6, 9);
  display->setDrawColor(1);

  display->setCursor(0, bootMessageLineIndex*9);
  display->println(text);
  display->sendBuffer();

  bootMessageLineIndex++;
  if (bootMessageLineIndex > 6)
  {
    bootMessageLineIndex = 0;
    display->clearBuffer();
    printScreen_Boot();
  }
}