/*
 * ui.c
 *
 *  Created on: Sep 11, 2025
 *      Author: USER
 */

#include <ssh1106.h>
#include <stdio.h>

static int s_cur = 25;
static int s_set = 25;

static int text_width(const char* s, const FontDef* font) {
  int n=0; while(*s){ n += font->width + 1; s++; }  // 글자간 1px
  return (n>0)? n-1 : 0;                             // 마지막 간격 제거
}
static int temp_text_width(const char* num, const FontDef* numFont,
                           int degBox, const FontDef* unitFont) {
  int w = text_width(num, numFont);  // 숫자
  w += 1 + degBox + 1;               // 공백 + °(예: 5px) + 공백
  w += unitFont->width;              // 'C'
  return w;
}

static int labeled_temp_width(const char* label, const FontDef* labelFont,
                              const char* num, const FontDef* numFont,
                              int degBox, const FontDef* unitFont) {
  return text_width(label, labelFont) + 1
       + temp_text_width(num, numFont, degBox, unitFont);
}

void UI_SetCurrentTemp(int t){ s_cur=t; }
void UI_SetSetpoint(int t){ s_set=t; }

void UI_Render(void){
  char buf[16];
  SSD1306_Clear();

  // 구분선
  SSD1306_VLine(64,0,64);
  SSD1306_HLine(0,32,64);

  // 좌상: 로고
  //SSD1306_DrawText(8,8,"UNIOT",&Font_11x18);


  //  {const int LX=0, LY=0, LW=64, LH=32;
  //  const char* t="UNIOT";
  //  int w = text_width(t,&Font_11x18);
  //  int x = LX + (LW - w)/2;
  //  int y = LY + (LH - Font_11x18.height)/2;
  // SSD1306_DrawText(x,y,t,&Font_11x18);}

  // === 좌측(0~63): SV (세팅온도, 중간 크기) ===
   {
     const int BX=0, BY=32, BW=64, BH=32;
     const int DEG=5; // 5x5 도트
     snprintf(buf, sizeof(buf), "%d", s_set);

     // 전체 폭: "SV" + ' ' + "nn" + " ° " + "C"
     int w = labeled_temp_width("SV", &Font_7x10, buf, &Font_11x18, DEG, &Font_7x10);
     int x = BX + (BW - w)/2;
     int y_num = BY + (BH - Font_11x18.height)/2;
     int y_lbl = y_num + (Font_11x18.height - Font_7x10.height); // 같은 베이스라인 느낌

     // "SV"
     int nx = SSD1306_DrawText(x, y_lbl, "SV", &Font_7x10);
     nx += 1; // 라벨 뒤 공백

     // 숫자
     nx = SSD1306_DrawText(nx, y_num, buf, &Font_11x18);

     // ° (세로 중앙 정렬)
     int dy = y_num + (Font_11x18.height - DEG)/2;
     SSD1306_DrawDegree5x5(nx + 1, dy);

     // 'C'
     SSD1306_DrawText(nx + 1 + DEG + 1, y_num, "C", &Font_7x10);
   }

  // 좌하: 현재온도 "25 °C"
  //snprintf(buf,sizeof(buf),"%d",s_cur);
 // int nx = SSD1306_DrawText(8,40,buf,&Font_7x10);
  //SSD1306_DrawDegree5x5(nx+2,40);
  //SSD1306_DrawText(nx+9,40,"C",&Font_7x10);
    // 좌하(0,32,64,32) 중앙 : "nn °C"

  // {const int BX=0, BY=32, BW=64, BH=32;
   //   snprintf(buf,sizeof(buf),"%d",s_cur);
    //  int w=temp_text_width(buf,&Font_7x10,5,&Font_7x10);
    //  int x= BX + (BW-w)/2;
    //  int y= BY + (BH-Font_7x10.height)/2;
    //  int nx=SSD1306_DrawText(x,y,buf,&Font_7x10);
    //  int dy = y + (Font_7x10.height - 5 - 6);  // = 10-5-2 → 3픽셀 위
   //   SSD1306_DrawDegree5x5(nx+1,dy);
    //  SSD1306_DrawText(nx+8,y,"C",&Font_7x10);}

  // 우측: 세팅온도 크게
  //snprintf(buf,sizeof(buf),"%d",s_set);
  //int bx = SSD1306_DrawText(74,16,buf,&Font_16x26);
  //SSD1306_DrawDegree5x5(bx+4,20);
  //SSD1306_DrawText(bx+16,16,"C",&Font_16x26);

     // 우측(64,0,64,64) 중앙 : "nn °C" (경계 마진 포함)
     {
    	 const int RX=64, RY=0, RW=64, RH=64, MARGIN=2;
    	   char buf[16];
    	   snprintf(buf,sizeof(buf), "%d", s_set);
    	   // 전체 폭: 숫자 + (공백1 + °5 + 공백1) + 'C'
    	   int w  = temp_text_width(buf, &Font_16x26, 5, &Font_16x26);
    	   int sx = RX + (RW - w)/2;
    	   int sy = RY + (RH - Font_16x26.height)/2;
    	   if (sx < RX + MARGIN) sx = RX + MARGIN;
    	   // 숫자
    	   int nx = SSD1306_DrawText(sx, sy, buf, &Font_16x26);
    	   // ° (왼쪽 공백 1px 반영 + 수직 중앙 정렬)
    	   int nxDeg = nx + 1;                              // ← 공백 1px
    	   int dy    = sy + (Font_16x26.height - 20)/2;      // 5x5를 세로 중앙
    	   SSD1306_DrawDegree5x5(nxDeg, dy);
    	   // 'C' (°5px + 공백1px 뒤)
    	   int nxC = nxDeg + 5 + 1;
    	   SSD1306_DrawText(nxC, sy, "C", &Font_16x26);
     }

  SSD1306_Update();
}
