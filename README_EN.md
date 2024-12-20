<font face="Times New Roman">

# Espressif Chip Quick Management Tool

---

## Table of Contents
- [Espressif Chip Quick Management Tool](#espressif-chip-quick-management-tool)
  - [Table of Contents](#table-of-contents)
  - [Copyright](#copyright)
  - [Introduction](#introduction)
  - [Development Environment](#development-environment)
  - [First Use](#first-use)
  - [Command List](#command-list)
  - [Example Project List](#example-project-list)
  - [Available Dependency Libraries](#available-dependency-libraries)

## Copyright
**Copyright (c) 2024, College of Electrical and Information Engineering, Jackson Hao<br>**
**Copyright (c) 2024, School of Software, Artificial Intelligence Lab<br>**
![alt text](picture/LAB.png)

Zhengzhou University of Light Industry, PRC<br>

la_jackson_hao@petalmail.com<br>

All rights reserved. Licensed under GPL-3.0 (hereinafter referred to as "this license"). Unless in compliance with this license and the following terms, you may not use this case set:<br>

**Any unauthorized individual or organization is prohibited from using this case set and its affiliated materials, innovations, creative designs, architecture designs, algorithms, derivative works, etc., for any commercial purposes, profit activities, various competitions, live teaching, recorded teaching, offline courses, book writing, textbook writing, conferences, training, public welfare activities, project topics, graduation projects, graduation theses, academic papers, etc.**

**This program is provided by the copyright holder and contributors "as is", without any express or implied warranties, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event shall the copyright holder or contributors be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including but not limited to procurement of substitute goods or services; loss of use, data, or profits; or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of the use of this software, even if advised of the possibility of such damage.**

## Introduction

This tool is a quick management tool for Espressif chips developed based on Python3, mainly used for quick creation, deletion, compilation, and firmware upload operations. This tool is developed based on PlatformIO's CLI tool, which can quickly compile and upload firmware. The main purpose of this tool is to improve development efficiency, reduce repetitive work, and enhance the work efficiency of developers.

At the same time, this repository contains some commonly used business projects that can be used directly or modified according to your needs. Please refer to the instructions below for the usage of this tool.

## Development Environment

- Operating System: Windows 10 Pro / Ubuntu 24.10
- Development Board: ESP32S3N16R8
- Development Tools: VSCode / PlatformIO / PyCharm
- Development Languages: C / Python
- Framework: ESP-IDF v5.3.1

## First Use
> Before using the tools in this repository, please ensure that you have installed Python 3.8 or above, and have installed and configured PlatformIO and its CLI tools.
The usage of this tool is as follows:
- 1. Download this repository to your local machine
- 2. Navigate to the root directory of this repository
- 3. Open the terminal and enter `python manage.py` to run this tool
- 4. Follow the prompts and enter the corresponding commands

## Command List
- `python manage.py -h` `python manage.py --help`: View help information
- `python manage.py --version`: View version information
- `python manage.py --list`: List all business projects
- `python manage.py --select [Project name]`: Select a business project
- `python manage.py --new [Project name]`: Create a new business project
- `python manage.py --delete [Project name]`: Delete a business project
- `python manage.py --clean`: Clean all compiled files
- `python manage.py --build`: Compile the selected business project
- `python manage.py --upload`: Upload firmware to the development board

## Example Project List
- `blink`: A simple LED blinking business project
- `mqtt_example`: A business project using the MQTT protocol
- `multi_task`: A multi-tasking business project
- `startup`: A Hello World business project
- `uart_example`: A serial loopback business project
- `wifi_ap_configure`: A WiFi AP configuration business project

## Available Dependency Libraries

- `ap_wifi_config`
- `http_server_lite`
- `iot_gpio`
- `iot_mqtt_oc`
- `iot_wifi_connect`
- `json_lite`
- `NVS`
- `osThread`
- `ping_lite` (untested)
- `sc_wifi_config`

</font>