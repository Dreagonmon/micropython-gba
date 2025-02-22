#include <stdint.h>

// Python internal features.
#ifndef MICROPY_PREVIEW_VERSION_2
#define MICROPY_PREVIEW_VERSION_2               (1)
#endif
#define MICROPY_OBJ_BASE_ALIGNMENT              __attribute__((aligned(4)))
#define MICROPY_CONFIG_ROM_LEVEL                (MICROPY_CONFIG_ROM_LEVEL_FULL_FEATURES)
#define MICROPY_ERROR_REPORTING                 (MICROPY_ERROR_REPORTING_DETAILED)
#define MICROPY_LONGINT_IMPL                    (MICROPY_LONGINT_IMPL_MPZ)
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_FLOAT)
#define MICROPY_ENABLE_GC                       (1)
#define MICROPY_PERSISTENT_CODE_LOAD            (1)
#define MICROPY_HELPER_REPL                     (1)
#define MICROPY_ALLOC_PATH_MAX                  (256)
#define MICROPY_WARNINGS                        (1)
#define MICROPY_ENABLE_EMERGENCY_EXCEPTION_BUF  (1)
#define MICROPY_EMERGENCY_EXCEPTION_BUF_SIZE    (256)
#define MICROPY_STACK_CHECK                     (1)
#define MICROPY_ENABLE_PYSTACK                  (1)
#define MICROPY_PYSTACK_ALIGN                   (4)

// Buggy features
// #define MICROPY_NLR_THUMB_USE_LONG_JUMP         (1)
// #define MICROPY_GCREGS_SETJMP                   (1)
// #define MICROPY_EMIT_THUMB                      (1)
// #define MICROPY_EMIT_INLINE_THUMB               (1)
// #define MICROPY_DEBUG_VERBOSE                   (1)
// #define MICROPY_DEBUG_PRINTERS                  (1)

// Fine control over Python builtins, classes, modules, etc.
#define MICROPY_PY_SYS_STDFILES                 (1)
#define MICROPY_PY_BUILTINS_HELP                (1)
#define MICROPY_PY_BUILTINS_SET                 (1)
#define MICROPY_PY_ATTRTUPLE                    (1)
#define MICROPY_PY_COLLECTIONS                  (1)
#define MICROPY_PY_MATH                         (1)
#define MICROPY_PY_IO                           (1)
#define MICROPY_PY_STRUCT                       (1)
#define MICROPY_PY_OS                           (1)
#define MICROPY_PY_OS_URANDOM                   (1)
#define MICROPY_PY_OS_INCLUDEFILE               "ports/gba/modos.c"
#define MICROPY_VFS                             (1)
#define MICROPY_READER_VFS                      (1)
#define MICROPY_PY_TIME                         (1)
#define MICROPY_PY_TIME_TIME_TIME_NS            (1)
#define MICROPY_PY_TIME_INCLUDEFILE             "ports/gba/modtime.c"
#define MICROPY_PY_MACHINE                      (1)
#define MICROPY_PY_MACHINE_MEMX                 (1)
#define MICROPY_PY_MACHINE_RESET                (1)
#define MICROPY_PY_MACHINE_INCLUDEFILE          "ports/gba/modmachine.c"

// Type definitions for the specific machine.
#define MP_SSIZE_MAX (0x7fffffff)
typedef intptr_t mp_int_t; // must be pointer size
typedef uintptr_t mp_uint_t; // must be pointer size
typedef long mp_off_t;

// We need to provide a declaration/definition of alloca().
#include <alloca.h>
// to use mgba_print everywhere
#include "mgba.h"

// Define the port's name and hardware.
#define MICROPY_HW_BOARD_NAME "Gameboy Advance"
#define MICROPY_HW_MCU_NAME   "arm7tdmi"

// py heap are in the EWRAM, the limit is 256k
#define MICROPY_HEAP_SIZE        (240 * 1024)       // unit: bytes
// the py stack and C stack is in the IWRAM, the limit is 32k
#if MICROPY_ENABLE_PYSTACK
#define MICROPY_PYSTACK_SIZE     (16 * 1024)        // unit: bytes
#endif
#define GBA_CSTACK_SIZE          (8 * 1024)         // unit: bytes
// the ROMBDEV is in the ROM, the limit is 32m
#define GBA_ROMBDEV_SIZE         (30 * 1024 * 1024) // unit: bytes

#define MP_STATE_PORT MP_STATE_VM