#include <stdio.h>
#include "oled.h"
#include "unistd.h"
/**
 * @brief i2c master initialization
 */

static int IICMasterPort;
static int IICMasterTimeout;

esp_err_t i2c_master_init(oled_iic_config_t *oled_iic_config) {
    int i2c_master_port = oled_iic_config->MasterNumber;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = oled_iic_config->SDA_IO,
        .scl_io_num = oled_iic_config->SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = oled_iic_config->MasterFreq
    };

    i2c_param_config(i2c_master_port, &conf);

    IICMasterPort = oled_iic_config->MasterNumber;
    IICMasterTimeout = oled_iic_config->MasterTimeOut;

    return i2c_driver_install(i2c_master_port, conf.mode, oled_iic_config->MasterRxBufDisable, oled_iic_config->MasterTxBufDisable, 0);
}

/**
 * @description: OLED 屏幕初始化
 * @return       无
 */
void OLEDInit(void)
{
    OLEDWriteByte(0xAE, OLED_CMD); //--display off
    OLEDWriteByte(0x00, OLED_CMD); //---set low column address
    OLEDWriteByte(0x10, OLED_CMD); //---set high column address
    OLEDWriteByte(0x40, OLED_CMD); //--set start line address
    OLEDWriteByte(0xB0, OLED_CMD); //--set page address
    OLEDWriteByte(0x81, OLED_CMD); // contract control
    OLEDWriteByte(0xFF, OLED_CMD); //--128
    OLEDWriteByte(0xA1, OLED_CMD); // set segment remap
    OLEDWriteByte(0xA6, OLED_CMD); //--normal / reverse
    OLEDWriteByte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    OLEDWriteByte(0x3F, OLED_CMD); //--1/32 duty
    OLEDWriteByte(0xC8, OLED_CMD); // Com scan direction
    OLEDWriteByte(0xD3, OLED_CMD); //-set display offset
    OLEDWriteByte(0x00, OLED_CMD); //
    OLEDWriteByte(0xD5, OLED_CMD); // set osc division
    OLEDWriteByte(0x80, OLED_CMD); //
    OLEDWriteByte(0xD8, OLED_CMD); // set area color mode off
    OLEDWriteByte(0x05, OLED_CMD); //
    OLEDWriteByte(0xD9, OLED_CMD); // Set Pre-Charge Period
    OLEDWriteByte(0xF1, OLED_CMD); //
    OLEDWriteByte(0xDA, OLED_CMD); // set com pin configuartion
    OLEDWriteByte(0x12, OLED_CMD); //
    OLEDWriteByte(0xDB, OLED_CMD); // set Vcomh
    OLEDWriteByte(0x30, OLED_CMD); //
    OLEDWriteByte(0x8D, OLED_CMD); // set charge pump enable
    OLEDWriteByte(0x14, OLED_CMD); //
    OLEDWriteByte(0xAF, OLED_CMD); //--turn on oled panel
    OLEDClear();
}

/**
 * @description: OLED 发送一个字节
 * @return       错误信息
 * @param {uint8_t} data 需要发送的内容，数据或者命令
 * @param {uint8_t} cmd_ 1:发送数据 0:发送命令
 */
esp_err_t OLEDWriteByte(uint8_t data, uint8_t cmd_) {
    int ret;

    uint8_t write_buf[2];
    if (cmd_ == 1) {
        write_buf[0] = 0x40;
    } else {
        write_buf[0] = 0x00;
    }
    write_buf[1] = data;

    ret = i2c_master_write_to_device(IICMasterPort, OLED_ADDR, write_buf, sizeof(write_buf), IICMasterTimeout / portTICK_PERIOD_MS);

    return ret;
}

/**
 * @description: OLED 屏幕 设置坐标
 * @return       无
 * @param {uint8_t} x 坐标x轴，范围0~127
 * @param {uint8_t} y 坐标y轴，范围0~63
 */
void OLEDSetPosition(uint8_t x, uint8_t y) {
    OLEDWriteByte(0xb0 + y, OLED_CMD);
    OLEDWriteByte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLEDWriteByte((x & 0x0f), OLED_CMD);
}

/**
 * @description: OLED 清屏
 * @return       无
 */
void OLEDClear(void) {
    uint8_t i, n;
    for (i = 0; i < 8; i++) {
        OLEDWriteByte(0xb0 + i, OLED_CMD);
        OLEDWriteByte(0x00, OLED_CMD);
        OLEDWriteByte(0x10, OLED_CMD);
        for (n = 0; n < 128; n++)
            OLEDWriteByte(0, OLED_DATA);
    }
}


/**
 * @description: OLED 清除指定行
 * @return       无
 * @param {uint8_t} i 行号，范围0~7
 */

void OLEDClearLine(uint8_t i) {
    uint8_t n;

    // 检查行号是否在有效范围内
    if (i >= 8) {
        return; // 如果行号无效，则直接返回
    }

    // 设置页地址
    OLEDWriteByte(0xb0 + i, OLED_CMD);
    // 设置列地址低位和高位
    OLEDWriteByte(0x00, OLED_CMD);
    OLEDWriteByte(0x10, OLED_CMD);

    // 清除指定行的所有列
    for (n = 0; n < 128; n++) {
        OLEDWriteByte(0, OLED_DATA);
    }
}

