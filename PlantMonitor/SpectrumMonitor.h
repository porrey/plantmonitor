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
#ifndef SPECTRUM_MONITOR_H
#define SPECTRUM_MONITOR_H

#include <Wire.h>
#include <Adafruit_TSL2591.h>

class SpectrumMonitor
{
  public:
    SpectrumMonitor();
    void begin();
    uint32_t getLuminosity(bool = true);
    uint16_t getIr(bool = false);
    uint16_t getFull(bool = false);
    float getLux(bool = false);
    uint16_t getVisible(bool = false);

  private:
    // ***
    // *** Store the last luminosity reading.
    // ***
    uint32_t _luminosity = 0;

    // ***
    // *** Create an instance of TSL2591 UV Sensor.
    // ***
    Adafruit_TSL2591 _tsl = Adafruit_TSL2591(2591);
};
#endif
