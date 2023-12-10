#include "crypto.h"
#include "flashd.h"
#include <stdlib.h>
#include <time.h>

uint8_t page_buffer[FLASH_PAGE_SIZE];

crypto_database_t crypto_db;

static const char allowed_chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!\"#$%&()*+,-./;<=>?@[\\]^_`{|}~";
static const char special_chars[] = "!\"#$%&()*+,-./;<=>?@[\\]^_`{|}~";

static char generate_random_char( void ) {
    char r = allowed_chars[rand() % (sizeof(allowed_chars) - 1)];
    return r;
}

void crypto_generate_password(uint8_t length, char* output){
	for(uint8_t i = 0; i < length; i++){
		output[i] = generate_random_char();
	}
	output[length] = '\0';
}

static char generate_random_number( void ){
	char r = rand() % 10 + '0';
	return r; 
}

static char generate_random_alphabet( void ){
	uint8_t a = rand() % 52;
	char r = a%26 + (a>=26?'a':'A');
	return r; 
}

static char generate_random_special( void ){
	uint8_t a = rand()%sizeof(special_chars);
	char r = special_chars[a];
	return r;
}

void crypto_generate_password_adv(uint8_t length, char* output, bool special_symbols, bool numbers, bool alphabet){
	if(output == NULL){
		return;
	}
	if(length == 0){
		return;
	}
	if(!special_symbols && !numbers && !alphabet){
		output[0] = '\0';
		return;
	}
	for(uint8_t i = 0; i < length;){
		uint8_t a = rand()%(10+52+sizeof(special_chars));
		if(a < 10){
			if(numbers){
				output[i++] = generate_random_number();
			}
		}else if(a < 62){
			if(alphabet){
				output[i++] = generate_random_alphabet();
			}
		}else{
			if(special_symbols){
				output[i++] = generate_random_special();
			}
		}
	}
	output[length] = '\0';
}

