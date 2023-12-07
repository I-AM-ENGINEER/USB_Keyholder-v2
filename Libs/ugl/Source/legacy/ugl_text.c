#include "ugl_core.h"

void ugl_text_destructor( ugl_text_t *text ){
	if(text->string != NULL) free(text->string);
	free(text);
	return;
}

ugl_text_t *ugl_text_constructor(){
	ugl_text_t *text = malloc(sizeof(ugl_text_t));
	text->length = 0;
	text->string = NULL;
	text->visible = true;
	return text;
}

// This function bad, depricated
void ugl_text_set_test( ugl_text_t *text, char *string, FontDef *Font ){
	if(string == NULL) return;
	//if(*string == 0) return;
	
	if(text->string != NULL){
		free(text->string);
	}
	text->font = Font;
	text->string = malloc(strlen(string)+1);
	strcpy(text->string, string);
}

void ugl_text_render( ugl_text_t *text, uint32_t position_x, uint32_t position_y ){
	if(text->visible){
		ssd1306_SetCursor(position_x, position_y);
		ssd1306_WriteString(text->string, *text->font, White);
		//ssd1306_DrawBitmap(position_x, position_y, text->bitmap, text->bitmap_width, text->bitmap_height, White);
	}
}
