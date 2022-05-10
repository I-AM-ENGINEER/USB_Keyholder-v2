#include "crypto.h"
#include "stdio.h"
#include "main.h"
dataType passwordDataBase[30];

void fill_database (void){
	for(uint8_t i = 0 ; i < 10; i++){
		sprintf(passwordDataBase[i].login, "ddd%d",i);
		sprintf(passwordDataBase[i].password, "pass%d",i);
		sprintf(passwordDataBase[i].comment, "com%d",i);
	} 
}

