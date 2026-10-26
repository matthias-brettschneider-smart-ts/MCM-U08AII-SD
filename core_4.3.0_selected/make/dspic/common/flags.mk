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

#--- preprocessor --------------------------------------------------------------

# describe dependencies
CPPFLAGS_COMMON := -MMD
CPPFLAGS_COMMON += -MP

#--- compiler ------------------------------------------------------------------

# use c99 
CFLAGS_COMMON := --std=c99

# set optimization level 1, produce debugging information and keep intermediate files
CFLAGS_COMMON += -O1
CFLAGS_COMMON += -g

# set code and data model
CFLAGS_COMMON += -msmall-code
CFLAGS_COMMON += -mlarge-data
CFLAGS_COMMON += -msmall-scalar

# prevent double from falling back to float
CFLAGS_COMMON += -fno-short-double

# enable warnings
CFLAGS_COMMON += -Wall
CFLAGS_COMMON += -Wextra
CFLAGS_COMMON += -Wmissing-declarations
CFLAGS_COMMON += -Wmissing-prototypes
CFLAGS_COMMON += -Wstrict-prototypes
CFLAGS_COMMON += -Wredundant-decls
CFLAGS_COMMON += -Wuninitialized
CFLAGS_COMMON += -Wshadow
CFLAGS_COMMON += -Wundef
CFLAGS_COMMON += -msfr-warn=on

# treat all warnings as errors
ifneq ($(MAKE_TREAT_ALL_WARNINGS_AS_ERRORS),false)
CFLAGS_COMMON += -Werror
endif

#--- linker --------------------------------------------------------------------

# set object module format to ELF/DWARF
LDFLAGS_COMMON := -Wl,-omf=elf

# support initialized data (packed = without filling highest instruction byte)
LDFLAGS_COMMON += -Wl,--data-init
LDFLAGS_COMMON += -Wl,--pack-data

# far code pointers
LDFLAGS_COMMON += -Wl,--handles

# create default ISR
LDFLAGS_COMMON += -Wl,--isr

# prevent allocating stack in extended data space memory and set minimum stack guard
LDFLAGS_COMMON += -Wl,--stack=0
LDFLAGS_COMMON += -Wl,--stackguard=16
LDFLAGS_COMMON += -Wl,--local-stack

# check for section overlapping (not allowed) and remove unused sections (garbage collection)
LDFLAGS_COMMON += -Wl,--check-sections
LDFLAGS_COMMON += -Wl,--gc-sections

# do not force linking objects that may not be compatible (processor is defined target specific)
LDFLAGS_COMMON += -Wl,--no-force-link

# allow attempting to conserve memory by merging I/O library function calls
LDFLAGS_COMMON += -Wl,--smart-io

# display memory usage
ifeq ($(MAKE_DISPLAY_MEMORY_USAGE),true)
LDFLAGS_COMMON += -Wl,--report-mem
endif

################################################################################
#  E N D   O F   F I L E                                                       #
################################################################################
