## Smoke/ Gas detector
### Prerequisites
* Thingspeak library that can be easily installed from *Library Manager* in Arduino IDE
* To compile the script without errors go to Tools -> Partition Scheme and set it to Huge. The schetch contains multiple lines and it uses a lot of program storage space. 
### Description
This project aims to develop a Smoke/Combustible Gas detector using ESP32 and MQ-2 sensor. ThingSpeak provides data ingestion and storage for the sensor, with the possibility to build Matlab algorithms to make the system smarter. When a certain threshold is exceeded ESP32 sends an alert email to signal the owner of the building/apartment that  a gas leakage has occurred.
Also to set up wifi credentials without hardcoding the SSID and password directly in code, a prior bluetooth connection is established via smartphone, using Bluetooth Terminal HC-05 application.

### Hardware Implementation
The hardware schematic contains the following devices: <br \>
* ESP32-WROOM-32 (integrated Wifi and Bluetooth)
* MQ-2 Smoke/Combustible Gas sensor
* 5V battery, ar any device that can supply 5V (e.g. Arduino Uno)

### Software Implementation
The following steps describe the functionality of the project: < br \>
* boot/reboot device
* introduce Wifi credentials from application and connect to it
* read data coming from MQ-2 sensor
* publish data on ThingSpeak
* if value is above threshold enter in 'aware state' (read 5 times and see if 2 high values above the threshold are met, if so it means that gas is detected in the room)
* if gas detected in room send email to the owner.
