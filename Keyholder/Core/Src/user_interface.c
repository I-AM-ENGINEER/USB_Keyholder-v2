#include "user_interface.h"
#include "stdio.h"
#include "crypto.h"
#include "main.h"
#include "stdlib.h"
#include "usb.h"
#include "string.h"
#include "system.h"
//#include "usbd_cdc_if.h"
#include "buttons.h"

//http://javl.github.io/image2cpp/

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


UI_menu_t *currentMenu = NULL;

/**********   Events  *******************/

//UI_event_button_t *UI_event_buttonStack[IO_EVENT_MAX_STACK]


void UI_getData( uint32_t address, void *icon, uint32_t size ){
	switch(address){
		case UI_ICON_ADDRESS_FOLDER: 		memcpy(icon, img_folder, size); 		break;
		case UI_ICON_ADDRESS_SETTINGS: 	memcpy(icon, img_settings, size); 	break;
		case UI_ICON_ADDRESS_LOCK: 			memcpy(icon, img_lock, size); 			break;
		case UI_ICON_ADDRESS_USB_WRITE: memcpy(icon, img_usb_write, size); 	break;
		case UI_ICON_ADDRESS_PASSWORDS: memcpy(icon, img_passwords, size); 	break;
		default: 												memset(icon, 0x55, size); 					break;
	}
}

/**********   Sprite  *******************/

void UI_Sprite_Destructor( UI_sprite_t *sprite ){
	free(sprite->bitmap);
	free(sprite);
	return;
}

UI_sprite_t *UI_Sprite_Constructor(){
	UI_sprite_t *sprite = malloc(sizeof(UI_sprite_t));
	sprite->bitmap = NULL;
	sprite->visible = true;
	sprite->bitmapHeight = 0;
	sprite->bitmapWidth = 0;
	return sprite;
}

// This function bad, depricated
void UI_Sprite_SetBitmap( UI_sprite_t *sprite, uint32_t iconAddress, uint32_t width, uint32_t height ){
	if(sprite->bitmap != NULL){
		free(sprite->bitmap);
	}
	sprite->bitmapHeight = height;
	sprite->bitmapWidth = width;
	uint8_t *bitmap = malloc(width * height);
	UI_getData(iconAddress, bitmap, width*height);
	sprite->bitmap = bitmap;
}

void UI_Sprite_Render( UI_sprite_t *sprite, uint32_t positionX, uint32_t positionY ){
	if(sprite->visible){
		ssd1306_DrawBitmap(positionX, positionY, sprite->bitmap, sprite->bitmapWidth, sprite->bitmapHeight, White);
	}
}

/**********   Text  *******************/

void UI_Text_Destructor( UI_text_t *text ){
	if(text->string != NULL) free(text->string);
	free(text);
	return;
}

UI_text_t *UI_Text_Constructor(){
	UI_text_t *text = malloc(sizeof(UI_text_t));
	text->length = 0;
	text->string = NULL;
	text->visible = true;
	return text;
}

// This function bad, depricated
void UI_Text_SetText( UI_text_t *text, char *string, FontDef *Font ){
	if(text->string != NULL){
		free(text->string);
	}
	if(*string == 0) return;
	text->font = Font;
	text->string = malloc(strlen(string)+1);
	strcpy(text->string, string);
}

void UI_Text_Render( UI_text_t *text, uint32_t positionX, uint32_t positionY ){
	if(text->visible){
		ssd1306_SetCursor(positionX, positionY);
		ssd1306_WriteString(text->string, *text->font, White);
		//ssd1306_DrawBitmap(positionX, positionY, text->bitmap, text->bitmapWidth, text->bitmapHeight, White);
	}
}

/**********   Item  *******************/

void UI_Item_Destructor( UI_item_t *item ){
	if(item->type == UI_ITEM_SPRITE){
		UI_Sprite_Destructor(item->sprite);
	}else if(item->type == UI_ITEM_TEXT){
		UI_Text_Destructor(item->text);
		//UI_Text_Destructor(item->text);
	}
	free(item);
	return;
}

UI_item_t *UI_Item_Constructor( uint32_t ID ){
	UI_item_t *item = malloc(sizeof(UI_item_t));
	item->ID = ID;
	item->type = UI_ITEM_NONE;
	item->nextItem = NULL;
	item->previousItem = NULL;
	item->positionX = 0;
	item->positionY = 0;
	item->positionXabs = 0;
	item->positionYabs = 0;
	return item;
}

