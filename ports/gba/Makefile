# Using DEVKITARM
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro)
endif
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif
export PATH := $(DEVKITPRO)/portlibs/gba/bin:$(DEVKITPRO)/portlibs/armv4t/bin:$(PATH)
PORTLIBS	:= $(DEVKITPRO)/portlibs/gba $(DEVKITPRO)/portlibs/armv4t
CROSS_COMPILE = $(DEVKITARM)/bin/arm-none-eabi-

TARGET		:= micropython
BUILD		:= build
GAME_TITLE  := $(TARGET)
GAME_CODE	:= 0000
MAKER_CODE	:= 00
MGBA_EMU	:= mgba-qt

ifeq ($(strip $(GAME_TITLE)),)
GAME_TITLE	:=	$(notdir $(OUTPUT))
endif

ifeq ($(strip $(GAME_CODE)),)
GAME_CODE	:=	0000
endif

ifeq ($(strip $(MAKER_CODE)),)
MAKER_CODE	:=	00
endif

# Include the core environment definitions; this will set $(TOP).
include ../../py/mkenv.mk
# fix some path
include $(DEVKITARM)/base_tools

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# MicroPython feature configurations
MICROPY_PREVIEW_VERSION_2 = 1
MICROPY_ROM_TEXT_COMPRESSION ?= 0
FROZEN_MANIFEST = frozen_manifest.py
MICROPY_VFS_LFS2 := 1

# include py core make definitions
include $(TOP)/py/py.mk
include $(TOP)/extmod/extmod.mk

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
# MPY_CROSS_FLAGS = -march=armv6m

CFLAGS	+=	-g -Wall -O2 \
		-mcpu=arm7tdmi -mtune=arm7tdmi \
		-mthumb -mthumb-interwork
CFLAGS	+=	-std=gnu99

LDFLAGS	+=	-specs=gba.specs -g -mthumb -mthumb-interwork -Wl,-Map,$(BUILD)/$(TARGET).map

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:= -lm -lgba
LDFLAGS += -L$(DEVKITPRO)/libgba/lib


#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
# LIBDIRS	:=	$(LIBGBA)

# include
INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(DEVKITPRO)/libgba/include
CFLAGS	+=	$(INC)
LDFLAGS += -L$(DEVKITPRO)/libgba/lib

# source file
SRC_C += \
	main.c \
	mgba.c \
	mphalport.c \
	shared/readline/readline.c \
	shared/runtime/gchelper_generic.c \
	shared/runtime/stdout_helpers.c \
	shared/runtime/pyexec.c

SRC_QSTR += shared/readline/readline.c shared/runtime/pyexec.c

# Define the required object files.
OBJ += $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

# Define the top-level target, the main firmware.
all: $(BUILD)/$(TARGET).gba

run: $(BUILD)/$(TARGET).gba
	$(MGBA_EMU) -l 255 $(BUILD)/$(TARGET).gba

%.gba: %.elf
	$(Q)$(OBJCOPY) -O binary $< $@
	@echo built ... $(notdir $@)
	$(Q)gbafix $@ -t$(GAME_TITLE) -c$(GAME_CODE) -m$(MAKER_CODE)

# Define how to build the firmware.
$(BUILD)/$(TARGET).elf: $(OBJ)
	$(ECHO)
	$(ECHO) "LINK $@"
	$(Q)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

$(BUILD)/_frozen_mpy.c: $(TOP)/tests/frozen/frozentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
	$(ECHO) "MISC freezing bytecode"
	$(Q)$(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@

# Include remaining core make rules.
include $(TOP)/py/mkrules.mk
