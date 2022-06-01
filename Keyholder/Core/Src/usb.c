#include "stdio.h"

#include "main.h"
#include "usb.h"
#include "user_interface.h"
#include "crypto.h"

#include "usbd_cdc_if.h"
#include "usbd_cdc.h"
#include "usbd_hid.h"
#include "usbd_core.h"


extern USBD_HandleTypeDef hUsbDeviceFS;
uint8_t usbDetectionFlag 	= 0;
uint8_t usbHotkeyNumber		= 0;
uint8_t usbWriteFlag 			= 0;

//extern uint8_t currentTab;

enum usbModes{
	suspend_mode 	= USB_SUSPEND_MODE,
	HID_mode 			= USB_HID_MODE,
	CDC_mode			=	USB_CDC_MODE,
	MSC_mode			= USB_MSC_MODE,
	DFU_mode			= USB_DFU_MODE,
} 
usbMode = suspend_mode,
usbModeOld = suspend_mode;

enum usbStates{
	suspend_state,
	idle_state,
	HID_state,
	CDC_state,
	MSC_state,
	DFU_state,
} usbState = suspend_state;

enum usbEvents{
	USB_insert_event,
	USB_extract_event,
	no_event,
};

//enum usbModes usbMode = HID_mode;

// If keys isnt in subsequence, uses this table
const char ascii2kbd[]={
	0x2C, 0x9E,	0xB4, 0xA0, 0xA1, 0xA2, 0xA4,	0xB4,				// 8  0x20-0x27
	0xA6, 0xA7, 0xA5,	0xAE, 0x36, 0x2D,	0x37, 0x38, 0x27, // 17 0x28-0x30
	0xB3, 0x33, 0xB6, 0x2E, 0xB7, 0xB8, 0x9F,							// 24 0x3A-0x40
	0x2F, 0x31, 0x30, 0xA3, 0xAD, 0x35,										// 30	0x5B-0x60
	0xAF, 0xB1, 0xB0, 0xB5																// 34 0x7B-0x7E
};

void USB_main( void ){
	if(usbMode != usbModeOld){
		usbModeOld = usbMode;
		if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED){
			USBD_Stop(&hUsbDeviceFS);
		}
		
		switch(usbMode){
			case HID_mode:
				
				USBD_RegisterClass(&hUsbDeviceFS, &USBD_HID);
				usbState = HID_state;
				break;
			case CDC_mode:
				USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);
				USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);
				//USBD_Interface_fops_FS.Init();
				usbState = CDC_state;
				break;
			case MSC_mode:
				usbState = MSC_state;
				break;
			case DFU_mode:
				usbState = DFU_state;
				break;
			case suspend_mode:
				usbState = suspend_state;
				break;
		}
		USBD_Start(&hUsbDeviceFS);
	}
	
	switch(usbState){
		case HID_state:
			if(get_USB_write_flag() && (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)){
				HAL_Delay(1000);
				dataType data;
				flash_data_grab(&data, usbHotkeyNumber);
				USB_keyboard_puts(data.password, strlen(data.password));
				usbWriteFlag = 0;
				setDisplayUpdateFlag();
			}
			break;
		case CDC_state:
			break;
		case MSC_state:
			break;
		case DFU_state:
			break;
		case idle_state:
			break;
		case suspend_state:
			break;
	}
}


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
		
		USBD_HID_SendReport(&hUsbDeviceFS,keyboardHIDsub,sizeof(keyboardHIDsub));
		HAL_Delay(20);
		
		// Release shift and key
		keyboardHIDsub[0] = 0x00;
		keyboardHIDsub[2] = 0x00;
		USBD_HID_SendReport(&hUsbDeviceFS,keyboardHIDsub,sizeof(keyboardHIDsub));
		HAL_Delay(20);
	}
}

void USB_keyboard_puts(char *str, uint8_t length){
	for(int i = 0; i < length; i++){
		USB_keyboard_putc(str[i]);
	}
}

void set_USB_write_flag( uint8_t passwordNum ){
	usbHotkeyNumber = passwordNum;
	usbWriteFlag = 1;
}

uint8_t get_USB_write_flag( void ){
	return usbWriteFlag;
}

void USB_set_mode(uint8_t mode){
	usbMode = (enum usbModes)mode;
}

uint8_t USB_get_state( void ){
	if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED){
		return 1;
	}
	return 0;
}