void UI_Item_SetNextPrevious( UI_item_t *item, \
			UI_item_t *nextItem, \
			UI_item_t *previousItem)
{
	item->nextItem = nextItem;
	item->previousItem = previousItem;
}

void UI_Item_SetSprite( UI_item_t *item, UI_sprite_t *sprite ){
	item->type = UI_ITEM_SPRITE;
	item->sprite = sprite;
}

void UI_Item_SetText( UI_item_t *item, UI_text_t *text ){
	item->type = UI_ITEM_TEXT;
	item->text = text;
}

void UI_Item_SetPosition( UI_item_t *item, int32_t positionX, uint32_t positionY ){
	item->positionX = positionX;
	item->positionY = positionY;
}

void UI_Item_CalcPositionAbs( UI_item_t *item, int32_t positionX, uint32_t positionY ){
	item->positionXabs = positionX + item->positionX;
	item->positionYabs = positionY + item->positionY;
}

void UI_Item_Render( UI_item_t *item ){
	if(item->type == UI_ITEM_SPRITE){
		UI_Sprite_Render( item->sprite, item->positionXabs, item->positionYabs );
	}else if(item->type == UI_ITEM_TEXT){
		UI_Text_Render( item->text, item->positionXabs, item->positionYabs );
	}
}

/**********   Group  *******************/

void UI_Group_Destructor( UI_group_t *group ){
	for(uint32_t i = 0; i < group->groupsCount; i++){
		UI_Group_Destructor(group->groups[i]);
	}
	for(uint32_t i = 0; i < group->itemsCount; i++){
		UI_Item_Destructor(group->items[i]);
	}
	free(group->groups);
	free(group->items);
	free(group);
}

UI_group_t *UI_Group_Constructor( uint32_t ID ){
	UI_group_t *group = malloc(sizeof(UI_item_t));
	group->ID = ID;
	group->positionXabs = 0;
	group->positionYabs = 0;
	group->positionX = 0;
	group->positionY = 0;
	group->itemsCount = 0;
	group->groupsCount = 0;
	group->items = NULL;
	group->groups = NULL;
	return group;
}

void UI_Group_AddItem( UI_group_t *group, UI_item_t *item ){
	if(group->items == NULL){
		group->items = malloc(sizeof(void *));
	}else{
		group->items = realloc(group->items, (group->itemsCount + 1) * sizeof(void *));//(menu->spritesCount + 1) * sizeof(sprite)
	}
	group->items[group->itemsCount++] = item;
}

void UI_Group_AddGroup( UI_group_t *group, UI_group_t *groupToInsert ){
	if(group->groups == NULL){
		group->groups = malloc(sizeof(void *));
	}else{
		group->groups = realloc(group->groups, (group->groupsCount + 1) * sizeof(void *));//(menu->spritesCount + 1) * sizeof(sprite)
	}
	group->groups[group->groupsCount++] = groupToInsert;
}

void UI_Group_CalcPositionAbs( UI_group_t *group, int32_t positionX, uint32_t positionY ){
	group->positionXabs = positionX + group->positionX;
	group->positionYabs = positionY + group->positionY;
	for(uint32_t i = 0; i < group->itemsCount; i++){
		UI_Item_CalcPositionAbs(group->items[i], group->positionXabs, group->positionYabs);
	}
	for(uint32_t i = 0; i < group->groupsCount; i++){
		UI_Group_CalcPositionAbs(group->groups[i], group->positionXabs, group->positionYabs);
	}
}

void UI_Group_SetPosition( UI_group_t *group, int32_t positionX, uint32_t positionY ){
	group->positionX = positionX;
	group->positionY = positionY;
}

UI_group_t *UI_Group_GetGroupByID( UI_group_t *group, uint32_t ID ){
	if(group->ID == ID) return group;
	for(uint32_t i = 0; i < group->groupsCount; i++){
		UI_group_t *tmp = UI_Group_GetGroupByID(group->groups[i], ID);
		if(tmp->ID == ID) return tmp;
	}
	return NULL;
}

