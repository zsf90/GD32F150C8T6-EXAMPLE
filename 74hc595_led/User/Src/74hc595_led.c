/*******************************************************************************
 * @file        74hc595_led.c
 * @date        2021.10.21
 * @author      信念D力量
 * @brief       使用 74HC595 用串行线控制多位 LED 数码管
 * @github      https://github.com/zsf90/GD32F150C8T6-EXAMPLE
 * @bilibili    https://space.bilibili.com/334715750
 ******************************************************************************/
#include "74hc595_led.h"

#include "gd32f1x0.h"
#include "systick.h"
#include <string.h>

#define LOAD_PORT   GPIOA
#define LOAD_PIN    GPIO_PIN_12
#define LOAD_LOW    gpio_bit_reset(LOAD_PORT, LOAD_PIN)
#define LOAD_HIGH   gpio_bit_set(LOAD_PORT, LOAD_PIN)



/* 共阳 */
uint8_t led_code[] = {
    0xc0,   // 0 ->0
    0xf9,   // 1 ->1
    0xa4,   // 2 ->2
    0xb0,   // 3 ->3
    0x99,   // 4 ->4
    0x92,   // 5 ->5
    0x82,   // 6 ->6
    0xf8,   // 7 ->7
    0x80,   // 8 ->8
    0x90,   // 9 ->9
    0x7f,   // . ->10
    0x88,   // A ->11
    0x83,   // B ->12
    0xc6,   // C ->13
    0xa1,   // D ->14
    0x86,   // E ->15
    0x8e,   // F ->16
    0xc2,   // G ->17
    0x89,   // H ->18
    0xf0,   // I ->19
    0xf1,   // J ->20
    0xa8,   // K ->21
    0xc7,   // L ->22
    0xc8,   // M ->23
    0xab,   // N ->24
    0xa3,   // O ->25
    0x8c,   // P ->26
    0x98,   // Q ->27
    0xaf,   // R ->28
    0xb6,   // S ->29
    0x87,   // T ->30
    0xc1,   // U ->31
    0x81,   // V ->32
    0x7e,   // W ->33
    0x9B,   // X ->34
    0x91,   // Y ->35
    0xa5,   // Z ->36
    0xbf,   // - ->37
    0x00,   // 全亮 ->38
    0xff    // 全灭 ->39
};

uint8_t led_flag_code[][4] = {
    { 0x86, 0xaf, 0xaf, 0xc0 },// ERR0 ->0
    { 0x86, 0xaf, 0xaf, 0xf9 },// ERR1 ->1
    { 0x86, 0xaf, 0xaf, 0xa4 },// ERR2 ->2
    { 0x86, 0xaf, 0xaf, 0xb0 },// ERR3 ->3
    { 0x86, 0xaf, 0xaf, 0x99 },// ERR4 ->4
    { 0x8e, 0xc1, 0xc0, 0xf9 },// FU01 ->5
    { 0x8e, 0xc1, 0xc0, 0xa4 },// FU02 ->6
    { 0x8e, 0xc1, 0xc0, 0xb0 },// FU03 ->7
    { 0x8e, 0xc1, 0xc0, 0x99 },// FU04 ->8
    { 0x8e, 0xc1, 0xc0, 0x92 },// FU05 ->9
    { 0xff, 0xff, 0xff, 0xff },// 全灭 ->10
    { 0x00, 0x00, 0x00, 0x00 },// 全亮 ->11
};

/* 文件内函数声明 */
static void spi_config(void);
static void gpio_config(void);
static void rcu_config(void);
static void show_string(LED_Index_enum);


/* 数码管 SPI 初始化 */

/*******************************************************************************
 * @brief 数码管 SPI 配置
 ******************************************************************************/
void init_74hc595(void)
{
    rcu_config();       /* RCU Config */
    gpio_config();      /* GPIO Config */
    spi_config();       /* SPI Config */
    spi_enable(SPI0);   /* 使能 SPI0 */
}

/*******************************************************************************
 * @brief SPI 配置
 ******************************************************************************/
void spi_config(void)
{
    spi_parameter_struct spi_init_struct;
    /* deinitialize SPI and the parameters */
    spi_i2s_deinit(SPI0);
    spi_struct_para_init(&spi_init_struct);
    
    /* SPI0 parameter config */
    spi_init_struct.trans_mode              = SPI_TRANSMODE_FULLDUPLEX; /* SPI 通信模式配置 */
    spi_init_struct.device_mode             = SPI_MASTER; /* 主机从机模式配置 */
    spi_init_struct.frame_size              = SPI_FRAMESIZE_8BIT; /* 数据帧大小 */
    spi_init_struct.clock_polarity_phase    = SPI_CK_PL_LOW_PH_1EDGE; /* PL 空闲时低电平，PH 第一个边沿采样 */
    spi_init_struct.nss                     = SPI_NSS_SOFT; /* SPI NSS control by handware or software */
    spi_init_struct.prescale                = SPI_PSC_16; /* SPI预分频因子 */
    spi_init_struct.endian                  = SPI_ENDIAN_MSB; /* 高位先发 */
    spi_init(SPI0, &spi_init_struct);
}