void crypto_save( void ) {
    uint8_t* struct_buffer = (uint8_t*)&crypto_db;
    uint32_t struct_offset = 0;
    uint32_t bytes_remaining = sizeof(crypto_database_t);
    uint32_t bytes_to_write;
	uint32_t page_id = CRYPTO_PAGE_OFFSET;
	
    while (bytes_remaining > 0) {
        bytes_to_write = (bytes_remaining > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : bytes_remaining;
        memcpy(page_buffer, &struct_buffer[struct_offset], bytes_to_write);
        flashd_page_write(page_buffer, page_id);
        struct_offset += bytes_to_write;
        bytes_remaining -= bytes_to_write;
        page_id++;
    }
}

void crypto_load(void) {
    uint8_t page_buffer[FLASH_PAGE_SIZE];
    uint8_t* struct_buffer = (uint8_t*)&crypto_db;
    uint32_t struct_offset = 0;
    uint32_t bytes_remaining = sizeof(crypto_database_t);
    uint32_t bytes_to_read;
    uint32_t page_id = CRYPTO_PAGE_OFFSET;
	
    while (bytes_remaining > 0) {
        bytes_to_read = (bytes_remaining > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : bytes_remaining;
        flashd_page_read(page_buffer, page_id);
        memcpy(&struct_buffer[struct_offset], page_buffer, bytes_to_read);
        struct_offset += bytes_to_read;
        bytes_remaining -= bytes_to_read;
        page_id++;
    }
}

void crypto_init( void ){
	srand(NULL);
	crypto_load();
	if(crypto_db.first_check != 0xAAAA){
		memset(&crypto_db, 0, sizeof(crypto_database_t));
		crypto_db.first_check = 0xAAAA;
		list_init(&crypto_db.password_list, crypto_db.password_list_buf, sizeof(crypto_db.password_list_buf)/sizeof(list_node_t));
		crypto_save();
	}
	crypto_db.password_list.nodes = crypto_db.password_list_buf;
}

uint16_t crypto_password_count( void ){
	return list_get_count(&crypto_db.password_list);
}

uint16_t crypto_password_count_max( void ){
	return list_get_count_max(&crypto_db.password_list);
}

void crypto_password_write( crypto_password_t* password, uint16_t ID ){
	flashd_page_read(page_buffer, CRYPTO_PAGE_PASSWORDS_OFFSET + (ID >> 1));
	memcpy(&page_buffer[((ID & 0x01) << 7)], password, sizeof(crypto_password_t));
	flashd_page_write(page_buffer, CRYPTO_PAGE_PASSWORDS_OFFSET + (ID >> 1));
}

void crypto_password_read( crypto_password_t* password, uint16_t ID ){
	flashd_page_read(page_buffer, CRYPTO_PAGE_PASSWORDS_OFFSET + (ID >> 1));
	memcpy(password, &page_buffer[((ID & 0x01) << 7)], sizeof(crypto_password_t));
}

void crypto_get_password_ptr( crypto_password_t** password, int32_t ID ){
	*password = (crypto_password_t*)( ID * sizeof(crypto_password_t) + FLASH_BASE + CRYPTO_PAGE_PASSWORDS_OFFSET * FLASH_PAGE_SIZE );
}

crypto_state_t crypto_password_get( crypto_password_t* password, uint16_t number ){
	uint32_t node_ID;
	
	if(password == NULL){
		return CRYPTO_STATE_ERROR;
	}
	
	if(list_get_node_data(&crypto_db.password_list, number, (void*)&node_ID) != LIST_STATE_OK){
		return CRYPTO_STATE_ERROR;
	}
	crypto_password_read(password, node_ID);
	return CRYPTO_STATE_OK;
}

crypto_state_t crypto_password_get_ptr( crypto_password_t**const password, uint16_t number ){
	uint32_t node_ID;
	
	if(password == NULL){
		return CRYPTO_STATE_ERROR;
	}
	
	if(number >= CRYPTO_PASSWORDS_COUNT_MAX){
		return CRYPTO_STATE_ERROR;
	}
	
	if(list_get_node_data(&crypto_db.password_list, number, (void*)&node_ID) != LIST_STATE_OK){
		return CRYPTO_STATE_ERROR;
	}
	crypto_get_password_ptr( password, node_ID );
	return CRYPTO_STATE_OK;
}

crypto_state_t crypto_password_set( crypto_password_t* password, uint16_t number ){
	uint32_t node_ID;
	
	if(password == NULL){
		return CRYPTO_STATE_ERROR;
	}
	
	if(list_get_node_data(&crypto_db.password_list, number, (void*)&node_ID) != LIST_STATE_OK){
		return CRYPTO_STATE_ERROR;
	}
	password->comment[sizeof(password->comment)-1] = 0;
	password->login[sizeof(password->login)-1] = 0;
	password->password[sizeof(password->password)-1] = 0;
	crypto_password_write(password, node_ID);
	return CRYPTO_STATE_OK;
}

crypto_state_t crypto_password_remove( uint16_t number ){
	crypto_password_t* password;
	uint32_t node_ID;
	
	if(list_get_node_data(&crypto_db.password_list, number, (void*)&node_ID) != LIST_STATE_OK){
		return CRYPTO_STATE_ERROR;
	}
	
	crypto_get_password_ptr(&password, node_ID);
	
	for(uint8_t i = 0; i < CRYPTO_HOTKEY_NUM; i++){
		if(password == crypto_db.hotkey[i]){
			crypto_db.hotkey[i] = NULL;
		}
	}
	
	if(list_remove(&crypto_db.password_list, number) != LIST_STATE_OK) {
		return CRYPTO_STATE_ERROR;
	}
	return CRYPTO_STATE_OK;
}

crypto_state_t crypto_password_move( uint16_t password_from, uint16_t password_to ){
	if(list_move(&crypto_db.password_list, password_from, password_to) != LIST_STATE_OK) {
		return CRYPTO_STATE_ERROR;
	}
	return CRYPTO_STATE_OK;
}

crypto_state_t crypto_password_swap( uint16_t password_a, uint16_t password_b ){
	if(list_swap(&crypto_db.password_list, password_a, password_b) != LIST_STATE_OK){
		return CRYPTO_STATE_ERROR;
	}
	return CRYPTO_STATE_OK;
}

crypto_state_t crypto_password_new( crypto_password_t* password ){
	if(list_get_count(&crypto_db.password_list) == list_get_count_max(&crypto_db.password_list)){
		return CRYPTO_STATE_ERROR;
	}
	if(password == NULL) {
		return CRYPTO_STATE_ERROR;
	}
	
	uint8_t searchArray[(CRYPTO_PASSWORDS_COUNT_MAX + 7) / 8];
	memset(searchArray, 0, sizeof(searchArray));
	
	for(uint8_t i = 0; i < list_get_count(&crypto_db.password_list); i++){
		uint32_t data;
		list_get_node_data(&crypto_db.password_list, i, (void*)&data);
		searchArray[data >> 3] |= (1 << (data & 0x07));
	}
	
	for(uint32_t i = 0; i < (CRYPTO_PASSWORDS_COUNT_MAX + 7)/8; i++){
		if(searchArray[i] != 0xFF){
			for (int j = 0; j < 8; j++) {
				if (!(searchArray[i] & (1 << j))) {
					uint32_t num = i * 8 + j;
					list_append(&crypto_db.password_list);
					list_set_head_node_data(&crypto_db.password_list, (void*)num);
					crypto_password_write(password, num);
					return CRYPTO_STATE_OK;
				}
			}
		}
	}
	return CRYPTO_STATE_ERROR;
}

crypto_state_t crypto_hotkey_password_get( uint8_t hothey, crypto_password_t**const password ){
	if(hothey >= CRYPTO_HOTKEY_NUM){
		return CRYPTO_STATE_ERROR;
	}
	if(crypto_db.hotkey[hothey] == NULL){
		return CRYPTO_STATE_ERROR;
	}
	if(password == NULL){
		return CRYPTO_STATE_ERROR;
	}
	
	//crypto_db.hotkey[hothey]
	*password = crypto_db.hotkey[hothey];
	//memcpy(password, crypto_db.hotkey[hothey], sizeof(crypto_password_t));
	//password = 
	//memcpy(password, crypto_db.hotkey[hothey]);
	return CRYPTO_STATE_OK;
}

crypto_state_t crypto_hotkey_password_set( uint8_t hotkey, int16_t password_number ){
	uint32_t node_ID;
	if(hotkey >= CRYPTO_HOTKEY_NUM){
		return CRYPTO_STATE_ERROR;
	}
	if(password_number < 0){
		crypto_db.hotkey[hotkey] = NULL;
		return CRYPTO_STATE_OK;
	}
	if(list_get_node_data(&crypto_db.password_list, password_number, (void*)&node_ID) != LIST_STATE_OK){
		return CRYPTO_STATE_ERROR;
	}
	crypto_password_t* password;
	crypto_get_password_ptr(&password, node_ID);
	if(password == NULL){
		return CRYPTO_STATE_ERROR;
	}
	crypto_db.hotkey[hotkey] = password;
	return CRYPTO_STATE_OK;
}
