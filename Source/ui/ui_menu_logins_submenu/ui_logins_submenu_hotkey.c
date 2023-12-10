#include "ui/core.h"
#include "crypto.h"

static uint16_t password_num;

void UI_menu_logins_submenu_hotkey_draw( void ){
	UI_event_button_t lastButton = UI_event_get_last();
	
	switch(lastButton.event_type){
		case BUTTON_STATE_PRESSED:
			if(lastButton.button_id == BTN_JPUSH_ID){
                crypto_save();
				ugl_return();
			}else if((lastButton.button_id >= BTN_SW1_ID) && (lastButton.button_id <= BTN_SW8_ID)){
                crypto_hotkey_password_set(lastButton.button_id - BTN_SW1_ID, password_num);
			}
			break;
		case BUTTON_STATE_HOLDED:
			if(lastButton.button_id == BTN_JPUSH_ID){
                crypto_save();
				ugl_return();
			}else if((lastButton.button_id >= BTN_SW1_ID) && (lastButton.button_id <= BTN_SW8_ID)){
                crypto_hotkey_password_set(lastButton.button_id - BTN_SW1_ID, -1);
			}
			break;
		default: break;
	}
	ssd1306_Fill(Black);
	ugl_get_menu_by_id(UI_MENU_ID_MAIN)->render_f();
	//UI_main_menu_render();
    ugl_menu_render( ugl_get_current_menu() );
    ssd1306_SetCursor(10, 20);
    ssd1306_WriteString("Set hotkey for", Font_7x10, White);
    ssd1306_SetCursor(10, 32);
    crypto_password_t* password;
    crypto_password_get_ptr(&password, password_num);
    ssd1306_WriteString(password->login, Font_7x10, White);
}

ugl_menu_t *UI_menu_logins_submenu_hotkey_constructor( int32_t ID, void* extra ){
    if(extra == NULL){
        return NULL;
    }
	ugl_menu_t *mainMenu = ugl_menu_constructor(UI_MENU_ID_LOGINS_SUBMENU_HOTKEY);
    password_num = *(uint16_t*)extra;
	mainMenu->process_f = UI_menu_logins_submenu_hotkey_draw;
	return mainMenu;
}