/*******************************************************************************
 * @brief SPI GPIO 配置
 ******************************************************************************/
void gpio_config(void)
{
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_12);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_12);
    LOAD_LOW;
    
    /* SPI0 GPIO config: SCK/PA5, MOSI/PA7 */
    gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_5 | GPIO_PIN_7);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5 | GPIO_PIN_7);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_7);
    
    
}

/*******************************************************************************
 * @brief RCU 配置
 ******************************************************************************/
void rcu_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_SPI0);
}

/*******************************************************************************
 * @brief 显示数字
 ******************************************************************************/
void hc595_show_number(uint16_t n)
{
    uint8_t ge = n % 10;
    uint8_t shi = n / 10 % 10;
    uint8_t bai = n / 100 % 10;
    uint8_t qian = n / 1000;
    uint8_t i;
    for(i=0; i<4; i++){
        switch(i)
        {
        case 0:
            spi_rw_byte(led_code[ge]);
            break;
        case 1:
            if(n < 10)
            {
                spi_rw_byte(led_code[39]);
            } else {
                spi_rw_byte(led_code[shi]);
            }
            break;
        case 2:
            if(n < 100)
            {
                spi_rw_byte(led_code[39]);
            } else {
                spi_rw_byte(led_code[bai]);
            }
            break;
        case 3:
            if(n < 1000)
            {
                spi_rw_byte(led_code[39]);
            } else {
                spi_rw_byte(led_code[qian]);
            }
            break;
        }
    }
    LOAD_HIGH;
    delay_1ms(1);
    LOAD_LOW;
}

/*******************************************************************************
* @brief SPI 发送一个字节数据
 ******************************************************************************/
uint8_t spi_rw_byte(uint8_t data)
{
    uint16_t tmp;
    while(spi_i2s_flag_get(SPI0, SPI_FLAG_TBE) == RESET);
    spi_i2s_data_transmit(SPI0, data);
    while(spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE) == RESET);
    tmp = spi_i2s_data_receive(SPI0);
    return ((uint8_t)(tmp & 0xff));
}

/*******************************************************************************
 * @brief 数码管测试
 ******************************************************************************/
void hc595_all_test(void)
{
    for(uint8_t x=0; x<40; x++){
        for(uint8_t i=0; i<4; i++)
        {
            switch(i)
            {
                case 0:
                    spi_rw_byte(led_code[x]);
                    break;
                case 1:
                    spi_rw_byte(led_code[39]);
                    break;
                case 2:
                    spi_rw_byte(led_code[39]);
                    break;
                case 3:
                    spi_rw_byte(led_code[39]);
                    break;
            }
        }
        LOAD_HIGH;
        delay_1ms(1);
        LOAD_LOW;
        delay_1ms(500);
    }  
}


void hc595_show_strings(char *str)
{
    if(strcmp(str, "ERR0") == 0){
        hc595_show_string(ERR0);
    }
    else if(strcmp(str, "ERR1") == 0) {
        hc595_show_string(ERR1);
    }
    else if(strcmp(str, "ERR2") == 0) {
        hc595_show_string(ERR2);
    }
    else if(strcmp(str, "ERR3") == 0) {
        hc595_show_string(ERR3);
    }
    else if(strcmp(str, "ERR4") == 0) {
        hc595_show_string(ERR4);
    }
    else if(strcmp(str, "FU-0") == 0) {
        hc595_show_string(FU01);
    }
    else if(strcmp(str, "FU-1") == 0) {
        hc595_show_string(FU02);
    }
    else if(strcmp(str, "FU-2") == 0) {
        hc595_show_string(FU03);
    }
    else if(strcmp(str, "FU-3") == 0) {
        hc595_show_string(FU04);
    }
    else if(strcmp(str, "FU-4") == 0) {
        hc595_show_string(FU05);
    }

}

void hc595_show_string(LED_Index_enum str)
{
    switch(str)
    {
    case ERR0:
        show_string(str);
        break;
    case ERR1:
        show_string(str);
        break;
    case ERR2:
        show_string(str);
        break;
    case ERR3:
        show_string(str);
        break;
    case ERR4:
        show_string(str);
        break;
    case FU01:
        show_string(str);
        break;
    case FU02:
        show_string(str);
        break;
    case FU03:
        show_string(str);
        break;
    case FU04:
        show_string(str);
        break;
    case FU05:
        show_string(str);
        break;
    }
}


static void show_string(LED_Index_enum s)
{
    for(uint8_t i=0; i<4; i++){
        switch(i)
        {
        case 0:
            spi_rw_byte(led_flag_code[s][3]);
            break;
        case 1:
            spi_rw_byte(led_flag_code[s][2]);
            break;
        case 2:
            spi_rw_byte(led_flag_code[s][1]);
            break;
        case 3:
            spi_rw_byte(led_flag_code[s][0]);
            break;
        }
    }
    LOAD_HIGH;
    delay_1ms(1);
    LOAD_LOW;
}