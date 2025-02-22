#include "py/mpconfig.h"
#include "py/runtime.h"
#include <gba_systemcalls.h>

static mp_obj_t gba_bios_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    return mp_const_none;
}

static mp_obj_t gba_bios_vblank_intr_wait() {
    VBlankIntrWait();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(gba_bios_vblank_intr_wait_fun_obj, gba_bios_vblank_intr_wait);
static MP_DEFINE_CONST_STATICMETHOD_OBJ(gba_bios_vblank_intr_wait_obj, MP_ROM_PTR(&gba_bios_vblank_intr_wait_fun_obj));

static mp_obj_t gba_bios_register_ram_reset(mp_obj_t reset_flag_in) {
    mp_int_t reset_flag = mp_obj_get_int(reset_flag_in);
    RegisterRamReset(reset_flag);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(gba_bios_register_ram_reset_fun_obj, gba_bios_register_ram_reset);
static MP_DEFINE_CONST_STATICMETHOD_OBJ(gba_bios_register_ram_reset_obj, MP_ROM_PTR(&gba_bios_register_ram_reset_fun_obj));

/*  Transfer via CPU in (half)word chunks.
The default mode is 16bit copies. With bit 24 set, it copies words; with bit 26 set it will keep the source address constant, effectively performing fills instead of copies. 
In fill-mode (bit 26), the source is still an address, not a value. 
*/
static mp_obj_t gba_bios_cpu_set(mp_obj_t source_in, mp_obj_t dest_in, mp_obj_t mode_in) {
    mp_uint_t source = mp_obj_int_get_uint_checked(source_in);
    mp_uint_t dest = mp_obj_int_get_uint_checked(dest_in);
    mp_uint_t mode = mp_obj_int_get_uint_checked(mode_in);
    CpuSet((void *) source, (void *) dest, mode);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_3(gba_bios_cpu_set_fun_obj, gba_bios_cpu_set);
static MP_DEFINE_CONST_STATICMETHOD_OBJ(gba_bios_cpu_set_obj, MP_ROM_PTR(&gba_bios_cpu_set_fun_obj));

/* A fast transfer via CPU in 32 byte chunks.
This uses ARM's ldmia/stmia instructions to copy 8 words at a time, making it rival DMA transfers in speed.
With bit 26 set it will keep the source address constant, effectively performing fills instead of copies.
Both source and destination must be word aligned; the number of copies must be a multiple of 8. 
In fill-mode (bit 26), the source is still an address, not a value. 
*/
static mp_obj_t gba_bios_cpu_set_fast(mp_obj_t source_in, mp_obj_t dest_in, mp_obj_t mode_in) {
    mp_uint_t source = mp_obj_int_get_uint_checked(source_in);
    mp_uint_t dest = mp_obj_int_get_uint_checked(dest_in);
    mp_uint_t mode = mp_obj_int_get_uint_checked(mode_in);
    CpuSet((void *) source, (void *) dest, mode);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_3(gba_bios_cpu_set_fast_fun_obj, gba_bios_cpu_set_fast);
static MP_DEFINE_CONST_STATICMETHOD_OBJ(gba_bios_cpu_set_fast_obj, MP_ROM_PTR(&gba_bios_cpu_set_fast_fun_obj));

static mp_obj_t gba_bios_intr_wait(mp_obj_t return_flag_in, mp_obj_t int_flag_in) {
    mp_uint_t return_flag = mp_obj_int_get_uint_checked(return_flag_in);
    mp_uint_t int_flag = mp_obj_int_get_uint_checked(int_flag_in);
    IntrWait(return_flag, int_flag);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(gba_bios_intr_wait_fun_obj, gba_bios_intr_wait);
static MP_DEFINE_CONST_STATICMETHOD_OBJ(gba_bios_intr_wait_obj, MP_ROM_PTR(&gba_bios_intr_wait_fun_obj));

static const mp_rom_map_elem_t gba_bios_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_vblank_intr_wait), MP_ROM_PTR(&gba_bios_vblank_intr_wait_obj) },
    { MP_ROM_QSTR(MP_QSTR_register_ram_reset), MP_ROM_PTR(&gba_bios_register_ram_reset_obj) },
    { MP_ROM_QSTR(MP_QSTR_cpu_set), MP_ROM_PTR(&gba_bios_cpu_set_obj) },
    { MP_ROM_QSTR(MP_QSTR_cpu_set_fast), MP_ROM_PTR(&gba_bios_cpu_set_fast_obj) },
    { MP_ROM_QSTR(MP_QSTR_intr_wait), MP_ROM_PTR(&gba_bios_intr_wait_obj) },
    
    { MP_ROM_QSTR(MP_QSTR_RESET_EWRAM), MP_ROM_INT(RESET_EWRAM) },
    { MP_ROM_QSTR(MP_QSTR_RESET_IWRAM), MP_ROM_INT(RESET_IWRAM) },
    { MP_ROM_QSTR(MP_QSTR_RESET_PALETTE), MP_ROM_INT(RESET_PALETTE) },
    { MP_ROM_QSTR(MP_QSTR_RESET_VRAM), MP_ROM_INT(RESET_VRAM) },
    { MP_ROM_QSTR(MP_QSTR_RESET_OAM), MP_ROM_INT(RESET_OAM) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SIO), MP_ROM_INT(RESET_SIO) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOUND), MP_ROM_INT(RESET_SOUND) },
    { MP_ROM_QSTR(MP_QSTR_RESET_OTHER), MP_ROM_INT(RESET_OTHER) },
};
static MP_DEFINE_CONST_DICT(gba_bios_locals_dict, gba_bios_locals_dict_table);

static MP_DEFINE_CONST_OBJ_TYPE(
    gba_bios_type,
    MP_QSTR_BIOS,
    MP_TYPE_FLAG_NONE,
    make_new, gba_bios_make_new,
    locals_dict, &gba_bios_locals_dict
);
