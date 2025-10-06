#include "task.h"
#include "ARMv7AR.h"

static KernelTcb_t sTask_list[MAX_TASK_NUM];
static uint32_t sAllocated_tcb_index;
static KernelTcb_t *sCurrent_tcb, *sNext_tcb;
static uint32_t sCurrent_tcb_index;

/*
Perform context switch from the task of `sCurrent_tcb` to the task of `sNext_tcb`
*/
static void Kernel_task_context_switching(void);
static void Save_context(void);
static void Restore_context(void);

static KernelTcb_t *Scheduler_round_robin_algorithm(void);
static KernelTcb_t *Scheduler_periority_algorithm(void);

void Kernel_task_init(void)
{
    sAllocated_tcb_index = 0;
    sCurrent_tcb_index = 0;
    sCurrent_tcb = NULL;

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

void Kernel_task_start(void)
{
    sNext_tcb = &sTask_list[sCurrent_tcb_index];
    Restore_context();
}

uint32_t Kernel_task_create(KernelTaskFunc_t startFunc, uint32_t priority)
{
    KernelTcb_t *new_tcb = &sTask_list[sAllocated_tcb_index++];
    new_tcb->priority = priority;

    if (sAllocated_tcb_index > MAX_TASK_NUM)
    {
        return -1;
    }

    KernelTaskContext_t *ctx = (KernelTaskContext_t *)new_tcb->sp;
    ctx->pc = (uint32_t)startFunc;

    return (sAllocated_tcb_index - 1);
}

void Kernel_task_scheduler(void)
{
    sCurrent_tcb = &sTask_list[sCurrent_tcb_index];
    sNext_tcb = Scheduler_round_robin_algorithm();
    if (sCurrent_tcb != NULL && sNext_tcb != NULL && sCurrent_tcb != sNext_tcb)
    {
        Kernel_task_context_switching();
    }
}

__attribute__((naked)) void Kernel_task_context_switching(void)
{
    __asm__("B Save_context");
    __asm__("B Restore_context");
}

__attribute__((naked)) void Save_context(void)
{
    __asm__("PUSH {lr, r0-r12}");
    __asm__("MRS r0, cpsr");
    __asm__("PUSH {r0}");

    __asm__("LDR r0, =sCurrent_tcb"); // sCurrent_tcb의 주솟값을 저장
    __asm__("LDR r0, [r0]");          // sCurrent_tcb의 값을 저장
    __asm__("STMIA r0!, {sp}");       // sCurrent_tcb (sp)자리에 현재의 sp값을 저장
}

__attribute__((naked)) void Restore_context(void)
{
    __asm__("LDR r0, =sNext_tcb");
    __asm__("LDR r0, [r0]");
    __asm__("LDMIA r0!, {sp}");

    __asm__("POP {r0}");
    __asm__("MSR cpsr, r0");
    __asm__("POP {r0-r12, pc}");
}

KernelTcb_t *Scheduler_round_robin_algorithm(void)
{
    if (sAllocated_tcb_index == 0)
    {
        return NULL;
    }

    sCurrent_tcb_index++;
    if (sCurrent_tcb_index >= sAllocated_tcb_index)
    {
        sCurrent_tcb_index = 0;
    }

    sNext_tcb = &sTask_list[sCurrent_tcb_index];

    return sNext_tcb;
}

KernelTcb_t *Scheduler_periority_algorithm(void)
{
    for (uint32_t i = 0; i < sAllocated_tcb_index; i++)
    {
        KernelTcb_t *pNextTcb = &sTask_list[i];
        if (pNextTcb != sCurrent_tcb && pNextTcb->priority < sCurrent_tcb->priority)
        {
            return pNextTcb;
        }
    }

    return sCurrent_tcb;
}