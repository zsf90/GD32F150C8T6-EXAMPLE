#include "usart.h"
#include <stdio.h>
#include "gd32f1x0.h"
#include "gd32f1x0_usart.h"

void usart1_init(void)
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
    usart_baudrate_set(USART1,115200U);
    usart_stop_bit_set(USART1, 1);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_enable(USART1); /* 使能串口1 */
}

#ifdef GD32F130_150
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART1, (uint8_t)ch);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
    return ch;
}
#endif
