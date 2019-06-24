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
#include "SoilMonitor.h"

SoilMonitor::SoilMonitor(uint8_t levelPin, uint8_t qualityPin, uint8_t temperaturePin)
{
  this->_levelPin = levelPin;
  this->_qualityPin = qualityPin;
  this->_temperaturePin = temperaturePin;
}

SoilMonitor::SoilMonitor(uint8_t levelPin, uint8_t qualityPin, uint8_t temperaturePin, float dryReading, float wetReading)
{
  this->_levelPin = levelPin;
  this->_qualityPin = qualityPin;
  this->_temperaturePin = temperaturePin;
  this->setCalibration(dryReading, wetReading);
}

void SoilMonitor::begin()
{
  // ***
  // *** Hardware SPI. This works only when the
  // *** pins are used as follows:
  // ***
  // *** GPIO14 [D5] -> CLK   (connect to pin 13 on the MCP3008)
  // *** GPIO11 [D6] -> MISO  (connect to pin 12 on the MCP3008)
  // *** GPIO13 [D7] -> MOSI  (connect to pin 11 on the MCP3008)
  // *** GPIO15 [D8] -> CS    (connect to pin 10 on the MCP3008)
  // ***
  // *** The remaindser of the MCP3008 pin are as follows:
  // *** Pin  9 [DGND] to GND
  // *** Pin 14 [AGND] to GND
  // *** Pin 15 [Vdd] to 3.3V
  // *** Pin 16 [Vref] to 3.3V
  // *** Pin  1 [CH0] to A0 pin on soil moisture monitor
  // *** Pin  2 [CH1] to D0 pin on soil moisture monitor
  // ***
  this->_adc.begin();

  // ***
  // *** Start the Dallas OneWire temperature sensor.
  // ***
  this->_oneWire.begin(this->_temperaturePin);
  this->_ds18b20.setOneWire(&this->_oneWire);
  this->_ds18b20.begin();
}

void SoilMonitor::begin(float dryReading, float wetReading)
{
  this->begin();
  this->setCalibration(dryReading, wetReading);
}

void SoilMonitor::setCalibration(float dryReading, float wetReading)
{
  this->_dryReading = dryReading;
  this->_wetReading = wetReading;
}

float SoilMonitor::getMoistureLevel()
{
  float returnValue = 0.0;

  // ***
  // *** Read the soil moisture sensor analog port
  // *** on the MCP3008.
  // ***
  int value0 = this->_adc.readADC(this->_levelPin);
  float voltage0 = (value0 / 1024.0) * 3.3;
  returnValue = mapF(voltage0, this->_dryReading, this->_wetReading, 0.0, 100.0);

  // ***
  // *** Uncomment to calibrate sensor.
  // ***
  //Serial.print("ADC Voltage = "); Serial.println(voltage0);
  //Serial.print("Level = "); Serial.println(returnValue);

  return returnValue;
}

float SoilMonitor::mapF(float x, float in_min, float in_max, float out_min, float out_max)
{
  return constrain(((x - in_min) * (out_max - out_min)) / ((in_max - in_min) + out_min), 0, 100);
}

String SoilMonitor::getQuality()
{
  String returnValue = "";

  // ***
  // *** Read the soil moisture sensor digital port. It
  // *** is connected to an analog port on MCP3008
  // *** to reserve digital pins on the microcontroller.
  // ***
  int value1 = _adc.readADC(this->_qualityPin);
  float voltage1 = (value1 / 1024.0) * 3.3;

  if (voltage1 >= 2.4)
  {
    returnValue = "Dry";
  }
  else
  {
    returnValue = "Good";
  }

  return returnValue;
}

float SoilMonitor::getTemperature(enum temperatureUnit unit)
{
  float returnValue = 0.0;

  this->_ds18b20.requestTemperatures();
  returnValue = _ds18b20.getTempCByIndex(0);

  if (unit == FAHRENHEIT)
  {
    returnValue = this->convertCtoF(returnValue);
  }

  return returnValue;
}
