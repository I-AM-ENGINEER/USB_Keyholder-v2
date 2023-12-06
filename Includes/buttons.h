#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include "main.h"

#define BTN_DEFAULT_PRESS_TIME			20
#define BTN_DEFAULT_PRESS_LONG_TIME	400
#define BTN_DEFAULT_RELEASE_TIME		50

#define BTN_MAX_NAME_LENGTH					8
#define BTN_MAX_BUTTON_COUNT				16

typedef enum {
	BUTTON_STATE_IDLE,
	BUTTON_STATE_PRESSING,
	BUTTON_STATE_PRESSED,
	BUTTON_STATE_HOLDED,
	BUTTON_STATE_RELEASING,
	BUTTON_STATE_RELEASED,
} BTN_button_state_t;

typedef struct{
	GPIO_TypeDef *GPIO;
	uint32_t			pin;
	bool (*get_state_f) ( GPIO_TypeDef *, uint32_t );
} BTN_push_button_hw_t;

typedef struct{
	BTN_push_button_hw_t hw;
	BTN_button_state_t state;
	BTN_button_state_t state_old;
	uint32_t timestamp;
	//char buttonChar[BTN_MAX_NAME_LENGTH];
	int32_t ID;
	bool inverted;
	void (*callback) ( uint32_t button_id, BTN_button_state_t state );
} BTN_push_button_t;


BTN_push_button_t* BTN_add_button( uint32_t button_id );
void BTN_connect_button( BTN_push_button_t* button, bool (*read_button_state_f) ( GPIO_TypeDef *GPIO, uint32_t pin ), GPIO_TypeDef *GPIO, uint32_t pin );
void BTN_set_callback( BTN_push_button_t *button, void (*callback) ( uint32_t button_id, BTN_button_state_t buttonState ) );
void BTN_update_buttons( void );

/*
typedef struct{
	
} push_button_stack_t;
*/
#endif // __BUTTONS_H__
