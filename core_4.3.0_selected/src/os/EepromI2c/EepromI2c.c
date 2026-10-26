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

#include "hal/II2c.h"
#include "hal/II2cSequence.h"
#include "middleware/IAssert.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"
#include "middleware/IWait.h"
#include "os/IEeprom.h"

#ifndef EEPROM_I2C_NODE
#error "error compile flags - EEPROM_I2C_NODE undefined"
#elif ((EEPROM_I2C_NODE < 0) || (EEPROM_I2C_NODE > 2))
#error "error compile flags - EEPROM_I2C_NODE invalid"
#endif

#ifndef EEPROM_I2C_BITRATE_IN_KBPS
#error "error compile flags - EEPROM_I2C_BITRATE_IN_KBPS undefined"
#elif ((EEPROM_I2C_BITRATE_IN_KBPS != 400000) && (EEPROM_I2C_BITRATE_IN_KBPS != 1000000))
#error "error compile flags - EEPROM_I2C_BITRATE_IN_KBPS invalid"
#endif

#ifndef EEPROM_I2C_DEVICE_ADDRESS
#error "error compile flags - EEPROM_I2C_DEVICE_ADDRESS undefined"
#elif ((EEPROM_I2C_DEVICE_ADDRESS == 0) || (EEPROM_I2C_DEVICE_ADDRESS > 0x7F))
#error "error compile flags - EEPROM_I2C_DEVICE_ADDRESS invalid"
#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define EEPROM_SIZE_OF_PHYSICAL_PAGE_IN_BYTE                                  32
#define EEPROM_NUMBER_OF_PHYSICAL_PAGES                                      128

#define EEPROM_WRITE_CYCLE_TIME_IN_MS                                          5

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static e_ERROR_ID EepromAccessPages(const bool read, uint16_t address, uint8_t * data, uint16_t number_of_bytes);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Determine the size of one page.
 *
 -----------------------------------------------------------------------------*/
uint16_t EepromGetPageSize(void)
{
  return EEPROM_SIZE_OF_PHYSICAL_PAGE_IN_BYTE;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Determine the available memory size.
 *
 -----------------------------------------------------------------------------*/
uint16_t EepromGetEepromSize(void)
{
  return EEPROM_SIZE_OF_PHYSICAL_PAGE_IN_BYTE * EEPROM_NUMBER_OF_PHYSICAL_PAGES;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID EepromInit(void)
{
  const e_I2C_BITRATE bitrate =
      (EEPROM_I2C_BITRATE_IN_KBPS == 400000) ? I2C_BITRATE_400000_BPS_FAST :
          (EEPROM_I2C_BITRATE_IN_KBPS == 1000000) ? I2C_BITRATE_1000000_BPS_FAST_PLUS :
              I2C_BITRATE_INVALID;

  I2cSetBitrate(EEPROM_I2C_NODE, bitrate);
  I2cSetSizeOfDeviceAddress(EEPROM_I2C_NODE, I2C_SIZE_OF_DEVICE_ADDRESS_7_BIT);
  I2cEnable(EEPROM_I2C_NODE);

  return ERROR_ID_SUCCESS;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Access page aligned byte block in EEPROM.
 *
 -----------------------------------------------------------------------------*/
static e_ERROR_ID EepromAccessPages(const bool read, uint16_t address, uint8_t * data, uint16_t number_of_bytes)
{
  ASSERT_FATAL((address % EEPROM_SIZE_OF_PHYSICAL_PAGE_IN_BYTE) == 0);
  ASSERT_FATAL((number_of_bytes % EEPROM_SIZE_OF_PHYSICAL_PAGE_IN_BYTE) == 0);
  ASSERT_FATAL(((uint32_t)address + number_of_bytes) <= ((uint32_t)EEPROM_NUMBER_OF_PHYSICAL_PAGES * EEPROM_SIZE_OF_PHYSICAL_PAGE_IN_BYTE));
  ASSERT_FATAL(data != NULL);

  e_ERROR_ID result = I2cAcquire(EEPROM_I2C_NODE);
  if (result == ERROR_ID_SUCCESS) {

    const uint16_t number_of_pages = number_of_bytes / EEPROM_SIZE_OF_PHYSICAL_PAGE_IN_BYTE;
    for (uint16_t page = 0; (page < number_of_pages) && (result == ERROR_ID_SUCCESS); page++) {

      const uint16_t offset = page * EEPROM_SIZE_OF_PHYSICAL_PAGE_IN_BYTE;

      t_I2C_SEQUENCE sequence;
      I2cSequenceStart(&sequence, EEPROM_I2C_DEVICE_ADDRESS, address + offset, EEPROM_SIZE_OF_PHYSICAL_PAGE_IN_BYTE, &data[offset]);

      if (read)
        while (I2C_SEQUENCE_UPDATE(EEPROM_I2C_NODE, I2C_SEQUENCE_MODE_READ_WITH_TWO_BYTE_DATA_ADDRESS, &sequence));
      else {
        while (I2C_SEQUENCE_UPDATE(EEPROM_I2C_NODE, I2C_SEQUENCE_MODE_WRITE_WITH_TWO_BYTE_DATA_ADDRESS, &sequence));
        WAIT_MS(EEPROM_WRITE_CYCLE_TIME_IN_MS);
      }

      result = (I2cSequenceGetState(&sequence) == I2C_SEQUENCE_STATE_SUCCEEDED) ? ERROR_ID_SUCCESS : ERROR_ID_FAILED;
    }

    I2cRelease(EEPROM_I2C_NODE);
  }

  return result;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Wrapper for reading page aligned byte block from EEPROM.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID EepromRead(uint16_t address, uint8_t * data, uint16_t number_of_bytes)
{
  return EepromAccessPages(true, address, data, number_of_bytes);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Wrapper for writing page aligned byte block into EEPROM.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID EepromWrite(uint16_t address, const uint8_t * data, uint16_t number_of_bytes)
{
  return EepromAccessPages(false, address, (uint8_t *)data, number_of_bytes);
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
