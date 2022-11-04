//
// Created by liudongpeng on 2022/11/2.
//

#include "snake.h"
#include "oled.h"

/**
 * @brief 绘制显示界面的边框
 */
void DrawBorder()
{
	/* 由外到内绘制5个矩形, 显示界面的边框 */
	for (int i = 0; i < BORDER_WIDTH; i++)
		OledDrawRectangle(i, i, 128 - i * 2, 64 - i * 2);

	/*OledDrawRectangle(0, 0, 128, 64);
	OledDrawRectangle(1, 1, 126, 62);
	OledDrawRectangle(2, 2, 124, 60);
	OledDrawRectangle(3, 3, 122, 58);
	OledDrawRectangle(4, 4, 120, 56);*/
}

/**
 * @brief 清空边框内的部分
 */
void ClearMap()
{
	OledClearCache();

	DrawBorder();
}

/**
 * @brief 3x3大小的蛇头
 * @param row 行 0~63
 * @param col 列 0~127
 */
void DrawHead(uint32_t row, uint32_t col)
{
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			OledDrawPoint(row + i, col + j, 1);
		}
	}
}

/**
 * @brief 创建一个2x2大小的食物
 * @param row 行 0~63
 * @param col 列 0~127
 */
void CreateFood(uint32_t row, uint32_t col)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			OledDrawPoint(row + i, col + j, 1);
		}
	}
}

