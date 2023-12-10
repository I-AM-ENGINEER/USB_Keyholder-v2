#include "ui/core.h"
#include "crypto.h"


enum new_pass_select_e {
	UI_GEN_PASS_SELECT_RETURN,
	UI_GEN_PASS_SELECT_GENERATE,
	UI_GEN_PASS_SELECT_PASS_LENGTH,
	UI_GEN_PASS_SELECT_USE_SPECIAL_SYMBOLS,
	UI_GEN_PASS_SELECT_USE_NUMERIC,
	UI_GEN_PASS_SELECT_USE_ALPHABET,
	
	UI_GEN_PASS_MESSAGE_GENERATE_OK = 100,
	UI_GEN_PASS_MESSAGE_GENERATE_ERROR = 101,
};

static int8_t password_len_select = -1;
static bool use_special_symbols = true;
static bool use_numbers = true;
static bool use_alphabet = true;
static uint8_t password_length;
static uint16_t password_num;
static enum new_pass_select_e cursor_position = UI_GEN_PASS_SELECT_RETURN;

void UI_menu_logins_submenu_edit_generate_password_process( void ){
	UI_event_button_t lastButton = UI_event_get_last();
	if((cursor_position == UI_GEN_PASS_MESSAGE_GENERATE_OK) || (cursor_position == UI_GEN_PASS_MESSAGE_GENERATE_ERROR)){
		if(lastButton.event_type == BUTTON_STATE_PRESSED){
			ugl_return();
		}
		return;
	}
	switch(lastButton.event_type){
		case BUTTON_STATE_PRESSED:
			if(lastButton.button_id == BTN_JCW_ID){
				if(password_len_select < 0){
					if(cursor_position == 0){
						cursor_position = 5;
					}else{
						cursor_position--;
					}
				}else if(password_len_select == 0){
					password_length+=10;
				}else{
					password_length++;
				}
			}else if(lastButton.button_id == BTN_JCCW_ID){
				if(password_len_select < 0){
					if(cursor_position == 5){
						cursor_position = 0;
					}else{
						cursor_position++;
					}
				}else if(password_len_select == 0){
					password_length-=10;
				}else{
					password_length--;
				}
			}else if(lastButton.button_id == BTN_JPUSH_ID){
				switch(cursor_position){
					case UI_GEN_PASS_SELECT_RETURN:
						ugl_return();
						break;
					case UI_GEN_PASS_SELECT_USE_SPECIAL_SYMBOLS:
						use_special_symbols = !use_special_symbols;
						break;
					case UI_GEN_PASS_SELECT_USE_NUMERIC:
						use_numbers = !use_numbers;
						break;
					case UI_GEN_PASS_SELECT_USE_ALPHABET:
						use_alphabet = !use_alphabet;
						break;
					case UI_GEN_PASS_SELECT_PASS_LENGTH:
						password_len_select++;
						break;
					case UI_GEN_PASS_SELECT_GENERATE:
						{
							if(!use_special_symbols && !use_numbers && !use_alphabet){
								cursor_position = UI_GEN_PASS_MESSAGE_GENERATE_ERROR;
								return;
							}
							crypto_password_t new_password;
							crypto_password_get(&new_password, password_num);
							memset(new_password.password, 0, CRYPTO_PASSWORD_MAX_LEN);
							crypto_generate_password_adv(password_length, new_password.password, use_special_symbols, use_numbers, use_alphabet);
							crypto_password_set(&new_password, password_num);
							cursor_position = UI_GEN_PASS_MESSAGE_GENERATE_OK;
							return;
						}
						break;
					default: break;
				}
			}
			break;
		default: break;
	}
	if(password_len_select == 2){
		password_len_select = -1;
	}
	if(password_length <= 0){
		password_length = 1;
	}else if(password_length >= CRYPTO_PASSWORD_MAX_LEN){
		password_length = CRYPTO_PASSWORD_MAX_LEN - 1;
	}
}

