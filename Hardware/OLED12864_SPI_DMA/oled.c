/**
  ******************************************************************************
  * @file    oled.c
  * @author  liudongpeng
  * @version V0.1
  * @date    2022/7/28
  * @brief   4��/3��(CS���Žӵ�)0.96��oled12864��Ļ����
  ******************************************************************************
  * @attention
  * SCL D0 ʱ��
  * SDA D1 ����
  * CS	   �͵�ƽѡ��(Ĭ�Ͻӵ�)
  ******************************************************************************
  */

#include <string.h>
#include "oled.h"
#include "oled_font.h"
#include "spi.h"


/**
 * @brief oled12864�Դ�, ���ڱ��ļ���ʹ��, �����⹫��
 * @note �����8ҳ, ���ϵ��±��Ϊ0~7, ÿҳ128�ֽ�
 *		[0] : [0][1][2][3] ... [127]
 * 		[1] : [0][1][2][3] ... [127]
 * 		:
 * 		[7] : [0][1][2][3] ... [127]
 */
uint8_t sOledCache[FRAME_SIZE];

/* ֻ���������Ļ��� */
static uint8_t sOledClearCache[FRAME_SIZE] = {0};


/**
 * @brief ��RES����Ϊ�ߵ�ƽ
 */
static inline void OLED_RES_SET()
{
	HAL_GPIO_WritePin(RES_GPIO_PORT, RES_GPIO_PIN, GPIO_PIN_SET);
}

/**
 * @brief ��RES����Ϊ�͵�ƽ
 */
static inline void OLED_RES_RESET()
{
	HAL_GPIO_WritePin(RES_GPIO_PORT, RES_GPIO_PIN, GPIO_PIN_RESET);
}

/**
 * @brief ��DC����Ϊ�ߵ�ƽ
 */
static inline void OLED_DC_SET()
{
	HAL_GPIO_WritePin(DC_GPIO_PORT, DC_GPIO_PIN, GPIO_PIN_SET);
}

/**
 * @brief ��DC����Ϊ�͵�ƽ
 */
static inline void OLED_DC_RESET()
{
	HAL_GPIO_WritePin(DC_GPIO_PORT, DC_GPIO_PIN, GPIO_PIN_RESET);
}

/**
 * @brief ��CS����Ϊ�ߵ�ƽ
 */
static inline void OLED_CS_SET()
{
	HAL_GPIO_WritePin(CS_GPIO_PORT, CS_GPIO_PIN, GPIO_PIN_SET);
}

/**
 * @brief ��CS����Ϊ�͵�ƽ
 */
static inline void OLED_CS_RESET()
{
	HAL_GPIO_WritePin(CS_GPIO_PORT, CS_GPIO_PIN, GPIO_PIN_RESET);
}


/**
 * @brief ���Ժ��������ڵ���ȫ����128x64������
 */
void OledFull()
{
	memset(sOledCache, 0xFF, FRAME_SIZE);
	OledSyncCache();
}

/**
 * @brief д�ֽں���
 * @param byte Ҫд��SSD1306�Ĵ������ֽ�
 */
static inline void OledWriteByte(uint8_t byte)
{
	HAL_SPI_Transmit(&SPI_INTERFACE, &byte, 1, 10);
}

/**
 * @brief д����
 * @param cmd Ҫд��Ŀ�������
 */
static inline void OledWriteCmd(uint8_t cmd)
{
	OLED_DC_RESET();
	OledWriteByte(cmd);
}

/**
 * @brief д����
 * @param data Ҫд�������
 */
static inline void OledWriteData(uint8_t data)
{
	OLED_DC_SET();
	OledWriteByte(data);
}


/**
 * @brief oled���ų�ʼ��
 */
static void OledGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	RES_RCC_ENABLE();
	DC_RCC_ENABLE();
	CS_RCC_ENABLE();

	/* ��ʼ��RES��DC���� */
	GPIO_InitStruct.Pin = OLED_RES_Pin | OLED_DC_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(OLED_RES_GPIO_Port, &GPIO_InitStruct);

	OLED_RES_SET();
}

/**
 * @brief ���ù��λ��
 * @param row ��
 * @param col ��
 */
static void OledSetCursor(uint32_t row, uint32_t col)
{
	OledWriteCmd(0xB0 | row);                   // ����Yλ��
	OledWriteCmd(0x10 | ((col & 0xF0) >> 4));   // ����Xλ�õ�4λ
	OledWriteCmd(0x00 | (col & 0x0F));          // ����Xλ�ø�4λ
}

