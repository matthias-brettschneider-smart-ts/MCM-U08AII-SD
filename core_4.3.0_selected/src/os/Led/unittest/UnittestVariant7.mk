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

MAKE_PROJECT_NAME := UnittestVariant7

MAKE_SETTINGS += LED_VARIANT=7

MAKE_MODULES_CORE += 

MAKE_MODULES_PROJECT += os/Led

MAKE_MODULES_TEST += MockCommon
MAKE_MODULES_TEST += MockVariant7
MAKE_MODULES_TEST += TestVariant7

MAKE_PATH_TO_CORE := ../../../..

MAKE_PATH_TO_PROJECT := ../../../..

include $(MAKE_PATH_TO_CORE)/make/pc/build_gtest.mk

################################################################################
#  E N D   O F   F I L E                                                       #
################################################################################
