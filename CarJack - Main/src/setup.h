#ifndef SETUP_H
#define SETUP_H

/*=== === === PINOUT === === ===*/
//Pullup Inputs
#define PIN_ENCODER_A           25  //Implemented
#define PIN_ENCODER_B           33  //Implemented
#define PIN_BUTTON              32  //Implemented
#define PIN_ENDSTOP_MIN         27  //Tested
#define PIN_ENDSTOP_MAX         14  //Tested
#define PIN_MANUALOVERIDE_SERVO 12  //Tested
//Inputs
#define PIN_VOLTAGESENSOR       35  //Implemented
#define PIN_AUDIO               26  //THEORY
//Analog Inputs
#define PIN_LDR                 34  //Implemented
//PWM Outputs
#define PIN_BUZZER              2   //Implemented
#define PIN_SERVO               16  //Implemented
//Digital Outputs
#define PIN_LEDS                22  //Implemented
#define PIN_SDA                 15  //Implemented
#define PIN_SCL                 13  //Implemented
#define PIN_RELAY               4   //Implemented

#define PWM_CHANNEL_BUZZER 2
#define PWM_CHANNEL_SERVO  0


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