# Espressif Chip Quick Management Auxiliary Tool

---

## Table of Contents
- [Espressif Chip Quick Management Auxiliary Tool](#espressif-chip-quick-management-auxiliary-tool)
  - [Table of Contents](#table-of-contents)
  - [Copyright](#copyright)
  - [Introduction](#introduction)
  - [Development Environment](#development-environment)
  - [First Use](#first-use)
  - [Command List](#command-list)

## Copyright
Copyright (c) 2024, Jackson Hao.<br>
la_jackson_hao@petalmail.com<br>
Zhengzhou University of Light Industry, PRC.<br>
This repository follows the GPL-3.0 open source license.

## Introduction


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