#include <stdint.h>
#include <stdio.h>

#define SRAM_START 0x20000000U
#define SRAM_SIZE (128 * 1024)
#define SRAM_END ((SRAM_START) + (SRAM_SIZE))
#define STACK_START SRAM_END

#define STACK_MSP_START STACK_START
#define STACK_MSP_END (STACK_MSP_START - 512)
#define STACK_PSP_START STACK_MSP_END


int fun_add(int a, int b, int c, int d) {
	return a+b+c+d;
}

__attribute__((naked)) void change_sp_to_psp(void) {
	__asm volatile(".equ SRAM_END, (0x20000000 + (128 * 1024))");
	__asm volatile(".equ PSP_START, (SRAM_END-512)");
	__asm volatile("LDR R0,=STACK_PSP_START");
	__asm volatile("MSR PSP,R0");
	__asm volatile("MOV R0,#0X02");
	__asm volatile("MSR CONTROL, R0");
	__asm volatile("BX LR");
}


void generate_exception(void) { //svc exception will be called
	__asm volatile("SVC #0X2");
}

int main(void)
{
	change_sp_to_psp();
	int ret;

	ret = fun_add(1,4,5,6);
	printf("result = %d\n", ret);

	generate_exception();


	for(;;);
	return 0;
}

void SVC_Handler(void) {
	printf(" in SVC_Handler\n");
}

