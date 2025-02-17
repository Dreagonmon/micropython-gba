#include <stdint.h>

// Python internal features.
#define MICROPY_CONFIG_ROM_LEVEL                (MICROPY_CONFIG_ROM_LEVEL_CORE_FEATURES)
#define MICROPY_ERROR_REPORTING                 (MICROPY_ERROR_REPORTING_DETAILED)
#define MICROPY_ENABLE_FINALISER                (1)
#define MICROPY_ENABLE_GC                       (1)
#define MICROPY_HELPER_REPL                     (1)
#define MICROPY_LONGINT_IMPL                    (MICROPY_LONGINT_IMPL_MPZ)
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_FLOAT)
#define MICROPY_ALLOC_PATH_MAX                  (256)
#define MICROPY_WARNINGS                        (1)
#define MICROPY_EMIT_THUMB                      (1)
#define MICROPY_EMIT_INLINE_THUMB               (1)

// Fine control over Python builtins, classes, modules, etc.
#define MICROPY_PY_BUILTINS_HELP                (1)
#define MICROPY_PY_ASYNC_AWAIT                  (1)
#define MICROPY_PY_BUILTINS_SET                 (1)
#define MICROPY_PY_ATTRTUPLE                    (1)
#define MICROPY_PY_COLLECTIONS                  (1)
#define MICROPY_PY_MATH                         (1)
#define MICROPY_PY_IO                           (1)
#define MICROPY_PY_STRUCT                       (1)
#define MICROPY_PY_OS                           (1)
#define MICROPY_VFS                             (1)
#define MICROPY_PY_MACHINE                      (1)
#define MICROPY_PY_MACHINE_MEMX                 (1)

// Type definitions for the specific machine.
#define MP_SSIZE_MAX (0x7fffffff)
typedef intptr_t mp_int_t; // must be pointer size
typedef uintptr_t mp_uint_t; // must be pointer size
typedef long mp_off_t;

// We need to provide a declaration/definition of alloca().
#include <alloca.h>
#include <math.h>

// Define the port's name and hardware.
#define MICROPY_HW_BOARD_NAME "Gameboy Advance"
#define MICROPY_HW_MCU_NAME   "arm7tdmi"
#define MICROPY_HEAP_SIZE      (20480) // unit: bytes

#define MP_STATE_PORT MP_STATE_VM