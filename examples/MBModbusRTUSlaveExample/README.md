# MBModbusRTUSlave Arduino Library

A simple and flexible Modbus RTU slave library for Arduino.

## Usage

**Modbus Commands**

Turn LED ON: Send 01 06 00 05 00 01 [CRC] (writes 1 to register 5).

Turn LED OFF: Send 01 06 00 05 00 00 [CRC] (writes 0 to register 5).

Read Registers: Send 01 03 00 00 00 0A [CRC] (reads 10 registers starting from 0).

**Customization**

You can adjust settings at runtime:

```cpp
modbus.setSlaveAddress(0x02);        // Change slave address to 0x02
modbus.setLedPin(12);                // Change LED pin to 12
modbus.setLedRegisterIndex(7);       // Use register 7 for LED control
modbus.setModbusBaudRate(115200);    // Change baud rate to 115200
```
