#ifndef __OLED_H__
#define __OLED_H__

#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "oled_font.h"




#define OLED_ADDR 0x3c
#define OLED_CMD 0
#define OLED_DATA 1

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_PAGES (OLED_HEIGHT / 8)

extern uint8_t oled_buffer[OLED_PAGES][OLED_WIDTH];

typedef struct {
    int SDA_IO;
    int SCL_IO;
    int MasterNumber;
    int MasterFreq;
    int MasterTxBufDisable;
    int MasterRxBufDisable;
    int MasterTimeOut;
} oled_iic_config_t;

esp_err_t i2c_master_init(oled_iic_config_t *oled_iic_config);
esp_err_t OLEDWriteByte(uint8_t data, uint8_t cmd_);
void OLEDInit(void);
void OLEDSetPosition(uint8_t x, uint8_t y);
void OLEDShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);
void OLEDClear(void);
void OLEDClearLine(uint8_t i);
void OLEDShowNumber(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size2);
uint32_t oled_pow(uint8_t m, uint8_t n);
void OLEDShowString(uint8_t x, uint8_t y, char *chr, uint8_t Char_Size);
void OLEDShowChinese(uint8_t x, uint8_t y, uint8_t no);
void OLEDShowCustom(uint8_t x, uint8_t y, uint8_t no);
#endif