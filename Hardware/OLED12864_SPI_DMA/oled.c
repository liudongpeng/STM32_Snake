/**
  ******************************************************************************
  * @file    oled.c
  * @author  liudongpeng
  * @version V0.1
  * @date    2022/7/28
  * @brief   4线/3线(CS引脚接地)0.96寸oled12864屏幕驱动
  ******************************************************************************
  * @attention
  * SCL D0 时钟
  * SDA D1 数据
  * CS	   低电平选中(默认接地)
  ******************************************************************************
  */

#include <string.h>
#include "oled.h"
#include "oled_font.h"
#include "spi.h"


/**
 * @brief oled12864显存, 仅在本文件中使用, 不对外公开
 * @note 缓存分8页, 从上到下编号为0~7, 每页128字节
 *		[0] : [0][1][2][3] ... [127]
 * 		[1] : [0][1][2][3] ... [127]
 * 		:
 * 		[7] : [0][1][2][3] ... [127]
 */
uint8_t sOledCache[FRAME_SIZE];

/* 只用于清屏的缓存 */
static uint8_t sOledClearCache[FRAME_SIZE] = {0};


/**
 * @brief 置RES引脚为高电平
 */
static inline void OLED_RES_SET()
{
	HAL_GPIO_WritePin(RES_GPIO_PORT, RES_GPIO_PIN, GPIO_PIN_SET);
}

/**
 * @brief 置RES引脚为低电平
 */
static inline void OLED_RES_RESET()
{
	HAL_GPIO_WritePin(RES_GPIO_PORT, RES_GPIO_PIN, GPIO_PIN_RESET);
}

/**
 * @brief 置DC引脚为高电平
 */
static inline void OLED_DC_SET()
{
	HAL_GPIO_WritePin(DC_GPIO_PORT, DC_GPIO_PIN, GPIO_PIN_SET);
}

/**
 * @brief 置DC引脚为低电平
 */
static inline void OLED_DC_RESET()
{
	HAL_GPIO_WritePin(DC_GPIO_PORT, DC_GPIO_PIN, GPIO_PIN_RESET);
}

/**
 * @brief 置CS引脚为高电平
 */
static inline void OLED_CS_SET()
{
	HAL_GPIO_WritePin(CS_GPIO_PORT, CS_GPIO_PIN, GPIO_PIN_SET);
}

/**
 * @brief 置CS引脚为低电平
 */
static inline void OLED_CS_RESET()
{
	HAL_GPIO_WritePin(CS_GPIO_PORT, CS_GPIO_PIN, GPIO_PIN_RESET);
}


/**
 * @brief 测试函数，用于点亮全部的128x64个格子
 */
void OledFull()
{
	memset(sOledCache, 0xFF, FRAME_SIZE);
	OledSyncCache();
}

/**
 * @brief 写字节函数
 * @param byte 要写入SSD1306寄存器的字节
 */
static inline void OledWriteByte(uint8_t byte)
{
	HAL_SPI_Transmit(&SPI_INTERFACE, &byte, 1, 10);
}

/**
 * @brief 写命令
 * @param cmd 要写入的控制命令
 */
static inline void OledWriteCmd(uint8_t cmd)
{
	OLED_DC_RESET();
	OledWriteByte(cmd);
}

/**
 * @brief 写数据
 * @param data 要写入的数据
 */
static inline void OledWriteData(uint8_t data)
{
	OLED_DC_SET();
	OledWriteByte(data);
}


/**
 * @brief oled引脚初始化
 */
static void OledGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	RES_RCC_ENABLE();
	DC_RCC_ENABLE();
	CS_RCC_ENABLE();

	/* 初始化RES和DC引脚 */
	GPIO_InitStruct.Pin = OLED_RES_Pin | OLED_DC_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(OLED_RES_GPIO_Port, &GPIO_InitStruct);

	OLED_RES_SET();
}

/**
 * @brief 设置光标位置
 * @param row 行
 * @param col 列
 */
static void OledSetCursor(uint32_t row, uint32_t col)
{
	OledWriteCmd(0xB0 | row);                   // 设置Y位置
	OledWriteCmd(0x10 | ((col & 0xF0) >> 4));   // 设置X位置低4位
	OledWriteCmd(0x00 | (col & 0x0F));          // 设置X位置高4位
}

/**
 * @brief oled初始化
 */
void OledInit()
{
	/* 初始化oled引脚 */
	OledGpioInit();

	/* 延时等待oled稳定 */
	HAL_Delay(500);

	/* 初始化oled寄存器 */
	OledWriteCmd(0xAE);    // 关闭显示

	OledWriteCmd(0xD5);    // 设置显示时钟分频比/振荡器频率
	OledWriteCmd(0x80);

	OledWriteCmd(0xA8);    // 设置多路复用率
	OledWriteCmd(0x3F);

	OledWriteCmd(0xD3);    // 设置显示偏移
	OledWriteCmd(0x00);

	OledWriteCmd(0x40);    // 设置显示开始行

	OledWriteCmd(0xA1);    // 设置左右方向，0xA1正常 0xA0左右反置

	OledWriteCmd(0xC8);    // 设置上下方向，0xC8正常 0xC0上下反置

	OledWriteCmd(0xDA);    // 设置COM引脚硬件配置
	OledWriteCmd(0x12);


	OledWriteCmd(0x20);    // 设置寻址模式
	OledWriteCmd(0x00);


	OledWriteCmd(0x81);    // 设置对比度控制
	OledWriteCmd(0xCF);

	OledWriteCmd(0xD9);    // 设置预充电周期
	OledWriteCmd(0xF1);

	OledWriteCmd(0xDB);    // 设置VCOMH取消选择级别
	OledWriteCmd(0x30);

	OledWriteCmd(0xA4);    // 设置整个显示打开/关闭

	OledWriteCmd(0xA6);    // 设置正常/倒转显示

	OledWriteCmd(0x8D);    // 设置充电泵
	OledWriteCmd(0x14);

	OledWriteCmd(0xAF);    // 开启显示

	OledClear();            // oled清屏
}

