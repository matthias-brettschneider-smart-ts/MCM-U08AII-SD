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

#include "IMock.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define MOCK_SUPPORT_DEBUG_OUTPUT                                          false

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

t_MOCK mock;

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

// --- i2c ---------------------------------------------------------------------

e_ERROR_ID I2cAcquire(const int node)
{
  ASSERT_FATAL(node == 0);
  return mock.i2c_not_available ? ERROR_ID_TEMPORARILY_UNAVAILABLE : ERROR_ID_SUCCESS;
}

void I2cRelease(const int node)
{
  ASSERT_FATAL(node == 0);
}

void I2cSetBitrate(const int node, const e_I2C_BITRATE bitrate)
{
  ASSERT_FATAL(node == 0);
  ASSERT_FATAL(bitrate == I2C_BITRATE_400000_BPS_FAST);
}

void I2cSetSizeOfDeviceAddress(const int node, const t_I2C_SIZE_OF_DEVICE_ADDRESS size_of_device_address)
{
  ASSERT_FATAL(node == 0);
  ASSERT_FATAL(size_of_device_address == I2C_SIZE_OF_DEVICE_ADDRESS_7_BIT);
}

void I2cEnable(const int node)
{
  (void)node;
}

void MockI2cAvailable(void)
{
  mock.i2c_not_available = false;
}

void MockI2cUnavailable(void)
{
  mock.i2c_not_available = true;
}

// --- i2c sequence ------------------------------------------------------------

void I2cSequenceStart(t_I2C_SEQUENCE * const sequence, const uint8_t device_address, const uint16_t data_address, const uint16_t number_of_bytes, uint8_t data[])
{
  (void)sequence;
  (void)data;

  MOCK_ADD_TO_LOG("start: device 0x%02X, address 0x%04X, 0x%04X bytes\n", device_address, data_address, number_of_bytes);
}

e_I2C_SEQUENCE_STATE I2cSequenceGetState(t_I2C_SEQUENCE * const sequence)
{
  (void)sequence;

  return mock.i2c_sequence_succeeds ? I2C_SEQUENCE_STATE_SUCCEEDED : I2C_SEQUENCE_STATE_FAILED;
}

void MockI2cSequenceSucceeds(void)
{
  mock.i2c_sequence_succeeds = true;
}

void MockI2cSequenceFails(void)
{
  mock.i2c_sequence_succeeds = false;
}

void MockResetLog(void)
{
  memset(mock.log, 0, sizeof(mock.log));
  mock.index = 0;
}

#if MOCK_SUPPORT_DEBUG_OUTPUT
void MockPrintLog(void)
{
  printf("%s\n", mock.log);
}
#endif

bool MockLogIsEqual(const char expected[], const size_t number_of_characters)
{
  ASSERT_FATAL(expected != NULL);

  bool equal = number_of_characters == mock.index;
  if (equal)
    equal = memcmp(expected, mock.log, number_of_characters) == 0;

#if MOCK_SUPPORT_DEBUG_OUTPUT
  if (!equal) {
    printf("EXPECTED: %3d - %s\n", (int)number_of_characters, expected);
    printf("GOT:      %3d - %s\n", (int)mock.index, mock.log);
  }
#endif

  return equal;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
