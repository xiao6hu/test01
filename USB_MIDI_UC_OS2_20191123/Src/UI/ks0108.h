
#ifndef __KS0108_H__
#define __KS0108_H__

#include "ks0108_cpu.h"


#define LCD_XSIZE  128
#define LCD_YSIZE  8


#define Ks0108_CMD_Disp_ON          0x3F
#define Ks0108_CMD_Disp_OFF         0x3E
#define Ks0108_CMD_Set_Addr         0x40
#define Ks0108_CMD_Set_Page         0xB8
#define Ks0108_CMD_Disp_Start_Line  0xC0

//显示模式
#define GUI_TEXTMODE_NORMAL      (0)
#define GUI_TEXTMODE_REVERSE     (1<<0)
#define GUI_TEXTMODE_UNDERLINE   (1<<1)
#define GUI_TEXTMODE_XOR         (1<<2)


//对齐方式
#define GUI_TA_LEFT              (0<<4)
#define GUI_TA_HCENTER           (1<<4)
#define GUI_TA_RIGHT             (2<<4)
#define GUI_JUST_TEXT            (1<<7)


//英文的两种字体
#define En_8x16      (0)
#define En_5x8       (1)
#define En_21x32     (2)
#define En_16x16     (3)
#define En_10x16     (4)


//字体宽度定义
#define En_8x16_XSIZE  8
#define En_8x16_YSIZE  2
#define En_5x8_XSIZE   8
#define En_5x8_YSIZE   1


#define En_21x32_XSIZE   21
#define En_21x32_YSIZE   4


#define En_16x16_XSIZE  16
#define En_16x16_YSIZE   2


#define En_10x16_XSIZE  10
#define En_10x16_YSIZE   2


extern U8 PosX;     //当前系统X坐标
extern U8 PosY;     //当前系统Y坐标


//英文字体宽度定义
extern U8 Char_XSIZE;
extern U8 Char_YSIZE;


//中文字体宽度定义
#define Chinese_XSIZE  16
#define Chinese_YSIZE  2


void GUI_GotoX(U8 x);
void GUI_GotoY(U8 y);
void GUI_GotoXY(U8 x,U8 y);
extern U8 GUI_SetTextMode(U8 TextMode);
void GUI_SetEnFont(U8 EnFont);
extern void GUI_DispCharAt(U8 c, U8 x, U8 y);
void GUI_DispChar(U8 c);
void GUI_Init(void);
void GUI_Clear(void);
void GUI_DispCEOL(void);
//void GUI_DispStringAt(U8 _CONST_ *s,U8 X,U8 Y);
void GUI_DispStringAt(U8 *s,U8 X,U8 Y);
void GUI_DispString(U8 _CONST_ *s);
void GUI_DispStringLen(U8 _CONST_ *s, U16 Len);
void GUI_DrawCursor(U8 x, U8 y);
void GUI_CursorOn(U8 x, U8 y);
void GUI_CursorOFF(U8 x, U8 y);
void GUI_DispDecAt(U16 v, U16 x, U16 y, U8 Len);
void GUI_DispHexAt(U32 v, U8 x, U8 y, U8 Len);
//void GUI_DispStringAtBar(U8 _CONST_ *s,U8 x0, U8 y0, U8 x1,U8 Mode);
void GUI_DispStringAtBar(U8 *s,U8 x0, U8 y0, U8 x1,U8 Mode);
void GUI_Delay(U16 Period);
void GUI_DrawIcon(U8 _CONST_ *Ico,U8 X,U8 Y);



void GUI_Draw8x16Icon(U8 _CONST_ *Ico,U8 x0,U8 y0);
void GUI_Draw16x16Icon(U8 _CONST_ *Ico,U8 x0,U8 y0,U8 IsDisplay);
void GUI_Draw16x24Icon(U8 _CONST_ *Ico,U8 x0,U8 y0);
void DisBitMap(_CONST_  U8 * bmp, U8 iXpos, U8 iYpos, U8 iWide, U8 iHeight);
void Clear_Locate(U8 iXpos, U8 iYpos, U8 iWide, U8 iHeight);


void HBar(U8 y, U8 x0, U8 x1,U8 percent);
void Bar(U8 Item_,U8 ItemNum_,U8 BarPosX,U8 BarNumPosX);
void TipDisp( U8 x0, U8 y0, U8 x1, U8 y1);
void TipClr( U8 x0, U8 y0, U8 x1, U8 y1);

extern void Display_ASCII_UnderLine(U8 Char, U8 X, U8 Y);

void LCD_DELAY(void);


#endif   /* __KS0108_H__ */


