#include "ssd1306.h"
#include "main.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>  // For memcpy

#if defined(SSD1306_USE_I2C)

void ssd1306_Reset(void) {
    /* for I2C - do nothing */
}

// Send a byte to the command register
void ssd1306_WriteCommand(uint8_t byte) {
    HAL_I2C_Mem_Write(&SSD1306_I2C_PORT, SSD1306_I2C_ADDR, 0x00, 1, &byte, 1, HAL_MAX_DELAY);
}

// Send data
void ssd1306_WriteData(uint8_t* buffer, size_t buff_size) {
    HAL_I2C_Mem_Write(&SSD1306_I2C_PORT, SSD1306_I2C_ADDR, 0x40, 1, buffer, buff_size, HAL_MAX_DELAY);
}

#elif defined(SSD1306_USE_SPI)

#include "tim.h"

#define SSPI_WRITE_BIT(bit_mask) \
	OLES_SSPI_SCK_GPIO_Port->BSRR = OLES_SSPI_SCK_Pin << 16;\
	if (dat & bit_mask) OLES_SSPI_MOSI_GPIO_Port->BSRR = OLES_SSPI_MOSI_Pin;\
	else OLES_SSPI_MOSI_GPIO_Port->BSRR = OLES_SSPI_MOSI_Pin << 16;\
	OLES_SSPI_SCK_GPIO_Port->BSRR = OLES_SSPI_SCK_Pin;

void sspi_w(uint32_t dat) {
	__disable_irq();
	
	
	/*
	for (register uint32_t sspi_i = 0x80; sspi_i != 0x00; sspi_i >>= 1) {
		OLES_SSPI_SCK_GPIO_Port->BSRR = OLES_SSPI_SCK_Pin << 16;
		if (dat & sspi_i) OLES_SSPI_MOSI_GPIO_Port->BSRR = OLES_SSPI_MOSI_Pin;
		else OLES_SSPI_MOSI_GPIO_Port->BSRR = OLES_SSPI_MOSI_Pin << 16;
		OLES_SSPI_SCK_GPIO_Port->BSRR = OLES_SSPI_SCK_Pin;
  }*/
	
	SSPI_WRITE_BIT(0x80);
	SSPI_WRITE_BIT(0x40);
	SSPI_WRITE_BIT(0x20);
	SSPI_WRITE_BIT(0x10);
	SSPI_WRITE_BIT(0x08);
	SSPI_WRITE_BIT(0x04);
	SSPI_WRITE_BIT(0x02);
	SSPI_WRITE_BIT(0x01);
	
	
	__enable_irq();
}


