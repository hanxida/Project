#include "stm32f10x.h" 
#include "LCD.h"
#include "SPI.h"
#include "SysTick.h" 
#include "GUI.h"

int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M。
	LCD_Init(0);
	
	Font_Test();
	LCD_Color_Fill(30,50,60,70,RED);
}


