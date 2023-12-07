#include "ui/core.h"
#include "crypto.h"
#include "usbd.h"

BTN_ids_t pushed_button;

const crypto_password_t* password;

/*
typedef enum{
	UI_MENU_HOTKEY_STATE_WAIT_PLUG,
	UI_MENU_HOTKEY_STATE_OPENED,
	//UI_MENU_HOTKEY_STATE_
}*/

static bool show_password = false;
static bool last_p = false;

void UI_hotkey_menu_process( void ){
	UI_event_button_t lastButton = UI_event_get_last();
	
	switch(lastButton.event_type){
		case BUTTON_STATE_RELEASED:
			if(lastButton.button_id == BTN_JPUSH_ID){
				if(last_p){
					last_p = false;
					break;
				}
				show_password = false;
				last_p = false;
				ugl_return();
				return;
			}
			break;
		case BUTTON_STATE_HOLDED:
			if(!last_p){
				show_password = !show_password;
			}
			if(lastButton.button_id == BTN_JPUSH_ID){
				last_p = true;
			}
			break;
		default: break;
	}
	
	if(!show_password){
		USB_update_status();
		if(USB_connection_status() == USB_STATUS_CONNECTED){
			HAL_Delay(300);
			USB_keyboard_puts(password->password);
			show_password = false;
			last_p = false;
			ugl_return();
			return;
		}
	}
}

void UI_hotkey_menu_render( void ){
	ssd1306_Fill(Black);
	ssd1306_SetCursor(20,0);
	ssd1306_WriteString((char*)password->login, Font_7x10, White);
	
	if(show_password){
		ssd1306_SetCursor(0,20);
		ssd1306_WriteString((char*)password->password, Font_7x10, White);
		ssd1306_SetCursor(0,40);
		ssd1306_WriteString((char*)password->comment, Font_7x10, White);
	}else{
		ssd1306_SetCursor(10,20);
		ssd1306_WriteString("INSERT!", Font_16x26, White);
	}
}

ugl_menu_t *UI_hotkey_menu_constructor( int32_t ID, void* extra ){
	if(extra == NULL){
		return NULL;
	}
	show_password = false;
	last_p = false;
	
	password = extra;
	
	ugl_menu_t *hotkey_menu = ugl_menu_constructor(UI_MENU_ID_HOTKEY);
	hotkey_menu->process_f = UI_hotkey_menu_process;
	hotkey_menu->render_f = UI_hotkey_menu_render;
	return hotkey_menu;
}

