//
// Created by liudongpeng on 2022/9/15.
//

#include "key.h"


/**
 * @brief 上下左右四个方向键初始化
 */
void KeyInit()
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	KEY_UP_RCC_ENABLE();
	KEY_DOWN_RCC_ENABLE();
	KEY_LEFT_RCC_ENABLE();
	KEY_RIGHT_RCC_ENABLE();

	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;

	GPIO_InitStructure.Pin = KEY_UP_PIN;
	HAL_GPIO_Init(KEY_UP_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = KEY_DOWN_PIN;
	HAL_GPIO_Init(KEY_DOWN_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = KEY_LEFT_PIN;
	HAL_GPIO_Init(KEY_LEFT_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = KEY_RIGHT_PIN;
	HAL_GPIO_Init(KEY_RIGHT_PORT, &GPIO_InitStructure);
}

/**
 * @brief 按键扫描，查询是那个按键被按下
 * @retval 返回按下的按键
 */
int8_t KeyScan()
{
	int8_t keyNum = -1;

	if (HAL_GPIO_ReadPin(KEY_UP_PORT, KEY_UP_PIN) == 0) /* 上方向键 */
	{
		HAL_Delay(10);
		while (HAL_GPIO_ReadPin(KEY_UP_PORT, KEY_UP_PIN) == 0);
		HAL_Delay(10);
		keyNum = KEY_UP;
	}
	else if (HAL_GPIO_ReadPin(KEY_DOWN_PORT, KEY_DOWN_PIN) == 0) /* 下方向键 */
	{
		HAL_Delay(10);
		while (HAL_GPIO_ReadPin(KEY_DOWN_PORT, KEY_DOWN_PIN) == 0);
		HAL_Delay(10);
		keyNum = KEY_DOWN;
	}
	else if (HAL_GPIO_ReadPin(KEY_LEFT_PORT, KEY_LEFT_PIN) == 0) /* 左方向键 */
	{
		HAL_Delay(10);
		while (HAL_GPIO_ReadPin(KEY_LEFT_PORT, KEY_LEFT_PIN) == 0);
		HAL_Delay(10);
		keyNum = KEY_LEFT;
	}
	else if (HAL_GPIO_ReadPin(KEY_RIGHT_PORT, KEY_RIGHT_PIN) == 0) /* 右方向键 */
	{
		HAL_Delay(10);
		while (HAL_GPIO_ReadPin(KEY_RIGHT_PORT, KEY_RIGHT_PIN) == 0);
		HAL_Delay(10);
		keyNum = KEY_RIGHT;
	}
	else /* 出错 */
	{
		keyNum = -1;
	}

	return keyNum;
}
