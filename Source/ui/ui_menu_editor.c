#include "ui/core.h"
#include "crypto.h"

enum {
	UI_EDITOR_SELECT_RETURN,
	UI_EDITOR_SELECT_CLEAR,
	UI_EDITOR_SELECT_EDITOR,
} menu_editor_select_e;

static bool edit_mode = false;
static int8_t key_selected = -1;
static int8_t char_selected = -1;
static int8_t cursor_position = 0;
static int8_t str_cursor_position = 0;
static UI_edit_menu_set_t edit_property;

static const char char_set[8][4] = {
	{'a', 'b', 'c', ' '},
	{'d', 'e', 'f', '#'},
	{'g', 'h', 'i', '^'},
	{'j', 'k', 'l', '<'},
	{'m', 'n', 'o', 'p'},
	{'q', 'r', 's', '.'},
	{'t', 'u', 'v', ','},
	{'w', 'x', 'y', 'z'},
};

static const char char_set_large[8][4] = {
	{'A', 'B', 'C', ' '},
	{'D', 'E', 'F', '#'},
	{'G', 'H', 'I', '^'},
	{'J', 'K', 'L', '<'},
	{'M', 'N', 'O', 'P'},
	{'Q', 'R', 'S', '.'},
	{'T', 'U', 'V', ','},
	{'W', 'X', 'Y', 'Z'},
};

static const char char_set_ext[8][4] = {
	{'1', '2', '3', ' '},
	{'4', '5', '6', '#'},
	{'7', '8', '9', '^'},
	{'0', '!', '?', '<'},
	{'@', '#', '$', '&'},
	{'/', '*', '-', '+'},
	{'(', ')', '\"', '\''},
	{'_', '%', '^', '~'},
};

static const char char_set_ext2[8][4] = {
    {'1', '2', '3', ' '},
	{'4', '5', '6', '#'},
	{'7', '8', '9', '^'},
	{'0', '=', '|', '<'},
	{'{', '}', '`', ' '},
	{';', '\\', ' ', ' '},
	{' ', ' ', ' ', ' '},
	{' ', ' ', ' ', ' '},
};

void UI_menu_editor_process( void ){
	static uint32_t last_pressed_timestamp;
	UI_event_button_t lastButton = UI_event_get_last();
	switch(lastButton.event_type){
		case BUTTON_STATE_PRESSED:
			if(lastButton.button_id == BTN_JCW_ID){
				if(!edit_mode){
					if(cursor_position == 0){
						cursor_position = 2;
					}else{
						cursor_position--;
					}
				}else{
					str_cursor_position--;
				}
			}else if(lastButton.button_id == BTN_JCCW_ID){
				if(!edit_mode){
					if(cursor_position == 2){
						cursor_position = 0;
					}else{
						cursor_position++;
					}
				}else{
					str_cursor_position++;
				}
			}else if(lastButton.button_id == BTN_JPUSH_ID){
				switch(cursor_position){
					case UI_EDITOR_SELECT_RETURN:
						ugl_return();
						break;
					case UI_EDITOR_SELECT_CLEAR:
						memset(edit_property.editable_string, 0, edit_property.string_max_length);
						break;
					case UI_EDITOR_SELECT_EDITOR:
						edit_mode = !edit_mode;
						break;
					default: break;
				}
			}else if ((lastButton.button_id >= BTN_SW1_ID) && (lastButton.button_id <= BTN_SW8_ID)){
				last_pressed_timestamp = HAL_GetTick();
				if((lastButton.button_id - BTN_SW1_ID) == key_selected){
					char_selected++;
					if(char_selected == 4){
						char_selected = 0;
					}
				}else{
					char_selected = 0;
					key_selected = lastButton.button_id - BTN_SW1_ID;
				}
			}
			break;
		case BUTTON_STATE_HOLDED:
			if(lastButton.button_id == BTN_JPUSH_ID){
				if(edit_mode){
					edit_mode = false;
				}
			}

		default: break;
	}

	if(str_cursor_position < 0){
		if(edit_property.zero_ended){
			str_cursor_position = (int8_t)strlen(edit_property.editable_string)-1;
		}else{
			str_cursor_position = edit_property.string_max_length-1;
		}
	}else{
		if(edit_property.zero_ended && (str_cursor_position >= strlen(edit_property.editable_string))){
			str_cursor_position = 0;
		}else if(!edit_property.zero_ended && (str_cursor_position >= edit_property.string_max_length)){
			str_cursor_position = 0;
		}
	}

	if((last_pressed_timestamp + 600) < HAL_GetTick()){
		if((key_selected >= 0) && (char_selected >= 0)){
			edit_property.editable_string[str_cursor_position] = char_set[key_selected][char_selected];
		}
		char_selected = -1;
		key_selected = -1;
	}
}

