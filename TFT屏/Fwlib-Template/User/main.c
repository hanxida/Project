#include "stm32f10x.h" 
#include "LCD.h"
#include "SPI.h"
#include "SysTick.h" 
#include "GUI.h"

int main(void)
{
	// ���������ʱ��ϵͳ��ʱ���Ѿ������ó�72M��
	LCD_Init(0);
	
	Font_Test();
	LCD_Color_Fill(30,50,60,70,RED);
}


