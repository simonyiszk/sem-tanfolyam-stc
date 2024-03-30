#include "spi.h"

void SPI_Init()
{
    SPCTL = 0xD0 | SPI_CLK_DIV; //Enable SPI master mode SSIG+SPEN+MSTR
    SPSTAT = 0xc0; //Clear the interrupt flag
}

unsigned char SPI_SendByte(unsigned char dat)
{
    SPDAT = dat; //Send data
    SPI_WAIT_BUSY; //Wait for data transfer to complete
    SPSTAT = 0xc0; //Clear flag bit
    return SPDAT; //Return the slave transmission data
}
