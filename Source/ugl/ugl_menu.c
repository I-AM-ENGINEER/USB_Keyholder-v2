#include "ugl_core.h"

void ugl_menu_destructor( ugl_menu_t *menu ){
	ugl_group_destructor(menu->group);
	free(menu);
}

ugl_menu_t *ugl_menu_constructor( int32_t ID ){
	ugl_menu_t *menu = malloc(sizeof(ugl_menu_t));
	menu->ID = ID;
	menu->group = ugl_group_constructor(0);
	menu->drawing_function = NULL;
	menu->selected_item = NULL;
	menu->caller_ptr = NULL;
	return menu;
}

void ugl_menu_next_item( ugl_menu_t *menu ){
	if(menu->selected_item == NULL) return;
	if(menu->selected_item->next_item == NULL) return;
	menu->selected_item = menu->selected_item->next_item;
}

void ugl_menu_previous_item( ugl_menu_t *menu ){
	if(menu->selected_item == NULL) return;
	if(menu->selected_item->previous_item == NULL) return;
	menu->selected_item = menu->selected_item->previous_item;
}

void ugl_menu_add_item( ugl_menu_t *menu, ugl_item_t *item ){
	ugl_group_add_item(menu->group, item);
}

void ugl_menu_add_group( ugl_menu_t *menu, ugl_group_t *group ){
	ugl_group_add_group(menu->group, group);
}

ugl_item_t *ugl_menu_get_item_by_id( ugl_menu_t *menu, int32_t ID ){
	return ugl_group_get_item_by_id(menu->group, ID);
}

void ugl_menu_render( ugl_menu_t *menu ){
	ugl_group_calc_position_abs( menu->group, 0, 0 );
	ugl_group_render(menu->group);
}
