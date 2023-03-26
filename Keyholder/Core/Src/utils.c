#include "utils.h"

static const char  *cmdcmp_str = NULL;

void utils_cmd_setstr( const char* cmd ){
	cmdcmp_str = cmd;
}

bool utils_cmdcmp( const char* cmd ){
	if(cmdcmp_str == NULL) return false;
	return !memcmp(cmdcmp_str, cmd, strlen(cmd));
}
