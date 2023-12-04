#ifndef __COMD_H__
#define __COMD_H__

#include "main.h"

#define COMD_RX_BUFFER_SIZE	256
#define COMD_TX_BUFFER_SIZE	256

#define COMD_MAX_CALLBACK_COUNT		8
#define COMD_MAX_PREFIX_LENGTH		8

typedef struct {
    char* (*callback)(const char*);
    char prefix[COMD_MAX_PREFIX_LENGTH];
} receive_callback_t;

void comd_send_IT( const uint8_t *data, uint16_t size );
void comd_send_str_IT( const char *str );
void comd_set_tx_function( void (*send)( const uint8_t *tranmitted_data, uint16_t size) );
void comd_add_receive_callback( char* (*callback)(const char*), char* prefix );	// Prefix - start of string, if prefix equall, call callback
void comd_receive_IRQ( uint8_t *received_data, uint16_t size );		// This function call when receive block of data from USB, start process when receive \r\n bytes

#endif // __COMD_H__
