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

#include "hal/ICompiler.h"
#include "middleware/IAssert.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"
#include "os/IBlock.h"
#include "os/IFlashSession.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef struct {
  bool active;
  uint32_t target_address_bytes;
  uint32_t number_of_bytes_to_receive;
  uint32_t number_of_bytes_received;
  uint8_t index_of_next_transfer_data;
  uint16_t index_of_next_free_byte;
} t_FLASH_SESSION;

#define FLASH_SESSION_NUMBER_OF_BYTES_PER_FLASH_START                       2048

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_FLASH_SESSION flash_session;
COMPILER_ALIGNED_4_BYTE static uint8_t flash_session_data[FLASH_SESSION_NUMBER_OF_BYTES_PER_FLASH_START];

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Erase memory range.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID FlashSessionErase(const uint32_t address, const uint32_t number_of_bytes)
{
  return BlockErase(address, number_of_bytes);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get state of flash session.
 *
 -----------------------------------------------------------------------------*/
bool FlashSessionIsActive(void)
{
  return flash_session.active;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Start an new flash session (and interrupt an already running).
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID FlashSessionStart(const uint32_t address, const uint32_t number_of_bytes)
{
  const e_ERROR_ID error =
      (number_of_bytes > FLASH_SESSION_NUMBER_OF_BYTES_PER_FLASH_START) ? ERROR_ID_FLASH_INVALID_LENGTH :
          BlockRangeIsAvailableForWrite(address, number_of_bytes);

  // terminate flash session at any error, otherwise initialize buffer and control information and (re-)activate session
  if (error != ERROR_ID_SUCCESS)
    flash_session.active = false;
  else {
    memset(flash_session_data, 0, FLASH_SESSION_NUMBER_OF_BYTES_PER_FLASH_START);
    flash_session.target_address_bytes = address;
    flash_session.number_of_bytes_to_receive = number_of_bytes;
    flash_session.number_of_bytes_received = 0;
    flash_session.index_of_next_free_byte = 0;
    flash_session.index_of_next_transfer_data = 0;
    flash_session.active = true;
  }

  return error;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Handle segmented data transfers.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID FlashSessionTransfer(const uint8_t index, const uint8_t number_of_bytes, const uint8_t data[])
{
  ASSERT_FATAL(data != NULL);

  const e_ERROR_ID error =
      (!flash_session.active) ? ERROR_ID_FLASH_NO_ACTIVE_SESSION :
          (index != flash_session.index_of_next_transfer_data) ? ERROR_ID_FLASH_UNEXPECTED_INDEX :
              ((uint32_t)number_of_bytes > flash_session.number_of_bytes_to_receive) ? ERROR_ID_FLASH_TOO_MUCH_DATA:
                  ERROR_ID_SUCCESS;

  // terminate flash session at any error, otherwise copy payload and update control information
  if (error != ERROR_ID_SUCCESS)
    flash_session.active = false;
  else {
    memcpy(&flash_session_data[flash_session.index_of_next_free_byte], data, number_of_bytes);
    flash_session.index_of_next_free_byte += (uint16_t)number_of_bytes;
    flash_session.number_of_bytes_received += (uint32_t)number_of_bytes;
    flash_session.number_of_bytes_to_receive -= (uint32_t)number_of_bytes;
    flash_session.index_of_next_transfer_data++;
  }

  return error;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Finish a flash session by writing the received data.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID FlashSessionProgram(void)
{
  const e_ERROR_ID error =
      (!flash_session.active) ? ERROR_ID_FLASH_NO_ACTIVE_SESSION :
          (flash_session.number_of_bytes_to_receive != 0) ? ERROR_ID_FLASH_NOT_ENOUGH_DATA :
              BlockWrite(flash_session.target_address_bytes, flash_session.number_of_bytes_received, flash_session_data);

  // terminate flash session in all cases
  // - finishes the current session when everything was fine
  // - terminates the current session when trying to program data at the wrong time
  flash_session.active = false;

  return error;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Do checksum calculation for memory range.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID FlashSessionCheck(const uint32_t address, const uint32_t number_of_bytes, uint16_t * const checksum)
{
  return BlockCheck(address, number_of_bytes, checksum);
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
