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

typedef enum
{
    ENCODER_MODE_GPIO = 0,
    ENCODER_MODE_EXIT
} encoder_mode_enum;

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

#endif /* __ENCODER_H */
