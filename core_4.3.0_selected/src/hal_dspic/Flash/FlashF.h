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

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

// flash organization (all sizes including phantom byte)
#define FLASH_SIZE_OF_WORD_IN_BYTE                                             4
#define FLASH_SIZE_OF_ROW_IN_BYTE                                            256
#define FLASH_SIZE_OF_PAGE_IN_BYTE                                          2048

#define FLASH_ERASE_VALUE                                             0x00FFFFFF

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Read 32 bit via table registers. Address has to be 4 byte aligned.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint32_t FLASH_INTERNAL_READ_32_BIT(const uint32_t address)
{
  ASSERT_FATAL((address % FLASH_SIZE_OF_WORD_IN_BYTE) == 0);

  const uint16_t savedTBLPAG = TBLPAG;
  TBLPAG = (uint16_t)(address >> 17);
  const uint32_t read = FLASH_COMMON_TABLE_READ((uint16_t)(address >> 1));
  TBLPAG = savedTBLPAG;

  return read;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Write 32 bit via table registers. Address has to be 4 byte aligned.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE e_ERROR_ID FLASH_INTERNAL_WRITE_32_BIT(const uint32_t address, const uint32_t value)
{
  ASSERT_FATAL((address % FLASH_SIZE_OF_WORD_IN_BYTE) == 0);

  const uint16_t tblpag_saved = TBLPAG;
  TBLPAG = (uint16_t)(address >> 17);
  FLASH_COMMON_TABLE_WRITE((uint16_t)(address >> 1), value);
  FlashExecuteEraseOrWrite(0x4003);
  TBLPAG = tblpag_saved;

  const uint32_t actual = FlashRead32Bit(address);

  return (actual == value) ? ERROR_ID_SUCCESS : ERROR_ID_WRITE_ERROR;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Write 64 bit. Address has to be 4 byte aligned.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE e_ERROR_ID FLASH_INTERNAL_WRITE_64_BIT(const uint32_t address, const uint32_t value[])
{
  ASSERT_FATAL((address % FLASH_SIZE_OF_WORD_IN_BYTE) == 0);
  ASSERT_FATAL(value != NULL);

  const bool done =
      (FLASH_INTERNAL_WRITE_32_BIT(address + 0, value[0]) == ERROR_ID_SUCCESS) &&
      (FLASH_INTERNAL_WRITE_32_BIT(address + 4, value[1]) == ERROR_ID_SUCCESS);

  return done ? ERROR_ID_SUCCESS : ERROR_ID_WRITE_ERROR;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Read memory block with all bytes within one flash page but without any further alignment or size limitations.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE e_ERROR_ID FLASH_INTERNAL_READ_WITHIN_PAGE(const uint32_t address, uint16_t number_of_bytes, uint8_t * data)
{
  return FLASH_COMMON_READ_WITHIN_PAGE(address, number_of_bytes, data);
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

  // set page and perform dummy write
  const uint16_t tblpag_saved = TBLPAG;
  TBLPAG = (uint16_t)(address >> 17);
  FLASH_COMMON_TABLE_WRITE((uint16_t)(address >> 1), 0);
  FlashExecuteEraseOrWrite(0x4042);
  TBLPAG = tblpag_saved;

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
  ASSERT_FATAL((address % FLASH_SIZE_OF_ROW_IN_BYTE) == 0);
  ASSERT_FATAL(data != NULL);

  uint16_t offset = (uint16_t)(address >> 1);

  // program via table registers
  const uint16_t tblpag_saved = TBLPAG;
  TBLPAG = (uint16_t)(address >> 17);
  uint32_t * source = (uint32_t *)data;
  for (uint16_t count = 0; count < (FlashGetRowSize() / FLASH_SIZE_OF_WORD_IN_BYTE); count++) {
    FLASH_COMMON_TABLE_WRITE(offset, *source);
    offset += FLASH_SIZE_OF_WORD_IN_BYTE / 2;
    source++;
  }
  FlashExecuteEraseOrWrite(0x4001);
  TBLPAG = tblpag_saved;

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
