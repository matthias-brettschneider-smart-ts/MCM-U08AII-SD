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

MERGE_BLOCK_APPLICATION_START := ${MEMORY_BLOCK_2_START_ADDRESS}
MERGE_BLOCK_APPLICATION_LENGTH := ${MEMORY_BLOCK_2_LENGTH}
MERGE_BLOCK_APPLICATION_CHECKSUM := ${MEMORY_BLOCK_2_CHECKSUM_ADDRESS}

################################################################################
#  E N D   O F   F I L E                                                       #
################################################################################