void ssd1306_Reset(void) {
    // CS = High (not selected)
    //HAL_GPIO_WritePin(SSD1306_CS_Port, SSD1306_CS_Pin, GPIO_PIN_SET);

    // Reset the OLED
    HAL_GPIO_WritePin(SSD1306_Reset_Port, SSD1306_Reset_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(SSD1306_Reset_Port, SSD1306_Reset_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
}

// Send a byte to the command register
void ssd1306_WriteCommand(uint8_t byte) {
    //HAL_GPIO_WritePin(SSD1306_CS_Port, SSD1306_CS_Pin, GPIO_PIN_RESET); // select OLED
    HAL_GPIO_WritePin(SSD1306_DC_Port, SSD1306_DC_Pin, GPIO_PIN_RESET); // command
		sspi_w(byte);
    //HAL_SPI_Transmit(&hspi2, (uint8_t *) &byte, 1, 1000);
    //HAL_GPIO_WritePin(SSD1306_CS_Port, SSD1306_CS_Pin, GPIO_PIN_SET); // un-select OLED
}

// Send data
void ssd1306_WriteData(uint8_t* buffer, size_t buff_size) {
    //HAL_GPIO_WritePin(SSD1306_CS_Port, SSD1306_CS_Pin, GPIO_PIN_RESET); // select OLED
    HAL_GPIO_WritePin(SSD1306_DC_Port, SSD1306_DC_Pin, GPIO_PIN_SET); // data
		for(register uint32_t i = 0; i < buff_size; i++){
			sspi_w(buffer[i]);
		}
		
    //HAL_SPI_Transmit(&hspi2, buffer, buff_size, 1000);
    //HAL_GPIO_WritePin(SSD1306_CS_Port, SSD1306_CS_Pin, GPIO_PIN_SET); // un-select OLED
}

#else
#error "You should define SSD1306_USE_SPI or SSD1306_USE_I2C macro"
#endif


// Screenbuffer
static uint8_t SSD1306_Buffer[SSD1306_BUFFER_SIZE];

// Screen object
static SSD1306_t SSD1306;

/* Fills the Screenbuffer with values from a given buffer of a fixed length */
SSD1306_Error_t ssd1306_FillBuffer(uint8_t* buf, uint32_t len) {
    SSD1306_Error_t ret = SSD1306_ERR;
    if (len <= SSD1306_BUFFER_SIZE) {
        memcpy(SSD1306_Buffer,buf,len);
        ret = SSD1306_OK;
    }
    return ret;
}




void ssd1306_Init(void) {
	// Reset OLED
  ssd1306_Reset();

  // Wait for the screen to boot
  HAL_Delay(1);

  // Init OLED
  //ssd1306_SetDisplayOn(0); //display off
	
	// Set multiplex ratio (visible lines)
	ssd1306_WriteCommand(0xA8); // 128MUX
	ssd1306_WriteCommand(0xFF);
	// Set display offset (offset of first line from the top of display)
	ssd1306_WriteCommand(0xD3); // Offset: 0
	ssd1306_WriteCommand(0);
	// Set display start line (first line displayed)
	ssd1306_WriteCommand(0x40  | 0x00); // Start line: 0
	// Set segment re-map (X coordinate)
	ssd1306_WriteCommand(0xA0);
	// Set COM output scan direction (Y coordinate)
	ssd1306_WriteCommand(0xC8);
	
	// Set COM pins hardware configuration
	// bit[4]: reset - sequential COM pin configuration
	//         set   - alternative COM pin configuration (reset value)
	// bit[5]: reset - disable COM left/right remap (reset value)
	//         set   - enable COM left/right remap
	ssd1306_WriteCommand(0xDA);
	ssd1306_WriteCommand(0x12);
	
	uint8_t dis_charge = 0x00;
	uint8_t pre_charge = 0x00;
	
	ssd1306_WriteCommand(0x22);
	ssd1306_WriteCommand(dis_charge | (pre_charge << 4));
	
	// Set contrast control
	ssd1306_SetContrast(0x4F);
	
	ssd1306_WriteCommand(0x30);
	
	
	
	// Disable entire display ON
	ssd1306_WriteCommand(0xA4); // Display follows RAM content

	// Disable display inversion
	ssd1306_WriteCommand(0xA6); // Normal display mode
	
	
	// Page adressing !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//ssd1306_WriteCommand(0x21);
	
	// Set clock divide ratio and oscillator frequency
	// bits[3:0] defines the divide ratio of the display clocks (bits[3:0] + 1)
	// bits[7:4] set the oscillator frequency (Fosc), frequency increases with the value of these bits
	// 0xF0 value gives maximum frequency (maximum Fosc without divider)
	// 0x0F value gives minimum frequency (minimum Fosc divided by 16)
	// The higher display frequency decreases image flickering but increases current consumption and vice versa
	ssd1306_WriteCommand(0xD5);
	ssd1306_WriteCommand(0xF0);
	// Display ON
	ssd1306_WriteCommand(0xAF ); // Display enabled
	
	ssd1306_SetDisplayOn(1); //--turn on SSD1306 panel

  // Clear screen
  ssd1306_Fill(Black);
  
  // Flush buffer to screen
  ssd1306_UpdateScreen();
	
	// Set default values for screen object
  SSD1306.CurrentX = 0;
  SSD1306.CurrentY = 0;
	
	SSD1306.Initialized = 1;
}

// Fill the whole screen with the given color
void ssd1306_Fill(SSD1306_COLOR color) {
    /* Set memory */
    uint32_t i;

    for(i = 0; i < sizeof(SSD1306_Buffer); i++) {
        SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
    }
}

// Write the screenbuffer with changed to the screen
void ssd1306_UpdateScreen(void) {
    // Write data to each page of RAM. Number of pages
    // depends on the screen height:
    //
    //  * 32px   ==  4 pages
    //  * 64px   ==  8 pages
    //  * 128px  ==  16 pages
    //for(uint8_t i = 0; i < SSD1306_HEIGHT/4; i++) {
	
		for(uint8_t i = 0; i < 16; i++) {
			ssd1306_WriteCommand(0x00);
			ssd1306_WriteCommand(0x10 | 0x02);
			//ssd1306_WriteCommand(0x40 | 0x04);
			ssd1306_WriteCommand(0xB0 | i);
			
			ssd1306_WriteData(&SSD1306_Buffer[SSD1306_WIDTH*i],64);
    }
}

//    Draw one pixel in the screenbuffer
//    X => X Coordinate
//    Y => Y Coordinate
//    color => Pixel color
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color) {
    if(x >= SSD1306_HEIGHT || y >=  SSD1306_WIDTH) {
        // Don't write outside the buffer
        return;
    }
   /*
    // Draw in the right color
    if(color == White) {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    } else { 
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }*/
		
		
		
    // Draw in the right color
    if(color == White) {
        SSD1306_Buffer[y + (x / 8) * SSD1306_WIDTH] |= 1 << (x % 8);
    } else { 
        SSD1306_Buffer[y + (x / 8) * SSD1306_WIDTH] &= ~(1 << (x % 8));
    }
}

// Draw 1 char to the screen buffer
// ch       => char om weg te schrijven
// Font     => Font waarmee we gaan schrijven
// color    => Black or White
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color) {
    uint32_t i, b, j;
    
    // Check if character is valid
    if (ch < 32 || ch > 126)
        return 0;
    
    // Check remaining space on current line
   // if (SSD1306_WIDTH < (SSD1306.CurrentY + Font.FontWidth) ||
    //    SSD1306_HEIGHT < (SSD1306.CurrentX + Font.FontHeight))
    //{
        // Not enough space on current line
    //    return 0;
    //}
    
    // Use the font to write
    for(i = 0; i < Font.FontHeight; i++) {
        b = Font.data[(ch - 32) * Font.FontHeight + i];
        for(j = 0; j < Font.FontWidth; j++) {
            if((b << j) & 0x8000)  {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
            } else {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
            }
        }
    }
    
    // The current space is now taken
    SSD1306.CurrentX += Font.FontWidth;
    
    // Return written char for validation
    return ch;
}

