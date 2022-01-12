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
extern EC11_t ec11_1;
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
    delay_decrement();
    
    if(ec11_1.sw_down_flag == 1){
        ec11_1.sw_down_time++;
    }

    // 当 SW 为长按时，让 led_auto_setp++
    if(ec11_1.sw_state == SW_LONG_PRESS)
    {
        ec11_1.sw_long_press_time++;
    }
}

/*******************************************************************************
 * @brief 0-1中断
 * @brief CLK 与 DT
 ******************************************************************************/
void EXTI0_1_IRQHandler(void)
{
    // 触发外部中断 CLK
    if(SET == exti_interrupt_flag_get(ENCODER_CLK_EXTI_LINE) && clk_value == RESET && ec11_1.clk_count == 0)
    {
        ec11_1.clk_flag = 0;
        if(dt_value) ec11_1.clk_flag = 1;

        ec11_1.clk_count = 1;
        exti_interrupt_flag_clear(ENCODER_CLK_EXTI_LINE);
    }

    if(SET == exti_interrupt_flag_get(ENCODER_CLK_EXTI_LINE) && clk_value == SET && ec11_1.clk_count == 1)
    {
        if(dt_value ==0 && ec11_1.clk_flag == 1)
        {
            #if ENABLE_LONG_PRESS == ENABLE
            ec11_1.direction = EC11_CW;
            #elif ENABLE_LONG_PRESS == DISABLE
            if(sw_value == RESET)
            {
                ec11_1.direction = EC11_DOWN_CW; // 按钮按下，并逆时针旋转
            } else {
                ec11_1.direction = EC11_CW; // 顺时针
            }
            #endif 
        }
        
        if(dt_value && ec11_1.clk_flag == 0)
        {
            if(sw_value == RESET)
            {
                ec11_1.direction = EC11_DOWN_CCW; // 按钮按下，并逆时针旋转
            } else {
                ec11_1.direction = EC11_CCW; // 顺时针
            }
        }
        ec11_1.clk_count = 0;
        exti_interrupt_flag_clear(ENCODER_CLK_EXTI_LINE);
    }
}

/*******************************************************************************
 * @brief 2-3 外部中断
 * @brief SW
 ************************************************************************** ****/
void EXTI2_3_IRQHandler(void)
{
    if(SET == exti_interrupt_flag_get(ENCODER_SW_EXTI_LINE)){
        ec11_1.sw_down_flag = 1;
        exti_interrupt_flag_clear(ENCODER_SW_EXTI_LINE);
    }
}

/*******************************************************************************
 * @brief 串口1中断
 ******************************************************************************/
void USART1_IRQHandler(void)
{
    
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
    
    
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_TBE))
    {
        usart_data_transmit(USART1, usart1.tx_buffer[usart1.tx_count++]);
        if(usart1.tx_count == TX_BUF_LENGTH)
        {
            usart1.tx_count = 0;
            usart_interrupt_disable(USART1, USART_INT_TBE);
        }
    }

    
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_IDLE))
    {
        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_IDLE);
    }
}


/*******************************************************************************
 * @brief 定时器1中断
 ******************************************************************************/
void TIMER1_IRQHandler(void)
{
    if(SET == timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP)){
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
        gd_eval_led_toggle(LED1); 
    }
}
