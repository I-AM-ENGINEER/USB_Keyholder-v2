#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__


/* -------------------------------------------------------------------------- */

typedef enum{
  login_tab 			= 0x00, 	// Black color, no pixel
  main_tab 				= 0x01,  	// Pixel is set. Color depends on OLED
	usb_write_tab 	= 0x02,
	paswd_list_tab 	= 0x03,
} menu_tab_type;


void PAS_print_menu( void );

void UI_print_menu( void );

/* -------------------------------------------------------------------------- */

#endif // __USER_INTERFACE_H__
