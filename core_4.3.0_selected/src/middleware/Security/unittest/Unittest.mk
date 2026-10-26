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

MAKE_SETTINGS += SECURITY_SIZE_OF_SEED_IN_BYTE=4
MAKE_SETTINGS += SECURITY_SIZE_OF_KEY_IN_BYTE=4

MAKE_MODULES_CORE +=

MAKE_MODULES_PROJECT += middleware/Security

MAKE_MODULES_TEST += Mock
MAKE_MODULES_TEST += Test

MAKE_PATH_TO_CORE := ../../../..

MAKE_PATH_TO_PROJECT := ../../../..

include $(MAKE_PATH_TO_CORE)/make/pc/build_gtest.mk

################################################################################
#  E N D   O F   F I L E                                                       #
################################################################################
