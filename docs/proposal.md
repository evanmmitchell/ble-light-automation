# Project Proposal

## 1. Motivation & Objective

<!--
What are you trying to do and why? (plain English without jargon)
-->

Modern smart home devices are typically interacted with via dedicated smartphone apps or by speaking to a virtual assistant. However, for turning on and off smart lights, these methods of interaction are often more of a hassle than simply flipping a traditional light switch. Many smart home apps and services provide for manually-configured automation, which allows the automatic switching of lights at specific times or when someone arrives (often detected by monitoring the GPS location or network connectivity of a smartphone). This can help to minimize the hassle of manually interacting with apps or a virtual assistant, but it is often hard to predict precisely when a particular light will need to be turned on and off, making it difficult to use time as an effective trigger for light switching. Additionally, the increased presence of Bluetooth Low Energy (BLE) devices, such as smart speakers, smart TVs, and smart washers and dryers, as well as Bluetooth-enabled computers and laptops, in typical homes provide relatively static reference points for more accurate position inference.

This project aims to both eliminate the need to manually configure smart light automations and minimize the need to directly interact with smart lights. These goals are achieved by using unsupervised learning to observe patterns in the manual switching of smart lights based on time, user motion, and inferred user position. The time component includes knowledge about both time of day and day of week. General magnitude and duration of significant user motion is detected by an inertial measurement unit (IMU) carried on the user's person. Finally, user position is inferred based on the signal strength of BLE devices nearby the user, which approximates the distance to those devices. The observed pattern are then used to predict and perform future light operations. Any incorrect operations will be recognized as such when the user counteracts them, and this feedback will be used to update the system's prediction-making.

