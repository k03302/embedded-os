#include "stdint.h"
#include "HalUart.h"
#include "stdio.h"
#include "stdarg.h"

#define PRINTF_BUF_LEN 1024

static char printf_buf[PRINTF_BUF_LEN]; // 1KB

uint32_t putstr(const char *s)
{
    uint32_t c = 0;
    while (*s)
    {
        Hal_uart_put_char(*s++);
        c++;
    }
    return c;
}

uint32_t utoa(char *buf, uint32_t val, utoa_t base)
{
    uint32_t c = 0;
    int32_t idx = 0;
    char tmp[11]; // It is big enough for store 32 bit int

    do
    {
        uint32_t t = val % (uint32_t)base;
        if (t >= 10)
        {
            t += 'A' - '0' - 10;
        }
        tmp[idx] = (t + '0');
        val /= base;
        idx++;
    } while (val);

    // reverse
    idx--;
    while (idx >= 0)
    {
        buf[c++] = tmp[idx];
        idx--;
    }

    return c;
}

uint32_t vsprintf(char *buf, const char *format, va_list arg)
{
    const char *format_ptr = format;
    char *buf_ptr = buf;
    char *str = NULL;
    uint32_t uint;

    while (*format_ptr != '\0')
    {
        if (*format_ptr == '%')
        {
            format_ptr++;
            switch (*format_ptr)
            {
            case 'c':
                *buf_ptr = (char)va_arg(arg, int32_t);
                buf_ptr++;
                break;

            case 's':
                str = (char *)va_arg(arg, char *);
                if (str == NULL)
                {
                    str = "(null)";
                }
                while (*str != '\0')
                {
                    *buf_ptr = *str;
                    buf_ptr++;
                    str++;
                }
                break;

            case 'u':
                uint = (uint32_t)va_arg(arg, uint32_t);
                buf_ptr += utoa(buf_ptr, uint, utoa_dec);
                break;

            case 'x':
                uint = (uint32_t)va_arg(arg, uint32_t);
                buf_ptr += utoa(buf_ptr, uint, utoa_hex);
                break;
            }
            format_ptr++;
        }
        else
        {
            *buf_ptr = *format_ptr;
            buf_ptr++;
            format_ptr++;
        }
    }

    *buf_ptr = '\0';
}

uint32_t debug_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsprintf(printf_buf, format, args);
    va_end(args);

    return putstr(printf_buf);
}