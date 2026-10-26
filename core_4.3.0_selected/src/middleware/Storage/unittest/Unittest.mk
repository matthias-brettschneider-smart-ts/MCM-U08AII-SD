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

MAKE_PROJECT_NAME := Unittest

MAKE_SETTINGS += STORAGE_MINIMUM_SIZE_PER_BLOCK_IN_BYTE=64
MAKE_SETTINGS += STORAGE_MAXIMUM_SIZE_PER_BLOCK_IN_BYTE=256
MAKE_SETTINGS += STORAGE_NUMBER_OF_REGISTRATIONS=14
MAKE_SETTINGS += STORAGE_BLOCK_CORE_IDENTIFICATION_SIZE_IN_BYTE=128
MAKE_SETTINGS += STORAGE_BLOCK_CORE_SETTINGS_SIZE_IN_BYTE=64
MAKE_SETTINGS += STORAGE_BLOCK_APPL_SETTINGS_SIZE_IN_BYTE=192
MAKE_SETTINGS += STORAGE_BLOCK_APPL_ADJUSTMENT_SMART_SIZE_IN_BYTE=64
MAKE_SETTINGS += STORAGE_BLOCK_APPL_ADJUSTMENT_CUSTOMER_SIZE_IN_BYTE=64

MAKE_MODULES_CORE +=

MAKE_MODULES_PROJECT += middleware/Storage

MAKE_MODULES_TEST += Mock
MAKE_MODULES_TEST += Test

MAKE_PATH_TO_CORE := ../../../..

MAKE_PATH_TO_PROJECT := ../../../..

include $(MAKE_PATH_TO_CORE)/make/pc/build_gtest.mk

################################################################################
#  E N D   O F   F I L E                                                       #
################################################################################
