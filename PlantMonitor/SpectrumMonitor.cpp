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
#include "SpectrumMonitor.h"

SpectrumMonitor::SpectrumMonitor()
{
}

void SpectrumMonitor::begin()
{
  // ***
  // *** Configure the TSL2591 gain.
  // ***
  // *** TSL2591_GAIN_LOW: Sets the gain to 1x (bright light)
  // *** TSL2591_GAIN_MED: Sets the gain to 25x (general purpose)
  // *** TSL2591_GAIN_HIGH: Sets the gain to 428x (low light)
  // *** TSL2591_GAIN_MAX: Sets the gain to 9876x (extremely low light)
  // ***
  this->_tsl.setGain(TSL2591_GAIN_MED);

  // ***
  // *** Configure the TSL2591 integration time.
  // ***
  // *** TSL2591_INTEGRATIONTIME_100MS
  // *** TSL2591_INTEGRATIONTIME_200MS
  // *** TSL2591_INTEGRATIONTIME_300MS
  // *** TSL2591_INTEGRATIONTIME_400MS
  // *** TSL2591_INTEGRATIONTIME_500MS
  // *** TSL2591_INTEGRATIONTIME_600MS
  // ***
  this->_tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
}

uint32_t SpectrumMonitor::getLuminosity(bool forceReading)
{
  // ***
  // *** Get a reading from the sensor if
  // *** requested otherwise use the last value.
  // ***
  if (forceReading)
  {
    this->_luminosity = _tsl.getFullLuminosity();
  }

  return this->_luminosity;
}

uint16_t SpectrumMonitor::getIr(bool forceReading)
{
  // ***
  // *** Get a reading from the sensor if
  // *** requested otherwise use the last value.
  // ***
  if (forceReading)
  {
    this->_luminosity = _tsl.getFullLuminosity();
  }
  
  return this->_luminosity >> 16;
}

uint16_t SpectrumMonitor::getFull(bool forceReading)
{
  // ***
  // *** Get a reading from the sensor if
  // *** requested otherwise use the last value.
  // ***
  if (forceReading)
  {
    this->_luminosity = _tsl.getFullLuminosity();
  }
  
  return this->_luminosity & 0xFFFF;
}

float SpectrumMonitor::getLux(bool forceReading)
{
  // ***
  // *** Get a reading from the sensor if
  // *** requested otherwise use the last value.
  // ***
  if (forceReading)
  {
    this->_luminosity = _tsl.getFullLuminosity();
  }
  
  return this->_tsl.calculateLux(this->getFull(), this->getIr());
}

uint16_t SpectrumMonitor::getVisible(bool forceReading)
{
  return this->getFull(forceReading) - this->getIr(forceReading);
}
