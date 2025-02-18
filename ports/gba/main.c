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
EWRAM_BSS static char heap[MICROPY_HEAP_SIZE];
#if MICROPY_ENABLE_PYSTACK
EWRAM_BSS static char stack[MICROPY_HEAP_SIZE];
#endif

int main(int argc, char **argv) {
    gba_init();
    // Initialise the MicroPython runtime.
    // mp_stack_ctrl_init(); // not in version 2
    #if MICROPY_ENABLE_PYSTACK
    mp_pystack_init(stack, stack + sizeof(stack));
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
