#include "ui/core.h"
#include "crypto.h"


static int32_t submenu_cursor_position = 0;
static int16_t submenu_start_position = 128;
static bool submenu_exit = false;
static ugl_menu_t *logins_submenu = NULL;
static uint16_t* password_num;
static bool exit_on_next_run = false;

typedef enum{
	UI_LOGINS_SUBMENU_MOVE = 0,
	UI_LOGINS_SUBMENU_EDIT = 1,
	UI_LOGINS_SUBMENU_DELETE = 2,
	UI_LOGINS_SUBMENU_HOTKEY = 3,
} ui_logins_submenu_e;

void UI_menu_logins_submenu_draw( void ){
	UI_event_button_t lastButton = UI_event_GetLast();
	
	static bool pressed = false;
	
	switch(lastButton.event_type){
		case BUTTON_STATE_PRESSED:
			if(lastButton.button_id == BTN_JCW_ID){
				submenu_cursor_position--;
				ugl_menu_previous_item(ugl_get_current_menu());
			}else if(lastButton.button_id == BTN_JCCW_ID){
				submenu_cursor_position++;
				ugl_menu_next_item(ugl_get_current_menu());
			}else if(lastButton.button_id == BTN_JPUSH_ID){
				pressed = true;
			}else if((lastButton.button_id >= BTN_SW1_ID) && (lastButton.button_id <= BTN_SW8_ID)){
				
			}
			break;
		case BUTTON_STATE_HOLDED:
			submenu_exit = true;
			//return;
			//ugl_enter(1, UI_menu_logins_submenu_constructor, password);
			//return;
		
			break;
		case BUTTON_STATE_RELEASED:
			if((lastButton.button_id == BTN_JPUSH_ID) && pressed){
				switch(submenu_cursor_position){
					case UI_LOGINS_SUBMENU_DELETE:
						ugl_enter(1, UI_menu_logins_delete_warning_constructor, password_num);
                        submenu_exit = true;
                        pressed = false;
                        return;
						break;
					case UI_LOGINS_SUBMENU_MOVE:
						break;
					case UI_LOGINS_SUBMENU_HOTKEY:
                        ugl_enter(1, UI_menu_logins_submenu_hotkey_constructor, password_num);
						break;
					case UI_LOGINS_SUBMENU_EDIT:
						break;
					default:break;
				}
				pressed = false;
			}
			break;
		default: break;
	}
	
	if(submenu_cursor_position < 0){
		submenu_cursor_position = 0;
	}else if(submenu_cursor_position >= UI_MENU_LOGINS_SUBMENU_COUNT){
		submenu_cursor_position = UI_MENU_LOGINS_SUBMENU_COUNT - 1;
	}
	
	if(submenu_exit && (submenu_start_position < 128)){
		submenu_start_position += 10;
	}else if(submenu_exit){
		submenu_exit = false;
		submenu_cursor_position = 0;
		ugl_return();
		return;
	}else if(submenu_start_position > 60){
		submenu_start_position -= 10;
	}
	
	UI_menu_logins_submenu_render();
}

void UI_menu_logins_submenu_render( void ){
	UI_menu_logins_render();
	logins_submenu->group->position_x = submenu_start_position;
	
	for(uint16_t i = 0; i < 32; i++){
		ssd1306_DrawRectangle(submenu_start_position - 2,i,128,63-i, Black);
	}
	ssd1306_DrawRectangle(submenu_start_position - 2,0, 128, 63, White);
	ssd1306_Line(logins_submenu->group->position_x, (submenu_cursor_position * UI_MENU_LOGINS_INTERVALS) + 3, logins_submenu->group->position_x + 2, (submenu_cursor_position * UI_MENU_LOGINS_INTERVALS) + 5, White);
	ssd1306_Line(logins_submenu->group->position_x + 2, (submenu_cursor_position * UI_MENU_LOGINS_INTERVALS) + 6, logins_submenu->group->position_x, (submenu_cursor_position * UI_MENU_LOGINS_INTERVALS) + 8, White);
	ugl_menu_render(logins_submenu);
}

ugl_menu_t *UI_menu_logins_submenu_constructor( int32_t ID, void* extra ){
    exit_on_next_run = false;
    if(extra == NULL){
        return NULL;
    }
    
    password_num = (uint16_t*)extra;

	ugl_menu_t *mainMenu = ugl_menu_constructor(0);
	logins_submenu = mainMenu;
	ugl_item_t *item = NULL;
	

	for(uint8_t i = 0; i < UI_MENU_LOGINS_SUBMENU_COUNT; i++){
		item = ugl_item_constructor(i);
		ugl_item_set_position(item, 4, 3+i*UI_MENU_LOGINS_INTERVALS);
		ugl_item_set_text(item, ugl_text_constructor());
		ugl_menu_add_item(mainMenu, item);
	}
	
	for(uint8_t i = 1; i < UI_MENU_LOGINS_SUBMENU_COUNT - 1; i++){
		ugl_item_set_nextprevious(\
			ugl_menu_get_item_by_id(mainMenu, i),\
			ugl_menu_get_item_by_id(mainMenu, i+1),\
			ugl_menu_get_item_by_id(mainMenu, i-1));
	}
	ugl_item_set_nextprevious(\
			ugl_menu_get_item_by_id(mainMenu, 0),\
			ugl_menu_get_item_by_id(mainMenu, 1),\
			NULL);
	ugl_item_set_nextprevious(\
			ugl_menu_get_item_by_id(mainMenu, UI_MENU_LOGINS_SUBMENU_COUNT - 1),\
			NULL,\
			ugl_menu_get_item_by_id(mainMenu, UI_MENU_LOGINS_SUBMENU_COUNT - 2));
	
	ugl_text_set_test(ugl_menu_get_item_by_id(mainMenu, 0)->text, "move", &UI_MENU_LOGINS_FONT);
	ugl_text_set_test(ugl_menu_get_item_by_id(mainMenu, 1)->text, "edit", &UI_MENU_LOGINS_FONT);
	ugl_text_set_test(ugl_menu_get_item_by_id(mainMenu, 2)->text, "delete", &UI_MENU_LOGINS_FONT);
	ugl_text_set_test(ugl_menu_get_item_by_id(mainMenu, 3)->text, "hotkey", &UI_MENU_LOGINS_FONT);
	
	
	mainMenu->drawing_function = UI_menu_logins_submenu_draw;
	return mainMenu;
}
