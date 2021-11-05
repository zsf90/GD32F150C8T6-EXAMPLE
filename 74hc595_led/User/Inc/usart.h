/*******************************************************************************
 * @file        usart.h
 * @date        2021.10.21
 * @author      信念D力量
 * @brief       串口头文件
 * @github      https://github.com/zsf90/GD32F150C8T6-EXAMPLE
 * @bilibili    https://space.bilibili.com/334715750
 ******************************************************************************/
#ifndef __USART_H
#define __USART_H

#include "gd32f1x0.h"

#define TX_BUF_LENGTH   (50)
#define RX_BUF_LENGTH   (50)


/* 串口结构架构 */
typedef struct
{
    uint8_t tx_buffer[TX_BUF_LENGTH];
    uint8_t rx_buffer[RX_BUF_LENGTH];
    __IO uint8_t tx_count;
    __IO uint8_t rx_count;
    uint8_t rx_over;
    uint8_t tx_over;
} USART_InitTypeDef;


void usart_config(void);
void usart_init_typedef(USART_InitTypeDef *u);
void receive_handle(void);


extern USART_InitTypeDef usart1;

#endif
