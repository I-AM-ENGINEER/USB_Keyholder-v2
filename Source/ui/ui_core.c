#include "ui/core.h"
#include "stdio.h"
#include "crypto.h"
#include "main.h"
#include "stdlib.h"
#include "usb.h"
#include "system.h"
#include "buttons.h"
#include "ui/bitmaps.h"

/**********   Events  *******************/

void UI_getData( uint32_t address, void *icon, uint32_t size ){
	switch(address){
		case UI_ICON_ADDRESS_FOLDER: 		memcpy(icon, img_folder, size); 		break;
		case UI_ICON_ADDRESS_SETTINGS: 	memcpy(icon, img_settings, size); 	break;
		case UI_ICON_ADDRESS_LOCK: 			memcpy(icon, img_lock, size); 			break;
		case UI_ICON_ADDRESS_USB_WRITE: memcpy(icon, img_usb_write, size); 	break;
		case UI_ICON_ADDRESS_PASSWORDS: memcpy(icon, img_passwords, size); 	break;
		default: 												memset(icon, 0x55, size); 					break;
	}
}

static bool UI_buttons_event_enabled = false;
static UI_event_button_t* UI_event_buttonStack = NULL;
static uint32_t UI_event_buttonStackSize;
static uint32_t UI_event_buttonCount;

void UI_event_clear_last( void ){
	if(UI_event_buttonCount == 0) return;
	UI_event_buttonCount--;
	UI_event_buttonStack[UI_event_buttonCount].event_type = BUTTON_STATE_IDLE;
}

UI_event_button_t UI_event_get_last( void ){
	if(UI_event_buttonCount == 0){
		UI_event_button_t t;
		t.button_id = -1;
		t.event_type = BUTTON_STATE_IDLE;
		return t;
	}
	UI_event_button_t return_event = UI_event_buttonStack[UI_event_buttonCount - 1];
	UI_event_clear_last();
	return return_event;
}

inline void UI_event_stop( void ){
	UI_buttons_event_enabled = false;
}

inline void UI_event_start( void ){
	UI_buttons_event_enabled = true;
}

void UI_event_set_button( uint32_t button_id, BTN_button_state_t state ){
	if(!UI_buttons_event_enabled){
		return;
	}
	if(UI_event_buttonCount >= UI_event_buttonStackSize) return;
	UI_event_buttonStack[UI_event_buttonCount].button_id  = button_id;
	UI_event_buttonStack[UI_event_buttonCount].event_type = state;
	UI_event_buttonCount++;
	return;
}

/**********   User  *******************/

void UI_Init( void ){
	UI_event_buttonStack = malloc(sizeof(UI_event_button_t)*IO_EVENT_MAX_STACK);
	UI_event_buttonStackSize = IO_EVENT_MAX_STACK;
	
	for(uint32_t i; i < UI_event_buttonStackSize; i++) UI_event_buttonStack[i].event_type = BUTTON_STATE_IDLE;
	UI_event_start();
	uint16_t num = 0;
	ugl_enter(0, UI_login_menu_constructor, NULL);
	//ugl_enter(0, UI_main_menu_constructor, NULL);
	//ugl_enter(0, UI_menu_logins_constructor, NULL);
	//ugl_enter(0, UI_menu_logins_submenu_constructor, &num);
	//ugl_enter(0, UI_menu_logins_submenu_edit_constructor, &num);
}

void UI_print_menu( void ){
	ugl_render();
	ssd1306_UpdateScreen();
}
