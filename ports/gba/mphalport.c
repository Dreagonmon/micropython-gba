#include <unistd.h>
#include "py/builtin.h"
#include "py/compile.h"
#include "py/mperrno.h"
#include "py/mpconfig.h"
#include "py/obj.h"
#include <gba_interrupt.h>
#include "mgba.h"
#include "hal_time.h"
#include "hal_rom.h"

#define MICROPY_BEGIN_ATOMIC_SECTION() (0)

#define MAX_PRINT_LENGTH (80)
static char pbuffer[MAX_PRINT_LENGTH + 1] = {0};
static char *pnow = ((char *) pbuffer);
static char * const ptop = ((char *) pbuffer) + MAX_PRINT_LENGTH;

void gba_init(void) {
    mgba_console_open();
    // init rom
    init_rom();
    // init irq for all
    irqInit(); 
    REG_IME = 1;
    // timer
    init_hal_time();
    // screen
    irqEnable(IRQ_VBLANK);
    // keypad
    irqEnable(IRQ_KEYPAD);
    // DMA
    irqEnable(IRQ_DMA0);
    irqEnable(IRQ_DMA1);
    irqEnable(IRQ_DMA2);
    irqEnable(IRQ_DMA3);
}

// no console, doing nothing
void mp_hal_set_interrupt_char(char c) {
}

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

// Handle uncaught exceptions (should never be reached in a correct C implementation).
void nlr_jump_fail(void *val) {
    if (mgba_console_open()) {
        mgba_printf(MGBA_LOG_ERROR, "NLR jump failed, val=%p\n", val);
    }
    while (1);
}
