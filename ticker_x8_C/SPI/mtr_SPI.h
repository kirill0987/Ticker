#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdbool.h>

typedef enum
{
	MTR_SPI_MASTER,
	MTR_SPI_SLAVE
}MTR_SPI_Direction_t;

typedef enum
{
	MTR_SPI_Mode_0,
	MTR_SPI_Mode_1,
	MTR_SPI_Mode_2,
	MTR_SPI_Mode_3
}MTR_SPI_Mode_t;

typedef enum
{
	MTR_SPI_Prescaler_2,
	MTR_SPI_Prescaler_4,
	MTR_SPI_Prescaler_8,
	MTR_SPI_Prescaler_16,
	MTR_SPI_Prescaler_32,
	MTR_SPI_Prescaler_64,
	MTR_SPI_Prescaler_128,
	
}MTR_SPI_Prescaler_t;

typedef enum
{
	MTR_SPI_BusFree,		//Шина свободна
	MTR_SPI_TransferData,	//Передача данный
	MTR_SPI_ReceiveData		//Прием данных
}MTR_SPI_State_t;

typedef enum
{
	MTR_SPI_FirstMSB,	//первым передается старший значащий разряд MSB	
	MTR_SPI_FirstLSB	//первым передается младший значащий разряд LSB
}MTR_SPI_Shift_Order_t;

void MTR_SPI_init(	MTR_SPI_Direction_t direction, 
					MTR_SPI_Mode_t mode,
					MTR_SPI_Shift_Order_t shiftOrder, 
					MTR_SPI_Prescaler_t prescaler,
					bool enableInterrupt);

void MTR_SPI_MasterTransmitByte( uint8_t newData );			
uint8_t MTR_SPI_SlaveReceive();

#endif /* SPI_H_ */