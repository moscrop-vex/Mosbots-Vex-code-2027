# VEXcode makefile 2019_03_26_01

# Ensure the VEX ARM toolchain is on PATH even when make is run from a plain shell
VEX_TOOLCHAIN_BIN := $(HOME)/.config/Code/User/globalStorage/vexrobotics.vexcode/tools/cpp/toolchain_linux64/gcc/bin
ifneq ($(wildcard $(VEX_TOOLCHAIN_BIN)/arm-none-eabi-ld),)
export PATH := $(VEX_TOOLCHAIN_BIN):$(PATH)
endif

# show compiler output
VERBOSE = 0

# include toolchain options
include vex/mkenv.mk

# location of the project source cpp and c files
SRC_C  = $(wildcard src/*.cpp) 
SRC_C += $(wildcard src/*.c)
SRC_C += $(wildcard src/*/*.cpp) 
SRC_C += $(wildcard src/*/*.c)

OBJ = $(addprefix $(BUILD)/, $(addsuffix .o, $(basename $(SRC_C))) )

# location of include files that c and cpp files depend on
SRC_H  = $(wildcard include/*.h)

# additional dependancies
SRC_A  = makefile

# project header file locations
INC_F  = include

# build targets
all: $(BUILD)/$(PROJECT).bin

# include build rules
include vex/mkrules.mk
