- Development Board: ESP32S3N16R8
- Development Tools: VSCode / PlatformIO
- Programming Language: C
- Framework: ESP-IDF v5.3.1
- Cloud Platform: Huawei Cloud IoT Platform

## Repository Structure
- /
  - /.vscode
  - /include
    - Configuration files corresponding to main.c are stored here
  - /lib
    - /iot_gpio_ctl
      - Code related to GPIO control is stored here
    - /iot_mqtt_oc
      - Business code related to the MQTT protocol is stored here, including MQTT protocol initialization, connection, subscription, publishing, etc.
    - /iot_wifi_connect
      - Dependency code related to Wi-Fi connection is stored here
  - /src
    - CMakelists.txt
    - main.c
      - The main function code is stored here, starting Wi-Fi connection, MQTT business, GPIO control, etc.
  - platformio.ini
  - partition.csv
    - ESP32 partition table configuration file is stored here
  - CMakeLists.txt
  - README.md

## Usage Instructions
Before use, please configure the device information of the Huawei Cloud IoT Platform, including device ID, device password, product ID, etc. After configuration, fill in the relevant information in the corresponding positions of the mqtt configuration file, and configure the WLAN information. Then you can compile and download it to the ESP32S3N16R8 development board for operation.
When configuring WLAN, you must choose the 2.4G band, otherwise it will not be able to connect to the Wi-Fi network.