#include <stdint.h>
#include <stdio.h>

int main(void)
{

	volatile uint32_t *pClkCtrlreg = (uint32_t*) 0x40023830;
	volatile uint32_t *pPortDModeReg = (uint32_t*) 0x40020C00;
	volatile uint32_t *pPortDOutReg = (uint32_t*) 0x40020C14;

	//1. enabling the  clock for GPOID peripheral GET FROM RCC_AHB1ENR
	/*
	uint32_t temp = *pClkCtrlreg;
	temp = temp | 0x08;
	*pClkCtrlReg = temp;

	*pClkCtrlReg = *pClkCtrlReg | 0x08;
	*/

	//*pClkCtrlreg |= 0x08; //shorter
	*pClkCtrlreg |= (1 << 3); //simplier w/bitwise shift operators

	//2. Configure the mode of the IO pin as output
	//*pPortDModeReg &= 0xFCFFFFFF; //CLEAR 24th and 25th bit positions
	*pPortDModeReg &= ~(3<<24);
	*pPortDModeReg &= ~(3<<26);

	//*pPortDModeReg |= 0x01000000; //make the 24th bit as 1 (SET)
	*pPortDModeReg |= (1<<24);
	*pPortDModeReg |= (1<<26);

	//*pPortDOutReg |= 0x1000; //SET 12th bit of the output data register to make I/O pin 12 high
	*pPortDOutReg |= (1<<12);
	*pPortDOutReg |= (1<<13);

	while(1);

	/*
	 Enabled the Peripheral Clock: Activated the clock for GPIOD via the RCC register so the hardware peripheral is powered and functional.

	Configured Pin Direction as Output: Used the MODER register to set the target pins to general-purpose output mode so the microcontroller actively drives signals outwards instead of reading inputs.

	Applied Voltage to Light the LEDs: Wrote 1 (HIGH / 3.3V) to the specific pin bits in the ODR register to supply current and illuminate the LEDs.
	 */

}
