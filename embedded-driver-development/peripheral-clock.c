#include <stdio.h>
#include <stdint.h>

#define ADC_Control_Register ((0x40012000) + (0x04))
#define APB2_Peripheral_Enable_Register ((0x40023800) + (0x44))

int main(void) {
	uint32_t *APB2_registerAddr = (uint32_t*) APB2_Peripheral_Reset_Register;
	*APB2_registerAddr |= (1 << 8); //enabling peripheral clock

	uint32_t *ADCControl_registerAddr = (uint32_t*) ADC_Control_Register;
	*ADCControl_registerAddr |= (1 << 8); //Bit 8 - scan mode enabled

	for(;;);
}
