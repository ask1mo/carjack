#ifndef SETUP_H
#define SETUP_H


/*

INSTRUCTIONS ARE DEPRECATED, SEE DRAW-IO FOR UPDATED INSTRUCTIONS

  Rotate:
  -MM- "CMD_MM_Navigate" (Navigates left or right in menu mode)
  >If CCMode "Live_Speed":
    -CC- Adjust CC speed
  >If CCMode "Live_Settings":
    Rotate Right
    -CC- Cycle precision (When Rotating Right)
    Rotate Left
    -CC- Cycle acceleration (When Rotating Left)
    



  Short 1x
  -CC- Enable/Disable Cruise Control (CCMode: LiveSpeed, LiveSettings, Off)
  -MM- Enter "CMD_MM_Enter" (Clicks on things when in menu mode)

  Long 1s:
  -CC- Speed->load from saved speed
  -MM- Exit "CMD_MM_Return" (Exits from a folder/screen in menu mode)

  Long 3s:
  -CC- Speed->save from current speed
  -MM>CC- Leave menu mode (MM), enable CC mode

  Long 5s:
  -CC<->MM- Switch between modes (CC<->MM)




  Commands:
  - MM Scroll Menu
  - MM Enter
  - MM Exit
  - CC Enable/Disable (CCMode: Locked, Adjust, Off)
  - CC scroll Speed 
  - CC Cycle Precision
  - CC Load MEM
  - CC Store MEM
  - Switch between modes


  // Reminder: "Saved speed" is not the car's saved speed but ESP32 saved speed.
  // I want the cruise control to be active at a different speed as the saved speed.

  // There is a manual cutoff switch that turns off the cruise control (Both in hardware and in software)
  // Pressing the brakes turns off the ESP32 cruise control. (By measuring the brake lights)
*/

/*=== === === DATATYPES === === ===*/

#define ONE_SECOND 1000

enum MenuInteraction
{
  NONE = 0,
  LEFT = 1,
  RIGHT = 2,
  ENTER = 3,
  EXIT = 4,
  CRUISECONTROL = 5,
};
enum OperatingMode
{
  CCMODE = 0,
  MENUMODE = 1,
};

enum CCRotaryMode
{
  LOCKED = 0,
  SPEED = 1,
  SETTINGS = 2,
};


enum DataPointID
{
  MENUINTERACTION,
  CCSTATE,
  VEHICLESPEED,
  THROTTLEPOSITION,
  DATATIMEDOUT,

};
struct CarJackDataFrame
{
  float speed = 0;
  float rpm = 0;
  float throttlePosition = 0;
  
};
struct ObdDataFrame
{
  float vehicleSpeed = 0;
  float throttlePosition = 0;
  float rpm = 0;
  bool dataTimedOut = false;
};

/*=== === === PINOUT === === ===*/

//Pullup Inputs
//#define PIN_MASTERENABLE        17 //Removed in favour of cutting power to the esp32 when CC is disabled
#define PIN_ENCODER_A           19  //Implemented
#define PIN_ENCODER_B           23  //Implemented
#define PIN_BUTTON              18  //Implemented
//Floating Inputs
#define PIN_BRAKESENSOR         5

//Communication input/outputs 
#define PIN_SERIAL_TX           17  
//Signal Outputs
#define PIN_PWM_LED   26 //PWM
//Binary Outputs
#define PIN_RELAY_PLUS          25  //Implemented
#define PIN_RELAY_MIN           32  //Implemented
#define PIN_RELAY_SET           33  //Implemented




#endif