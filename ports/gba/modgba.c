#include "py/mpconfig.h"
#include "py/runtime.h"
#include "modgba_bios.c"
#include <gba_base.h>

static const mp_rom_map_elem_t gba_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_gba) },
    { MP_ROM_QSTR(MP_QSTR_BIOS), MP_ROM_PTR(&gba_bios_type) },

    // { MP_ROM_QSTR(MP_QSTR_DMA_MODES_FILL), MP_ROM_INT(FILL) },
    // { MP_ROM_QSTR(MP_QSTR_DMA_MODES_COPY16), MP_ROM_INT(COPY16) },
    // { MP_ROM_QSTR(MP_QSTR_DMA_MODES_COPY32), MP_ROM_INT(COPY32) },
};

static MP_DEFINE_CONST_DICT(gba_module_globals, gba_module_globals_table);

const mp_obj_module_t gba_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *) &gba_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_gba, gba_module);