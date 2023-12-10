#include "ui/core.h"
#include "crypto.h"

enum edit_menu_select_e {
	UI_EDIT_SELECT_RETURN,
	UI_EDIT_SELECT_LOGIN,
	UI_EDIT_SELECT_HOTKEY,
	UI_EDIT_SELECT_COMMENTARY,
	UI_EDIT_SELECT_PASSWORD,
	UI_EDIT_SELECT_REGENERATE_PASSWORD,
} ;

static uint16_t password_num;
static enum edit_menu_select_e cursor_position = UI_EDIT_SELECT_RETURN;
static UI_edit_menu_set_t* edit_property;

static void __enter_in_edit_menu(const char* commentary, const char* editable_string, uint8_t editable_string_len, bool zero_ended){
	edit_property = (UI_edit_menu_set_t*)malloc(sizeof(UI_edit_menu_set_t));
	if(edit_property != NULL){
		edit_property->comment_string = commentary;
		edit_property->string_max_length = editable_string_len;
		edit_property->editable_string = (char*)malloc(edit_property->string_max_length);
		edit_property->zero_ended = zero_ended;
		if(edit_property->editable_string != NULL){
			memcpy(edit_property->editable_string, editable_string, edit_property->string_max_length);
			ugl_enter(0, UI_menu_editor_constructor, (void*)edit_property);
		}else{
			free(edit_property);
		}
	}
}

void UI_menu_logins_submenu_edit_process( void ){
	UI_event_button_t lastButton = UI_event_get_last();
	if(edit_property != NULL){
		crypto_password_t password;
		crypto_password_get(&password, password_num);
		char* changed_ptr;
		switch (cursor_position){
		case UI_EDIT_SELECT_LOGIN:
			changed_ptr = password.login;
			break;
		case UI_EDIT_SELECT_HOTKEY:
			changed_ptr = password.short_name;
			break;
		case UI_EDIT_SELECT_COMMENTARY:
			changed_ptr = password.comment;
			break;
		case UI_EDIT_SELECT_PASSWORD:
			changed_ptr = password.password;
			break;
		default: break;
		}
		memcpy(changed_ptr, edit_property->editable_string, edit_property->string_max_length);
		crypto_password_set(&password, password_num);

		free(edit_property->editable_string);
		free(edit_property);
		edit_property = NULL;
	}

	switch(lastButton.event_type){
		case BUTTON_STATE_PRESSED:
			if(lastButton.button_id == BTN_JCW_ID){
				if(cursor_position == 0){
					cursor_position = 5;
				}else{
					cursor_position--;
				}
			}else if(lastButton.button_id == BTN_JCCW_ID){
				if(cursor_position == 5){
					cursor_position = 0;
				}else{
					cursor_position++;
				}
			}else if(lastButton.button_id == BTN_JPUSH_ID){
				crypto_password_t *password;
				crypto_password_get_ptr(&password, password_num);
				switch(cursor_position){
					case UI_EDIT_SELECT_RETURN:
						ugl_return();
						break;
					case UI_EDIT_SELECT_LOGIN:
						__enter_in_edit_menu("edit login", password->login, CRYPTO_LOGIN_MAX_LEN, true);
						break;
					case UI_EDIT_SELECT_HOTKEY:
						__enter_in_edit_menu("edit hotkey", password->short_name, CRYPTO_SHORT_NAME_LEN, false);
						break;
					case UI_EDIT_SELECT_COMMENTARY:
						__enter_in_edit_menu("edit comment", password->comment, CRYPTO_COMMENT_MAX_LEN, true);
						break;
					case UI_EDIT_SELECT_PASSWORD:
						__enter_in_edit_menu("edit pass", password->password, CRYPTO_PASSWORD_MAX_LEN, true);
						break;
					case UI_EDIT_SELECT_REGENERATE_PASSWORD:
						ugl_enter(0, UI_menu_logins_submenu_edit_generate_password_construct, (void*)&password_num);
						cursor_position = UI_EDIT_SELECT_RETURN;
						break;
					default: break;
				}
			}
			break;
		default: break;
	}
}

void UI_menu_logins_submenu_edit_render( void ){
	ssd1306_Fill(Black);
	crypto_password_t *password;
    crypto_password_get_ptr(&password, password_num);
	ssd1306_SetCursor(4, 2);
    ssd1306_WriteString("<--", Font_7x10, White);
	ssd1306_SetCursor(40, 2);
    ssd1306_WriteString("what change?", Font_7x10, White);
	ssd1306_SetCursor(4, 12);
    ssd1306_WriteString("logn ", Font_7x10, White);
	ssd1306_WriteString(password->login, Font_7x10, White);
	ssd1306_SetCursor(4, 22);
    ssd1306_WriteString("hotk ", Font_7x10, White);
	for(uint32_t i = 0; i < CRYPTO_SHORT_NAME_LEN; i++){
		ssd1306_WriteChar(password->short_name[i],Font_7x10, White);
	}
	ssd1306_SetCursor(4, 32);
    ssd1306_WriteString("comm ", Font_7x10, White);
	ssd1306_WriteString(password->comment, Font_7x10, White);
	ssd1306_SetCursor(4, 42);
    ssd1306_WriteString("pass ***********", Font_7x10, White);
	ssd1306_SetCursor(4, 52);
    ssd1306_WriteString("regenerate pass", Font_7x10, White);
	
	ssd1306_Line(0, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 3, 2, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 5, White);
	ssd1306_Line(2, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 6, 0, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 8, White);
}

ugl_menu_t *UI_menu_logins_submenu_edit_constructor( int32_t ID, void* extra ){
	if(extra == NULL){
		return NULL;
	}
	ugl_menu_t *mainMenu = ugl_menu_constructor(UI_MENU_ID_LOGINS_SUBMENU_EDIT);
	ugl_item_t *item = NULL;
    password_num = *(uint16_t*)extra;
	cursor_position = 0;
	mainMenu->process_f = UI_menu_logins_submenu_edit_process;
    mainMenu->render_f = UI_menu_logins_submenu_edit_render;
	return mainMenu;
}
