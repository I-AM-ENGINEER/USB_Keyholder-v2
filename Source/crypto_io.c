#include "crypto_io.h"
#include "crypto.h"
//#include "list.h"
#include "utils.h"

crypto_password_t passwordBuffer;
char out_buffer[64];

static char* crypto_io_cmd_save( void );
static char* crypto_io_cmd_set_password( const char* cmd );
static char* crypto_io_cmd_get_password( const char* cmd );
static char* crypto_io_cmd_password_append( void );
static char* crypto_io_cmd_get_password_count( void );
static char* crypto_io_cmd_password_remove( const char* cmd );
static char* crypto_io_cmd_password_move( const char* cmd );
static char* crypto_io_cmd_password_swap( const char* cmd );
static char* crypto_io_cmd_password_insert( const char* cmd );
static char* crypto_io_cmd_hotkey_password_set( const char* cmd );

char* crypto_io_cmd_parse( const char* cmd ){
	utils_cmd_setstr( cmd );
	
	if (utils_cmdcmp(CRYPTO_CMD_PASSWORD_SET))
		return crypto_io_cmd_set_password(&cmd[strlen(CRYPTO_CMD_PASSWORD_SET)]);
	if (utils_cmdcmp(CRYPTO_CMD_PASSWORD_GET))
		return crypto_io_cmd_get_password(&cmd[strlen(CRYPTO_CMD_PASSWORD_GET)]);
	if (utils_cmdcmp(CRYPTO_CMD_PASSWORD_GET_COUNT))
		return crypto_io_cmd_get_password_count();
	if (utils_cmdcmp(CRYPTO_CMD_PASSWORD_REMOVE))
		return crypto_io_cmd_password_remove(&cmd[strlen(CRYPTO_CMD_PASSWORD_REMOVE)]);
	if (utils_cmdcmp(CRYPTO_CMD_PASSWORD_INSERT))
		return crypto_io_cmd_password_insert(&cmd[strlen(CRYPTO_CMD_PASSWORD_INSERT)]);
	//if (utils_cmdcmp(CRYPTO_CMD_PASSWORD_APPEND))
	//	return crypto_io_cmd_password_append();
	if (utils_cmdcmp(CRYPTO_CMD_PASSWORD_MOVE))
		return crypto_io_cmd_password_move(&cmd[strlen(CRYPTO_CMD_PASSWORD_MOVE)]);
	if (utils_cmdcmp(CRYPTO_CMD_PASSWORD_SWAP))
		return crypto_io_cmd_password_swap(&cmd[strlen(CRYPTO_CMD_PASSWORD_SWAP)]);
	if (utils_cmdcmp(CRYPTO_CMD_HOTKEY_PASSWORD_SET))
		return crypto_io_cmd_hotkey_password_set(&cmd[strlen(CRYPTO_CMD_HOTKEY_PASSWORD_SET)]);
	if (utils_cmdcmp(CRYPTO_CMD_SAVE))
		return crypto_io_cmd_save();
	return CRYPTO_REPLY_INVALID_COMMAND;
}

static char* crypto_io_cmd_save( void ){
	crypto_save();
	return CRYPTO_REPLY_OK;
}

static char* crypto_io_cmd_get_password_count( void ){
	sprintf(out_buffer, "count: %d\r\n", crypto_password_count());
	return out_buffer;
}

static char* crypto_io_cmd_password_remove( const char* cmd ){
	uint16_t password_number = atoi(cmd);
	if(crypto_password_remove( password_number ) != CRYPTO_STATE_OK){
		return CRYPTO_REPLY_ERROR;
	}
	return CRYPTO_REPLY_OK;
}

static char* crypto_io_cmd_password_move( const char* cmd ){
	const char* first_part = cmd;
	if(first_part  == NULL){
		return CRYPTO_REPLY_ERROR;
	}
	const char* second_part = strchr(cmd, ':') + 1;
	if(second_part == NULL){
		return CRYPTO_REPLY_ERROR;
	}
	uint16_t password_number_from = atoi(first_part);
	uint16_t password_number_to 	= atoi(second_part);
	
	if(crypto_password_move(password_number_from, password_number_to) != CRYPTO_STATE_OK){
		return CRYPTO_REPLY_ERROR;
	}
	
	return CRYPTO_REPLY_OK;
}

static char* crypto_io_cmd_password_swap( const char* cmd ){
	const char* pass_a = cmd;
	if(pass_a  == NULL){
		return CRYPTO_REPLY_ERROR;
	}
	const char* pass_b = strchr(cmd, ':') + 1;
	if(pass_b == NULL){
		return CRYPTO_REPLY_ERROR;
	}
	uint16_t password_a = atoi(pass_a);
	uint16_t password_b 	= atoi(pass_b);
	
	if(crypto_password_swap(password_a, password_b) != CRYPTO_STATE_OK){//list_move(&password_list, password_number_from, password_number_to) != LIST_STATE_OK) {
		return CRYPTO_REPLY_ERROR;
	}
	
	return CRYPTO_REPLY_OK;
}

