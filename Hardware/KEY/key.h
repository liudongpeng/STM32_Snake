//
// Created by liudongpeng on 2022/9/15.
//

#ifndef HAL_KEY_H
#define HAL_KEY_H


#include "stm32f1xx_hal.h"


#define KEY1    0

#define KEY1_RCC_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define KEY1_PORT            GPIOB
#define KEY1_PIN            GPIO_PIN_9


/**
 * @brief 四个方向键对应的管脚
 */

#define KEY_UP_RCC_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY_UP          1
#define KEY_UP_PORT     GPIOA
#define KEY_UP_PIN      GPIO_PIN_3

#define KEY_DOWN_RCC_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY_DOWN        2
#define KEY_DOWN_PORT   GPIOA
#define KEY_DOWN_PIN    GPIO_PIN_4

#define KEY_LEFT_RCC_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY_LEFT        3
#define KEY_LEFT_PORT   GPIOA
#define KEY_LEFT_PIN    GPIO_PIN_5

#define KEY_RIGHT_RCC_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY_RIGHT       4
#define KEY_RIGHT_PORT  GPIOA
#define KEY_RIGHT_PIN   GPIO_PIN_6


/**
 * @brief 按键初始化
 */
void KeyInit();

/**
 * @brief 按键扫描，查询是那个按键被按下
 * @retval 返回按下的按键
 */
int8_t KeyScan();


#endif // HAL_KEY_H
