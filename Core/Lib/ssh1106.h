/*
 * ssd1306.h
 *
 *  Created on: Sep 11, 2025
 *      Author: USER
 */

#pragma once
#include <stdint.h>
#include "i2c.h"

#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH   128
#endif
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT   64
#endif

// 필요 시 0x3D로 바꿔 테스트
#ifndef SSD1306_I2C_ADDR
#define SSD1306_I2C_ADDR (0x3C<<1)
#endif

// 공개 API
void SSD1306_BindI2C(I2C_HandleTypeDef *hi2c); // hi2c1 바인딩
int  SSD1306_Init(void);
void SSD1306_Update(void);

// 프레임버퍼 유틸
void SSD1306_Clear(void);
void SSD1306_Pixel(int x, int y, int on);
void SSD1306_HLine(int x,int y,int w);
void SSD1306_VLine(int x,int y,int h);

// 텍스트(폰트 모듈 사용)
#include "fonts.h"
int  SSD1306_DrawChar(int x,int y,char ch,const FontDef *font);
int  SSD1306_DrawText(int x,int y,const char *s,const FontDef *font);

// 작은 ° 기호
void SSD1306_DrawDegree5x5(int x,int y);