For this project, Home Assistant [[1](#1)] will be used to operate the smart home setup due to its wide compatibility with smart home devices and its open approach to customization. Home Assistant will be running on a Raspberry Pi 4, which has built-in Bluetooth support. Motion detection and BLE scanning will be performed by an Arduino Nano 33 BLE Sense, which is portable enough to be kept on the user's person and incorporates both an IMU and a BLE radio.

## 2. State of the Art & Its Limitations

<!--
How is it done today, and what are the limits of current practice?
-->

As described above, smart home devices are typically controlled via smartphone apps or virtual assistants. This can make turning lights on and off quite a hassle. When you walk into a dark room, you either have to pull out your smartphone and open an app or speak to a virtual assistant through a smart speaker. Compared to flipping a light switch, this is a rather large price to pay for having smart lights. While smart light switches are available, this mitigates the benefit of purchasing smart lights.

While many smart home services allow for manually-configured automation, one of the primary benefits of smart lighting, this requires determining triggers like specific times of day or GPS-based position. Since few people keep schedules to the minute, there is often energy wasted when lights turn on or off before or after they are actually needed.

Room Assistant [[2](#2)] provides room-scale positioning by using a network of BLE-equipped Linux computers, one in each room, to track a user's smartphone or smartwatch. The need to station a Raspberry Pi or similar computer in every room, however, limits Room Assistant's practicability. Additionally, automations must still be manually configured.

## 3. Novelty & Rationale

<!--
What is new in your approach and why do you think it will be successful?
-->

This project's unsupervised learning approach to home light automation is a novel one. However, use of similar techniques seems to be the future of the smart home industry. Tom Taylor, senior vice president of Alexa at Amazon, recently said, "We believe that the future of consumer technology is ambient intelligence, which uses AI to weave together devices and intelligent services" [[3](#3)]. The convenience of unsupervised learning over manually-configured automation would likely make this project's approach successful.

This project's use of existing household BLE devices for positioning is also novel. Given the increasing number of BLE-equipped devices positioned throughout the average household, Room Assistant's approach of having Raspberry Pi or similar computers in each room is largely unnecessary. This project's approach requires less cost and setup effort for similar benefits, likely making it more popular.

## 4. Potential Impact

<!--
If the project is successful, what difference will it make, both technically and broadly?
-->

If this project is successful, it has the potential to begin changing the mindset around smart home automation. The idea of using unsupervised learning to intelligently control smart devices can be extended far beyond turning on and off lights. Additionally, the functions of the Arduino Nano can be performed by smartphones, which already have the IMUs and BLE radios, and similar unsupervised learning code can be directly incorporated into smart home services like Home Assistant. More broadly, people will not need to waste time manually controlling home appliances like lights except to initially teach the artificial intelligence and to correct it if its predictions are wrong.

## 5. Challenges

<!--
What are the challenges and risks?
-->

Implementing the unsupervised learning to recognize how patterns in time, motion, and position correspond with light switching is the most daunting challenge. Realizing BLE communication between the Arduino Nano and Home Assistant may also be a difficult endeavor. An additional risk is that there might not be enough BLE devices around the house to infer position accurately.

## 6. Requirements for Success

<!--
What skills and resources are necessary to perform the project?
-->

This project requires familiarity with both Arduino and Python. Experience with Linux on Raspberry Pi will be useful for setting up Home Assistant. Additionally, knowledge of unsupervised learning will be necessary.

With regard to hardware, this project requires a Raspberry Pi 4 and an Arduino Nano 33 BLE Sense with a portable charger. Smart light bulbs or outlets compatible with Home Assistant are also needed, along with a wireless network connection. Finally, several stationary BLE devices are necessary for position inference.

## 7. Metrics of Success

<!--
What are metrics by which you would check for success?
-->

This project will be considered successful if at least 80% of light operations are predicted accurately on the fifth day of real-life operation. Note that the user is presumed to be only person interacting with the smart lights. Note also that only two rooms are tested due to both the limited number of available rooms for testing and budget constraints for purchasing smart light bulbs or outlets, although the project will likely produce the proper results with a larger number of rooms given sufficient BLE devices distributed among them.

Several intermediate tests will also be performed to verify proper operation of the different components. First, proper communication between the Arduino Nano and Home Assistant will be verified. Second, I will test the Arduino's ability to distinguish between the living room and the bedroom based on proximity to recognized BLE devices after scanning ten times in each room. Next, the system will be tested using mock historical data with little variation, testing for the ability to successfully detect patterns and make accurate predictions. Finally, mock historical data with more realistic variation will be used to validate the system, testing for the ability to generalize pattern detection and prediction. In the case of an issue with the real-life test, the final mock test will be used to validate the system.

## 8. Execution Plan

<!--
Describe the key tasks in executing your project, and in case of team project describe how will you partition the tasks.
-->

1. Install and set up Home Assistant on Raspberry Pi.
1. Purchase compatible smart light bulbs or outlets and set them up with Home Assistant.
1. Write code to perform BLE communication between Arduino and Home Assistant. This includes support for using Arduino to turn the lights on and off, along with reporting changes in the lights' status to Arduino.
1. Write code to infer position based on the signal strength of remembered nearby BLE devices.
1. Write code to perform unsupervised learning to associate light status changes with time, IMU motion, and inferred position. Arduino should make light status predictions based on historical patterns and act on them if above a specified certainty.
1. Verify metrics of success.

## 9. Related Work

### 9.a. Papers

<!--
List the key papers that you have identified relating to your project idea, and describe how they related to your project. Provide references (with full citation in the References section below).
-->

The paper "Indoor positioning system based on BLE location fingerprinting with classification approach" [[4](#4)] addresses a technique for establishing a BLE fingerprint for a given location. This may prove useful for recognizing a previous room based on nearby BLE signals. "Smart indoor positioning using BLE technology" [[5](#5)] provides information about additional techniques for identifying a position based on BLE signals.

The paper "Unsupervised Learning" [[6](#6)] introduces the idea and may prove useful in the development of the unsupervised learning system.

### 9.b. Datasets

<!--
List datasets that you have identified and plan to use. Provide references (with full citation in the References section below).
-->

There is no planned use of datasets for this project.

### 9.c. Software

<!--
List softwate that you have identified and plan to use. Provide references (with full citation in the References section below).
-->

Home Assistant [[1](#1)] will be used to manage the smart home integration. The ArduinoBLE Arduino library [[7](#7)] and the bluepy Python library [[8](#8)] will be utilized for performing Bluetooth communication between the Nano and Home Assistant, with the ArduinoBLE library also facilitating the scanning of nearby BLE devices. The LSM9DS1 Arduino library [[9](#9)] will be used for receiving acceleration data from onboard IMU. The Python library scikit-learn [[10](#10)] is being evaluated for performing unsupervised learning. Additionally, Room Assistant [[2](#2)] may serve as a reference for performing accurate BLE position inference.

## 10. References

<!--
List references correspondign to citations in your text above. For papers please include full citation and URL. For datasets and software include name and URL.
-->

[<a name="1">1</a>] Home Assistant. Available: <https://www.home-assistant.io>  
[<a name="2">2</a>] Room Assistant. Available: <https://room-assistant.io>  
[<a name="3">3</a>] S. Shead, "Amazon wants us to stop talking to Alexa so much," CNBC. Available: <https://www.cnbc.com/2021/11/02/amazon-wants-us-to-stop-talking-to-alexa-so-much.html>  
[<a name="4">4</a>] Y. Pu, P. You, "Indoor positioning system based on BLE location fingerprinting with classification approach," Applied Mathematical Modelling, vol. 62, pp. 654-663, 2018, doi: 10.1016/j.apm.2018.06.031.  
[<a name="5">5</a>] S. Memon, M. M. Memon, F. K. Shaikh and S. Laghari, "Smart indoor positioning using BLE technology," IEEE International Conference on Engineering Technologies and Applied Sciences, pp. 1-5, 2017, doi: 10.1109/ICETAS.2017.8277872.  
[<a name="6">6</a>] H. B. Barlow, "Unsupervised Learning," Neural Computation, vol. 1, iss. 3, pp. 295–311, 1989, doi: 10.1162/neco.1989.1.3.295.  
[<a name="7">7</a>] ArduinoBLE. Available: <https://www.arduino.cc/en/Reference/ArduinoBLE>  
[<a name="8">8</a>] bluepy. Available: <https://github.com/IanHarvey/bluepy>  
[<a name="9">9</a>] Arduino LSM9DS1. Available: <https://www.arduino.cc/en/Reference/ArduinoLSM9DS1>  
[<a name="10">10</a>] scikit-learn. Available: <https://scikit-learn.org>  
