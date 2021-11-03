#include "mtr_SPI.h"
#include "mtr_SPI_defines.h"
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>

void MTR_SPI_init(	MTR_SPI_Direction_t direction, 
					MTR_SPI_Mode_t mode,
					MTR_SPI_Shift_Order_t shiftOrder,
					MTR_SPI_Prescaler_t prescaler,
					bool enableInterrupt )
{
	uint8_t temporarySPCR = 0;
	uint8_t temporarySPSR = 0;
	
	//reset register SPI
	SPCR = 0;	//Register control SPI
	SPSR = 0;	//Register state SPI
	SPDR = 0;	//Register data SPI
	
	//-----direction-----
	switch( direction )
	{
		case( MTR_SPI_MASTER ):
		{
			PinMode( MTR_SPI_SCK,	Output );
			PinMode( MTR_SPI_MOSI,	Output );
			PinMode( MTR_SPI_SS,	Output );
			PinMode( MTR_SPI_MISO,	Input  );
			
			DigitalWrite( MTR_SPI_SS,	High );	
			DigitalWrite( MTR_SPI_SCK,	Low  );
			DigitalWrite( MTR_SPI_MOSI, Low  );
			
			temporarySPCR |= (1 << MSTR);
		}
		break;
		
		case( MTR_SPI_SLAVE ):
		{
			PinMode( MTR_SPI_MISO,	Output );
			PinMode( MTR_SPI_SCK,	Input  );
			PinMode( MTR_SPI_MOSI,	Input  );
			PinMode( MTR_SPI_SS,	Input  );
			
			DigitalWrite( MTR_SPI_MISO,	Low );
			
			temporarySPCR &= ~(1 << MSTR);	
		}
		break;
	}
	
	//-----shift order-----
	switch( shiftOrder )
	{
		case( MTR_SPI_FirstMSB ):
		{
			temporarySPCR &= ~(1 << DORD);
		}
		break;
			
		case( MTR_SPI_FirstLSB ):
		{
			temporarySPCR |=  (1 << DORD);
		}
		break;
	}
	
	//-----mode-----
	switch( mode )
	{
		case( MTR_SPI_Mode_0 ):
		{
			temporarySPCR &= ~(1 << CPHA);
			temporarySPCR &= ~(1 << CPOL);
		}
		break;
		
		case( MTR_SPI_Mode_1 ):
		{
			temporarySPCR |=  (1 << CPHA);
			temporarySPCR &= ~(1 << CPOL);
		}
		break;
		
		case( MTR_SPI_Mode_2 ):
		{
			temporarySPCR &= ~(1 << CPHA);
			temporarySPCR |=  (1 << CPOL);
		}
		break;
		
		case( MTR_SPI_Mode_3 ):
		{
			temporarySPCR |=  (1 << CPHA);
			temporarySPCR |=  (1 << CPOL);
		}
		break;
	}

	//-----prescaler-----
	switch( prescaler )
	{
		case( MTR_SPI_Prescaler_2 ):
		{
			temporarySPCR &= ~(1 << SPR0);
			temporarySPCR &= ~(1 << SPR1);
					
			temporarySPSR |=  (1 << SPI2X);
		}
		break;
		
		case( MTR_SPI_Prescaler_4 ):
		{
			temporarySPCR &= ~(1 << SPR0);
			temporarySPCR &= ~(1 << SPR1);
					
			temporarySPSR &= ~(1 << SPI2X);
		}
		break;
		
		case( MTR_SPI_Prescaler_8 ):
		{
			temporarySPCR |=  (1 << SPR0);
			temporarySPCR &= ~(1 << SPR1);
			
			temporarySPSR |=  (1 << SPI2X);
		}
		break;
		
		case( MTR_SPI_Prescaler_16 ):
		{
			temporarySPCR |=  (1 << SPR0);
			temporarySPCR &= ~(1 << SPR1);
				
			temporarySPSR &= ~(1 << SPI2X);
		}
		break;

		case( MTR_SPI_Prescaler_32 ):
		{
			temporarySPCR &= ~(1 << SPR0);
			temporarySPCR |=  (1 << SPR1);
			
			temporarySPSR |=  (1 << SPI2X);
		}
		break;

		case( MTR_SPI_Prescaler_64 ):
		{
			temporarySPCR &= ~(1 << SPR0);
			temporarySPCR |=  (1 << SPR1);
			
			temporarySPSR &= ~(1 << SPI2X);		
		}
		break;
		
		case( MTR_SPI_Prescaler_128 ):
		{
			temporarySPCR |=  (1 << SPR0);
			temporarySPCR |=  (1 << SPR1);
			
			temporarySPSR &= ~(1 << SPI2X);
		}
		break;
	}

	//Enable interrupt SPI
	if( enableInterrupt )
	{
		temporarySPCR |= (1 << SPIE);
	}
		
	//Enable SPI
	temporarySPCR |= (1 << SPE);
	
	SPCR |= temporarySPCR;
	SPSR |= temporarySPSR;
}

void MTR_SPI_MasterTransmitByte( uint8_t newData )
{
	SPDR = newData;
}

uint8_t MTR_SPI_SlaveReceive()
{
	return SPDR;
}

