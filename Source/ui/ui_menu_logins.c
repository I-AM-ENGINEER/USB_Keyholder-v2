#include "ui/core.h"
#include "crypto.h"

static ugl_menu_t *logins_menu = NULL;
static int32_t cursor_position = 0;
static int16_t password_id;
static bool move_mode = false;
static int8_t holded;

enum {
	UI_MENU_LOGINS_NEW_PASS = -2,
	UI_MENU_LOGINS_RETURN = -1,
} ui_menu_logins_e;

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

void UI_menu_logins_process( void ){
	static bool pressed = false;
	
	crypto_password_t* password;
	int16_t passwords_count = crypto_password_count();
	
	UI_event_button_t lastButton = UI_event_get_last();
	
	
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
			}
			break;
		case BUTTON_STATE_HOLDED:
			if(lastButton.button_id == BTN_JCW_ID){
				holded=-1;
				//cursor_position--;
				//ugl_menu_previous_item(ugl_get_current_menu());
			}else if(lastButton.button_id == BTN_JCCW_ID){
				holded=1;
				//cursor_position++;
				//ugl_menu_next_item(ugl_get_current_menu());
			}else if(lastButton.button_id == BTN_JPUSH_ID){
				pressed = false;
				password_id = ugl_get_current_menu()->selected_item->ID;
				if(password_id >= 0){
					ugl_enter(1, UI_menu_logins_submenu_constructor, &password_id);
				}
				return;
			}
			break;
		case BUTTON_STATE_RELEASED:
			holded = 0;
			if((lastButton.button_id == BTN_JPUSH_ID) && pressed){
				pressed = false;
				if((ugl_get_current_menu()->selected_item->ID >= 0) && (ugl_get_current_menu()->selected_item->ID < passwords_count)){
					crypto_password_get_ptr(&password, ugl_get_current_menu()->selected_item->ID);
					if(password != NULL){
						ugl_enter(1, UI_hotkey_menu_constructor, password);
						return;
					}
				}else if( ugl_get_current_menu()->selected_item->ID == UI_MENU_LOGINS_RETURN){
					ugl_return();
					return;
				}else if(ugl_get_current_menu()->selected_item->ID == UI_MENU_LOGINS_NEW_PASS){
					crypto_password_t new_password = {
						.comment = "",
						.login = "New login",
						.password = "",
						.short_name = "NEW",
					};
					crypto_password_new( &new_password );
				}
			}
			break;
		default: break;
	}
	
	ugl_item_t *item = NULL;
	UI_menu_logins_process_list();
	ugl_item_t *current_item = ugl_get_current_menu()->selected_item;

	// WHAT THE FUCK, I DONT KNOW HOW I WROTE THIS SHIT
	if(cursor_position >= passwords_count){
		cursor_position = passwords_count - 1;
	}
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
	}else if(current_item->ID == UI_MENU_LOGINS_NEW_PASS){ // New password
		cursor_position = 0;
	}else if(current_item->ID == UI_MENU_LOGINS_RETURN){ // First password
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

	if(current_item->position_y_abs == (cursor_position * UI_MENU_LOGINS_INTERVALS + 2)){
		if((holded==1) && (cursor_position != 5)){
			if(cursor_position != 5){
				ugl_menu_next_item(ugl_get_current_menu());
				cursor_position++;
			}
		}else if((holded==-1) && (cursor_position != 0)){
			cursor_position--;
			ugl_menu_previous_item(ugl_get_current_menu());
		}
		UI_event_start();
	}else if(holded==0){
		UI_event_stop();
	}

	// 123
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
}

void UI_menu_logins_render( void ){
	ssd1306_Fill(Black);
	
	ugl_menu_render(logins_menu);
	
	ssd1306_DrawRectangle(0,0,127,1, Black);
	ssd1306_DrawRectangle(0,62,127,63, Black);
	
	ssd1306_Line(0, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 3, 2, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 5, White);
	ssd1306_Line(2, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 6, 0, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 8, White);
}

ugl_menu_t *UI_menu_logins_constructor( int32_t ID, void* extra ){
	ugl_menu_t *mainMenu = ugl_menu_constructor(UI_MENU_ID_LOGINS);
	ugl_item_t *item = NULL;
	ugl_sprite_t *sprite = NULL;
	
	logins_menu = mainMenu;
	mainMenu->process_f = UI_menu_logins_process;
	mainMenu->render_f = UI_menu_logins_render;
	move_mode = false;

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

	return mainMenu;
}
