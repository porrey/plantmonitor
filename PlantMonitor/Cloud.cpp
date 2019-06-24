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
#include "Cloud.h"

Cloud::Cloud()
{
  this->_io = new AdafruitIO_WiFi(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

  this->_environmentalTemperatureFeed = this->_io->feed("plant-monitor.environmental-temperature");
  this->_environmentalRelativeHumidityFeed = this->_io->feed("plant-monitor.environmental-relative-humidity");

  this->_soilMoistureLevelFeed = this->_io->feed("plant-monitor.soil-moisture-level");
  this->_soilMoistureQualityFeed = this->_io->feed("plant-monitor.soil-moisture-quality");
  this->_soilTemperatureFeed = this->_io->feed("plant-monitor.soil-temperature");

  this->_spectrumLuxFeed = this->_io->feed("plant-monitor.spectrum-lux");
  this->_spectrumIrFeed = this->_io->feed("plant-monitor.spectrum-ir");
  this->_spectrumFullFeed = this->_io->feed("plant-monitor.spectrum-full");
  this->_spectrumVisibleFeed = this->_io->feed("plant-monitor.spectrum-visible");

  this->_waterPumpFeed = this->_io->feed("plant-monitor.water-pump");
}

// ***
// *** Initializes the Wi-Fi and sets up
// *** the data feeds.
// ***
void Cloud::begin()
{
  // ***
  // *** Connect to io.adafruit.com.
  // ***
  Serial.print("Connecting to the cloud (Adafruit IO)");
  _io->connect();

  // ***
  // *** Wait for a connection...
  // ***
  while (_io->status() < AIO_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  // ***
  // *** We are connected
  // ***
  Serial.println();
  Serial.println(_io->statusText());
}

void Cloud::onWaterPumpChanged(AdafruitIODataCallbackType cb)
{
  this->_waterPumpFeed->onMessage(cb);
}

// ***
// *** Runs the background tasks and listens
// *** for incoming requests.
// ***
void Cloud::process()
{
  _io->run();
}

// ***
// ***
// ***
void Cloud::setWaterPumpSpeed(uint8_t speed)
{
  this->_waterPumpFeed->save(speed);
}

// ***
// *** Takes the cloud data structure and upload
// *** each individual sensor reading to the cloud.
// ***
void Cloud::sendData(CloudData data)
{
  this->_environmentalTemperatureFeed->save(data.environmentalTemperature);
  this->_environmentalRelativeHumidityFeed->save(data.environmentalRelativeHumidity);

  this->_soilMoistureLevelFeed->save(data.soilMoistureLevel);
  this->_soilMoistureQualityFeed->save(data.soilMoistureQuality);
  this->_soilTemperatureFeed->save(data.soilTemperature);

  this->_spectrumLuxFeed->save(data.spectrumLux);
  this->_spectrumIrFeed->save(data.spectrumIr);
  this->_spectrumFullFeed->save(data.spectrumFull);
  this->_spectrumVisibleFeed->save(data.spectrumVisible);
}
