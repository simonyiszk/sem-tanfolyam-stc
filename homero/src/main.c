#include "fw_hal.h"
#include <math.h>

#define CLK    P15
#define DATA   P13
#define LATCH  P12

const uint8_t cau8SevenSegmentTableLeft[10] = 
{
    0x81,  // 0
    0xF9,  // 1
    0x43,  // 2
    0x61,  // 3
    0x39,  // 4
    0x25,  // 5
    0x05,  // 6
    0xF1,  // 7
    0x01,  // 8
    0x21   // 9
};

const uint8_t cau8SevenSegmentTableRight[10] = 
{
    0x18,  // 0
    0xF9,  // 1
    0x2C,  // 2
    0x68,  // 3
    0xC9,  // 4
    0x4A,  // 5
    0x0A,  // 6
    0xF8,  // 7
    0x08,  // 8
    0x48   // 9
};

void ShiftOut7Segment( uint16_t u16CodeWord )
{
    uint8_t u8Index;

    for( u8Index = 0; u8Index < 16; u8Index++ )
    {
        if( 0 != (u16CodeWord & ((uint16_t)1<<u8Index) ) )
        {
            DATA = 1;
        }
        else
        {
            DATA = 0;
        }
        SYS_DelayUs( 1 );
        CLK = 1;
        SYS_DelayUs( 1 );
        CLK = 0;
        SYS_DelayUs( 1 );
    }
    LATCH = 1;
    SYS_DelayUs( 1 );
    LATCH = 0;
}

int main( void )
{
    uint8_t __xdata u8Counter = 0;
    uint16_t u16Result;
    float    fTemp;

    // Init shift register pins
    GPIO_P1_SetMode( GPIO_Pin_2, GPIO_Mode_Output_PP );
    GPIO_P1_SetMode( GPIO_Pin_3, GPIO_Mode_Output_PP );
    GPIO_P1_SetMode( GPIO_Pin_5, GPIO_Mode_Output_PP );
    CLK = 0;
    LATCH = 0;

    // Init LED pins
    GPIO_P1_SetMode( GPIO_Pin_1, GPIO_Mode_Output_PP );  // right LED
    P11 = RESET;

    // Init ADC
    GPIO_P1_SetMode( GPIO_Pin_4, GPIO_Mode_Input_HIP );
    ADC_SetChannel( 4 );  // P1.4 is ADC channel 4
    // ADC Clock = SYSCLK / 2 / (1+prescaler)
    ADC_SetClockPrescaler( 15 );
    // Right alignment, high 2-bit in ADC_RES, low 8-bit in ADC_RESL
    ADC_SetResultAlignmentRight();
    // Turn on ADC power
    ADC_SetPowerState( HAL_State_ON );

    /*
    // Enable ADC interrupt
    EXTI_ADC_SetIntState(HAL_State_ON);
    // Globally enable interrupts
    EXTI_Global_SetIntState(HAL_State_ON);
    // Start conversion
    ADC_Start();
    */

    // Main loop
    while( 1 )
    {
        ADC_Start();  // start conversion
        NOP();
        NOP();
        while( !ADC_SamplingFinished() );
        ADC_ClearInterrupt();
        u16Result = ((uint16_t)ADC_RES<<8) | (uint16_t)ADC_RESL;

        fTemp = logf( ( 1024.0 / (float)u16Result ) - 1.0 ) / 3950.0 + 1.0/298.15;
        fTemp = 1.0/fTemp - 273.15;

        ShiftOut7Segment( (uint16_t)cau8SevenSegmentTableRight[ (uint8_t)floorf(fTemp+0.5)%10 ] | (uint16_t)cau8SevenSegmentTableLeft[ (uint8_t)floorf(fTemp+0.5)/10 ]<<8 );

        SYS_Delay(500);  // 100 ms delay
        P11 = !P11;
        u8Counter++;
        if( u8Counter >= 10 )
        {
            u8Counter = 0;
        }
    }
}

/*
INTERRUPT(ADC_Routine, EXTI_VectADC)
{
    uint16_t u16Result;
    float    fTemp;

    ADC_ClearInterrupt();

    u16Result = ((uint16_t)ADC_RES<<8) | (uint16_t)ADC_RESL;

    fTemp = logf( ( 1024.0 / (float)u16Result ) - 1.0 ) / 3950.0 + 1.0/298.15;
    fTemp = 1.0/fTemp - 273.15;

    ShiftOut7Segment( (uint16_t)cau8SevenSegmentTableRight[ (uint8_t)fTemp%10 ] | (uint16_t)cau8SevenSegmentTableLeft[ (uint8_t)fTemp/10 ]<<8 );

    // Restart ADC for continuous sampling
    ADC_Start();
}
*/