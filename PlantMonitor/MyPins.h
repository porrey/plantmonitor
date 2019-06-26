// Copyright © 2019 Daniel Porrey
//
// This file is part of the Plant Monitor and Watering System.
// 
// This software is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this software. If not, see http://www.gnu.org/licenses/.
//
#ifndef MY_PINS_H
#define MY_PINS_H

// ***
// *** The are the channels on the MCP3008 used by the soil
// *** moisture sensor.
// ***
#define SOIL_ANALOG_CHANNEL 7
#define SOIL_DIGITAL_CHANNEL 6

// ***
// *** The digital pin on which the Dallas Temperature (DS18B20)
// *** sensor is connected.
// ***
#define SOIL_TEMPERATURE_PIN 0

// ***
// *** The digital pin on which the DHT22 is connected.
// ***
#define DHT22_DATA_PIN 10

// ***
// *** The digital pin that controls the water pump.
// ***
#define WATER_PUMP_PIN 2

// ***
// *** The Spectrum Monitor uses a device that is connected 
// *** to the i2c bus. SCL is GPIO5 and SDA is GPIO4. These
// *** pins do not need to be defined.
// ***

#endif