UI_item_t *UI_Group_GetItemByID( UI_group_t *group, uint32_t ID ){
	for(uint32_t i = 0; i < group->itemsCount; i++){
		if(group->items[i]->ID == ID) return group->items[i];
	}
	
	for(uint32_t i = 0; i < group->groupsCount; i++){
		UI_item_t *tmp = UI_Group_GetItemByID(group->groups[i], ID);
		if(tmp->ID == ID) return tmp;
	}
	return NULL;
}

void UI_Group_Render( UI_group_t *group ){
	for(uint32_t i = 0; i < group->itemsCount; i++){
		UI_Item_Render(group->items[i]);
	}
	for(uint32_t i = 0; i < group->groupsCount; i++){
		UI_Group_Render(group->groups[i]);
	}
}

/**********   Menu  *******************/

void UI_Menu_Destructor( UI_menu_t *menu ){
	UI_Group_Destructor(menu->group);
	free(menu);
}

UI_menu_t *UI_Menu_Constructor( uint32_t ID ){
	UI_menu_t *menu = malloc(sizeof(UI_menu_t));
	menu->ID = ID;
	menu->group = UI_Group_Constructor(0);
	menu->drawingFunction = NULL;
	menu->selectedItem = NULL;
	menu->callerPtr = NULL;
	return menu;
}

void UI_Menu_AddItem( UI_menu_t *menu, UI_item_t *item ){
	UI_Group_AddItem(menu->group, item);
}

void UI_Menu_AddGroup( UI_menu_t *menu, UI_group_t *group ){
	UI_Group_AddGroup(menu->group, group);
}

UI_item_t *UI_Menu_GetItemByID( UI_menu_t *menu, uint32_t ID ){
	return UI_Group_GetItemByID(menu->group, ID);
}

void UI_menuRender( UI_menu_t *menu ){
	UI_Group_CalcPositionAbs( menu->group, 0, 0 );
	UI_Group_Render(menu->group);
}

void UI_menu_Return( void ){
	if(currentMenu->callerPtr == NULL) return;
	UI_menu_t *returnMenu = currentMenu->callerPtr;
	UI_Menu_Destructor(currentMenu);
	currentMenu = returnMenu;
}

/********** Buttons *******************/

UI_event_button_t *UI_event_buttonStack = NULL;
uint32_t UI_event_buttonStackSize = 0;
uint32_t UI_event_buttonCount = 0;

UI_event_button_t* UI_event_GetLast( void ){
	if(UI_event_buttonCount == 0) return NULL;
	return &UI_event_buttonStack[UI_event_buttonCount - 1];
}

void UI_event_ClearLast( void ){
	if(UI_event_buttonCount == 0) return;
	UI_event_buttonCount--;
	UI_event_buttonStack[UI_event_buttonCount].eventType = BUTTON_STATE_IDLE;
}

void UI_event_SetButton( uint32_t buttonID, BTN_buttonState_t state ){
	if(UI_event_buttonCount >= UI_event_buttonStackSize) return;
	UI_event_buttonStack[UI_event_buttonCount].buttonID  = buttonID;
	UI_event_buttonStack[UI_event_buttonCount].eventType = state;
	UI_event_buttonCount++;
	return;
}

/**********   User  *******************/

void UI_switchToLoginMenu( uint32_t ID );

