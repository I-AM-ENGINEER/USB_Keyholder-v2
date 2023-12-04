#include "ugl_core.h"

extern void UI_getData( uint32_t address, void *icon, uint32_t size );

void ugl_sprite_destructor( ugl_sprite_t *sprite ){
	free(sprite->bitmap);
	free(sprite);
	return;
}

ugl_sprite_t *ugl_sprite_constructor(){
	ugl_sprite_t *sprite = malloc(sizeof(ugl_sprite_t));
	sprite->bitmap = NULL;
	sprite->visible = true;
	sprite->bitmap_height = 0;
	sprite->bitmap_width = 0;
	return sprite;
}

// This function bad, depricated
void ugl_sprite_set_bitmap( ugl_sprite_t *sprite, uint32_t iconAddress, uint32_t width, uint32_t height ){
	if(sprite->bitmap != NULL){
		free(sprite->bitmap);
	}
	sprite->bitmap_height = height;
	sprite->bitmap_width = width;
	uint8_t *bitmap = malloc(width * height);
	UI_getData(iconAddress, bitmap, width*height);
	sprite->bitmap = bitmap;
}

void ugl_sprite_render( ugl_sprite_t *sprite, uint32_t position_x, uint32_t position_y ){
	if(sprite->visible){
		ssd1306_DrawBitmap(position_x, position_y, sprite->bitmap, sprite->bitmap_width, sprite->bitmap_height, White);
	}
}
