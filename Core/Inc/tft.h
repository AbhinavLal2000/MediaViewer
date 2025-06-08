/*
 * tft.h
 *
 *  Created on: May 4, 2025
 *      Author: user
 */

#ifndef INC_TFT_H_
#define INC_TFT_H_

#include <stdio.h>
#include <string.h>
#include "stm32f1xx_hal.h"

#define ST_WIDTH 128
#define ST_HEIGHT 160
#define ST_MADCTL_MY 0x80
#define ST_MADCTL_MX 0x40
#define ST_MADCTL_MV 0x20
#define ST_MADCTL_RGB 0x00
#define ST_MADCTL_BGR 0x08
#define ST_NOP 0x00
#define ST_SWRESET 0x01
#define ST_RDDID 0x04
#define ST_RDDST 0x09
#define ST_RDDPM 0x0A
#define ST_RDD_MADCTL 0x0B
#define ST_RDD_COLMOD 0x0C
#define ST_RDDIM 0x0D
#define ST_RDDSM 0x0E
#define ST_SLPIN 0x10
#define ST_SLPOUT 0x11
#define ST_PLTON 0x12
#define ST_NORON 0x13
#define ST_INVOFF 0x20
#define ST_INVON 0x21
#define ST_GAMSET 0x26
#define ST_DISPOFF 0x28
#define ST_DISPON 0x29
#define ST_CASET 0x2A
#define ST_RASET 0x2B
#define ST_RAMWR 0x2C
#define ST_RGBSET 0x2D
#define ST_RAMRD 0x2E
#define ST_PTLAR 0x30
#define ST_TEOFF 0x34
#define ST_TEON 0x35
#define ST_MADCTL 0x36
#define ST_IDMOFF 0x38
#define ST_IDMON 0x39
#define ST_COLMOD 0x3A
#define ST_RDID1 0xDA
#define ST_RDID2 0xDB
#define ST_RDID3 0xDC
#define ST_DUMMY 0x80

/* PANEL FUNCTION COMMAND LIST */

#define ST_FRMCTR1 0xB1
#define ST_FRMCTR2 0xB2
#define ST_FRMCTR3 0xB3
#define ST_INVCTR 0xB4
#define ST_PWCTR1 0xC0
#define ST_PWCTR2 0xC1
#define ST_PWCTR3 0xC2
#define ST_PWCTR4 0xC3
#define ST_PWCTR5 0xC4
#define ST_VMCTR1 0xC5
#define ST_VMOCFCTR 0xC7
#define ST_WRID2 0xD1
#define ST_WRID3 0xD2
#define ST_NVCTR1 0xD9
#define ST_NVCTR2 0xDE
#define ST_NVCTR3 0xDF
#define ST_GAMCTRP1 0xE0
#define ST_GAMCTRN1 0xE1

#define DEFINED_COLORS 10

#define BLACK   0x0000
#define BLUE    0xF800
#define RED     0x001F
#define GREEN   0x07E0
#define CYAN    0xFFE0
#define MAGENTA 0xF81F
#define YELLOW  0x07FF
#define WHITE   0xFFFF
#define ORANGE  0x0BFE
#define LIGHTGREEN 0x07EF

typedef struct tft
{
	SPI_HandleTypeDef *hspi;
	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;
	GPIO_TypeDef *reset_port;
	uint16_t reset_pin;
	GPIO_TypeDef *dc_port;
	uint16_t dc_pin;

} tft_t;

void tft_send_data(tft_t *tft, uint8_t data);
void tft_send_cmd(tft_t *tft, uint8_t cmd);
void tft_cs_high(tft_t *tft);
void tft_cs_low(tft_t *tft);
void tft_init(tft_t *tft, SPI_HandleTypeDef *hspi, GPIO_TypeDef *reset_port, uint16_t reset_pin, GPIO_TypeDef *dc_port, uint16_t dc_pin, GPIO_TypeDef *cs_port, uint16_t cs_pin);
void tft_set_addr_window(tft_t *tft, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void tft_fill_rect(tft_t *tft, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void tft_write_char(tft_t* tft, char c, uint16_t tcolor, uint16_t bcolor);
void tft_write_string(tft_t* tft, uint8_t x, uint8_t y, char* str, uint16_t tcolor, uint16_t bcolor);
void tft_write_num(tft_t* tft, uint8_t x, uint8_t y, int num, uint16_t tcolor, uint16_t bcolor);
void tft_new_bar(tft_t *tft, uint16_t barcolor, uint8_t ypos);

#endif /* INC_TFT_H_ */
