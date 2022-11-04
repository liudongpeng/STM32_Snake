//
// Created by liudongpeng on 2022/11/2.
//

#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

#include <stdint.h>


/**
 * @brief ��ʾ����߿���, Ŀǰ�趨Ϊ5
 */
#define BORDER_WIDTH    5

/**
 * @brief ÿ���ߵĴ�С, Ŀǰ�趨Ϊ3, ��������ĵ㶨λ
 */
#define SNAKE_SIZE  3

/**
 * @brief ʳ��Ĵ�С, Ŀǰ�趨Ϊ2
 */
#define FOOD_SIZE   2

/**
 * @brief �ߵ��ƶ��ٶ�
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
