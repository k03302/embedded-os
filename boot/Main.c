#include "stdint.h"
#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"
#include "stdlib.h"
#include "stdio.h"

static void Hw_init(void);
static void Printf_test(void);
static void Timer_test(void);

int main()
{
    Hw_init();

    Timer_test();

    return 0;
}

void Hw_init(void)
{
    Hal_interrupt_init();
    Hal_uart_init();
}

void Printf_test(void)
{
    while (1)
    {
        __asm__ volatile("WFI"); // Wait for interrupt
    }
}

void Timer_test(void)
{
    while (1)
    {
        debug_printf("current 1ms count: %u\n", Hal_timer_get_1ms_counter());
        delay(1000);
    }
}