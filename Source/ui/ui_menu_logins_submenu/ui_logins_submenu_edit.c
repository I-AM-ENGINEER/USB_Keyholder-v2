#include "ui/core.h"
#include "crypto.h"

static uint16_t password_num;

void UI_menu_logins_submenu_edit_process( void ){
	UI_event_button_t lastButton = UI_event_get_last();
	
	switch(lastButton.event_type){
		case BUTTON_STATE_PRESSED:
			if(lastButton.button_id == BTN_JCW_ID){
				ugl_menu_previous_item(ugl_get_current_menu());
			}else if(lastButton.button_id == BTN_JCCW_ID){
				ugl_menu_next_item(ugl_get_current_menu());
			}else if(lastButton.button_id == BTN_JPUSH_ID){
                if(ugl_get_current_menu()->selected_item->ID){
                    //crypto_password_remove(password_num);
                    //crypto_save();
                }
				ugl_return();
				return;
			}
			break;
		default: break;
	}
}

void UI_menu_logins_submenu_edit_render( void ){
	ssd1306_Fill(Black);
    ssd1306_SetCursor(0,0);
    ssd1306_WriteString("Placeholder", Font_7x10, White);
}

ugl_menu_t *UI_menu_logins_submenu_edit_constructor( int32_t ID, void* extra ){
	ugl_menu_t *mainMenu = ugl_menu_constructor(UI_MENU_ID_LOGINS_SUBMENU_EDIT);
	ugl_item_t *item = NULL;
    password_num = *(uint16_t*)extra;
	
	mainMenu->process_f = UI_menu_logins_submenu_edit_process;
    mainMenu->render_f = UI_menu_logins_submenu_edit_render;
	return mainMenu;
}
