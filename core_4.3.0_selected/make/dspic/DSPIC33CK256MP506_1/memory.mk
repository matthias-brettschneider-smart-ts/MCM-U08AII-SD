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

MEMORY_NUMBER_OF_BLOCKS := 3

MEMORY_BLOCK_0_TYPE := BLOCK_INFO_TYPE_FLASHLOADER
MEMORY_BLOCK_0_START_ADDRESS := 0
MEMORY_BLOCK_0_LENGTH := 0x10000
MEMORY_BLOCK_0_CHECKSUM_ADDRESS := 0xFFFC

MEMORY_BLOCK_1_TYPE := BLOCK_INFO_TYPE_FLASHLOADER
MEMORY_BLOCK_1_START_ADDRESS := 0x2B000
MEMORY_BLOCK_1_LENGTH := 0x1000
MEMORY_BLOCK_1_CHECKSUM_ADDRESS := 0x2BDFC

MEMORY_BLOCK_2_TYPE := BLOCK_INFO_TYPE_APPLICATION
MEMORY_BLOCK_2_START_ADDRESS := 0x800000
MEMORY_BLOCK_2_LENGTH := 0x2C000
MEMORY_BLOCK_2_CHECKSUM_ADDRESS := 0x82BDFC

MEMORY_SHARED_RAM_ADDRESS := 0x1000

################################################################################
#  E N D   O F   F I L E                                                       #
################################################################################