/**
 * @description: OLED 显示单个字符
 * @return       无
 * @param {uint8_t} x 显示字符的x坐标，范围0~127
 * @param {uint8_t} y 显示字符的y坐标，字符大小为16，取值0,2,4,6；字符大小6，取值0,1,2,3,4,5,6,7
 * @param {uint8_t} chr 显示的单个字符，在字库中出现的字符
 * @param {uint8_t} Char_Size 字符大小，取16或者8
 */
void OLEDShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size) {
    uint8_t c = 0;
    uint8_t i = 0;
    c = chr - ' ';
    if (x > 127)
    {
        x = 0;
        y = y + 2;
    }
    if (Char_Size == 16)
    {
        OLEDSetPosition(x, y);
        for (i = 0; i < 8; i++)
            OLEDWriteByte(F8X16[c * 16 + i], OLED_DATA);
        OLEDSetPosition(x, y + 1);
        for (i = 0; i < 8; i++)
            OLEDWriteByte(F8X16[c * 16 + i + 8], OLED_DATA);
    }
    else
    {
        OLEDSetPosition(x, y);
        for (i = 0; i < 6; i++)
            OLEDWriteByte(F6x8[c][i], OLED_DATA);
    }
}

/**
 * @description: OLED 显示字符串，会自动换行
 * @return       无
 * @param {uint8_t} x 显示字符串第一个字符的x坐标，范围0~127
 * @param {uint8_t} y 显示字符串第一个字符的y坐标，字符大小为16，取值0,2,4,6；字符大小6，取值0,1,2,3,4,5,6,7
 * @param {char} *chr 显示的字符串
 * @param {uint8_t} Char_Size 字符大小，取16或者8
 * @note       字符大小为16，最多显示16个字符；字符大小为6，最多显示16个字符
 */
void OLEDShowString(uint8_t x, uint8_t y, char *chr, uint8_t Char_Size) {
    unsigned char j = 0;
    while (chr[j] != '\0')
    {
        OLEDShowChar(x, y, chr[j], Char_Size);
        x += 8;
        if (x > 120)
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}

/**
 * @description: OLED 显示汉字
 * @return       无
 * @param {uint8_t} x 显示汉字的x坐标
 * @param {uint8_t} y 显示汉字的y坐标
 * @param {uint8_t} no 显示汉字在字库中的序号
 */
void OLEDShowChinese(uint8_t x, uint8_t y, uint8_t no)  // 汉字渲染字符串每一个汉字渲染后x坐标加16
{
    uint8_t t, adder = 0;
    OLEDSetPosition(x, y);
    for (t = 0; t < 16; t++)
    {
        OLEDWriteByte(Hzk[2 * no][t], OLED_DATA);
        adder += 1;
    }
    OLEDSetPosition(x, y + 1);
    for (t = 0; t < 16; t++)
    {
        OLEDWriteByte(Hzk[2 * no + 1][t], OLED_DATA);
        adder += 1;
    }
}

/**
 * @description: OLED 显示自定义字符串
 * @return       无
 * @param {uint8_t} x 显示自定义字符串的第一个字符的x坐标
 * @param {uint8_t} y 显示自定义字符串的第一个字符的y坐标
 * @param {uint8_t} no 显示自定义字符串在字库中的序号
 */

void OLEDShowCustom(uint8_t x, uint8_t y, uint8_t no)  // 渲染自定义字符串每一个字符渲染后x坐标加16
{
    uint8_t t, adder = 0;
    OLEDSetPosition(x, y);
    for (t = 0; t < 8; t++)
    {
        OLEDWriteByte(Custom[2 * no][t], OLED_DATA);
        adder += 1;
    }
    OLEDSetPosition(x, y + 1);
    for (t = 0; t < 8; t++)
    {
        OLEDWriteByte(Custom[2 * no][t+8], OLED_DATA);
        adder += 1;
    }
    OLEDSetPosition(x, y);
}

/**
 * @description: 求m^n的函数
 * @return       m^n的结果
 * @param {uint8_t} m 底数
 * @param {uint8_t} n 指数
 */
uint32_t oled_pow(uint8_t m, uint8_t n) {
    uint32_t result = 1;
    while (n--)
        result *= m;
    return result;
}

/**
 * @description: OLED 显示数字
 * @return       无
 * @param {uint8_t} x 显示数字的第一个位置的x坐标
 * @param {uint8_t} y 显示数字的第一个位置的y坐标
 * @param {uint32_t} num 欲显示的数字
 * @param {uint8_t} len 显示所占的长度，不建议小于真正要显示的数字的长度
 * @param {uint8_t} size2 显示的数字的大小，16、8可选
 */
void OLEDShowNumber(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size2) {
    uint8_t t, temp;
    uint8_t enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                OLEDShowChar(x + (size2 / 2) * t, y, ' ', size2);
                continue;
            } else {
                enshow = 1;
            }
        }
        OLEDShowChar(x + (size2 / 2) * t, y, temp + '0', size2);
    }
}