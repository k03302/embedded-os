#include "task.h"
#include "ARMv7AR.h"

static KernelTcb_t sTask_list[MAX_TASK_NUM];
static uint32_t sAllocated_tcb_index;

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
void Kernel_task_init(void)
{
    sAllocated_tcb_index = 0;

    for (uint32_t i = 0; i < MAX_TASK_NUM; i++)
    {
        sTask_list[i].stack_base = (uint32_t)(TASK_STACK_START + i * TASK_STACK_SIZE);
        sTask_list[i].sp = (uint32_t)sTask_list[i].stack_base + TASK_STACK_SIZE - 4;
        sTask_list[i].sp -= sizeof(KernelTaskContext_t);
        KernelTaskContext_t *ctx = (KernelTaskContext_t *)sTask_list[i].sp;
        ctx->pc = 0;
        ctx->spsr = ARM_MODE_BIT_SYS;
    }
}

/*
Create a new task with given start function
Returns task ID on success, -1 if task limit exceeded
*/
uint32_t Kernel_task_create(KernelTaskFunc_t startFunc)
{
    KernelTcb_t *new_tcb = &sTask_list[sAllocated_tcb_index++];

    if (sAllocated_tcb_index > MAX_TASK_NUM)
    {
        return -1;
    }

    KernelTaskContext_t *ctx = (KernelTaskContext_t *)new_tcb->sp;
    ctx->pc = (uint32_t)startFunc;

    return (sAllocated_tcb_index - 1);
}