# ESP32-MQTT-Client-Robot-Car
This program allows one to control a robot car using MQTT messages. It is supposed to be used in conjunction with [NodeJS-MQTT-Robot-Car](https://github.com/robinwesterik/NodeJS-MQTT-Robot-Car)

## Assembly
To build this robot car, one needs the following:
- An ESP32 Microcontroller. I used a DevKitC WROOM32D, available [here](https://www.aliexpress.com/item/4000296658456.html)
- An L298N H-Bridge, available [here](https://www.aliexpress.com/item/4000559513023.html)
- A generic two wheel smart car kit available [here](https://www.aliexpress.com/item/4000427602595.html) (Without the microcontroller!)
  - A video guide on how to build this kit can be found [here](https://youtu.be/3a-bE1VlaU8) 

In total this build should cost you about $20,-
The parts are readily available and pretty generic, so you should also be able to find these in more local shops. The pinout is as follows:
![pinout.png](https://github.com/robinwesterik/ESP32-MQTT-Robot-Car/blob/main/pinout.png)

## Compiling and uploading
The program is created using the Arduino environment. It is highly recommended however to make use of this project via PlatformIO. Importing this project in PlatformIO deals with the dependencies and allows you to easily flash the program to your board without any extra configuration. Do make sure to fill in your right MQTT and Wi-Fi credentials.
