#include "ui_core.h"
#include "crypto.h"

#define UI_MENU_LOGINS_PRINTED_COUNT 	6
#define UI_MENU_LOGINS_SUBMENU_COUNT  4
#define UI_MENU_LOGINS_FONT						Font_7x10
#define UI_MENU_LOGINS_INTERVALS			10

static ugl_menu_t *logins_menu = NULL;
static ugl_menu_t *logins_submenu = NULL;
static int32_t cursor_position = 0;
static int32_t submenu_cursor_position = 0;
static int16_t submenu_start_position = 128;
static bool submenu_exit = false;

void UI_menu_logins_render( void );
void UI_menu_logins_submenu_render( void );
ugl_menu_t *UI_menu_logins_submenu_constructor( int32_t ID, void* extra );
void UI_menu_logins_delete_warning_draw();
ugl_menu_t *UI_menu_logins_delete_warning_constructor( int32_t ID, void* extra );

void UI_menu_logins_process_list( void ){
	ugl_item_t *item = NULL;
	ugl_item_t *current_item = ugl_get_current_menu()->selected_item;
	if(current_item->previous_item == NULL){
		item = ugl_menu_get_item_by_id(ugl_get_current_menu(), current_item->ID + (UI_MENU_LOGINS_PRINTED_COUNT + 1));
		if(item == NULL){
			return;
		}
		item->ID = current_item->ID - 1;
		ugl_item_set_position(item, 4, current_item->position_y - UI_MENU_LOGINS_INTERVALS);
		current_item->previous_item = item;
		if(item->previous_item != NULL){
			item->previous_item->next_item = NULL;
		}
		item->next_item = current_item;
		item->previous_item = NULL;
	}else if(current_item->next_item == NULL){
		item = ugl_menu_get_item_by_id(ugl_get_current_menu(), current_item->ID - (UI_MENU_LOGINS_PRINTED_COUNT + 1));
		if(item == NULL){
			return;
		}
		item->ID = current_item->ID + 1;
		ugl_item_set_position(item, 4, current_item->position_y + UI_MENU_LOGINS_INTERVALS);
		current_item->next_item = item;
		if(item->next_item != NULL){
			item->next_item->previous_item = NULL;
		}
		item->next_item = NULL;
		item->previous_item = current_item;
	}
}

