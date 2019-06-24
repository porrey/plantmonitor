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
#ifndef ENVIRONMENTAL_MONITOR_H
#define ENVIRONMENTAL_MONITOR_H

#include <DHTesp.h>
#include "Temperature.h"

class EnvironmentalMonitor : Temperature
{
  public:
    EnvironmentalMonitor(uint8_t);
    void begin();
    float getTemperature(enum temperatureUnit, bool = true);
    float getRelativeHumidity(bool = false);
    float getHeatIndex(enum temperatureUnit);

  private:
    // ***
    // *** This is the pin on which the DHT22 sensor is connected.
    // ***
    uint8_t _temperaturePin;

    // ***
    // *** Create DHT22 instance.
    // ***
    DHTesp _dht;

    // ***
    // *** Stores the last reading.
    // ***
    TempAndHumidity _lastReading;
};
#endif
