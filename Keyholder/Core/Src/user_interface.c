#include "user_interface.h"
#include "ssd1306.h"
#include "stdio.h"
#include "crypto.h"
#include "main.h"
#include "stdlib.h"
#include "usb.h"
#include "string.h"
#include "system.h"
#include "usbd_cdc_if.h"
//http://javl.github.io/image2cpp/

//#define DEBUG

const unsigned char img_folder[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x1e, 0x70, 0xe3, 0x00, 
	0x10, 0xc0, 0xb0, 0xfc, 0x20, 0x81, 0x00, 0x80, 0x20, 0x81, 0x00, 0x80, 0x20, 0x81, 0x80, 0x80, 
	0x20, 0xf0, 0xe0, 0x80, 0x20, 0x80, 0x20, 0x80, 0x20, 0x83, 0xa0, 0x80, 0x10, 0x80, 0xe0, 0x40, 
	0x00, 0xf8, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x01, 0xc3, 0xf0, 0x03, 0xc3, 0x46, 0x10, 0x0c, 0x40, 0x44, 0x10, 0x10, 0xc0, 0x40, 0x20, 
	0x30, 0x80, 0x40, 0xe0, 0x00, 0x80, 0x80, 0x20, 0x01, 0x01, 0x88, 0x20, 0x02, 0x01, 0x08, 0x20, 
	0x02, 0x06, 0x0c, 0x20, 0x02, 0x04, 0x03, 0xe0, 0x04, 0x0f, 0x00, 0x40, 0x04, 0x00, 0xf0, 0x00, 
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};
const unsigned char img_lock[] = {
	0x03, 0xff, 0xff, 0x00, 0x07, 0xff, 0xff, 0x80, 0x0f, 0xff, 0xff, 0xc0, 0x0f, 0x00, 0x03, 0xc0, 
	0x0e, 0x00, 0x01, 0xc0, 0x0e, 0x00, 0x01, 0xc0, 0x0e, 0x00, 0x01, 0xc0, 0x0e, 0x00, 0x01, 0xc0, 
	0x0e, 0x00, 0x01, 0xc0, 0x0e, 0x00, 0x01, 0xc0, 0x0e, 0x00, 0x01, 0xc0, 0x0e, 0x00, 0x01, 0xc0, 
	0x7f, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0xf8, 0x60, 0x00, 0x00, 0x18, 0x60, 0x00, 0x00, 0x18, 
	0x60, 0x07, 0x80, 0x18, 0x60, 0x0f, 0xc0, 0x18, 0x60, 0x0f, 0xc0, 0x18, 0x60, 0x0f, 0xc0, 0x18, 
	0x60, 0x0f, 0xc0, 0x18, 0x60, 0x07, 0x80, 0x18, 0x60, 0x03, 0x00, 0x18, 0x60, 0x03, 0x00, 0x18, 
	0x60, 0x03, 0x00, 0x18, 0x60, 0x03, 0x00, 0x18, 0x60, 0x00, 0x00, 0x18, 0x60, 0x00, 0x00, 0x18, 
	0x7f, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0xf8
};
const unsigned char img_settings[] = {
  0x00, 0x07, 0x80, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x0c, 0x0f, 0xc0, 0xc0, 0x1e, 0x0f, 0xc1, 0xe0, 
	0x3f, 0x3f, 0xf3, 0xf0, 0x3f, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xc0, 
	0x07, 0xf8, 0x7f, 0x80, 0x07, 0xe0, 0x1f, 0x80, 0x0f, 0xc0, 0x0f, 0xc0, 0x0f, 0x87, 0x87, 0xc0, 
	0x7f, 0x8f, 0xc7, 0xf8, 0xff, 0x1c, 0xe3, 0xfc, 0xff, 0x18, 0x63, 0xfc, 0xff, 0x18, 0x63, 0xfc, 
	0xff, 0x1c, 0xe3, 0xfc, 0x7f, 0x8f, 0xc7, 0xf8, 0x0f, 0x87, 0x87, 0xc0, 0x0f, 0xc0, 0x0f, 0xc0, 
	0x07, 0xe0, 0x1f, 0x80, 0x07, 0xf8, 0x7f, 0x80, 0x0f, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xe0, 
	0x3f, 0xff, 0xff, 0xf0, 0x3f, 0x3f, 0xf3, 0xf0, 0x1e, 0x0f, 0xc1, 0xe0, 0x0c, 0x0f, 0xc0, 0xc0, 
	0x00, 0x0f, 0xc0, 0x00, 0x00, 0x07, 0x80, 0x00
};
const unsigned char img_usb_write[] = {
	0xc1, 0x8f, 0x87, 0xf8, 0xc1, 0x9f, 0xc7, 0xfc, 0xc1, 0x98, 0x06, 0x0c, 0xc1, 0x98, 0x06, 0x0c, 
	0xc1, 0x98, 0x06, 0x0c, 0xc1, 0x9f, 0x87, 0xf8, 0xc1, 0x8f, 0xc7, 0xf8, 0xc1, 0x80, 0xc6, 0x0c, 
	0xc1, 0x80, 0xc6, 0x0c, 0xe3, 0x80, 0xc6, 0x0c, 0x7f, 0x1f, 0xc7, 0xfc, 0x3e, 0x0f, 0x87, 0xf8, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0xff, 0xfc, 
	0x80, 0x00, 0x00, 0x24, 0x80, 0x00, 0x00, 0x24, 0xa4, 0xa4, 0xa4, 0xa4, 0x95, 0x15, 0x15, 0x24, 
	0x8e, 0x0e, 0x0e, 0x24, 0x8e, 0x0e, 0x0e, 0x24, 0x95, 0x15, 0x15, 0x24, 0xa4, 0xa4, 0xa4, 0xa4, 
	0x80, 0x00, 0x00, 0x24, 0x80, 0x00, 0x00, 0x24, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x20, 
	0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00
};
const unsigned char img_passwords[] = {
	0x0f, 0xff, 0xf0, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x0c, 0x00, 0x1c, 0x00, 0x0c, 0x00, 0x0e, 0x00, 
	0x0c, 0xfc, 0x07, 0x00, 0x0c, 0x00, 0x03, 0x80, 0x0c, 0x00, 0xf9, 0xc0, 0x0c, 0x01, 0xfc, 0xc0, 
	0x0c, 0xf3, 0xce, 0xc0, 0x0c, 0x03, 0x86, 0xc0, 0x0c, 0x03, 0x86, 0xc0, 0x0c, 0x03, 0xce, 0xc0, 
	0x0c, 0xc7, 0xfe, 0xc0, 0x0c, 0x0f, 0xfc, 0xc0, 0x0c, 0x1f, 0xf8, 0xc0, 0x0c, 0x3e, 0x00, 0xc0, 
	0x0c, 0x7e, 0x00, 0xc0, 0x0c, 0xf8, 0x1c, 0xc0, 0x0d, 0xf8, 0x00, 0xc0, 0x0d, 0xe0, 0x00, 0xc0, 
	0x0c, 0xe0, 0x00, 0xc0, 0x0c, 0x01, 0xfc, 0xc0, 0x0c, 0x00, 0x00, 0xc0, 0x0c, 0x00, 0x00, 0xc0, 
	0x0c, 0x00, 0x00, 0xc0, 0x0c, 0xff, 0xfc, 0xc0, 0x0c, 0x00, 0x00, 0xc0, 0x0c, 0x00, 0x00, 0xc0, 
	0x0f, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 0xc0
};


