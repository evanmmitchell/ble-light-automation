# Table of Contents
* Abstract
* [Introduction](#1-introduction)
* [Related Work](#2-related-work)
* [Technical Approach](#3-technical-approach)
* [Evaluation and Results](#4-evaluation-and-results)
* [Discussion and Conclusions](#5-discussion-and-conclusions)
* [References](#6-references)

# Abstract

<!--
Provide a brief overview of the project objectives, approach, and results.
-->

Modern smart home devices are typically interacted with via smartphone apps or by speaking to a virtual assistant. However, for turning on and off smart lights, these methods of interaction are often more of a hassle than simply flipping a traditional light switch. Additionally, while many smart home services provide for manually-configured automation, it is often difficult to predict precisely when a particular light will need to be turned on and off, leading to wasted energy. This project aims to both eliminate the need to manually configure smart light automations and minimize the time spent directly interacting with smart lights. These goals are achieved by using online supervised learning to observe the manual switching of smart lights and correlate the room-level position of a user with the signal strengths of nearby household Bluetooth Low Energy (BLE) devices. When an inertial measurement unit (IMU) carried by the user detects significant motion, the neural network is queried with nearby BLE signal strengths to determine whether the user has moved to a different room. If so, the appropriate lights are turned on, and lights in the previous room are turned off. In testing, the system correctly recognized every time the user switched rooms, with rooms being alternated every ten, thirty, and sixty minutes.

# 1. Introduction

<!--
This section should cover the following items:

* Motivation & Objective: What are you trying to do and why? (plain English without jargon)
* State of the Art & Its Limitations: How is it done today, and what are the limits of current practice?
* Novelty & Rationale: What is new in your approach and why do you think it will be successful?
* Potential Impact: If the project is successful, what difference will it make, both technically and broadly?
* Challenges: What are the challenges and risks?
* Requirements for Success: What skills and resources are necessary to perform the project?
* Metrics of Success: What are metrics by which you would check for success?
-->


## Motivation & Objective

<!--
What are you trying to do and why? (plain English without jargon)
-->

Modern smart home devices are typically interacted with via dedicated smartphone apps or by speaking to a virtual assistant. However, for turning on and off smart lights, these methods of interaction are often more of a hassle than simply flipping a traditional light switch. Many smart home apps and services provide for manually-configured automation, which allows the automatic switching of lights at specific times or when someone arrives home (often detected by monitoring the GPS location or network connectivity of a smartphone). This can help to minimize the hassle of manually interacting with apps or a virtual assistant, but it is often hard to predict precisely when a particular light will need to be turned on and off, making it difficult to use time as an effective trigger for light switching. Additionally, the increased presence of Bluetooth Low Energy (BLE) devices, such as smart speakers, smart TVs, and smart washers and dryers, as well as Bluetooth-enabled computers and laptops, in typical homes provide relatively static reference points for more accurate position inference.

This project aims to both eliminate the need to manually configure smart light automations and minimize the need to directly interact with smart lights. These goals are achieved by using online supervised learning to observe the manual switching of smart lights and correlate the room-level position of a user with the signal strengths of nearby household Bluetooth Low Energy (BLE) devices, which approximates the distances to those devices. Significant user motion, which indicates that a user might be changing rooms, is detected by an inertial measurement unit (IMU) carried on the user's person. This significant motion triggers a scan of nearby BLE devices and their signal strengths, the results of which are used to query the neural network and determine whether the user has changed rooms. If so, the appropriate lights are turned on, and lights in the previously occupied room are turned off.

For this project, Home Assistant [[1](#1)] is used to operate the smart home setup due to its wide compatibility with smart home devices and its open approach to customization. Home Assistant is running on a Raspberry Pi 4, which also hosts an MQTT broker and the neural network Python script. BLE scanning is performed by an Arduino Nano 33 BLE Sense, and motion detection and MQTT publishing is handled by an Arduino Nano RP2040 Connect.

## State of the Art & Its Limitations

<!--
How is it done today, and what are the limits of current practice?
-->

As described above, smart home devices are typically controlled via smartphone apps or virtual assistants. This can make turning lights on and off quite a hassle. When you walk into a dark room, you either have to pull out your smartphone and open an app or speak to a virtual assistant through a smart speaker. Compared to flipping a light switch, this is a rather large price to pay for having smart lights. While some smart light switches are available to help with this issue, this severely diminishes the benefit of purchasing smart lights.

Many smart home services allow for manually-configured automation, one of the primary benefits of smart lighting, but this requires determining triggers like specific times of day or GPS-based position. Since few people keep schedules to the minute, there is often energy wasted when lights turn on or off before or after they are actually needed.

Room Assistant [[2](#2)] provides room-scale positioning by using a network of BLE-equipped Linux computers, one in each room, to track a user's smartphone or smartwatch. The need to station a Raspberry Pi or similar computer in every room, however, limits Room Assistant's practicability. Additionally, automations must still be manually configured.

## Novelty & Rationale

<!--
What is new in your approach and why do you think it will be successful?
-->

This project's online supervised learning approach to home light automation is a novel one. However, use of similar techniques seems to be the future of the smart home industry. For example, V. H. Bhide et al. discuss using a Naive Bayes Classifier to perform data mining and learn home automation [[3](#3)]. Tom Taylor, senior vice president of Alexa at Amazon, recently said, "We believe that the future of consumer technology is ambient intelligence, which uses AI to weave together devices and intelligent services" [[4](#4)]. The convenience of online learning over manually-configured automation would undoubtedly make this project's approach successful.

This project's use of existing household BLE devices for room-level positioning is also novel. Given the increasing number of BLE-equipped devices positioned throughout the average household, it is largely unnecessary to position BLE beacons in each room in order to perform position tracking. This project's approach requires less cost and setup effort for similar benefits.

Finally, the implementation of dynamic features in the project's neural network is believed to be novel. The full number of weights, representing the maximum number of BLE devices used as features for classification, are initialized, but only a subset of them, matching the seen BLE devices, are trained. As new BLE devices are seen, they are assigned as new features which update the previously untrained weights. After all available features have been assigned, the least recently seen BLE device is replaced with the new one, and the associated weight is dynamically reinitialized. This implementation allows for a dynamic feature set while keeping the maximum number of features fixed to prevent memory issues.

## Potential Impact

<!--
If the project is successful, what difference will it make, both technically and broadly?
-->

If this project is successful, it has the potential to begin changing the mindset around smart home automation. The idea of using online learning to intelligently control smart devices can be extended far beyond turning on and off lights. Additionally, the functions of the Arduino Nanos can be performed by smartphones, which already have IMUs, Wi-Fi, and BLE radios, and similar online learning code can be directly incorporated into smart home services like Home Assistant. More broadly, people will not need to waste time manually controlling home appliances like lights except to initially teach the artificial intelligence and in the case that its predictive actions are wrong.

## Challenges

<!--
What are the challenges and risks?
-->

Implementing online supervised learning to correlate room-level position with BLE devices and their signal strengths is the most daunting challenge. Realizing communication between the Arduino Nanos and Home Assistant may also be a difficult endeavor. An additional risk is that there might not be enough BLE devices around the house to infer position accurately.

## Requirements for Success

<!--
What skills and resources are necessary to perform the project?
-->

This project requires familiarity with both Arduino and Python. Experience with Linux on Raspberry Pi will be useful for setting up Home Assistant. Additionally, knowledge of online supervised learning will be necessary.

With regard to hardware, this project requires a Raspberry Pi 4, an Arduino Nano RP2040 Connect, and an Arduino Nano 33 BLE Sense, along with a portable charger. Smart light bulbs or outlets compatible with Home Assistant are also needed, along with a Wi-Fi network. Finally, several relatively stationary BLE devices are necessary for accurate position inference.

## Metrics of Success

<!--
What are metrics by which you would check for success?
-->

This project will be considered successful if at least 80% of room predictions are accurate after ten hours of real-life operation. Note that the user is presumed to be the only person interacting with the smart lights. Note also that only two rooms are tested due to both the limited number of available rooms for testing and budget constraints for purchasing smart light bulbs or outlets, although the project will likely produce the proper results with a larger number of rooms given sufficient BLE devices distributed among them.

Several intermediate tests will also be performed to verify proper operation of the different components. First, proper communication between the Arduino Nano and Home Assistant will be verified. Second, I will test the Arduino's ability to distinguish between the living room and the bedroom based on proximity to recognized BLE devices after scanning ten times in each room. Next, the supervised learning will be tested using logged BLE device scans. Finally, the overall system will be tested empirically, spending ten minutes in each room and seeing if the lights switch as desired.

# 2. Related Work

Many previous papers have discussed BLE localization in the context of indoor position tracking, suggesting that BLE may provide benefits over other technologies, such as infrared and ultrasonic positioning. However, all previous approaches utilize BLE beacons, which are explicitly designed for more accurate localization, and require an initial offline mapping phase.

Z. Jianyong et al. discusses filtering based on trilateral relations to resolve position [[5](#5)]. A. I. Kyritsis et al. use just one BLE beacon per room in conjunction with advanced threshold classification [[6](#6)]. S. Memon et al. perform positioning using many BLE tags and detecting the nearest one [[7](#7)]. Y. Pu et al. use a fingerprinting algorithm with kNN machine learning to perform localization [[8](#8)].

While these approaches have can very accurate results, within a square meter, this level of accuracy requires quite a few BLE beacons to be placed in each room. I argue that room-level localization is sufficient for light automation, and I show that existing household BLE devices are enough to reach that level of accuracy. This project's online mapping initialization, using which lights are on to infer user position, is also much more convenient and intuitive than the offline mapping sequences discussed.

# 3. Technical Approach

This project incorporates several technical components. First, an Arduino Nano 33 BLE Sense runs a script to scan nearby BLE devices and record their MAC addresses and signal strengths. Second, an Arduino Nano RP2040 Connect uses a wired UART connection to request these BLE scans regularly every thirty seconds, as well as when significant motion is detected. It then uses a Wi-Fi connection to publish the scan results to an MQTT topic. Third, a Raspberry Pi 4 runs Home Assistant, which is set up to communicate with three smart outlets with lights plugged into them. These lights are divided between two rooms, a bedroom and a living room. The Raspberry Pi also hosts an MQTT broker to receive messages published by the Nano RP2040. Finally, a Python script running on the Raspberry Pi reads these MQTT messages and uses them to train a neural network to associate BLE signal strengths with the room which currently has lights on. When significant motion is detected, the message is instead used to query neural network for the room that the user is in and control lights accordingly.

## Arduino Operation and BLE Scanning

While there were initially plans to only use one Arduino, no Arduino supported using BLE and Wi-Fi simultaneously, and so the roles were divided between an Arduino Nano RP2040 Connect and an Arduino Nano 33 BLE Sense. The Nano RP2040 acts as a master, communicating with the Raspberry Pi over Wi-Fi and initiating BLE scans, while the Nano 33 BLE Sense acts as a slave, performing the requested scans.

The Nano RP2040 first connects to the network using the Arduino WiFiNINA library [[9](#9)]. The PubSubClient library [[10](#10)] is then used to establish a connection to the MQTT broker running on the Raspberry Pi. MQTT is a network protocol which allows clients to publish messages to a topic, as well as to subscribe to a topic to receive messages. The MQTT broker orchestrates the delivery of messages, receiving them from publishing clients and passing them on to subscribed clients. This broker allows clients to communicate without establishing a direct connection, enabling communication between devices which may not even be on the network at the same time [[11](#11)].

The Nano RP2040 then uses the LSM6DSOX library [[12](#12)] to enable the built-in IMU's accelerometer. An interrupt handler is set up for when significant motion is detected by the IMU. While significant motion detection is a feature of the IMU itself, it was not implemented in the LSM6DOSX library. To rectify this, I modified the library to include additional functions for setting up detection and listening for significant motion [[13](#13)]. All communication between the Nano RP2040 and its IMU conforms to the I2C protocol.

A UART serial connection is established between the Nano RP2040 and the Nano 33 BLE Sense. The Nano RP2040 uses this connection to request a BLE scan every thirty seconds or whenever significant motion is detected. The Nano 33 BLE Sense uses the ArduinoBLE library [[14](#14)] to perform a five-second scan in response, sending the MAC addresses and associated signal strengths of all BLE devices seen back over the same serial connection. The Nano RP2040 then publishes these results and whether or not they were triggered by significant motion to an MQTT topic.

## Python Script

A Python script running on the Raspberry Pi serves to automate smart lights based on the results of the Arduino BLE scans.

First, some constants are defined providing the names of the Home Assistant light entities and the rooms they are in. Get requests are then sent frequently to the Home Assistant API [[15](#15)] using the requests library [[16](#16)] in order to determine which room's lights are on at any given time. The room whose lights are on is used as the correct class for neural network training. Since only one user is assumed for purposes of cost and simplicity, training is paused whenever lights are on in multiple rooms.

Next, the paho-mqtt library [[17](#17)]. is used to subscribe to the MQTT topic where Arduino BLE scan results are posted. These results are used as training data for the neural network and as queries when significant motion is detected.

Finally, the output of a neural network query is used to identify lights that should be turned on and turned off. Post requests are sent to the Home Assistant API to make these changes.

This Python script was initially going to be written as a Home Assistant integration for ease of use with Home Assistant, but the integration development process was not very well documented and I was not able to develop a Home Assistant integration in the time I had. However, I believe the Python script can be developed into an integration in a relatively straightforward manner, since Home Assistant supports MQTT for data transfer. All API calls would then be replaced with function calls provided for integrations.

## Neural Network

A dynamic online one-layer neural network was developed for this project using the scikit-learn Python library [[18](#18)]. The stochastic gradient descent classifier was chosen from their library as a flexible linear classifier which allowed me to perform a partial fit on the data available, enabling online learning. It also allowed me to choose the loss function and set a learning rate. The "log" loss was chosen due to its support for providing probability values for each class. A constant learning rate of 0.5 was chosen after some experimental success with that value.

Training is performed by feeding the neural network BLE signal strength values along with the corresponding class name. The signal strength features of the training sample are transformed according to the equation for approximating distance based on signal strength (RSS): <img src="media/eq1.png" alt="d=d_0 \cdot 10^{\frac{RSS(d_0)-RSS(d)}{10n}}" title="RSSI distance equation" height="20"/> [[8](#8)]. The feature values for all unseen BLE devices is then set to zero.

In order to successfully implement online learning, samples are needed from both rooms during training, even though the user is only in one room at a time. To solve this, BLE scan results are obtained every thirty seconds. Half of these samples are used for immediate training, while the other half are stored for the next time the user is in the other room. Stored samples from the room the user is not in are thus fed to the neural network in between immediate samples from the current room. This helps to prevent overfitting when the user remains in the same room for a significant time.

One major difficulty that I dealt with in designing the neural network was incorporating previously unseen BLE devices as features dynamically. First, in order to deal with the memory implications of supporting a potentially infinite number of BLE devices, I limited the number of remembered devices to 250. Thus, the MAC addresses of the 250 most recently seen devices are stored and used to format both the features of a training sample and the learned weights of the classifier. Because the weights have to be dynamically reformatted, I was limited to a one-layer neural network.

The other significant challenge that I encountered was that the scanned BLE MAC addresses changed significantly over time for the same room. I believe this to be the result of Apple implementing dynamic Bluetooth MAC addresses for their products in order to ensure privacy and prevent the possibility of tracking. This severely limits the ability of the classifier to associate MAC addresses and signal strengths with rooms over periods of time longer than thirty minutes. 

# 4. Evaluation and Results

Neural network evaluation was performed using the results of 360 BLE scans taken minutely over a five-hour period in each of the two rooms. This data was taken in ten-minute intervals, with the "current" room (the room the user is in) switching at the end of each interval. For each interval, the ten samples from the current room were fed to the neural network, interspersed with the previous ten minutes' samples from the other room, mimicking the way immediate samples are alternated with stored samples during normal operation. At the end of each interval, the following sample from each room was classified. With ten-minute intervals, an 87% accuracy was found. Since this is above 80% for a ten-hour period, it is determined to be a success.

Performing the same evaluation with thirty-minute time intervals, the accuracy dropped dramatically to 64%. This is most likely due to the issue of dynamic MAC addresses discussed in the previous section. The MAC addresses in the room that is not the current room change so much over the thirty-minute interval that the other room is difficult to classify. Accordingly, an alternative classification method was developed and used in the final implementation. This method uses the likelihoods of each classification output by the neural network. When the likelihood of a sample being from the current room falls below 0.95, the user is assumed to have switched rooms. Since the current room's samples are the most up-to-date, it is presumed (and observed) that the current room's likelihood will be above 0.95 if the user stayed in the same room. Using this method, 100% accuracy was obtained for thirty-minute intervals. For sixty-minute intervals, an accuracy of 60% was obtained using the classifier directly, while the new classification method was again found to have an accuracy of 100%.

The overall system was also tested empirically. The Arduino Nanos were first placed in the living room for approximately ten minutes and then taken to the bedroom for another ten minutes, allowing initial training to be performed. The Nanos were then carried back into the living room. Significant motion was detected, and the lights were turned on in the living room and off in the bedroom as desired. Ten minutes later, the Nanos were returned to the bedroom, and the lights again responded appropriately.

The delay from initial detection of significant motion to light switching was found to be approximately eight seconds on average, five of which are the BLE scanning period. This may be interpreted by the user as the system being unresponsive, so perhaps a shorter BLE scanning period could be used when significant motion is detected.

# 5. Discussion and Conclusions

This project turned out to be very successful, needing only a small amount of configuration to effectively automate smart lights in the home. However, there are some limitations that can be addressed in the future.

First, the system is designed for only one user, requiring only one room's lights to be on for the system to function. The system could be extended to support multiple users, but it would require knowing which user turned on which lights. One way to accomplish this might be to use the voice recognition built into smart speakers to identify the user who requested that a light be turned on. Additionally, a check of all users' positions would be necessary before turning off lights.

Another limitation to address in the future is that the current implementation only supports two rooms. Because the neural network was less successful at classifying a room seen too long ago, a decrease in confidence in the old room was used as an indicator that the user switched rooms as described in the previous section. However, this does not generalize well to more than two rooms. This problem might be solved by exploring other neural network architectures, loss functions, or training strategies. Using a scheduled learning rate which decreases over time while the user remains in the same room may also improve the classifier's performance, particularly when the user remains in the same room for multiple hours.

Despite its limitations, this project successfully demonstrates a path into the future for smart homes, using online supervised learning to automate routine tasks. With the functionality of the Arduinos incorporated into a smartphone app and the Python script developed into a Home Assistant integration, the results of this project could be widely utilized with little to no cost to the user. Despite the challenges I faced, this project's novel use of existing household BLE devices for room-level position inference demonstrates the great potential of the BLE devices that slowly fill our homes.

# 6. References

<!--
List references corresponding to citations in your text above. For papers please include full citation and URL. For datasets and software include name and URL.
-->

[<a name="1">1</a>] Home Assistant. Available: <https://www.home-assistant.io>  
[<a name="2">2</a>] Room Assistant. Available: <https://room-assistant.io>  
[<a name="3">3</a>] V. H. Bhide and S. Wagh, "i-learning IoT: An intelligent self learning system for home automation using IoT," 2015 International Conference on Communications and Signal Processing (ICCSP), 2015, pp. 1763-1767, doi: 10.1109/ICCSP.2015.7322825.  
[<a name="4">4</a>] S. Shead, "Amazon wants us to stop talking to Alexa so much," CNBC. Available: <https://www.cnbc.com/2021/11/02/amazon-wants-us-to-stop-talking-to-alexa-so-much.html>  
[<a name="5">5</a>] Z. Jianyong, L. Haiyong, C. Zili and L. Zhaohui, "RSSI based Bluetooth low energy indoor positioning," 2014 International Conference on Indoor Positioning and Indoor Navigation (IPIN), 2014, pp. 526-533, doi: 10.1109/IPIN.2014.7275525.  
[<a name="6">6</a>] A. I. Kyritsis, P. Kostopoulos, M. Deriaz and D. Konstantas, "A BLE-based probabilistic room-level localization method," 2016 International Conference on Localization and GNSS (ICL-GNSS), 2016, pp. 1-6, doi: 10.1109/ICL-GNSS.2016.7533848.  
[<a name="7">7</a>] S. Memon, M. M. Memon, F. K. Shaikh and S. Laghari, "Smart indoor positioning using BLE technology," IEEE International Conference on Engineering Technologies and Applied Sciences, 2017, pp. 1-5, doi: 10.1109/ICETAS.2017.8277872.  
[<a name="8">8</a>] Y. Pu, P. You, "Indoor positioning system based on BLE location fingerprinting with classification approach," Applied Mathematical Modelling, vol. 62, pp. 654-663, 2018, doi: 10.1016/j.apm.2018.06.031.  
[<a name="9">9</a>] WiFiNINA. Available: <https://www.arduino.cc/en/Reference/WiFiNINA>  
[<a name="10">10</a>] PubSubClient. Available: <https://www.arduino.cc/reference/en/libraries/pubsubclient/>  
[<a name="11">11</a>] "MQTT Essentials," HiveMQ. Available: <https://www.hivemq.com/mqtt-essentials>  
[<a name="12">12</a>] LSM6DSOX. Available: <https://github.com/stm32duino/LSM6DSOX>  
[<a name="13">13</a>] LSM6DSOX. Available: <https://github.com/evanmmitchell/LSM6DSOX>  
[<a name="14">14</a>] ArduinoBLE. Available: <https://www.arduino.cc/en/Reference/ArduinoBLE>  
[<a name="15">15</a>] Home Assistant REST API. Available: <https://developers.home-assistant.io/docs/api/rest/>  
[<a name="16">16</a>] requests. Available: <https://docs.python-requests.org/en/latest/>  
[<a name="17">17</a>] paho-mqtt. Available: <https://www.eclipse.org/paho/index.php?page=clients/python/index.php>  
[<a name="18">18</a>] scikit-learn. Available: <https://scikit-learn.org>  
