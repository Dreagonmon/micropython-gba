#include "py/mpconfig.h"
#include "py/obj.h"
#include <gba_base.h>
#include <gba_interrupt.h>
#include <gba_timers.h>
#include <gba_systemcalls.h>

// 16777216 ticks / sec
// 1024 x 16384 ticks / sec
// overflow every 1 sec
#define TIMER_FREQ  16777216
#define TICKS_PER_SEC_D1024 (TIMER_FREQ / 1024)
#define TICKS_PER_MS_D1 (TIMER_FREQ / 1000)
#define INIT_SYS_TICK TICKS_PER_SEC_D1024
#define TM_FREQ_1 0
#define TM_FREQ_1024 3
#define REG_IFBIOS *((vu16 *) 0x03007FF8)

static uint32_t sec_passed = 0;
static uint32_t t1_count = 0;
static uint32_t t1_target = 0;

IWRAM_CODE ARM_CODE static void Timer0Interrupt(void) {
    sec_passed += 1;
}

IWRAM_CODE ARM_CODE static void Timer1Interrupt(void) {
    t1_count += 1;
}

// init
void init_hal_time(void) {
    irqSet(IRQ_TIMER0, Timer0Interrupt);
    irqSet(IRQ_TIMER1, Timer1Interrupt);
    irqEnable(IRQ_TIMER0);
    // configure timer
    REG_TM0CNT_L = -INIT_SYS_TICK;          // INIT_SYS_TICK ticks till overflow
    unsigned int tm_cnt = TM_FREQ_1024;
    // tm_cnt |= TIMER_COUNT;           // Cascade mode // off
    tm_cnt |= TIMER_IRQ;             // Raise an interrupt on overflow
    tm_cnt |= TIMER_START;           // Enable the timer
    REG_TM0CNT_H = tm_cnt;
}

// from py/mphal.h
void mp_hal_delay_ms(mp_uint_t ms) {
    if (ms == 0) return;
    // init timer for count
    t1_count = 0;
    t1_target = ms;
    irqEnable(IRQ_TIMER1);
    REG_TM1CNT_L = -TICKS_PER_MS_D1; // count every ms
    REG_TM1CNT_H = TM_FREQ_1 | TIMER_IRQ | TIMER_START;
    while (t1_count < t1_target) {
        IntrWait(0, IRQ_TIMER1);
    }
    REG_TM1CNT_H = 0; // disable timer 1
    irqDisable(IRQ_TIMER1);
}

void mp_hal_delay_us(mp_uint_t us) {
    // there is no way to tick correctly...
    mp_hal_delay_ms(us / 1000);
}

uint64_t mp_hal_time_ns(void) {
    uint32_t last = sec_passed;
    uint16_t ticks = REG_TM0CNT_L - ((uint16_t) (-INIT_SYS_TICK)); // ticks since sec
    if (last != sec_passed) {
        // sec changed during the last read, just retry
        return mp_hal_time_ns();
    }
    // calc ns
    uint64_t ns = ((uint64_t)last) * 1000000000; // sec passed
    ns += (((uint64_t)ticks) * 1000000000 / TICKS_PER_SEC_D1024);
    return ns;
}

mp_uint_t mp_hal_ticks_ms(void) {
    return (mp_uint_t)(mp_hal_time_ns() / 1000000);
}

mp_uint_t mp_hal_ticks_us(void) {
    return (mp_uint_t)(mp_hal_time_ns() / 1000);
}

mp_uint_t mp_hal_ticks_cpu(void) {
    return (mp_uint_t)(mp_hal_time_ns() & UINT32_MAX);
}

static mp_obj_t mp_time_time_get(void) {
    return mp_obj_new_int(sec_passed & INT32_MAX);
}