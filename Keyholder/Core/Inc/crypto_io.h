#ifndef __CRYPTO_IO_H__
#define __CRYPTO_IO_H__

#include "main.h"


#define CRYPTO_CMD_PASSWORD_GET_COUNT			"PASSWORD_COUNT"
#define CRYPTO_CMD_PASSWORD_GET						"PASSWORD_GET_"
#define CRYPTO_CMD_PASSWORD_SET						"PASSWORD_SET_"						// PASSWORD_INSERT_idx:login \t password \t comment
#define CRYPTO_CMD_PASSWORD_REMOVE				"PASSWORD_REMOVE_"				// PASSWORD_MOVE_idx Remove with shift
#define CRYPTO_CMD_PASSWORD_APPEND				"PASSWORD_APPEND"				// PASSWORD_INSERT_idx	// create new password
#define CRYPTO_CMD_PASSWORD_INSERT				"PASSWORD_INSERT"				// PASSWORD_INSERT_idx	// create new password
#define CRYPTO_CMD_PASSWORD_MOVE					"PASSWORD_MOVE_"					// PASSWORD_MOVE:from:to
#define CRYPTO_CMD_PASSWORD_SWAP					"PASSWORD_SWAP_"					// PASSWORD_SWAP:firs:second

//#define CRYPTO_CMD_PASSWORD_NEW						"PASSWORD_NEW:"

#define CRYPTO_REPLY_GET_PASSWORD					"PASSWORD_"

#define CRYPTO_REPLY_PASSWORD_APPEND			"NEW_PASSWORD_IDX:"
#define CRYPTO_REPLY_OK										"OK"
#define CRYPTO_REPLY_ERROR								"ERROR"


char* crypto_io_cmd_parse( const char* cmd );

#endif // __CRYPTO_IO_H__
