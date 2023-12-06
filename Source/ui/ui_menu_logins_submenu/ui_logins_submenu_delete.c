#include "ui/core.h"
#include "crypto.h"

static uint16_t password_num;

typedef enum{
    ITEM_YES_ID,
    ITEM_NO_ID,
} item_id;

void UI_menu_logins_delete_warning_draw( void ){
	UI_event_button_t lastButton = UI_event_GetLast();
	
	switch(lastButton.event_type){
		case BUTTON_STATE_PRESSED:
			if(lastButton.button_id == BTN_JCW_ID){
				ugl_menu_previous_item(ugl_get_current_menu());
			}else if(lastButton.button_id == BTN_JCCW_ID){
				ugl_menu_next_item(ugl_get_current_menu());
			}else if(lastButton.button_id == BTN_JPUSH_ID){
                if(ugl_get_current_menu()->selected_item->ID == ITEM_YES_ID){
                    crypto_password_remove(password_num);
                    crypto_save();
                }
				ugl_return();
				return;
			}
			break;
		default: break;
	}
	
	UI_menu_logins_submenu_render();
	
	for(uint16_t i = 0; i < 22; i++){
		ssd1306_DrawRectangle(16,i+10,111,53-i, Black);
	}
	ssd1306_DrawRectangle(16,10, 111, 53, White);
	
	ssd1306_SetCursor(28, 18);
	ssd1306_WriteString("DELETE?", Font_11x18, White);	
	
	ugl_menu_render(ugl_get_current_menu());
	ssd1306_Line(ugl_get_current_menu()->selected_item->position_x_abs, ugl_get_current_menu()->selected_item->position_y_abs + 11, \
		ugl_get_current_menu()->selected_item->position_x_abs + 8, ugl_get_current_menu()->selected_item->position_y_abs + 11, White);
}

ugl_menu_t *UI_menu_logins_delete_warning_constructor( int32_t ID, void* extra ){
	ugl_menu_t *mainMenu = ugl_menu_constructor(0);
	ugl_item_t *item = NULL;
    password_num = *(uint16_t*)extra;
	
	item = ugl_item_constructor(ITEM_NO_ID);
	ugl_item_set_position(item, 40, 40);
	ugl_item_set_text(item, ugl_text_constructor());
	ugl_text_set_test(item->text, "NO", &UI_MENU_LOGINS_FONT);
	ugl_menu_add_item(mainMenu, item);
	
	item = ugl_item_constructor(ITEM_YES_ID);
	ugl_item_set_position(item, 60, 40);
	ugl_item_set_text(item, ugl_text_constructor());
	ugl_text_set_test(item->text, "YES", &UI_MENU_LOGINS_FONT);
	ugl_menu_add_item(mainMenu, item);
	
	ugl_item_set_nextprevious(\
			ugl_menu_get_item_by_id(mainMenu, ITEM_NO_ID),\
			ugl_menu_get_item_by_id(mainMenu, ITEM_YES_ID),\
			ugl_menu_get_item_by_id(mainMenu, ITEM_YES_ID));
	
	ugl_item_set_nextprevious(\
			ugl_menu_get_item_by_id(mainMenu, ITEM_YES_ID),\
			ugl_menu_get_item_by_id(mainMenu, ITEM_NO_ID),\
			ugl_menu_get_item_by_id(mainMenu, ITEM_NO_ID));
	
	mainMenu->selected_item = ugl_menu_get_item_by_id(mainMenu, ITEM_NO_ID);
	mainMenu->drawing_function = UI_menu_logins_delete_warning_draw;
	return mainMenu;
}
