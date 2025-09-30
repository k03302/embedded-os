#include <stdint.h>
#include "Uart.h"
#include "HalUart.h"

extern volatile PL011_t *Uart;

void Hal_uart_init(void)
{
    // unable UART
    Uart->uartcr.bits.UARTEN = 0;

    // enable transmimt
    Uart->uartcr.bits.TXE = 1;

    // enable receive
    Uart->uartcr.bits.RXE = 1;

    // enable UART
    Uart->uartcr.bits.UARTEN = 1;
}

void Hal_uart_put_char(uint8_t ch)
{
    // wait until transmit FIFO is not full
    while (Uart->uartfr.bits.TXFF)
        ;

    // send data
    Uart->uartdr.all = (ch & 0xFF);
}

uint8_t Hal_uart_get_char(void)
{
    // wait until FIFO is empty
    while (Uart->uartfr.bits.RXFE)
        ;

    // check error flag
    if (Uart->uartdr.all & 0xFFFFFF00)
    {
        Uart->uartrsr.all = 0xFF;
        return 0;
    }

    // set output char
    uint8_t data = Uart->uartdr.bits.DATA;
    return data;
}