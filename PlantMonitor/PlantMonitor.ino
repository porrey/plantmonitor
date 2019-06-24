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
#include "SoilMonitor.h"
#include "EnvironmentalMonitor.h"
#include "SpectrumMonitor.h"
#include "WaterPumpController.h"
#include "MyPins.h"

// ***
// *** Temperature units to use.
// ***
enum temperatureUnit _myUnits = FAHRENHEIT;

// ***
// *** Define the parameters needed for the Soil Monitor. This is the range
// *** where the sensor behaves in an approximate linear fashion. The values
// *** here represent voltages. Your sensor may need to be calibrated. This 
// *** can be done by taking the voltage reading with the sensor submerged
// *** in water up to the black line. This is the wet value. Next take a voltage
// *** reading with just the tip of the metal electrodes just barely in in 
// *** the water. This is the dry value.
// ***
#define SOIL_MOISTURE_DRY 1.91
#define SOIL_MOISTURE_WET 0.96

// ***
// *** Create an instance of the Soil Monitor.
// ***
SoilMonitor _soilMonitor(SOIL_ANALOG_CHANNEL, SOIL_DIGITAL_CHANNEL, SOIL_TEMPERATURE_PIN, SOIL_MOISTURE_DRY, SOIL_MOISTURE_WET);

// ***
// *** Create an instance of the Environmental Monitor.
// ***
EnvironmentalMonitor _envMonitor(DHT22_DATA_PIN);

// ***
// *** Create an instance of the Spectrum Monitor.
// ***
SpectrumMonitor _spectrumMonitor;

// ***
// *** Create an instance of the Spectrum Monitor.
// ***
WaterPumpController _waterPumpController(WATER_PUMP_PIN);

// ***
// *** Holds the most recent set of sensor readings.
// ***
CloudData _sensorData;

// ***
// *** Create an instance of Cloud.
// ***
Cloud _cloud;

// ***
// *** Setup a timer to read sensors every 10 seconds
// *** and display the results on the sewrial port.
// ***
#define READ_SENSOR_DATA_INTERVAL 10000
os_timer_t _readSensorDataTimer;
volatile bool _readSensorData = false;

// ***
// *** Setup a timer to send the sensor data every
// *** 60 seconds to the cloud.
// ***
#define SEND_SENSOR_DATA_INTERVAL 60000
os_timer_t _sendSensorDataTimer;
volatile bool _sendSensorData = false;

void setup()
{
  // ***
  // *** Start the serial connection.
  // ***
  Serial.begin(230400);

  while (!Serial)
  {
    // ***
    // *** Wait for serial port to connect.
    // ***
  }

  // ***
  // *** Let the system stabilize. This prevents garbage output
  // *** on the serial port.
  // ***
  delay(1000);

  // ***
  // *** Show the startup message.
  // ***
  Serial.println();
  Serial.println("Initializing Plant Monitoring System...");

  // ***
  // *** Initialize the water pump controller.
  // ***
  _waterPumpController.begin();

  // ***
  // *** Initialize the Soil Monitor.
  // ***
  Serial.println("Starting Soil Monitor...");
  _soilMonitor.begin();

  // ***
  // *** Initialize the Environmental Monitor.
  // ***
  Serial.println("Starting Environmental Monitor...");
  _envMonitor.begin();

  // ***
  // *** Initialize the Spectrum Monitor.
  // ***
  Serial.println("Starting Spectrum Monitor...");
  _spectrumMonitor.begin();

  // ***
  // *** Initialize the cloud.
  // ***
  _cloud.begin();
  _cloud.onWaterPumpChanged(handleWaterPumpMessage);
  _cloud.setWaterPumpSpeed(0);

  // ***
  // *** Initialize the sensor data read timer.
  // ***
  os_timer_setfn(&_readSensorDataTimer, readSensorDataTimerCallback, NULL);
  os_timer_arm(&_readSensorDataTimer, READ_SENSOR_DATA_INTERVAL, true);

  // ***
  // *** Initialize the sensor data send timer.
  // ***
  os_timer_setfn(&_sendSensorDataTimer, _sendSensorDataTimerCallback, NULL);
  os_timer_arm(&_sendSensorDataTimer, SEND_SENSOR_DATA_INTERVAL, true);

  // ***
  // *** The system is initialized and ready to go.
  // ***
  Serial.println("Ready.");
}

void loop()
{
  // ***
  // *** This is required for all sketches. It should always be
  // *** present at the top of the loop function. It keeps
  // *** the client connected to io.adafruit.com, and processes
  // *** any incoming data.
  // ***
  _cloud.process();

  // ***
  // *** Read the data if the flag is set.
  // ***
  readSensorData();

  // ***
  // *** Send the data to the cloud if the flag is set.
  // ***
  sendSensorData();

  // ***
  // *** Yield to the microcontroller.
  // ***
  yield();
}

