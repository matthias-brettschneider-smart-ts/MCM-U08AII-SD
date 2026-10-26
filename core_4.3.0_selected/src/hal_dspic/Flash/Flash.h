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
#include "middleware/IAssert.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

void FlashInternalPreEraseAndWrite(void); // IME: core - dsPIC33C - flash - not here
void FlashInternalPostEraseAndWrite(void);
void FlashExecuteEraseOrWrite(const uint16_t nvmcon);

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Read 32 bit via table registers.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint32_t FLASH_COMMON_TABLE_READ(const uint32_t offset)
{
  ASSERT_FATAL((offset & 1) == 0);

  t_UNSIGNED_SIZES read;
  read.data_word[0] = __builtin_tblrdl(offset);
  read.data_word[1] = __builtin_tblrdh(offset);

  return read.data_long;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Write 32 bit via table registers.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void FLASH_COMMON_TABLE_WRITE(const uint32_t offset, const uint32_t data)
{
  ASSERT_FATAL((offset & 1) == 0);

  t_UNSIGNED_SIZES write;
  write.data_long = data;
  __builtin_tblwtl(offset, write.data_word[0]);
  __builtin_tblwth(offset, write.data_word[1]);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Perform the desired operation.
 *
 -----------------------------------------------------------------------------*/
void FlashExecuteEraseOrWrite(const uint16_t nvmcon)
{
  FlashInternalPreEraseAndWrite();

  NVMCON = nvmcon;

  // unlock NVM (with disabled interrupts) and execute erase/write including additional NOPs
  __builtin_disi(5);
  __builtin_write_NVM();

  // wait till it's done (necessary in case CPU does not stall)
  const uint32_t endless_loop_timer = WAIT_GET_TIMER_VALUE_32();
  while (NVMCONbits.WR != 0)
    if (WAIT_CHECK_TIMEOUT_MS_32(endless_loop_timer, 1000))
      ErrorAddEntry(ERROR_ID_FATAL_ENDLESS_LOOP, 0);

  ASSERT_FATAL(NVMCONbits.WRERR == 0);

  FlashInternalPostEraseAndWrite();
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Read memory block with all bytes within one flash page but without any further alignment or size limitations.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE e_ERROR_ID FLASH_COMMON_READ_WITHIN_PAGE(const uint32_t address, uint16_t number_of_bytes, uint8_t * data)
{
  ASSERT_FATAL(data != NULL);

  const uint16_t tblpag_saved = TBLPAG;
  TBLPAG = (uint16_t)(address >> 17);
  uint16_t offset = (uint16_t)((address & ~3ul) >> 1);

  t_UNSIGNED_SIZES value;

  // handle start at addresses not multiple of 4 byte
  const uint16_t first_offset = (uint16_t)(address & 3);
  ASSERT_FATAL(first_offset <= 3);
  if (first_offset != 0) {
    uint16_t first_bytes = 4 - first_offset;
    first_bytes = (first_bytes > number_of_bytes) ? number_of_bytes : first_bytes;
    value.data_long = FLASH_COMMON_TABLE_READ(offset);
    memcpy(data, &value.data_byte[first_offset], first_bytes);
    number_of_bytes -= first_bytes;
    data += first_bytes;
    offset += 2;
  }

  // handle all 4 byte aligned data
  while (number_of_bytes >= 4) {
    value.data_long = FLASH_COMMON_TABLE_READ(offset);
    memcpy(data, &value, 4);
    number_of_bytes -= 4;
    data += 4;
    offset += 2;
  }

  // handle remaining bytes
  if (number_of_bytes > 0) {
    value.data_long = FLASH_COMMON_TABLE_READ(offset);
    memcpy(data, &value, number_of_bytes);
  }

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
