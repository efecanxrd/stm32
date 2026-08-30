#include <stdint.h>
#include <stdio.h>

/*
 	 Calculate the bit band alias address for given bit band memory address and bit position
 	 7th bit position of the memory location 0x2000_0200 using its alias address
 	 General formula:
 	 Alias address = alias_base + (32 * (bit_band_memory_addr - bit_band_base)) + bit * 4

	 Bit-banding is a hardware-supported shortcut for what normally do with bitwise operations like &= ~(1 << n)
   Can be used for SRAM bit-band region: 0x20000000 – 0x200FFFFF   AND
   Peripheral bit-band region: 0x40000000 – 0x400FFFFF
*/

#define ALIAS_BASE 0x22000000
#define BITBAND_BASE 0x20000000

int main(void)
{
	uint8_t *ptr = (uint8_t*)0x20000000;
	*ptr = 0xff;

	//normal method
	//clearing 7th position
	*ptr &= ~(1 << 7);

	//reset
	*ptr = 0xff;


	//bit band method
	uint8_t* alias_addr = (uint8_t*) ALIAS_BASE + (32 * (0x20000200 - BITBAND_BASE)) + 7 * 4;

	//clearing 7th bit of 0x20000200
	*alias_addr = 0;


	for(;;);
	return 0;
}