// Write full string to screenbuffer
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color) {
    // Write until null-byte
    while (*str) {
        if (ssd1306_WriteChar(*str, Font, color) != *str) {
            // Char could not be written
            return *str;
        }
        
        // Next char
        str++;
    }
    
    // Everything ok
    return *str;
}

// Position the cursor
void ssd1306_SetCursor(uint8_t x, uint8_t y) {
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}

// Draw line by Bresenhem's algorithm
void ssd1306_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color) {
  int32_t deltaX = abs(x2 - x1);
  int32_t deltaY = abs(y2 - y1);
  int32_t signX = ((x1 < x2) ? 1 : -1);
  int32_t signY = ((y1 < y2) ? 1 : -1);
  int32_t error = deltaX - deltaY;
  int32_t error2;
    
  ssd1306_DrawPixel(x2, y2, color);
    while((x1 != x2) || (y1 != y2))
    {
    ssd1306_DrawPixel(x1, y1, color);
    error2 = error * 2;
    if(error2 > -deltaY)
    {
      error -= deltaY;
      x1 += signX;
    }
    else
    {
    /*nothing to do*/
    }
        
    if(error2 < deltaX)
    {
      error += deltaX;
      y1 += signY;
    }
    else
    {
    /*nothing to do*/
    }
  }
  return;
}
//Draw polyline
void ssd1306_Polyline(const SSD1306_VERTEX *par_vertex, uint16_t par_size, SSD1306_COLOR color) {
  uint16_t i;
  if(par_vertex != 0){
    for(i = 1; i < par_size; i++){
      ssd1306_Line(par_vertex[i - 1].x, par_vertex[i - 1].y, par_vertex[i].x, par_vertex[i].y, color);
    }
  }
  else
  {
    /*nothing to do*/
  }
  return;
}
/*Convert Degrees to Radians*/
static float ssd1306_DegToRad(float par_deg) {
    return par_deg * 3.14 / 180.0;
}
/*Normalize degree to [0;360]*/
static uint16_t ssd1306_NormalizeTo0_360(uint16_t par_deg) {
  uint16_t loc_angle;
  if(par_deg <= 360)
  {
    loc_angle = par_deg;
  }
  else
  {
    loc_angle = par_deg % 360;
    loc_angle = ((par_deg != 0)?par_deg:360);
  }
  return loc_angle;
}
/*DrawArc. Draw angle is beginning from 4 quart of trigonometric circle (3pi/2)
 * start_angle in degree
 * sweep in degree
 */
