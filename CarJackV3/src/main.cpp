#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP2515.h>

#ifdef ESP8266
   #define CS_PIN    2
   #define INT_PIN   15
#elif defined(ESP32) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S3)
   #define CS_PIN    14
   #define INT_PIN   5
#elif defined(TEENSYDUINO)
   #define CS_PIN    8
   #define INT_PIN   3
#elif defined(ARDUINO_STM32_FEATHER)
   #define CS_PIN    PC5
   #define INT_PIN   PC4
#elif defined(ARDUINO_NRF52832_FEATHER)  /* BSP 0.6.5 and higher! */
   #define CS_PIN    27
   #define INT_PIN   30
#elif defined(ARDUINO_MAX32620FTHR) || defined(ARDUINO_MAX32630FTHR)
   #define CS_PIN    P3_2
   #define INT_PIN   P3_3
#elif defined(ARDUINO_ADAFRUIT_FEATHER_RP2040)
   #define CS_PIN    7
   #define INT_PIN   6
#elif defined(ARDUINO_ADAFRUIT_FEATHER_RP2040_CAN)
   #define CS_PIN    PIN_CAN_CS
   #define INT_PIN   PIN_CAN_INT
#elif defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_RASPBERRY_PI_PICO_W) // PiCowbell CAN Bus
   #define CS_PIN    20
   #define INT_PIN   21
#else
    // Anything else, defaults!
   #define CS_PIN    5
   #define INT_PIN   4
#endif

// Set CAN bus baud rate
#define CAN_BAUDRATE (500000)

Adafruit_MCP2515 mcp(CS_PIN);

void handleCANMessage(int packetSize) {
  if (packetSize) {
    // received a packet
    Serial.print("Received ");

    if (mcp.packetExtended()) {
      Serial.print("extended ");
    }

    if (mcp.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print("RTR ");
    }

    Serial.print("packet with id 0x");
    Serial.print(mcp.packetId(), HEX);

    if (mcp.packetRtr()) {
      Serial.print(" and requested length ");
      Serial.println(mcp.packetDlc());
    } else {
      Serial.print(" and length ");
      Serial.println(packetSize);

      // only print packet data for non-RTR packets
      while (mcp.available()) {
        Serial.print((char)mcp.read());
      }
      Serial.println();
    }

    Serial.println();
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial) delay(10);

  Serial.println("MCP2515 Receiver test!");

  if (!mcp.begin(CAN_BAUDRATE)) {
    Serial.println("Error initializing MCP2515.");
    while(1) delay(10);
  }
  Serial.println("MCP2515 chip found");
  
  // Set interrupt pin and callback function
  mcp.onReceive(INT_PIN, handleCANMessage);
}

void loop() {
  // Main loop can be empty because handleCANMessage will be called on interrupt
}
