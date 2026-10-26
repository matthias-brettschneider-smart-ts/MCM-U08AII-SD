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

include $(MAKE_PATH_TO_CORE)/make/dspic/${MAKE_CONFIGURATION}/memory.mk

BUILD_PROCESSOR := 33CK256MP508
BUILD_CONTROLLER := CONTROLLER_DSPIC33CK256MP508

BUILD_FLASHLOADER := false

BUILD_BLOCK_0_START := ${MEMORY_BLOCK_2_START_ADDRESS}
BUILD_BLOCK_0_LENGTH := ${MEMORY_BLOCK_2_LENGTH}
BUILD_BLOCK_0_CHECKSUM := ${MEMORY_BLOCK_2_CHECKSUM_ADDRESS}

BUILD_BLOCK_1_START := 0
BUILD_BLOCK_1_LENGTH := 0
BUILD_BLOCK_1_CHECKSUM := 0

BUILD_HEX_OFFSET := 0x400000

BUILD_INCLUDE_SPECIFIC := support/dsPIC33C/h

BUILD_CFLAGS_SPECIFIC := --partition 2

BUILD_LDFLAGS_SPECIFIC := -Wl,--partition,2
BUILD_LDFLAGS_SPECIFIC += -Wl,-DLINK_FLASHLOADER=0

################################################################################
#  E N D   O F   F I L E                                                       #
################################################################################
