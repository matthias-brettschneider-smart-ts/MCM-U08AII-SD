/*-----------------------------------------------------------------------------+
|                                                                              |
|                         |\    /|   /- - -  |\    /|                          |
|                         | \  / |  |        | \  / |                          |
|                         |  \/  |  |        |  \/  |                          |
|                         |      |   \_ _ _  |      |                          |
|                                                                              |
|                                                                              |
|         T H E   S M A R T   M E A S U R E M E N T   S O L U T I O N          |
|                                                                              |
+------------------------------------------------------------------------------+
|             (c) SMART TESTSOLUTIONS GmbH - all rights reserved               |
+-----------------------------------------------------------------------------*/
#pragma once

/*============================================================================*/
/* I N C L U D E S                                                            */
/*============================================================================*/

#include "hal/ICompiler.h"
#include "hal/IController.h"
#include "hal/IFlash.h"
#include "middleware/IAssert.h"
#include "middleware/IGeneric.h"
#include "Flash.h"

#ifndef FLASH_TRANSLATE_ADDRESSES
#error "error compile flags - FLASH_TRANSLATE_ADDRESSES undefined"
#elif ((FLASH_TRANSLATE_ADDRESSES != false) && (FLASH_TRANSLATE_ADDRESSES != true))
#error "error compile flags - FLASH_TRANSLATE_ADDRESSES invalid"
#endif

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

#define FLASH_PARTITION_ONE_START_ADDRESS                               0x000000
#define FLASH_PARTITION_ONE_END_ADDRESS                                 0x0157FF
#define FLASH_PARTITION_TWO_START_ADDRESS                               0x800000
#define FLASH_PARTITION_TWO_END_ADDRESS                                 0x8157FF
#define FLASH_OFFSET_OF_LAST_FLASH_PAGE_WITHIN_PARTITION                 0x15000
#define FLASH_OFFSET_OF_FSIGN_WITHIN_LAST_FLASH_PAGE                       0x728

// flash organization (all sizes including phantom byte)
#define FLASH_SIZE_OF_WORD_IN_BYTE                                             4
#define FLASH_SIZE_OF_INSTRUCTION_IN_BYTE                                      8
#define FLASH_SIZE_OF_ROW_IN_BYTE                                            256
#define FLASH_SIZE_OF_PAGE_IN_BYTE                                          2048

