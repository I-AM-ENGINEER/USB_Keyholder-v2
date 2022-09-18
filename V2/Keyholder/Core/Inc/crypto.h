#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include <stddef.h>
#include "stm32l1xx_hal.h"

/* -------------------------------------------------------------------------- */	

typedef struct {
	char login[64];
	char password[64];
	char comment[128];
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
