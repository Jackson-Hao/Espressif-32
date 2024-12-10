# Espressif Development Board Connecting to Huawei Cloud IoT Platform Framework

---

## Table of Contents
- [Espressif Development Board Connecting to Huawei Cloud IoT Platform Framework](#espressif-development-board-connecting-to-huawei-cloud-iot-platform-framework)
  - [Table of Contents](#table-of-contents)
  - [Copyright](#copyright)
  - [Introduction](#introduction)
  - [Development Environment](#development-environment)
  - [Usage Instructions](#usage-instructions)

## Copyright
Copyright (c) 2024, Jackson Hao.<br>
la_jackson_hao@petalmail.com<br>
Zhengzhou University of Light Industry, PRC.<br>
This repository follows the GPL-3.0 open-source license.

## Introduction
This repository contains the framework code for connecting the ESP32S3N16R8 development board to the Huawei Cloud IoT platform using the MQTT protocol. It is developed based on the Hisilicon Hi3861 platform and uses the MQTT protocol interface provided by the Huawei Cloud IoT platform to achieve data communication between the device and the cloud. This repository provides the ported code for the Huawei Cloud IoT platform MQTT protocol interface, as well as a simple example code. Developers can perform secondary development according to their own needs.

## Development Environment

- Operating System: Windows 10 / Ubuntu 24.10
- Development Board: ESP32S3N16R8
- Development Tools: VSCode / PlatformIO
- Programming Language: C
- Framework: ESP-IDF v5.3.1
- Cloud Platform: Huawei Cloud IoT Platform

## Usage Instructions
Before use, please configure the device information of the Huawei Cloud IoT platform, including device ID, device password, product ID, etc. After configuration, fill in the relevant information in the corresponding positions of the mqtt configuration file, and configure the WLAN information. Then you can compile and download it to the ESP32S3N16R8 development board for operation.
When configuring WLAN, you must choose the 2.4G band, otherwise it will not be able to connect to the Wi-Fi network.