/*
 * MBModbusRTUSlave.h - Modbus RTU Slave Library for Arduino
 *
 * Description: This library configures Arduino devices as Modbus RTU slaves.
 *              It supports LED control, register reading/writing, and flexible baud rate settings.
 * Author: S.Mersin (electrocoder) <electrocoder@gmail.com> (Assisted by Grok)
 * Date: March 07, 2025
 * Version: 1.0.0
 * License: MIT License
 * 
 * Usage: The MBModbusRTUSlave class is initialized with a slave address, LED pin, and LED register index.
 *        Baud rate can be set via the begin() method using a variable.
 */

#ifndef MBModbusRTUSlave_h
#define MBModbusRTUSlave_h

#include <Arduino.h>

class MBModbusRTUSlave {
public:
  MBModbusRTUSlave(uint8_t slaveAddress = 0x01, uint8_t ledPin = 13, uint8_t ledRegisterIndex = 0, uint16_t registerCount = 10); // Constructor
  void begin(long baudRate = 9600);  // Setup function
  void update();                     // Loop function

  // Setter methods
  void setSlaveAddress(uint8_t slaveAddress);
  void setLedPin(uint8_t ledPin);
  void setLedRegisterIndex(uint8_t ledRegisterIndex);
  void setBaudRate(long baudRate);

private:
  uint8_t _slaveAddress;             // Slave address
  uint8_t _ledPin;                   // LED pin
  uint8_t _ledRegisterIndex;         // Register index for LED control
  uint16_t _registerCount;           // Register count
  long _baudRate;                    // Baud rate

  uint16_t* modbusRegisters;         // Dynamic register array
  uint8_t requestBuffer[8];                  // Incoming data buffer
  uint8_t responseBuffer[256];               // Data buffer to be sent

  void processReadHoldingRegisters();        // Register reading process
  void processWriteSingleRegister();         // Single register writing operation
  uint16_t calculateCRC(uint8_t *buffer, uint8_t length);  // CRC calculation
  bool checkCRC(uint8_t *buffer, uint8_t length);          // CRC check
};

#endif