//
// Created by liudongpeng on 2022/11/2.
//

#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

#include <stdint.h>


/**
 * @brief 显示界面边框宽度, 目前设定为5
 */
#define BORDER_WIDTH    5

/**
 * @brief 每节蛇的大小, 目前设定为3, 方便从中心点定位
 */
#define SNAKE_SIZE  3

/**
 * @brief 食物的大小, 目前设定为2
 */
#define FOOD_SIZE   2

/**
 * @brief 蛇的移动速度
 */
enum SnakeSpeed
{
	SLOW = 2,
	MED = 4,
	FAST = 6
};



void DrawBorder();

void DrawHead(uint32_t row, uint32_t col);

void ClearMap();

void CreateFood(uint32_t row, uint32_t col);


#endif //SNAKE_SNAKE_H