#define DELTA_SLIDE 4

const uint8_t menuItemsCount = 5;
// Menu icons
const unsigned char *icons[menuItemsCount] = \
{img_settings, img_passwords, img_usb_write, img_folder, img_lock};


// Tabs name and number, require mach with icon position!
#define	settings_tab		0x00
#define	paswd_list_tab 	0x01
#define	usb_write_tab 	0x02
#define	folder_tab 			0x03
#define	lock_tab 				0x04
#define usb_hotkey_tab	0xFD
#define	main_tab 				0xFE
#define	login_tab 			0xFF
#define password_print_tab 0xFC
#define usb_write_tab_insert_in_usb 0xFB
uint8_t currentTab = login_tab;

const char passTrue[6] = "111111";

uint8_t pushedButtonFlag 		= 0;
uint8_t releasedButtonFlag 	= 0;
uint8_t displayUpdateFlag		= 1;
uint8_t pushedButtonNum 		= 0;
uint8_t releasedButtonNum 	= 0;

void menu_main 			( void );
void menu_login			( void );
void menu_folder		( void );
void menu_passwords	( void );
void menu_passwords_show( void );
void menu_settings 	( void );
void menu_usb_write	( void );
void menu_usb_hotkey( void );
void menu_passwords_case0( void );

uint8_t getPushedButtonFlag		( void );
uint8_t getReleasedButtonFlag	( void );
uint8_t getDisplayUpdateFlag	( void );
void setDisplayUpdateFlag			( void );


void setDisplayUpdateFlag( void ){
	displayUpdateFlag = 1;
}

