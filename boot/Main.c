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

    i = 100;
    while (i--)
    {
        uint8_t ch = Hal_uart_get_char();
        Hal_uart_put_char(ch);
    }

    while (1)
        ;
}

void Hw_init(void)
{
    Hal_uart_init();
}