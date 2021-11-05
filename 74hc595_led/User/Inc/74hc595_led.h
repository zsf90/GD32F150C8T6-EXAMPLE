/*******************************************************************************
 * @file        74hc595_led.h
 * @date        2021.10.21
 * @author      信念D力量
 * @brief       使用 74HC595 用串行线控制多位 LED 数码管
 * @github      https://github.com/zsf90/GD32F150C8T6-EXAMPLE
 * @bilibili    https://space.bilibili.com/334715750
 ******************************************************************************/
#ifndef __74HC595_LED_H
#define __74HC595_LED_H

#include "gd32f1x0.h"

extern uint8_t led_code[];


typedef enum
{
    ERR0=0,
    ERR1,
    ERR2,
    ERR3,
    ERR4,
    
    FU0,
    FU1,
    FU2,
    FU3,
    FU4
} LED_Index_enum;

/* 数码管 SPI 初始化 */
void init_74hc595(void);
uint8_t spi_rw_byte(uint8_t data);
void hc595_show_number(uint16_t n);
void hc595_show_strings(char *str);
void hc595_all_test(void);
void hc595_show_string(LED_Index_enum str);
#endif /* __74HC595_LED_H */
