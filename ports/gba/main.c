#include "py/builtin.h"
#include "py/compile.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "py/stackctrl.h"
#include "shared/runtime/gchelper.h"
#include "shared/runtime/pyexec.h"

// Allocate memory for the MicroPython GC heap.
static char heap[MICROPY_HEAP_SIZE];

int main(int argc, char **argv) {
    // Initialise the MicroPython runtime.
    // mp_stack_ctrl_init(); // not in version 2
    gc_init(heap, heap + sizeof(heap));
    mp_init();

    // Start with frozen module _init.py.
    #if MICROPY_VFS || MICROPY_MBFS || MICROPY_MODULE_FROZEN
    pyexec_file_if_exists("_init.py");
    #endif

    // Deinitialise the runtime.
    gc_sweep_all();
    mp_deinit();
    while (1) ;  // stop here
    return 0;
}

// Do a garbage collection cycle.
void gc_collect(void) {
    gc_collect_start();
    gc_helper_collect_regs_and_stack();
    gc_collect_end();
}
