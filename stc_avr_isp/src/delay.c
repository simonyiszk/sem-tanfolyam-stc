#include "delay.h"

void delay_(void)	//@12.000MHz
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}

void delay(unsigned int ms)
{
	while (ms--) {
		delay_();
	}
}
