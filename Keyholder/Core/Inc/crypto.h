#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include <stddef.h>
#include "stm32l1xx_hal.h"

/* -------------------------------------------------------------------------- */	

typedef struct {
	char login[32];
	char password[32];
	char comment[64];
} dataType; 

/* -------------------------------------------------------------------------- */

extern dataType passwordDataBaseHot[8];

/* -------------------------------------------------------------------------- */

//void fill_database (void);
void flash_data_grab(dataType *data, uint32_t dataNumber );
void flash_data_save( dataType* data, uint32_t dataNumber );

uint32_t flash_get_passwords_count( void );

/* -------------------------------------------------------------------------- */

#endif // __CRYPTO_H__
