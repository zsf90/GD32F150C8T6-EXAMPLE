#include "usart.h"
#include "gd32f1x0.h"
#include "gd32f1x0_usart.h"
#include "gd32f1x0r_eval.h"
#include <stdio.h>
#include <string.h>

#define HELLO   ("Hello,World\n")

USART_InitTypeDef usart1;

/*******************************************************************************
 * @brief ��������
 ******************************************************************************/
void usart_config(void)
{
    rcu_periph_clock_enable(RCU_USART1);    /* ʹ�� USART1 ʱ��*/
    usart_deinit(USART1);
    
    /* ���� port PA2 ������ Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2);
    /* ���� port PA3 ������ Rx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_3);
     
    /* gpio config */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2); /* PA2 Tx ���ù������� */
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3); /* PA3 Rx ���ù������� */
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_3);
    
    /* USART configure */
    usart_deinit(USART1);
    usart_baudrate_set(USART1,115200U);                     /* ���������� */
    usart_stop_bit_set(USART1, 1);                          /* ֹͣλ���� */
    usart_word_length_set(USART1, USART_WL_8BIT);           /* ����λ���� */
    usart_parity_config(USART1, USART_PM_NONE);             /* ��żУ������ */
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);   /* ��������ʹ�� */
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);     /* ��������ʹ�� */
    usart_enable(USART1);                                   /* ʹ�ܴ���1 */
    
    /* ʹ��USART1�ж�������������������ռ���ȼ�����Ӧ���ȼ���Ϊ0 */
    nvic_irq_enable(USART1_IRQn, 0, 0);
    /* ʹ��USART1���ͼĴ������ж�ʹ�� */
    usart_interrupt_disable(USART1, USART_INT_TBE);
    /* ʹ�ܶ����ݻ������ǿ��ж� */
    usart_interrupt_enable(USART1, USART_INT_RBNE);
    /* ʹ�ܿ����ж� */
    usart_interrupt_disable(USART1, USART_INT_IDLE);
    
    usart_init_typedef(&usart1);
}


/*******************************************************************************
 * @brief �����Զ������ͳ�ʼ��
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
 * @brief ���մ���
 ******************************************************************************/
void receive_handle(void)
{
    if(usart1.rx_over == 1)
    {
        printf("�����������\n");
        if(strstr((const char*)usart1.rx_buffer, "LED_ON") != NULL)
        {
            gd_eval_led_off(LED3);
        }

        if(strstr((const char*)usart1.rx_buffer, "LED_OFF") != NULL)
        {
            gd_eval_led_on(LED3);
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
