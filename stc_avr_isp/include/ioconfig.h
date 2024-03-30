/************************
* STC8 series IO configuration
 * @Author  DT9025A
 * @Date    21/1/15
 * @License Apache License 2.0
 *
* This document is written based on STC8H documentation and can be adapted according to the target platform.
 ***********************/
#ifndef __STC8_IO_CONFIG_H__
#define __STC8_IO_CONFIG_H__

#include <fw_reg_stc8g.h>

#ifdef __STC8H_H_
#warning The default header file has not been changed, and the target platform is STC8H
#endif

//IO definition, add it when using it
#define PIN_0 0x01
#define PIN_1 0x02
#define PIN_2 0x04
#define PIN_3 0x08
#define PIN_4 0x10
#define PIN_5 0x20
#define PIN_6 0x40
#define PIN_7 0x80
#define PIN_ALL 0xFF

//Mode definition
#define PIN_MODE_STANDARD 0, 0
#define PIN_MODE_PUSHPULL 1, 0
#define PIN_MODE_HIRGRESIN 0, 1
#define PIN_MODE_OPENDRAIN 1, 1

//PULLUP status
#define PIN_PULLUP_ENABLE 1
#define PIN_PULLUP_DISABLE 0

//NCS status
#define PIN_NCS_ENABLE 0
#define PIN_NCS_DISABLE 1

//SR status
#define PIN_SWITCHRATE_HIGH 0
#define PIN_SWITCHRATE_LOW 1

//DR status
#define PIN_DRAINRATE_HIGH 0
#define PIN_DRAINRATE_LOW 1

//IE status
#define PIN_DIGITALSIGINPUTE_DISABLE 0
#define PIN_DIGITALSIGINPUTE_ENABLE 1

//IO mode
#define PIN_MODE_CONFIG(port, pin, mode) _PINMODE_CONFIG(port, pin, mode)
#define _PINMODE_CONFIG(port, pin, stat0, stat1)      \
    stat0 ? (port##M0 |= (pin)) : (port##M0 &= (~(pin))); \
    stat1 ? (port##M1 |= (pin)) : (port##M1 &= (~(pin)));

//Pull-up resistor setting
#define PIN_PULLUP_CONFIG(port, pin, stat) \
    (P_SW2 |= 0x80); \
    stat ? (port##PU |= (pin)) : (port##PU &= (~(pin))); \
    (P_SW2 &= ~0x80);

//Schmitt trigger setting, you need to set the highest position of P_SW2 to 1 before setting
#define PIN_NCS_CONFIG(port, pin, stat) \
    stat ? (port##NCS |= pin) : (port##NCS &= (~pin));

//Port conversion rate setting, you need to set the highest position of P_SW2 to 1 before setting
#define PIN_SWITCHRATE_CONFIG(port, pin, stat) \
    stat ? (port##SR |= pin) : (port##SR &= (~pin));

//Port drive current setting, you need to set the highest position of P_SW2 to 1 before setting
#define PIN_DRAINRATE_CONFIG(port, pin, stat) \
    stat ? (port##DR |= pin) : (port##DR &= (~pin));

//Digital signal enable setting, you need to set the highest position of P_SW2 to 1 before setting
#define PIN_DIGITALSIGINPUTENABLE_CONFIG(port, pin, stat) \
    stat ? (port##IE |= pin) : (port##IE &= (~pin));

#endif