// ***
// *** Called by the timer.
// ***
void readSensorDataTimerCallback(void *pArg)
{
  _readSensorData = true;
}

// ***
// *** Called by the timer.
// ***
void _sendSensorDataTimerCallback(void *pArg)
{
  _sendSensorData = true;
}

// ***
// *** Called by the loop to get sensor data.
// ***
void readSensorData()
{
  // ***
  // *** Read the data if the flag is set.
  // ***
  if (_readSensorData)
  {
    // ***
    // *** Reset the flag.
    // ***
    _readSensorData = false;

    Serial.println("Reading sensor data.");

    // ***
    // *** Read the sensor data.
    // ***
    getSensorData();

    // ***
    // *** Show the data on the serial port.
    // ***
    displaySensorData();
  }
}

// ***
// *** Called by the loop to send sensor data.
// ***
void sendSensorData()
{
  // ***
  // *** Send the data to the cloud if the flag is set.
  // ***
  if (_sendSensorData)
  {
    // ***
    // *** Reset the flag.
    // ***
    _sendSensorData = false;

    if (_sensorData.initialized)
    {
      Serial.println("Sending sensor data to the cloud.");

      // ***
      // *** Send the data tot he cloud.
      // ***
      _cloud.sendData(_sensorData);
    }

    // ***
    // *** Reset the is active flag.
    // ***
    Serial.println("Data send to cloud completed.");
  }
}

// ***
// *** Reads the sensors into the data structure.
// ***
void getSensorData()
{
  _sensorData.environmentalTemperature = _envMonitor.getTemperature(_myUnits);
  _sensorData.environmentalRelativeHumidity = _envMonitor.getRelativeHumidity();
  _sensorData.soilMoistureLevel = _soilMonitor.getMoistureLevel();
  _sensorData.soilMoistureQuality = _soilMonitor.getQuality();
  _sensorData.soilTemperature = _soilMonitor.getTemperature(_myUnits);
  _sensorData.spectrumFull = _spectrumMonitor.getFull(true);
  _sensorData.spectrumIr = _spectrumMonitor.getIr();
  _sensorData.spectrumLux = _spectrumMonitor.getLux();
  _sensorData.spectrumVisible = _spectrumMonitor.getVisible();
  _sensorData.initialized = true;
}

// ***
// *** Display the last sensor data readings
// *** on the serial port.
// ***
void displaySensorData()
{
  if (_sensorData.initialized)
  {
    // ***
    // *** Read the environmental temperature and humidity.
    // ***
    Serial.println();
    Serial.print(F("Air Temperature: ")); Serial.print(_sensorData.environmentalTemperature); Serial.println(_myUnits == FAHRENHEIT ? F(" F") : F(" C"));
    Serial.print(F("Humidity: ")); Serial.print(_sensorData.environmentalRelativeHumidity); Serial.println(F("%"));

    // ***
    // *** Get Soil Readings
    // ***
    Serial.print(F("Soil Temperature is: ")); Serial.print(_sensorData.soilTemperature); Serial.println(_myUnits == FAHRENHEIT ? F(" F") : F(" C"));
    Serial.print(F("Soil Moisture Level: ")); Serial.print(_sensorData.soilMoistureLevel); Serial.println(F(" %"));
    Serial.print(F("Soil Moisture Quality: ")); Serial.println(_sensorData.soilMoistureQuality);

    // ***
    // *** Get Light Spectrum Readings
    // ***
    Serial.print(F("Full: ")); Serial.println(_spectrumMonitor.getFull(true));
    Serial.print(F("IR: ")); Serial.println(_spectrumMonitor.getIr());
    Serial.print(F("Visible: ")); Serial.println(_spectrumMonitor.getVisible());
    Serial.print(F("Lux: ")); Serial.println(_spectrumMonitor.getLux(), 2);

    // ***
    // *** Put an extra blank line in the serial output between readings.
    // ***
    Serial.println();
  }
}

// ***
// *** This function is called whenever a 'plant-monitor.water-pump'
// *** message is received from the cloud. This message sets the
// *** water pump speed from the dashboard.
// ***
void handleWaterPumpMessage(AdafruitIO_Data *data)
{
  Serial.print("Received speed value of "); Serial.print(data->value()); Serial.println(" from the cloud.");
  uint8_t speed = data->toUnsignedInt();
  Serial.print("Setting water pump speed to "); Serial.println(speed);
  _waterPumpController.on(speed);
}
