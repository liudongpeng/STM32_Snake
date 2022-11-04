//
// Created by liudongpeng on 2022/11/2.
//

#include "snake.h"
#include "oled.h"

/**
 * @brief ������ʾ����ı߿�
 */
void DrawBorder()
{
	/* ���⵽�ڻ���5������, ��ʾ����ı߿� */
	for (int i = 0; i < BORDER_WIDTH; i++)
		OledDrawRectangle(i, i, 128 - i * 2, 64 - i * 2);

	/*OledDrawRectangle(0, 0, 128, 64);
	OledDrawRectangle(1, 1, 126, 62);
	OledDrawRectangle(2, 2, 124, 60);
	OledDrawRectangle(3, 3, 122, 58);
	OledDrawRectangle(4, 4, 120, 56);*/
}

/**
 * @brief ��ձ߿��ڵĲ���
 */
void ClearMap()
{
	OledClearCache();

	DrawBorder();
}

/**
 * @brief 3x3��С����ͷ
 * @param row �� 0~63
 * @param col �� 0~127
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
 * @brief ����һ��2x2��С��ʳ��
 * @param row �� 0~63
 * @param col �� 0~127
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