void UI_mainMenuDraw( void ){
	ssd1306_Fill(Black);
	static BTN_IDs holdedButton = BTN_NONE;
	static uint32_t lastSwipeTimestamp = 0;
	
	
	UI_event_button_t* lastButton = UI_event_GetLast();
	switch(lastButton->eventType){
		case BUTTON_STATE_PRESSED:
			if(lastButton->buttonID == BTN_JCW_ID) currentMenu->selectedItem = currentMenu->selectedItem->nextItem;
			else if(lastButton->buttonID == BTN_JCCW_ID) currentMenu->selectedItem = currentMenu->selectedItem->previousItem;
			break;
		case BUTTON_STATE_HOLDED:
			holdedButton = lastButton->buttonID;
			break;
		case BUTTON_STATE_RELEASED:
			holdedButton = BTN_NONE;
			break;
		default: break;
	}
	
	
	if((lastButton->eventType == BUTTON_STATE_PRESSED) && (lastButton->buttonID == BTN_JPUSH_ID)){
		switch(currentMenu->selectedItem->ID){
			case UI_MENU_MAIN_LOCK: 
				UI_switchToLoginMenu(1);
				return;
				break;
			default: break;
		}
	}
	
	UI_event_ClearLast();
		
	// Holded swipe
	if(((HAL_GetTick() - lastSwipeTimestamp) > 200) && ((holdedButton == BTN_JCW_ID) || (holdedButton == BTN_JCCW_ID))){
		if(holdedButton == BTN_JCW_ID) 	currentMenu->selectedItem = currentMenu->selectedItem->nextItem;
		else 														currentMenu->selectedItem = currentMenu->selectedItem->previousItem;
		lastSwipeTimestamp = HAL_GetTick();
	}
	
	
	if(currentMenu->selectedItem->positionXabs > 50) {
		currentMenu->group->positionX -= 5;
	}else if(currentMenu->selectedItem->positionXabs < 50){
		currentMenu->group->positionX += 5;
	}
	
	UI_menuRender( currentMenu );
	
	ssd1306_DrawRectangle(currentMenu->selectedItem->positionXabs, currentMenu->selectedItem->positionYabs, currentMenu->selectedItem->positionXabs+10, currentMenu->selectedItem->positionYabs+10, White);
	ssd1306_DrawRectangle(0, 0,  127, 10, White);
	ssd1306_DrawRectangle(0, 53, 127, 63, White);
	ssd1306_Line(1, 0,  126, 0,  Black);
	ssd1306_Line(1, 63, 126, 63, Black);
	
	
	ssd1306_SetCursor(2,0);
	ssd1306_WriteString("AAAA", Font_7x10, White);
	ssd1306_Line(31, 0, 31, 10, White);
	ssd1306_SetCursor(33,0);
	ssd1306_WriteString("KEY2", Font_7x10, White);
	ssd1306_Line(63, 0, 63, 10, White);
	ssd1306_SetCursor(65,0);
	ssd1306_WriteString("WIND", Font_7x10, White);
	ssd1306_Line(95, 0, 95, 10, White);
	ssd1306_SetCursor(97,0);
	ssd1306_WriteString("SSH4", Font_7x10, White);
	
	
	
	ssd1306_SetCursor(2,55);
	ssd1306_WriteString("TEST", Font_7x10, White);
	ssd1306_Line(31, 53, 31, 63, White);
	ssd1306_SetCursor(33,55);
	ssd1306_WriteString("FUCK", Font_7x10, White);
	ssd1306_Line(63, 53, 63, 63, White);
	ssd1306_SetCursor(65,55);
	ssd1306_WriteString("SHIT", Font_7x10, White);
	ssd1306_Line(95, 53, 95, 63, White);
	ssd1306_SetCursor(97,55);
	ssd1306_WriteString("TEXT", Font_7x10, White);
}

