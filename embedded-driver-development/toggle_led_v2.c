#include <stdio.h>
#include <stdint.h>

#include "stm32f407xx.h"
//Toggling the LED with open drain
//include an external PULL UP. Connect them in breadboard (5V and PD12) WITH 470Ohm resistor
void delay(uint32_t value) {
	for(uint32_t i = 0; i < value; i++);
}

int main(void) {
	GPIO_Handle_t GpioLed;
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&GpioLed);

	while(1) {
		GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
		delay(500000);
	}
	for(;;);
}
