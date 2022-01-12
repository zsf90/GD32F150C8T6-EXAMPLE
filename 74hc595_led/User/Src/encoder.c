/*******************************************************************************
 * @file        encoder.c
 * @date        2021.10.21
 * @author      信念D力量
 * @brief       EC11 旋转编码器
 * @github      https://github.com/zsf90/GD32F150C8T6-EXAMPLE
 * @bilibili    https://space.bilibili.com/334715750
 ******************************************************************************/
#include "encoder.h"

// 创建结构体并初始化
EC11_t ec11_1 = {
    .sw_down_count = 0,
    .sw_down_flag = 0,
    .sw_down_time = 0,
    .sw_long_press_time = 0,
    .sw_state = SW_DEFAULT,

    // 方向判断
    .clk_flag = 0,
    .clk_count = 0,
    .direction = EC11_NONE_W,
};

extern uint16_t test_number;

/*******************************************************************************
 * @brief 旋转编码器外设配置
 ******************************************************************************/
void encoder_exit_config(encoder_mode_enum _zsf_eme)
{
    /* 使能外设时钟 */
    rcu_periph_clock_enable(ENCODER_CLK_GPIO_CLK);
    rcu_periph_clock_enable(ENCODER_DT_GPIO_CLK);
    rcu_periph_clock_enable(ENCODER_SW_GPIO_CLK);
    rcu_periph_clock_enable(RCU_CFGCMP);

    /* configure button pin as input */
    gpio_mode_set(ENCODER_CLK_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, ENCODER_CLK_PIN);
    gpio_mode_set(ENCODER_DT_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, ENCODER_DT_PIN);
    gpio_mode_set(ENCODER_SW_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, ENCODER_SW_PIN);

    if (_zsf_eme == ENCODER_MODE_EXIT)
    {
        /* enable and set key EXTI interrupt to the lowest priority */
        nvic_irq_enable(ENCODER_CLK_EXTI_IRQn, 1U, 1U);
        nvic_irq_enable(ENCODER_SW_EXTI_IRQn, 1U, 1U);

        /* connect key EXTI line to key GPIO pin */
        syscfg_exti_line_config(ENCODER_CLK_EXTI_PORT_SOURCE, ENCODER_CLK_EXTI_PIN_SOURCE);
        syscfg_exti_line_config(ENCODER_SW_EXTI_PORT_SOURCE, ENCODER_SW_EXTI_PIN_SOURCE);

        /* configure key EXTI line */
        exti_init(ENCODER_CLK_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
        exti_interrupt_flag_clear(ENCODER_CLK_EXTI_LINE);
        exti_init(ENCODER_SW_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(ENCODER_SW_EXTI_LINE);
    }
}

/*******************************************************************************
 * @brief 旋转编码器处理函数
 ******************************************************************************/
void encoder_handle(void)
{
    // FlagStatus SW_State = gpio_input_bit_get(ENCODER_SW_GPIO_PORT, ENCODER_SW_PIN);
    if(ec11_1.sw_down_flag == 1 && ec11_1.sw_down_time > 10 && ec11_1.sw_down_count == 0)
    {
        if(sw_value == RESET)
        {
            ec11_1.sw_down_count = 1;
        }
    }

    /* 按键长按 */
    if(ec11_1.sw_down_count == 1 && ec11_1.sw_down_time > 500 && sw_value == RESET)
    {
        ec11_1.sw_state = SW_LONG_PRESS;    // 长按
    }
    if(ec11_1.sw_down_count == 1 && ec11_1.sw_down_time > 500 && sw_value == SET)
    {
        ec11_1.sw_down_count = 0;   // 长按需要把 sw_down_count 清零
        ec11_1.sw_down_time = 0;    // 长按需要 sw_down_time 清零
        ec11_1.sw_down_flag = 0;    // 清除 sw_down_flag 标志
        ec11_1.sw_state = SW_DEFAULT;    // 模式清除
    }

    /* 单击双击判断 */
    if(ec11_1.sw_down_count == 1 && sw_value == SET && ec11_1.sw_down_time < 500)
    {
        ec11_1.sw_down_count = 2; // sw_down_time < 500 需要判断双击还是单击
        ec11_1.sw_down_time = 0;
    }

    if(ec11_1.sw_down_flag == 1 && ec11_1.sw_down_count == 2 && ec11_1.sw_down_time > 100)
    {
        if(sw_value == RESET)
        {
            ec11_1.sw_state = SW_DOUBLE_CLICK;    // 双击
            ec11_1.sw_down_time = 0;
            ec11_1.sw_down_flag = 0;
            ec11_1.sw_down_count = 0;
        } else {
            ec11_1.sw_state = SW_CLICK;    // 单击
            ec11_1.sw_down_time = 0;
            ec11_1.sw_down_flag = 0;
            ec11_1.sw_down_count = 0;
        }
    }

    /* 按键处理 */
    switch (ec11_1.sw_state)
    {
    case SW_CLICK: // 单击

        ec11_1.sw_state = SW_DEFAULT;
        break;
    case SW_DOUBLE_CLICK: // 双击
        test_number = 0;
        ec11_1.sw_state = SW_DEFAULT;
        break;
    #if ENABLE_LONG_PRESS
    case SW_LONG_PRESS: // 长按
        if(ec11_1.direction == SW_DEFAULT)
        {
            // 指定多少毫秒加1
            if(ec11_1.sw_long_press_time == LED_AUTO_SETP)
            {
                test_number = 0;
                ec11_1.sw_long_press_time = 0;
            }
            
            // FlagStatus sw_state_temp = gpio_input_bit_get(ENCODER_SW_GPIO_PORT, ENCODER_SW_PIN);
            if(sw_value == SET)
            {
                ec11_1.sw_state = SW_DEFAULT;
            }
        }
        break;
    #endif
    default:
        break;
    }


    /* 旋转方向处理 */
    switch (ec11_1.direction)
    {
    case EC11_CW:   // 顺时针
        test_number++;
        ec11_1.direction = EC11_NONE_W;
        break;
    case EC11_CCW:  // 逆时针
        test_number--;
        ec11_1.direction = EC11_NONE_W;
        break;
    case EC11_DOWN_CW:  // 按下按钮并顺时针旋转
        test_number += 10;
        ec11_1.direction = EC11_NONE_W;
        break;
    case EC11_DOWN_CCW: // 按下按钮并逆时针旋转
        test_number -= 10;
        ec11_1.direction = EC11_NONE_W;
        break;
    
    default:
        break;
    }
}
