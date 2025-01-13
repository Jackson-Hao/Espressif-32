# Espressif IoT Rapid Development Framework (Development Version)

---

## Table of Contents
- [Espressif IoT Rapid Development Framework (Development Version)](#espressif-iot-rapid-development-framework-development-version)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Development Environment](#development-environment)
  - [Version Information](#version-information)
  - [Usage](#usage)
    - [First Time Use](#first-time-use)
    - [Creating a New Project](#creating-a-new-project)
    - [Deleting a Project](#deleting-a-project)
    - [Advanced Operations](#advanced-operations)
      - [Configuring Dependencies](#configuring-dependencies)
    - [Viewing Example Documentation](#viewing-example-documentation)
    - [Viewing Help Information](#viewing-help-information)
  - [Appendix](#appendix)
    - [Example Project List](#example-project-list)
    - [Available Dependency Libraries](#available-dependency-libraries)
    - [Command List](#command-list)
  - [Copyright Statement](#copyright-statement)
  - [Open Source License](#open-source-license)

---

## Introduction

This framework is an Espressif IoT rapid development framework based on Python3, mainly used for quickly creating, deleting, compiling, and uploading firmware. The framework is developed based on PlatformIO's CLI, allowing for quick firmware compilation and upload. The main purpose of this framework is to improve development efficiency, reduce repetitive work, and enhance developers' productivity.
Additionally, this repository contains some commonly used IoT business projects that can be used directly or modified according to your needs. Please refer to the instructions below for usage.

## Development Environment

- Operating System: Windows 10 Pro / Ubuntu 24.10
- Development Board: ESP32S3N16R8
- Development Tools: VSCode / PlatformIO / PyCharm
- Development Languages: C / Python / Cmake
- Framework: ESP-IDF v5.3.1

## Version Information

Framework Version: 19.01-Dev (HWE2019.1.1.110)
Example Version: [Example Version Records](docs/Examples.md)
Support Period: 2025-01-13 ~ 2025-01-31

---

## Usage

### First Time Use
> Before using this framework, please ensure that you have installed Python 3.8 or above, and have installed and configured PlatformIO and its CLI.

The usage of this framework is as follows:
- 1. Download this repository to your local machine
- 2. Navigate to the root directory of this repository
- 3. Open a terminal and enter `python manage.py` to run the framework
- 4. Follow the prompts and enter the corresponding commands

### Creating a New Project
> Again, before using this framework, please ensure that you have installed Python 3.8 or above, and have installed and configured PlatformIO and its CLI.

- 1. Navigate to the root directory of this repository
- 2. Open a terminal and enter `python manage.py --new [Project name]` to create a project named `[Project name]`
- 3. Enter `python manage.py --select [Project name]` to select the newly created project and wait for PlatformIO initialization
- 4. Enter `python manage.py --build` or use the PlatformIO plugin in Visual Studio Code to compile
- 5. Enter `python manage.py --upload` or use the PlatformIO plugin in Visual Studio Code to upload the firmware

### Deleting a Project
- 1. Navigate to the root directory of this repository
- 2. Open a terminal and enter `python manage.py --delete [Project name]` to delete the project named `[Project name]`
> Warning: Deleting a project will delete all source files, please operate with caution
> Warning: Do not directly delete the project folder, otherwise it may cause the framework to malfunction

### Advanced Operations
#### Configuring Dependencies
If you need to configure library dependencies for a project, go to the following file to configure:
- `src/application/wifi_iot/app/{Service name}/CMakeLists.txt`: Configure the dependencies of the project
- Add the required dependency library `include` paths in `INCLUDE_DIRS`
  - For example: `${CMAKE_SOURCE_DIR}/src/application/wifi_iot/app/startup/include`
- Open the `platformio.ini` file in the root directory, refresh the current file, and wait for PlatformIO to automatically configure to make the dependencies effective

### Viewing Example Documentation
- This repository contains some commonly used IoT business projects that can be used directly or modified according to your needs
- Please check the `Example Project List` and select an example project you need to use
- The example projects in this repository have detailed documentation, which can be viewed in the `src/application/wifi_iot/app/{Service name}/docs/README.md` file

### Viewing Help Information
- `python manage.py -h` `python manage.py --help`: View help information

---

## Appendix

### Example Project List
- `blink`: A simple LED blinking project
- `mqtt_example`: An MQTT protocol project
- `multi_task`: A multi-task project
- `startup`: A Hello World project
- `uart_example`: A serial loopback project
- `wifi_ap_configure`: A WiFi AP configuration project
- `wifi_ap_configure_with_ota`: A WiFi AP configuration project with OTA support

### Available Dependency Libraries

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
- `version`

### Command List
- `python manage.py -h` `python manage.py --help`: View help information
- `python manage.py --version`: View version information
- `python manage.py --list`: List all projects
- `python manage.py --select [Project name]`: Select a project
- `python manage.py --new [Project name]`: Create a new project
- `python manage.py --delete [Project name]`: Delete a project
- `python manage.py --clean`: Clean all compiled files
- `python manage.py --build`: Compile the selected project
- `python manage.py --upload`: Upload firmware to the development board

---

## Copyright Statement
**© School of Electrical and Information Engineering Jackson Hao<br>**
**© School of Software Artificial Intelligence Innovation Lab<br>**
![alt text](picture/LAB.png)

If there is any infringement, please contact:
Email: la_jackson_hao@petalmail.com<br>

Zhengzhou University of Light Industry, People's Republic of China<br>

## Open Source License

Licensed under GPL-3.0 (hereinafter referred to as "this license"). Unless in compliance with this license and the following terms, you may not use this example set:<br>

**Any unauthorized individual or organization is prohibited from using this example set and its affiliated materials, innovations, creative designs, algorithms, derivative works, etc., for any commercial purposes, profit activities, various competitions, live teaching, recorded teaching, offline courses, book writing, textbook writing, conferences, training, public welfare activities, project topics, graduation projects, graduation theses, academic papers, etc.**

**This program is provided by the copyright holders and contributors "as is" without any express or implied warranties, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event shall the copyright holders or contributors be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including but not limited to procurement of substitute goods or services; loss of use, data, or profits; business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of the use of this program, even if advised of the possibility of such damage.**
<center>All rights reserved</center>