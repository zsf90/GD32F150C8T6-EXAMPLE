/*******************************************************************************
 * @file        encoder.c
 * @date        2021.10.21
 * @author      信念D力量
 * @brief       旋转编码器源文件
 * @github      https://github.com/zsf90/GD32F150C8T6-EXAMPLE
 * @bilibili    https://space.bilibili.com/334715750
 ******************************************************************************/
#include "encoder.h"

void encoder_exit_config(encoder_mode_enum _zsf_eme)
{
    rcu_periph_clock_enable(ENCODER_CLK_GPIO_CLK);  /* 使能 ENCODER 外设时钟 */
    rcu_periph_clock_enable(ENCODER_DT_GPIO_CLK);   /* 使能 ENCODER 外设时钟 */
    rcu_periph_clock_enable(ENCODER_SW_GPIO_CLK);   /* 使能 ENCODER 外设时钟 */
    rcu_periph_clock_enable(RCU_CFGCMP); /* 开启系统配置与比较器模块时钟 */
    
    /* configure button pin as input */
    gpio_mode_set(ENCODER_CLK_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, ENCODER_CLK_PIN);
    gpio_mode_set(ENCODER_DT_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, ENCODER_DT_PIN);
    gpio_mode_set(ENCODER_SW_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, ENCODER_SW_PIN);
    
    if(_zsf_eme == ENCODER_MODE_EXIT)
    {
        /* enable and set key EXTI interrupt to the lowest priority */
        nvic_irq_enable(ENCODER_CLK_EXTI_IRQn, 1U, 1U);
        nvic_irq_enable(ENCODER_DT_EXTI_IRQn, 1U, 1U);
        nvic_irq_enable(ENCODER_SW_EXTI_IRQn, 1U, 1U);
        
        /* connect key EXTI line to key GPIO pin */
        syscfg_exti_line_config(ENCODER_CLK_EXTI_PORT_SOURCE, ENCODER_CLK_EXTI_PIN_SOURCE);
        syscfg_exti_line_config(ENCODER_DT_EXTI_PORT_SOURCE, ENCODER_DT_EXTI_PIN_SOURCE);
        syscfg_exti_line_config(ENCODER_SW_EXTI_PORT_SOURCE, ENCODER_SW_EXTI_PIN_SOURCE);
        
        /* configure key EXTI line */
        exti_init(ENCODER_CLK_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(ENCODER_CLK_EXTI_LINE);
        exti_init(ENCODER_DT_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(ENCODER_DT_EXTI_LINE);
        exti_init(ENCODER_SW_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(ENCODER_SW_EXTI_LINE);
    }
    
}



