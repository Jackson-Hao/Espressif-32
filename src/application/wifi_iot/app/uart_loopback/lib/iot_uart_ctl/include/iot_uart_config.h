#ifndef __IOT_UART_CONFIG_H__
#define __IOT_UART_CONFIG_H__

#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_err.h"

#define UART1_TX_BUF_SIZE 256
#define UART1_RX_BUF_SIZE 256

static const char *TAG = "UART section0";

void IoTUartConfig(void);

#endif