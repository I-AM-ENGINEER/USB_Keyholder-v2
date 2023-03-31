#include "ui_core.h"
#include "crypto.h"

void UI_mainMenuDraw( void ){
	ssd1306_Fill(Black);
	static BTN_ids_t holdedButton = BTN_NONE;
	static uint32_t lastSwipeTimestamp = 0;
	
	
	UI_event_button_t lastButton = UI_event_GetLast();
	UI_event_clear_last();
	
	switch(lastButton.event_type){
		case BUTTON_STATE_PRESSED:
			if(lastButton.button_id == BTN_JCW_ID){
				ugl_get_current_menu()->selected_item = ugl_get_current_menu()->selected_item->next_item;
			}else if(lastButton.button_id == BTN_JCCW_ID){
				ugl_get_current_menu()->selected_item = ugl_get_current_menu()->selected_item->previous_item;
			}else if(lastButton.button_id == BTN_JPUSH_ID){
				switch(ugl_get_current_menu()->selected_item->ID){
					case UI_MENU_MAIN_LOCK: 
						ugl_enter(1, UI_login_menu_constructor, NULL);
						return;
						break;
					default: break;
				}
			}else if((lastButton.button_id >= BTN_SW1_ID) && (lastButton.button_id <= BTN_SW8_ID)){
				crypto_password_t* password;
				//BTN_ids_t pushed_button = lastButton.button_id;
				if(crypto_hotkey_password_get(lastButton.button_id, &password) == CRYPTO_STATE_OK){
					if(password != NULL){
						ugl_enter(1, UI_hotkey_menu_constructor, password);
						return;
					}
				}
			}
			break;
		case BUTTON_STATE_HOLDED:
			holdedButton = lastButton.button_id;
			break;
		case BUTTON_STATE_RELEASED:
			holdedButton = BTN_NONE;
			break;
		default: break;
	}
	
	// Holded swipe
	if(((HAL_GetTick() - lastSwipeTimestamp) > 200) && ((holdedButton == BTN_JCW_ID) || (holdedButton == BTN_JCCW_ID))){
		if(holdedButton == BTN_JCW_ID) 	ugl_get_current_menu()->selected_item = ugl_get_current_menu()->selected_item->next_item;
		else 														ugl_get_current_menu()->selected_item = ugl_get_current_menu()->selected_item->previous_item;
		lastSwipeTimestamp = HAL_GetTick();
	}
	
	if(ugl_get_current_menu()->selected_item->position_x_abs > 50) {
		ugl_get_current_menu()->group->position_x -= 5;
	}else if(ugl_get_current_menu()->selected_item->position_x_abs < 50){
		ugl_get_current_menu()->group->position_x += 5;
	}
	
	ugl_menu_render( ugl_get_current_menu() );
	
	ssd1306_DrawRectangle(ugl_get_current_menu()->selected_item->position_x_abs, ugl_get_current_menu()->selected_item->position_y_abs, ugl_get_current_menu()->selected_item->position_x_abs+10, ugl_get_current_menu()->selected_item->position_y_abs+10, White);
	ssd1306_DrawRectangle(0, 0,  127, 10, White);
	ssd1306_DrawRectangle(0, 53, 127, 63, White);
	ssd1306_Line(1, 0,  126, 0,  Black);
	ssd1306_Line(1, 63, 126, 63, Black);
	
	
	crypto_password_t* password;
	for(uint8_t i = 0; i < 8; i++){
		ssd1306_SetCursor(((i & 0x03) * 32) + 2, i >> 3);
		if(crypto_hotkey_password_get(i, &password) == CRYPTO_STATE_OK){
			char name[5];
			memcpy(name, password->short_name, 4);
			name[4] = 0;
			ssd1306_WriteString(name, Font_7x10, White);
		}
	}
	
	ssd1306_Line(31, 0, 31, 10, White);
	ssd1306_Line(63, 0, 63, 10, White);
	ssd1306_Line(95, 0, 95, 10, White);
	ssd1306_Line(31, 53, 31, 63, White);
	ssd1306_Line(63, 53, 63, 63, White);
	ssd1306_Line(95, 53, 95, 63, White);
}

