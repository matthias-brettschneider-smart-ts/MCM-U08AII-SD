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

/*============================================================================*/
/* I N C L U D E S                                                            */
/*============================================================================*/

#include "hal/IFlash.h"
#include "middleware/IAssert.h"
#include "middleware/ICrc.h"
#include "middleware/IGeneric.h"
#include "os/IBlock.h"
#include "os/IBlockInfo.h"

#ifndef BLOCK_FLASHLOADER
#error "error compile flags - BLOCK_FLASHLOADER undefined"
#elif ((BLOCK_FLASHLOADER != false) && (BLOCK_FLASHLOADER != true))
#error "error compile flags - BLOCK_FLASHLOADER invalid"
#endif

#ifndef BLOCK_DISABLE_INTERRUPTS
#error "error compile flags - BLOCK_DISABLE_INTERRUPTS undefined"
#elif ((BLOCK_DISABLE_INTERRUPTS != false) && (BLOCK_DISABLE_INTERRUPTS != true))
#error "error compile flags - BLOCK_DISABLE_INTERRUPTS invalid"
#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define BLOCK_CHECKSUM_SIZE                                                    2

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static bool BlockRangeIsInRange(const uint32_t address, const uint32_t number_of_bytes, const uint32_t valid_first, const uint32_t valid_last);
static bool BlockRangeIsPartOfBlockType(const uint32_t address, const uint32_t number_of_bytes, const e_BLOCK_INFO_TYPE block_type);

static e_ERROR_ID BlockCalculateChecksumInternal(uint32_t address, uint32_t number_of_bytes, uint16_t * const checksum, const uint32_t checksum_address);
static e_ERROR_ID BlockCheckAllBlocks(const e_BLOCK_INFO_TYPE type);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Check whether a range is completely within another range.
 *
 -----------------------------------------------------------------------------*/
