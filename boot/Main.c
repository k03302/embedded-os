#include "stdint.h"
#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"
#include "task.h"
#include "stdlib.h"
#include "stdio.h"
#include "Kernel.h"

static void Hw_init(void);
static void Printf_test(void);
static void Timer_test(void);
static void Kernel_init(void);
static void User_task0(void);
static void User_task1(void);
static void User_task2(void);
static bool User_event_handle0(uint8_t cmdBuf[16], uint32_t *cmdBufIdx);

int main()
{
    Hw_init();
    Kernel_init();

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
    Kernel_event_flag_init();
    Kernel_msgQ_init();
    Kernel_sem_init(1);

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

    Kernel_start();
}

bool User_event_handle0(uint8_t cmdBuf[16], uint32_t *cmdBufIdx)
{
    uint8_t uartch = 0;

    bool pendingEvent = true;
    KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_UartIn | KernelEventFlag_CmdOut);

    switch (handle_event)
    {
    case KernelEventFlag_UartIn:
        Kernel_recv_msg(KernelMsgQ_Task0, &uartch, 1);
        // if carriage return or out of length, send accumulated chars to task1
        if (uartch == '\r' || *cmdBufIdx >= 15)
        {
            cmdBuf[*cmdBufIdx] = '\0';
            Kernel_send_msg(KernelMsgQ_Task1, cmdBufIdx, 1);       // send length of message
            Kernel_send_msg(KernelMsgQ_Task1, cmdBuf, *cmdBufIdx); // send message
            Kernel_send_events(KernelEventFlag_CmdIn);
            *cmdBufIdx = 0;
        }
        // accumulate recieved char to buffer
        else
        {
            cmdBuf[*cmdBufIdx] = uartch;
            *cmdBufIdx = *cmdBufIdx + 1;
        }

        debug_printf("UartIn Event handled by task0\n");
        break;
    case KernelEventFlag_CmdOut:
        debug_printf("CmdOut Event handled by task0\n");
    default:
        pendingEvent = false;
    }

    return pendingEvent;
}

void User_task0(void)
{
    uint32_t local = 0;
    uint8_t cmdBuf[16];
    uint32_t cmdBufIdx = 0;

    debug_printf("User Task#0 SP=0x%x\n", &local);
    while (true)
    {
        while (User_event_handle0(cmdBuf, &cmdBufIdx))
            ;
        Kernel_yield();
    }
}

void User_task1(void)
{
    uint8_t cmdlen = 0;
    uint8_t cmd[16] = {0};
    uint32_t local = 0;
    debug_printf("User Task#1 SP=0x%x\n", &local);
    while (true)
    {
        KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_CmdIn);
        switch (handle_event)
        {
        case KernelEventFlag_CmdIn:
            Kernel_recv_msg(KernelMsgQ_Task1, &cmdlen, 1);
            Kernel_recv_msg(KernelMsgQ_Task1, cmd, cmdlen);
            debug_printf("CmdIn Event handled by task1, recieved : %s\n", cmd);
            break;
        }
        Kernel_yield();
    }
}