Project Overview
- This repository contains the firmware for the "CarJack" embedded project. It is a PlatformIO/Arduino-style ESP32-firmware to elevate an older car's (My Hyundai atos 2003) infotainment and control systems with modern and completely new features. The firmware is modular, with a main image that handles the core user interface, LED effects, and peripheral control, and additional modules for specific features like cruise control and driving condition-based music playback.

Project Metadata
- Type: Firmware (PlatformIO / Arduino-style)
- Client: Personal / portfolio project (no client information supplied)
- Status: Partially tested; actively maintained in-repo but requires hardware verification
- Product: CarJack — Main firmware image and supporting modules
- Confidentiality: No NDA known; repository intended for local review and portfolio viewing

Technical Stack
- Programming language(s): C++ (Arduino-style)
- Hardware / platform: ESP32 (see `platformio.ini` — `env:esp32dev`)
- RTOS Arduino framework on bare metal
- 3 Modules:
    - "Carjack - main": Infotainment system: Bluetooth audio, menu display, LED effects, buzzer feedback, thermomter and servo climate control for a car project.
    - "CarJack - CruiseControl" (WIP): A safety-focussed seperate ESP32-firmare for controlling the car's cruise control sistem via a lever or dial. Effectively replacing the gas pedal.
    - "CarJack - VUMeter" (Planned): A additional ESP32-based firmware that plays music based on driving conditions (e.g., throttle position, speed). Eg: When at full throttle, the music will play the main refrain of a song, but at low speed it plays a slower verse.
- Communication protocols / interfaces:
    - GPIO: Buttons, encoders, voltage sensor via divider, ldr
    - ELM327 via Bluetooth (ELMDuino library)
    - PWM: servos, Buzzer
    - SPI: Display
    - I2C: Thermometer
    - WS28XX: LED drivers (TrinityLED custom library using FastLED )

System Description
- High-level architecture: The firmware is modular: small driver/manager modules are initialized from `main.cpp` and executed inside a cooperative main loop. Inputs (buttons/encoder) are sampled, menu logic runs on a menu display module, LED effects are handled by a dedicated LED manager library (Trinity), and peripheral actions (buzzer, servos, climate control) are driven by focused modules.
- Main components / modules:
  - `MenuDisplay`: Menu and dialog abstractions for user interaction
  - `Trinity` / LED manager: FastLED-based effect processing and brightness control
  - `AskButton`, `AskBuzzer`, `AskServo`: Device driver wrappers for inputs, sound, and servos
  - `PID`: PID control utilities used for thermal/actuator control
  - `main.cpp`: Top-level initialization and cooperative loop coordinating modules

Current State
- What is working:
  - Carjack-Main module is functional for basic menu navigation, LED effects, buzzer feedback. Also: servo/temperature control logic (unverified on hardware)
- What is partially working:
  - PID control and temperature setpoint logic exist but require hardware testing and tuning. (The mechanical hardware I used broke during testing, and had not replaced and re-tested it before I sold the car)
- What is not implemented / known limitations:
  - Detailed wiring, pinout and hardware schematics are not included in this repository
  - The CruiseControl and VUMeter modules are present but not fully implemented or tested; They are in development and still require significant work to be functional.


Key Technical Challenges
- Coordinating time-critical peripherals (LED updates, servos) with a cooperative main loop: addressed by keeping per-module responsibilities small and using lightweight buffering for audio/encoder events.
- Implementing a PID control loop for temperature/actuator management: I have implemented the basic PID logic and setpoint handling, but tuning the parameters will require real-world testing and iteration.
- Building a safe cruise control system: This is a work in progress, but the plan is to use a separate ESP32 firmware. This system uses a multi core approach to isolate obd bluetooth data parsing and control logic.


How to Build / Run (if applicable)
- I am not planning to release this firmware publicly or provide detailed build instructions, as it is intended for local review and portfolio purposes.

Future Improvements
- Step 1: Buy a car to be able to continue testing. (I sold mine to my sister, and my girlfriend's parents arent too keen with me using their fancy mini cooper for testing a safety critical cruise control system. Can't imagine why ;)
- If my next car's climate control hardware is too complex to add servo's to: Remove the climate control hardware and merge the VUMeter (Reactive music) module into the main firmware. (Currently not enough GPIOs free to add the required actuators).
- Develop and test the VUMeter and CruiseControl modules fully.


Disclaimer
- This Readme file has been generated by an AI language model (ChatGPT) based on the provided code context.
- I have only quickly reviewed and edited the content for accuracy.
