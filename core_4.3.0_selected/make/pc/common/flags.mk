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

# set optimization level 1, produce debugging information and keep intermediate files
CFLAGS_COMMON := -O1
CFLAGS_COMMON += -g

# enable warnings
CFLAGS_COMMON += -Wall
CFLAGS_COMMON += -Wextra
CFLAGS_COMMON += -Wmissing-declarations
CFLAGS_COMMON += -Wredundant-decls
CFLAGS_COMMON += -Wuninitialized
CFLAGS_COMMON += -Wshadow

# 
CFLAGS_COMMON += -Wno-unused-local-typedefs
CFLAGS_COMMON += -Wno-unused-function

# treat all warnings as errors
CFLAGS_COMMON += -Werror

#--- linker --------------------------------------------------------------------

LDFLAGS_COMMON += 

################################################################################
#  E N D   O F   F I L E                                                       #
################################################################################
