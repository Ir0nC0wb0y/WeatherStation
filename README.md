# WeatherStation
ESP8266 based weather station. All sensors to be built from component parts (closest to raw as possible). Most mechanical parts will be 3D printed (those designs will become available on Thingiverse).

# Required Environment
Development used a NodeMCU (V2 & V3), and the Arduino IDE for programming. There are many tutorials for incorporating the necessary libraries.

The sketch uses the following external libraries:
  - Adafruit_Sensor
  - Adafruit_BME280
  - ESP8266WiFi
  - PuSubClient (MQTT)

# Planned development
Planned sensors:
  - BME280
    - temperature
    - pressure
    - humidity
  - Wind Vane
  - Anemometer
  - Rain Guage

Potential Sensors:
  - Ambient Light

# Solarization

# Wifi/Messaging
Currently, the sketch uses hard coded credentials. This could be changed to use the WifiManager library.

Messaging uses the MQTT protocol, with some ready-made topics.