static bool BlockRangeIsInRange(const uint32_t address, const uint32_t number_of_bytes, const uint32_t valid_first, const uint32_t valid_last)
{
  bool in_range = true;

  if ((address < valid_first) || (address > valid_last))
    in_range = false;
  if (number_of_bytes > (valid_last - valid_first + 1))
    in_range = false;
  if ((address + number_of_bytes - 1) > valid_last)
    in_range = false;

  return in_range;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Check whether a range belongs to a block of the given type.
 *
 -----------------------------------------------------------------------------*/
static bool BlockRangeIsPartOfBlockType(const uint32_t address, const uint32_t number_of_bytes, const e_BLOCK_INFO_TYPE block_type)
{
  bool part_of = false;

  const t_BLOCK_INFO * const blocks = BlockInfoGet();
  if (blocks != NULL) {
    for (uint8_t index = 0; blocks[index].type != BLOCK_INFO_TYPE_INVALID; index++) {
      bool in_range = BlockRangeIsInRange(address, number_of_bytes, blocks[index].start_address, blocks[index].end_address);
      part_of = (in_range && (blocks[index].type == block_type)) ? true : part_of;
    }
  }

  return part_of;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Check whether a range is available for erase access.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID BlockRangeIsAvailableForErase(const uint32_t address, const uint32_t number_of_bytes)
{
  const e_BLOCK_INFO_TYPE type_other = BLOCK_FLASHLOADER ? BLOCK_INFO_TYPE_APPLICATION : BLOCK_INFO_TYPE_FLASHLOADER;
  const bool belongs_to_other = BlockRangeIsPartOfBlockType(address, number_of_bytes, type_other);

  return belongs_to_other ? FlashAddressesOneOrMultiplePages(address, number_of_bytes) : ERROR_ID_FLASH_EXCEED_VALID_ADDRESS_RANGE;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Erase one or multiple of the smallest chunks in flash (page).
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID BlockErase(uint32_t address, const uint32_t number_of_bytes)
{
  e_ERROR_ID error = BlockRangeIsAvailableForErase(address, number_of_bytes);
  if (error == ERROR_ID_SUCCESS) {

    error = FlashAquire(BLOCK_DISABLE_INTERRUPTS ? FLASH_DISABLE_INTERRUPTS_PERMANENTLY : FLASH_DISABLE_INTERRUPTS_NEVER);
    if (error == ERROR_ID_SUCCESS) {

      const uint32_t page_size = FlashGetPageSize();
      const uint32_t address_end = address + number_of_bytes - 1;

      while ((error == ERROR_ID_SUCCESS) && (address < address_end)) {
        const bool page_not_erased = FlashBlankCheckPage(address) != ERROR_ID_SUCCESS;
        if (page_not_erased)
          error = FlashErasePage(address);
        address += page_size;
      }

      FlashRelease();
    }
  }

  return error;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Check whether a range is available for write access.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID BlockRangeIsAvailableForWrite(const uint32_t address, const uint32_t number_of_bytes)
{
  const e_BLOCK_INFO_TYPE type_other = BLOCK_FLASHLOADER ? BLOCK_INFO_TYPE_APPLICATION : BLOCK_INFO_TYPE_FLASHLOADER;
  const bool belongs_to_other = BlockRangeIsPartOfBlockType(address, number_of_bytes, type_other);

  return belongs_to_other ? FlashAddressesOneOrMultipleRows(address, number_of_bytes) : ERROR_ID_FLASH_EXCEED_VALID_ADDRESS_RANGE;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Write one or multiple of the smallest chunks in flash (row).
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID BlockWrite(uint32_t address, uint32_t number_of_bytes, uint8_t data[])
{
  ASSERT_FATAL(data != NULL);

  e_ERROR_ID error = BlockRangeIsAvailableForWrite(address, number_of_bytes);
  if (error == ERROR_ID_SUCCESS)
    error = FlashAddressesOneOrMultipleRows(address, number_of_bytes);
  if (error == ERROR_ID_SUCCESS)
    error = FlashAquire(BLOCK_DISABLE_INTERRUPTS ? FLASH_DISABLE_INTERRUPTS_PERMANENTLY : FLASH_DISABLE_INTERRUPTS_NEVER);
  if (error == ERROR_ID_SUCCESS) {

    const uint32_t size_of_row_in_byte = FlashGetRowSize();

    while (number_of_bytes > 0) {
      FlashWriteRow(address, data);
      address += size_of_row_in_byte;
      data += size_of_row_in_byte;
      number_of_bytes -= size_of_row_in_byte;
    }

    FlashRelease();
  }

  return error;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Check whether a range is available for checksum calculation.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID BlockRangeIsAvailableForCheck(const uint32_t address, const uint32_t number_of_bytes)
{
  const bool is_part_of_flashloader = BlockRangeIsPartOfBlockType(address, number_of_bytes, BLOCK_INFO_TYPE_FLASHLOADER);
  const bool is_part_of_application = BlockRangeIsPartOfBlockType(address, number_of_bytes, BLOCK_INFO_TYPE_APPLICATION);

  const bool unknown = !is_part_of_flashloader && !is_part_of_application;
  const bool invalid_address = (address % 2) != 0;
  const bool invalid_length = (number_of_bytes == 0) || ((number_of_bytes % 2) != 0);

  return
      unknown ? ERROR_ID_FLASH_EXCEED_VALID_ADDRESS_RANGE :
          invalid_address ? ERROR_ID_FLASH_INVALID_ADDRESS :
              invalid_length ? ERROR_ID_FLASH_INVALID_LENGTH :
                  ERROR_ID_SUCCESS;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Calculate checksum for memory range that may include its checksum already.
 *
 -----------------------------------------------------------------------------*/
static e_ERROR_ID BlockCalculateChecksumInternal(uint32_t address, uint32_t number_of_bytes, uint16_t * const checksum, const uint32_t checksum_address)
{
  ASSERT_FATAL(checksum != NULL);

  e_ERROR_ID error = BlockRangeIsAvailableForCheck(address, number_of_bytes);
  if (error == ERROR_ID_SUCCESS) {
    error = FlashAquire(FLASH_DISABLE_INTERRUPTS_NEVER);
    if (error == ERROR_ID_SUCCESS) {

      CrcInitCrc16(checksum);

      const bool checksum_provided = checksum_address != 0;
      const bool checksum_within_range = ((checksum_address + BLOCK_CHECKSUM_SIZE - 1) >= address) && (checksum_address < (address + number_of_bytes));
      bool checksum_not_reached = checksum_provided && checksum_within_range;

      while ((error == ERROR_ID_SUCCESS) && (number_of_bytes > 0)) {

        // split parsing into smaller portions because of limited RAM
        uint16_t bytes_once = (number_of_bytes > 256) ? 256 : (uint16_t)number_of_bytes;

        // prevent including checksum itself
        if (checksum_not_reached) {
          if ((address + bytes_once) >= checksum_address) {
            bytes_once = checksum_address - address;
            checksum_not_reached = false;
          }
        }

        // read data and update checksum
        uint8_t buffer[bytes_once];
        error = FlashRead(address, bytes_once, buffer);
        CrcUpdateCrc16(buffer, bytes_once, checksum);
        address += bytes_once;
        number_of_bytes -= bytes_once;

        // skip checksum itself
        if (address == checksum_address) {
          address += BLOCK_CHECKSUM_SIZE;
          number_of_bytes -= BLOCK_CHECKSUM_SIZE;
        }
      }

      // include checksum itself
      if (checksum_provided) {
        uint8_t buffer[BLOCK_CHECKSUM_SIZE];
        error = FlashRead(checksum_address, BLOCK_CHECKSUM_SIZE, buffer);
        CrcUpdateCrc16(buffer, BLOCK_CHECKSUM_SIZE, checksum);
      }

      FlashRelease();
    }
  }

  return error;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Calculate checksum for memory range.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID BlockCheck(uint32_t address, uint32_t number_of_bytes, uint16_t * const checksum)
{
  return BlockCalculateChecksumInternal(address, number_of_bytes, checksum, 0);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Check all blocks of the given type.
 *
 -----------------------------------------------------------------------------*/
static e_ERROR_ID BlockCheckAllBlocks(const e_BLOCK_INFO_TYPE type)
{
  e_ERROR_ID result = ERROR_ID_FAILED;

  const t_BLOCK_INFO * const blocks = BlockInfoGet();
  if (blocks != NULL) {

    result = ERROR_ID_SUCCESS;

    for (uint8_t index = 0; (result == ERROR_ID_SUCCESS) && (blocks[index].type != BLOCK_INFO_TYPE_INVALID); index++) {
      if (blocks[index].type == type) {

        const uint32_t block_length = blocks[index].end_address - blocks[index].start_address + 1;
        uint16_t checksum = 1;
        result = BlockCalculateChecksumInternal(blocks[index].start_address, block_length, &checksum, blocks[index].checksum_address);

        // calculation succeeded but block is not valid
        if ((result == ERROR_ID_SUCCESS) && (checksum != 0))
          result = ERROR_ID_WRONG_CHECKSUM;
      }
    }
  }

  return result;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Check all flashloader blocks.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID BlockCheckFlashloaderBlocks(void)
{
  return BlockCheckAllBlocks(BLOCK_INFO_TYPE_FLASHLOADER);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Check all application blocks.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID BlockCheckApplicationBlocks(void)
{
  return BlockCheckAllBlocks(BLOCK_INFO_TYPE_APPLICATION);
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