void UI_loginMenuDraw( void ){
	UI_event_button_t* lastButton = UI_event_GetLast();
	
	ssd1306_Fill(Black);
	
	static uint32_t cursor = 0;
	static char pswd[6] = "      ";
	
	switch(lastButton->eventType){
		case BUTTON_STATE_PRESSED:
			if((lastButton->buttonID >= BTN_SW1_ID) && (lastButton->buttonID <= BTN_SW8_ID)){
				pswd[cursor] = lastButton->buttonID + '1';
				if((cursor < 5) && (pswd[cursor+1] == ' ')){ 
					cursor++;
				}
			}else if(lastButton->buttonID == BTN_JCW_ID) {
				if(cursor < 5) cursor++;
			}
			else if(lastButton->buttonID == BTN_JCCW_ID) {
				if(cursor > 0) cursor--;
			}
			break;
		default: break;
	}
	UI_event_ClearLast();
	
	/*for(uint32_t i = 0; i < 7; i++){
		ssd1306_Line(6 + i*19, 17, 6 + i*19, 46, White);
	}*/
	
	bool stringFilled = true;
	for(uint32_t i = 0; i < 6; i++){
		if(pswd[i] == ' '){
			stringFilled = false;
		}
	}
	
	if(stringFilled){
		cursor = 0;
		if(!memcmp(pswd, "111111", 6)){
			memset(pswd, ' ', 6);
			UI_menu_Return();
			return;
		}else{
			memset(pswd, ' ', 6);
		}
	}
	
	
	
	for(uint32_t i = 0; i < 6; i++){
		ssd1306_SetCursor(6 + i*19, 20);
		if(i == cursor){
			ssd1306_WriteChar(pswd[i], Font_16x26, White);
		}else if((i == (cursor-1)) && pswd[cursor] == ' '){
			ssd1306_WriteChar(pswd[i], Font_16x26, White);
		}else if(pswd[i] != ' '){
			ssd1306_WriteChar('*', Font_16x26, White);
		}
		
		
		ssd1306_Line(6 + i*19, 43, 22 + i*19, 43, White);
		ssd1306_Line(6 + i*19, 17, 22 + i*19, 17, White);
	}
	ssd1306_Line(6 + cursor*19, 45, 22 + cursor*19, 45, White);
	
	ssd1306_DrawRectangle(0, 0,  127, 10, White);
	ssd1306_DrawRectangle(0, 53, 127, 63, White);
	ssd1306_Line(1, 0,  126, 0,  Black);
	ssd1306_Line(1, 63, 126, 63, Black);
	
	uint32_t offset = 11;
	
	ssd1306_SetCursor(2+offset,0);
	ssd1306_WriteString("1", Font_7x10, White);
	ssd1306_Line(31, 0, 31, 10, White);
	ssd1306_SetCursor(33+offset,0);
	ssd1306_WriteString("2", Font_7x10, White);
	ssd1306_Line(63, 0, 63, 10, White);
	ssd1306_SetCursor(65+offset,0);
	ssd1306_WriteString("3", Font_7x10, White);
	ssd1306_Line(95, 0, 95, 10, White);
	ssd1306_SetCursor(97+offset,0);
	ssd1306_WriteString("4", Font_7x10, White);
	
	
	
	ssd1306_SetCursor(2+offset,55);
	ssd1306_WriteString("5", Font_7x10, White);
	ssd1306_Line(31, 53, 31, 63, White);
	ssd1306_SetCursor(33+offset,55);
	ssd1306_WriteString("6", Font_7x10, White);
	ssd1306_Line(63, 53, 63, 63, White);
	ssd1306_SetCursor(65+offset,55);
	ssd1306_WriteString("7", Font_7x10, White);
	ssd1306_Line(95, 53, 95, 63, White);
	ssd1306_SetCursor(97+offset,55);
	ssd1306_WriteString("8", Font_7x10, White);
	
	
	//ssd1306_DrawRectangle(10, 10, 30, 40, White);
	
	//ssd1306_WriteString(currentMenu->group->items[0]->text->string, Font_7x10, White);
	
	UI_menuRender( currentMenu );
}

UI_menu_t *UI_loginMenuConstructor( uint32_t ID ){
	UI_menu_t *menu = UI_Menu_Constructor(ID);
	//UI_item_t *item = NULL;
	//UI_text_t *text = NULL;
	
	/*
	item = UI_Item_Constructor(0);
	UI_Item_SetText(item, UI_Text_Constructor());
	UI_Text_SetText(item->text, "      ", &Font_16x26);
	UI_Menu_AddItem(menu, item);
	
	UI_Item_SetPosition(UI_Menu_GetItemByID(menu, 0), 0,  0);
	
	
	menu->group->positionX = 16;
	menu->group->positionY = 18;
	*/
	menu->drawingFunction = UI_loginMenuDraw;
	
	return menu;
}

void UI_switchToLoginMenu( uint32_t ID ){
	UI_menu_t *oldMenu = currentMenu;
	currentMenu = UI_loginMenuConstructor(ID);
	currentMenu->callerPtr = oldMenu;
}

