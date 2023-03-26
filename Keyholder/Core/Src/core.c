#include "core.h"
#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "crypto.h"
#include "user_interface.h"
#include "system.h"
//#include "usbd_hid.h"
//#include "usb.h"
#include "buttons.h"
//#include "usbd_core.h"
//#include "usbd_desc.h"

#include "usbd_cdc_acm_if.h"
#include "usbd_hid_keyboard.h"

#define DISPLAY_FRAMERATE		1000
#define DISPLAY_FRAME_TIME	1000/DISPLAY_FRAMERATE

extern USBD_HandleTypeDef hUsbDevice;

void EXTI1_IRQHandler(void)
{
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


const char ascii2kbd[]={
	0x2C, 0x9E,	0xB4, 0xA0, 0xA1, 0xA2, 0xA4,	0xB4,				// 8  0x20-0x27
	0xA6, 0xA7, 0xA5,	0xAE, 0x36, 0x2D,	0x37, 0x38, 0x27, // 17 0x28-0x30
	0xB3, 0x33, 0xB6, 0x2E, 0xB7, 0xB8, 0x9F,							// 24 0x3A-0x40
	0x2F, 0x31, 0x30, 0xA3, 0xAD, 0x35,										// 30	0x5B-0x60
	0xAF, 0xB1, 0xB0, 0xB5																// 34 0x7B-0x7E
};

char convert_askii_kbd(char symbol){
	char chToSend = 0;
	if((symbol <= 0x30) && (symbol >= 0x20))
		chToSend = ascii2kbd[symbol - 0x20];
	else if(symbol <= 0x39)
		chToSend = symbol - '1' + 0x1E;
	else if(symbol <= 0x40)
		chToSend = ascii2kbd[symbol - 0x3A + 17];
	else if(symbol <= 0x5A)
		chToSend = symbol - 'A' + 0x04 + 0x80;
	else if(symbol <= 0x60)
		chToSend = ascii2kbd[symbol - 0x5B + 24];
	else if(symbol <= 0x7A)
		chToSend = symbol - 'a' + 0x04;
	else if(symbol <= 0x7E)
		chToSend = ascii2kbd[symbol - 0x7B + 30];
	return chToSend;
}

void USB_keyboard_putc(char ch){
	char chToSend = convert_askii_kbd(ch);
	if(chToSend){
		//MODIFIER, RESERVED, KEYCODE1..6
		uint8_t keyboardHIDsub[8] = {0,0,0,0,0,0,0,0};
		// Press shift and key
		if(chToSend & 0x80)
			keyboardHIDsub[0] = 0x02;
		keyboardHIDsub[2] = chToSend & 0x7F;
		USBD_HID_Keybaord_SendReport(&hUsbDevice, keyboardHIDsub, sizeof(keyboardHIDsub));
		HAL_Delay(10);
		keyboardHIDsub[0] = 0x00;
		keyboardHIDsub[2] = 0x00;
		USBD_HID_Keybaord_SendReport(&hUsbDevice,keyboardHIDsub,sizeof(keyboardHIDsub));
		//CDC_Transmit(0, keyboardHIDsub, sizeof(keyboardHIDsub));
	}
		/*
		if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)
			USBD_HID_SendReport(&hUsbDeviceFS,keyboardHIDsub,sizeof(keyboardHIDsub));
		HAL_Delay(20);
		
		// Release shift and key
		keyboardHIDsub[0] = 0x00;AAAA
		keyboardHIDsub[2] = 0x00;
		if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)
			USBD_HID_SendReport(&hUsbDeviceFS,keyboardHIDsub,sizeof(keyboardHIDsub));
		HAL_Delay(20);
	}
	*/
}


inline void CORE_Process( void ){
	uint32_t currentTick = HAL_GetTick();
	static uint32_t buttonsTimestamp = 0;
	static uint32_t usbKbrdTimestamp = 0;
	static uint32_t UITimestamp = 0;
	
	if(currentTick != buttonsTimestamp){
		buttonsTimestamp = currentTick;
		BTN_UpdateButtons();
	}
	
	static uint32_t circlePosition = 10;
	
	if((currentTick - UITimestamp) > DISPLAY_FRAME_TIME){
		UITimestamp = currentTick;
		UI_print_menu();
	}
	
	if((currentTick - usbKbrdTimestamp) > 1000){
		usbKbrdTimestamp = currentTick;
		//USB_keyboard_putc('A');
		//UI_print_menu();
	}
}

