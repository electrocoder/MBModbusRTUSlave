/*
 * MBModbusRTUSlaveExampleESP32.ino - Example Sketch for MBModbusRTUSlave Library
 *
 * Description: This sketch ESP32 with RS485 Module and Serial2 Modbus write and read configure
 * Author: S.Mersin (electrocoder) <electrocoder@gmail.com> (Assisted by Grok)
 * Date: April 08, 2025
 * Version: 1.1.0
 * License: MIT License
 *
 * Hardware:
 *  - ESP32 Nodemcu
 *
 * Dependencies:
 *  - MBModbusRTUSlave library (version 1.1.0 or higher)
 *
 * Usage:
 *  - Upload this sketch to your ESP32 DEv Board.
 *  - Use a Modbus master (e.g., Modbus Poll) to send commands:
 *    - Write 1 to register 5: "01 06 00 05 00 01 [CRC]" (turns LED ON)
 *    - Write 0 to register 5: "01 06 00 05 00 00 [CRC]" (turns LED OFF)
 */

#include "MBModbusRTUSlave.h"

#define RXD2 16 // ESP32 Serial2
#define TXD2 17 // ESP32 Serial2

#define REGISTER_COUNT 20
#define rs485_select_pin 26

long myModbusBaudRate = 9600;
MBModbusRTUSlave modbus(0x02, &Serial2, rs485_select_pin, REGISTER_COUNT);

unsigned long previousMillis = 0;
const long interval = 5000;

float float_value = 51.6;
int int_value = 0;

void setup()
{

  Serial.begin(9600);

  Serial2.begin(9600, SERIAL_8N2, RXD2, TXD2);

  pinMode(rs485_select_pin, OUTPUT);

  modbus.begin(myModbusBaudRate);

  Serial.println("Modbus start");
}

void loop()
{

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {

    previousMillis = currentMillis;

    float_value += 0.1;
    modbus.writeRegister(0, int_value);        // 0 address, Uint16_t
    modbus.writeFloatRegister(1, float_value); // 1 address, Float
  }

  // Read Register
  if (modbus.readRegisters())
  {

    Serial.println("Modbus read");
    Serial.println(modbus.getRegister(0));
    Serial.println(modbus.getFloatRegister(1));
  }

  Serial.println(" ");

  delay(100);  
}
