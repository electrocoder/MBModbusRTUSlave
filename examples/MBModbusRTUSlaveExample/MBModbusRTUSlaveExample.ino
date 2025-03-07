/*
 * MBModbusRTUSlaveExample.ino - Example Sketch for MBModbusRTUSlave Library
 *
 * Description: This sketch demonstrates the usage of the ModbusSlave library to configure
 *              an Arduino as a Modbus RTU slave. It controls an LED based on a specific
 *              register value and allows customization of the slave address, LED pin,
 *              and baud rate.
 * Author: S.Mersin (electrocoder) <electrocoder@gmail.com> (Assisted by Grok)
 * Date: March 07, 2025
 * Version: 1.0.0
 * License: MIT License
 * 
 * Hardware: 
 *  - Arduino Uno (or any compatible board)
 *  - LED connected to pin 13 (onboard LED can be used)
 * 
 * Dependencies:
 *  - MBModbusRTUSlave library (version 1.0.0 or higher)
 * 
 * Usage:
 *  - Upload this sketch to your Arduino.
 *  - Use a Modbus master (e.g., Modbus Poll) to send commands:
 *    - Write 1 to register 5: "01 06 00 05 00 01 [CRC]" (turns LED ON)
 *    - Write 0 to register 5: "01 06 00 05 00 00 [CRC]" (turns LED OFF)
 */

#include "MBModbusRTUSlave.h"

MBModbusRTUSlave modbus(0x01, 13, 0, 10);  // Slave address: 0x01, LED pin: 13, LED register: 0, Register count: 10
long modbusBaudRate = 9600;  // If you want it can be another value like 115200, 19200

void setup() {

  modbus.begin(modbusBaudRate);
  Serial.println("Modbus was downloaded.");

}

void loop() {

  modbus.update();

}