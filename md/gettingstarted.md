# 快速入门

## 项目简介

### ESP-Intelligent-Vehicle：ESP32 WIFI 智能小车

ESP-Intelligent-Vehicle 是乐鑫推出的基于 ESP32 开发的wifi智能小车示例项目，内含产品开发的完整步骤、最佳做法，可实现摄像头扫描二维码配网、扫描二维码信息解析并上报、通过遥控器sbus协议或局域网内TCP通信对小车的移动控制、局域网内通过TCP通信发送命令控制小车进行物料搬运、小车定点寻路、视频同步、OTA等功能。本项目功能扩展方便，可用于教育领域。



本项目中的智能小车有以下功能：

- 板载自动下载电路
- 支持解析二维码信息
- 支持扫描二维码配网
- 支持定点寻路
- 支持遥控器（sbus协议）控制
- 支持PC远程控制
- 支持视频同步
- 支持 OTA 升级



## 硬件介绍

**ESP-Intelligent-Vehicle 使用了ESP-WROVER-KIT-V3开发板，搭载OV2640 Camera模块**

**小车驱动部分由CAN通信进行控制的电机连接全向轮组成**

**物料搬运部分由舵机连接组成**

|序号| 模块名 | 主要元器件 | 功能 |
|:-:|:-:|:-:|:-:|
|1| 主控制板 - **ESP32** | ESP32-WROVER-B |提供 I2C SPI GPIO 扩展接口  |
|2|反相器模块 | 74HC04D | 反相接收机信号 |
|3| CAN收发器模块 | TJA1050 | Tx、Rx电平转为CAN协议的差分信号 |

## ESP-IDF 简介

ESP-IDF 是乐鑫为 ESP32/ESP32-S2 提供的物联网开发框架。

* ESP-IDF 包含一系列库及头文件，提供了基于 ESP32/ESP32-S2 构建软件项目所需的核心组件。
* ESP-IDF 还提供了开发和量产过程中最常用的工具及功能，例如：构建、烧录、调试和测量等。

详情可查阅：[ESP-IDF 编程指南](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32s2/get-started/index.html)。


