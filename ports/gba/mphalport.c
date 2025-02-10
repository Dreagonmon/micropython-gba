#include <unistd.h>
#include "py/mpconfig.h"
#include "mgba.h"

// Send the string of given length.
mp_uint_t mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    // int r = write(STDOUT_FILENO, str, len);
    // (void)r;
    if (mgba_console_open()) {
        mgba_printf(MGBA_LOG_INFO, str);
    }
    return len;
}