void UI_menu_logins_submenu_edit_generate_password_render( void ){
	ssd1306_Fill(Black);
	ssd1306_SetCursor(4, 2);
    ssd1306_WriteString("<--", Font_7x10, White);
	ssd1306_SetCursor(40, 2);
    ssd1306_WriteString("pass options", Font_7x10, White);
	ssd1306_SetCursor(4, 12);
    ssd1306_WriteString("regenerate pass", Font_7x10, White);
	ssd1306_SetCursor(4, 22);
    ssd1306_WriteString("length: ", Font_7x10, White);
	
	if(password_len_select == 0){
		ssd1306_WriteChar((password_length/10)+'0', Font_7x10, Black);
	}else{
		ssd1306_WriteChar((password_length/10)+'0', Font_7x10, White);
	}
	if(password_len_select == 1){
		ssd1306_WriteChar((password_length%10)+'0', Font_7x10, Black);
	}else{
		ssd1306_WriteChar((password_length%10)+'0', Font_7x10, White);
	}

	ssd1306_SetCursor(13, 32);
    ssd1306_WriteString("special symbols", Font_7x10, White);
	ssd1306_SetCursor(13, 42);
    ssd1306_WriteString("numbers ", Font_7x10, White);
	ssd1306_SetCursor(13, 52);
    ssd1306_WriteString("alphabet", Font_7x10, White);
	
	for(uint32_t i = 0; i < 3; i++){
		ssd1306_DrawRectangle(5, i*10+33, 11, i*10+33+6, White);
	}
	const uint32_t buffer = 0xFFFFFFFF;
	if(use_special_symbols){
		ssd1306_DrawBitmap(7, 35, (const uint8_t*)&buffer, 3, 3, White);
	}
	if(use_numbers){
		ssd1306_DrawBitmap(7, 35+10, (const uint8_t*)&buffer, 3, 3, White);
	}
	if(use_alphabet){
		ssd1306_DrawBitmap(7, 35+20, (const uint8_t*)&buffer, 3, 3, White);
	}
	if(password_len_select < 0){
		ssd1306_Line(0, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 3, 2, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 5, White);
		ssd1306_Line(2, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 6, 0, (cursor_position * UI_MENU_LOGINS_INTERVALS) + 8, White);
	}

	if((cursor_position == UI_GEN_PASS_MESSAGE_GENERATE_OK) || (cursor_position == UI_GEN_PASS_MESSAGE_GENERATE_ERROR)){
		for(uint16_t i = 0; i < 22; i++){
			ssd1306_DrawRectangle(6,i+10,121,53-i, Black);
		}
		ssd1306_DrawRectangle(6,10, 121, 53, White);
		
		ssd1306_SetCursor(8, 18);
		if(cursor_position == UI_GEN_PASS_MESSAGE_GENERATE_OK){
			ssd1306_WriteString("New password:", Font_7x10, White);
			ssd1306_SetCursor(8, 32);
			crypto_password_t *password;
			crypto_password_get_ptr(&password, password_num);
			ssd1306_WriteString(password->password, Font_7x10, White);
		}else{
			ssd1306_WriteString("Are you idiot?", Font_7x10, White);
		}
	}
}

ugl_menu_t *UI_menu_logins_submenu_edit_generate_password_construct( int32_t ID, void* extra ){
	if(extra == NULL){
		return NULL;
	}
	ugl_menu_t *mainMenu = ugl_menu_constructor(UI_MENU_ID_LOGINS_SUBMENU_EDIT);
	ugl_item_t *item = NULL;
    password_num = *(uint16_t*)extra;
	cursor_position = 0;
	password_length = 16;
	mainMenu->process_f = UI_menu_logins_submenu_edit_generate_password_process;
    mainMenu->render_f = UI_menu_logins_submenu_edit_generate_password_render;
	return mainMenu;
}
