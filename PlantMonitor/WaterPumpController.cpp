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
#include "WaterPumpController.h"

WaterPumpController::WaterPumpController(uint8_t pin)
{
  this->_pin = pin;
}

void WaterPumpController::begin()
{
  // ***
  // *** Set the pion to output.
  // ***
  pinMode(this->_pin, OUTPUT);

  // ***
  // *** Turn the pump off.
  // ***
  this->off();
}

bool WaterPumpController::isOn()
{
  // ***
  // *** Return the value of the
  // *** internal flag.
  // ***
  return this->_isOn;
}

void WaterPumpController::off()
{
  // ***
  // *** Setting the pin to LOW
  // *** turns the pump off.
  // ***
  digitalWrite(this->_pin, LOW);

  // ***
  // *** Set the is on flag.
  // ***
  this->_isOn = false;
}

void WaterPumpController::on()
{
  // ***
  // *** Setting the pin to HIGH
  // *** turns the pump on.
  // ***
  digitalWrite(this->_pin, HIGH);

  // ***
  // *** Set the is on flag.
  // ***
  this->_isOn = true;
}

// ***
// *** Speed is a value from 0 to 255. 0 is
// *** defined as "off" and 255 is full
// *** speed "on".
// ***
void WaterPumpController::on(uint8_t speed)
{
  if (speed == 0)
  {
    this->off();
  }
  else if (speed == 255)
  {
    this->on();
  }
  else
  {
    // ***
    // *** Map a value from 0 to 255 to a range
    // *** the valid PWM range.
    // ***
    uint16_t value = map(speed, 0, 255, MINIMUM_PUMP_PWM, PWMRANGE);

    // ***
    // *** Set the PWM.
    // ***
    analogWrite(this->_pin, value);

    // ***
    // *** Set the is on flag.
    // ***
    this->_isOn = true;
  }
}

bool WaterPumpController::on(uint8_t speed, uint32_t duration)
{
  uint32_t stamp = millis();

  this->on(speed);

  while ((millis() - stamp) <= duration)
  {
    yield();
  }

  this->off();
}
