#ifndef MTR_SPI_DEFINES_H_
#define MTR_SPI_DEFINES_H_

#define MTR_SPI_SS		B, 0
#define MTR_SPI_SCK		B, 1
#define MTR_SPI_MOSI	B, 2
#define MTR_SPI_MISO	B, 3

//Macros to edit PORT, DDR and PIN
#define PinMode(			x, y)	( 	y 	?	_SET(DDR, x)	:	_CLEAR(DDR, x)		)
#define DigitalWrite(		x, y)	( 	y 	?	_SET(PORT, x)	:	_CLEAR(PORT, x)		)
#define DigitalRead(		x)		(			_GET(PIN, x)							)
#define PinModeToggle(		x)		(			_TOGGLE(DDR, x)							)
#define DigitalLevelToggle(	x)		(			_TOGGLE(PORT, x)						)

//General use bit manipulating commands
#define BitSet(		x, y)			(	x |=	 (1UL<<y)			)
#define BitClear(	x, y)			(	x &=	(~(1UL<<y))			)
#define BitToggle(	x, y)			(	x ^=	 (1UL<<y)			)
#define BitCheck(	x, y)			(	x &		 (1UL<<y)	? 1 : 0	)

//Access PORT, DDR and PIN
#define PORT(	port)				(_PORT(	port))
#define DDR(	port)				(_DDR(	port))
#define PIN(	port)				(_PIN(	port))

#define _PORT(	port)				(PORT##	port)
#define _DDR(	port)				(DDR##	port)
#define _PIN(	port)				(PIN##	port)

#define _SET(	type, port, bit)	(	BitSet(		(type##port),	bit)	)
#define _CLEAR(	type, port, bit)	(	BitClear(	(type##port),	bit)	)
#define _TOGGLE(type, port, bit)	(	BitToggle(	(type##port),	bit)	)
#define _GET(	type, port, bit)	(	BitCheck(	(type##port),	bit)	)

//Definitions
#define Input		0
#define Output		!Input
#define Low			0
#define High		!Low
#define False		0
#define True		!False

/* -----Example-----
 * PinMode(LCD_CS1, Output);
 * DigitalWrite(LCD_DI, Low)
 * DigitalRead(LCD_D7)
*/

#endif /* MTR_SPI_DEFINES_H_ */