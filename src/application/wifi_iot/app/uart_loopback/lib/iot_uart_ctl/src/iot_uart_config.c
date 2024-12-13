#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_err.h"
#include "iot_uart_config.h"



void IoTUartConfig(void);

const uart_config_t uart1_config = {
    .baud_rate = 115200,                   /* 通讯波特率 */
    .data_bits = UART_DATA_8_BITS,         /* 每一个数据是8位 */
    .parity    = UART_PARITY_DISABLE,      /* 关闭奇偶校验 */
    .stop_bits = UART_STOP_BITS_1,         /* 停止位是1位 */
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, /* 软件控流 */
    .source_clk = UART_SCLK_APB,           /* APB时钟 */
};

void IoTUartConfig(void) {
    ESP_LOGI(TAG, "Example configured to uart communication");
    uart_param_config(UART_NUM_1, &uart1_config);
    uart_set_pin(UART_NUM_1, 17, 18, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_1, UART1_RX_BUF_SIZE * 4, UART1_TX_BUF_SIZE * 4, 0, NULL, 0);
}