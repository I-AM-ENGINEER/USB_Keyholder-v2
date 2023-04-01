#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include "main.h"
#include "buttons.h"
#include "ssd1306.h"
#include "system.h" // buttons here
#include "ugl_core.h"

//#include <stddef.h>
//#include "stm32l1xx_hal.h"

/* -------------------------------------------------------------------------- */


#define MAIN_MENU_ICONS_HEIGHT 30
#define MAIN_MENU_ICONS_WIDTH  30

#define UI_ICON_ADDRESS_FOLDER 		0x0001
#define UI_ICON_ADDRESS_SETTINGS 	0x0002
#define UI_ICON_ADDRESS_LOCK 			0x0003
#define UI_ICON_ADDRESS_USB_WRITE 0x0004
#define UI_ICON_ADDRESS_PASSWORDS 0x0005

#define IO_EVENT_MAX_STACK	4

#pragma anon_unions


typedef struct{
	BTN_button_state_t event_type;
	uint32_t button_id;
} UI_event_button_t;

typedef enum{
	UI_MENU_MAIN_SETTINGS,
	UI_MENU_MAIN_PASSWORDS,
	UI_MENU_MAIN_USB_WRITE,
	UI_MENU_MAIN_FOLDER,
	UI_MENU_MAIN_LOCK,
} UI_Menu_mainItems_t;


ugl_menu_t *UI_main_menu_constructor( int32_t ID, void* extra );
ugl_menu_t *UI_login_menu_constructor( int32_t ID, void* extra );
ugl_menu_t *UI_hotkey_menu_constructor( int32_t ID, void* extra );
ugl_menu_t *UI_menu_logins_constructor( int32_t ID, void* extra );

void UI_print_menu( void );
void UI_event_set_button( uint32_t button_id, BTN_button_state_t state );
void UI_Init( void );


void UI_menu_Return( void );
void UI_event_clear_last( void );
UI_event_button_t UI_event_GetLast( void );


/* -------------------------------------------------------------------------- */

#endif // __USER_INTERFACE_H__
