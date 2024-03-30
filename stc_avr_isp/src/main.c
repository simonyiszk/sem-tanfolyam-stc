/**
 *          STC8G1K08 AVRISP
 *
 *            License: BSD
 *
 *    STK500.c Referenced ArduinoISP.ino
 *   Copyright (c) 2008-2011 Randall Bohn
 *
 *        Modified by: DT9025A
 *           Date: 2021/2/9
 * ---------------------------------------
 *    Modified once more by: weiszpal
 *          Date: 2024/03/24
 * ---------------------------------------
 *
 * System clock = 12MHz
 * Reference: ArduinoISP.ino (44-52)
 *
 * UART rate selectable via P55: HIGH - 115200, LOW - 19200bps
 * Reference: ATMEL AVRISP User Guide (138) (https://community.atmel.com/sites/default/files/project_files/ATAVRISP_User_Guide.pdf)
 *
 * ---------------------------------------
 * 
 * P10 - PROGMODE LED
 * P11 - ERROR LED
 * P12 - RST
 * P13 - MOSI
 * P14 - MISO
 * P15 - SCK
 * P55 - UART RATE SELECT
 * 
 */

#include "stk500.h"
#include "uart.h"

INTERRUPT(Uart1_ISR, EXTI_VectUART1)
{
    if (RI) { //Receive data
        RI = 0; //Clear interrupt flag
        if (UART_RECV_PTR == UART_RECV_BUFFER_SIZE) //The queue is full
            UART_RECV_PTR = 0; //Reset the ring queue
        URAT_RECV_BUF[UART_RECV_PTR++] = SBUF; //Put in data
    }
}

void main()
{
    //Configure pin mode
    PIN_MODE_CONFIG(P1, PIN_2 | PIN_3 | PIN_4 | PIN_5, PIN_MODE_STANDARD); // SPI
    PIN_MODE_CONFIG(P1, PIN_0 | PIN_1, PIN_MODE_PUSHPULL); // LEDs

    PIN_MODE_CONFIG(P5, PIN_5, PIN_MODE_HIRGRESIN);
    PIN_PULLUP_CONFIG(P5, PIN_5, PIN_PULLUP_ENABLE) // Baud selector JUMPER with pull-up

    Uart_Init(P55);

    EA = 1; //Enable global interrupt

    while (1) {
        P10 = ISP_PROGMODE;
        P11 = ((ISP_Error_Count>0) ? 1:0);
        if (Uart_Available()) {
            ISP_Process_Data(); //Process data
        }
    }
}