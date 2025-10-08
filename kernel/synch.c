#include "synch.h"

static uint32_t sSemMax;
static int32_t sSem;

void Kernel_sem_init(uint32_t max)
{
    sSemMax = max > 0 ? max : 1;
}

bool Kernel_sem_test(void)
{
    if (sSem <= 0)
    {
        return false;
    }

    sSem--;
    return true;
}

void Kernel_sem_release(void)
{
    sSem++;
    if (sSem >= sSemMax)
    {
        sSem = sSemMax;
    }
}