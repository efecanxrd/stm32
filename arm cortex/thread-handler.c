#include <stdint.h>
#include <stdio.h>

void RTC_WKUP_IRQHandler(void);

void generate_interrupt() //Executes in THREAD MODE
{
	uint32_t *pSTIR = (uint32_t*) 0xE000EF00;
	uint32_t *pISER0 = (uint32_t*) 0xE000E100;

	*pISER0 |= (1 << 3); //Enable IRQ3 interrupt
	*pSTIR = (3 & 0x1FF); //Generate an interrupt from software for IRQ3 - TRIGGERING
}

int main(void) //Executes in THREAD MODE
{
	printf("IN THREAD MODE: Before interrupt:\n");
	generate_interrupt();
	printf("IN THREAD MODE: After interrupt\n");

	for(;;);
}

void RTC_WKUP_IRQHandler(void) { //Executes in HANDLER MODE
	printf("In handler mode: ISR\n");
}
