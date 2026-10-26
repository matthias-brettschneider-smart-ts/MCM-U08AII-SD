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
#include "middleware/ICrc.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"
#include "os/IBlockInfo.h"

#include "IMock.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef struct {
  int error_count;
  uint32_t flash_erase_counter;
  uint32_t flash_write_counter;
} t_MOCK;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_MOCK mock;

static const t_BLOCK_INFO mock_block_info[] = {
    { .type = BLOCK_INFO_TYPE_FLASHLOADER, .start_address =        0, .end_address = 0x009FFF, .checksum_address = 0x009FFC },
    { .type = BLOCK_INFO_TYPE_APPLICATION, .start_address = 0x00A000, .end_address = 0x02AFFF, .checksum_address = 0x02AFFC },
    { .type = BLOCK_INFO_TYPE_INVALID,     .start_address =        0, .end_address =        0, .checksum_address =        0 }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

// --- block info --------------------------------------------------------------

const t_BLOCK_INFO * BlockInfoGet(void)
{
  return mock_block_info;
}

// --- crc ---------------------------------------------------------------------

void CrcInitCrc16(uint16_t * const crc)
{
  (void)crc;
}

void CrcUpdateCrc16(const uint8_t data[], const uint16_t nbr_of_bytes, uint16_t * const crc)
{
  (void)data;
  (void)nbr_of_bytes;
  (void)crc;
}

// --- flash -------------------------------------------------------------------

uint32_t FlashGetRowSize(void)
{
  return 256;
}

uint32_t FlashGetPageSize(void)
{
  return 2048;
}

e_ERROR_ID FlashAddressesOneOrMultipleRows(const uint32_t address, const uint32_t number_of_bytes)
{
  (void)address;
  (void)number_of_bytes;

  return ERROR_ID_SUCCESS;
}

e_ERROR_ID FlashAddressesOneOrMultiplePages(const uint32_t address, const uint32_t number_of_bytes)
{
  (void)address;
  (void)number_of_bytes;

  return ERROR_ID_SUCCESS;
}

e_ERROR_ID FlashAquire(const e_FLASH_DISABLE_INTERRUPTS action)
{
  (void)action;

  return ERROR_ID_SUCCESS;
}

void FlashRelease(void)
{
}

e_ERROR_ID FlashRead(const uint32_t address, const uint16_t number_of_bytes, uint8_t * const data)
{
  (void)address;
  (void)number_of_bytes;
  (void)data;

  return ERROR_ID_SUCCESS;
}

e_ERROR_ID FlashBlankCheckPage(const uint32_t address)
{
  (void)address;

  return ERROR_ID_FLASH_NOT_EMPTY;
}

e_ERROR_ID FlashErasePage(const uint32_t address)
{
  (void)address;

  mock.flash_erase_counter++;

  return ERROR_ID_SUCCESS;
}

e_ERROR_ID FlashWriteRow(const uint32_t address, const uint8_t data[])
{
  (void)address;
  (void)data;

  mock.flash_write_counter++;

  return ERROR_ID_SUCCESS;
}

void MockResetCounters(void)
{
  mock.flash_erase_counter = 0;
  mock.flash_write_counter = 0;
}

uint32_t MockGetFlashEraseCounter(void)
{
  return mock.flash_erase_counter;
}

uint32_t MockGetFlashWriteCounter(void)
{
  return mock.flash_write_counter;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
