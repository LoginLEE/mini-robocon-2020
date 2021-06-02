#ifndef __LCD_MAIN_H
#define __LCD_MAIN_H

// 3.5inch
// #define MODEL_ILI9486
// 2.8/3.2inch
//#define MODEL_ILI9341
// 1.8inch
#define MODEL_ST7735

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_rcc.h"
#include "ticks.h"
#include "gpio.h"
#include "spi_protocol.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

typedef enum {
	PIN_ON_TOP,
	PIN_ON_LEFT,
	PIN_ON_BOTTOM,
	PIN_ON_RIGHT
}TFT_ORIENTATION;

/*
typedef enum {
	DISPLAY_ON_BOARD,
	DISPLAY_REMOTE
}DISPLAY_MODE;
*/

/* GPIOPin settings */

#define tft_rst &PA8
#define tft_dc  &PA6
#define tft_cs  &PA4

#define TFT_DC_PORT GPIOA
#define TFT_DC_PIN  ((u16)GPIO_Pin_6)

#define tft_spi_port SPI1

/* Colors */
#include "colors.h"

/* Pixels */
#define CHAR_WIDTH              8
#define CHAR_HEIGHT             16

#ifdef MODEL_ILI9486
	#define SCREEN_CHOSEN
	#define MAX_WIDTH               320
	#define MAX_HEIGHT              480

	// #define CHAR_BUF_SIZE           600

	#define CHAR_MAX_X_VERTICAL     40
	#define CHAR_MAX_Y_VERTICAL     30

	#define CHAR_MAX_X_HORIZONTAL   60
	#define CHAR_MAX_Y_HORIZONTAL   20

	#define CHAR_MAX_X              60      // max between CHAR_MAX_X_VERTICAL and CHAR_MAX_X_HORIZONTAL
	#define CHAR_MAX_Y              30      // max between CHAR_MAX_Y_VERTICAL and CHAR_MAX_Y_HORIZONTAL

#endif

#ifdef MODEL_ILI9341
	#define SCREEN_CHOSEN
	#define MAX_WIDTH               240
	#define MAX_HEIGHT              320

	// #define CHAR_BUF_SIZE           600

	#define CHAR_MAX_X_VERTICAL     30
	#define CHAR_MAX_Y_VERTICAL     20

	#define CHAR_MAX_X_HORIZONTAL   40
	#define CHAR_MAX_Y_HORIZONTAL   15

	#define CHAR_MAX_X              40      // max between CHAR_MAX_X_VERTICAL and CHAR_MAX_X_HORIZONTAL
	#define CHAR_MAX_Y              20      // max between CHAR_MAX_Y_VERTICAL and CHAR_MAX_Y_HORIZONTAL

#endif

#ifdef MODEL_ST7735
	#define SCREEN_CHOSEN
	#define MAX_WIDTH               128
	#define MAX_HEIGHT              160

	// #define CHAR_BUF_SIZE           600

	#define CHAR_MAX_X_VERTICAL     16
	#define CHAR_MAX_Y_VERTICAL     10

	#define CHAR_MAX_X_HORIZONTAL   20
	#define CHAR_MAX_Y_HORIZONTAL   8

	#define CHAR_MAX_X              20      // max between CHAR_MAX_X_VERTICAL and CHAR_MAX_X_HORIZONTAL
	#define CHAR_MAX_Y              10      // max between CHAR_MAX_Y_VERTICAL and CHAR_MAX_Y_HORIZONTAL

#endif

#ifndef SCREEN_CHOSEN
#error "You must choose which screen you are using at the top"
#endif

extern u8 tft_orientation;
extern u16 curr_bg_color;
extern u16 curr_text_color;
extern u16 curr_text_color_sp;
extern u16 curr_highlight_color;

extern char text_buf[2][CHAR_MAX_X][CHAR_MAX_Y];
extern u16 text_color_buf[2][CHAR_MAX_X][CHAR_MAX_Y];
extern u16 bg_color_buf[2][CHAR_MAX_X][CHAR_MAX_Y];

// garuda
extern u8 char_max_x, char_max_y;

/* TFT communication control */
void tft_spi_init(void);
void tft_init(TFT_ORIENTATION orientation, u16 in_bg_color, u16 in_text_color, u16 in_text_color_sp, u16 in_highlight_color); //, DISPLAY_MODE mode);
//void tft_deinit(void);
//void tft_toggle_display(void);
void tft_enable(void);
void tft_disable(void);

/* Display properties configuration */
void tft_set_bg_color(u16 in_bg_color);
void tft_set_text_color(u16 in_text_color);
void tft_set_highlight_color(u16 sp_bg_color);
void tft_set_special_color(u16 text_color_sp);
u8 tft_get_orientation(void);

/* Pixel printing */
void tft_put_pixel(u16 x, u16 y, u16 color);
void tft_fill_color(u16 color);
void tft_force_clear(void);

/* Text buffer printing */
void tft_clear_line(u8 line);
void tft_clear(void);
char tft_get_buffer_char(u8 x, u8 y);
u16 tft_get_buffer_color(u8 x, u8 y);
void tft_prints(u8 x, u8 y, const char * pstr, ...);
void tft_println(const char * pstr, ...);
void tft_update(void);
void tft_update_dma(void);

//If you have space for it you can buffer all pixels in the entire display and push it using DMA
void tft_pxbuf_update(void); //Update pixel buffer
void tft_push_pxbuf(void* buf, u32 x, u32 y, u32 w, u32 h); //Push the pixel buffer to the display with DMA
void tft_circ_push_pxbuf(void* buf, u32 x, u32 y, u32 w, u32 h); //Set DMA in circular mode, call once

void tft_put_logo(u16 x, u16 y);

#endif		/* __LCD_MAIN_H */
