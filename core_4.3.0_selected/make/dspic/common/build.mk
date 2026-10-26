################################################################################
#                                                                              #
#                         |\    /|   /- - -  |\    /|                          #
#                         | \  / |  |        | \  / |                          #
#                         |  \/  |  |        |  \/  |                          #
#                         |      |   \_ _ _  |      |                          #
#                                                                              #
#                                                                              #
#         T H E   S M A R T   M E A S U R E M E N T   S O L U T I O N          #
#                                                                              #
################################################################################
#             (c) SMART TESTSOLUTIONS GmbH - all rights reserved               #
################################################################################

SHELL := /bin/bash

DETECTED_OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
ifneq ($(DETECTED_OS),Linux)
$(error wrong operating system)
endif

ifeq ($(MAKE_COMPILER_NAME),)
$(error no compiler selected)
endif

ifeq ($(MAKE_COMPILER_VERSION),)
$(error no compiler selected)
endif

ifeq ($(MAKE_DEVICE_FAMILY_PACK_NAME),)
$(error no device family pack selected)
endif

ifeq ($(MAKE_DEVICE_FAMILY_PACK_VERSION),)
$(error no device family pack selected)
endif

ifeq ($(MAKE_CONFIGURATION),)
$(error no make configuration selected)
endif

COMPILER_FOLDER := /opt/microchip/${MAKE_COMPILER_NAME}/v${MAKE_COMPILER_VERSION}
ifeq ("$(wildcard $(COMPILER_FOLDER))","")
$(error compiler "$(COMPILER_FOLDER)" not available)
endif

ifneq ($(MAKE_DEVICE_FAMILY_PACK_NAME),none)
DEVICE_FAMILY_PACK_FOLDER := /opt/microchip/dfp/${MAKE_DEVICE_FAMILY_PACK_NAME}/${MAKE_DEVICE_FAMILY_PACK_VERSION}
ifeq ("$(wildcard $(DEVICE_FAMILY_PACK_FOLDER))","")
$(error device family pack "$(DEVICE_FAMILY_PACK_FOLDER)" not available)
endif
endif

TOOLS_COMPILER := ${COMPILER_FOLDER}/bin/${MAKE_COMPILER_NAME}-gcc
TOOLS_BIN2HEX := ${COMPILER_FOLDER}/bin/${MAKE_COMPILER_NAME}-bin2hex
TOOLS_OBJDUMP := ${COMPILER_FOLDER}/bin/${MAKE_COMPILER_NAME}-objdump
TOOLS_IHEX_MODIFIER := ihex_modifier

ARTIFACT_VERSION := $(MAKE_FIRMWARE_VERSION_MAJOR).$(MAKE_FIRMWARE_VERSION_MINOR).$(MAKE_FIRMWARE_VERSION_PATCH)
ARTIFACT_NAME := $(MAKE_PROJECT_NAME)_$(ARTIFACT_VERSION)

SOURCE_FOLDER_CORE := $(MAKE_PATH_TO_CORE)/src
SOURCE_FOLDER_PROJECT := src

MODULE_FOLDERS_CORE := $(foreach module,$(MAKE_MODULES_CORE),$(addprefix $(SOURCE_FOLDER_CORE)/,$(module)))
MODULE_FOLDERS_PROJECT := $(foreach module,$(MAKE_MODULES_PROJECT),$(addprefix $(SOURCE_FOLDER_PROJECT)/,$(module)))

BUILD_FOLDERS := $(foreach module,$(MAKE_MODULES_CORE),$(addprefix build/$(MAKE_PROJECT_NAME)/core/,$(module)))
BUILD_FOLDERS += $(foreach module,$(MAKE_MODULES_PROJECT),$(addprefix build/$(MAKE_PROJECT_NAME)/project/,$(module)))

