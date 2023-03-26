#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include "main.h"

#define BTN_DEFAULT_PRESS_TIME			20
#define BTN_DEFAULT_PRESS_LONG_TIME	400
#define BTN_DEFAULT_RELEASE_TIME		20

#define BTN_MAX_NAME_LENGTH					8
#define BTN_MAX_BUTTON_COUNT				16

typedef enum {
	BUTTON_STATE_IDLE,
	BUTTON_STATE_PRESSING,
	BUTTON_STATE_PRESSED,
	BUTTON_STATE_HOLDED,
	BUTTON_STATE_RELEASING,
	BUTTON_STATE_RELEASED,
} BTN_buttonState_t;

typedef struct{
	GPIO_TypeDef *GPIO;
	uint32_t			pin;
	bool (*getState_f) ( GPIO_TypeDef *, uint32_t );
} BTN_pushButtonHW_t;

typedef struct{
	BTN_pushButtonHW_t HW;
	BTN_buttonState_t state;
	BTN_buttonState_t stateOld;
	uint32_t timestamp;
	//char buttonChar[BTN_MAX_NAME_LENGTH];
	uint32_t ID;
	bool inverted;
	void (*callback) ( uint32_t buttonID, BTN_buttonState_t state );
} BTN_pushButton_t;


BTN_pushButton_t* BTN_AddButton( uint32_t buttonID );
void BTN_ConnectButton( BTN_pushButton_t* button, bool (*readButtonState) ( GPIO_TypeDef *GPIO, uint32_t pin ), GPIO_TypeDef *GPIO, uint32_t pin );
void BTN_SetCallback( BTN_pushButton_t *button, void (*callback) ( uint32_t buttonID, BTN_buttonState_t buttonState ) );
void BTN_UpdateButtons( void );

/*
typedef struct{
	
} pushButtonStack_t;
*/
#endif // __BUTTONS_H__