void ssd1306_DrawArc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_COLOR color) {
    #define CIRCLE_APPROXIMATION_SEGMENTS 36
    float approx_degree;
    uint32_t approx_segments;
    uint8_t xp1,xp2;
    uint8_t yp1,yp2;
    uint32_t count = 0;
    uint32_t loc_sweep = 0;
    float rad;
    
    loc_sweep = ssd1306_NormalizeTo0_360(sweep);
    
    count = (ssd1306_NormalizeTo0_360(start_angle) * CIRCLE_APPROXIMATION_SEGMENTS) / 360;
    approx_segments = (loc_sweep * CIRCLE_APPROXIMATION_SEGMENTS) / 360;
    approx_degree = loc_sweep / (float)approx_segments;
    while(count < approx_segments)
    {
        rad = ssd1306_DegToRad(count*approx_degree);
        xp1 = x + (int8_t)(sin(rad)*radius);
        yp1 = y + (int8_t)(cos(rad)*radius);    
        count++;
        if(count != approx_segments)
        {
            rad = ssd1306_DegToRad(count*approx_degree);
        }
        else
        {            
            rad = ssd1306_DegToRad(loc_sweep);
        }
        xp2 = x + (int8_t)(sin(rad)*radius);
        yp2 = y + (int8_t)(cos(rad)*radius);    
        ssd1306_Line(xp1,yp1,xp2,yp2,color);
    }
    
    return;
}
//Draw circle by Bresenhem's algorithm
void ssd1306_DrawCircle(uint8_t par_x,uint8_t par_y,uint8_t par_r,SSD1306_COLOR par_color) {
  int32_t x = -par_r;
  int32_t y = 0;
  int32_t err = 2 - 2 * par_r;
  int32_t e2;

  //if (par_x >= SSD1306_WIDTH || par_y >= SSD1306_HEIGHT) {
    //return;
  //}

    do {
      ssd1306_DrawPixel(par_x - x, par_y + y, par_color);
      ssd1306_DrawPixel(par_x + x, par_y + y, par_color);
      ssd1306_DrawPixel(par_x + x, par_y - y, par_color);
      ssd1306_DrawPixel(par_x - x, par_y - y, par_color);
        e2 = err;
        if (e2 <= y) {
            y++;
            err = err + (y * 2 + 1);
            if(-x == y && e2 <= x) {
              e2 = 0;
            }
            else
            {
              /*nothing to do*/
            }
        }
        else
        {
          /*nothing to do*/
        }
        if(e2 > x) {
          x++;
          err = err + (x * 2 + 1);
        }
        else
        {
          /*nothing to do*/
        }
    } while(x <= 0);

    return;
}

//Draw rectangle
void ssd1306_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color) {
  ssd1306_Line(x1,y1,x2,y1,color);
  ssd1306_Line(x2,y1,x2,y2,color);
  ssd1306_Line(x2,y2,x1,y2,color);
  ssd1306_Line(x1,y2,x1,y1,color);

  return;
}

//Draw bitmap - ported from the ADAFruit GFX library

void ssd1306_DrawBitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, SSD1306_COLOR color)
{
    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t byte = 0;

    //if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
    //    return;
    //}

    for (uint8_t j = 0; j < h; j++, y++) {
        for (uint8_t i = 0; i < w; i++) {
            if (i & 7)
                byte <<= 1;
            else
                byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
            if (byte & 0x80)
                ssd1306_DrawPixel(x + i, y, color);
        }
    }
    return;
}

void ssd1306_SetContrast( uint8_t value ) {
    const uint8_t kSetContrastControlRegister = 0x81;
    ssd1306_WriteCommand(kSetContrastControlRegister);
    ssd1306_WriteCommand(value);
}

void ssd1306_SetDisplayOn( uint8_t on ) {
    uint8_t value;
		SSD1306.NeedInit = 0;
    if (on) {
				SSD1306.DisplayOn = 1;
        value = 0xAF;   // Display on
    } else {
        value = 0xAE;   // Display off
        SSD1306.DisplayOn = 0;
    }
		ssd1306_WriteCommand(value);
}

void ssd1306_SetDisplayPower( uint8_t on) {
    if (on) {
				HAL_GPIO_WritePin(VBUS_EN_GPIO_Port, VBUS_EN_Pin, GPIO_PIN_RESET);
				//HAL_GPIO_WritePin(VBUS_EN_GPIO_Port, VBUS_EN_Pin, GPIO_PIN_RESET);
				//HAL_Delay(500);
				ssd1306_Init();
    } else {
        SSD1306.DisplayOn = 0;
				SSD1306.Initialized = 0;
				SSD1306.NeedInit = 0;
				//HAL_GPIO_WritePin(VBUS_EN_GPIO_Port, VBUS_EN_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(VBUS_EN_GPIO_Port, VBUS_EN_Pin, GPIO_PIN_SET);
    }
}

uint8_t ssd1306_GetDisplayOn( void ) {
    return SSD1306.DisplayOn;
}

void ssd1306_SetNeedInitFlag( void ) {
		SSD1306.NeedInit = 1;
}

uint8_t ssd1306_GetNeedInitFlag( void ) {
    return SSD1306.NeedInit;
}