uint8_t getDisplayUpdateFlag( void ){
	if(displayUpdateFlag){
		displayUpdateFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t getPushedButtonFlag( void ){
	if(pushedButtonFlag){
		pushedButtonFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t getReleasedButtonFlag( void ){
	if(releasedButtonFlag){
		releasedButtonFlag = 0;
		return 1;
	}
	return 0;
}


void setPushedButton( void ){
	if(switches_byte != 0){
		pushedButtonFlag = 1;
		releasedButtonFlag = 0;
		releasedButtonNum = 0;
		for(int i = 0; i < 8; i++){
			if(switches_byte & (1 << i)){			
					pushedButtonNum = i + 1;
					break;
			}
		}
	}else{
		releasedButtonNum = pushedButtonNum;
		pushedButtonNum = 0;
		pushedButtonFlag = 0;
		releasedButtonFlag = 1;
	}
	setDisplayUpdateFlag();
}

// Main function of GUI
void UI_print_menu( void ){
	if(getDisplayUpdateFlag()){
		switch(currentTab){
			case main_tab:
					menu_main();
				break;
			case usb_write_tab:
				//menu_usb_write();
				menu_passwords();
				break;
			case paswd_list_tab:
				menu_passwords();
				break;
			case folder_tab:
				menu_folder();
				break;
			case lock_tab:
				//setDisplayUpdateFlag();
				power_SetNeedSleepFlag();
				currentTab = login_tab;
				break;
			case usb_hotkey_tab:
				menu_usb_hotkey();
				break;
			case password_print_tab:
				menu_passwords_case0();
				break;
			case usb_write_tab_insert_in_usb:
				menu_usb_write();
				break;
			default:
				menu_login();
		}
		ssd1306_UpdateScreen();
	}
}


void menu_usb_hotkey( void ){
	static int i = 0;
	if(!is_USB_connected()){
		ssd1306_Fill(Black);
		ssd1306_DrawRectangle(0, 0, 127, 31, White);
		ssd1306_SetCursor(2,2);
		char text[30];
		
		sprintf(text,"%s %d %d",passwordDataBaseHot[1].login, i++, !is_USB_connected());
		ssd1306_WriteString(text,Font_6x8, White);
		ssd1306_SetCursor(2,12);
		ssd1306_WriteString("Waiting connection",Font_6x8, White);
		setDisplayUpdateFlag();
	}else{
		if(get_USB_write_flag() == 0){
			currentTab = main_tab;
			setDisplayUpdateFlag();
		}else{
			ssd1306_Fill(Black);
			ssd1306_DrawRectangle(0, 0, 127, 31, White);
			ssd1306_SetCursor(2,2);
			char text[20];
			sprintf(text,"%s",passwordDataBaseHot[pushedButtonNum - 4].login);
			ssd1306_WriteString(text,Font_6x8, White);
			ssd1306_SetCursor(2,12);
			ssd1306_WriteString("Connect to USB...",Font_6x8, White);
		}
	}
}


// Main menu function
void menu_main ( void ){
	static uint8_t menuPosition 				= 2;
	static uint8_t menuPositionOld 			= 2;
	static uint8_t menuPositionTarget 	= 2;
	static int8_t animationPoint 				= 0;
	static int8_t animationPointTarget 	= 0;
	
	
	if(getPushedButtonFlag()){
		if((pushedButtonNum == 1) && (menuPositionTarget != 0)){
			menuPositionTarget--; // Slide left
		}else if((pushedButtonNum == 3) && (menuPositionTarget != (menuItemsCount - 1))){
			menuPositionTarget++; // Slide right
		}else if(pushedButtonNum == 2){
			
			// Switch tab
			currentTab = menuPositionTarget;
			// Reset menu state
			menuPosition 				= 2;
			menuPositionOld 		= 2;
			menuPositionTarget 	= 2;
			
			setDisplayUpdateFlag();
			return;
		}else if((pushedButtonNum >= 4) && (pushedButtonNum <= 8)){
			set_USB_write_flag(pushedButtonNum - 1);
			currentTab = usb_hotkey_tab;
			menu_usb_hotkey();
			return;
		}
	}
	
	if(animationPoint == animationPointTarget){
		animationPointTarget = 0;
		animationPoint = 0;
		
		if(menuPositionTarget != menuPosition){
			if(menuPosition > menuPositionTarget){
				menuPositionOld = menuPosition;
				menuPosition--;
				animationPointTarget = 42;
			}else{
				menuPositionOld = menuPosition;
				menuPosition++;
				animationPointTarget = -42;
			}
		}
	}
	
	// Icons animation
	if((animationPoint != animationPointTarget) && (menuPositionOld != menuPosition)){
		// If menu counter adds, slide right, else slide left
		if(abs(animationPoint - animationPointTarget) >= DELTA_SLIDE)
			animationPoint += (animationPoint > animationPointTarget) ? -DELTA_SLIDE : DELTA_SLIDE;
		else
			animationPoint = animationPointTarget;
		
		ssd1306_Fill(Black);
		// Print central icon
		ssd1306_DrawBitmap((uint8_t)(animationPoint + 50), 0, icons[menuPositionOld],  30, 30, White);
		ssd1306_Line(48, 31, 80, 31, White);
		// Print side icons
		if(menuPositionOld-2 >= 0)
			ssd1306_DrawBitmap((uint8_t)(animationPoint - 34), 0, icons[menuPositionOld-2],  30, 30, White);
		if(menuPositionOld-1 >= 0)
			ssd1306_DrawBitmap((uint8_t)(animationPoint +  8), 0, icons[menuPositionOld-1],  30, 30, White);
		if(menuPositionOld+1 < menuItemsCount)
			ssd1306_DrawBitmap((uint8_t)(animationPoint + 92), 0, icons[menuPositionOld+1],  30, 30, White);
		if(menuPositionOld+2 < menuItemsCount)
			ssd1306_DrawBitmap((uint8_t)(animationPoint +134), 0, icons[menuPositionOld+2],  30, 30, White);
		
		setDisplayUpdateFlag();
	}else{
		ssd1306_Fill(Black);
		ssd1306_Line(48, 31, 80, 31, White);
		ssd1306_DrawBitmap(50, 0, icons[menuPosition],  	30, 30, White);
		if(menuPosition-1 >= 0)
			ssd1306_DrawBitmap(8, 0, icons[menuPosition-1], 30, 30, White);
		if(menuPosition+1 < menuItemsCount)
			ssd1306_DrawBitmap(92, 0, icons[menuPosition+1],30, 30, White);
	}
}

// Authorisation tab, displayed on startup
void menu_login( void ){
	static char pass[6];
	static uint8_t j = 0;
	
	ssd1306_Fill(Black);
	ssd1306_DrawRectangle(0, 0, 127, 31, White);
	
	if(getPushedButtonFlag()){
		pass[++j - 1] = pushedButtonNum + '0';
	}
	
	if(j == 6){
		// Check password
		uint8_t unPass = 0;
		for (uint8_t i = 0; i < 6; i++){
			if (pass[i] == passTrue[i]){
				unPass++;
			}
		}
		
		if (unPass == 6){
			// If password correct, go to menu
			currentTab = main_tab;
		}else{
			// Print "ERROR" and wait next try
			ssd1306_SetCursor(4, 4);
			ssd1306_WriteString("ERROR", Font_16x26, White);
			ssd1306_UpdateScreen();
			HAL_Delay(3000);
		}
		j = 0;
		setDisplayUpdateFlag();
	}else{
		for(uint8_t i = 0; i < (j - 1); i++){
			ssd1306_SetCursor(2 + 16 * i, 2);		
			ssd1306_WriteChar('*', Font_16x26, White);
		}
		
		ssd1306_SetCursor(2 + 16 * (j - 1), 2);
		ssd1306_WriteChar(pass[j - 1], Font_16x26, White);
	}
}

// IN DEVELOPMENT: Tab with passwords list
	

uint8_t currentPassword = 0;
void menu_passwords_case0( void ){
	
	
	char textsas[20];
	if(getPushedButtonFlag()){
		if(pushedButtonNum == 2){
			currentTab = paswd_list_tab;
			setDisplayUpdateFlag();
			return;
		}
	}
	dataType passwordData;
	flash_data_grab(&passwordData, currentPassword);
	
	ssd1306_Fill(Black);
	//ssd1306_DrawRectangle(0, 0, 127, 31, White);
	ssd1306_SetCursor(0,0);
	sprintf(textsas,"l:%s",passwordData.login); 
	ssd1306_WriteString(textsas,Font_6x8, White);
	ssd1306_SetCursor(0,8);
	sprintf(textsas,"p:%s",passwordData.password); 
	ssd1306_WriteString(textsas,Font_6x8, White);
	ssd1306_SetCursor(0,16);
	sprintf(textsas,"c:%s",passwordData.comment); 
	ssd1306_WriteString(textsas, Font_6x8, White);
}


void menu_passwords( void ){
	char textsas[20];
	static uint8_t currentTabPass = 0;
	if(getPushedButtonFlag()){
		if((pushedButtonNum == 1) && (currentTabPass != 0))
			currentTabPass -= 4;
		else if((pushedButtonNum == 3) && (currentTabPass < ((uint8_t)flash_get_passwords_count())))
			currentTabPass += 4;
		else if(pushedButtonNum == 2){
			currentTab = main_tab;
			setDisplayUpdateFlag();
			return;
		}else if((pushedButtonNum >= 5) && (pushedButtonNum <= 7)){
			
			if(currentTab != usb_write_tab){
				currentTab = password_print_tab;
				currentPassword = pushedButtonNum - 5 + currentTabPass;
				setDisplayUpdateFlag();
				return;
			}else{
				currentTab = usb_write_tab_insert_in_usb;
			}
		}
	}
	
	ssd1306_Fill(Black);
	for(int i = 0; i < 4; i++){
		ssd1306_SetCursor(2, i*8);
		dataType passwordData;
		flash_data_grab(&passwordData, currentTabPass + i);
		sprintf(textsas,"%d:%s", i+4, passwordData.login); 
		ssd1306_WriteString(textsas,Font_6x8, White);
	}
}
		


// IN DEVELOPMENT: settings
void menu_settings ( void ){
	ssd1306_Fill(Black);
	ssd1306_SetCursor(4, 4);
	ssd1306_WriteString("Settings", Font_16x26, White);
	ssd1306_DrawRectangle(0, 0, 127, 31, White);
	ssd1306_UpdateScreen();
	
	// Wait push button
	while(!switches_byte)
		HAL_Delay(1);
	// Wait button release
	while(switches_byte)
		HAL_Delay(1);
	
	currentTab = main_tab;
}




// IN DEVELOPMENT: Password list with USB access
void menu_usb_write( void ){
	
	// If button was pushed
	if(getPushedButtonFlag()){
	// If push 2 button
		if(pushedButtonNum == 2){
			// Switch tab
			currentTab = main_tab;
			// And update display on next cycle for display main tab
			setDisplayUpdateFlag();
			// End exit from function
			return;
		}
	}
	ssd1306_Fill(Black);
	ssd1306_SetCursor(2,2); 
	ssd1306_WriteString("insert in usb",Font_7x10, White);
	
	set_USB_write_flag(2);
	//currentTab = usb_hotkey_tab;
	//menu_usb_hotkey();
	/*
	static uint8_t counter = 0;
	
	// If button was pushed
	if(getPushedButtonFlag()){
	// If push 2 button
		if(pushedButtonNum == 2){
			// Switch tab
			currentTab = main_tab;
			// And update display on next cycle for display main tab
			setDisplayUpdateFlag();
			// End exit from function
			return;
		}
		
	}
	
	ssd1306_SetCursor(4, 4);
	//if(pushedButtonNum != 0){
		ssd1306_Fill(Black);
		ssd1306_DrawRectangle(0, 0, 127, 31, White);
	//}
	if(pushedButtonNum == 1){
		ssd1306_WriteString("Test1", Font_16x26, White);
	}else if(pushedButtonNum == 3){
		ssd1306_WriteString("Button3", Font_16x26, White);
	}else if(pushedButtonNum != 0){
		char tmp[10];
		sprintf(tmp, "i:%d", counter++);
		ssd1306_WriteString(tmp, Font_16x26, White);
	}else if(releasedButtonNum != 0){
		ssd1306_WriteString("Released!", Font_16x26, White);
	}
	*/
}

// IN DEVELOPMENT: 
void menu_folder( void ){
	ssd1306_Fill(Black);
	ssd1306_SetCursor(4, 4);
	ssd1306_WriteString("Folder", Font_16x26, White);
	ssd1306_DrawRectangle(0, 0, 127, 31, White);
	ssd1306_UpdateScreen();
	
	char tmp[40] = "allllll";
	char s[40] = "test";
	char tmp2[40] = "";
	uint8_t t = 0;
	//while(1){
		/*
		//CDC_Receive_FS(tmp2, 4);
		for(int i = 0; i < 4; i++){
			if(tmp2[i] == s[i])
				t++;
			else {
				t = 0;
				break;
			}
		}*/
			
		//CDC_Transmit_FS(UserRxBufferFS, 5);
	//}
	while(switches_byte)
		HAL_Delay(1);
	// Wait push button
	while(!switches_byte)
		HAL_Delay(1);
	// Wait button release
	while(switches_byte)
		HAL_Delay(1);
	
	currentTab = main_tab;
}
