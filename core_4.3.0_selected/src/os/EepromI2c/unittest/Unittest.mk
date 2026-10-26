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

MAKE_SETTINGS += EEPROM_I2C_NODE=0
MAKE_SETTINGS += EEPROM_I2C_BITRATE_IN_KBPS=400000
MAKE_SETTINGS += EEPROM_I2C_DEVICE_ADDRESS=0x57

MAKE_MODULES_CORE += 

MAKE_MODULES_PROJECT += os/EepromI2c

MAKE_MODULES_TEST += Mock
MAKE_MODULES_TEST += Test

MAKE_PATH_TO_CORE := ../../../..

MAKE_PATH_TO_PROJECT := ../../../..

include $(MAKE_PATH_TO_CORE)/make/pc/build_gtest.mk

################################################################################
#  E N D   O F   F I L E                                                       #
################################################################################
