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

static bool released = false;
static bool show_password = false;
static bool last_p = false;

void UI_hotkey_menu_draw( void ){
	UI_event_button_t lastButton = UI_event_GetLast();
	
	
	ssd1306_Fill(Black);
	
	switch(lastButton.event_type){
		case BUTTON_STATE_RELEASED:
			released = true;
			if(lastButton.button_id == BTN_JPUSH_ID){
				if(last_p){
					last_p = false;
					break;
				}
				show_password = false;
				released = false;
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
		default: break;
	}
	
	ssd1306_SetCursor(20,0);
	ssd1306_WriteString((char*)password->login, Font_7x10, White);
	
	
	if(!show_password){
		ssd1306_SetCursor(10,20);
		ssd1306_WriteString("INSERT!", Font_16x26, White);
		USB_update_status();
		if((USB_connection_status() == USB_STATUS_CONNECTED) && released){
			HAL_Delay(100);
			USB_keyboard_puts(password->password);
			show_password = false;
			released = false;
			last_p = false;
			ugl_return();
			return;
		}
	}else{
		ssd1306_SetCursor(0,20);
		ssd1306_WriteString((char*)password->password, Font_7x10, White);
		ssd1306_SetCursor(0,40);
		ssd1306_WriteString((char*)password->comment, Font_7x10, White);
	}
	
	ugl_menu_render( ugl_get_current_menu() );
}

ugl_menu_t *UI_hotkey_menu_constructor( int32_t ID, void* extra ){
	if(extra == NULL){
		return NULL;
	}
	released = false;
	show_password = false;
	last_p = false;
	
	password = extra;
	
	ugl_menu_t *hotkey_menu = ugl_menu_constructor(ID);
	hotkey_menu->drawing_function = UI_hotkey_menu_draw;
	
	return hotkey_menu;
}

