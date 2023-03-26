#include "buttons.h"

uint32_t BTN_pressTime 			= BTN_DEFAULT_PRESS_TIME;
uint32_t BTN_longPressTime 	= BTN_DEFAULT_PRESS_LONG_TIME;
uint32_t BTN_releaseTime 		= BTN_DEFAULT_RELEASE_TIME;

static BTN_pushButton_t *pushButtonStack[BTN_MAX_BUTTON_COUNT] = {NULL};

BTN_pushButton_t* BTN_Constructor( void ){
	BTN_pushButton_t *button = (BTN_pushButton_t*)malloc(sizeof(BTN_pushButton_t));
	button->callback = NULL;
	button->HW.getState_f = NULL;
	button->HW.GPIO = NULL;
	button->HW.pin = 0;
	button->ID = 0;
	button->state = BUTTON_STATE_IDLE;
	button->stateOld = BUTTON_STATE_IDLE;
	button->timestamp = 0;
	return button;
}

BTN_pushButton_t* BTN_AddButton( uint32_t buttonID ){
	// Check for same name exist
	for(uint32_t i = 0; i < BTN_MAX_BUTTON_COUNT; i++){
		if(pushButtonStack[i] != NULL){
			if(buttonID == pushButtonStack[i]->ID){
				return NULL;
			}
		}
	}
	
	// Add new button
	for(uint32_t i = 0; i < BTN_MAX_BUTTON_COUNT; i++){
		if(pushButtonStack[i] == NULL){
			pushButtonStack[i] = BTN_Constructor();
			pushButtonStack[i]->ID = buttonID;
			return pushButtonStack[i];
		}
	}
	
	// Stack overflow
	return NULL;
}

void BTN_ConnectButton( BTN_pushButton_t* button, bool (*readButtonState) ( GPIO_TypeDef *GPIO, uint32_t pin ), GPIO_TypeDef *GPIO, uint32_t pin ){
	button->HW.GPIO = GPIO;
	button->HW.pin  = pin;
	button->HW.getState_f = readButtonState;
}

void BTN_SetCallback( BTN_pushButton_t *button, void (*callback) ( uint32_t buttonID, BTN_buttonState_t buttonState ) ){
	button->callback = callback;
}

void BTN_UpdateButton( BTN_pushButton_t *button ) {
	if(button->HW.GPIO == NULL) return;
	
	bool isPressed = button->HW.getState_f( button->HW.GPIO, button->HW.pin );
    switch (button->state) {
        case BUTTON_STATE_IDLE:
						button->stateOld = BUTTON_STATE_IDLE;
            if (isPressed) {
                button->state = BUTTON_STATE_PRESSING;
                button->timestamp = HAL_GetTick();
            }
            break;
        case BUTTON_STATE_PRESSING:
            if (isPressed && ((HAL_GetTick() - button->timestamp) >= BTN_pressTime)) {
                button->state = BUTTON_STATE_PRESSED;
                button->timestamp = HAL_GetTick();
								if(button->callback != NULL) button->callback( button->ID, button->state);
            } else if (!isPressed) {
                button->state = BUTTON_STATE_IDLE;
            }
            break;
        case BUTTON_STATE_PRESSED:
					button->stateOld = BUTTON_STATE_PRESSED;
					if (isPressed && ((HAL_GetTick() - button->timestamp) >= BTN_longPressTime)) {
                button->state = BUTTON_STATE_HOLDED;
                button->timestamp = HAL_GetTick();
								if(button->callback != NULL) button->callback( button->ID, button->state);
            } else if (!isPressed) {
							  
                button->state = BUTTON_STATE_RELEASING;
                button->timestamp = HAL_GetTick();
            }
            break;
				case BUTTON_STATE_HOLDED:
					button->stateOld = BUTTON_STATE_HOLDED;
					if (!isPressed) {
						button->state = BUTTON_STATE_RELEASING;
            button->timestamp = HAL_GetTick();
          }
					break;
        case BUTTON_STATE_RELEASING:
            if (!isPressed && ((HAL_GetTick() - button->timestamp) >= BTN_releaseTime)) {
                button->state = BUTTON_STATE_RELEASED;
								if(button->callback != NULL) button->callback( button->ID, button->state);
            } else if (isPressed) {
                button->state = button->stateOld;
            }
            break;
        case BUTTON_STATE_RELEASED:
					button->stateOld = BUTTON_STATE_RELEASED;
            if (isPressed) {
                button->state = BUTTON_STATE_PRESSING;
                button->timestamp = HAL_GetTick();
            }
            break;
				default:
						break;
    }
}

void BTN_UpdateButtons( void ){
	for(uint32_t i = 0; i < BTN_MAX_BUTTON_COUNT; i++){
		if(pushButtonStack[i] != NULL){
			BTN_UpdateButton(pushButtonStack[i]);
		}
	}
}
