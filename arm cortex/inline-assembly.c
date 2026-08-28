#include <stdint.h>
#include <stdio.h>

//__asm volatile("MOV R0,R1");

//LDR R0,[R1]: 	LOAD: 	Loads the value stored in memory address R1 to R0
//ADD R4,R0,R3: 	ADD: 	Adds two values (R0 and R3),then stores it to R4
//STR R4,[R5]: 	STORE: 	Stores R4 TO THE R5
//MOV R0,R1: 	MOVE:	Copies the contents of R1 to R0   |   MOV R0, #10 directly copies constant 10 to R0
//MRS{cond} Rd, spec_reg:Move the contents of a specific register to a general-purpose register

//__asm volatile(CODE: 	OUTPUT OPERAND LIST: INPUT OPERAND LIST: CLOBBER LIST)

int main(void)
{
	 /*__asm volatile
	 (
	 "LDR R1,=#0x20001000\n\t"
	 "LDR R2,=#0x200001004\n\t"
	 "LDR R0,[R1]\n\t"
	 "LDR R1,[R2]\n\t"
	 "ADD R0,R0,R1\n\t"
	 "STR R0,[R2]\n\t"
	);*/

	__asm volatile("LDR R1,=#0x20001000");
	__asm volatile("LDR R2,=#0x20001004");
	__asm volatile("LDR R0,[R1]");
	__asm volatile("LDR R1,[R2]");
	__asm volatile("ADD R0,R0,R1");
	__asm volatile("STR R0,[R2]");

	int val=50;
	__asm volatile("MOV R0,%0": :"r"(val)); //you can omit the colon for CLOBBER LIST
	//The constraint string "r" tells the compiler to use the registers for the data manipulation
	//Check manuals for more

	int control_reg;
	//read CONTROL register to control_reg
	__asm volatile("MRS %0,CONTROL":"=r"(control_reg)::);
	//= write-only operand    |   + read-write operand   |   & A register that should be used for output only


	//Ex: Copy the content of var1 to var2
	int var1=10, var2;
	__asm volatile("MOV %0,%1":"=r"(var2):"r"(var1));

	//Ex: Copy the contents of a pointer into another variable
	int p1, *p2;
	p2 = (int*)0x20000008;
	__asm volatile("LDR %0,[%1]": "=r"(p1): "r"(p2)); //p1 = *p2

	for(;;);
	return 0;
}