void UI_menu_logins_delete_warning_draw( void ){
	
	
	UI_event_button_t lastButton = UI_event_GetLast();
	UI_event_clear_last();
	
	switch(lastButton.event_type){
		case BUTTON_STATE_PRESSED:
			if(lastButton.button_id == BTN_JCW_ID){
				ugl_menu_previous_item(ugl_get_current_menu());
			}else if(lastButton.button_id == BTN_JCCW_ID){
				ugl_menu_next_item(ugl_get_current_menu());
			}else if(lastButton.button_id == BTN_JPUSH_ID){
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
	//ssd1306_SetCursor(17, 29);
	//ssd1306_WriteString("", Font_7x10, White);
	
	
	ugl_menu_render(ugl_get_current_menu());
	ssd1306_Line(ugl_get_current_menu()->selected_item->position_x_abs, ugl_get_current_menu()->selected_item->position_y_abs + 11, \
		ugl_get_current_menu()->selected_item->position_x_abs + 8, ugl_get_current_menu()->selected_item->position_y_abs + 11, White);
	
	
}

void UI_menu_logins_submenu_draw( void ){
	UI_event_button_t lastButton = UI_event_GetLast();
	UI_event_clear_last();
	
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
				if(submenu_cursor_position == 2){
					ugl_enter(1, UI_menu_logins_delete_warning_constructor, NULL);
				}
				pressed = false;
				//if((ugl_get_current_menu()->selected_item->ID >= 0) && (ugl_get_current_menu()->selected_item->ID < passwords_count)){
					//crypto_password_get_ptr(&password, ugl_get_current_menu()->selected_item->ID);
					//if(password != NULL){
					//	ugl_enter(1, UI_hotkey_menu_constructor, password);
					//	return;
					//}
				//}
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


void UI_menu_logins_draw( void ){
	static bool pressed = false;
	
	crypto_password_t* password;
	int16_t passwords_count = crypto_password_count();
	
	UI_event_button_t lastButton = UI_event_GetLast();
	UI_event_clear_last();
	
	
	switch(lastButton.event_type){
		case BUTTON_STATE_PRESSED:
			if(lastButton.button_id == BTN_JCW_ID){
				cursor_position--;
				ugl_menu_previous_item(ugl_get_current_menu());
			}else if(lastButton.button_id == BTN_JCCW_ID){
				cursor_position++;
				ugl_menu_next_item(ugl_get_current_menu());
			}else if(lastButton.button_id == BTN_JPUSH_ID){
				pressed = true;
			}else if((lastButton.button_id >= BTN_SW1_ID) && (lastButton.button_id <= BTN_SW8_ID)){
				
			}
			break;
		case BUTTON_STATE_HOLDED:
			pressed = false;
			ugl_enter(1, UI_menu_logins_submenu_constructor, password);
			return;
		
			break;
		case BUTTON_STATE_RELEASED:
			if((lastButton.button_id == BTN_JPUSH_ID) && pressed){
				pressed = false;
				if((ugl_get_current_menu()->selected_item->ID >= 0) && (ugl_get_current_menu()->selected_item->ID < passwords_count)){
					crypto_password_get_ptr(&password, ugl_get_current_menu()->selected_item->ID);
					if(password != NULL){
						ugl_enter(1, UI_hotkey_menu_constructor, password);
						return;
					}
				}else if( ugl_get_current_menu()->selected_item->ID == -1 ){
					ugl_return();
					return;
				}
			}
			break;
		default: break;
	}
	/*
	// Holded swipe
	if(((HAL_GetTick() - lastSwipeTimestamp) > 200) && ((holdedButton == BTN_JCW_ID) || (holdedButton == BTN_JCCW_ID))){
		if(holdedButton == BTN_JCW_ID) 	ugl_get_current_menu()->selected_item = ugl_get_current_menu()->selected_item->next_item;
		else 														ugl_get_current_menu()->selected_item = ugl_get_current_menu()->selected_item->previous_item;
		lastSwipeTimestamp = HAL_GetTick();
	}*/
	
	
	ugl_item_t *item = NULL;
	UI_menu_logins_process_list();
	ugl_item_t *current_item = ugl_get_current_menu()->selected_item;
	
	// WHAT THE FUCK, I DONT KNOW HOW I WROTE THIS SHIT
	// 
	if(current_item->ID < -2){ // To end of list
		if(passwords_count < UI_MENU_LOGINS_PRINTED_COUNT){
			cursor_position = passwords_count-1;
		}else{
			cursor_position = 5;
		}
		while(current_item->ID < (passwords_count - 1)){
			ugl_menu_next_item(ugl_get_current_menu());
			UI_menu_logins_process_list();
			current_item = ugl_get_current_menu()->selected_item;
		}
	}else if(current_item->ID == -2){ // New password
		cursor_position = 0;
	}else if(current_item->ID == -1){ // First password
		cursor_position = 0;
	}else if(current_item->ID == (passwords_count - 1)){ // Last password
		if(passwords_count < UI_MENU_LOGINS_PRINTED_COUNT){
			cursor_position = passwords_count - 1;
		}else{
			cursor_position = UI_MENU_LOGINS_PRINTED_COUNT - 1;
		}
	}else if(current_item->ID == passwords_count){ // New password
		if(passwords_count < UI_MENU_LOGINS_PRINTED_COUNT){
			ugl_menu_previous_item(ugl_get_current_menu());
			UI_menu_logins_process_list();
			current_item = ugl_get_current_menu()->selected_item;
			cursor_position = passwords_count - 1;
		}else{
			cursor_position = UI_MENU_LOGINS_PRINTED_COUNT - 1;
		}
	}else if(current_item->ID == (passwords_count + 1)){ // To begin list
		cursor_position = 0;
		while(current_item->ID > 0){
			ugl_menu_previous_item(ugl_get_current_menu());
			UI_menu_logins_process_list();
			current_item = ugl_get_current_menu()->selected_item;
		}
	}else{
		if(cursor_position == (UI_MENU_LOGINS_PRINTED_COUNT - 1)){
			cursor_position = UI_MENU_LOGINS_PRINTED_COUNT - 2;
		}else if(cursor_position == 0){
			cursor_position = 1;
		}
	}
	// 
	// Calculate positions
	if(abs(current_item->position_y_abs - (cursor_position * UI_MENU_LOGINS_INTERVALS + 2)) >= 20){
		ugl_get_current_menu()->group->position_y -= current_item->position_y_abs - (cursor_position * UI_MENU_LOGINS_INTERVALS + 2);
	}else if(current_item->position_y_abs < (cursor_position * UI_MENU_LOGINS_INTERVALS + 2)){
		ugl_get_current_menu()->group->position_y += 1;
	}else if(current_item->position_y_abs > (cursor_position * UI_MENU_LOGINS_INTERVALS + 2)){
		ugl_get_current_menu()->group->position_y -= 1;
	}
	// 
	// Naming
	item = current_item;
	for(uint8_t i = 0; i < (UI_MENU_LOGINS_PRINTED_COUNT + 2); i++){
		if(item->previous_item == NULL){
			for(int16_t i = 0; i < (UI_MENU_LOGINS_PRINTED_COUNT + 2); i++){
				if((item->ID >= 0) && (item->ID != passwords_count)){
					crypto_password_get_ptr(&password, item->ID);
					ugl_text_set_test(item->text, password->login, &UI_MENU_LOGINS_FONT);
				}else if((item->ID == -2) || (passwords_count >= (UI_MENU_LOGINS_PRINTED_COUNT-1))){
					ugl_text_set_test(item->text, " NEW PASSWORD", &UI_MENU_LOGINS_FONT);
				}else{
					ugl_text_set_test(item->text, " ", &UI_MENU_LOGINS_FONT);
				}
				if(item->ID == -1){
					ugl_text_set_test(item->text, "<---", &UI_MENU_LOGINS_FONT);
				}
				// Some bycicles
				if(passwords_count < UI_MENU_LOGINS_PRINTED_COUNT){
					if(item->ID >= passwords_count){
						ugl_text_set_test(item->text, " ", &UI_MENU_LOGINS_FONT);
					}
				}
				if(item->next_item == NULL){
					break;
				}
				item = item->next_item;
			}
			break;
		}
		item = item->previous_item;
	}
	
	// Render
	UI_menu_logins_render();
}

static void UI_menu_logins_render( void ){
	ssd1306_Fill(Black);
	
	ugl_menu_render(logins_menu);
	
	ssd1306_DrawRectangle(0,0,127,1, Black);
	ssd1306_DrawRectangle(0,62,127,63, Black);
	
	ssd1306_Line(0, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 3, 2, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 5, White);
	ssd1306_Line(2, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 6, 0, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 8, White);
}

ugl_menu_t *UI_menu_logins_constructor( int32_t ID, void* extra ){
	ugl_menu_t *mainMenu = ugl_menu_constructor(0);
	ugl_item_t *item = NULL;
	ugl_sprite_t *sprite = NULL;
	
	logins_menu = mainMenu;
	mainMenu->drawing_function = UI_menu_logins_draw;
	
	for(uint8_t i = 0; i < UI_MENU_LOGINS_PRINTED_COUNT + 2; i++){
		item = ugl_item_constructor(i-1);
		ugl_item_set_position(item, 4, 3+i*UI_MENU_LOGINS_INTERVALS);
		ugl_item_set_text(item, ugl_text_constructor());
		ugl_text_set_test(item->text, " ", &UI_MENU_LOGINS_FONT);
		ugl_menu_add_item(mainMenu, item);
	}
	
	for(uint8_t i = 1; i < UI_MENU_LOGINS_PRINTED_COUNT + 1; i++){
		ugl_item_set_nextprevious(\
			ugl_menu_get_item_by_id(mainMenu, i-1),\
			ugl_menu_get_item_by_id(mainMenu, i),\
			ugl_menu_get_item_by_id(mainMenu, i-2));
	}
	ugl_item_set_nextprevious(\
			ugl_menu_get_item_by_id(mainMenu, -1),\
			ugl_menu_get_item_by_id(mainMenu, 0),\
			NULL);
	ugl_item_set_nextprevious(\
			ugl_menu_get_item_by_id(mainMenu, UI_MENU_LOGINS_PRINTED_COUNT),\
			NULL,\
			ugl_menu_get_item_by_id(mainMenu, UI_MENU_LOGINS_PRINTED_COUNT-1));
	
	mainMenu->selected_item = ugl_menu_get_item_by_id(mainMenu, -1);
	
	//mainMenu->selected_item = ugl_menu_get_item_by_id(mainMenu, 0);
	return mainMenu;
}

static ugl_menu_t *UI_menu_logins_submenu_constructor( int32_t ID, void* extra ){
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

ugl_menu_t *UI_menu_logins_delete_warning_constructor( int32_t ID, void* extra ){
	ugl_menu_t *mainMenu = ugl_menu_constructor(0);
	ugl_item_t *item = NULL;
	
	item = ugl_item_constructor(0);
	ugl_item_set_position(item, 40, 40);
	ugl_item_set_text(item, ugl_text_constructor());
	ugl_text_set_test(item->text, "NO", &UI_MENU_LOGINS_FONT);
	ugl_menu_add_item(mainMenu, item);
	
	
	item = ugl_item_constructor(0);
	ugl_item_set_position(item, 60, 40);
	ugl_item_set_text(item, ugl_text_constructor());
	ugl_text_set_test(item->text, "YES", &UI_MENU_LOGINS_FONT);
	ugl_menu_add_item(mainMenu, item);
	
	ugl_item_set_nextprevious(\
			ugl_menu_get_item_by_id(mainMenu, 0),\
			ugl_menu_get_item_by_id(mainMenu, 1),\
			ugl_menu_get_item_by_id(mainMenu, 1));
	
	ugl_item_set_nextprevious(\
			ugl_menu_get_item_by_id(mainMenu, 1),\
			ugl_menu_get_item_by_id(mainMenu, 0),\
			ugl_menu_get_item_by_id(mainMenu, 1));
	
	mainMenu->selected_item = ugl_menu_get_item_by_id(mainMenu, 0);
	mainMenu->drawing_function = UI_menu_logins_delete_warning_draw;
	return mainMenu;
}
