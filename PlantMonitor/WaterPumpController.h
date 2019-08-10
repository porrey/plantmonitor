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
#ifndef WATER_PUMP_CONTROLLER_H
#define WATER_PUMP_CONTROLLER_H

#include <Arduino.h>

// ***
// *** The pump will not move at PWM values below this threshold.
// ***
#define MINIMUM_PUMP_PWM  400

class WaterPumpController
{
  public:
    WaterPumpController(uint8_t);
    void begin();
    void off();
    void on();
    void on(uint8_t);
    bool on(uint8_t, uint32_t);
    bool isOn();

  private:
    // ***
    // *** The PWM pin on which the pump is connected.
    // ***
    uint8_t _pin;

    // ***
    // *** Flag to keep track of the pump state.
    // ***
    bool _isOn = false;
};
#endif
