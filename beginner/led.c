#include <stdint.h>
#include <stdio.h>

int main(void)
{

	uint32_t *pClkCtrlreg = (uint32_t*) 0x40023830;
	uint32_t *pPortDModeReg = (uint32_t*) 0x40020C30;
	uint32_t *pPortDOutReg = (uint32_t*) 0x40020C14;

	//1. enabling the  clock for GPOID peripheral
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
	*pPortDModeReg &= ~(1<<24);
	*pPortDModeReg &= ~(1<<25);

	//*pPortDModeReg |= 0x01000000; //make the 24th bit as 1 (SET)
	*pPortDModeReg |= (1<<24);
	
	//*pPortDOutReg |= 0x1000; //SET 12th bit of the output data register to make I/O pin 12 high
	*pPortDOutReg |= (1<<12);

	while(1);


}
