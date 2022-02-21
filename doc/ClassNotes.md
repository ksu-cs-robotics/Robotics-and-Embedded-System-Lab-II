<p align="center">
  <a href="" rel="noopener">
 <img src="https://raw.githubusercontent.com/ksu-cs-robotics/Software-Development-for-Robotics/master/resources/images/ATR-logo.gif" alt="ATR"></a>
</p>

<h3 align="center">CS 23301/23302: Robotics and Embedded System Lab Class</h3>

<div align="center">

  [![Status](https://img.shields.io/badge/status-active-success.svg)]() 
  [![License](https://img.shields.io/badge/license-MIT-blue.svg)](/LICENSE)

</div>

---

<p align="center"> Class Notes
</p>



## ✍️ Notes <a name = "rosinstall"></a>
#### 2.1 Raspberry PI Setup
Since its not really a good idea to put your laptop/desktop on a *0.2m^3* robot, we are going to use *Raspberry Pi* as our on board PC. 

- Raspberry Pi 3: Please follow [this video]() link to setup your raspberry pi
- Raspberry Pi 4: Since the offical Ubuntu support for raspberry pi 4 havent come out yet. You can install [Raspbian Buster with desktop](https://www.raspberrypi.org/downloads/raspbian/), the setup process would be the similar as above.
- **DVI type monitor Setup:** 
  ```
  sudo nano /boot/config.txt
  ```
  ```
  ### edit these lines ###
  hdmi_force_hotplug=1
  hdmi_drive=1
  hdmi_group=2
  hdmi_mode=82
  ```
