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
#include "EnvironmentalMonitor.h"

EnvironmentalMonitor::EnvironmentalMonitor(uint8_t temperaturePin)
{
  this->_temperaturePin = temperaturePin;
}

void EnvironmentalMonitor::begin()
{
  this->_dht.setup(this->_temperaturePin, DHTesp::DHT22);
}

float EnvironmentalMonitor::getTemperature(enum temperatureUnit unit, bool forceReading)
{
  float returnValue = 0.0;

  // ***
  // *** Get a reading from the sensor if
  // *** requested otherwise use the last value.
  // ***
  if (forceReading)
  {
    this->_lastReading = this->_dht.getTempAndHumidity();
  }

  // ***
  // *** Get the temperature from the structure returned.
  // ***
  returnValue = this->_lastReading.temperature;

  // ***
  // *** Convert the value if necessary.
  // ***
  if (unit == FAHRENHEIT)
  {
    returnValue = (returnValue * 1.8) + 32.0;
  }

  return returnValue;
}

float EnvironmentalMonitor::getRelativeHumidity(bool forceReading)
{
  // ***
  // *** Get a reading from the sensor if
  // *** requested otherwise use the last value.
  // ***
  if (forceReading)
  {
    this->_lastReading = this->_dht.getTempAndHumidity();
  }

  // ***
  // *** Get the humidity from the structure returned.
  // ***
  return this->_lastReading.humidity;
}

float EnvironmentalMonitor::getHeatIndex(enum temperatureUnit unit)
{
  // ***
  // *** Ported/converted from Adafruit DHT library.
  // *** Using both Rothfusz and Steadman's equations
  // *** http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml
  // ***
  float temperature = this->convertCtoF(this->_lastReading.temperature);
  float percentHumidity = this->_lastReading.humidity;

  float hi = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) + (percentHumidity * 0.094));

  if (hi > 79)
  {
    hi = -42.379 +
         2.04901523 * temperature +
         10.14333127 * percentHumidity +
         -0.22475541 * temperature * percentHumidity +
         -0.00683783 * pow(temperature, 2) +
         -0.05481717 * pow(percentHumidity, 2) +
         0.00122874 * pow(temperature, 2) * percentHumidity +
         0.00085282 * temperature * pow(percentHumidity, 2) +
         -0.00000199 * pow(temperature, 2) * pow(percentHumidity, 2);

    if ((percentHumidity < 13) && (temperature >= 80.0) && (temperature <= 112.0))
    {
      hi -= ((13.0 - percentHumidity) * 0.25) * sqrt((17.0 - abs(temperature - 95.0)) * 0.05882);
    }
    else if ((percentHumidity > 85.0) && (temperature >= 80.0) && (temperature <= 87.0))
    {
      hi += ((percentHumidity - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
    }
  }

  return (unit == FAHRENHEIT) ? hi : this->convertFtoC(hi);
}