/**
 * @brief oled��ʼ��
 */
void OledInit()
{
	/* ��ʼ��oled���� */
	OledGpioInit();

	/* ��ʱ�ȴ�oled�ȶ� */
	HAL_Delay(500);

	/* ��ʼ��oled�Ĵ��� */
	OledWriteCmd(0xAE);    // �ر���ʾ

	OledWriteCmd(0xD5);    // ������ʾʱ�ӷ�Ƶ��/����Ƶ��
	OledWriteCmd(0x80);

	OledWriteCmd(0xA8);    // ���ö�·������
	OledWriteCmd(0x3F);

	OledWriteCmd(0xD3);    // ������ʾƫ��
	OledWriteCmd(0x00);

	OledWriteCmd(0x40);    // ������ʾ��ʼ��

	OledWriteCmd(0xA1);    // �������ҷ���0xA1���� 0xA0���ҷ���

	OledWriteCmd(0xC8);    // �������·���0xC8���� 0xC0���·���

	OledWriteCmd(0xDA);    // ����COM����Ӳ������
	OledWriteCmd(0x12);


	OledWriteCmd(0x20);    // ����Ѱַģʽ
	OledWriteCmd(0x00);


	OledWriteCmd(0x81);    // ���öԱȶȿ���
	OledWriteCmd(0xCF);

	OledWriteCmd(0xD9);    // ����Ԥ�������
	OledWriteCmd(0xF1);

	OledWriteCmd(0xDB);    // ����VCOMHȡ��ѡ�񼶱�
	OledWriteCmd(0x30);

	OledWriteCmd(0xA4);    // ����������ʾ��/�ر�

	OledWriteCmd(0xA6);    // ��������/��ת��ʾ

	OledWriteCmd(0x8D);    // ���ó���
	OledWriteCmd(0x14);

	OledWriteCmd(0xAF);    // ������ʾ

	OledClear();            // oled����
}

/**
 * @brief ͬ�������е�����
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
 * @brief ���oled�Ļ���
 */
void OledClearCache()
{
	memset(sOledCache, 0, FRAME_SIZE);
}

/**
 * @brief oled����
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
 * @brief ��ָ��λ����ʾһ��6x8��С��ASCII�ַ�
 * @param row �� 0~63
 * @param col �� 0~127
 * @param ch Ҫ��ʾ��ASCII�ַ�
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
 * @brief ��ָ��λ����ʾ6x8��С��ASCII�ַ���
 * @param row �� 0~63
 * @param col �� 0~127
 * @param ch Ҫ��ʾ��ASCII�ַ���
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
 * @brief ��ָ��λ����ʾ6x8��С������
 * @param row �У�1~8
 * @param col �У�1~21
 * @param ch Ҫ��ʾ������
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
 * @brief ����һ����
 * @param row �� 0~63
 * @param col �� 0~127
 * @param color ��ɫ 1�� 0��
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
 * @brief ����ֱ��
 * @param startRow ��������� 0~63
 * @param startCol ��������� 0~127
 * @param endRow �յ������� 0~63
 * @param endCol �յ������� 0~127
 */
void OledDrwaLine(uint32_t startRow, uint32_t startCol, uint32_t endRow, uint32_t endCol)
{
	/* x��ֱ�� */
	if (endRow == startRow)
	{
		while (startCol <= endCol)
		{
			OledDrawPoint(startRow, startCol++, 1);
		}
	}

	/* y��ֱ�� */
	if (endCol == startCol)
	{
		while (startRow <= endRow)
		{
			OledDrawPoint(startRow++, startCol, 1);
		}
	}

	/* б�� */

    //OledSyncCache();
}

/**
 * @brief ���ƾ���
 * @param row �� 0~63
 * @param col �� 0~127
 * @param length ���εĳ�
 * @param width ���εĿ�
 */
void OledDrawRectangle(uint32_t row, uint32_t col, uint32_t length, uint32_t width)
{
	/* ������� */
	OledDrwaLine(row, col, row, col + length - 1);
	OledDrwaLine(row + width - 1, col, row + width - 1, col + length - 1);

	/* �������� */
	OledDrwaLine(row, col, row + width - 1, col);
	OledDrwaLine(row, col + length - 1, row + width - 1, col + length - 1);

    //OledSyncCache();
}
