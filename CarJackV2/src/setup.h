#ifndef SETUP_H
#define SETUP_H

//Pinout
#define PIN_ENCODER_A       18  //Implemented
#define PIN_ENCODER_B       5   //Implemented
#define PIN_BUTTON          25  //Implemented
#define PIN_VOLTAGESENSOR   35  //Implemented
#define PIN_AUDIO           14
#define PIN_BUZZER          27  //Implemented - Needs work
#define PIN_LEDS            22  //Implemented
#define PIN_SDA             32  //Implemented
#define PIN_SCL             33  //Implemented
#define PIN_SERVO           17  //Implemented - Needs work
#define PIN_RELAY           26  //Implemented
#define PIN_LDR             34  //Testing


//Debug settings
#define DEBUGLEVEL DEBUG_DISABLED   //This is the level of debug messages you'll receive. Not all code supports this type of messaging.
//Debug presets, do not touch
#define DEBUG_DISABLED              0
#define DEBUG_ERRORS                1
#define DEBUG_WARNINGS              2
#define DEBUG_OPERATIONS            3
#define DEBUG_DAYISRUINED           5
//Debug settings


/*---=== Trinity setup ===---*/
#define PLATFORM_ESP32_WROOMDEVKIT  //This is where you select the type of platform you're using. Old feature from Trinity
#define LEDTYPE_STANDARD            //This is where you select the type of LED strip you're using. Old feature from Trinity
#define ENABLE_DIODECONTROL true    //This is where you select if you want to use individual diode control. Old feature from Trinity
#define CUSTOMPALETTEAMOUNT 5       //This is where you select the amount of custom palette slots you want to use. Old feature from Trinity

//Strip Setup
#ifdef LEDTYPE_STANDARD
    #define LEDCOLORDER GRB 
#endif
#ifdef LEDTYPE_CHRISTMASSTRING
    #define LEDCOLORDER RGB
#endif
//Strip Setup
/*---=== Trinity setup ===---*/

#endif