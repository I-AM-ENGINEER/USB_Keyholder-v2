#include "ui/core.h"
#include "crypto.h"

static uint16_t password_num;
static uint16_t password_num_new;
static uint16_t cursor_position;

void UI_menu_logins_submenu_move_process( void ){
	UI_event_button_t lastButton = UI_event_get_last();
	
	switch(lastButton.event_type){
		case BUTTON_STATE_PRESSED:
			if(lastButton.button_id == BTN_JCW_ID){
				if(password_num > 0){
					crypto_password_swap(password_num, password_num-1);
					password_num--;
					cursor_position--;
				}
			}else if(lastButton.button_id == BTN_JCCW_ID){
				if(password_num < (crypto_password_count()-1)){
					crypto_password_swap(password_num, password_num+1);
					password_num++;
					cursor_position++;
				}
			}else if(lastButton.button_id == BTN_JPUSH_ID){
				crypto_save();
				ugl_return();
				return;
			}
			break;
		default: break;
	}
}

void UI_menu_logins_submenu_move_render( void ){
	//ugl_menu_t* logins_menu = ugl_get_menu_by_id(UI_MENU_ID_LOGINS);
	//logins_menu->selected_item = ugl_menu_get_item_by_id(logins_menu, 3);
	//logins_menu->render_f();
	uint32_t start_render_pos;
	if(cursor_position < 0){
		cursor_position = 0;
	}else if(cursor_position > 6){
		cursor_position = 6;
	}

	ssd1306_Fill(Black);
    ssd1306_SetCursor(0,0);
    //ssd1306_WriteString("Placeholder", Font_7x10, White);
	for(int32_t i = -3; i < 3; i++){
		crypto_password_t* password;
		
		ssd1306_Line(0, (26) + 3, 2, (26) + 5, White);
		ssd1306_Line(2, (26) + 6, 0, (26) + 8, White);
		
		if(crypto_password_get_ptr(&password, i + password_num) == CRYPTO_STATE_OK){
			ssd1306_SetCursor(4,28+i*(UI_MENU_LOGINS_INTERVALS+1));
			ssd1306_WriteString(password->login, UI_MENU_LOGINS_FONT, White);
		}
		
	}
}

ugl_menu_t *UI_menu_logins_submenu_move_constructor( int32_t ID, void* extra ){
	ugl_menu_t *mainMenu = ugl_menu_constructor(UI_MENU_ID_LOGINS_SUBMENU_MOVE);
	ugl_item_t *item = NULL;
    password_num = *(uint16_t*)extra;
	
	mainMenu->selected_item = ugl_menu_get_item_by_id(mainMenu, 0);
	mainMenu->process_f = UI_menu_logins_submenu_move_process;
    mainMenu->render_f = UI_menu_logins_submenu_move_render;
	return mainMenu;
}
