#include "core.h"
#include "ssd1306.h"
#include "crypto.h"
#include "ui/core.h"
#include "system.h"
#include "battery.h"
#include "buttons.h"
#include "usbd.h"

#define DISPLAY_FRAMERATE		1000
#define DISPLAY_FRAME_TIME	1000/DISPLAY_FRAMERATE

void EXTI1_IRQHandler(void){
  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
}

inline void CORE_Init( void ){
	SYS_init();
	HAL_GPIO_WritePin(VBUS_EN_GPIO_Port, VBUS_EN_Pin, GPIO_PIN_RESET);
	
	ssd1306_SetDisplayPower(1);
	ssd1306_SetDisplayOn(1);
	
	ssd1306_Init();
	SYS_ButtonsInit();
	UI_Init();
}

inline void CORE_Process( void ){
	uint32_t currentTick = HAL_GetTick();
	static uint32_t buttonsTimestamp = 0;
	static uint32_t usbKbrdTimestamp = 0;
	static uint32_t UITimestamp = 0;
	
	if(currentTick != buttonsTimestamp){
		buttonsTimestamp = currentTick;
		BTN_update_buttons();
	}
	
	static uint32_t circlePosition = 10;
	
	if((currentTick - UITimestamp) > DISPLAY_FRAME_TIME){
		UITimestamp = currentTick;
		UI_print_menu();
	}

	battery_process();
}

