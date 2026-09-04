#include <stdint.h>
#include <stdio.h>

#define USART3_IRQNO 39

int main(void)
{
	//1. Manually pend the pending bit for the USART3 IRQ number in NVIC
	uint32_t *pISPR1 = (uint32_t*) 0XE000E204;

	*pISPR1 |= (1 << (USART3_IRQNO % 32));

	//2. Enable the USART 3 IRQ Number in NVIC
	uint32_t *pISER1 = (uint32_t*) 0xE000E104;
	*pISER1 |= (1 << (USART3_IRQNO % 32));



	for(;;);
	return 0;
}

//USART3 ISR. You can get the names from startup.s file USART3_IRQHandler
void USART3_IRQHandler(void) {
	printf("in USART3 IRQ\n");
}

