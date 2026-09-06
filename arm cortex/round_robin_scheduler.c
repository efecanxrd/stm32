/*
This code implements a bare-metal, round-robin preemptive multitasking scheduler designed for an ARM Cortex-M processor.
It partitions SRAM into dedicated stack spaces to separate the scheduler's Main Stack Pointer (MSP) from each individual task's Process Stack Pointer (PSP).
The system enables processor fault exceptions and configures the core SysTick timer to trigger periodic interrupts at 1 kHz to act as the OS time slice.
It pre-populates each task's stack with a fabricated exception frame, including both the hardware-saved registers (such as xPSR, PC, and LR) and software-saved registers (R4–R11), making tasks ready to execute upon context restoration.
The CPU switches thread-mode execution to use the Process Stack Pointer (PSP) via the CONTROL register and jumps directly into the first task.
When a SysTick interrupt occurs, the hardware automatically saves registers R0–R3, R12, LR, PC, and xPSR to the active task's PSP, after which the handler manually pushes R4–R11 and stores the updated stack pointer.
The scheduler updates the task index in a round-robin sequence to select the next task and loads its previously saved PSP.
Finally, the handler restores registers R4–R11 from the new task's stack and executes an exception return (BX LR), allowing the ARM hardware to automatically unstack the remaining registers and seamlessly resume the next task.
*/

#include <stdint.h>
#include <stdio.h>

#define MAX_TASKS 4
void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

