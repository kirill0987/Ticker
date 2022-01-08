#include "ticker.h"
#include "../SPI/mtr_SPI.h"
#include "../SPI/mtr_SPI_defines.h"
#include "../font/Font5x7.h"
#include <time.h>

const uint8_t stateBitSPI[4] = 
	{	0xFC,	//For MTR_SPI_Prescaler_2 - 1111 1100 | 1 (252|0xFC)
		0xC0,	//For MTR_SPI_Prescaler_2 - 1100 0000 | 0 (192|0xC0)
		0x0E,	//For MTR_SPI_Prescaler_4 - 0000 1110 | 1
		0x08};	//For MTR_SPI_Prescaler_4 - 0000 1000 | 0

ticker_t ticker;

void createTickerLine()
{
	ticker.amountLed = AMOUNT_LED;
	
	//Auto random
	srand( time( 0 ) );
	
	MTR_SPI_init(	MTR_SPI_MASTER,
					MTR_SPI_Mode_1,
					MTR_SPI_FirstMSB,
					MTR_SPI_Prescaler_2,
					true);
	
	ticker.allowedSend = false;
	
	for( uint32_t i = 0; 
		 i < ticker.amountLed; 
		 i++ )
	{
		ticker.arrayLed[i].red = 0;
		ticker.arrayLed[i].green = 0;
		ticker.arrayLed[i].blue = 0;
	}
	
	ticker.typeTicker = TickerLine;
	ticker.widthMatrix = 0;
	ticker.heightMatrix = 0;
	ticker.text = 0;
	ticker.sizeText = 0;
}

void addString(const char* newString)
{
	ticker.text = newString;

	uint32_t sizeText = 0;

	do
	{
		if (sizeText < 10)
		{
			addChar(*(newString), sizeText);
		}
		else
		{
			//Дальше не поместиться
		}

		sizeText++;

	} while ( *(++newString) );

	ticker.sizeText = sizeText;
}

void addChar(unsigned char newChar, uint16_t positionOffset)
{
	uint16_t numberCharInArray = ( (uint16_t)newChar) * 5;
	
	for (int j = 0; j < 7; j++)
	{
		for (int i = 0; i < 6; i++)
		{
			uint16_t counterLed = 0;
			
			if (j % 2 == 0)
			{
				counterLed = ((j * 60)) + i + (positionOffset * 6);
			}
			else
			{
				counterLed = ( ((j + 1 ) * 60) - 1 ) - i - (positionOffset * 6);
			}

			if( i == 5 )
			{
				addColorLedHexRGB(counterLed, 0x0);
			}
			else
			{
				uint8_t font = pgm_read_byte(&Font5x7[numberCharInArray + i]);

				if ( getBit(font, j) )
				{
					addColorLedHexRGB(counterLed, 0xFFFFFF);
				}
				else
				{
					addColorLedHexRGB(counterLed, 0x0);
				}
			}
		}
	}
	
}

void createTickerMatrix(uint32_t width,
						uint32_t height)
{
	ticker.amountLed = AMOUNT_LED;
	
	//Auto random
	srand( time( 0 ) );
	
	MTR_SPI_init(	MTR_SPI_MASTER,
					MTR_SPI_Mode_1,
					MTR_SPI_FirstMSB,
					MTR_SPI_Prescaler_2,
					true);
	
	ticker.allowedSend = false;
	
	for( uint32_t i = 0;
	i < ticker.amountLed;
	i++ )
	{
		ticker.arrayLed[i].red = 0;
		ticker.arrayLed[i].green = 0;
		ticker.arrayLed[i].blue = 0;
	}
	
	ticker.typeTicker = TickerMatrix;
	ticker.widthMatrix = width;
	ticker.heightMatrix = height;
	ticker.text = 0;
	ticker.sizeText = 0;
}

void addColorLedHexRGB(uint32_t elem, uint32_t hexRGB)
{
	ticker.arrayLed[elem].red = 
		(hexRGB >> 16) & 0xFF;
		
	ticker.arrayLed[elem].green = 
		(hexRGB >> 8) & 0xFF;
		
	ticker.arrayLed[elem].blue = 
		(hexRGB >> 0) & 0xFF;
}

void cyclicShift(long int shift)
{
	if(shift == 0)
	{
		return;
	}
	
	do{
		if( shift > 0 ) //left
		{
			ColorLed_t lastElement =
			ticker.arrayLed[(ticker.amountLed - 1)];
		
			for (	uint32_t i = ( ticker.amountLed - 1 );
					i >= 1;
					i--)
			{
				ticker.arrayLed[i] =
				ticker.arrayLed[i-1];
			}
		
			ticker.arrayLed[0] = lastElement;
			
			shift--;
		}
		else			//right
		{
			ColorLed_t firstElement =
			ticker.arrayLed[0];
		
			for (	uint32_t i = 0;
					i < ( ticker.amountLed - 1 );
					i++ )
			{
				ticker.arrayLed[i] =
				ticker.arrayLed[i+1];
			}
		
			ticker.arrayLed[(ticker.amountLed - 1)] =
			firstElement;
			
			shift++;
		}
	}while(shift != 0);
}

void startSend()
{
	if( ticker.allowedSend == false )
	{
		ticker.allowedSend = true;
		MTR_SPI_MasterTransmitByte( 0x00 );
	}
}

ISR( SPI_STC_vect )
{
	static uint32_t counterBit = 0;
	static uint32_t counterColor = 0;
	static uint32_t counterLed = 0;
	static uint8_t sendByte = 0;
	
	if(	ticker.allowedSend )
	{
		DigitalWrite(MTR_SPI_SS, Low);

		switch(counterColor)
		{
			case(0):
			{
				if( getBit( ticker.arrayLed[counterLed].green, counterBit ) )
				{
					sendByte = stateBitSPI[0];
				}
				else
				{
					sendByte = stateBitSPI[1];
				}
			}
			break;
	
			case(1):
			{
				if( getBit( ticker.arrayLed[counterLed].red, counterBit ) )
				{
					sendByte = stateBitSPI[0];
				}
				else
				{
					sendByte = stateBitSPI[1];
				}
			}
			break;
	
			case(2):
			{
				if( getBit( ticker.arrayLed[counterLed].blue, counterBit ) )
				{
					sendByte = stateBitSPI[0];
				}
				else
				{
					sendByte = stateBitSPI[1];
				}
			}
			break;
		}
		
		if(counterBit <= 6)
		{
			counterBit++;
		}
		else
		{
			counterBit = 0;
			counterColor++;
		}
		
		if(counterColor > 2)
		{
			counterColor = 0;
			
			if( counterLed < (AMOUNT_LED - 1) )
			{
				counterLed++;
			}
			else
			{
				counterLed = 0;
				ticker.allowedSend = false;
				DigitalWrite(MTR_SPI_SS, High);
			}
		}

		MTR_SPI_MasterTransmitByte( sendByte );
	}
}