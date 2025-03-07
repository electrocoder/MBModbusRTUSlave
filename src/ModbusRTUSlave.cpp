/*
 * ModbusRTUSlave.cpp - Implementation of the Modbus RTU Slave Library for Arduino
 *
 * Description: This file implements the methods of the ModbusRTUSlave class.
 *              It supports Modbus RTU protocol for register reading (0x03) and writing (0x06).
 *              Provides LED control and flexible baud rate adjustments.
 * Author: S.Mersin (electrocoder) <electrocoder@gmail.com> (Assisted by Grok)
 * Date: March 07, 2025
 * Version: 1.0.0
 * License: MIT License
 * 
 * Note: This library uses the Arduino Serial port to provide Modbus RTU slave functionality.
 *       All registers except the LED register are updated with random values.
 */

#include "ModbusRTUSlave.h"

ModbusRTUSlave::ModbusRTUSlave(uint8_t slaveAddress, uint8_t ledPin, uint8_t ledRegisterIndex, uint16_t registerCount) {
  _slaveAddress = slaveAddress;
  _ledPin = ledPin;
  _ledRegisterIndex = ledRegisterIndex;
  _registerCount = registerCount;
  _baudRate = 9600;  // Default baud rate

  // Create dynamic register array
  modbusRegisters = new uint16_t[_registerCount];

  // Fill the registers with initial values
  modbusRegisters[0] = 0;
  modbusRegisters[1] = 5678;
  modbusRegisters[2] = 90;
  for (int i = 3; i < _registerCount; i++) {
    modbusRegisters[i] = 0;
  }
}

void ModbusRTUSlave::begin(long baudRate) {
  _baudRate = baudRate;
  pinMode(_ledPin, OUTPUT);
  digitalWrite(_ledPin, LOW);
  Serial.begin(_baudRate, SERIAL_8N1);
  while (!Serial)
    ;
}

void ModbusRTUSlave::update() {
  // We assign random values to all registers except the LED register
  for (int i = 0; i < _registerCount; i++) {
    if (i != _ledRegisterIndex)
      modbusRegisters[i] = random(1, 10000);
  }

  if (Serial.available() >= 8) {
    for (int i = 0; i < 8; i++) {
      requestBuffer[i] = Serial.read();
    }

    if (requestBuffer[0] == _slaveAddress) {
      if (checkCRC(requestBuffer, 6)) {
        if (requestBuffer[1] == 0x03) {
          processReadHoldingRegisters();
        } else if (requestBuffer[1] == 0x06) {
          processWriteSingleRegister();
        }
      }
    }

    while (Serial.available()) Serial.read();
  }

  digitalWrite(_ledPin, modbusRegisters[_ledRegisterIndex] == 1 ? HIGH : LOW);
}

void ModbusRTUSlave::processReadHoldingRegisters() {
  uint16_t startAddress = (requestBuffer[2] << 8) | requestBuffer[3];
  uint16_t registerCount = (requestBuffer[4] << 8) | requestBuffer[5];

  responseBuffer[0] = _slaveAddress;
  responseBuffer[1] = 0x03;
  responseBuffer[2] = registerCount * 2;

  for (int i = 0; i < registerCount; i++) {
    uint16_t regValue = modbusRegisters[startAddress + i];
    responseBuffer[3 + i * 2] = (regValue >> 8) & 0xFF;
    responseBuffer[4 + i * 2] = regValue & 0xFF;
  }

  uint16_t crc = calculateCRC(responseBuffer, 3 + registerCount * 2);
  responseBuffer[3 + registerCount * 2] = crc & 0xFF;
  responseBuffer[4 + registerCount * 2] = (crc >> 8) & 0xFF;

  Serial.write(responseBuffer, 5 + registerCount * 2);
}

void ModbusRTUSlave::processWriteSingleRegister() {
  uint16_t registerAddress = (requestBuffer[2] << 8) | requestBuffer[3];
  uint16_t registerValue = (requestBuffer[4] << 8) | requestBuffer[5];

  if (registerAddress < _registerCount) {
    modbusRegisters[registerAddress] = registerValue;
  }

  for (int i = 0; i < 6; i++) {
    responseBuffer[i] = requestBuffer[i];
  }
  responseBuffer[6] = requestBuffer[6];
  responseBuffer[7] = requestBuffer[7];

  Serial.write(responseBuffer, 8);
}

uint16_t ModbusRTUSlave::calculateCRC(uint8_t *buffer, uint8_t length) {
  uint16_t crc = 0xFFFF;
  for (uint8_t pos = 0; pos < length; pos++) {
    crc ^= (uint16_t)buffer[pos];
    for (uint8_t i = 8; i != 0; i--) {
      if ((crc & 0x0001) != 0) {
        crc >>= 1;
        crc ^= 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  return crc;
}

bool ModbusRTUSlave::checkCRC(uint8_t *buffer, uint8_t length) {
  uint16_t receivedCRC = (buffer[length + 1] << 8) | buffer[length];
  uint16_t calculatedCRC = calculateCRC(buffer, length);
  return (receivedCRC == calculatedCRC);
}

// Setter methods
void ModbusRTUSlave::setSlaveAddress(uint8_t slaveAddress) {
  _slaveAddress = slaveAddress;
}

void ModbusRTUSlave::setLedPin(uint8_t ledPin) {
  _ledPin = ledPin;
  pinMode(_ledPin, OUTPUT);
}

void ModbusRTUSlave::setLedRegisterIndex(uint8_t ledRegisterIndex) {
  if (ledRegisterIndex < _registerCount) {
    _ledRegisterIndex = ledRegisterIndex;
  }
}

void ModbusRTUSlave::setBaudRate(long baudRate) {
  _baudRate = baudRate;
  Serial.begin(_baudRate, SERIAL_8N1);
}