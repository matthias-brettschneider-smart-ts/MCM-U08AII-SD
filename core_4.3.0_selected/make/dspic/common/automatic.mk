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

### common settings ###

AUTOMATIC_SETTINGS += CONTROLLER_TYPE=${BUILD_CONTROLLER}

### individual settings ###

AUTOMATIC_SETTINGS += BLINK_FLASHLOADER=${BUILD_FLASHLOADER}
AUTOMATIC_SETTINGS += BLOCK_FLASHLOADER=${BUILD_FLASHLOADER}
AUTOMATIC_SETTINGS += BLOCK_INFO_NUMBER_OF_BLOCKS=${MEMORY_NUMBER_OF_BLOCKS}
AUTOMATIC_SETTINGS += BLOCK_INFO_0_TYPE=${MEMORY_BLOCK_0_TYPE}
AUTOMATIC_SETTINGS += BLOCK_INFO_0_START_ADDRESS=${MEMORY_BLOCK_0_START_ADDRESS}
AUTOMATIC_SETTINGS += BLOCK_INFO_0_LENGTH=${MEMORY_BLOCK_0_LENGTH}
AUTOMATIC_SETTINGS += BLOCK_INFO_0_CHECKSUM_ADDRESS=${MEMORY_BLOCK_0_CHECKSUM_ADDRESS}
AUTOMATIC_SETTINGS += BLOCK_INFO_1_TYPE=${MEMORY_BLOCK_1_TYPE}
AUTOMATIC_SETTINGS += BLOCK_INFO_1_START_ADDRESS=${MEMORY_BLOCK_1_START_ADDRESS}
AUTOMATIC_SETTINGS += BLOCK_INFO_1_LENGTH=${MEMORY_BLOCK_1_LENGTH}
AUTOMATIC_SETTINGS += BLOCK_INFO_1_CHECKSUM_ADDRESS=${MEMORY_BLOCK_1_CHECKSUM_ADDRESS}
AUTOMATIC_SETTINGS += BLOCK_INFO_2_TYPE=${MEMORY_BLOCK_2_TYPE}
AUTOMATIC_SETTINGS += BLOCK_INFO_2_START_ADDRESS=${MEMORY_BLOCK_2_START_ADDRESS}
AUTOMATIC_SETTINGS += BLOCK_INFO_2_LENGTH=${MEMORY_BLOCK_2_LENGTH}
AUTOMATIC_SETTINGS += BLOCK_INFO_2_CHECKSUM_ADDRESS=${MEMORY_BLOCK_2_CHECKSUM_ADDRESS}
AUTOMATIC_SETTINGS += CALL_APPLICATION_JUMP_ADDRESS=${BUILD_APPLICATION_JUMP_ADDRESS}
AUTOMATIC_SETTINGS += COMPILER_SHARED_RAM_ADDRESS=${MEMORY_SHARED_RAM_ADDRESS}
AUTOMATIC_SETTINGS += CONFIGURATION_BITS_FLASHLOADER=${BUILD_FLASHLOADER}
AUTOMATIC_SETTINGS += EEPROM_EMULATION_START_ADDRESS=${MEMORY_EEPROM_EMULATION_START_ADDRESS}
AUTOMATIC_SETTINGS += EEPROM_EMULATION_NUMBER_OF_PAGES=${MEMORY_EEPROM_EMULATION_NUMBER_OF_PAGES}
AUTOMATIC_SETTINGS += INFO_FIRMWARE_TYPE=${MAKE_FIRMWARE_TYPE}
AUTOMATIC_SETTINGS += INFO_FIRMWARE_VARIANT=${MAKE_FIRMWARE_VARIANT}
AUTOMATIC_SETTINGS += INFO_FIRMWARE_VERSION_MAJOR=$(MAKE_FIRMWARE_VERSION_MAJOR)
AUTOMATIC_SETTINGS += INFO_FIRMWARE_VERSION_MINOR=$(MAKE_FIRMWARE_VERSION_MINOR)
AUTOMATIC_SETTINGS += INFO_FIRMWARE_VERSION_PATCH=$(MAKE_FIRMWARE_VERSION_PATCH)
AUTOMATIC_SETTINGS += MAIN_FLASHLOADER=${BUILD_FLASHLOADER}
AUTOMATIC_SETTINGS += UDS_FLASHLOADER=${BUILD_FLASHLOADER}

################################################################################
#  E N D   O F   F I L E                                                       #
################################################################################
