# Espressif芯片快速管理辅助工具

---

## 目录
- [Espressif芯片快速管理辅助工具](#espressif芯片快速管理辅助工具)
  - [目录](#目录)
  - [版权](#版权)
  - [前言](#前言)
  - [开发环境](#开发环境)
  - [初次使用](#初次使用)
  - [命令列表](#命令列表)

## 版权
Copyright (c) 2024, Jackson Hao.<br>
la_jackson_hao@petalmail.com<br>
Zhengzhou University of Light Industry, PRC.<br>
本仓库遵循GPL-3.0开源协议。

## 前言


## 开发环境

- 操作系统：Windows 10 Pro / Ubuntu 24.10
- 开发板：ESP32S3N16R8
- 开发工具：VSCode / PlatformIO / PyCharm
- 开发语言：C / Python
- 框架：ESP-IDF v5.3.1

## 初次使用
> 在使用本仓库的工具之前，请确保你已经安装了Python3.8以上的版本，并且已经安装和配置好了PlatformIO及其CLI工具。
本工具使用方法如下：
- 1. 下载本仓库到本地
- 2. 进入到本仓库的根目录
- 3. 打开终端，输入`python manage.py`，即可运行本工具
- 4. 根据提示，输入相应的命令即可

## 命令列表
- `python manage.py -h` `python manage.py --help`：查看帮助信息
- `python manage.py --version`：查看版本信息
- `python manage.py --list`：列出所有的业务工程
- `python manage.py --select [Project name]`：选择一个业务工程
- `python manage.py --new [Project name]`：创建一个新的业务工程
- `python manage.py --delete [Project name]`：删除一个业务工程
- `python manage.py --clean`：清理所有的编译文件
- `python manage.py --build`：编译选中的业务工程
- `python manage.py --upload`：上传固件到开发板