/**
 * @brief 同步缓存中的数据
 */
void OledSyncCache()
{
	OledSetCursor(0, 0);

	OLED_DC_SET();

#if (USE_SPITx_DMA)
    HAL_SPI_Transmit_DMA(&SPI_INTERFACE, sOledCache, FRAME_SIZE);
	while (__HAL_DMA_GET_FLAG(&hdma_spi2_tx, DMA_FLAG_TC5) == RESET)
	{
	}
	__HAL_DMA_CLEAR_FLAG((&hdma_spi2_tx), DMA_FLAG_TC5);
#else
	HAL_SPI_Transmit(&SPI_INTERFACE, sOledCache, FRAME_SIZE, 1000);
#endif
}

/**
 * @brief 清空oled的缓存
 */
void OledClearCache()
{
	memset(sOledCache, 0, FRAME_SIZE);
}

/**
 * @brief oled清屏
 */
void OledClear()
{
	OledClearCache();

	OledSetCursor(0, 0);

#if (USE_SPITx_DMA)
	HAL_SPI_Transmit_DMA(&SPI_INTERFACE, sOledCache, FRAME_SIZE);
#else
	HAL_SPI_Transmit(&SPI_INTERFACE, sOledCache, FRAME_SIZE, 1000);
#endif
}

/**
 * @brief 在指定位置显示一个6x8大小的ASCII字符
 * @param row 行 0~63
 * @param col 列 0~127
 * @param ch 要显示的ASCII字符
 */
void OledPutchar6x8(uint32_t row, uint32_t col, char ch)
{
	uint32_t page;

	page = row / 8 % 8;

	OledSetCursor(row, col);

	for (int i = 0; i < 6; i++)
	{
		sOledCache[(page * 128) + (col + i)] |= Font6x8[ch - 32][i];
	}

	OledSyncCache();
}

/**
 * @brief 在指定位置显示6x8大小的ASCII字符串
 * @param row 行 0~63
 * @param col 列 0~127
 * @param ch 要显示的ASCII字符串
 */
void OledPuts6x8(uint32_t row, uint32_t col, const char *str)
{
	if (str == NULL)
		return;

	while (*str)
	{
		OledPutchar6x8(row, col, *str++);
		col += 6;
	}
}

uint32_t OledPow(uint32_t x, uint32_t y)
{
	uint32_t result = 1;

	while (y--)
	{
		result *= x;
	}
	return result;
}

/**
 * @brief 在指定位置显示6x8大小的数字
 * @param row 行，1~8
 * @param col 列，1~21
 * @param ch 要显示的数字
 */
void OledPutnum6x8(uint32_t row, uint32_t col, int num, int len)
{
	col *= 6;
	for (int i = 0; i < len; i++)
	{
		OledPutchar6x8(row, col, num / OledPow(10, len - i - 1) % 10 + '0');
		col += 6;
	}
}

void OledPutbmp128x64(uint8_t *bmp)
{
	uint32_t index = 0;

    /*
	for (int i = 0; i < 8; i++)
	{
		OledSetCursor(i, 0);
		for (int j = 0; j < 128; j++)
		{
			//oled_write_data(bmp[index++]);
            gs_oled_cache[i][j] = bmp[index++];
		}
	}
     */
    HAL_SPI_Transmit_DMA(&SPI_INTERFACE, bmp, 1024);
}

/**
 * @brief 绘制一个点
 * @param row 行 0~63
 * @param col 列 0~127
 * @param color 颜色 1白 0黑
 */
void OledDrawPoint(uint32_t row, uint32_t col, uint8_t color)
{
	uint32_t page, offset;
	uint8_t cur, tar;

	page = row / 8 % 8;
	offset = row % 8;

	cur = sOledCache[(page << 7) + col];
	tar = (0x01 << offset) | cur;

	sOledCache[(page << 7) + col] = tar;
}

/**
 * @brief 绘制直线
 * @param startRow 起点行坐标 0~63
 * @param startCol 起点列坐标 0~127
 * @param endRow 终点行坐标 0~63
 * @param endCol 终点列坐标 0~127
 */
void OledDrwaLine(uint32_t startRow, uint32_t startCol, uint32_t endRow, uint32_t endCol)
{
	/* x轴直线 */
	if (endRow == startRow)
	{
		while (startCol <= endCol)
		{
			OledDrawPoint(startRow, startCol++, 1);
		}
	}

	/* y轴直线 */
	if (endCol == startCol)
	{
		while (startRow <= endRow)
		{
			OledDrawPoint(startRow++, startCol, 1);
		}
	}

	/* 斜线 */

    //OledSyncCache();
}

/**
 * @brief 绘制矩形
 * @param row 行 0~63
 * @param col 列 0~127
 * @param length 矩形的长
 * @param width 矩形的宽
 */
void OledDrawRectangle(uint32_t row, uint32_t col, uint32_t length, uint32_t width)
{
	/* 两条横边 */
	OledDrwaLine(row, col, row, col + length - 1);
	OledDrwaLine(row + width - 1, col, row + width - 1, col + length - 1);

	/* 两条竖边 */
	OledDrwaLine(row, col, row + width - 1, col);
	OledDrwaLine(row, col + length - 1, row + width - 1, col + length - 1);

    //OledSyncCache();
}
