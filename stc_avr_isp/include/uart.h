#ifndef __UART_H__
#define __UART_H__

#include <fw_reg_stc8g.h>
#include "ioconfig.h"

//UART receive buffer size
#define UART_RECV_BUFFER_SIZE 128
#define EXTI_VectUART1 4

extern unsigned char __XDATA URAT_RECV_BUF[UART_RECV_BUFFER_SIZE]; //Ring buffer queue
extern unsigned char UART_RECV_PTR; //buffer queue receive pointer
extern unsigned char UART_RECV_READ_PTR; //The buffer queue has read pointer

void Uart_Init(unsigned char highspeed); //Initialize UART
void Uart_Send(unsigned char dat); //Send one byte
void Uart_SendString(char *dat); //Send string
unsigned char Uart_Getch(); //Read one byte from the buffer, blocking if there is no content in the buffer
__BIT Uart_Available(); //Whether the buffer has content to be read

#endif
