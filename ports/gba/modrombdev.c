#include <string.h>
#include "py/runtime.h"
#include "py/mperrno.h"
#include "py/mphal.h"
#include "extmod/vfs.h"

#define MAGIC_BLOCK_SIZE              (0x67452301)
#define MAGIC_BLOCK_COUNT             (0xEFCDAB89)

extern const mp_obj_type_t gba_rombdev_type;

typedef struct _gba_rombdev_block {
    const char romtag[8];
    uint32_t block_size;
    uint32_t block_count;
    uint32_t dev_size;
    const uint8_t rom[GBA_ROMBDEV_SIZE];
    const char romtagend[8];
} gba_rombdev_block;

typedef struct _gba_rombdev_obj_t {
    mp_obj_base_t base;
    const gba_rombdev_block * bdev;
} gba_rombdev_obj_t;

// aligned to 8, make it easier to search.
__attribute__((aligned(8))) static const gba_rombdev_block gba_rombdev = {
    .romtag = "GBABDEV", // \0
    .block_size = MAGIC_BLOCK_SIZE,
    .block_count = MAGIC_BLOCK_COUNT,
    .dev_size = GBA_ROMBDEV_SIZE,
    .romtagend = "BDEVGBA" // \0
};

static gba_rombdev_obj_t *gba_rombdev_new() {
    gba_rombdev_obj_t *self = mp_obj_malloc(gba_rombdev_obj_t, &gba_rombdev_type);
    self->bdev = &gba_rombdev;
    return self;
}

static mp_obj_t gba_rombdev_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    // Check args
    mp_arg_check_num(n_args, n_kw, 0, 0, false);
    // Return new object
    return MP_OBJ_FROM_PTR(gba_rombdev_new());
}

__attribute__((optimize("O0"))) static mp_obj_t gba_rombdev_readblocks(size_t n_args, const mp_obj_t *args) {
    gba_rombdev_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    uint32_t offset = mp_obj_get_int(args[1]) * self->bdev->block_size;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[2], &bufinfo, MP_BUFFER_WRITE);
    if (n_args == 4) {
        offset += mp_obj_get_int(args[3]);
    }
    if (offset + bufinfo.len > GBA_ROMBDEV_SIZE) {
        mp_raise_OSError(MP_ENODEV);
        return mp_const_none;
    }
    memcpy(bufinfo.buf, self->bdev->rom + offset, bufinfo.len);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(gba_rombdev_readblocks_fun_obj, 3, 4, gba_rombdev_readblocks);

__attribute__((optimize("O0"))) static mp_obj_t gba_rombdev_ioctl(mp_obj_t self_in, mp_obj_t cmd_in, mp_obj_t arg_in) {
    gba_rombdev_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_int_t cmd = mp_obj_get_int(cmd_in);
    switch (cmd) {
        case MP_BLOCKDEV_IOCTL_INIT:
            return MP_OBJ_NEW_SMALL_INT(0);
        case MP_BLOCKDEV_IOCTL_DEINIT:
            return MP_OBJ_NEW_SMALL_INT(0);
        case MP_BLOCKDEV_IOCTL_SYNC:
            return MP_OBJ_NEW_SMALL_INT(0);
        case MP_BLOCKDEV_IOCTL_BLOCK_COUNT:
            if (self->bdev->block_size == MAGIC_BLOCK_SIZE) {
                return MP_OBJ_NEW_SMALL_INT(0);
            }
            return MP_OBJ_NEW_SMALL_INT(self->bdev->block_count);
        case MP_BLOCKDEV_IOCTL_BLOCK_SIZE:
            if (self->bdev->block_count == MAGIC_BLOCK_COUNT) {
                return MP_OBJ_NEW_SMALL_INT(0);
            }
            return MP_OBJ_NEW_SMALL_INT(self->bdev->block_size);
        case MP_BLOCKDEV_IOCTL_BLOCK_ERASE: {
            // raise error, can't write to ROM
            return MP_OBJ_NEW_SMALL_INT(-MP_EACCES);
        }
        default:
            return mp_const_none;
    }
}
static MP_DEFINE_CONST_FUN_OBJ_3(gba_rombdev_ioctl_fun_obj, gba_rombdev_ioctl);

static mp_obj_t gba_rombdev_writeblocks(size_t n_args, const mp_obj_t *args) {
    // raise error, can't write to ROM
    mp_raise_OSError(MP_EACCES);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(gba_rombdev_writeblocks_fun_obj, 3, 4, gba_rombdev_writeblocks);

static const mp_rom_map_elem_t gba_rombdev_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_readblocks), MP_ROM_PTR(&gba_rombdev_readblocks_fun_obj) },
    { MP_ROM_QSTR(MP_QSTR_writeblocks), MP_ROM_PTR(&gba_rombdev_writeblocks_fun_obj) },
    { MP_ROM_QSTR(MP_QSTR_ioctl), MP_ROM_PTR(&gba_rombdev_ioctl_fun_obj) },
};
static MP_DEFINE_CONST_DICT(gba_rombdev_locals_dict, gba_rombdev_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    gba_rombdev_type,
    MP_QSTR_RomBlockDevice,
    MP_TYPE_FLAG_NONE,
    make_new, gba_rombdev_make_new,
    locals_dict, &gba_rombdev_locals_dict
);

static const mp_rom_map_elem_t rombdev_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_rombdev) },
    { MP_ROM_QSTR(MP_QSTR_RomBlockDevice), MP_ROM_PTR(&gba_rombdev_type) },
};

static MP_DEFINE_CONST_DICT(rombdev_module_globals, rombdev_module_globals_table);

const mp_obj_module_t rombdev_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *) &rombdev_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR__rombdev, rombdev_module);

void init_rom(void) {
    //
}
