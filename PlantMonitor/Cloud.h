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
#ifndef CLOUD_H
#define CLOUD_H


#include "Credentials.h"
#include "AdafruitIO_WiFi.h"

typedef struct cloudData
{
  bool initialized;

  float environmentalTemperature;
  float environmentalRelativeHumidity;

  float soilMoistureLevel;
  String soilMoistureQuality;
  float soilTemperature;

  uint16_t spectrumIr;
  uint16_t spectrumFull;
  float spectrumLux;
  uint16_t spectrumVisible;

} CloudData;

class Cloud
{
  public:
    Cloud();
    void begin();
    void process();
    void sendData(CloudData);
    void onWaterPumpChanged(AdafruitIODataCallbackType);
    void setWaterPumpSpeed(uint8_t speed);
    
  private:
    // ***
    // *** Setup an instance of ther IO service.
    // ***
    AdafruitIO_WiFi* _io;

    // ***
    // *** Create the data feeds.
    // ***
    AdafruitIO_Feed* _environmentalTemperatureFeed;
    AdafruitIO_Feed* _environmentalRelativeHumidityFeed;

    AdafruitIO_Feed* _soilMoistureLevelFeed;
    AdafruitIO_Feed* _soilMoistureQualityFeed;
    AdafruitIO_Feed* _soilTemperatureFeed;

    AdafruitIO_Feed* _spectrumLuxFeed;
    AdafruitIO_Feed* _spectrumIrFeed;
    AdafruitIO_Feed* _spectrumFullFeed;
    AdafruitIO_Feed* _spectrumVisibleFeed;

    AdafruitIO_Feed* _waterPumpFeed;
};
#endif
