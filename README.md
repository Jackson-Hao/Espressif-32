# Espressif开发板连接华为云物联网平台框架

---

## 目录
- [Espressif开发板连接华为云物联网平台框架](#espressif开发板连接华为云物联网平台框架)
  - [目录](#目录)
  - [版权](#版权)
  - [前言](#前言)
  - [开发环境](#开发环境)
  - [使用说明](#使用说明)

## 版权
Copyright (c) 2024, Jackson Hao.<br>
la_jackson_hao@petalmail.com<br>
Zhengzhou University of Light Industry, PRC.<br>
本仓库遵循GPL-3.0开源协议。

## 前言
本仓库为ESP32S3N16R8开发板使用MQTT协议连接华为云物联网平台的框架代码。基于Hisilicon Hi3861平台移植开发，使用华为云物联网平台提供的MQTT协议接口，实现设备与云端的数据通信。本仓库提供了华为云物联网平台MQTT协议接口的移植代码，以及一个简单的示例代码，开发者可以根据自己的需求进行二次开发。

## 开发环境

- 操作系统：Windows 10 / Ubuntu 24.10
- 开发板：ESP32S3N16R8
- 开发工具：VSCode / PlatformIO
- 开发语言：C
- 框架：ESP-IDF v5.3.1
- 云平台：华为云物联网平台

## 使用说明
使用前请先配置好华为云物联网平台的设备信息，包括设备ID、设备密码、产品ID等信息。配置完成后，将相关信息填写到mqtt配置文件中的相应位置，同时配置好WLAN信息，即可编译下载到ESP32S3N16R8开发板上运行。
WLAN配置时，必须选择2.4G频段，否则无法连接到Wi-Fi网络。