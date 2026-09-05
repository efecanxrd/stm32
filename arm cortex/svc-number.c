#include <stdint.h>
#include <stdio.h>

/*
Hardware Automatically Stacks 8 Registers:
Before entering the handler, the Cortex-M hardware pushes an 8-word stack frame onto the Main Stack Pointer (MSP):
[0] = Saved R0, [1] = Saved R1, [2] = Saved R2, [3] = Saved R3, [4] = Saved R12, [5] = Saved LR, [6] = Saved PC (points to instruction after SVC #8), [7] = Saved xPSR
*/

int main(void)
{
	__asm("SVC #8"); //The CPU halts standard execution and triggers the SVC_Handler exception.

	//register uint32_t data __asm("r0"); //register variable to bind the C variable "data" directly to the CPU's physical hardware register "r0"

	uint32_t data;
	__asm volatile("MOV %0,R0":"=r"(data)::);

	printf(" data = %ld\n",data);

	for(;;);
}

__attribute__ ((naked)) void SVC_Handler(void) {
	//1. get the value of the MSP
	__asm("MRS R0,MSP"); //Get MSP to R0
	__asm("B SVC_Handler_c"); //According to the AAPCS, R0 becomes first argument for calling the func
}

void SVC_Handler_c(uint32_t *pBaseOfStackFrame) {
	printf("in SVC handler!!\n");

	uint8_t *pReturn_addr = (uint8_t*) pBaseOfStackFrame[6];

	//2. Decrement the return address by 2 to point to opcode of the SVC instruction in program memory
	pReturn_addr -= 2;

	//3. extract the SVC number (LSByte of the opcode)
	uint8_t svc_number = *pReturn_addr;

	printf("SVC number is: %d\n", svc_number);

	svc_number += 4;

	pBaseOfStackFrame[0] = svc_number; //Location of r0
}
