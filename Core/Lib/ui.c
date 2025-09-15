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
  ssh1106_Clear();

  // 구분선
  ssh1106_VLine(64,0,64);
  ssh1106_HLine(0,32,64);

  // 좌상: 로고
  //ssh1106_DrawText(8,8,"UNIOT",&Font_11x18);


    {const int LX=0, LY=0, LW=64, LH=32;
    const char* t="UNIOT";
    int w = text_width(t,&Font_11x18);
    int x = LX + (LW - w)/2;
    int y = LY + (LH - Font_11x18.height)/2;
   ssh1106_DrawText(x,y,t,&Font_11x18);}


    // 좌하(0,32,64,32) 중앙 : "sv nn ° C"
    {
        const int BX = 0, BY = 32, BW = 64, BH = 32;
        const int DEG = 5;          // ° 5x5
        const int DEG_RAISE = 4;    // °를 위로 올릴 픽셀 수

        char buf[16];
        snprintf(buf, sizeof(buf), "%d", s_set);

        const FontDef* LBL = &Font_7x10;   // "sv"
        const FontDef* NUM = &Font_11x18;  // 숫자
        const FontDef* CF  = &Font_11x18;  // 'C'

        // 전체 폭: "sv"(작게) + 공백1 + 숫자 + 공백1 + ° + 공백1 + C
        int w  = labeled_temp_width("sv", LBL, buf, NUM, DEG, CF);

        // 가로 중앙
        int x = BX + (BW - w)/2;
        if (x < BX + 2) x = BX + 2;

        // 세로 중앙: 숫자 기준
        int y_num  = BY + (BH - NUM->height)/2;
        int y_lbl  = y_num + (NUM->height - LBL->height);  // 라벨 베이스라인 맞춤

        // "sv"
        int nx = ssh1106_DrawText(x, y_lbl, "sv", LBL);
        nx += 1; // 공백 1px

        // 숫자
        nx = ssh1106_DrawText(nx, y_num, buf, NUM);

        // °
        int nxDeg = nx + 1;
        int dy    = y_num + (NUM->height - DEG)/2 - DEG_RAISE;
        if (dy < 0) dy = 0;
        ssh1106_DrawDegree5x5(nxDeg, dy);

        // 'C'
        ssh1106_DrawText(nxDeg + DEG + 1, y_num, "C", CF);
	}


    /* ---------- RIGHT (64~127 x 0~63): 위에 'cv', 아래 큰 숫자 + °(살짝 위) + C ---------- */
    {
        const int RX=64, RY=0, RW=64, RH=64, MARGIN=2;
        const int DEG = 5;          // ° 5x5
        const int DEG_RAISE = 5;    // °를 위로 띄우는 픽셀(1~3에서 취향)

        char buf[16];
        snprintf(buf, sizeof(buf), "%d", s_cur);   // cv = 현재온도

        // 폰트: 라벨 작게, 숫자/단위는 큼
        const FontDef* LBL = &Font_7x10;    // "cv"
        const FontDef* NUM = &Font_16x26;   // 숫자(두 자리까지 안전)
        const FontDef* CF  = &Font_16x26;   // 'C'도 숫자와 동일 크기

        /* 1) 윗줄: 'cv' (작게, 수평 중앙) */
        int w_lbl = text_width("cv", LBL);
        int x_lbl = RX + (RW - w_lbl) / 2;
        int y_lbl = RY + 10; // 상단에서 약간 내려오게
        ssh1106_DrawText(x_lbl, y_lbl, "cv", LBL);

        /* 2) 아랫줄: "nn ° C" (큰 폰트, 수평 중앙) */
        int w_num = temp_text_width(buf, NUM, DEG, CF);   // 숫자+°+C 전체폭
        int sx    = RX + (RW - w_num) / 2;
        if (sx < RX + MARGIN) sx = RX + MARGIN;
        // 숫자 줄은 화면 중앙보다 약간 아래로 내려 균형
        int y_num = RY + (RH - NUM->height) / 2 + 4;

        // 숫자
        int nx = ssh1106_DrawText(sx, y_num, buf, NUM);

        // ° : 세로 중앙에서 살짝 위로
        int dy = y_num + (NUM->height - DEG) / 2 - DEG_RAISE;
        if (dy < 0) dy = 0;
        ssh1106_DrawDegree5x5(nx + 1, dy);         // 숫자 뒤 공백 1px

        // 'C' : 숫자와 같은 폰트/베이스라인
        ssh1106_DrawText(nx + 1 + DEG + 1, y_num, "C", CF);
    }
  ssh1106_Update();
}
