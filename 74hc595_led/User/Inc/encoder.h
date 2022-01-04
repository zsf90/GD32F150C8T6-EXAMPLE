/*******************************************************************************
 * @file        encoder.c
 * @date        2021.10.21
 * @author      信念D力量
 * @brief       旋转编码器头文件
 * @github      https://github.com/zsf90/GD32F150C8T6-EXAMPLE
 * @bilibili    https://space.bilibili.com/334715750
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
} SW_STATE;

/* EC11 编码器结构定义 */
typedef struct
{
    uint16_t    sw_down_time; /* >100ms 为单击，> 900为长按 */
    uint8_t     sw_mode_flag; /* 单击（1），长按（2），双击（3），未知（0）*/
    uint8_t     sw_down_flag; /* SW 下降沿标志，下降沿触发时为：1，用完后设置为：0 */
    uint8_t     sw_down_count; /* 下降沿计数，用户判断双击还是单击 */
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

void ec11_init(EC11_t ec11);

__STATIC_INLINE void ec11_struct_reset(EC11_t ec11);

#endif /* __ENCODER_H */
