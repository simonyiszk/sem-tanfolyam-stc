#include "uart.h"

unsigned char __XDATA URAT_RECV_BUF[UART_RECV_BUFFER_SIZE]; //Ring buffer queue
unsigned char UART_RECV_PTR; //buffer queue receive pointer
unsigned char UART_RECV_READ_PTR; //The buffer queue has read pointer

void Uart_Init(unsigned char highspeed)
{
    SCON = 0x50; //8-bit data, variable baud rate
    AUXR |= 0x40; //Timer 1 clock is Fosc, which is 1T
    AUXR &= 0xFE; //Serial port 1 selects timer 1 as the baud rate generator
    TMOD &= 0x0F; //Set timer 1 to 16-bit automatic reload mode

    if(highspeed != 0){
        TL1 = 0xE6; //115200bps @12MHz
    }else{
        TL1 = 0x64; //19200bps @12MHz
    }

    TH1 = 0xFF; //Set the initial value of the timing
    ET1 = 0; //Disable timer 1 interrupt
    TR1 = 1; //Start timer 1
    ES = 1; //Allow serial port interrupt

    UART_RECV_PTR = 0;
    UART_RECV_READ_PTR = 0;
}

void Uart_Send(unsigned char dat)
{
    SBUF = dat;
    while (TI == 0)
        ;
    TI = 0;
}

void Uart_SendString(char *s)
{
    while (*s) {
        Uart_Send(*s++);
    }
}

__BIT Uart_Available()
{
//If the receiving pointer is greater than the reading pointer, it indicates that there must be unread data
    if (UART_RECV_PTR > UART_RECV_READ_PTR)
        return 1;
//If the receiving pointer is equal to the reading pointer, it indicates that the data reading is completed
    else if (UART_RECV_PTR == UART_RECV_READ_PTR)
        return 0;
//If the receiving pointer is smaller than the reading pointer, it indicates that the ring queue is reset and there is unread data at the head of the queue.
    else
        return 1;
}

unsigned char Uart_Getch()
{
    while (!Uart_Available()) //Wait for valid data
        ;
    if (UART_RECV_READ_PTR == UART_RECV_BUFFER_SIZE) //Read to the end of the queue
        UART_RECV_READ_PTR = 0; //Reset the read pointer
    return URAT_RECV_BUF[UART_RECV_READ_PTR++]; //Read data
}
