/*
 * MBModbusRTUSlave.h - Modbus RTU Slave Library for Arduino
 *
 * Description: This library configures Arduino devices as Modbus RTU slaves.
 *              It supports register reading/writing, flexible baud rate settings, and RS485 transceivers
 * Author: S.Mersin (electrocoder) <electrocoder@gmail.com> (Assisted by Grok)
 * Date: April 08, 2025
 * Version: 1.1.0
 * License: MIT License
 *
 * Usage: The MBModbusRTUSlave class is initialized with a slave address, serial port, RS485 control pin, and register count.
 *        Baud rate can be set via the begin() method. Register reading/writing is handled in the user's Arduino code.
 */

#ifndef MBModbusRTUSlave_h
#define MBModbusRTUSlave_h

#include <Arduino.h>

class MBModbusRTUSlave
{
public:
  MBModbusRTUSlave(uint8_t slaveAddress = 0x01, HardwareSerial *serialPort = &Serial, uint8_t rs485ControlPin = 2, uint16_t registerCount = 10);
  ~MBModbusRTUSlave(); // Destructor 
  void begin(long modbusBaudRate = 9600);

  bool readRegisters();
  void writeRegister(uint16_t registerAddress, uint16_t value);
  void writeFloatRegister(uint16_t registerAddress, float value); 
  uint16_t getRegister(uint16_t registerAddress); // Integer register value
  float getFloatRegister(uint16_t registerAddress); // Float register value

  void setSlaveAddress(uint8_t slaveAddress);
  void setModbusBaudRate(long modbusBaudRate);
  void setSerialPort(HardwareSerial *serialPort);
  void setRS485ControlPin(uint8_t rs485ControlPin);
  uint16_t getRegisterCount(); // Register count

private:
  uint8_t _slaveAddress;
  long _modbusBaudRate;
  HardwareSerial *_serialPort;
  uint8_t _rs485ControlPin;
  uint16_t _registerCount;   // Register count
  uint16_t *modbusRegisters; // Dynamic registers

  uint8_t requestBuffer[8];
  uint8_t responseBuffer[256];

  void processReadHoldingRegisters();
  void processWriteSingleRegister();
  uint16_t calculateCRC(uint8_t *buffer, uint8_t length);
  bool checkCRC(uint8_t *buffer, uint8_t length);
};

#endif