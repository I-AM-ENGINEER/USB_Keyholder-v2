#include "comd.h"

static receive_callback_t receive_callbacks[COMD_MAX_CALLBACK_COUNT];
static uint8_t receive_callback_count = 0;

static uint8_t rx_buffer[COMD_RX_BUFFER_SIZE];
static uint8_t tx_buffer[COMD_TX_BUFFER_SIZE];


static void (*tx_send)( const uint8_t *transmitted_data, uint16_t size);

void comd_send_IT( const uint8_t *data, uint16_t size ) {
    if (tx_send != NULL) {
        tx_send(data, size);
    }
}

void comd_send_str_IT( const char *str ) {
	strcpy((char*)tx_buffer, str);
	strcat((char*)tx_buffer, "\r\n");
  comd_send_IT((uint8_t*)str, strlen(str));
}

void comd_set_tx_function(void (*send)( const uint8_t *transmitted_data, uint16_t size)) {
    tx_send = send;
}

void comd_add_receive_callback(char* (*callback)(const char*), char* prefix) {
    if (receive_callback_count >= COMD_MAX_CALLBACK_COUNT) {
        return;
    }

    receive_callbacks[receive_callback_count].callback = callback;
    strncpy(receive_callbacks[receive_callback_count].prefix, prefix, COMD_MAX_PREFIX_LENGTH - 1);
    receive_callbacks[receive_callback_count].prefix[COMD_MAX_PREFIX_LENGTH - 1] = '\0';
    receive_callback_count++;
}

void comd_receive_IRQ(uint8_t *received_data, uint16_t size) {
    static char buffer[256]; // static buffer to store received data
    static uint16_t buffer_index = 0; // index of the next available position in the buffer
    
    // process received data
    for (uint16_t i = 0; i < size; i++) {
        buffer[buffer_index++] = received_data[i]; // add received byte to the buffer
				if(buffer_index == 255){
					
					
					buffer[255] = 0;
					for (uint8_t j = 0; j < receive_callback_count; j++) {
                // iterate through all registered callbacks and call them if their prefix matches
                if (strncmp(buffer, receive_callbacks[j].prefix, strlen(receive_callbacks[j].prefix)) == 0) {
                    comd_send_str_IT(receive_callbacks[j].callback(&buffer[strlen(receive_callbacks[j].prefix)]));
                }
            }
					
					
					buffer_index = 0;
					return;
				}
        if ((received_data[i] == '\n') && (buffer[buffer_index-2] == '\r')) {
            // if we received a complete line ending with \r\n, process it
            buffer[buffer_index-2] = '\0'; // replace \r with null terminator to create a C string
            for (uint8_t j = 0; j < receive_callback_count; j++) {
                // iterate through all registered callbacks and call them if their prefix matches
                if (strncmp(buffer, receive_callbacks[j].prefix, strlen(receive_callbacks[j].prefix)) == 0) {
                    comd_send_str_IT(receive_callbacks[j].callback(&buffer[strlen(receive_callbacks[j].prefix)]));
                }
            }
            buffer_index = 0; // reset buffer index for next line
        }
        if (buffer_index == sizeof(buffer)) {
            // if the buffer is full, discard the data and start over
            buffer_index = 0;
        }
    }
}
					
					
					