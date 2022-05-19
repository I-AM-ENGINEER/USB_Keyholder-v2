#include "usb.h"
#include "stdio.h"
#include "main.h"
#include "usbd_hid.h"
#include "usbd_core.h"
#include "user_interface.h"
#include "crypto.h"

extern USBD_HandleTypeDef hUsbDeviceFS;
uint8_t usbDetectionFlag 	= 0;
uint8_t usbHotkeyNumber		= 0;
uint8_t usbWriteFlag 			= 0;

//extern uint8_t currentTab;

// If keys isnt in subsequence, uses this table
char ascii2kbd[]={
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

void usb_keyboard_putc(char ch){
	char chToSend = convert_askii_kbd(ch);
	if(chToSend){
		//MODIFIER, RESERVED, KEYCODE1..6
		uint8_t keyboardHIDsub[8] = {0,0,0,0,0,0,0,0};
		// Press shift and key
		if(chToSend & 0x80)
			keyboardHIDsub[0] = 0x02;
		keyboardHIDsub[2] = chToSend & 0x7F;
		
		//USBD_HID_SendReport(&hUsbDeviceFS,keyboardHIDsub,sizeof(keyboardHIDsub));
		HAL_Delay(20);
		
		// Release shift and key
		keyboardHIDsub[0] = 0x00;
		keyboardHIDsub[2] = 0x00;
		
		//USBD_HID_SendReport(&hUsbDeviceFS,keyboardHIDsub,sizeof(keyboardHIDsub));
		HAL_Delay(20);
	}
}

void usb_keyboard_puts(char *str, uint8_t length){
	for(int i = 0; i < length; i++){
		usb_keyboard_putc(str[i]);
	}
}

uint8_t is_USB_connected( void ){
	return usbDetectionFlag;
}


void set_USB_connection( void ){
	usbDetectionFlag = 1;
}

void reset_USB_connection( void ){
	usbDetectionFlag = 0;
}

void set_USB_write_flag( uint8_t passwordNum ){
	usbHotkeyNumber = passwordNum;
	usbWriteFlag = 1;
}

uint8_t get_USB_write_flag( void ){
	return usbWriteFlag;
}

void wait_USB_insert_hotkey( void ){
	if(is_USB_connected()){
		HAL_Delay(1000);
		usb_keyboard_puts(passwordDataBaseHot[usbHotkeyNumber].password, strlen(passwordDataBaseHot[usbHotkeyNumber].password));
		usbWriteFlag = 0;
		setDisplayUpdateFlag();
	}
}

