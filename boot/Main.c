#include "stdint.h"
#include "HalUart.h"
#include "stdio.h"

static void Hw_init(void);

int main()
{
    Hw_init();
    uint32_t i = 100;
    while (i--)
    {
        Hal_uart_put_char('N');
    }
    Hal_uart_put_char('\n');

    putstr("Hello World!");
}

void Hw_init(void)
{
    Hal_uart_init();
}