#include "msg.h"

static KernelCirQ_t sMsgQ[KernelMsgQ_Num];

void Kernel_msgQ_init(void)
{
    for (uint32_t i = 0; i < KernelMsgQ_Num; i++)
    {
        sMsgQ[i].front = 0;
        sMsgQ[i].rear = 0;
    }
}
bool Kernel_msgQ_is_empty(KernelMsgQ_t Qname)
{
    return sMsgQ[Qname].front == sMsgQ[Qname].rear;
}
bool Kernel_msgQ_is_full(KernelMsgQ_t Qname)
{
    return (sMsgQ[Qname].rear + 1) % MSG_Q_SIZE_BYTE == sMsgQ[Qname].front;
}
uint32_t Kernel_msgQ_get_size_left(KernelMsgQ_t Qname)
{
    uint32_t front = sMsgQ[Qname].front;
    uint32_t rear = sMsgQ[Qname].rear;
    if (front <= rear)
    {
        return MSG_Q_SIZE_BYTE + front - rear;
    }
    return MSG_Q_SIZE_BYTE + rear - front;
}
bool Kernel_msgQ_enqueue(KernelMsgQ_t Qname, uint8_t data)
{
    if (Kernel_msgQ_is_full(Qname))
    {
        return false;
    }

    uint32_t idx = (sMsgQ[Qname].rear + 1) % MSG_Q_SIZE_BYTE;
    sMsgQ[Qname].Queue[idx] = data;
    sMsgQ[Qname].rear = idx;

    return true;
}
bool Kernel_msgQ_dequeue(KernelMsgQ_t Qname, uint8_t *out_data)
{
    if (Kernel_msgQ_is_empty(Qname))
    {
        return false;
    }

    uint32_t idx = (sMsgQ[Qname].front + 1) % MSG_Q_SIZE_BYTE;
    *out_data = sMsgQ[Qname].Queue[idx];
    sMsgQ[Qname].front = idx;

    return true;
}