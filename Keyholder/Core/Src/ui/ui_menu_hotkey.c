#include "ui_core.h"
#include "crypto.h"

BTN_ids_t pushed_button;

crypto_password_t* password;

void UI_hotkey_menu_draw( void ){
	UI_event_button_t lastButton = UI_event_GetLast();
	UI_event_clear_last();
	
	static bool show_password = false;
	static bool last_p = false;
	
	ssd1306_Fill(Black);
	
	switch(lastButton.event_type){
		//case BUTTON_STATE_PRESSED:
			
			//break;
		case BUTTON_STATE_RELEASED:
			if(lastButton.button_id == BTN_JPUSH_ID){
				if(last_p){
					last_p = false;
					break;
				}
				show_password = false;
				ugl_return();
				return;
			}
			break;
		case BUTTON_STATE_HOLDED:
			if(lastButton.button_id == BTN_JPUSH_ID){
				show_password = true;
				last_p = true;
			}
		default: break;
	}
	
	ssd1306_SetCursor(0,0);
	ssd1306_WriteString("LLL", Font_7x10, White);
	ssd1306_SetCursor(0,10);
	ssd1306_WriteString("PSWD:", Font_7x10, White);
	ssd1306_SetCursor(0,20);
	if(show_password){
		ssd1306_WriteString("YES", Font_7x10, White);
	}
	/*
	switch(lastButton.event_type){
		case BUTTON_STATE_PRESSED:
			if((lastButton.button_id >= BTN_SW1_ID) && (lastButton.button_id <= BTN_SW8_ID)){
				pswd[cursor] = lastButton.button_id + '1';
				if((cursor < 5) && (pswd[cursor+1] == ' ')){ 
					cursor++;
				}
			}else if(lastButton.button_id == BTN_JCW_ID) {
				if(cursor < 5) cursor++;
			}
			else if(lastButton.button_id == BTN_JCCW_ID) {
				if(cursor > 0) cursor--;
			}
			break;
		default: break;
	}
	UI_event_clear_last();
	
	
	bool stringFilled = true;
	for(uint32_t i = 0; i < 6; i++){
		if(pswd[i] == ' '){
			stringFilled = false;
		}
	}
	
	if(stringFilled){
		cursor = 0;
		if(!memcmp(pswd, "222222", 6)){
			memset(pswd, ' ', 6);
			ugl_return();
			//UI_menu_Return();
			return;
		}else{
			memset(pswd, ' ', 6);
		}
	}
	
	
	
	for(uint32_t i = 0; i < 6; i++){
		ssd1306_SetCursor(6 + i*19, 20);
		if(i == cursor){
			ssd1306_WriteChar(pswd[i], Font_16x26, White);
		}else if((i == (cursor-1)) && pswd[cursor] == ' '){
			ssd1306_WriteChar(pswd[i], Font_16x26, White);
		}else if(pswd[i] != ' '){
			ssd1306_WriteChar('*', Font_16x26, White);
		}
		
		
		ssd1306_Line(6 + i*19, 43, 22 + i*19, 43, White);
		ssd1306_Line(6 + i*19, 17, 22 + i*19, 17, White);
	}
	ssd1306_Line(6 + cursor*19, 45, 22 + cursor*19, 45, White);
	
	ssd1306_DrawRectangle(0, 0,  127, 10, White);
	ssd1306_DrawRectangle(0, 53, 127, 63, White);
	ssd1306_Line(1, 0,  126, 0,  Black);
	ssd1306_Line(1, 63, 126, 63, Black);
	*/
	
	
	/*
	uint32_t offset = 11;
	
	ssd1306_SetCursor(2+offset,0);
	ssd1306_WriteString("1", Font_7x10, White);
	ssd1306_Line(31, 0, 31, 10, White);
	ssd1306_SetCursor(33+offset,0);
	ssd1306_WriteString("2", Font_7x10, White);
	ssd1306_Line(63, 0, 63, 10, White);
	ssd1306_SetCursor(65+offset,0);
	ssd1306_WriteString("3", Font_7x10, White);
	ssd1306_Line(95, 0, 95, 10, White);
	ssd1306_SetCursor(97+offset,0);
	ssd1306_WriteString("4", Font_7x10, White);
	*/
	
	
	//ssd1306_DrawRectangle(10, 10, 30, 40, White);
	
	//ssd1306_WriteString(UI_current_menu->group->items[0]->text->string, Font_7x10, White);
	
	ugl_menu_render( ugl_get_current_menu() );
}

ugl_menu_t *UI_hotkey_menu_constructor( uint32_t ID, void* extra ){
	//password
	if(extra == NULL){
		return NULL;
		//memcpy(&pushed_button, extra, sizeof(BTN_ids_t));
	}
	//password
	
	ugl_menu_t *hotkey_menu = ugl_menu_constructor(ID);
	//ugl_item_t *item = NULL;
	//ugl_text_t *text = NULL;
	
	/*
	item = ugl_item_constructor(0);
	ugl_item_set_text(item, ugl_text_constructor());
	ugl_text_set_test(item->text, "      ", &Font_16x26);
	ugl_menu_add_item(menu, item);
	
	ugl_item_set_position(ugl_menu_get_item_by_id(menu, 0), 0,  0);
	
	
	menu->group->position_x = 16;
	menu->group->position_y = 18;
	*/
	hotkey_menu->drawing_function = UI_hotkey_menu_draw;
	
	return hotkey_menu;
}

