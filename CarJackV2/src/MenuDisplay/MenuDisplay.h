#ifndef MENUDISPLAY_H
#define MENUDISPLAY_H

#include <Arduino.h>
#include <SPI.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include <U8g2lib.h>
#include "MenuItem_Bool/MenuItem_Bool.h"
#include "MenuItem_1Bool/MenuItem_1Bool.h"
#include "MenuItem_Folder/MenuItem_Folder.h"
#include "MenuItem_Static/MenuItem_Static.h"
#include "MenuItem_U8/MenuItem_U8.h"
#include "MenuItem_U64/MenuItem_U64.h"
#include "MenuItem_Float/MenuItem_Float.h"
#include "MenuItem_StringInt8/MenuItem_StringInt8.h"
#include "Sprites.h"
#include "setup.h"



#define SCREEN_WIDTH 128 // OLED //display width, in pixels
#define SCREEN_HEIGHT 64 // OLED //display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define CHARACTERWIDTH 6
#define CHARACTERHEIGTH 9

#define GRAPHICSTYPE_TEXT       0
#define GRAPHICSTYPE_SPRITES    1

#define ENABLEPRINTGRAPHICSTYPES false

#define SCROLLBARHEIGHT 5

#define SCREENSAVERTIME     5000
#define FRAMESPERSECOND     1
#define SCREENUPDATETIME    (1000/FRAMESPERSECOND)
#define SHOWWARNINGMILLIS   500

#define SCREEN_NONE                                     0
#define SCREEN_REBOOT                                   1
#define SCREEN_SCREENSAVER                              2
#define SCREEN_MENU                                     3
#define SCREEN_BLA                                      4
#define SCREEN_DRONECONTROL                             5
#define SCREEN_DRONESETUP                               6
#define SCREEN_EMERGENCY                                7
#define SCREEN_AUTOMATIC                                8
#define SCREEN_TEST                                     9
#define SCREEN_VAREDIT_SELECTPOINTTOEDIT                10
#define SCREEN_VAREDIT_GAMETIME                         11
#define SCREEN_VAREDIT_DRONESPEED                       12
#define SCREEN_POINTEDIT                                13
#define SCREEN_POWERLOSS                                14
//Old?
#define PAGENAMEOFFSET 100
#define MENU_MAXINDEX 3

#define MENUINTERACTION_NONE            0
#define MENUINTERACTION_MENU_OPEN       1
#define MENUINTERACTION_SCREENSAVER_ON  3
#define MENUINTERACTION_SCREENSAVER_OFF 4
#define MENUINTERACTION_NEXT            5
#define MENUINTERACTION_PREVIOUS        6
#define MENUINTERACTION_ENTER           7
#define MENUINTERACTION_EXIT            8

#define DIALOGTYPE_EXTERNAL 0
#define DIALOGTYPE_INTERNAL 1
#define DIALOGTYPE_MESSAGE  2

#define SCREENSAVER_BLACK       0
#define SCREENSAVER_GRAPHICAL   1





struct DisplayData
{
    //Varaibles
    uint8_t     *powerLossTimer;
    //Settings
    uint64_t    *ledEffect;
    uint64_t    *ledBrightness;
    bool        *autoClimate;
    uint64_t    *goalTemperature;
    bool        *screenOff;
    bool        *nightScreenOff;
    bool        *sound;
    //Sensors
    float       *currentTemperature;
    float       *currentHumidity;
    bool        *dayBrightness;
    bool        *carPowered;
};

class MenuDisplay
{    
    private:
    DisplayData *displayData;
    MenuItem_Folder *rootFolder;
    MenuItem_Folder *currentFolder;
    MenuItem        *currentMenuItem;
    bool enabled;
    uint8_t graphicsType;
    uint8_t currentIndex;
    uint8_t indexOffset;
    uint8_t activeScreen;
    uint8_t lastActiveScreen;
    uint64_t lastInteractionTime;
    bool screensaverEnabled;
    bool screensaverStyle = SCREENSAVER_GRAPHICAL;
    uint64_t lastScreenupdateTime;
    bool continousFramerateMode;
    U8G2_SH1106_128X64_NONAME_F_HW_I2C  *display;
    bool allowScreensaver;
    uint8_t bootMessageLineIndex;
    uint8_t currentFrame;


    bool showWarning;
    uint64_t lastShowWarningMillis;


    //DialogBox
    bool dialogBoxOpen;
    String dialogBoxText;
    uint8_t dialogBoxIndex; 
    uint8_t dialogBoxType;
    bool dialogBoxVariableEditMode;
    bool dialogBoxHasButtons;
    uint64_t *dialogBoxEditableVariable;
    //function pointer
    void (*dialogFunctionYes)();
    void (*dialogFunctionNo)();
    void (MenuDisplay::*dialogFunction_InternalYes)();
    void (MenuDisplay::*dialogFunction_InternalNo)();
    
    void print();
    virtual void printTestraster();
    virtual void printDialogBox();
    virtual void printWifiDisconnected(uint8_t x, uint8_t y);
    virtual void printEstopEmergency();
    virtual void printEmergencyOverheat();
    virtual void printScreen_Screensaver();
    virtual void printScreen_Menu();
    virtual void printScreen_Boot();
    virtual void printScreen_Varedit_GameTime();
    virtual void printScreen_pointEdit();
    virtual void printScreen_Reboot();

    void navigateNext();
    void navigatePrevious();
    void navigateEnter();
    void navigateExit();

    void resolveDialogBox_yes();
    void resolveDialogBox_no();

    byte relativeIndex();
        void openDialogBox_Internal(String text, void (MenuDisplay::*dialogFunction_InternalYes)(), void (MenuDisplay::*dialogFunction_InternalNo)());

    public:
    MenuDisplay(MenuItem_Folder *rootFolder, uint8_t graphicsType, DisplayData *displayData);

    void tick();
    void forceTick();
    void loadMenuStructure(MenuItem_Folder *rootFolder);
    void openMessageBox         (String text, void (*dialogFunction)());
    void openDialogBox          (String text, void (*dialogFunctionYes)(), void (*dialogFunctionNo)());
    void openDialogBox_Variable (bool withButtons, String text, uint64_t *variableToEdit, void (*dialogFunctionYes)(), void (*dialogFunctionNo)());

    bool isScreensaverActive();

    void doMenuInteraction(uint8_t interaction);
    void forceMenuInteraction(uint8_t interaction);
    void setActiveScreen(uint8_t screen);
    void keepAwake();

    void startScreensaver();
    void setAllowScreensaver(bool allowScreensaver);
    void setContinousFramerateMode(bool continousFramerateMode);

    bool isDialogBoxOpen();


    void printFoldersAndFiles();
    void printCurrentFolder();

    void printBootMessage(String text);
};

#endif