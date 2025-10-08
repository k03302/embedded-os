#ifndef KERNEL_SYNCH_H_
#define KERNEL_SYNCH_H_

#include "stdbool.h"
#include "stdint.h"

void Kernel_sem_init(uint32_t max);
bool Kernel_sem_test(void);
void Kernel_sem_release(void);

#endif