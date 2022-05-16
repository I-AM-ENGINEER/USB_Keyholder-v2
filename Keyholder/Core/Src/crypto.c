#include "crypto.h"
#include "stdio.h"
#include "main.h"
dataType passwordDataBase[30];
dataType passwordDataBaseHot[8] = {
	{"test1", "password1", ""},
	{"test2", "password2", ""},
	{"test3", "password3", ""},
	{"test4", "cfyzXE1cjcb", ""},
	{"test5", "password5", ""},
	{"test6", "password6", ""},
	{"test7", "password7", ""},
	{"test8", "password8", ""}
};

void fill_database (void){
	for(uint8_t i = 0 ; i < 10; i++){
		sprintf(passwordDataBase[i].login, "ddd%d",i);
		sprintf(passwordDataBase[i].password, "pass%d",i);
		sprintf(passwordDataBase[i].comment, "com%d",i);
	} 
}
