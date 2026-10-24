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

MAKE_PATH_TO_FLASHLOADER := flashloader
MAKE_FLASHLOADER_PROJECT_NAME := MCM_U08AII_SD_flashloader

MAKE_PATH_TO_APPLICATION := application
MAKE_APPLICATION_PROJECT_NAME := MCM_U08AII_SD_application

MAKE_CONFIGURATION := DSPIC33CK256MP506_1

MAKE_PATH_TO_CORE := core

include $(MAKE_PATH_TO_CORE)/make/dspic/merge.mk

################################################################################
#  E N D   O F   F I L E                                                       #
################################################################################
