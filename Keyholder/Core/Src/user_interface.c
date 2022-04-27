#include "user_interface.h"
#include "ssd1306.h"
#include "stdio.h"
#include "main.h"

#include "usbd_cdc_if.h"

char passTrue[6] = "123456";
char pass[6];
void PAS_print_menu( void ){
	
	
	ssd1306_Fill(Black);
	ssd1306_DrawRectangle(0, 0, 127, 31, White);
	
	for (int j = 0; j < 6; j++){
		while(!switches_byte)
			HAL_Delay(1);
		
		ssd1306_SetCursor(2 + 16 * j, 2);
		for(int i = 0; i < 8; i++){
			if(switches_byte & (1 << i)){		
				pass[j] = i + '1';	
				ssd1306_WriteChar(i + '1', Font_16x26, White);
				break;
			}
		}
		
		for(int i = 0; i < j; i++){
			ssd1306_SetCursor(2 + 16 * i, 2);		
			ssd1306_WriteChar('*', Font_16x26, White);
		}
		
		ssd1306_UpdateScreen(); 
		
		while(switches_byte)
			HAL_Delay(1);
	}
	int unPass = 0;
	for (int i = 0; i < 6; i++){
		if (pass[i] == passTrue[i]){
			unPass++;
		}
	}
	if (unPass == 6){
		ssd1306_Fill(White);
		ssd1306_UpdateScreen(); 
		tPass = 1;
	}
	HAL_Delay(1000);
}



void UI_print_menu( void ){
	ssd1306_Fill(Black);
	ssd1306_DrawRectangle(0, 0, 127, 31, White);
	
	while(!switches_byte)
	HAL_Delay(1);
	
	int numMenu = 0;		
	for(int i = 0; i < 8; i++){
		if(switches_byte & (1 << i)){			
				numMenu = i + 1;
				break;
		}
	}
	
	char text[15] = "1 break";
	ssd1306_SetCursor(2,2);
	ssd1306_WriteString(text, Font_6x8, White);
	ssd1306_SetCursor(2,10);
	sprintf(text, "2 ...");
	ssd1306_WriteString(text, Font_6x8, White);
	ssd1306_SetCursor(2,18);
	sprintf(text, "3 chang pass");
	ssd1306_WriteString(text, Font_6x8, White);
	
	switch(numMenu){
		case 1:
			tPass = 0;
			break;
		
	}
			
	ssd1306_UpdateScreen();
	
	while(switches_byte)
			HAL_Delay(1);
	/*
	for(int i = 0; i < 8; i++){
		ssd1306_SetCursor(2 + (i*5) ,17);
		if(switches_byte & (1 << i)){			
			ssd1306_WriteChar('1', Font_6x8, White);
		}
		else{
			ssd1306_WriteChar('0', Font_6x8, White);
		}
	} */
	 
}
