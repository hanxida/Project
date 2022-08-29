#ifndef __LCD_H
#define __LCD_H

#include "stm32f10x.h" 

#define X_MAX_PIXEL	        128
#define Y_MAX_PIXEL	        160

#define LCD_RS_SCK_APBxClock_FUN			RCC_APB2PeriphClockCmd
#define	LCD_RS_SCK										RCC_APB2Periph_GPIOB
#define LCD_RS_PORT										GPIOB
#define LCD_RS_PIN										GPIO_Pin_1

#define LCD_RST_SCK_APBxClock_FUN			RCC_APB2PeriphClockCmd
#define	LCD_RST_SCK										RCC_APB2Periph_GPIOB
#define LCD_RST_PORT									GPIOB
#define LCD_RST_PIN										GPIO_Pin_0

#define LCD_RS_Set										GPIO_SetBits(LCD_RS_PORT,LCD_RS_PIN)
#define LCD_RST_Set										GPIO_SetBits(LCD_RST_PORT,LCD_RST_PIN)

#define LCD_RS_Reset									GPIO_ResetBits(LCD_RS_PORT,LCD_RS_PIN)
#define LCD_RST_Reset									GPIO_ResetBits(LCD_RST_PORT,LCD_RST_PIN)

#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//»ÒÉ«0 3165 00110 001011 00101
#define GRAY1   0x8410      	//»ÒÉ«1      00000 000000 00000
#define GRAY2   0x4208      	//»ÒÉ«2  1111111111011111
void LCD_GPIO_Init(void);
void Lcd_WriteIndex(u8 Index);
void Lcd_WriteData(u8 Data);
void LCD_WriteData_16Bit(u16 Data);
void Lcd_WriteReg(u8 Index,u8 Data);
void Lcd_Reset(void);
void LCD_Init(u8 dir);
void Lcd_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end);
void Lcd_SetXY(u16 x,u16 y);
void Gui_DrawPoint(u16 x,u16 y,u16 Data);
unsigned int Lcd_ReadPoint(u16 x,u16 y);
void Show_Pic(void);
void Lcd_Clear(u16 Color);
void Font_Test(void);
void LCD_Color_Fill(u16 x1, u16 y1,u16 x2, u16 y2,u16 Color);
#endif
