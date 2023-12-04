#ifndef __UGL_CORE_H__
#define __UGL_CORE_H__

#pragma anon_unions

#include "main.h"
#include "ssd1306.h"	// For fonts only

typedef enum{
	UI_ITEM_TYPE_SPRITE,
	UGL_ITEM_TYPE_TEXT,
	UI_ITEM_TYPE_NONE,
} ugl_item_type_t;

typedef struct{
	uint8_t* bitmap;
	uint32_t bitmap_height;
	uint32_t bitmap_width;
	bool visible;
} ugl_sprite_t;

typedef struct{
	char* string;
	uint32_t length;
	FontDef* font;
	bool visible;
} ugl_text_t;

typedef struct ugl_item 	ugl_item_t;
typedef struct ugl_group 	ugl_group_t;
typedef struct ugl_menu 	ugl_menu_t;

struct ugl_item{
	int32_t ID;
	ugl_item_type_t type;
	union{
		ugl_sprite_t	*sprite;
		ugl_text_t		*text;
	};
	int32_t position_x;
	int32_t position_y;
	
	int32_t position_x_abs;
	int32_t position_y_abs;
	
	ugl_item_t (*next_item);
	ugl_item_t (*previous_item);
};

struct ugl_group{
	int32_t ID;
	uint32_t groupsCount;
	uint32_t itemsCount;
	
	int32_t position_x;
	int32_t position_y;
	
	int32_t position_x_abs;
	int32_t position_y_abs;
	
	ugl_group_t **groups;
	ugl_item_t 	**items;
};

struct ugl_menu{
	int32_t ID;
	void (*drawing_function) ( void );
	ugl_menu_t 	*caller_ptr;
	ugl_item_t 	*selected_item;
	ugl_group_t 	*group;
};


void ugl_render( void );
void ugl_return( void );
void ugl_enter( int32_t ID, ugl_menu_t* (*menu_constructor) ( int32_t ID, void* extra ), void* extra );
ugl_menu_t *ugl_get_current_menu( void );

void ugl_item_destructor( ugl_item_t *item );
ugl_item_t *ugl_item_constructor( int32_t ID );
void ugl_item_set_nextprevious( ugl_item_t *item, ugl_item_t *next_item, ugl_item_t *previous_item);
void ugl_item_set_sprite( ugl_item_t *item, ugl_sprite_t *sprite );
void ugl_item_set_text( ugl_item_t *item, ugl_text_t *text );
void ugl_item_set_position( ugl_item_t *item, int32_t position_x, uint32_t position_y );
void ugl_item_calc_position_abs( ugl_item_t *item, int32_t position_x, uint32_t position_y );
void ugl_item_render( ugl_item_t *item );

ugl_group_t *ugl_group_constructor( int32_t ID );
void ugl_group_destructor( ugl_group_t *group );
void ugl_group_add_item( ugl_group_t *group, ugl_item_t *item );
void ugl_group_add_group( ugl_group_t *group, ugl_group_t *groupToInsert );
void ugl_group_calc_position_abs( ugl_group_t *group, int32_t position_x, uint32_t position_y );
void ugl_group_set_position( ugl_group_t *group, int32_t position_x, uint32_t position_y );
ugl_group_t *ugl_group_get_group_by_id( ugl_group_t *group, int32_t ID );
ugl_item_t *ugl_group_get_item_by_id( ugl_group_t *group, int32_t ID );
void ugl_group_render( ugl_group_t *group );

void ugl_text_destructor( ugl_text_t *text );
ugl_text_t *ugl_text_constructor();
void ugl_text_set_test( ugl_text_t *text, char *string, FontDef *Font );
void ugl_text_render( ugl_text_t *text, uint32_t position_x, uint32_t position_y );

void ugl_sprite_destructor( ugl_sprite_t *sprite );
ugl_sprite_t *ugl_sprite_constructor();
void ugl_sprite_set_bitmap( ugl_sprite_t *sprite, uint32_t iconAddress, uint32_t width, uint32_t height );
void ugl_sprite_render( ugl_sprite_t *sprite, uint32_t position_x, uint32_t position_y );

void ugl_menu_destructor( ugl_menu_t *menu );
ugl_menu_t *ugl_menu_constructor( int32_t ID );
void ugl_menu_add_item( ugl_menu_t *menu, ugl_item_t *item );
void ugl_menu_add_group( ugl_menu_t *menu, ugl_group_t *group );
ugl_item_t *ugl_menu_get_item_by_id( ugl_menu_t *menu, int32_t ID );
void ugl_menu_render( ugl_menu_t *menu );
void ugl_menu_next_item( ugl_menu_t *menu );
void ugl_menu_previous_item( ugl_menu_t *menu );

#endif // __UGL_CORE_H__
