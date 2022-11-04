/**
  ******************************************************************************
  * @file    oled.h
  * @author  liudongpeng
  * @version V0.1
  * @date    2022/7/28
  * @brief   4线硬件SPI 0.96寸oled12864屏幕驱动.
  ******************************************************************************
  * @attention
  * SCL D0 时钟
  * SDA D1 数据
  * CS	   低电平选中
  ******************************************************************************
  */

#ifndef HAL_SPI_OLED_H
#define HAL_SPI_OLED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


/* RES引脚配置 */
#define RES_RCC_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define RES_GPIO_PORT       GPIOC
#define RES_GPIO_PIN        GPIO_PIN_13

/* DC引脚配置 */
#define DC_RCC_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define DC_GPIO_PORT        GPIOC
#define DC_GPIO_PIN         GPIO_PIN_14

/* CS引脚配置 */
#define CS_RCC_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define CS_GPIO_PORT        GPIOC
#define CS_GPIO_PIN         GPIO_PIN_15


/* 宏定义是否使用SPI的DMA */
#define USE_SPITx_DMA   0

/* SPI接口选择 */
#define SPI_INTERFACE   hspi2

/* 每帧图像的字节大小 */
#define FRAME_SIZE  1024

/* 屏幕的长宽(像素) */
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64


/* oled显存 */
extern uint8_t sOledCache[FRAME_SIZE];


/* 测试函数，点亮全部的128x64个格子 */
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
 * @brief 绘制一个点
 * @param row 行 0~63
 * @param col 列 0~127
 * @param color 颜色 1白 0黑
 */
void OledDrawPoint(uint32_t row, uint32_t col, uint8_t color);

/**
 * @brief 绘制直线
 * @param startRow 起点行坐标 0~63
 * @param startCol 起点列坐标 0~127
 * @param endRow 终点行坐标 0~63
 * @param endCol 终点列坐标 0~127
 */
void OledDrwaLine(uint32_t startRow, uint32_t startCol, uint32_t endRow, uint32_t endCol);

/**
 * @brief 绘制矩形
 * @param row 行 0~63
 * @param col 列 0~127
 * @param length 矩形的长
 * @param width 矩形的宽
 */
void OledDrawRectangle(uint32_t row, uint32_t col, uint32_t length, uint32_t width);


#ifdef __cplusplus
}
#endif

#endif //HAL_SPI_OLED_H
