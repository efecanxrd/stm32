#include <stdio.h>
#include <stdint.h>

//Enables MCO1 and PA8 pin.
//See the results in PulseView

#define RCC_CFGR_REG_ADDR ((0x40023800) + (0x44))
#define RCC_AHB1_REG_ADDR ((0x40023800) + (0x30))
#define GPIOA_MODE_REG_ADDR ((0x40020000) + 00)
#define GPIOA_ALTFUNHIGH_ADDR ((0x40020000) + 0x24)


int main(void) {
	//1. Configure the RCC_CFGR MCO1 bit fields to select HSI as clock source
	uint32_t *pRccCfgrReg = (uint32_t*) RCC_CFGR_REG_ADDR;
	*pRccCfgrReg &= ~(0x3 << 21); //clear 21th and 23th bit positions

	//configure MCO1 prescaler
	*pRccCfgrReg |= (1<<24);

	//2. Configure PA8 to AF0 mode to behave as MCO1 signal
	//2.1 Enable the peripheral clock for GPIOA peripheral
	uint32_t *pRCCAhb1Enr = (uint32_t*) (RCC_AHB1_REG_ADDR);
	*pRCCAhb1Enr |= (1 << 0); //enable GPIOA peripheral clock

	//2.2 Configure the mode of GPIOA pin 8 as alternate function mode
	uint32_t *pGPIOAModeReg = (uint32_t*) (GPIOA_MODE_REG_ADDR);
	*pGPIOAModeReg &= ~(0x3 << 16); //clear
	*pGPIOAModeReg |= (0x2 << 16); //set

	//2.3 Conffigure the alternation function register to set the mode 0 for PA8
	uint32_t *pGPIOAAltFunHighReg = (uint32_t*)(GPIOA_ALTFUNHIGH_ADDR);
	*pGPIOAAltFunHighReg &= ~(0xf << 0);



	for(;;);
}
