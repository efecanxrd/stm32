#include <stdint.h>
#include <stdio.h>
#include "bit-fields.h"

#define RCC_AHB1ENR (*(volatile RCC_AHB1ENR_t*) 0x40023830)
#define GPIOD_MODER (*(volatile GPIOx_MODER_t*) 0x40020C00)
#define GPIOD_ODR (*(volatile GPIOx_ODR_t*) 0x40020C14)

int main(void)
{
	RCC_AHB1ENR.GPIODEN = 1;
	GPIOD_MODER.PIN_12 = 0b01;
	GPIOD_MODER.PIN_13 = 0b01;
	GPIOD_MODER.PIN_14 = 0b01;
	GPIOD_MODER.PIN_15 = 0b01;
	GPIOD_ODR.ODR12 = 1;
	for(uint32_t i=0; i<100000; i++);
	GPIOD_ODR.ODR12 = 0;
	for(uint32_t i=0; i<100000; i++);
	GPIOD_ODR.ODR12 = 1;
	GPIOD_ODR.ODR13 = 1;
	for(uint32_t i=0; i<2500000; i++);
	GPIOD_ODR.ODR14 = 1;
	GPIOD_ODR.ODR15 = 1;

	for(;;);
}
