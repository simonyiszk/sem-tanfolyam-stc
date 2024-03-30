#ifndef __SPI_H__
#define __SPI_H__

#include <fw_reg_stc8g.h>

#define SPI_CLK_DIV 3 //SPI clock divided by 32
#define SPI_WAIT_BUSY while (!(SPSTAT & 0x80)) //Wait for transmission to complete

void SPI_Init(); //Initialize SPI
unsigned char SPI_SendByte(unsigned char dat); //SPI sends bytes

#endif