static char* crypto_io_cmd_password_insert( const char* cmd ){
	if(cmd == NULL) return CRYPTO_REPLY_ERROR;
	uint16_t password_number = atoi(cmd);
	if(password_number > crypto_password_count()){
		password_number = crypto_password_count();
	}
	memset(&passwordBuffer, 0, sizeof(passwordBuffer));
	strcpy(passwordBuffer.login, "New login");
	strcpy(passwordBuffer.short_name, "NEW");
	crypto_password_new(&passwordBuffer);
	crypto_password_move(crypto_password_count() - 1, password_number);
	sprintf(out_buffer, "%s%d", CRYPTO_REPLY_PASSWORD_APPEND, password_number);
	return out_buffer;
}

static char* crypto_io_cmd_password_append( void ){
	if(crypto_password_count() >= crypto_password_count_max()){
		return CRYPTO_REPLY_ERROR;
	}
	
	crypto_password_new(&passwordBuffer);
	sprintf(out_buffer, "%s%d", CRYPTO_REPLY_PASSWORD_APPEND, crypto_password_count() - 1);
	return out_buffer;
}

static char* crypto_io_cmd_set_password( const char* cmd ){
	static volatile uint32_t data;
	
	// Cmd check
	if(cmd 			== NULL){
		return CRYPTO_REPLY_ERROR;
	}
	// Login check
	const char* login 		= strchr(cmd, ':');
	if(login++ 		== NULL){
		return CRYPTO_REPLY_ERROR;
	}
	// Short login check
	const char* slogin 		= strchr(login, ':');
	if(slogin++ 	== NULL){
		return CRYPTO_REPLY_ERROR;
	}
	// Password check
	const char* password 	= strchr(slogin, ':');
	if(password++ == NULL){
		return CRYPTO_REPLY_ERROR;
	}
	// Comment check
	char* comment 	= strchr(password, ':');
	if(comment++  == NULL){
		return CRYPTO_REPLY_ERROR;
	}
	
	uint16_t password_number = atoi(cmd);
	
	if(crypto_password_count() <= password_number){
		return CRYPTO_REPLY_ERROR;
	}
	
	uint8_t login_length = slogin - login - 1;
	uint8_t slogin_length = password - slogin - 1;
	uint8_t password_length = comment - password - 1;
	uint8_t comment_length = strlen(comment) - 1;
	
	if(login_length >= sizeof(passwordBuffer.login)){
		login_length = sizeof(passwordBuffer.login) - 1;
	}
	
	if(slogin_length >= sizeof(passwordBuffer.short_name)){
		slogin_length = sizeof(passwordBuffer.short_name) - 1;
	}
	
	if(password_length >= sizeof(passwordBuffer.password)){
		password_length = sizeof(passwordBuffer.password) - 1;
	}
	
	if(comment_length >= sizeof(passwordBuffer.comment)){
		comment_length = sizeof(passwordBuffer.comment) - 1;
	}
	
	memset(&passwordBuffer, 0, sizeof(passwordBuffer));
	strncpy(passwordBuffer.login, login, login_length);
	strncpy(passwordBuffer.short_name, slogin, slogin_length);
	strncpy(passwordBuffer.password, password, password_length);
	strncpy(passwordBuffer.comment, comment, comment_length);
	passwordBuffer.comment[comment_length] = '\0';
	crypto_password_set(&passwordBuffer, password_number);
	
	return CRYPTO_REPLY_OK;
}

static char* crypto_io_cmd_get_password( const char* cmd ){
	if(cmd == NULL){
		return CRYPTO_REPLY_ERROR;
	}
	
	uint16_t password_number = atoi(cmd);
	
	if(crypto_password_get(&passwordBuffer, password_number) != CRYPTO_STATE_OK){
		return CRYPTO_REPLY_ERROR;
	}
	
	sprintf(out_buffer, "%s%d:%s:%s:%s:%s", CRYPTO_REPLY_GET_PASSWORD, password_number, passwordBuffer.login, passwordBuffer.short_name, passwordBuffer.password, passwordBuffer.comment);
	return out_buffer;
}

static char* crypto_io_cmd_hotkey_password_set( const char* cmd ){
	if(cmd == NULL){
		return CRYPTO_REPLY_ERROR;
	}
	// Login check
	const char* password_number = strchr(cmd, ':') + 1;
	if(password_number == NULL){
		return CRYPTO_REPLY_ERROR;
	}
	
	if(crypto_hotkey_password_set(atoi(cmd), atoi(password_number)) != CRYPTO_STATE_OK){
		return CRYPTO_REPLY_ERROR;
	}
	return CRYPTO_REPLY_OK;
}



