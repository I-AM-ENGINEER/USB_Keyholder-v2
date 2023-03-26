#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include "main.h"
#include "buttons.h"
#include "ssd1306.h"
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

typedef enum{
	UI_ITEM_SPRITE,
	UI_ITEM_TEXT,
	UI_ITEM_NONE,
} UI_item_type_t;

typedef struct{
	uint8_t* bitmap;
	uint32_t bitmapHeight;
	uint32_t bitmapWidth;
	bool visible;
} UI_sprite_t;

typedef struct{
	char* string;
	uint32_t length;
	FontDef* font;
	bool visible;
} UI_text_t;

typedef struct UI_item 	UI_item_t;
typedef struct UI_group UI_group_t;
typedef struct UI_menu 	UI_menu_t;

struct UI_item{
	uint32_t ID;
	UI_item_type_t type;
	union{
		UI_sprite_t	*sprite;
		UI_text_t		*text;
	};
	//bool visible;
	int32_t positionX;
	int32_t positionY;
	
	int32_t positionXabs;
	int32_t positionYabs;
	
	UI_item_t (*nextItem);
	UI_item_t (*previousItem);
};

struct UI_group{
	uint32_t ID;
	uint32_t groupsCount;
	uint32_t itemsCount;
	
	int32_t positionX;
	int32_t positionY;
	
	int32_t positionXabs;
	int32_t positionYabs;
	
	UI_group_t **groups;
	UI_item_t 	**items;
};

struct UI_menu{
	uint32_t ID;
	void (*drawingFunction) ( void );
	UI_menu_t 	*callerPtr;
	UI_item_t 	*selectedItem;
	UI_group_t 	*group;
};

typedef struct{
	BTN_buttonState_t eventType;
	uint32_t buttonID;
} UI_event_button_t;

typedef enum{
	UI_MENU_MAIN_SETTINGS,
	UI_MENU_MAIN_PASSWORDS,
	UI_MENU_MAIN_USB_WRITE,
	UI_MENU_MAIN_FOLDER,
	UI_MENU_MAIN_LOCK,
} UI_Menu_mainItems_t;

void UI_print_menu( void );
void UI_event_SetButton( uint32_t buttonID, BTN_buttonState_t state );
void UI_Init( void );

/* -------------------------------------------------------------------------- */

#endif // __USER_INTERFACE_H__
