#include <stdio.h>
#include "main.h"
#include "driver/i2c.h"
#include "osThread.h"
#include "OLED.h"
#include "unistd.h"

#define I2C_MASTER_SCL_IO 2
#define I2C_MASTER_SDA_IO 3
#define I2C_MASTER_NUM 0
#define I2C_MASTER_FREQ_HZ 1100000 // 1.1MHz
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_TIMEOUT_MS 100

static const char *TAG = "i2c-simple-example";

oled_iic_config_t oled_iic_config = {
    .SDA_IO = I2C_MASTER_SDA_IO,
    .SCL_IO = I2C_MASTER_SCL_IO,
    .MasterNumber = I2C_MASTER_NUM,
    .MasterFreq = I2C_MASTER_FREQ_HZ,
    .MasterTxBufDisable = I2C_MASTER_TX_BUF_DISABLE,
    .MasterRxBufDisable = I2C_MASTER_RX_BUF_DISABLE,
    .MasterTimeOut = I2C_MASTER_TIMEOUT_MS
};

// 实现时钟功能，每秒钟刷新一次，显示时间 HH:MM:SS
static void OLED_Clock(void) {
    int Hour, Min, Sec;
    char str[20];
    while (1) {
        Hour = 0;
        Min = 0;
        Sec = 0;
        while (Hour < 24) {
            while (Min < 60) {
                while (Sec < 60) {
                    sprintf(str, "|   %02d:%02d:%02d   |", Hour, Min, Sec);
                    //OLEDClearLine(3);
                    OLEDShowString(0, 4, str, 16);
                    usleep(1000000);
                    Sec++;
                }
                Min++;
                Sec = 0;
            }
            Hour++;
            Min = 0;
        }
    }

}

void app_main(void) {
    
    if(i2c_master_init(&oled_iic_config) != ESP_OK) {
        ESP_LOGE(TAG, "I2C initialized failed");
        return;
    }
    ESP_LOGI(TAG, "I2C initialized successfully");

    // OLED屏幕初始化
    OLEDInit();

    // LOADING
    OLEDShowString(0, 0, "   Loading...   ", 16);
    OLEDShowString(0, 2, " +------------+ ", 16);
    OLEDShowString(0, 4, " |            | ", 16);
    OLEDShowString(0, 6, " +------------+ ", 16);
    for (int i = 0; i < 12; i++) {
        OLEDShowCustom(16 + i * 8, 4, 0);
        vTaskDelay(150 / portTICK_PERIOD_MS);
    }

    OLEDClear();

    usleep(100); // 等待OLED清屏完成
    for(int i = 0; i < 4; i++) {
        OLEDShowChinese(32+i*16, 0, i);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    OLEDShowString(0, 2, "+--------------+", 16);
    OLEDShowString(0, 4, "|              |", 16);
    OLEDShowString(0, 6, "+--------------+", 16);


    OLED_Clock();

    ESP_LOGI(TAG, "I2C unitialized successfully");
}
