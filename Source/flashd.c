#include "flashd.h"

//uint32_t FLASHD_pageBuffer[PAGESIZE/4];

void flashd_read_mem( void* flash_ptr, void* data, uint32_t size ){
	
}

void flashd_write_mem( void* flash_ptr, const void* data, uint32_t size ){
	
}

void flashd_page_erase ( uint16_t pageNumber ){
	static FLASH_EraseInitTypeDef EraseInitStruct;
	
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = (uint32_t)(pageNumber * FLASH_PAGE_SIZE + FLASH_BASE);
  EraseInitStruct.NbPages = 1;
  uint32_t page_error = 0;
	
  HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&EraseInitStruct, &page_error);
	HAL_FLASH_Lock();
}

/**
  * @brief  flash page to FLASH memory (256 byte for STM32L151), 
	* @param  page: pointer to page of FLASH (array of 64 x 4 byte words) to write
	* @param  pageNumber: page number to write
  */
void flashd_page_write ( void* page, uint16_t pageNumber ){
	uint32_t* page_ptr = page;
	flashd_page_erase(pageNumber);
	uint32_t* address = (uint32_t*)(pageNumber * FLASH_PAGE_SIZE + FLASH_BASE);
	HAL_FLASH_Unlock();
	for(uint32_t i = 0; i < (FLASH_PAGE_SIZE / 4); i++){
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)address, page_ptr[i]);
		address += 1;
	}
	HAL_FLASH_Lock();
}

/**
  * @brief  Read page from FLASH memory
	* @param  page: pointer to array of words, which need fill
	* @param  pageNumber: page number to read
  */
void flashd_page_read ( void* page, uint16_t pageNumber ){
	uint32_t* page_ptr = page;
	uint32_t* address = (uint32_t*)(pageNumber * FLASH_PAGE_SIZE + FLASH_BASE);
	for(uint32_t i = 0; i < (FLASH_PAGE_SIZE / 4); i++){
		page_ptr[i] = address[i];
	}
}


void* flashd_get_page_address ( uint16_t pageNumber ){
	return (void*)(pageNumber * FLASH_PAGE_SIZE + FLASH_BASE);
}
