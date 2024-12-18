# API说明

## 版本
v1.0.0

## 简介
本文档主要介绍了操作系统线程模块的接口说明，封装了系统原有的线程接口，提供了更加简单易用的表达形式。

## 接口
|接口|功能|参数|返回值|
|---|---|---|---|
|`osThreadCreate`|创建线程|`osThreadFunc_t func`：线程函数<br>`void *arg`：线程函数参数<br>`const osThreadAttr_t *attr`：线程属性|`status`：线程创建状态|
|`osThreadTerminate`|终止线程|NULL|NULL|