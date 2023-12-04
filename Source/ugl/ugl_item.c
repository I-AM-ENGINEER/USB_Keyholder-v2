#include "ugl_core.h"


void ugl_item_destructor( ugl_item_t *item ){
	if(item->type == UI_ITEM_TYPE_SPRITE){
		ugl_sprite_destructor(item->sprite);
	}else if(item->type == UGL_ITEM_TYPE_TEXT){
		ugl_text_destructor(item->text);
		//ugl_text_destructor(item->text);
	}
	free(item);
	return;
}

ugl_item_t *ugl_item_constructor( int32_t ID ){
	ugl_item_t *item = malloc(sizeof(ugl_item_t));
	item->ID = ID;
	item->type = UI_ITEM_TYPE_NONE;
	item->next_item = NULL;
	item->previous_item = NULL;
	item->position_x = 0;
	item->position_y = 0;
	item->position_x_abs = 0;
	item->position_y_abs = 0;
	return item;
}

void ugl_item_set_nextprevious( ugl_item_t *item, \
			ugl_item_t *next_item, \
			ugl_item_t *previous_item)
{
	item->next_item = next_item;
	item->previous_item = previous_item;
}

void ugl_item_set_sprite( ugl_item_t *item, ugl_sprite_t *sprite ){
	item->type = UI_ITEM_TYPE_SPRITE;
	item->sprite = sprite;
}

void ugl_item_set_text( ugl_item_t *item, ugl_text_t *text ){
	item->type = UGL_ITEM_TYPE_TEXT;
	item->text = text;
}

void ugl_item_set_position( ugl_item_t *item, int32_t position_x, uint32_t position_y ){
	item->position_x = position_x;
	item->position_y = position_y;
}

void ugl_item_calc_position_abs( ugl_item_t *item, int32_t position_x, uint32_t position_y ){
	item->position_x_abs = position_x + item->position_x;
	item->position_y_abs = position_y + item->position_y;
}

void ugl_item_render( ugl_item_t *item ){
	if(item->type == UI_ITEM_TYPE_SPRITE){
		ugl_sprite_render( item->sprite, item->position_x_abs, item->position_y_abs );
	}else if(item->type == UGL_ITEM_TYPE_TEXT){
		ugl_text_render( item->text, item->position_x_abs, item->position_y_abs );
	}
}




void ugl_group_destructor( ugl_group_t *group ){
	for(uint32_t i = 0; i < group->groupsCount; i++){
		ugl_group_destructor(group->groups[i]);
	}
	for(uint32_t i = 0; i < group->itemsCount; i++){
		ugl_item_destructor(group->items[i]);
	}
	free(group->groups);
	free(group->items);
	free(group);
}

ugl_group_t *ugl_group_constructor( int32_t ID ){
	ugl_group_t *group = malloc(sizeof(ugl_item_t));
	group->ID = ID;
	group->position_x_abs = 0;
	group->position_y_abs = 0;
	group->position_x = 0;
	group->position_y = 0;
	group->itemsCount = 0;
	group->groupsCount = 0;
	group->items = NULL;
	group->groups = NULL;
	return group;
}

void ugl_group_add_item( ugl_group_t *group, ugl_item_t *item ){
	if(group->items == NULL){
		group->items = malloc(sizeof(void *));
	}else{
		group->items = realloc(group->items, (group->itemsCount + 1) * sizeof(void *));//(menu->spritesCount + 1) * sizeof(sprite)
	}
	group->items[group->itemsCount++] = item;
}

void ugl_group_add_group( ugl_group_t *group, ugl_group_t *groupToInsert ){
	if(group->groups == NULL){
		group->groups = malloc(sizeof(void *));
	}else{
		group->groups = realloc(group->groups, (group->groupsCount + 1) * sizeof(void *));//(menu->spritesCount + 1) * sizeof(sprite)
	}
	group->groups[group->groupsCount++] = groupToInsert;
}

void ugl_group_calc_position_abs( ugl_group_t *group, int32_t position_x, uint32_t position_y ){
	group->position_x_abs = position_x + group->position_x;
	group->position_y_abs = position_y + group->position_y;
	for(uint32_t i = 0; i < group->itemsCount; i++){
		ugl_item_calc_position_abs(group->items[i], group->position_x_abs, group->position_y_abs);
	}
	for(uint32_t i = 0; i < group->groupsCount; i++){
		ugl_group_calc_position_abs(group->groups[i], group->position_x_abs, group->position_y_abs);
	}
}

void ugl_group_set_position( ugl_group_t *group, int32_t position_x, uint32_t position_y ){
	group->position_x = position_x;
	group->position_y = position_y;
}

ugl_group_t *ugl_group_get_group_by_id( ugl_group_t *group, int32_t ID ){
	if(group->ID == ID) return group;
	for(uint32_t i = 0; i < group->groupsCount; i++){
		ugl_group_t *tmp = ugl_group_get_group_by_id(group->groups[i], ID);
		if(tmp->ID == ID) return tmp;
	}
	return NULL;
}

ugl_item_t *ugl_group_get_item_by_id( ugl_group_t *group, int32_t ID ){
	for(uint32_t i = 0; i < group->itemsCount; i++){
		if(group->items[i]->ID == ID) return group->items[i];
	}
	
	for(uint32_t i = 0; i < group->groupsCount; i++){
		ugl_item_t *tmp = ugl_group_get_item_by_id(group->groups[i], ID);
		if(tmp->ID == ID) return tmp;
	}
	return NULL;
}

void ugl_group_render( ugl_group_t *group ){
	for(uint32_t i = 0; i < group->itemsCount; i++){
		ugl_item_render(group->items[i]);
	}
	for(uint32_t i = 0; i < group->groupsCount; i++){
		ugl_group_render(group->groups[i]);
	}
}
