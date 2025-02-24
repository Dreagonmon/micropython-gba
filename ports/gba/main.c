#include "py/builtin.h"
#include "py/compile.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "py/stackctrl.h"
#include "shared/runtime/gchelper.h"
#include "shared/runtime/pyexec.h"
#include "mphalport.h" // gba_init
#include <gba_base.h>

// Allocate memory for the MicroPython GC heap.
#if MICROPY_HEAP_IN_IRAM
IWRAM_DATA static char heap[MICROPY_HEAP_SIZE];
#else
EWRAM_BSS static char heap[MICROPY_HEAP_SIZE];
#endif

#if MICROPY_ENABLE_PYSTACK
#define STACK_ARRAY_SIZE (MICROPY_PYSTACK_SIZE / sizeof(mp_obj_t))
IWRAM_DATA static mp_obj_t stack[STACK_ARRAY_SIZE];
#endif

int main(int argc, char **argv) {
    gba_init();
    // C stack in the GBA's IWRAM, maybe start near the 0x03007FFF, count down.
    mp_cstack_init_with_sp_here(GBA_CSTACK_SIZE);
    // Initialise the MicroPython runtime.
    #if MICROPY_ENABLE_PYSTACK
    mp_pystack_init(stack, &stack[STACK_ARRAY_SIZE]);
    #endif
    gc_init(heap, heap + sizeof(heap));
    mp_init();

    // Start with frozen module _init.py.
    #if MICROPY_VFS || MICROPY_MBFS || MICROPY_MODULE_FROZEN
    pyexec_file_if_exists("_init.py");
    #endif
    #if MICROPY_VFS
    pyexec_file_if_exists("boot.py");
    pyexec_file_if_exists("main.py");
    #endif

    // Deinitialise the runtime.
    gc_sweep_all();
    mp_deinit();

    if (mgba_console_open()) {
        mgba_printf(MGBA_LOG_WARN, "[Micropython Code End]");
    }
    while (1) ;  // stop here
    return 0;
}

// Do a garbage collection cycle.
void gc_collect(void) {
    gc_collect_start();
    gc_helper_collect_regs_and_stack();
    gc_collect_end();
}
