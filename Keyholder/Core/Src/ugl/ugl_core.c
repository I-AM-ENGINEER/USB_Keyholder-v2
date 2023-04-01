#include "ugl_core.h"

ugl_menu_t *UI_current_menu = NULL;

void ugl_return( void ){
	if(UI_current_menu->caller_ptr == NULL) return;
	ugl_menu_t *returnMenu = UI_current_menu->caller_ptr;
	ugl_menu_destructor(UI_current_menu);
	UI_current_menu = returnMenu;
}

void ugl_enter( int32_t ID, ugl_menu_t* (*menu_constructor) ( int32_t ID, void* extra ), void* extra ){
	ugl_menu_t *oldMenu = UI_current_menu;
	ugl_menu_t *curMenu = menu_constructor(ID, extra);
	if(curMenu == NULL) return;
	UI_current_menu = curMenu;
	UI_current_menu->caller_ptr = oldMenu;
}

ugl_menu_t *ugl_get_current_menu( void ){
	return UI_current_menu;
}

void ugl_render( void ){
	if(UI_current_menu->drawing_function != NULL){
		UI_current_menu->drawing_function();
	}
}
