#include <unistd.h>
#include "py/builtin.h"
#include "py/compile.h"
#include "py/mperrno.h"
#include "py/mpconfig.h"
#include "py/obj.h"
#include "mgba.h"

#define MAX_PRINT_LENGTH (80)
static char pbuffer[MAX_PRINT_LENGTH + 1] = {0};
static char *pnow = ((char *) pbuffer);
static char * const ptop = ((char *) pbuffer) + MAX_PRINT_LENGTH;

// Read nothing
int mp_hal_stdin_rx_chr(void) {
    while (1) ;
    return 0;
}

// Send the string of given length.
mp_uint_t mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    while (len > 0) {
        char ch = *str;
        str ++;
        len --;
        if (ch != '\n') {
            *pnow = ch;
            pnow ++;
        }
        if (pnow >= ptop || ch == '\n') {
            // buffer is full, or new line, print out
            *pnow = '\0';
            if (mgba_console_open()) {
                mgba_printf(MGBA_LOG_INFO, pbuffer);
            }
            pnow = ((char *) pbuffer);
        }
    }
    return len;
}

#ifndef MICROPY_VFS
// Support IO
mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

// There is no filesystem so stat'ing returns nothing.
mp_import_stat_t mp_import_stat(const char *path) {
    return MP_IMPORT_STAT_NO_EXIST;
}
#endif

// There is no filesystem so opening a file raises an exception.
mp_lexer_t *mp_lexer_new_from_file(qstr filename) {
    mp_raise_OSError(MP_ENOENT);
}

// from py/mphal.h
uint64_t mp_hal_time_ns(void) {
    return 0;
}