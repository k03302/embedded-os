#ifndef KERNEL_TASK_H_
#define KERNEL_TASK_H_

#include "MemoryMap.h"
#include "stdint.h"

#define USR_TASK_STACK_SIZE 0x100000
#define MAX_TASK_NUM (TASK_STACK_SIZE / USR_TASK_STACK_SIZE)

typedef struct KernelTaskContext_t
{
    uint32_t spsr;
    uint32_t r0_r12[13];
    uint32_t pc;
} KernelTaskContext_t;

typedef struct KernelTcb_t
{
    uint32_t sp;
    uint8_t *stack_base;
    uint32_t priority;
} KernelTcb_t;

typedef void (*KernelTaskFunc_t)(void);

/*
Initialize kernel stack & context
Each task's stack size is fixed as USR_TASK_STACK_SIZE

From top to bottom, each stack is composed of:
- padding (4byte)
- task context
- stack data
- sp (top of stack)
- empty memory
- stack_base (0)
*/
void Kernel_task_init(void);

/*
Create a new task with given start function
Returns task ID on success, -1 if task limit exceeded
*/
uint32_t Kernel_task_create(KernelTaskFunc_t startFunc, uint32_t priority);

/*
Perform context switch to next task

1. Backup current task's context
2. Get next task's control block
3. Read sp from control block
4. Reconstruct context from next task's stack
5. Set pc to next execution position

*/
void Kernel_task_schedule(void);

#endif