#ifndef __TICKER_H__
#define __TICKER_H__

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include "../config.h"

typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}ColorLed_t;

typedef enum
{
	TickerLine,
	TickerMatrix
}typeTicker_t;

typedef enum
{
	Green,
	Red,
	Blue
}ColorTickerSPI_t;

typedef struct
{
	uint32_t counterBit;
	uint32_t counterColor;
	uint32_t counterLed;
	uint8_t sendByte;
	
	//For matrix
	uint32_t counterHeight;
}InterruptSPI_ticker_t;

typedef struct
{
	uint32_t amountLed;
	ColorLed_t arrayLed[AMOUNT_LED];
	bool allowedSend;
	typeTicker_t typeTicker;
	uint32_t widthMatrix;	//Ширина
	uint32_t heightMatrix;	//Высота
	const char* text;
	uint32_t sizeText;
}ticker_t;

void createTickerLine();

void createTickerMatrix(uint32_t width,
						uint32_t height);
						
void addColorLedHexRGB(	uint32_t elem, 
						uint32_t hexRGB);
						
void addString(const char* newString, uint32_t hexRGB);
void addChar(unsigned char newChar, uint16_t positionOffset, uint32_t hexRGB);
						
void cyclicShiftLine(long int shift);
void cyclicShiftMatrix(long int shift);
void startSend();
void show();

#define getBit(x, y) ( (x >> y) & 1 )

#endif //__TICKER_H__
