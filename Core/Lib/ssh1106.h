/*
 * ssh1106.h
 *
 *  Created on: Sep 11, 2025
 *      Author: USER
 */

#pragma once
#include <stdint.h>
#include "i2c.h"

#ifndef ssh1106_WIDTH
#define ssh1106_WIDTH   128
#endif
#ifndef ssh1106_HEIGHT
#define ssh1106_HEIGHT   64
#endif

// 필요 시 0x3D로 바꿔 테스트
#ifndef ssh1106_I2C_ADDR
#define ssh1106_I2C_ADDR (0x3C<<1)
#endif

// 공개 API
void ssh1106_BindI2C(I2C_HandleTypeDef *hi2c); // hi2c1 바인딩
int  ssh1106_Init(void);
void ssh1106_Update(void);

// 프레임버퍼 유틸
void ssh1106_Clear(void);
void ssh1106_Pixel(int x, int y, int on);
void ssh1106_HLine(int x,int y,int w);
void ssh1106_VLine(int x,int y,int h);

// 텍스트(폰트 모듈 사용)
#include "fonts.h"
int  ssh1106_DrawChar(int x,int y,char ch,const FontDef *font);
int  ssh1106_DrawText(int x,int y,const char *s,const FontDef *font);

// 작은 ° 기호
void ssh1106_DrawDegree5x5(int x,int y);
