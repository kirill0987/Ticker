#include "../inc/init.h"
#include "../inc/task.h"
#include "../config.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>

#include "../class/ticker.h"
#include "../SPI/mtr_SPI.h"


const char* m = "Старт";

void initData()
{
	createTickerMatrix(60,8);
	addString(m);
	//createTickerLine();
	
	/*
	for( uint32_t i = 0; 
		 i < AMOUNT_LED;
		 i++ )
	{			
		if(i < 120)
		{
			addColorLedHexRGB(i, 0xFFFFFF);
			continue;
		}
		
		if(i < 240)
		{
			addColorLedHexRGB(i, 0x00FF00);
			continue;
		}
		
		addColorLedHexRGB(i, 0xFF0000);
		
	}
	*/
}