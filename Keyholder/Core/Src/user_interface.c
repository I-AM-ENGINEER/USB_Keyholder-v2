#include "user_interface.h"
#include "ssd1306.h"
#include "stdio.h"
#include "main.h"

void UI_print_menu( void ){
	ssd1306_Fill(Black);
	ssd1306_DrawRectangle(0, 0, 127, 31, White);
	ssd1306_SetCursor(2,2);
	char text[20] = "test text";
	ssd1306_WriteString(text, Font_6x8, White);
	sprintf(text, "test text 2: %d", test_value);
	ssd1306_SetCursor(2,10);
	ssd1306_WriteString(text, Font_6x8, White);
	ssd1306_UpdateScreen();
}
