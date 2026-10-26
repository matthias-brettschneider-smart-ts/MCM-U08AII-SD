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

#define CONTROLLER_DSPIC33FJ256GP710_SUPPORTED
#define CONTROLLER_DSPIC33EP128GS804_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP508_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP506_SUPPORTED

#include "hal/IController.h"
#include "hal/IInterrupt.h"
#include "hal/IFlash.h"
#include "hal/ISemaphore.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"
#include "middleware/IWait.h"

#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
#include "FlashF.h"
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
#include "FlashE.h"
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
#include "FlashC.h"
#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef struct {
  t_SEMAPHORE_MUTEX mutex;
  e_FLASH_DISABLE_INTERRUPTS disable_interrupts;
} t_FLASH;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_FLASH flash;

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static e_ERROR_ID FlashAddressesWithinOnePages(const uint32_t address, const uint32_t number_of_bytes);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get the size of a single row.
 *
 -----------------------------------------------------------------------------*/
uint32_t FlashGetRowSize(void)
{
  return FLASH_SIZE_OF_ROW_IN_BYTE;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get the size of a single page.
 *
 -----------------------------------------------------------------------------*/
uint32_t FlashGetPageSize(void)
{
  return FLASH_SIZE_OF_PAGE_IN_BYTE;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Check whether memory range is accessible row-wise.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID FlashAddressesOneOrMultipleRows(const uint32_t address, const uint32_t number_of_bytes)
{
  const bool invalid_address = (address % FLASH_SIZE_OF_ROW_IN_BYTE) != 0;
  const bool invalid_length = (number_of_bytes == 0) || ((number_of_bytes % FLASH_SIZE_OF_ROW_IN_BYTE) != 0);
  return invalid_address ? ERROR_ID_FLASH_INVALID_ADDRESS : invalid_length ? ERROR_ID_FLASH_INVALID_LENGTH : ERROR_ID_SUCCESS;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Check whether memory range is accessible page-wise.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID FlashAddressesOneOrMultiplePages(const uint32_t address, const uint32_t number_of_bytes)
{
  const bool invalid_address = (address % FLASH_SIZE_OF_PAGE_IN_BYTE) != 0;
  const bool invalid_length = (number_of_bytes == 0) || ((number_of_bytes % FLASH_SIZE_OF_PAGE_IN_BYTE) != 0);
  return invalid_address ? ERROR_ID_FLASH_INVALID_ADDRESS : invalid_length ? ERROR_ID_FLASH_INVALID_LENGTH : ERROR_ID_SUCCESS;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Check whether memory range is within one page
 *
 -----------------------------------------------------------------------------*/
static e_ERROR_ID FlashAddressesWithinOnePages(const uint32_t address, const uint32_t number_of_bytes)
{
  const uint32_t address_page_start = address - (address % FLASH_SIZE_OF_PAGE_IN_BYTE);
  const uint32_t address_end = address + number_of_bytes - 1;
  const bool invalid_length = (number_of_bytes == 0) || (address_end >= (address_page_start + FLASH_SIZE_OF_PAGE_IN_BYTE));
  return invalid_length ? ERROR_ID_FLASH_INVALID_LENGTH : ERROR_ID_SUCCESS;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Try to allocate the flash and determine how interrupts have to be handled during write & erase.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID FlashAquire(const e_FLASH_DISABLE_INTERRUPTS action)
{
  const bool got_it = SEMAPHORE_TRY_TO_LOCK_MUTEX(&flash.mutex);
  if (got_it)
    flash.disable_interrupts = action;

  return got_it ? ERROR_ID_SUCCESS : ERROR_ID_TEMPORARILY_UNAVAILABLE;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Release the flash.
 *
 -----------------------------------------------------------------------------*/
void FlashRelease(void)
{
  SEMAPHORE_RELEASE_MUTEX(&flash.mutex);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void FlashInternalPreEraseAndWrite(void)
{
  if ((flash.disable_interrupts == FLASH_DISABLE_INTERRUPTS_DURING_ERASE_AND_WRITE_ONLY) ||
      (flash.disable_interrupts == FLASH_DISABLE_INTERRUPTS_PERMANENTLY))
    INTERRUPT_DISABLE();
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void FlashInternalPostEraseAndWrite(void)
{
  if (flash.disable_interrupts == FLASH_DISABLE_INTERRUPTS_DURING_ERASE_AND_WRITE_ONLY)
    INTERRUPT_ENABLE();
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Read 32 bit from the given address.
 *
 -----------------------------------------------------------------------------*/
uint32_t FlashRead32Bit(const uint32_t address)
{
  return FLASH_INTERNAL_READ_32_BIT(address);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Write 32 bit to the given address.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID FlashWrite32Bit(const uint32_t address, const uint32_t value)
{
  return FLASH_INTERNAL_WRITE_32_BIT(address, value);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Write 64 bit to the given address.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID FlashWrite64Bit(const uint32_t address, const uint32_t value[])
{
  return FLASH_INTERNAL_WRITE_64_BIT(address, value);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Read memory block.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID FlashRead(const uint32_t address, const uint16_t number_of_bytes, uint8_t * const data)
{
  e_ERROR_ID error = ERROR_ID_SUCCESS;

  uint32_t address_temp = address;
  uint32_t number_of_bytes_temp = number_of_bytes;
  uint8_t * data_temp = data;

  while ((number_of_bytes_temp > 0) && (error == ERROR_ID_SUCCESS)) {

    const uint32_t bytes_within_page = FLASH_SIZE_OF_PAGE_IN_BYTE - (address_temp % FLASH_SIZE_OF_PAGE_IN_BYTE);
    const uint32_t bytes_to_read = (bytes_within_page > number_of_bytes_temp) ? number_of_bytes_temp : bytes_within_page;

    error = FlashAddressesWithinOnePages(address_temp, number_of_bytes_temp);
    if (error == ERROR_ID_SUCCESS)
      error = FLASH_INTERNAL_READ_WITHIN_PAGE(address_temp, number_of_bytes_temp, data_temp);

    address_temp += bytes_to_read;
    data_temp += bytes_to_read;
    number_of_bytes_temp -= bytes_to_read;
  }

  return error;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Do blank check for single flash page. Address has to be page aligned.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID FlashBlankCheckPage(const uint32_t address)
{
  e_ERROR_ID error = FlashAddressesOneOrMultiplePages(address, FLASH_SIZE_OF_PAGE_IN_BYTE);
  if (error == ERROR_ID_SUCCESS) {

    uint32_t address_temp = address;
    uint32_t number_of_words = FLASH_SIZE_OF_PAGE_IN_BYTE / FLASH_SIZE_OF_WORD_IN_BYTE;

    while ((error == ERROR_ID_SUCCESS) && (number_of_words > 0)) {
      const uint32_t word = FlashRead32Bit(address_temp);
      if (word != FLASH_ERASE_VALUE)
        error = ERROR_ID_FLASH_NOT_EMPTY;
      address_temp += FLASH_SIZE_OF_WORD_IN_BYTE;
      number_of_words--;
    }
  }

  return error;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Erase single flash page.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID FlashErasePage(const uint32_t address)
{
  e_ERROR_ID error = FlashAddressesOneOrMultiplePages(address, FLASH_SIZE_OF_PAGE_IN_BYTE);
  if (error == ERROR_ID_SUCCESS)
    error = FLASH_INTERNAL_ERASE_PAGE(address);

  return error;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Write one row within flash page.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID FlashWriteRow(const uint32_t address, const uint8_t data[])
{
  e_ERROR_ID error = FlashAddressesOneOrMultipleRows(address, FLASH_SIZE_OF_ROW_IN_BYTE);
  if (error == ERROR_ID_SUCCESS)
    error = FLASH_INTERNAL_WRITE_ROW(address, data);

  return error;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
