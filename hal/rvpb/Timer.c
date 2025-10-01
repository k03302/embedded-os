#include <stdint.h>
#include "Timer.h"
#include "HalTimer.h"
#include "HalInterrupt.h"

extern volatile Timer_t *Timer;

static void interrupt_handler(void);

static uint32_t internal_1ms_counter;

void Hal_timer_init(void)
{
    // unable timer before setting
    Timer->timerxcontrol.bits.TimerEn = 0;

    // free-running mode
    Timer->timerxcontrol.bits.TimerMode = TIMER_PERIOIC;
    Timer->timerxcontrol.bits.OneShot = 0;

    // 32bits size timer
    Timer->timerxcontrol.bits.TimerSize = TIMER_32BIT_COUNTER;

    // set prescalar 1
    Timer->timerxcontrol.bits.TimerPre = 0;

    // enable interrupt
    Timer->timerxcontrol.bits.IntEnable = 1;

    uint32_t internal_1ms = TIMER_10HZ_INTERVAL / 1000;
    internal_1ms_counter = 0;

    // set timer
    Timer->timerxload = internal_1ms;

    // setting finished. enable timer
    Timer->timerxcontrol.bits.TimerEn = 1;

    Hal_interrupt_enable(TIMER_INTERRUPT);
    Hal_interrupt_register_handler(interrupt_handler, TIMER_INTERRUPT);
}

static void interrupt_handler(void)
{
    internal_1ms_counter++;

    Timer->timerxintclr = 1;
}