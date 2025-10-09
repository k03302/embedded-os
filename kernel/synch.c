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

static KernelMutex_t sMutex;

void Kernel_mutex_init(void)
{
    sMutex.owner = 0;
    sMutex.lock = false;
}

bool Kernel_mutex_lock(uint32_t owner)
{
    if (sMutex.lock)
    {
        return false;
    }
    sMutex.owner = owner;
    sMutex.lock = true;
    return true;
}

bool Kernel_mutex_unlock(uint32_t owner)
{
    if (sMutex.owner == owner)
    {
        sMutex.lock = false;
        return true;
    }
    return false;
}