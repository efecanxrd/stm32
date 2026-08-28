#include <stdint.h>
#include <stdio.h>

void RTC_WKUP_IRQHandler(void);
void HardFault_Handler(void);

void generate_interrupt() //Executes in THREAD MODE (needs to be priviliged though)
{
	uint32_t *pSTIR = (uint32_t*) 0xE000EF00;
	uint32_t *pISER0 = (uint32_t*) 0xE000E100;

	*pISER0 |= (1 << 3); //Enable IRQ3 interrupt
	*pSTIR = (3 & 0x1FF); //Generate an interrupt from software for IRQ3 - TRIGGERING
}

void change_access_level_unpriv(void) {
	__asm volatile("MRS R0, CONTROL"); //read
	__asm volatile("ORR R0,R0,#0x01"); //modify
	__asm volatile("MSR CONTROL,R0"); //write
}

int main(void) //Executes in THREAD MODE
{
	printf("IN THREAD MODE: Before interrupt:\n");
	void *(fun_ptr)(void);
	fun_ptr = change_access_level_unpriv;
	fun_ptr();
	generate_interrupt();
	printf("IN THREAD MODE: After interrupt\n");

	for(;;);
}

void RTC_WKUP_IRQHandler(void) { //Executes in HANDLER MODE
	printf("In handler mode: ISR\n");
}

void HardFault_Handler(void) {
	printf("Hard fault detected\n");
	while(1);
}
