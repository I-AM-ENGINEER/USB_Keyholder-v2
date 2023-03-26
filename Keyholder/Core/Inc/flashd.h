#ifndef __FLASHD_H__
#define __FLASHD_H__

#include "main.h"

/* -------------------------------------------------------------------------- */

//void flashd_read_mem( void* flash_ptr, void* data, uint32_t size );
//void flashd_write_mem( void* flash_ptr, const void* data, uint32_t size );
void flashd_page_erase ( uint16_t pageNumber );
void flashd_page_write ( void* page, uint16_t pageNumber );
void flashd_page_read  ( void* page, uint16_t pageNumber );
void* flashd_get_page_address ( uint16_t pageNumber );
//flash_page_t* flashd_get_page_address ( uint16_t pageNumber );

/* -------------------------------------------------------------------------- */

#endif // __FLASHD_H__
