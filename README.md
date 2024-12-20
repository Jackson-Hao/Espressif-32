<font face="宋体">

# Espressif芯片快速管理辅助框架

---

## 目录
- [Espressif芯片快速管理辅助框架](#espressif芯片快速管理辅助框架)
  - [目录](#目录)
  - [版权声明](#版权声明)
  - [前言](#前言)
  - [开发环境](#开发环境)
  - [初次使用](#初次使用)
  - [命令列表](#命令列表)
  - [案例工程列表](#案例工程列表)
  - [可使用依赖库列表](#可使用依赖库列表)

## 版权声明
**© 电气与信息工程学院 Jackson Hao<br>**
**© 软件学院 人工智能创新实验室<br>**
![alt text](picture/LAB.png)

郑州轻工业大学，中华人民共和国<br>

la_jackson_hao@petalmail.com<br>

依据GPL-3.0（以下简称"本许可"）进行许可。除非符合（遵守）本许可及以下条款，否则您不得使用此案例集：<br>

**禁止任何未经授权的个人或组织以任何形式将本案例集及其附属资料、创新、创意、架构设计、算法、衍生作品等用于任何商业目的、盈利活动、各类竞赛(比赛)、直播教学、录播教学、线下课程、书籍编写、教材编写、会议、培训、公益活动、项目课题、毕业设计、毕业论文、学术论文等。**

**本程序由版权持有人和贡献人“按原样”提供，不提供任何明示或暗示担保，包括但不局限于对适销性和特定用途适合性的暗示担保。在任何情况下，版权持有人或贡献人对因使用本程序而导致的任何直接、间接、附带、特殊、典型或后果性损害（包括但不限于购买替代商品或服务；使用损失、数据丢失或利润损失；业务中断）不承担任何责任，无论其如何产生，也不论其责任理论为何，无论是合同、严格责任还是侵权（包括疏忽或其他），即使已告知此类损害的可能性。**
<center>保留一切权利</center>

## 前言

本框架是一个基于Python3开发的Espressif芯片快速管理辅助框架，主要用于快速创建、删除、编译、上传固件等操作。本框架基于PlatformIO的CLI进行开发，可以快速的进行固件的编译和上传。本框架的主要目的是为了提高开发效率，减少重复性的工作，提高开发人员的工作效率。

同时，本仓库包含了一些常用的业务工程，可以直接使用，也可以根据自己的需求进行修改。本框架的使用方法请参考下文的说明。

## 开发环境

- 操作系统：Windows 10 Pro / Ubuntu 24.10
- 开发板：ESP32S3N16R8
- 开发工具：VSCode / PlatformIO / PyCharm
- 开发语言：C / Python
- 框架：ESP-IDF v5.3.1

## 初次使用
> 在使用本仓库的框架之前，请确保你已经安装了Python3.8以上的版本，并且已经安装和配置好了PlatformIO及其CLI。
本框架使用方法如下：
- 1. 下载本仓库到本地
- 2. 进入到本仓库的根目录
- 3. 打开终端，输入`python manage.py`，即可运行本框架
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

## 案例工程列表
- `blink`：一个简单的LED闪烁业务工程
- `mqtt_example`：一个MQTT协议的业务工程
- `multi_task`：多任务的业务工程
- `startup`：Hello World的业务工程
- `uart_example`：串口回环的业务工程
- `wifi_ap_configure`：WiFi AP配置的业务工程

## 可使用依赖库列表

- `ap_wifi_config`
- `http_server_lite`
- `iot_gpio`
- `iot_mqtt_oc`
- `iot_wifi_connect`
- `json_lite`
- `NVS`
- `osThread`
- `ping_lite` 未经验证
- `sc_wifi_config`

</font>