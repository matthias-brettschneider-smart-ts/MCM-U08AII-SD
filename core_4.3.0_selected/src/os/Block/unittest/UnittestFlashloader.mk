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

MAKE_PROJECT_NAME := UnittestFlashloader

MAKE_SETTINGS += BLOCK_DISABLE_INTERRUPTS=false
MAKE_SETTINGS += BLOCK_FLASHLOADER=true

MAKE_MODULES_CORE +=

MAKE_MODULES_PROJECT += os/Block

MAKE_MODULES_TEST += Mock
MAKE_MODULES_TEST += TestFlashloader

MAKE_PATH_TO_CORE := ../../../..

MAKE_PATH_TO_PROJECT := ../../../..

include $(MAKE_PATH_TO_CORE)/make/pc/build_gtest.mk

################################################################################
#  E N D   O F   F I L E                                                       #
################################################################################
