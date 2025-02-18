#include "py/mpconfig.h"
#include <gba_systemcalls.h>

#ifdef MICROPY_PY_MACHINE
static void mp_machine_idle(void) {
    VBlankIntrWait();
}

#if MICROPY_PY_MACHINE_RESET
NORETURN static void mp_machine_reset(void) {
    SoftReset(ROM_RESTART);
    while (1);
}
static mp_int_t mp_machine_reset_cause(void) {
    return 0;
}
#endif

#endif