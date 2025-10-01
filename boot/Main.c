#include "stdint.h"
#include "HalUart.h"
#include "HalInterrupt.h"
#include "stdio.h"

static void Hw_init(void);

int main()
{
    Hw_init();
    uint32_t i = 100;

    putstr("Hello World!\n");

    while (1)
        ;
}

void Hw_init(void)
{
    Hal_interrupt_init();
    Hal_uart_init();
}