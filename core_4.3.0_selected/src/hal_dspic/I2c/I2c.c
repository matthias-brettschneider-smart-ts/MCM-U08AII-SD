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
#include "hal/II2c.h"
#include "hal/IOscillator.h"
#include "hal/ISemaphore.h"
#include "middleware/IAssert.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804))
#define I2C_NUMBER_OF_NODES                                                    2
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
#define I2C_NUMBER_OF_NODES                                                    3
#endif

typedef struct {
  t_SEMAPHORE_MUTEX mutex[I2C_NUMBER_OF_NODES];
} t_I2C;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_I2C i2c;

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
 * Try to allocate the given node.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID I2cAcquire(const int node)
{
  const bool got_it = SEMAPHORE_TRY_TO_LOCK_MUTEX((t_SEMAPHORE_MUTEX * const)&i2c.mutex[node]);
  return got_it ? ERROR_ID_SUCCESS : ERROR_ID_TEMPORARILY_UNAVAILABLE;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Release the given node.
 *
 -----------------------------------------------------------------------------*/
void I2cRelease(const int node)
{
  SEMAPHORE_RELEASE_MUTEX((t_SEMAPHORE_MUTEX * const)&i2c.mutex[node]);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void I2cSetBitrate(const int node, const e_I2C_BITRATE bitrate)
{
  const bool slew_rate_control = bitrate == I2C_BITRATE_400000_BPS_FAST;
  const uint16_t baudrate =
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
      ((OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000) && (bitrate == I2C_BITRATE_400000_BPS_FAST)) ? 92 :
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
      ((OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000) && (bitrate == I2C_BITRATE_100000_BPS_STANDARD)) ? 195 :
      ((OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000) && (bitrate == I2C_BITRATE_400000_BPS_FAST)) ? 45 :
      ((OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000) && (bitrate == I2C_BITRATE_1000000_BPS_FAST_PLUS)) ? 15 :
      ((OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000) && (bitrate == I2C_BITRATE_100000_BPS_STANDARD)) ? 392 :
      ((OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000) && (bitrate == I2C_BITRATE_400000_BPS_FAST)) ? 92 :
      ((OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000) && (bitrate == I2C_BITRATE_1000000_BPS_FAST_PLUS)) ? 32 :
      ((OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000) && (bitrate == I2C_BITRATE_100000_BPS_STANDARD)) ? 491 :
      ((OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000) && (bitrate == I2C_BITRATE_400000_BPS_FAST)) ? 116 :
      ((OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000) && (bitrate == I2C_BITRATE_1000000_BPS_FAST_PLUS)) ? 41 :
#endif
      0;

  ASSERT_FATAL(baudrate != 0);

  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: I2C1BRG = baudrate; I2C1CONbits.DISSLW = slew_rate_control ? 0 : 1; break;
    case 1: I2C2BRG = baudrate; I2C2CONbits.DISSLW = slew_rate_control ? 0 : 1; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: I2C1BRG = baudrate; I2C1CONLbits.DISSLW = slew_rate_control ? 0 : 1; break;
    case 1: I2C2BRG = baudrate; I2C2CONLbits.DISSLW = slew_rate_control ? 0 : 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: I2C1BRG = baudrate; I2C1CONLbits.DISSLW = slew_rate_control ? 0 : 1; break;
    case 1: I2C2BRG = baudrate; I2C2CONLbits.DISSLW = slew_rate_control ? 0 : 1; break;
    case 2: I2C3BRG = baudrate; I2C3CONLbits.DISSLW = slew_rate_control ? 0 : 1; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void I2cSetSizeOfDeviceAddress(const int node, const t_I2C_SIZE_OF_DEVICE_ADDRESS size_of_device_address)
{
  ASSERT_FATAL((size_of_device_address == I2C_SIZE_OF_DEVICE_ADDRESS_7_BIT) || (size_of_device_address == I2C_SIZE_OF_DEVICE_ADDRESS_10_BIT));

  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: I2C1CONbits.A10M = (size_of_device_address == I2C_SIZE_OF_DEVICE_ADDRESS_10_BIT) ? 1 : 0; break;
    case 1: I2C2CONbits.A10M = (size_of_device_address == I2C_SIZE_OF_DEVICE_ADDRESS_10_BIT) ? 1 : 0; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: I2C1CONLbits.A10M = (size_of_device_address == I2C_SIZE_OF_DEVICE_ADDRESS_10_BIT) ? 1 : 0; break;
    case 1: I2C2CONLbits.A10M = (size_of_device_address == I2C_SIZE_OF_DEVICE_ADDRESS_10_BIT) ? 1 : 0; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: I2C1CONLbits.A10M = (size_of_device_address == I2C_SIZE_OF_DEVICE_ADDRESS_10_BIT) ? 1 : 0; break;
    case 1: I2C2CONLbits.A10M = (size_of_device_address == I2C_SIZE_OF_DEVICE_ADDRESS_10_BIT) ? 1 : 0; break;
    case 2: I2C3CONLbits.A10M = (size_of_device_address == I2C_SIZE_OF_DEVICE_ADDRESS_10_BIT) ? 1 : 0; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void I2cDisable(const int node)
{
  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: I2C1CONbits.I2CEN = 0; break;
    case 1: I2C2CONbits.I2CEN = 0; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: I2C1CONLbits.I2CEN = 0; break;
    case 1: I2C2CONLbits.I2CEN = 0; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: I2C1CONLbits.I2CEN = 0; break;
    case 1: I2C2CONLbits.I2CEN = 0; break;
    case 2: I2C3CONLbits.I2CEN = 0; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void I2cEnable(const int node)
{
  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: I2C1CONbits.I2CEN = 1; break;
    case 1: I2C2CONbits.I2CEN = 1; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: I2C1CONLbits.I2CEN = 1; break;
    case 1: I2C2CONLbits.I2CEN = 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: I2C1CONLbits.I2CEN = 1; break;
    case 1: I2C2CONLbits.I2CEN = 1; break;
    case 2: I2C3CONLbits.I2CEN = 1; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool I2cIsActive(const int node)
{
  bool active = false;

  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: active = I2C1CONbits.I2CEN == 1; break;
    case 1: active = I2C2CONbits.I2CEN == 1; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: active = I2C1CONLbits.I2CEN == 1; break;
    case 1: active = I2C2CONLbits.I2CEN == 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: active = I2C1CONLbits.I2CEN == 1; break;
    case 1: active = I2C2CONLbits.I2CEN == 1; break;
    case 2: active = I2C3CONLbits.I2CEN == 1; break;
#endif
    default: ASSERT_FATAL(false); break;
  }

  return active;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
