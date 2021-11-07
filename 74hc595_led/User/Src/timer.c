/*******************************************************************************
 * @file        timer.c
 * @date        2021.11.6
 * @author      信念D力量
 * @brief       定时器的使用
 * @github      https://github.com/zsf90/GD32F150C8T6-EXAMPLE
 * @bilibili    https://space.bilibili.com/334715750
 ******************************************************************************/
#include "timer.h"
#include "gd32f1x0.h"

/*******************************************************************************
 * @brief 定时器的配置
 ******************************************************************************/
void timer_config(void)
{
    
    
    /* 创建定时器结构体对象 */
    timer_parameter_struct      timer_initpara;
    timer_ic_parameter_struct   timer_icinitpara;
    
    /* NVIC Config */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(TIMER1_IRQn, 1, 2);
    /* 使能TIMER1时钟源 */
    rcu_periph_clock_enable(RCU_TIMER1);
    
    /* 复位定时器外设 */
    timer_deinit(TIMER1);

    
    /* 初始化定时器外设结构 */
    timer_struct_para_init(&timer_initpara);
    /* TIMER1 Configuration */
    timer_initpara.prescaler    = 7199;    /* 预分频器值 = 10KHz */
    timer_initpara.alignedmode  = TIMER_COUNTER_EDGE; /* center-aligned mode selection */
    timer_initpara.counterdirection = TIMER_COUNTER_UP; /* 向上计数 */
    timer_initpara.period           = 4999;
    timer_initpara.clockdivision    = TIMER_CKDIV_DIV1;
    timer_init(TIMER1, &timer_initpara);
    
    /* TIMER1 CH0 input capture configuration */
//    timer_icinitpara.icpolarity = TIMER_IC_POLARITY_RISING;
//    timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
//    timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;
//    timer_input_capture_config(TIMER1, TIMER_CH_0, &timer_icinitpara);
    
    
    
    
    /* enable the TIMER interrupt */
    timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
    timer_interrupt_enable(TIMER1, TIMER_INT_UP);
    timer_enable(TIMER1);
    
    
}







