# Abstract
<!--
Provide a brief overview of the project objectives, approach, and results.
-->

Modern smart home devices are typically interacted with via smartphone apps or by speaking to a virtual assistant. However, for turning on and off smart lights, these methods of interaction are often more of a hassle than simply flipping a traditional light switch. Additionally, while many smart home services provide for manually configured automation, it is often difficult to predict precisely when a particular light will need to be turned on and off, leading to wasted energy. This project aims to both eliminate the need to manually configure smart light automations and minimize the time spent directly interacting with smart lights. These goals are achieved by using online supervised learning to observe the manual switching of smart lights and correlate the room-level position of a user with the signal strengths of nearby household Bluetooth Low Energy (BLE) devices. When an inertial measurement unit (IMU) carried by the user detects significant motion, the neural network is queried with nearby BLE signal strengths to determine whether the user has moved to a different room. If so, the appropriate lights are turned on, and lights in the previous room are turned off. In testing, the system correctly recognized every time the user switched rooms, with rooms being alternated every ten, thirty, and sixty minutes.

# Team

* Evan Mitchell

# Required Submissions

* [Proposal](proposal)
* [Midterm Checkpoint Presentation Slides](checkpoint.pdf)
* [Midterm Checkpoint Presentation Recording](https://youtu.be/0vCRBBYPsWg)
* [Final Presentation Slides](presentation.pdf)
* [Final Presentation Recording](https://youtu.be/WkyJOP_pBwI)
* [Final Report](report)
