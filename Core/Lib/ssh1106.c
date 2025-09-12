/*
 * ssd1306.c
 *
 *  Created on: Sep 11, 2025
 *      Author: USER
 */


#include <ssh1106.h>
#include <string.h>


static I2C_HandleTypeDef *s_i2c;
static uint8_t fb[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

void SSD1306_BindI2C(I2C_HandleTypeDef *hi2c){ s_i2c = hi2c; }

static void cmd(uint8_t c){
  uint8_t b[2]={0x00,c};
  HAL_I2C_Master_Transmit(s_i2c, SSD1306_I2C_ADDR, b, 2, HAL_MAX_DELAY);
}
static void dataN(uint8_t *d, uint16_t n){
  static uint8_t buf[129]; buf[0]=0x40;
  while(n){
    uint16_t m = (n>128?128:n);
    memcpy(&buf[1], d, m);
    HAL_I2C_Master_Transmit(s_i2c, SSD1306_I2C_ADDR, buf, m+1, HAL_MAX_DELAY);
    d+=m; n-=m;
  }
}

int SSD1306_Init(void){
  HAL_Delay(100);
  cmd(0xAE);
  cmd(0x20); cmd(0x00); // Horizontal
  cmd(0xB0);
  cmd(0xC8);
  cmd(0x00); cmd(0x10); // col=0
  cmd(0x40);
  cmd(0x81); cmd(0x7F);
  cmd(0xA1);
  cmd(0xA6);
  cmd(0xA8); cmd(0x3F);
  cmd(0xA4);
  cmd(0xD3); cmd(0x00);
  cmd(0xD5); cmd(0x80);
  cmd(0xD9); cmd(0x22);
  cmd(0xDA); cmd(0x12);
  cmd(0xDB); cmd(0x20);
  cmd(0x8D); cmd(0x14);
  cmd(0xAF);
  SSD1306_Clear();
  SSD1306_Update();
  return 0;
}

//void SSD1306_Update(void){
  //for(uint8_t p=0;p<SSD1306_HEIGHT/8;p++){
    //cmd(0xB0+p); cmd(0x00); cmd(0x10);
    //dataN(&fb[p*SSD1306_WIDTH], SSD1306_WIDTH);}}

#define SH1106_COL_OFFSET  2   // 보통 2. 보드따라 0~4 튜닝
void SSD1306_Update(void){
    for (uint8_t p = 0; p < SSD1306_HEIGHT/8; p++) {
        cmd(0xB0 + p);                               // page
        uint8_t col = SH1106_COL_OFFSET;
        cmd(0x00 | (col & 0x0F));                    // low column
        cmd(0x10 | ((col >> 4) & 0x0F));             // high column
        dataN(&fb[p*128], 128);                      // 한 페이지 128바이트
    }
}

void SSD1306_Clear(void){ memset(fb,0,sizeof(fb)); }

void SSD1306_Pixel(int x,int y,int on){
  if((unsigned)x>=SSD1306_WIDTH || (unsigned)y>=SSD1306_HEIGHT) return;
  uint16_t i = x + (y>>3)*SSD1306_WIDTH;
  uint8_t  m = 1<<(y&7);
  if(on) fb[i] |= m; else fb[i] &= ~m;
}
void SSD1306_HLine(int x,int y,int w){ for(int i=0;i<w;i++) SSD1306_Pixel(x+i,y,1); }
void SSD1306_VLine(int x,int y,int h){ for(int i=0;i<h;i++) SSD1306_Pixel(x,y+i,1); }

int SSD1306_DrawChar(int x,int y,char ch,const FontDef *font){
  if(ch<32||ch>126) ch='?';
  const uint16_t *g = &font->data[(ch-32)*font->height];
  for(int r=0;r<font->height;r++){
    uint16_t rowBits = g[r];
    for(int c=0;c<font->width;c++){
      int on = (rowBits & (1<<(15-c)))!=0;
      SSD1306_Pixel(x+c,y+r,on);
    }
  }
  return x + font->width + 1;
}
int SSD1306_DrawText(int x,int y,const char *s,const FontDef *font){
  int cx=x; while(*s) cx=SSD1306_DrawChar(cx,y,*s++,font); return cx;
}
void SSD1306_DrawDegree5x5(int x,int y){
  static const uint8_t d[5]={0x06,0x09,0x09,0x09,0x06};
  for(int r=0;r<5;r++) for(int c=0;c<5;c++)
    if(d[r] & (1<<(4-c))) SSD1306_Pixel(x+c,y+r,1);
}