void init_tasks_stack(void);
void init_systick_timer(uint32_t tick_hz);
void enable_processor_faults(void);
void update_next_task(void);
void save_psp_value(uint32_t current_stack_addr);
uint32_t get_psp_value(void);
__attribute__ ((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack);
__attribute__ ((naked)) void switch_sp_to_psp(void);

// some stack memory calculations
#define SIZE_TASK_STACK 1024U
#define SIZE_SCHED_STACK 1024U

#define SRAM_START 0x20000000
#define SIZE_SRAM ((128) * (1024))
#define SRAM_END ((SRAM_START) + (SIZE_SRAM))

#define T1_STACK_START SRAM_END
#define T2_STACK_START ((SRAM_END) - (1 * SIZE_TASK_STACK))
#define T3_STACK_START ((SRAM_END) - (2 * SIZE_TASK_STACK))
#define T4_STACK_START ((SRAM_END) - (3 * SIZE_TASK_STACK))
#define SCHED_STACK_START ((SRAM_END) - (4 * SIZE_TASK_STACK))

#define TICK_HZ 1000U
#define HSI_CLOCK 16000000U
#define SYSTICK_TIM_CLK HSI_CLOCK

#define DUMMY_XPSR 0x01000000U

uint32_t psp_of_tasks[MAX_TASKS] = {T1_STACK_START, T2_STACK_START, T3_STACK_START, T4_STACK_START};
uint32_t task_handlers[MAX_TASKS];
uint8_t current_task = 0;


int main(void)
{
	enable_processor_faults();
	init_scheduler_stack(SCHED_STACK_START);

	task_handlers[0] = (uint32_t) task1_handler;
	task_handlers[1] = (uint32_t) task2_handler;
	task_handlers[2] = (uint32_t) task3_handler;
	task_handlers[3] = (uint32_t) task4_handler;

	init_tasks_stack();
	init_systick_timer(TICK_HZ);

	switch_sp_to_psp();

	task1_handler();

	for(;;);
}

void task1_handler(void) {
	while(1) {
		printf("This is task1\n");
	}
}

void task2_handler(void) {
	while(1) {
		printf("This is task2\n");
	}
}

void task3_handler(void) {
	while(1) {
		printf("This is task3\n");
	}
}

void task4_handler(void) {
	while(1) {
		printf("This is task4\n");
	}
}

void init_systick_timer(uint32_t tick_hz) {
	uint32_t *pSRVR = (uint32_t*)0xE000E014;
	uint32_t *pSCSR = (uint32_t*)0xE000E010;
	uint32_t count_value = (SYSTICK_TIM_CLK / tick_hz)-1;

	//Clear the value of SVR
	*pSRVR &= ~(0x00FFFFFFFF);

	//load the value into SVR
	*pSRVR |= count_value;

	//do some settings
	*pSCSR |= (1 << 1); //Enables Systick exception request
	*pSCSR |= (1 << 2);

	//enable the systick
	*pSCSR |= (1 << 0); //enables the counter
}


__attribute__ ((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack) {
	__asm volatile("MSR MSP,R0"::"r"(sched_top_of_stack):);
	__asm volatile("BX LR"); //return from func call
}

void init_tasks_stack(void) {
    uint32_t *pPSP;

    for(int i = 0; i < MAX_TASKS; i++) {
        pPSP = (uint32_t*) psp_of_tasks[i];

        // Hardware Frame (Pushed automatically by Cortex-M on exception)
        pPSP--;
        *pPSP = DUMMY_XPSR;               // xPSR (bit 24 = 1)
        pPSP--;
        *pPSP = task_handlers[i];          // PC (Entry point)
        pPSP--;
        *pPSP = 0;                         // LR (Return address for task handler)
        pPSP--;
        *pPSP = 0;                         // R12
        pPSP--;
        *pPSP = 0;                         // R3
        pPSP--;
        *pPSP = 0;                         // R2
        pPSP--;
        *pPSP = 0;                         // R1
        pPSP--;
        *pPSP = 0;                         // R0

        // Software Frame (Pushed manually via STMDB R0!, {R4-R11})
        for(int j = 0; j < 8; j++) {
            pPSP--;
            *pPSP = 0;                     // R11 down to R4
        }

        psp_of_tasks[i] = (uint32_t) pPSP;
    }
}

void enable_processor_faults(void) {
	uint32_t *pSHCSR = (uint32_t*) 0xE000ED24;

	*pSHCSR |= (1 << 16); //mem manage
	*pSHCSR |= (1 << 17); //bus fault
	*pSHCSR |= (1 << 18); //usage fault
}


uint32_t get_psp_value(void) {
	return psp_of_tasks[current_task];
}

void save_psp_value(uint32_t current_stack_addr) {
	psp_of_tasks[current_task] = current_stack_addr;
}

void update_next_task(void) {
	current_task++;
	current_task = current_task % MAX_TASKS;
}

__attribute__ ((naked)) void switch_sp_to_psp(void) {
	//1. Initialize the PSP with TASK1 stack start*
	//get the value of psp of current task
	__asm volatile("PUSH {LR}"); //preserve LR which connects back to main()
	__asm volatile("BL get_psp_value"); //Code will branch to get get_psp_value. Return val recorded to R0
	__asm volatile("MSR PSP,R0"); //initialize psp
	__asm volatile("POP {LR}"); //pops back LR value

	//2. Change SP to PSP using CONTROL Register
	__asm volatile("MOV R0,#0x02");
	__asm volatile("MSR CONTROL,R0");
	__asm volatile("BX LR");
}


__attribute__ ((naked)) void SysTick_Handler(void) {
	/* Save the context of current task */

	//1. Get current running task's PSP value
	__asm volatile("MRS R0,PSP");
	//2. Using that PSP value to sore SF2 (R4 to R11)
	__asm volatile("STMDB R0!,{R4-R11}");
	__asm volatile("PUSH {LR}");
	//3. Save the current value of PSP
	__asm volatile("BL save_psp_value");

	/* Retrieve the context of the next task */

	//1. Decide next task to run
	__asm volatile("BL update_next_task");
	//2. get its past PSP value
	__asm volatile("BL get_psp_value");
	//3. Using that PSP value retrieve SF2 (R4 to R11)
	__asm volatile("LDMIA R0!,{R4-R11}");
	//4. update PSP and exit
	__asm volatile("MSR PSP,R0");
	__asm volatile("POP {LR}");
	__asm volatile("BX LR");
}

//2. Implement the fault handlers
void HardFault_Handler(void) {
	printf("Exception: HardFault\n");
	while(1);
}

void MemManage_Handler(void) {
	printf("Exception: MemManage\n");
	while(1);
}

void BusFault_Handler(void) {
	printf("Exception: BusFault\n");
	while(1);
}

