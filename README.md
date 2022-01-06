# Automation of Home Lighting with Learned BLE Positioning
**Evan Mitchell**  
Mani Srivastava, ECE M202A  
University of California, Los Angeles

## Abstract
Modern smart home devices are typically interacted with via smartphone apps or by speaking to a virtual assistant. However, for turning on and off smart lights, these methods of interaction are often more of a hassle than simply flipping a traditional light switch. Additionally, while many smart home services provide for manually configured automation, it is often difficult to predict precisely when a particular light will need to be turned on and off, leading to wasted energy. This project aims to both eliminate the need to manually configure smart light automations and minimize the time spent directly interacting with smart lights. These goals are achieved by using online supervised learning to observe the manual switching of smart lights and correlate the room-level position of a user with the signal strengths of nearby household Bluetooth Low Energy (BLE) devices. When an inertial measurement unit (IMU) carried by the user detects significant motion, the neural network is queried with nearby BLE signal strengths to determine whether the user has moved to a different room. If so, the appropriate lights are turned on, and lights in the previous room are turned off. In testing, the system correctly recognized every time the user switched rooms, with rooms being alternated every ten, thirty, and sixty minutes.

## Installation
First, install Home Assistant on a Raspberry Pi and set up your smart lights (or smart outlets with lights plugged in). Then, install the Mosquitto MQTT broker as described in [this tutorial](https://randomnerdtutorials.com/how-to-install-mosquitto-broker-on-raspberry-pi/).

Next, clone this GitHub repository to your Raspberry Pi by running
```
git clone --recursive https://github.com/evanmmitchell/ble-light-automation
```

Modify *software/NanoRP2040Connect/secrets.h*, replacing `127,0,0,1` with the IP address of your Raspberry Pi, and replacing `YOUR_SSID` and `YOUR_PASSWORD` with the SSID and password of your Wi-Fi network. Then, upload the Arduino programs in *software/Arduino/* to the respective Nano boards and connect the RX, TX, and ground pins as shown [here](https://iot-guider.com/arduino/serial-communication-between-two-arduino-boards/).

Modify *software/secrets.py*, replacing `YOUR_TOKEN` with a long-lived access token obtained from Home Assistant. Modify the `LIGHT_ENTITIES` variable in *software/ble_light_automation.py* to match your Home Assistant setup. Note that only two rooms are supported at this time.

## Usage
Run *software/ble_light_automation.py* while both Arduino Nanos are operating.

## Submissions
* [Proposal](docs/proposal.md)
* [Midterm Checkpoint Presentation Slides](docs/checkpoint.pdf)
* [Midterm Checkpoint Presentation Recording](https://youtu.be/0vCRBBYPsWg)
* [Final Presentation Slides](docs/presentation.pdf)
* [Final Presentation Recording](https://youtu.be/WkyJOP_pBwI)
* [Final Report](docs/report.md)
