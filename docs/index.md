# Abstract
<!--
Provide a brief overview of the project objectives, approach, and results.
-->

Modern smart home devices are typically interacted with via dedicated smartphone apps or by speaking to a virtual assistant. However, for turning on and off smart lights, these methods of interaction are often more of a hassle than simply flipping a traditional light switch. Additionally, while many smart home services provide for manually-configured automation, it is often difficult to predict precisely when a particular light will need to be turned on and off. This project aims to both eliminate the need to manually configure smart light automations and minimize time spent directly interacting with smart lights. These goals are achieved by using unsupervised learning to observe and replicate patterns in the manual switching of smart lights based on time, user motion, and inferred user position. General magnitude and duration of user motion is detected by an inertial measurement unit (IMU). User position is inferred based on the signal strength of various household Bluetooth Low Energy (BLE) devices nearby the user, made possible by the increased presence of such devices in the typical home. The smart home setup is operated by Home Assistant running on a Raspberry Pi 4. Motion detection and BLE scanning are performed by an Arduino Nano 33 BLE Sense, which incorporates both an IMU and a BLE radio.

# Team

* Evan Mitchell

# Required Submissions

* [Proposal](proposal)
* [Midterm Checkpoint Presentation Slides](http://)
* [Final Presentation Slides](http://)
* [Final Report](report)
