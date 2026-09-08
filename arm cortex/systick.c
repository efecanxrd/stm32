#include <stdint.h>
#include <stdio.h>

#define SYST_CSR   (*(volatile uint32_t *)0xE000E010) // Control & Status Register
#define SYST_RVR   (*(volatile uint32_t *)0xE000E014) // Reload Value Register
#define SYST_CVR   (*(volatile uint32_t *)0xE000E018) // Current Value Register

#define SYST_CSR_ENABLE     (1U << 0)  // 1 = Counter enabled
#define SYST_CSR_TICKINT    (1U << 1)  // 1 = Exception request on countdown to 0
#define SYST_CSR_CLKSOURCE  (1U << 2)  // 1 = Processor clock (AHB), 0 = External

#define CPU_CLOCK_HZ        16000000U  // Example: 16 MHz default internal clock

volatile uint32_t ms_ticks = 0;

// 1. Systick ISR (Runs automatically every 1 ms)
void SysTick_Handler(void)
{
    ms_ticks++;
}

// 2. Configuration Function
void systick_init_1ms(void)
{
    // A 24-bit down-counter decrements every clock cycle.
    // For 1 ms at 16 MHz: 16,000,000 / 1000 = 16,000 cycles.
    // We subtract 1 because counting reaches 0 (N to 0 takes N+1 cycles).
    SYST_RVR = (CPU_CLOCK_HZ / 1000U) - 1U;

    // Clear the current value register by writing any value to it
    SYST_CVR = 0U;

    // Start SysTick: Processor clock + Enable Interrupt + Enable Counter
    SYST_CSR = SYST_CSR_CLKSOURCE | SYST_CSR_TICKINT | SYST_CSR_ENABLE;
}

// 3. Non-blocking & Blocking Delay Utilities
void delay_ms(uint32_t delay)
{
    uint32_t start = ms_ticks;
    while ((ms_ticks - start) < delay) {
        // Wait until enough 1ms ticks elapse
    }
}

int main(void)
{
    systick_init_1ms();

    while (1) {
        printf("Current uptime: %lu ms\n", ms_ticks);
        delay_ms(1000); // Wait 1 second cleanly without empty for-loops
    }
}
