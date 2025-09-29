#include "MemoryMap.h"
#include "ARMv7AR.h"

.text
    .code 32

    .global vector_start
    .global vector_end

    vector_start:
    @ exception vector table
        LDR PC, reset_handler_addr
        LDR PC, undef_handler_addr
        LDR PC, svc_handler_addr
        LDR PC, pftch_abt_handler_addr
        LDR PC, data_abt_handler_addr
    @ infinite loop
        B .
        LDR PC, irq_handler_addr
        LDR PC, fiq_handler_addr

    reset_handler_addr: .word reset_handler
    undef_handler_addr: .word dummy_handler
    svc_handler_addr: .word dummy_handler
    pftch_abt_handler_addr: .word dummy_handler
    data_abt_handler_addr: .word dummy_handler
    irq_handler_addr: .word dummy_handler
    fiq_handler_addr: .word dummy_handler


    @ initialize stack pointer to top
    reset_handler:
        MRS R0, CPSR
        BIC R1, R0, #0x1F
        ORR R1, R1, #ARM_MODE_BIT_SVC
        MSR CPSR, R1
        LDR SP, =SVC_STACK_TOP

        MRS R0, CPSR
        BIC R1, R0, #0x1F
        ORR R1, R1, #ARM_MODE_BIT_IRQ
        MSR CPSR, R1
        LDR SP, =IRQ_STACK_TOP

        MRS R0, CPSR
        BIC R1, R0, #0x1F
        ORR R1, R1, #ARM_MODE_BIT_FIQ
        MSR CPSR, R1
        LDR SP, =FIQ_STACK_TOP

        MRS R0, CPSR
        BIC R1, R0, #0x1F
        ORR R1, R1, #ARM_MODE_BIT_ABT
        MSR CPSR, R1
        LDR SP, =ABT_STACK_TOP

        MRS R0, CPSR
        BIC R1, R0, #0x1F
        ORR R1, R1, #ARM_MODE_BIT_UND
        MSR CPSR, R1
        LDR SP, =UND_STACK_TOP
        
        MRS R0, CPSR
        BIC R1, R0, #0x1F
        ORR R1, R1, #ARM_MODE_BIT_SYS
        MSR CPSR, R1
        LDR SP, =USRSYS_STACK_START

        BL main

    dummy_handler:
        B .


.end