OBJECTS := $(foreach module,$(MODULE_FOLDERS_CORE),$(patsubst $(SOURCE_FOLDER_CORE)/%.c,build/$(MAKE_PROJECT_NAME)/core/%.o,$(wildcard $(module)/*.c)))
OBJECTS += $(foreach module,$(MODULE_FOLDERS_PROJECT),$(patsubst $(SOURCE_FOLDER_PROJECT)/%.c,build/$(MAKE_PROJECT_NAME)/project/%.o,$(wildcard $(module)/*.c)))

INCLUDE_FOLDERS := $(COMPILER_FOLDER)/include
INCLUDE_FOLDERS += $(COMPILER_FOLDER)/support/generic/h
INCLUDE_FOLDERS += $(COMPILER_FOLDER)/$(BUILD_INCLUDE_SPECIFIC)
INCLUDE_FOLDERS += $(MAKE_PATH_TO_CORE)/include
INCLUDE_FOLDERS += include
INCLUDE_FOLDERS += $(MODULE_FOLDERS_CORE)
INCLUDE_FOLDERS += $(MODULE_FOLDERS_PROJECT)

CPPFLAGS := $(CPPFLAGS_COMMON)
CPPFLAGS += $(foreach define,$(AUTOMATIC_SETTINGS),$(addprefix -D,$(define)))
CPPFLAGS += $(foreach define,$(MAKE_SETTINGS),$(addprefix -D,$(define)))

CFLAGS := $(CFLAGS_COMMON)
ifneq ($(DEVICE_FAMILY_PACK_FOLDER),)
CFLAGS += -mdfp=$(DEVICE_FAMILY_PACK_FOLDER)/xc16
endif
CFLAGS += -mcpu=$(BUILD_PROCESSOR)
CFLAGS += $(BUILD_CFLAGS_SPECIFIC)
CFLAGS += $(foreach include,$(INCLUDE_FOLDERS),$(addprefix -I,$(include)))

LDFLAGS := $(LDFLAGS_COMMON)
ifneq ($(DEVICE_FAMILY_PACK_FOLDER),)
LDFLAGS += -Wl,-mdfp=$(DEVICE_FAMILY_PACK_FOLDER)/xc16
endif
LDFLAGS += -Wl,--processor=$(BUILD_PROCESSOR)
LDFLAGS += -Wl,--script=$(MAKE_PATH_TO_CORE)/make/dspic/${MAKE_CONFIGURATION}/linker.gld
LDFLAGS += $(BUILD_LDFLAGS_SPECIFIC)
LDFLAGS += -Wl,-Map=build/$(ARTIFACT_NAME)_original.map

XFLAGS := -omf=elf
ifneq ($(DEVICE_FAMILY_PACK_FOLDER),)
XFLAGS += -mdfp=$(DEVICE_FAMILY_PACK_FOLDER)/xc16
endif

.PHONY: all clean prerequisites

.SECONDARY:

all: prerequisites build/$(ARTIFACT_NAME).hex build/$(ARTIFACT_NAME)_original.lst

clean:
	@echo "removing build folder"
	@rm -rf build

prerequisites:
	@mkdir -p $(BUILD_FOLDERS)

build/$(MAKE_PROJECT_NAME)/core/%.o: $(SOURCE_FOLDER_CORE)/%.c $(MAKE_PROJECT_NAME).mk
	@echo "compiling $@"
	@$(TOOLS_COMPILER) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build/$(MAKE_PROJECT_NAME)/project/%.o: $(SOURCE_FOLDER_PROJECT)/%.c $(MAKE_PROJECT_NAME).mk
	@echo "compiling $@"
	@$(TOOLS_COMPILER) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build/$(ARTIFACT_NAME)_original.elf: $(OBJECTS)
	@echo "linking $@"
	@$(TOOLS_COMPILER) $(LDFLAGS) $^ -o $@ 2> >(grep -v "Info: Loading file" | grep .)

build/$(ARTIFACT_NAME)_original.lst: build/$(ARTIFACT_NAME)_original.elf
	@echo "creating $@"
	@$(TOOLS_OBJDUMP) $(XFLAGS) -S $< > $@

build/$(ARTIFACT_NAME)_original.hex: build/$(ARTIFACT_NAME)_original.elf
	@echo "creating $@"
	@$(TOOLS_BIN2HEX) $(XFLAGS) -a --offset $(BUILD_HEX_OFFSET) $<

build/$(ARTIFACT_NAME).hex: build/$(ARTIFACT_NAME)_original.hex
	@echo "creating $@"
	@touch $@
	@$(TOOLS_IHEX_MODIFIER) --create $(BUILD_BLOCK_0_START) $(BUILD_BLOCK_0_LENGTH) 0x00FFFFFF build/$(ARTIFACT_NAME)_empty_0.hex
	@$(TOOLS_IHEX_MODIFIER) --create $(BUILD_BLOCK_1_START) $(BUILD_BLOCK_1_LENGTH) 0x00FFFFFF build/$(ARTIFACT_NAME)_empty_1.hex	
	@$(TOOLS_IHEX_MODIFIER) --merge build/$(ARTIFACT_NAME)_empty_0.hex $@
	@$(TOOLS_IHEX_MODIFIER) --merge build/$(ARTIFACT_NAME)_empty_1.hex $@
	@rm build/$(ARTIFACT_NAME)_empty_0.hex
	@rm build/$(ARTIFACT_NAME)_empty_1.hex
	@$(TOOLS_IHEX_MODIFIER) --merge $< $@	
	@$(TOOLS_IHEX_MODIFIER) --checksum $(BUILD_BLOCK_0_START) $(BUILD_BLOCK_0_LENGTH) $(BUILD_BLOCK_0_CHECKSUM) $@
	@$(TOOLS_IHEX_MODIFIER) --checksum $(BUILD_BLOCK_1_START) $(BUILD_BLOCK_1_LENGTH) $(BUILD_BLOCK_1_CHECKSUM) $@

-include $(OBJECTS:.o=.d)

################################################################################
#  E N D   O F   F I L E                                                       #
################################################################################