ugl_menu_t *UI_main_menu_constructor( uint32_t ID, void* extra ){
	ugl_menu_t *mainMenu = ugl_menu_constructor(0);
	ugl_item_t *item = NULL;
	ugl_sprite_t *sprite = NULL;
	
	mainMenu->drawing_function = UI_mainMenuDraw;
	
	item = ugl_item_constructor(UI_MENU_MAIN_SETTINGS);
	ugl_item_set_sprite(item, ugl_sprite_constructor());
	ugl_sprite_set_bitmap(item->sprite, UI_ICON_ADDRESS_SETTINGS, MAIN_MENU_ICONS_WIDTH, MAIN_MENU_ICONS_HEIGHT);
	ugl_menu_add_item(mainMenu, item);
	
	item = ugl_item_constructor(UI_MENU_MAIN_PASSWORDS);
	ugl_item_set_sprite(item, ugl_sprite_constructor());
	ugl_sprite_set_bitmap(item->sprite, UI_ICON_ADDRESS_PASSWORDS, MAIN_MENU_ICONS_WIDTH, MAIN_MENU_ICONS_HEIGHT);
	ugl_menu_add_item(mainMenu, item);
	
	item = ugl_item_constructor(UI_MENU_MAIN_FOLDER);
	ugl_item_set_sprite(item, ugl_sprite_constructor());
	ugl_sprite_set_bitmap(item->sprite, UI_ICON_ADDRESS_FOLDER, MAIN_MENU_ICONS_WIDTH, MAIN_MENU_ICONS_HEIGHT);
	ugl_menu_add_item(mainMenu, item);
	
	item = ugl_item_constructor(UI_MENU_MAIN_LOCK);
	ugl_item_set_sprite(item, ugl_sprite_constructor());
	ugl_sprite_set_bitmap(item->sprite, UI_ICON_ADDRESS_LOCK, MAIN_MENU_ICONS_WIDTH, MAIN_MENU_ICONS_HEIGHT);
	ugl_menu_add_item(mainMenu, item);
	
	item = ugl_item_constructor(UI_MENU_MAIN_USB_WRITE);
	ugl_item_set_sprite(item, ugl_sprite_constructor());
	ugl_sprite_set_bitmap(item->sprite, UI_ICON_ADDRESS_USB_WRITE, MAIN_MENU_ICONS_WIDTH, MAIN_MENU_ICONS_HEIGHT);
	ugl_menu_add_item(mainMenu, item);
	
	
	ugl_item_set_position(ugl_menu_get_item_by_id(mainMenu, 0), 0,  0);
	ugl_item_set_position(ugl_menu_get_item_by_id(mainMenu, 1), 35, 0);
	ugl_item_set_position(ugl_menu_get_item_by_id(mainMenu, 2), 70, 0);
	ugl_item_set_position(ugl_menu_get_item_by_id(mainMenu, 3), 105, 0);
	ugl_item_set_position(ugl_menu_get_item_by_id(mainMenu, 4), 140, 0);
	
	ugl_item_set_nextprevious(\
		ugl_menu_get_item_by_id(mainMenu, 0),\
		ugl_menu_get_item_by_id(mainMenu, 1),\
		ugl_menu_get_item_by_id(mainMenu, 0));
	ugl_item_set_nextprevious(\
		ugl_menu_get_item_by_id(mainMenu, 1),\
		ugl_menu_get_item_by_id(mainMenu, 2),\
		ugl_menu_get_item_by_id(mainMenu, 0));
	ugl_item_set_nextprevious(\
		ugl_menu_get_item_by_id(mainMenu, 2),\
		ugl_menu_get_item_by_id(mainMenu, 3),\
		ugl_menu_get_item_by_id(mainMenu, 1));
	ugl_item_set_nextprevious(\
		ugl_menu_get_item_by_id(mainMenu, 3),\
		ugl_menu_get_item_by_id(mainMenu, 4),\
		ugl_menu_get_item_by_id(mainMenu, 2));
	ugl_item_set_nextprevious(\
		ugl_menu_get_item_by_id(mainMenu, 4),\
		ugl_menu_get_item_by_id(mainMenu, 4),\
		ugl_menu_get_item_by_id(mainMenu, 3));
	
	mainMenu->group->position_y = 17;
	mainMenu->group->position_x = -15;
	mainMenu->selected_item = ugl_menu_get_item_by_id(mainMenu, 3);
	
	return mainMenu;
}
