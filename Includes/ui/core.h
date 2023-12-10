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


#define UI_MENU_LOGINS_PRINTED_COUNT 	6
#define UI_MENU_LOGINS_SUBMENU_COUNT  4
#define UI_MENU_LOGINS_FONT						Font_7x10
#define UI_MENU_LOGINS_INTERVALS			10

#define IO_EVENT_MAX_STACK	4

typedef struct{
	char* editable_string;
	const char* comment_string;
	uint8_t string_max_length;
	bool zero_ended;
} UI_edit_menu_set_t;

typedef enum{
	UI_MENU_ID_AUTH,
	UI_MENU_ID_MAIN,
	UI_MENU_ID_LOGINS,
	UI_MENU_ID_HOTKEY,
	UI_MENU_ID_LOGINS_SUBMENU,
	UI_MENU_ID_LOGINS_SUBMENU_DELETE,
	UI_MENU_ID_LOGINS_SUBMENU_MOVE,
	UI_MENU_ID_LOGINS_SUBMENU_EDIT,
	UI_MENU_ID_LOGINS_SUBMENU_HOTKEY,
	UI_MENU_ID_EDITOR,
} UI_menu_id_e;

typedef struct{
	BTN_button_state_t event_type;
	uint32_t button_id;
} UI_event_button_t;

ugl_menu_t *UI_menu_editor_constructor( int32_t ID, void* extra );
ugl_menu_t *UI_main_menu_constructor( int32_t ID, void* extra );
ugl_menu_t *UI_login_menu_constructor( int32_t ID, void* extra );
ugl_menu_t *UI_hotkey_menu_constructor( int32_t ID, void* extra );
ugl_menu_t *UI_menu_logins_constructor( int32_t ID, void* extra );
ugl_menu_t *UI_menu_logins_submenu_constructor( int32_t ID, void* extra );
ugl_menu_t *UI_menu_logins_delete_warning_constructor( int32_t ID, void* extra );
ugl_menu_t *UI_menu_logins_submenu_hotkey_constructor( int32_t ID, void* extra );
ugl_menu_t *UI_menu_logins_submenu_move_constructor( int32_t ID, void* extra );
ugl_menu_t *UI_menu_logins_submenu_edit_constructor( int32_t ID, void* extra );
ugl_menu_t *UI_menu_logins_submenu_edit_generate_password_construct( int32_t ID, void* extra );

void UI_print_menu( void );
void UI_event_set_button( uint32_t button_id, BTN_button_state_t state );
void UI_Init( void );

UI_event_button_t UI_event_get_last( void );
void UI_event_stop( void );
void UI_event_start( void );

/* -------------------------------------------------------------------------- */

#endif // __USER_INTERFACE_H__
