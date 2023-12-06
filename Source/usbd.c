#include "usbd.h"

#include "usbd_cdc_acm_if.h"
#include "usbd_hid_keyboard.h"


extern USBD_HandleTypeDef hUsbDevice;

static USB_status_t connection_state = USB_STATUS_DISCONNECTED;


void USB_update_status( void ){
	uint8_t keyboardHIDsub[8] = {0,0,0,0,0,0,0,0};
	USBD_HID_Keybaord_SendReport(&hUsbDevice, keyboardHIDsub, sizeof(keyboardHIDsub));
	HAL_Delay(5);
}

USB_status_t USB_connection_status( void ){
	USBD_HID_Keyboard_HandleTypeDef *hhid = (USBD_HID_Keyboard_HandleTypeDef *)hUsbDevice.pClassData_HID_Keyboard;
	if(hUsbDevice.dev_state != 0x03){
		return USB_STATUS_DISCONNECTED;
	}

	if(hhid->state != KEYBOARD_HID_IDLE){
		return USB_STATUS_DISCONNECTED;
	}
	return USB_STATUS_CONNECTED;
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
		USBD_HID_Keybaord_SendReport(&hUsbDevice, keyboardHIDsub, sizeof(keyboardHIDsub));
		HAL_Delay(10);
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

void USB_keyboard_puts(const char* str){
	for(uint16_t i = 0; i < strlen(str); i++){
		USB_keyboard_putc(str[i]);
	}
}
