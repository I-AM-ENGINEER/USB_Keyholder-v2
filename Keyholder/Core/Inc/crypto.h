#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#define CRYPTO_FLASH_OFFSET					0xC000
#define CRYPTO_FLASH_SIZE						(0x0800FFFFUL - FLASH_BASE - CRYPTO_FLASH_OFFSET + 1)
#define CRYPTO_PAGE_OFFSET 					(CRYPTO_FLASH_OFFSET/PAGESIZE)
#define CRYPTO_PASSWORDS_COUNT_MAX	64
//((sizeof(crypto_db) + FLASH_PAGE_SIZE - 1)/FLASH_PAGE_SIZE)
#define CRYPTO_PAGE_PASSWORDS_OFFSET	(CRYPTO_PAGE_OFFSET + ((sizeof(crypto_database_t) + FLASH_PAGE_SIZE - 1)/FLASH_PAGE_SIZE))

//(FLASH_END - FLASH_BASE - CRYPTO_FLASH_OFFSET + 1)
//#define CRYPTO_PASSWORDS_COUNT_MAX		((CRYPTO_FLASH_SIZE/sizeof(CRYPTO_password_t))-1)

#include "main.h"
#include "list.h"


#define CRYPTO_HOTKEY_NUM						8

#define CRYPTO_MAX_LIST_SIZE sizeof(CRYPTO_ListNode_t)


void crypto_init( void );
	

typedef struct{
	char password[32];
	char login[32];
	char comment[60];
	char short_name[4];
} crypto_password_t;

typedef struct{
	uint16_t first_check;
	uint32_t chipper[8];
	list_t 	password_list;
	crypto_password_t *hotkey[8];
	list_node_t password_list_buf[CRYPTO_PASSWORDS_COUNT_MAX];
} crypto_database_t;

typedef enum{
	CRYPTO_STATE_OK,
	CRYPTO_STATE_ERROR,
} crypto_state_t;

void crypto_save(void);

// Return saved passwords count
uint16_t crypto_password_count( void );
// Return maximum posible passwords count
uint16_t crypto_password_count_max( void );

// Write to *password struct from flash memory
crypto_state_t crypto_password_get( crypto_password_t* password, uint16_t number );
// Get pointer to memory
crypto_state_t crypto_password_get_ptr( crypto_password_t**const password, uint16_t number );
// Write from *password struct to flash memory
crypto_state_t crypto_password_set( crypto_password_t* password, uint16_t number );
// Move passwords in list
crypto_state_t crypto_password_move( uint16_t password_from, uint16_t password_to );
// Swap 2 passwords in list
crypto_state_t crypto_password_swap( uint16_t password_a, uint16_t password_b );
// Create new password and add it to the end of list
crypto_state_t crypto_password_new( crypto_password_t* password );
// Delete password from list, not delete from flash memory
crypto_state_t crypto_password_remove( uint16_t number );


crypto_state_t crypto_hotkey_password_set( uint8_t hotkey, uint16_t password_number );
crypto_state_t crypto_hotkey_password_get( uint8_t hothey, crypto_password_t**const password );

#endif // __CRYPTO_H__
