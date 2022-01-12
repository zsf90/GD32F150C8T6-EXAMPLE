/*******************************************************************************
 * @file        encoder.c
 * @date        2021.10.21
 * @author      信念D力量
 * @brief       旋转编码器头文件
 * @github      https://github.com/zsf90/GD32F150C8T6-EXAMPLE
 * @bilibili    https://space.bilibili.com/334715750
 * 
 * Docs
 * 旋转状态有：顺时针、逆时针、按下并顺时针、按下并逆时针
 * 按钮状态有：单击、双击、长按（长按和选中的按下并旋转冲突，可以通过宏定义选择
 ******************************************************************************/
/*******************************************************************************
 * CLK -> PA0
 * DT -> PA1
 * SW -> PA2
 ******************************************************************************/
#ifndef __ENCODER_H
#define __ENCODER_H

#include "gd32f1x0.h"

#define LONG_PRESS_VALUE    (1000)
#define LED_AUTO_SETP       (50) // 毫秒
#define ENABLE      (1)
#define DISABLE     (0)
#define ENABLE_LONG_PRESS   DISABLE // 是否启用按钮双击
#define clk_value   gpio_input_bit_get(ENCODER_CLK_GPIO_PORT, ENCODER_CLK_PIN)
#define dt_value    gpio_input_bit_get(ENCODER_DT_GPIO_PORT, ENCODER_DT_PIN)
#define sw_value    gpio_input_bit_get(ENCODER_SW_GPIO_PORT, ENCODER_SW_PIN)

typedef enum
{
    ENCODER_MODE_GPIO = 0,
    ENCODER_MODE_EXIT
} encoder_mode_enum;


/* 按钮状态标志 */
typedef enum
{
    SW_DEFAULT = 0, // 默认
    SW_CLICK,       // 单击
    SW_LONG_PRESS,  // 长按
    SW_DOUBLE_CLICK // 双击
} ec11_switch_state;

typedef enum
{
    EC11_NONE_W = 0,   // 没有旋转方向
    EC11_CW,            // 顺时针
    EC11_CCW,           // 逆时针
    EC11_DOWN_CW,       // 按钮按下并顺时针旋转
    EC11_DOWN_CCW,      // 按钮按下并逆时针旋转
} ec11_directron;

typedef enum
{
    EC11_NONE = 0,
    EC11_CW, // 顺时针旋转
    EC11_CCW, // 逆时针旋转
    EC11_DOWN_CW, // 按下按钮并顺时针旋转
    EC11_DOWN_CCW, // 按下按钮并逆时针旋转
    EC11_CLICK, // 单击中键
    EC11_LONG_PRESS, // 长按中键
    EC11_DOUBLE_CLICK, // 双击中键
} ec11_return_result;

/* EC11 编码器结构定义 */
typedef struct
{
    uint16_t            sw_down_time; /* >100ms 为单击，> 900为长按 */
    ec11_switch_state   sw_state; /* 单击（1），长按（2），双击（3），未知（0）*/
    uint8_t             sw_down_flag; /* SW 下降沿标志，下降沿触发时为：1，用完后在适当的时候设置为：0 */
    uint8_t             sw_down_count; /* 下降沿计数，用户判断状态的临时计数值 */
    uint16_t            sw_long_press_time;  /* sw 长按时间，用于判断长按时多长时间执行一次相应代码 */

    uint8_t             clk_flag; /* 旋转标志 */
    ec11_directron      direction; /* 0：默认 1: 顺时针 2：逆时针  */
    uint16_t            clk_count;
} EC11_t;

/* GPIO Define */
/* encoder CLK Pin 脉冲 */
#define ENCODER_CLK_PIN                     GPIO_PIN_0
#define ENCODER_CLK_GPIO_PORT               GPIOA
#define ENCODER_CLK_GPIO_CLK                RCU_GPIOA
#define ENCODER_CLK_EXTI_LINE               EXTI_0
#define ENCODER_CLK_EXTI_PORT_SOURCE        EXTI_SOURCE_GPIOA
#define ENCODER_CLK_EXTI_PIN_SOURCE         EXTI_SOURCE_PIN0
#define ENCODER_CLK_EXTI_IRQn               EXTI0_1_IRQn

/* encoder DT Pin 方向 */
#define ENCODER_DT_PIN                     GPIO_PIN_1
#define ENCODER_DT_GPIO_PORT               GPIOA
#define ENCODER_DT_GPIO_CLK                RCU_GPIOA
#define ENCODER_DT_EXTI_LINE               EXTI_1
#define ENCODER_DT_EXTI_PORT_SOURCE        EXTI_SOURCE_GPIOA
#define ENCODER_DT_EXTI_PIN_SOURCE         EXTI_SOURCE_PIN1
#define ENCODER_DT_EXTI_IRQn               EXTI0_1_IRQn

/* encoder SW Pin 按钮 */
#define ENCODER_SW_PIN                     GPIO_PIN_2
#define ENCODER_SW_GPIO_PORT               GPIOA
#define ENCODER_SW_GPIO_CLK                RCU_GPIOA
#define ENCODER_SW_EXTI_LINE               EXTI_2
#define ENCODER_SW_EXTI_PORT_SOURCE        EXTI_SOURCE_GPIOA
#define ENCODER_SW_EXTI_PIN_SOURCE         EXTI_SOURCE_PIN2
#define ENCODER_SW_EXTI_IRQn               EXTI2_3_IRQn


void encoder_exit_config(encoder_mode_enum _zsf_eme);
void encoder_handle(void);

#endif /* __ENCODER_H */
