#include "Kernel.h"
#include "memio.h"

void Kernel_yield(void)
{
    Kernel_task_scheduler();
}

void Kernel_start(void)
{
    Kernel_task_start();
}

void Kernel_send_events(uint32_t event_list)
{
    for (uint32_t i = 0; i < 32; i++)
    {
        if ((event_list >> i) & 1)
        {
            Kernel_event_flag_set((KernelEventFlag_t)(1 << i));
        }
    }
}

KernelEventFlag_t Kernel_wait_events(uint32_t event_list)
{
    for (uint32_t i = 0; i < 32; i++)
    {
        if ((event_list >> i) & 1)
        {
            KernelEventFlag_t waiting_event = KernelEventFlag_Empty;
            waiting_event = (KernelEventFlag_t)SET_BIT(waiting_event, i);

            if (Kernel_event_flag_check(waiting_event))
            {
                return waiting_event;
            }
        }
    }

    return KernelEventFlag_Empty;
}

bool Kernel_send_msg(KernelMsgQ_t Qname, void *data, uint32_t count)
{
    uint8_t *data_ptr = (uint8_t *)data;
    uint32_t size_left = Kernel_msgQ_get_size_left(Qname);
    if (size_left < count)
    {
        return false;
    }
    for (uint32_t i = 0; i < count; i++)
    {
        if (Kernel_msgQ_enqueue(Qname, *data_ptr) == false)
        {
            for (uint32_t j = 0; j < i; j++)
            {
                uint8_t rollback;
                Kernel_msgQ_dequeue(Qname, &rollback);
            }
            return false;
        }
        data_ptr++;
    }
    return true;
}

uint32_t Kernel_recv_msg(KernelMsgQ_t Qname, void *out_data, uint32_t count)
{
    uint8_t *out_data_ptr = (uint8_t *)out_data;

    for (uint32_t i = 0; i < count; i++)
    {
        if (Kernel_msgQ_dequeue(Qname, out_data_ptr) == false)
        {
            return i;
        }
        out_data_ptr++;
    }

    return count;
}

void Kernel_lock_sem(void)
{
    while (false == Kernel_sem_test())
    {
        Kernel_yield();
    }
}

void Kernel_unlock_sem(void)
{
    Kernel_sem_release();
}