UI_menu_t *UI_mainMenuConstructor( void ){
	UI_menu_t *mainMenu = UI_Menu_Constructor(0);
	UI_item_t *item = NULL;
	UI_sprite_t *sprite = NULL;
	
	mainMenu->drawingFunction = UI_mainMenuDraw;
	
	item = UI_Item_Constructor(UI_MENU_MAIN_SETTINGS);
	UI_Item_SetSprite(item, UI_Sprite_Constructor());
	UI_Sprite_SetBitmap(item->sprite, UI_ICON_ADDRESS_SETTINGS, MAIN_MENU_ICONS_WIDTH, MAIN_MENU_ICONS_HEIGHT);
	UI_Menu_AddItem(mainMenu, item);
	
	item = UI_Item_Constructor(UI_MENU_MAIN_PASSWORDS);
	UI_Item_SetSprite(item, UI_Sprite_Constructor());
	UI_Sprite_SetBitmap(item->sprite, UI_ICON_ADDRESS_PASSWORDS, MAIN_MENU_ICONS_WIDTH, MAIN_MENU_ICONS_HEIGHT);
	UI_Menu_AddItem(mainMenu, item);
	
	item = UI_Item_Constructor(UI_MENU_MAIN_FOLDER);
	UI_Item_SetSprite(item, UI_Sprite_Constructor());
	UI_Sprite_SetBitmap(item->sprite, UI_ICON_ADDRESS_FOLDER, MAIN_MENU_ICONS_WIDTH, MAIN_MENU_ICONS_HEIGHT);
	UI_Menu_AddItem(mainMenu, item);
	
	item = UI_Item_Constructor(UI_MENU_MAIN_LOCK);
	UI_Item_SetSprite(item, UI_Sprite_Constructor());
	UI_Sprite_SetBitmap(item->sprite, UI_ICON_ADDRESS_LOCK, MAIN_MENU_ICONS_WIDTH, MAIN_MENU_ICONS_HEIGHT);
	UI_Menu_AddItem(mainMenu, item);
	
	item = UI_Item_Constructor(UI_MENU_MAIN_USB_WRITE);
	UI_Item_SetSprite(item, UI_Sprite_Constructor());
	UI_Sprite_SetBitmap(item->sprite, UI_ICON_ADDRESS_USB_WRITE, MAIN_MENU_ICONS_WIDTH, MAIN_MENU_ICONS_HEIGHT);
	UI_Menu_AddItem(mainMenu, item);
	
	
	UI_Item_SetPosition(UI_Menu_GetItemByID(mainMenu, 0), 0,  0);
	UI_Item_SetPosition(UI_Menu_GetItemByID(mainMenu, 1), 35, 0);
	UI_Item_SetPosition(UI_Menu_GetItemByID(mainMenu, 2), 70, 0);
	UI_Item_SetPosition(UI_Menu_GetItemByID(mainMenu, 3), 105, 0);
	UI_Item_SetPosition(UI_Menu_GetItemByID(mainMenu, 4), 140, 0);
	
	UI_Item_SetNextPrevious(\
		UI_Menu_GetItemByID(mainMenu, 0),\
		UI_Menu_GetItemByID(mainMenu, 1),\
		UI_Menu_GetItemByID(mainMenu, 0));
	UI_Item_SetNextPrevious(\
		UI_Menu_GetItemByID(mainMenu, 1),\
		UI_Menu_GetItemByID(mainMenu, 2),\
		UI_Menu_GetItemByID(mainMenu, 0));
	UI_Item_SetNextPrevious(\
		UI_Menu_GetItemByID(mainMenu, 2),\
		UI_Menu_GetItemByID(mainMenu, 3),\
		UI_Menu_GetItemByID(mainMenu, 1));
	UI_Item_SetNextPrevious(\
		UI_Menu_GetItemByID(mainMenu, 3),\
		UI_Menu_GetItemByID(mainMenu, 4),\
		UI_Menu_GetItemByID(mainMenu, 2));
	UI_Item_SetNextPrevious(\
		UI_Menu_GetItemByID(mainMenu, 4),\
		UI_Menu_GetItemByID(mainMenu, 4),\
		UI_Menu_GetItemByID(mainMenu, 3));
	
	mainMenu->group->positionY = 17;
	mainMenu->group->positionX = -15;
	mainMenu->selectedItem = UI_Menu_GetItemByID(mainMenu, 3);
	
	return mainMenu;
}

void UI_Init( void ){
	UI_event_buttonStack = malloc(sizeof(UI_event_button_t)*IO_EVENT_MAX_STACK);
	UI_event_buttonStackSize = IO_EVENT_MAX_STACK;
	
	for(uint32_t i; i < UI_event_buttonStackSize; i++) UI_event_buttonStack[i].eventType = BUTTON_STATE_IDLE;
	currentMenu = UI_mainMenuConstructor();
}

void UI_print_menu( void ){
	if(currentMenu->drawingFunction != NULL){
		currentMenu->drawingFunction();
	}
	
	ssd1306_UpdateScreen();
}