#define FLASH_ERASE_VALUE                                             0x00FFFFFF

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Convert the absolute address from the linear address space into the current partitions perspective.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint32_t FLASH_TRANSLATE_ADDRESS_INTO_PARTITION_PERSPECTIVE(uint32_t address)
{
#if (FLASH_TRANSLATE_ADDRESSES) // workaround because NVMCONbits.P2ACTIV does not work correctly

    if ((address >= FLASH_PARTITION_TWO_START_ADDRESS) && (address <= FLASH_PARTITION_TWO_END_ADDRESS))
      address -= FLASH_PARTITION_TWO_START_ADDRESS;
    else if (address <= FLASH_PARTITION_ONE_END_ADDRESS)
      address += FLASH_PARTITION_TWO_START_ADDRESS;

#endif

  return address;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Read 32 bit via table registers. Address has to be 4 byte aligned.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint32_t FLASH_INTERNAL_READ_32_BIT(const uint32_t address)
{
  ASSERT_FATAL((address % FLASH_SIZE_OF_WORD_IN_BYTE) == 0);

  const uint32_t address_translated = FLASH_TRANSLATE_ADDRESS_INTO_PARTITION_PERSPECTIVE(address);
  const uint16_t savedTBLPAG = TBLPAG;
  TBLPAG = (uint16_t)(address_translated >> 17);
  const uint16_t offset = (uint16_t)(address_translated >> 1);
  const uint32_t read = FLASH_COMMON_TABLE_READ(offset);
  TBLPAG = savedTBLPAG;

  return read;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Write 64 bit via table registers. Address has to be 8 byte aligned.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE e_ERROR_ID FLASH_INTERNAL_WRITE_64_BIT(const uint32_t address, const uint32_t value[])
{
  ASSERT_FATAL((address % FLASH_SIZE_OF_INSTRUCTION_IN_BYTE) == 0);
  ASSERT_FATAL(value != NULL);

  const uint32_t address_translated = FLASH_TRANSLATE_ADDRESS_INTO_PARTITION_PERSPECTIVE(address);
  const uint16_t tblpag_saved = TBLPAG;
  TBLPAG = 0xFA;
  NVMADRH = (uint16_t)(address_translated >> 17);
  NVMADRL = (uint16_t)(address_translated >> 1);
  FLASH_COMMON_TABLE_WRITE(0, value[0]);
  FLASH_COMMON_TABLE_WRITE(2, value[1]);
  FlashExecuteEraseOrWrite(0x4001);
  TBLPAG = tblpag_saved;

  uint32_t actual[2];
  actual[0] = FlashRead32Bit(address);
  actual[1] = FlashRead32Bit(address + 4);
  const bool done = (actual[0] == value[0]) && (actual[1] == value[1]);

  return done ? ERROR_ID_SUCCESS : ERROR_ID_WRITE_ERROR;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Write 32 bit. Address has to be 4 byte aligned.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE e_ERROR_ID FLASH_INTERNAL_WRITE_32_BIT(const uint32_t address, const uint32_t value)
{
  ASSERT_FATAL((address % FLASH_SIZE_OF_WORD_IN_BYTE) == 0);

  // read
  const uint32_t address_aligned = address & ~7ul;
  const bool write_odd_32_bit = (address & 7ul) != 0;
  uint32_t expected[2];
  expected[0] = FlashRead32Bit(address_aligned);
  expected[1] = FlashRead32Bit(address_aligned + 4);
  ASSERT_FATAL((!write_odd_32_bit && (expected[0] == 0x00FFFFFFul)) || (write_odd_32_bit && (expected[1] == 0x00FFFFFFul)));

  // modify
  expected[write_odd_32_bit ? 1 : 0] = value;

  // write
  return FlashWrite64Bit(address_aligned, expected);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Read memory block with all bytes within one flash page but without any further alignment or size limitations.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE e_ERROR_ID FLASH_INTERNAL_READ_WITHIN_PAGE(const uint32_t address, uint16_t number_of_bytes, uint8_t * data)
{
  const uint32_t address_translated = FLASH_TRANSLATE_ADDRESS_INTO_PARTITION_PERSPECTIVE(address);
  return FLASH_COMMON_READ_WITHIN_PAGE(address_translated, number_of_bytes, data);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Erase single flash page. Address has to be page aligned.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE e_ERROR_ID FLASH_INTERNAL_ERASE_PAGE(const uint32_t address)
{
  ASSERT_FATAL((address % FLASH_SIZE_OF_PAGE_IN_BYTE) == 0);

  const uint32_t address_translated = FLASH_TRANSLATE_ADDRESS_INTO_PARTITION_PERSPECTIVE(address);
  NVMADRH = (uint16_t)(address_translated >> 17);
  NVMADRL = (uint16_t)(address_translated >> 1);
  FlashExecuteEraseOrWrite(0x4003);

  // reestablish FSIGN configuration bits if necessary
  const bool last_page =
      (address == FLASH_OFFSET_OF_LAST_FLASH_PAGE_WITHIN_PARTITION) ||
      (address == (FLASH_PARTITION_TWO_START_ADDRESS + FLASH_OFFSET_OF_LAST_FLASH_PAGE_WITHIN_PARTITION));
  if (last_page) {
    const uint32_t fsign_address = address + FLASH_OFFSET_OF_FSIGN_WITHIN_LAST_FLASH_PAGE;
    const uint32_t fsign_value_32 = 0x00FF7FFF;
    ASSERT_FATAL(FlashRead32Bit(fsign_address) == FLASH_ERASE_VALUE);
    ASSERT_FATAL(FlashWrite32Bit(fsign_address, fsign_value_32) == ERROR_ID_SUCCESS);
    ASSERT_FATAL(FlashRead32Bit(fsign_address) == fsign_value_32);
  }

  return ERROR_ID_SUCCESS;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Write one row within flash page. Address has to be row aligned.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE e_ERROR_ID FLASH_INTERNAL_WRITE_ROW(const uint32_t address, const uint8_t data[])
{
  ASSERT_STATIC(sizeof(uint8_t *) == 2);
  ASSERT_FATAL((address % FLASH_SIZE_OF_ROW_IN_BYTE) == 0);
  ASSERT_FATAL(data != NULL);

  const uint32_t address_translated = FLASH_TRANSLATE_ADDRESS_INTO_PARTITION_PERSPECTIVE(address);
  NVMADRH = (uint16_t)(address_translated >> 17);
  NVMADRL = (uint16_t)(address_translated >> 1);
  NVMSRCADRH = 0;
  NVMSRCADR = (uint16_t)data;
  FlashExecuteEraseOrWrite(0x4002);

  return ERROR_ID_SUCCESS;
}

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
