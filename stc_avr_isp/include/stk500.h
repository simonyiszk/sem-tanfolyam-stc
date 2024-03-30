#ifndef __STK500_H__
#define __STK500_H__

#include "command.h"
#include "delay.h"
#include "spi.h"
#include "uart.h"

#define Hardware_Version 1 //Hardware version
#define Software_Major_Version 1 //Major software version
#define Software_Minor_Version 20 //Software minor version

#define EECHUNK (32) //EEPROM block size
#define GET_WORD_DATA(addr) (*addr * 256 + *(addr + 1)) //Synthesize one word (16 bits) of data from the two bytes of the current address, with the high byte first

SBIT(MOSI, _P1, 3);
SBIT(RST, _P1, 2);
SBIT(MISO, _P1, 4);
SBIT(SCLK, _P1, 5);

extern unsigned char ISP_PROGMODE;
extern unsigned char ISP_Error_Count;

//Device settings, refer to page 5 of <AVR061: STK500 Communication Protocol>
typedef struct {
    unsigned char devicecode;
    unsigned char revision;
    unsigned char progtype;
    unsigned char parmode;
    unsigned char polling;
    unsigned char selftimed;
    unsigned char lockbytes;
    unsigned char fusebytes;
    unsigned char flashpoll;
    unsigned int eeprompoll;
    unsigned int pagesize;
    unsigned int eepromsize;
    unsigned long flashsize;
} ISP_Parameter;

void ISP_Process_Data(); //Process data

#endif