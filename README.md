# Arduino Library for PoolReader boards

## Introduction

This library is the interface with the PoolReader board. Communication are made using a single 1-Wire bus.


The library is divided in two parts.
1. The Client part, that act as the client.
2. The Device part that is used inside an ATTNY85 on the PoolReader board.


The board is available in the repo named Hardware-PoolReader (https://github.com/Willena/Hardware-PoolReader).

## Structure

The code is architectured around two main classes: PoolReaderDevice, and PoolReaderClient. 
The first class is designed to be used for device emulation (OneWire device).
The second class is the library client that will send and receive message from the device.

### Requirements

The library is built around some other libraries:

- OneWire : https://github.com/PaulStoffregen/OneWire
- OneWireHub : https://github.com/orgua/OneWireHub

### Types

#### `pool_reader_register_t`

This a shared type between the client and the device class. It contains captured raw values for each measurment. 

- Temperature: 16 bits integer (same as the DS18B20 device)
- pH: 16 bits unsigned integer (to store the value of the ADC from 0 to 1024)
- ORP: 16 bits unsigned integer (to store the value of the ADC from 0 to 1024)
- Water levle: 8 bits unsigned integer (values are stored from 0 to 255, max distance is 25.5 cm)
- Sample interval: 8 bits unsigned integer (value in seconds, max is 255 second interval, min is one second)
- CRC: The OneWire CRC value over one byte.

#### `PoolReaderDevice::family_code` and `PoolReaderClient::family_code`

The `family_code` identifies the type of device on the bus. All devices with an address starting by `0xB7`are PoolReader boards. The family code is then folowed by the uniq address of the device.

### PoolReaderDevice

PoolReaderDevice is a subclass of OneWireItem from the OneWireHub library. It allow us to describe the commands and how it should answer them. 

#### Protocol definition

The communication protocol is rather simple. Over OneWire, two commands are defined.

- READ_MEASURMENTS (0x6B): When this command is received, the device returns a 9 bytes structure representing `pool_reader_register_t` type.
- SET_SAMPLE_INTERVAL (0x6C): This command followed by the sample interval value will change how frequently the device actualize the measurment register


### Examples

See the PoolReaderDevice example in the examples folder.

### PoolReaderClient class

The PoolReaderClient interract with the device and allow you to set and retrieve the raw structure from the device. 
It also provide callibration functions for the pH value as well as a rather simple ATC if the pH is retrieved given a temperature value.

### Examples

See the PoolReaderClient example in the examples folder.

## Licence

```
PoolReader Hardware, Firmware, Library
Copyright 2020 Guillaume VILLENA aka "Willena"

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```
