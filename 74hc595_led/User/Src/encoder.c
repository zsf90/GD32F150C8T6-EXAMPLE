/*******************************************************************************
 * @file        encoder.c
 * @date        2021.10.21
 * @author      信念D力量
 * @brief       EC11 旋转编码器
 * @github      https://github.com/zsf90/GD32F150C8T6-EXAMPLE
 * @bilibili    https://space.bilibili.com/334715750
 ******************************************************************************/
#include "encoder.h"

EC11_t ec11_1;
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
        nvic_irq_enable(ENCODER_DT_EXTI_IRQn, 1U, 1U);
        nvic_irq_enable(ENCODER_SW_EXTI_IRQn, 1U, 1U);

        /* connect key EXTI line to key GPIO pin */
        syscfg_exti_line_config(ENCODER_CLK_EXTI_PORT_SOURCE, ENCODER_CLK_EXTI_PIN_SOURCE);
        syscfg_exti_line_config(ENCODER_DT_EXTI_PORT_SOURCE, ENCODER_DT_EXTI_PIN_SOURCE);
        syscfg_exti_line_config(ENCODER_SW_EXTI_PORT_SOURCE, ENCODER_SW_EXTI_PIN_SOURCE);

        /* configure key EXTI line */
        exti_init(ENCODER_CLK_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
        exti_interrupt_flag_clear(ENCODER_CLK_EXTI_LINE);
        exti_init(ENCODER_DT_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(ENCODER_DT_EXTI_LINE);
        exti_init(ENCODER_SW_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(ENCODER_SW_EXTI_LINE);
    }

    ec11_init(ec11_1);
}

/*******************************************************************************
 * @brief 旋转编码器处理函数
 ******************************************************************************/
void encoder_handle(void)
{
    FlagStatus SW_State = gpio_input_bit_get(ENCODER_SW_GPIO_PORT, ENCODER_SW_PIN);
    if(ec11_1.sw_down_flag == 1 && ec11_1.sw_down_time > 10 && ec11_1.sw_down_count == 0)
    {
        if(SW_State == RESET)
        {
            ec11_1.sw_down_count = 1;
        }
    }

    /* 按键长按 */
    if(ec11_1.sw_down_count == 1 && ec11_1.sw_down_time > 500 && SW_State == RESET)
    {
        ec11_1.sw_mode_flag = 2;    // 长按
    }
    if(ec11_1.sw_down_count == 1 && ec11_1.sw_down_time > 500 && SW_State == SET)
    {
        ec11_1.sw_down_count = 0;   // 长按需要把 sw_down_count 清零
        ec11_1.sw_down_time = 0;    // 长按需要 sw_down_time 清零
        ec11_1.sw_down_flag = 0;    // 清除 sw_down_flag 标志
        ec11_1.sw_mode_flag = 0;    // 模式清除
    }

    /* 单击双击判断 */
    if(ec11_1.sw_down_count == 1 && SW_State == SET && ec11_1.sw_down_time < 500)
    {
        ec11_1.sw_down_count = 2; // sw_down_time < 500 需要判断双击还是单击
        ec11_1.sw_down_time = 0;
    }

    // if(SW_State == SET && ec11_1.sw_down_count == 2 && ec11_1.sw_down_time > 100)
    // {
    //     ec11_1.sw_mode_flag = 1;    // 单击
    //     ec11_1.sw_down_time = 0;
    //     ec11_1.sw_down_flag = 0;
    //     ec11_1.sw_down_count = 0;
    // }

    if(ec11_1.sw_down_flag == 1 && ec11_1.sw_down_count == 2 && ec11_1.sw_down_time > 100)
    {
        if(SW_State == RESET)
        {
            ec11_1.sw_mode_flag = 3;    // 双击
            ec11_1.sw_down_time = 0;
            ec11_1.sw_down_flag = 0;
            ec11_1.sw_down_count = 0;
        } else {
            ec11_1.sw_mode_flag = 1;    // 单击
            ec11_1.sw_down_time = 0;
            ec11_1.sw_down_flag = 0;
            ec11_1.sw_down_count = 0;
        }
    }

    // 长按
    if(ec11_1.sw_mode_flag == 2)
    {
        test_number++;
        ec11_1.sw_mode_flag = 0;
    }
    // 单击
    if(ec11_1.sw_mode_flag == 1)
    {
        test_number--;
        ec11_1.sw_mode_flag = 0;
    }

    // 双击
    if(ec11_1.sw_mode_flag == 3)
    {
        test_number = 0;
        ec11_1.sw_mode_flag = 0;
    } 
}

/*******************************************************************************
 * @brief EC11 结构体初始化
 * @brief sw_mode_flag: 
 ******************************************************************************/
void ec11_init(EC11_t ec11)
{
    ec11.sw_down_time = 0;
    ec11.sw_mode_flag = 0;
    ec11.sw_down_flag = 0;
    ec11.sw_down_count = 0;
}

__STATIC_INLINE void ec11_struct_reset(EC11_t ec11)
{
    ec11.sw_down_flag = 0;
    ec11.sw_down_time = 0;
    ec11_1.sw_down_count = 0;
    ec11.sw_mode_flag = 0;
}
