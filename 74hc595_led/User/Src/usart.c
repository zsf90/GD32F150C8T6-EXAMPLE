/*******************************************************************************
 * @file        usart.c
 * @date        2021.10.21
 * @author      信念D力量
 * @brief       串口源文件
 * @github      https://github.com/zsf90/GD32F150C8T6-EXAMPLE
 * @bilibili    https://space.bilibili.com/334715750
 ******************************************************************************/
#include "usart.h"
#include "gd32f1x0.h"
#include "gd32f1x0_usart.h"
#include "gd32f1x0r_eval.h"
#include <stdio.h>
#include <string.h>

#define HELLO   ("Hello,World\n")

USART_InitTypeDef usart1;

/*******************************************************************************
 * @brief 串口配置
 ******************************************************************************/
void usart_config(void)
{
    rcu_periph_clock_enable(RCU_USART1);    /* 使能 USART1 时钟*/
    usart_deinit(USART1);
    
    /* 链接 port PA2 到串口 Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2);
    /* 链接 port PA3 到串口 Rx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_3);
     
    /* gpio config */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2); /* PA2 Tx 复用功能上拉 */
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3); /* PA3 Rx 复用功能上拉 */
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_3);
    
    /* USART configure */
    usart_deinit(USART1);
    usart_baudrate_set(USART1,115200U);                     /* 波特率设置 */
    usart_stop_bit_set(USART1, 1);                          /* 停止位长度 */
    usart_word_length_set(USART1, USART_WL_8BIT);           /* 数据位长度 */
    usart_parity_config(USART1, USART_PM_NONE);             /* 奇偶校验设置 */
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);   /* 发送数据使能 */
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);     /* 接收数据使能 */
    usart_enable(USART1);                                   /* 使能串口1 */
    
    /* 使能USART1中断向量控制器，设置抢占优先级和相应优先级都为0 */
    nvic_irq_enable(USART1_IRQn, 0, 0);
    /* 使能USART1发送寄存器空中断使能 */
    usart_interrupt_disable(USART1, USART_INT_TBE);
    /* 使能读数据缓冲区非空中断 */
    usart_interrupt_enable(USART1, USART_INT_RBNE);
    /* 使能空闲中断 */
    usart_interrupt_disable(USART1, USART_INT_IDLE);
    
    usart_init_typedef(&usart1);
}


/*******************************************************************************
 * @brief 串口自定义类型初始化
 ******************************************************************************/
void usart_init_typedef(USART_InitTypeDef *u)
{
    u->rx_count     = 0;
    u->rx_over      = 0;
    u->tx_count     = 0;
    u->tx_over      = 0;
    strcpy((char*)u->tx_buffer, HELLO);
}

/*******************************************************************************
 * @brief 接收处理
 ******************************************************************************/
void receive_handle(void)
{
    if(usart1.rx_over == 1)
    {
        printf("接收数据完成\n");
        if(strstr((const char*)usart1.rx_buffer, "LED_ON") != NULL)
        {
            //gd_eval_led_off(LED3);
        }

        if(strstr((const char*)usart1.rx_buffer, "LED_OFF") != NULL)
        {
            //gd_eval_led_on(LED3);
        }
        usart1.rx_over = 0;
    }
}

#ifdef GD32F130_150
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART1, (uint8_t)ch);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
    return ch;
}
#endif
