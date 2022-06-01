#include "crypto.h"
#include "stdio.h"
#include "string.h"
#include "main.h"

#define PAGE_START	200


dataType passwordDataBase[30];

void flash_data_grab( dataType *data, uint32_t dataNumber );
void flash_data_save( dataType* data, uint32_t dataNumber );
void flash_page_erase ( uint16_t pageNumber );
void flash_page_write ( uint32_t* page, uint16_t pageNumber );
void flash_page_read (uint32_t* page, uint16_t pageNumber );
uint32_t flash_get_passwords_count( void );


void fill_database ( void ){
	for(uint8_t i = 0 ; i < 30; i++){
		sprintf(passwordDataBase[i].login, "test_login%d",i);
		sprintf(passwordDataBase[i].password, "test_password%d",i);
		sprintf(passwordDataBase[i].comment, "test_commentary%d",i);
		flash_data_save(&passwordDataBase[i],i);
	} 
}

/**
  * @brief  read data struct from FLASH memory
	* @param  dataNumber: pointer of data to grab
	* @param  dataNumber: data number to read
  */
void flash_data_grab(dataType *data, uint32_t dataNumber ){
	uint32_t page[64];
	flash_page_read(page, PAGE_START + 1 + dataNumber);
	memcpy(data, page, 256);
}

/**
  * @brief  copy data struct to page and flash this page to FLASH memory
	* @param  data: pointer to data for save
	* @param  dataNumber: data number (for exclude override other data in flash)
  */
void flash_data_save( dataType* data, uint32_t dataNumber ){
	uint32_t page[64];
	//flash_page_read(page, PAGE_START + 1 + dataNumber);
	memcpy(page, data, 256);
	flash_page_write(page, PAGE_START + 1 + dataNumber);
}

/**
  * @brief  erase page of FLASH memory
	* @param  pageNumber: page number to erase
  */
void flash_page_erase ( uint16_t pageNumber ){
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
void flash_page_write ( uint32_t* page, uint16_t pageNumber ){
	flash_page_erase(pageNumber);
	uint32_t address = (uint32_t)(pageNumber * FLASH_PAGE_SIZE + FLASH_BASE);
	HAL_FLASH_Unlock();
	for(uint16_t i = 0; i < (FLASH_PAGE_SIZE / 4); i++){
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, page[i]);
		address += 4;
	}
	HAL_FLASH_Lock();
}

/**
  * @brief  Read page from FLASH memory
	* @param  page: pointer to array of words, which need fill
	* @param  pageNumber: page number to read
  */
void flash_page_read (uint32_t* page, uint16_t pageNumber ){
	uint32_t* address = (uint32_t*)(pageNumber * FLASH_PAGE_SIZE + FLASH_BASE);
	for(uint16_t i = 0; i < (FLASH_PAGE_SIZE / 4); i++){
		page[i] = address[i];
	}
}

/**
  * @brief  Write passwords count to FLASH memory
	* @param  count: new value of passwords count
  */
void flash_set_passwords_count( uint32_t count ){
	uint32_t page[64];
	flash_page_read(page, PAGE_START);
	page[0] = count;
	flash_page_erase(PAGE_START);
	flash_page_write(page, PAGE_START);
}

/**
  * @brief  Read passwords count from FLASH memory
  * @return passwords count
  */
inline uint32_t flash_get_passwords_count( void ){
	return *(uint32_t*)(FLASH_BASE + PAGE_START);
}

