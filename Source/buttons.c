#include "buttons.h"

uint32_t BTN_pressTime 			= BTN_DEFAULT_PRESS_TIME;
uint32_t BTN_long_press_time 	= BTN_DEFAULT_PRESS_LONG_TIME;
uint32_t BTN_releaseTime 		= BTN_DEFAULT_RELEASE_TIME;

static BTN_push_button_t *push_button_stack[BTN_MAX_BUTTON_COUNT] = {NULL};

BTN_push_button_t* BTN_constructor( void ){
	BTN_push_button_t *button = (BTN_push_button_t*)malloc(sizeof(BTN_push_button_t));
	button->callback = NULL;
	button->hw.get_state_f = NULL;
	button->hw.GPIO = NULL;
	button->hw.pin = 0;
	button->ID = 0;
	button->state = BUTTON_STATE_IDLE;
	button->state_old = BUTTON_STATE_IDLE;
	button->timestamp = 0;
	return button;
}

BTN_push_button_t* BTN_add_button( uint32_t button_id ){
	// Check for same name exist
	for(uint32_t i = 0; i < BTN_MAX_BUTTON_COUNT; i++){
		if(push_button_stack[i] != NULL){
			if(button_id == push_button_stack[i]->ID){
				return NULL;
			}
		}
	}
	
	// Add new button
	for(uint32_t i = 0; i < BTN_MAX_BUTTON_COUNT; i++){
		if(push_button_stack[i] == NULL){
			push_button_stack[i] = BTN_constructor();
			push_button_stack[i]->ID = button_id;
			return push_button_stack[i];
		}
	}
	
	// Stack overflow
	return NULL;
}

void BTN_connect_button( BTN_push_button_t* button, bool (*read_button_state_f) ( GPIO_TypeDef *GPIO, uint32_t pin ), GPIO_TypeDef *GPIO, uint32_t pin ){
	button->hw.GPIO = GPIO;
	button->hw.pin  = pin;
	button->hw.get_state_f = read_button_state_f;
}

void BTN_set_callback( BTN_push_button_t *button, void (*callback) ( uint32_t button_id, BTN_button_state_t buttonState ) ){
	button->callback = callback;
}

void BTN_update_button( BTN_push_button_t *button ) {
	if(button->hw.GPIO == NULL) return;
	
	bool is_pressed = button->hw.get_state_f( button->hw.GPIO, button->hw.pin );
    switch (button->state) {
        case BUTTON_STATE_IDLE:
						button->state_old = BUTTON_STATE_IDLE;
            if (is_pressed) {
                button->state = BUTTON_STATE_PRESSING;
                button->timestamp = HAL_GetTick();
            }
            break;
        case BUTTON_STATE_PRESSING:
            if (is_pressed && ((HAL_GetTick() - button->timestamp) >= BTN_pressTime)) {
                button->state = BUTTON_STATE_PRESSED;
                button->timestamp = HAL_GetTick();
								if(button->callback != NULL) button->callback( button->ID, button->state);
            } else if (!is_pressed) {
                button->state = BUTTON_STATE_IDLE;
            }
            break;
        case BUTTON_STATE_PRESSED:
					button->state_old = BUTTON_STATE_PRESSED;
					if (is_pressed && ((HAL_GetTick() - button->timestamp) >= BTN_long_press_time)) {
                button->state = BUTTON_STATE_HOLDED;
                button->timestamp = HAL_GetTick();
								if(button->callback != NULL) button->callback( button->ID, button->state);
            } else if (!is_pressed) {
							  
                button->state = BUTTON_STATE_RELEASING;
                button->timestamp = HAL_GetTick();
            }
            break;
				case BUTTON_STATE_HOLDED:
					button->state_old = BUTTON_STATE_HOLDED;
					if (!is_pressed) {
						button->state = BUTTON_STATE_RELEASING;
            button->timestamp = HAL_GetTick();
          }
					break;
        case BUTTON_STATE_RELEASING:
            if (!is_pressed && ((HAL_GetTick() - button->timestamp) >= BTN_releaseTime)) {
                button->state = BUTTON_STATE_RELEASED;
								if(button->callback != NULL) button->callback( button->ID, button->state);
            } else if (is_pressed) {
                button->state = button->state_old;
            }
            break;
        case BUTTON_STATE_RELEASED:
					button->state_old = BUTTON_STATE_RELEASED;
            if (is_pressed) {
                button->state = BUTTON_STATE_PRESSING;
                button->timestamp = HAL_GetTick();
            }
            break;
				default:
						break;
    }
}

void BTN_update_buttons( void ){
	for(uint32_t i = 0; i < BTN_MAX_BUTTON_COUNT; i++){
		if(push_button_stack[i] != NULL){
			BTN_update_button(push_button_stack[i]);
		}
	}
}
