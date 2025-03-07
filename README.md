# MBModbusRTUSlave Arduino Library

A simple and flexible Modbus RTU slave library for Arduino.

## Overview

The `MBModbusRTUSlave` library allows Arduino devices to function as Modbus RTU slaves over a serial connection. It supports reading and writing registers via Modbus function codes `0x03` (Read Holding Registers) and `0x06` (Write Single Register), with provisions for LED control based on a designated register value. The library is designed to be customizable, with configurable slave address, LED pin, LED register index, and baud rate.

### Features
- Supports Modbus RTU protocol (function codes `0x03` and `0x06`).
- Configurable slave address, LED pin, and LED register index.
- Flexible baud rate setting via a variable.
- LED control tied to a specific register value (turns ON when the register equals 1).
- Lightweight and easy to integrate into Arduino projects.

### Version
- **1.0.0** (Released: March 07, 2025)

### Author
- S.Mersin (electrocoder) <electrocoder@gmail.com> (Assisted by Grok)

### License
- MIT License (See [LICENSE](#license) section for details)

---

## Installation

1. **Download the Repository**:
   - Clone this repository or download it as a ZIP file:
     ```bash
     git clone https://github.com/electrocoder/MBModbusRTUSlave.git
     ```

2. **Install to Arduino IDE**:
   - Move the `MBModbusRTUSlave` folder to your Arduino `libraries` directory:
     - Windows: `Documents/Arduino/libraries/`
     - macOS/Linux: `~/Documents/Arduino/libraries/`
   - Restart the Arduino IDE.

3. **Include in Your Sketch**:
   - Add the library to your sketch with:
     ```cpp
     #include <MBModbusRTUSlave.h>
     ```

---

## Usage

### Basic Example
This example initializes a Modbus slave with default settings and controls an LED based on register 5.

```cpp
#include <MBModbusRTUSlave.h>

long modbusBaudRate = 9600;  // Customizable baud rate
MBModbusRTUSlave modbus(0x01, 13, 5);  // Slave address: 0x01, LED pin: 13, LED register: 5

void setup() {
  modbus.begin(modbusBaudRate);
  Serial.println("Modbus started");
}

void loop() {
  modbus.update();
}
```

### Modbus Commands
Turn LED ON: Send 01 06 00 05 00 01 [CRC] (writes 1 to register 5).

Turn LED OFF: Send 01 06 00 05 00 00 [CRC] (writes 0 to register 5).

Read Registers: Send 01 03 00 00 00 0A [CRC] (reads 10 registers starting from 0).

### Customization
You can adjust settings at runtime:

```cpp
modbus.setSlaveAddress(0x02);        // Change slave address to 0x02
modbus.setLedPin(12);                // Change LED pin to 12
modbus.setLedRegisterIndex(7);       // Use register 7 for LED control
modbus.setModbusBaudRate(115200);    // Change baud rate to 115200
```

## Hardware Requirements
Any Arduino-compatible board with a serial interface (e.g., Uno, Mega, Nano).

An LED connected to the specified pin (default: pin 13, onboard LED on most Arduino boards).

A Modbus master device or software (e.g., Modbus Poll, QModMaster) for testing.

## Dependencies
None (uses only the Arduino core library).

## Contributing
Contributions are welcome! Feel free to:
Fork this repository.

Create a new branch for your feature or bug fix.

Submit a pull request with a clear description of your changes.

## License
This project is licensed under the MIT License. See the LICENSE file for details.
Acknowledgments
Developed with assistance from Grok, created by xAI.

Inspired by the need for a simple Modbus RTU slave implementation on Arduino.

## Contact
For questions or support, open an issue on this repository or reach out to [electrocoder@gmail.com (mailto:electrocoder@gmail.com)].

