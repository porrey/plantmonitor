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
#ifndef SOIL_MONITOR_H
#define SOIL_MONITOR_H

#include <Adafruit_MCP3008.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Temperature.h"

class SoilMonitor : Temperature
{
  public:
    SoilMonitor(uint8_t, uint8_t, uint8_t);
    SoilMonitor(uint8_t, uint8_t, uint8_t, float, float);
    void begin();
    void begin(float, float);
    void setCalibration(float, float);
    float getMoistureLevel();
    String getQuality();
    float getTemperature(enum temperatureUnit);

  private:
    // ***
    // *** This is the pin on which the Dallas temperature sensor is connected.
    // ***Units.h
    uint8_t _temperaturePin;

    // ***
    // *** The MCP3008 channel on which the soil moisture analog pin is connected.
    // ***
    uint8_t _levelPin;

    // ***
    // *** The MCP3008 channel on which the soil moisture digital pin is connected.
    // ***
    uint8_t _qualityPin;

    // ***
    // *** They dry and wet values are used to calibrate the soil
    // *** moisture sensor to get a reading between 0 and 100%. This
    // *** calibration assumes a linear relationship.
    // ***
    
    // ***
    // *** This is the voltage reading when the sensor is completley dry.
    // *** 
    float _dryReading = 3.3;
    
    // ***
    // *** This is the voltage reading when the sensor is completley wet.
    // ***
    float _wetReading = 0.0;

    // ***
    // *** Use the Adafruit library to connect to the MCP3008.
    // ***
    Adafruit_MCP3008 _adc;

    // ***
    // *** Setup a oneWire instance to communicate with any OneWire 
    // *** devices (not just Maxim/Dallas temperature ICs).
    // ***
    OneWire _oneWire = OneWire(0);
    
    // ***
    // *** Create an instance of DS18B20.
    // ***
    DallasTemperature _ds18b20 = DallasTemperature();

    float mapF(float x, float in_min, float in_max, float out_min, float out_max);
};
#endif
