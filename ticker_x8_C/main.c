#include "inc/init.h"
#include "inc/task.h"
#include <avr/interrupt.h>
#include "config.h"
#include "SPI/mtr_SPI.h"
#include "class/ticker.h"
#include <util/delay.h>

extern ticker_t ticker;

int main(void)
{
	cli();

	initData();

	sei();
	
    while(1) 
    {
		if( ticker.allowedSend == false )
		{
			//cyclicShift(10);
			_delay_ms(1000);
			startSend();
		}
		
		
    }
}

