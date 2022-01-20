## Smoke/ Gas detector
### Description
This project aims to develop a Smoke/Combustible Gas detector using ESP32 and MQ-2 sensor. ThingSpeak provides data ingestion and storage for the sensor, with the possibility to build Matlab algorithms to make the system smarter. When a certain threshold is exceeded ESP32 sends an alert email to signal the owner of the building/apartment that  a gas leakage has occurred.
Also to set up wifi credentials without hardcoding the SSID and password directly in code, a prior bluetooth connection is established via smartphone, using Bluetooth Terminal HC-05 application.

### Hardware Implementation
The hardware schematic contains the following devices: <br \>
* ESP32-WROOM-32 (integrated Wifi and Bluetooth)
* MQ-2 Smoke/Combustible Gas sensor
* 5V battery, ar any device that can supply 5V (e.g. Arduino Uno)
