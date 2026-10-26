/*============================================================================*/
/* I N C L U D E S                                                            */
/*============================================================================*/

#include "os/IBlock.h"

#include "IMock.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

// --- block -------------------------------------------------------------------

e_ERROR_ID BlockErase(uint32_t address, const uint32_t number_of_bytes)
{
  return ((address == MOCK_ADDRESS_ERASEABLE) && (number_of_bytes == MOCK_NUMBER_OF_BYTES_ERASEABLE)) ? ERROR_ID_SUCCESS : ERROR_ID_FLASH_EXCEED_VALID_ADDRESS_RANGE;
}

e_ERROR_ID BlockRangeIsAvailableForWrite(const uint32_t address, const uint32_t number_of_bytes)
{
  return ((address == MOCK_ADDRESS_WRITEABLE) && (number_of_bytes == MOCK_NUMBER_OF_BYTES_WRITEABLE)) ? ERROR_ID_SUCCESS : ERROR_ID_FLASH_EXCEED_VALID_ADDRESS_RANGE;
}

e_ERROR_ID BlockWrite(uint32_t address, uint32_t number_of_bytes, uint8_t data[])
{
  return ((address == MOCK_ADDRESS_WRITEABLE) && (number_of_bytes == MOCK_NUMBER_OF_BYTES_WRITEABLE) && (data != NULL)) ? ERROR_ID_SUCCESS : ERROR_ID_FLASH_EXCEED_VALID_ADDRESS_RANGE;
}

e_ERROR_ID BlockCheck(uint32_t address, uint32_t number_of_bytes, uint16_t * const checksum)
{
  const bool valid = (address == MOCK_ADDRESS_CHECKABLE) && (number_of_bytes == MOCK_NUMBER_OF_BYTES_CHECKABLE) && (checksum != NULL);
  if (valid)
    *checksum = MOCK_CHECKSUM;

  return valid ? ERROR_ID_SUCCESS : ERROR_ID_FLASH_EXCEED_VALID_ADDRESS_RANGE;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