void UI_menu_editor_render( void ){
	ssd1306_Fill(Black);

	ssd1306_DrawRectangle(1, 0,  127, 10, White);
	ssd1306_DrawRectangle(1, 53, 127, 63, White);
	ssd1306_Line(1, 0,  126, 0,  Black);
	ssd1306_Line(1, 63, 126, 63, Black);
	
	ssd1306_Line(32, 0, 32, 10, White);
	ssd1306_Line(64, 0, 64, 10, White);
	ssd1306_Line(96, 0, 96, 10, White);
	ssd1306_Line(32, 53, 32, 63, White);
	ssd1306_Line(64, 53, 64, 63, White);
	ssd1306_Line(96, 53, 96, 63, White);

	
	for(uint32_t i = 0; i < 8; i++){
		ssd1306_SetCursor(2+(i%4)*32, (i/4)*54);
		for(uint32_t j = 0; j < 4; j++){
			if((key_selected == i) && (char_selected == j)){
				ssd1306_WriteChar(char_set[i][j], Font_7x10, Black);
			}else{
				ssd1306_WriteChar(char_set[i][j], Font_7x10, White);
			}
		}
	}
	ssd1306_SetCursor(40, 12);
	ssd1306_WriteString(edit_property.comment_string, Font_7x10, White);
	ssd1306_SetCursor(4, 12);
	ssd1306_WriteString("<--", Font_7x10, White);
	ssd1306_SetCursor(4, 22);
	ssd1306_WriteString("clear", Font_7x10, White);
	ssd1306_SetCursor(4, 34);

	for(uint32_t i = 0; i < edit_property.string_max_length; i++){
		char print_char = edit_property.editable_string[i];
		if(edit_property.editable_string[i] == '\0'){
			if(edit_property.zero_ended){
				break;
			}else{
				print_char = ' ';
			}
		}
		if(i == str_cursor_position){
			ssd1306_WriteChar(print_char, Font_7x10, Black);
		}else{
			ssd1306_WriteChar(print_char, Font_7x10, White);
		}
	}
	ssd1306_WriteChar('!', Font_7x10, White);
	if(!edit_mode){
		ssd1306_Line(0, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 3 + 11, 2, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 5 + 11, White);
		ssd1306_Line(2, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 6 + 11, 0, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 8 + 11, White);
	}
}

ugl_menu_t *UI_menu_editor_constructor( int32_t ID, void* extra ){
    if(extra == NULL){
        return NULL;
    }
	UI_edit_menu_set_t* given_props = (UI_edit_menu_set_t*)extra;
	if(given_props->editable_string == NULL){
		return NULL;
	}
	if(given_props->string_max_length == 0){
		return NULL;
	}
	ugl_menu_t *mainMenu = ugl_menu_constructor(UI_MENU_ID_LOGINS_SUBMENU_EDIT);
	memcpy((void*)&edit_property, given_props, sizeof(UI_edit_menu_set_t));
	cursor_position = UI_EDITOR_SELECT_RETURN;
	char_selected = -1;
	key_selected = -1;
	edit_mode = false;
	mainMenu->process_f = UI_menu_editor_process;
    mainMenu->render_f = UI_menu_editor_render;
	return mainMenu;
}
