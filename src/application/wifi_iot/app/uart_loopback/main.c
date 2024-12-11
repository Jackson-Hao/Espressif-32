#include <stdio.h>
#include "main.h"
#include "iot_uart_config.h"
#include "osThread.h"

static uint8_t s_led_state = 0;

static char msg_test[] = "Hello, this is a test message from uart1.";
static char buffer[256];

static void IoTUartLoopback(void* arg) {
    IoTUartConfig();

    while (1) {
        uart_write_bytes(UART_NUM_1,msg_test,strlen(msg_test));

        s_led_state = !s_led_state;

        vTaskDelay(500 / portTICK_PERIOD_MS);
        gpio_set_level(26,s_led_state);
        int len = uart_read_bytes(UART_NUM_1,buffer,(UART1_RX_BUF_SIZE - 1), 20 / portTICK_PERIOD_MS);

        if(len) {
            buffer[len] = '\0';
            ESP_LOGI(TAG,"Recv str -> \"%s\" , and the length is:%d \n",buffer,strlen(buffer)); 
            ESP_LOGI(TAG,"The size of buffer is %d, and ready to clear this buffer.\n",sizeof(buffer));
            memset(buffer,0,sizeof(buffer));
        }
    }
}

static void IoTMainTaskEntry(void) {
    int ret = 0;
    osThreadAttr_t attr;

    attr.name = "IoTUartLoopback";
    attr.stack_size = 1024*4;
    attr.priority = 5;
    attr.cpu_id = 1;
    if((ret = osThreadCreate(&attr, IoTUartLoopback, NULL)) != 0) {
        printf("IoTUartLoopback create failed\n");
    }
}

APP_SERVICE_INIT(IoTMainTaskEntry);