/*!
    \file    gd32f1x0_it.c
    \brief   interrupt service routines

    \version 2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    \version 2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    \version 2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2019-11-20, V3.2.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2020-09-21, V3.3.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f1x0_it.h"
#include "systick.h"
#include "main.h"
#include "gd32f1x0r_eval.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include "encoder.h"

extern USART_InitTypeDef usart1;

uint16_t test_number = 0;
uint16_t test_led_value = 0;
/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
    led_spark();
    delay_decrement(); /* delay 递减 */
}

/*******************************************************************************
 * @brief 外部中断0-1服务函数
 * @brief 处理旋转编码器脉冲信号
 ******************************************************************************/
void EXTI0_1_IRQHandler(void)
{
    if(SET == exti_interrupt_flag_get(ENCODER_CLK_EXTI_LINE)){
        usart_interrupt_enable(USART1, USART_INT_TBE); // 使能USART发送器空中断
        
        /* 读取DT 引脚的电平，即可判断旋转方向 */
        if(SET == gpio_input_bit_get(ENCODER_DT_GPIO_PORT, ENCODER_DT_PIN))
        {/* 正转 */
            test_number++;
        } else {/* 反转 */
            test_number--;
        }
        exti_interrupt_flag_clear(ENCODER_CLK_EXTI_LINE);
    }
    
    if(SET == exti_interrupt_flag_get(ENCODER_DT_EXTI_LINE)){
        
        exti_interrupt_flag_clear(ENCODER_DT_EXTI_LINE);
    }
}

/*******************************************************************************
 * @brief 外部中断2-3服务函数
 * @brief 处理旋转编码器SW信号
 ******************************************************************************/
void EXTI2_3_IRQHandler(void)
{
    if(SET == exti_interrupt_flag_get(ENCODER_SW_EXTI_LINE)){
        test_number++;
        exti_interrupt_flag_clear(ENCODER_SW_EXTI_LINE);
    }
}

/*******************************************************************************
 * @brief 串口1中断服务函数
 ******************************************************************************/
void USART1_IRQHandler(void)
{
    /* 接收器非空 */
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE))
    {
        uint8_t ch;
        ch = usart_data_receive(USART1);
        if('#' != ch)
        {
            usart1.rx_buffer[usart1.rx_count++] = ch;
        } else {
            usart1.rx_count = 0;
            usart1.rx_over  = 1;
        }
    }
    
    /* 发送器空 */
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_TBE))
    {
        usart_data_transmit(USART1, usart1.tx_buffer[usart1.tx_count++]);
        if(usart1.tx_count == TX_BUF_LENGTH)
        {
            usart1.tx_count = 0;
            usart_interrupt_disable(USART1, USART_INT_TBE);
        }
    }

    /* 空闲判断 */
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_IDLE))
    {
        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_IDLE);
    }
}


/*******************************************************************************
 * @brief TIMER1 中断处理函数
 ******************************************************************************/
void TIMER1_IRQHandler(void)
{
    if(SET == timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP)){
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
//        test_led_value++;
//        if(test_led_value > 100){
            gd_eval_led_toggle(LED1);
//            test_led_value = 0;
//        }
        
    }
    
}
