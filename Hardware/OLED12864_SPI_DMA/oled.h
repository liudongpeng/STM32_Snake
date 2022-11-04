/**
  ******************************************************************************
  * @file    oled.h
  * @author  liudongpeng
  * @version V0.1
  * @date    2022/7/28
  * @brief   4��Ӳ��SPI 0.96��oled12864��Ļ����.
  ******************************************************************************
  * @attention
  * SCL D0 ʱ��
  * SDA D1 ����
  * CS	   �͵�ƽѡ��
  ******************************************************************************
  */

#ifndef HAL_SPI_OLED_H
#define HAL_SPI_OLED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


/* RES�������� */
#define RES_RCC_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define RES_GPIO_PORT       GPIOC
#define RES_GPIO_PIN        GPIO_PIN_13

/* DC�������� */
#define DC_RCC_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define DC_GPIO_PORT        GPIOC
#define DC_GPIO_PIN         GPIO_PIN_14

/* CS�������� */
#define CS_RCC_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define CS_GPIO_PORT        GPIOC
#define CS_GPIO_PIN         GPIO_PIN_15


/* �궨���Ƿ�ʹ��SPI��DMA */
#define USE_SPITx_DMA   0

/* SPI�ӿ�ѡ�� */
#define SPI_INTERFACE   hspi2

/* ÿ֡ͼ����ֽڴ�С */
#define FRAME_SIZE  1024

/* ��Ļ�ĳ���(����) */
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64


/* oled�Դ� */
extern uint8_t sOledCache[FRAME_SIZE];


/* ���Ժ���������ȫ����128x64������ */
void OledFull();

void OledInit();

void OledSyncCache();

void OledClear();

void OledClearCache();

void OledPutchar6x8(uint32_t row, uint32_t col, char ch);

void OledPuts6x8(uint32_t row, uint32_t col, const char *str);

void OledPutnum6x8(uint32_t row, uint32_t col, int num, int len);

void OledPutbmp128x64(uint8_t *bmp);

/**
 * @brief ����һ����
 * @param row �� 0~63
 * @param col �� 0~127
 * @param color ��ɫ 1�� 0��
 */
void OledDrawPoint(uint32_t row, uint32_t col, uint8_t color);

/**
 * @brief ����ֱ��
 * @param startRow ��������� 0~63
 * @param startCol ��������� 0~127
 * @param endRow �յ������� 0~63
 * @param endCol �յ������� 0~127
 */
void OledDrwaLine(uint32_t startRow, uint32_t startCol, uint32_t endRow, uint32_t endCol);

/**
 * @brief ���ƾ���
 * @param row �� 0~63
 * @param col �� 0~127
 * @param length ���εĳ�
 * @param width ���εĿ�
 */
void OledDrawRectangle(uint32_t row, uint32_t col, uint32_t length, uint32_t width);


#ifdef __cplusplus
}
#endif

#endif //HAL_SPI_OLED_H
