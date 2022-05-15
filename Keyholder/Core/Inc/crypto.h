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
extern dataType passwordDataBase[30];
extern dataType passwordDataBaseHot[8];
/* -------------------------------------------------------------------------- */
void fill_database (void);

/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */

#endif // __CRYPTO_H__
