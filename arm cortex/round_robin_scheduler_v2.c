#include <stdint.h>
#include <stdio.h>

#define MAX_TASKS 5 //plus 1 idle task
void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

void init_tasks_stack(void);
void init_systick_timer(uint32_t tick_hz);
void enable_processor_faults(void);
void update_next_task(void);
void idle_task(void);
void schedule(void);
void task_delay(uint32_t tick_count);
void unblock_tasks(void);
void save_psp_value(uint32_t current_stack_addr);
void update_global_tick_count(void);
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
#define IDLE_STACK_START ((SRAM_END) - (4 * SIZE_TASK_STACK))
#define SCHED_STACK_START ((SRAM_END) - (5 * SIZE_TASK_STACK))

#define TICK_HZ 1000U
#define HSI_CLOCK 16000000U
#define SYSTICK_TIM_CLK HSI_CLOCK

#define DUMMY_XPSR 0x01000000U
#define TASK_RUNNING_STATE 0x00
#define TASK_BLOCKED_STATE 0xFF


uint8_t current_task = 1;
uint32_t g_tick_count = 0;

typedef struct {
	uint32_t psp_value;
	uint32_t block_count;
	uint8_t current_state;
	void (*task_handler)(void);
} TCB_t;

TCB_t user_tasks[MAX_TASKS];

int main(void)
{
	enable_processor_faults();
	init_scheduler_stack(SCHED_STACK_START);

	init_tasks_stack();
	init_systick_timer(TICK_HZ);

	switch_sp_to_psp();

	task1_handler();

	for(;;);
}

void idle_task(void) {
	while(1);
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
	user_tasks[0].current_state = TASK_RUNNING_STATE;
	user_tasks[1].current_state = TASK_RUNNING_STATE;
	user_tasks[2].current_state = TASK_RUNNING_STATE;
	user_tasks[3].current_state = TASK_RUNNING_STATE;
	user_tasks[4].current_state = TASK_RUNNING_STATE;

	user_tasks[0].psp_value = IDLE_STACK_START;
	user_tasks[1].psp_value = T1_STACK_START;
	user_tasks[2].psp_value = T2_STACK_START;
	user_tasks[3].psp_value = T3_STACK_START;
	user_tasks[4].psp_value = T4_STACK_START;


	user_tasks[0].task_handler = idle_task;
	user_tasks[1].task_handler = task1_handler;
	user_tasks[2].task_handler = task2_handler;
	user_tasks[3].task_handler = task3_handler;
	user_tasks[4].task_handler = task4_handler;


    uint32_t *pPSP;

    for(int i = 0; i < MAX_TASKS; i++) {
        pPSP = (uint32_t*) user_tasks[i].psp_value;

        // Hardware Frame (Pushed automatically by Cortex-M on exception)
        pPSP--;
        *pPSP = DUMMY_XPSR;               // xPSR (bit 24 = 1)
        pPSP--;
        *pPSP = (uint32_t) user_tasks[i].task_handler;          // PC (Entry point)
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

        user_tasks[i].psp_value = (uint32_t) pPSP;
    }
}

void enable_processor_faults(void) {
	uint32_t *pSHCSR = (uint32_t*) 0xE000ED24;

	*pSHCSR |= (1 << 16); //mem manage
	*pSHCSR |= (1 << 17); //bus fault
	*pSHCSR |= (1 << 18); //usage fault
}


uint32_t get_psp_value(void) {
	return user_tasks[current_task].psp_value;
}

void save_psp_value(uint32_t current_stack_addr) {
	user_tasks[current_task].psp_value = current_stack_addr;
}

void update_next_task(void) {
	int state = TASK_BLOCKED_STATE;

	for(int i = 0; i < (MAX_TASKS); i++) {
		current_task++;
		current_task %= MAX_TASKS;
		state = user_tasks[current_task].current_state;
		if( (state == TASK_RUNNING_STATE) && (current_task != 0))
			break;
	}
	if(state != TASK_RUNNING_STATE) {
		current_task = 0;
	}
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


__attribute__ ((naked)) void PendSV_Handler(void) {
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

void unblock_tasks(void) {
	for(int i = 0; i < MAX_TASKS; i++) {
		if(user_tasks[i].current_state != TASK_RUNNING_STATE) {
			if(user_tasks[i].block_count == g_tick_count) {
				user_tasks[i].current_state = TASK_RUNNING_STATE;
			}
		}
	}
}

void update_global_tick_count(void) {
	g_tick_count++;
}

__attribute__ ((naked)) void SysTick_Handler(void) {
	uint32_t *pICSR = (uint32_t*) 0xE000ED04;
	update_global_tick_count();
	unblock_tasks();
	//pend the pendSV Exception
	*pICSR |= (1 << 28);
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

void schedule(void) {
	uint32_t *pICSR = (uint32_t*) 0xE000ED04;
	*pICSR |= (1 << 28);
}

void task_delay(uint32_t tick_count) {
	if(current_task) {
	user_tasks[current_task].block_count = g_tick_count + tick_count;
	user_tasks[current_task].current_state = TASK_BLOCKED_STATE;
	schedule();
	}
}

