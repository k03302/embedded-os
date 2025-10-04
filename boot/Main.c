#include "stdint.h"
#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"
#include "task.h"
#include "stdlib.h"
#include "stdio.h"

static void Hw_init(void);
static void Printf_test(void);
static void Timer_test(void);
static void Kernel_init(void);
static void User_task0(void);
static void User_task1(void);
static void User_task2(void);

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
    Hal_timer_init();
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

void Kernel_init(void)
{
    uint32_t taskId;

    Kernel_task_init();

    taskId = Kernel_task_create(User_task0, 0);
    if (taskId == -1)
    {
        putstr("Task0 create failed\n");
    }

    taskId = Kernel_task_create(User_task1, 0);
    if (taskId == -1)
    {
        putstr("Task1 create failed\n");
    }

    taskId = Kernel_task_create(User_task2, 0);
    if (taskId == -1)
    {
        putstr("Task2 create failed\n");
    }
}

void User_task0(void)
{
    putstr("User Task0\n");
}

void User_task1(void)
{
    putstr("User Task1\n");
}

void User_task2(void)
{
    putstr("User Task2\n");
}