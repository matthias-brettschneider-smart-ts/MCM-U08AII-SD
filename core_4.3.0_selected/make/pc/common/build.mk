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

ARTIFACT_NAME := $(MAKE_PROJECT_NAME)

TOOLS_COMPILER := g++

SOURCE_FOLDER_CORE := $(MAKE_PATH_TO_CORE)/src
SOURCE_FOLDER_PROJECT := $(MAKE_PATH_TO_PROJECT)/src
SOURCE_FOLDER_TEST := src

MODULE_FOLDERS_CORE := $(foreach module,$(MAKE_MODULES_CORE),$(addprefix $(SOURCE_FOLDER_CORE)/,$(module)))
MODULE_FOLDERS_PROJECT := $(foreach module,$(MAKE_MODULES_PROJECT),$(addprefix $(SOURCE_FOLDER_PROJECT)/,$(module)))
MODULE_FOLDERS_TEST := $(foreach module,$(MAKE_MODULES_TEST),$(addprefix $(SOURCE_FOLDER_TEST)/,$(module)))

BUILD_FOLDERS := $(foreach module,$(MAKE_MODULES_CORE),$(addprefix build/$(MAKE_PROJECT_NAME)/core/,$(module)))
BUILD_FOLDERS += $(foreach module,$(MAKE_MODULES_PROJECT),$(addprefix build/$(MAKE_PROJECT_NAME)/project/,$(module)))
BUILD_FOLDERS += $(foreach module,$(MAKE_MODULES_TEST),$(addprefix build/$(MAKE_PROJECT_NAME)/test/,$(module)))

OBJECTS := $(foreach module,$(MODULE_FOLDERS_CORE),$(patsubst $(SOURCE_FOLDER_CORE)/%.c,build/$(MAKE_PROJECT_NAME)/core/%.o,$(wildcard $(module)/*.c)))
OBJECTS += $(foreach module,$(MODULE_FOLDERS_CORE),$(patsubst $(SOURCE_FOLDER_CORE)/%.cpp,build/$(MAKE_PROJECT_NAME)/core/%.o,$(wildcard $(module)/*.cpp)))
OBJECTS += $(foreach module,$(MODULE_FOLDERS_PROJECT),$(patsubst $(SOURCE_FOLDER_PROJECT)/%.c,build/$(MAKE_PROJECT_NAME)/project/%.o,$(wildcard $(module)/*.c)))
OBJECTS += $(foreach module,$(MODULE_FOLDERS_PROJECT),$(patsubst $(SOURCE_FOLDER_PROJECT)/%.cpp,build/$(MAKE_PROJECT_NAME)/project/%.o,$(wildcard $(module)/*.cpp)))
OBJECTS += $(foreach module,$(MODULE_FOLDERS_TEST),$(patsubst $(SOURCE_FOLDER_TEST)/%.c,build/$(MAKE_PROJECT_NAME)/test/%.o,$(wildcard $(module)/*.c)))
OBJECTS += $(foreach module,$(MODULE_FOLDERS_TEST),$(patsubst $(SOURCE_FOLDER_TEST)/%.cpp,build/$(MAKE_PROJECT_NAME)/test/%.o,$(wildcard $(module)/*.cpp)))

INCLUDE_FOLDERS := $(MAKE_PATH_TO_CORE)/include
INCLUDE_FOLDERS += $(MAKE_PATH_TO_PROJECT)/include
INCLUDE_FOLDERS += include
INCLUDE_FOLDERS += $(MAKE_INCLUDE)
INCLUDE_FOLDERS += $(MODULE_FOLDERS_CORE)
INCLUDE_FOLDERS += $(MODULE_FOLDERS_PROJECT)
INCLUDE_FOLDERS += $(MODULE_FOLDERS_TEST)

CPPFLAGS := $(CPPFLAGS_COMMON)
CPPFLAGS += $(foreach define,$(MAKE_SETTINGS),$(addprefix -D,$(define)))

CFLAGS := $(CFLAGS_COMMON)
CFLAGS += $(foreach include,$(INCLUDE_FOLDERS),$(addprefix -I,$(include)))

LDFLAGS := $(LDFLAGS_COMMON)

.PHONY: all clean prerequisites run

.SECONDARY:

all: prerequisites build/$(ARTIFACT_NAME).run

clean:
	@echo "removing build folder"
	@rm -rf build

prerequisites:
	@mkdir -p $(BUILD_FOLDERS)

build/$(MAKE_PROJECT_NAME)/core/%.o: $(SOURCE_FOLDER_CORE)/%.c
	@echo "compiling $@"
	@$(TOOLS_COMPILER) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build/$(MAKE_PROJECT_NAME)/core/%.o: $(SOURCE_FOLDER_CORE)/%.cpp
	@echo "compiling $@"
	@$(TOOLS_COMPILER) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build/$(MAKE_PROJECT_NAME)/project/%.o: $(SOURCE_FOLDER_PROJECT)/%.c
	@echo "compiling $@"
	@$(TOOLS_COMPILER) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build/$(MAKE_PROJECT_NAME)/project/%.o: $(SOURCE_FOLDER_PROJECT)/%.cpp
	@echo "compiling $@"
	@$(TOOLS_COMPILER) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build/$(MAKE_PROJECT_NAME)/test/%.o: $(SOURCE_FOLDER_TEST)/%.c
	@echo "compiling $@"
	@$(TOOLS_COMPILER) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build/$(MAKE_PROJECT_NAME)/test/%.o: $(SOURCE_FOLDER_TEST)/%.cpp
	@echo "compiling $@"
	@$(TOOLS_COMPILER) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build/$(ARTIFACT_NAME).run: $(OBJECTS)
	@echo "linking $@"
	@$(TOOLS_COMPILER) $(LDFLAGS) $^ $(MAKE_LIB) -o $@

run:
	@./build/$(ARTIFACT_NAME).run --gtest_print_time=0

-include $(OBJECTS:.o=.d)

################################################################################
#  E N D   O F   F I L E                                                       #
################################################################################
