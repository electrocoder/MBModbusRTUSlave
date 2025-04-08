/*
 * MBModbusRTUSlave.cpp - Implementation of the Modbus RTU Slave Library for Arduino
 *
 * Description: This file implements the methods of the MBModbusRTUSlave class.
 *              It supports Modbus RTU protocol for register reading (0x03) and writing (0x06).
 *              Provides flexible baud rate adjustments and RS485 transceiver support.
 * Author: S.Mersin (electrocoder) <electrocoder@gmail.com> (Assisted by Grok)
 * Date: April 08, 2025
 * Version: 1.1.0
 * License: MIT License
 *
 * Note: This library uses the Arduino Serial port to provide Modbus RTU slave functionality.
 *       All registers except the LED register are updated with random values.
 */

#include "MBModbusRTUSlave.h"

MBModbusRTUSlave::MBModbusRTUSlave(uint8_t slaveAddress, HardwareSerial *serialPort, uint8_t rs485ControlPin, uint16_t registerCount)
{
  _slaveAddress = slaveAddress;
  _modbusBaudRate = 9600;
  _serialPort = serialPort;
  _rs485ControlPin = rs485ControlPin;
  _registerCount = registerCount;

  modbusRegisters = new uint16_t[_registerCount];
  modbusRegisters[0] = 0;
  modbusRegisters[1] = 5678;
  modbusRegisters[2] = 90;
  for (int i = 3; i < _registerCount; i++)
  {
    modbusRegisters[i] = 0;
  }
}

MBModbusRTUSlave::~MBModbusRTUSlave()
{
  delete[] modbusRegisters; // Delete registers
}

void MBModbusRTUSlave::begin(long modbusBaudRate)
{
  _modbusBaudRate = modbusBaudRate;
  pinMode(_rs485ControlPin, OUTPUT);
  digitalWrite(_rs485ControlPin, LOW);
  _serialPort->begin(_modbusBaudRate, SERIAL_8N1);
  while (!_serialPort)
    ;
}

bool MBModbusRTUSlave::readRegisters()
{
  digitalWrite(_rs485ControlPin, LOW); // Receiver
  if (_serialPort->available() >= 8)
  {
    for (int i = 0; i < 8; i++)
    {
      requestBuffer[i] = _serialPort->read();
    }

    if (requestBuffer[0] == _slaveAddress && checkCRC(requestBuffer, 6))
    {
      if (requestBuffer[1] == 0x03)
      {
        processReadHoldingRegisters();
      }
      else if (requestBuffer[1] == 0x06)
      {
        processWriteSingleRegister();
      }
      while (_serialPort->available())
        _serialPort->read();
      return true;
    }
  }
  return false;
}

void MBModbusRTUSlave::writeFloatRegister(uint16_t registerAddress, float value)
{
  if (registerAddress < _registerCount - 1)
  {
    uint32_t floatBits;
    memcpy(&floatBits, &value, sizeof(float));
    modbusRegisters[registerAddress] = (floatBits >> 16) & 0xFFFF;
    modbusRegisters[registerAddress + 1] = floatBits & 0xFFFF;
  }
}

void MBModbusRTUSlave::writeRegister(uint16_t registerAddress, uint16_t value)
{
  if (registerAddress < _registerCount)
  {
    modbusRegisters[registerAddress] = value;
  }
}

uint16_t MBModbusRTUSlave::getRegister(uint16_t registerAddress)
{
  if (registerAddress < _registerCount)
  {
    return modbusRegisters[registerAddress];
  }
  return 0;
}

float MBModbusRTUSlave::getFloatRegister(uint16_t registerAddress)
{
  if (registerAddress < _registerCount - 1)
  {
    uint32_t floatBits = ((uint32_t)modbusRegisters[registerAddress] << 16) | modbusRegisters[registerAddress + 1];
    float value;
    memcpy(&value, &floatBits, sizeof(float));
    return value;
  }
  return 0.0;
}

void MBModbusRTUSlave::processReadHoldingRegisters()
{
  uint16_t startAddress = (requestBuffer[2] << 8) | requestBuffer[3];
  uint16_t registerCount = (requestBuffer[4] << 8) | requestBuffer[5];

  if (startAddress + registerCount <= _registerCount)
  { // Register check
    responseBuffer[0] = _slaveAddress;
    responseBuffer[1] = 0x03;
    responseBuffer[2] = registerCount * 2;

    for (int i = 0; i < registerCount; i++)
    {
      uint16_t regValue = modbusRegisters[startAddress + i];
      responseBuffer[3 + i * 2] = (regValue >> 8) & 0xFF;
      responseBuffer[4 + i * 2] = regValue & 0xFF;
    }

    uint16_t crc = calculateCRC(responseBuffer, 3 + registerCount * 2);
    responseBuffer[3 + registerCount * 2] = crc & 0xFF;
    responseBuffer[4 + registerCount * 2] = (crc >> 8) & 0xFF;

    digitalWrite(_rs485ControlPin, HIGH);
    _serialPort->write(responseBuffer, 5 + registerCount * 2);
    _serialPort->flush();
    digitalWrite(_rs485ControlPin, LOW);
  }
}

void MBModbusRTUSlave::processWriteSingleRegister()
{
  uint16_t registerAddress = (requestBuffer[2] << 8) | requestBuffer[3];
  uint16_t registerValue = (requestBuffer[4] << 8) | requestBuffer[5];

  if (registerAddress < _registerCount)
  {
    modbusRegisters[registerAddress] = registerValue;
  }

  for (int i = 0; i < 6; i++)
  {
    responseBuffer[i] = requestBuffer[i];
  }
  responseBuffer[6] = requestBuffer[6];
  responseBuffer[7] = requestBuffer[7];

  digitalWrite(_rs485ControlPin, HIGH);
  _serialPort->write(responseBuffer, 8);
  _serialPort->flush();
  digitalWrite(_rs485ControlPin, LOW);
}

uint16_t MBModbusRTUSlave::calculateCRC(uint8_t *buffer, uint8_t length)
{
  uint16_t crc = 0xFFFF;
  for (uint8_t pos = 0; pos < length; pos++)
  {
    crc ^= (uint16_t)buffer[pos];
    for (uint8_t i = 8; i != 0; i--)
    {
      if ((crc & 0x0001) != 0)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      {
        crc >>= 1;
      }
    }
  }
  return crc;
}

bool MBModbusRTUSlave::checkCRC(uint8_t *buffer, uint8_t length)
{
  uint16_t receivedCRC = (buffer[length + 1] << 8) | buffer[length];
  uint16_t calculatedCRC = calculateCRC(buffer, length);
  return (receivedCRC == calculatedCRC);
}

void MBModbusRTUSlave::setSlaveAddress(uint8_t slaveAddress)
{
  _slaveAddress = slaveAddress;
}

void MBModbusRTUSlave::setModbusBaudRate(long modbusBaudRate)
{
  _modbusBaudRate = modbusBaudRate;
  _serialPort->begin(_modbusBaudRate, SERIAL_8N1);
}

void MBModbusRTUSlave::setSerialPort(HardwareSerial *serialPort)
{
  _serialPort = serialPort;
  _serialPort->begin(_modbusBaudRate, SERIAL_8N1);
}

void MBModbusRTUSlave::setRS485ControlPin(uint8_t rs485ControlPin)
{
  _rs485ControlPin = rs485ControlPin;
  pinMode(_rs485ControlPin, OUTPUT);
  digitalWrite(_rs485ControlPin, LOW);
}

uint16_t MBModbusRTUSlave::getRegisterCount()
{
  return _registerCount;
}
