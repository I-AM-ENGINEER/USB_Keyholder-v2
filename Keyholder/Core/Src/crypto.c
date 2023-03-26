#include "crypto.h"
#include "flashd.h"

uint8_t page_buffer[FLASH_PAGE_SIZE];

crypto_database_t crypto_db;

void crypto_save( void ) {
    uint8_t* struct_buffer = (uint8_t*)&crypto_db;
    uint32_t struct_offset = 0;
    uint32_t bytes_remaining = sizeof(crypto_database_t);
    uint32_t bytes_to_write;
		uint32_t page_id = CRYPTO_PAGE_OFFSET;
	
    // Write the structure to flash memory in chunks of FLASH_PAGE_SIZE bytes
    while (bytes_remaining > 0) {
        bytes_to_write = (bytes_remaining > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : bytes_remaining;

        // Copy the next chunk of data from the structure buffer to the page buffer
        memcpy(page_buffer, &struct_buffer[struct_offset], bytes_to_write);

        // Write the page buffer to flash memory
        flashd_page_write(page_buffer, page_id);

        // Update the offsets and remaining bytes
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
	
    // Read the structure from flash memory in chunks of FLASH_PAGE_SIZE bytes
    while (bytes_remaining > 0) {
        bytes_to_read = (bytes_remaining > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : bytes_remaining;

        // Read the page buffer from flash memory
        flashd_page_read(page_buffer, page_id);

        // Copy the next chunk of data from the page buffer to the structure buffer
        memcpy(&struct_buffer[struct_offset], page_buffer, bytes_to_read);

        // Update the offsets and remaining bytes
        struct_offset += bytes_to_read;
        bytes_remaining -= bytes_to_read;
        page_id++;
    }
}

void crypto_init( void ){
	crypto_load();
	if(crypto_db.first_check != 0x55AA){
		crypto_db.first_check = 0x55AA;
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

crypto_password_t* crypto_get_passwdord_ptr( uint16_t ID ){
	return (crypto_password_t*)(ID * sizeof(crypto_password_t) + CRYPTO_FLASH_OFFSET + FLASH_BASE);
}

crypto_state_t crypto_password_get( crypto_password_t* password, uint16_t number ){
	static volatile uint32_t node_ID;
	if(list_get_node_data(&crypto_db.password_list, number, (void*)&node_ID) != LIST_STATE_OK){
		return CRYPTO_STATE_ERROR;
	}
	crypto_password_read(password, node_ID);
	return CRYPTO_STATE_OK;
}

crypto_state_t crypto_password_set( crypto_password_t* password, uint16_t number ){
	static volatile uint32_t node_ID;
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
	uint8_t searchArray[(CRYPTO_PASSWORDS_COUNT_MAX + 7) / 8];
	
	if(list_get_count(&crypto_db.password_list) == list_get_count_max(&crypto_db.password_list)){
		return CRYPTO_STATE_ERROR;
	}
	
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
