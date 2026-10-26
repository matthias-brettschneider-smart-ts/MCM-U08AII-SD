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
#include "middleware/IErrorId.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

typedef enum {
  I2C_BITRATE_100000_BPS_STANDARD,
  I2C_BITRATE_400000_BPS_FAST,
  I2C_BITRATE_1000000_BPS_FAST_PLUS,
  I2C_BITRATE_INVALID
} e_I2C_BITRATE;

typedef enum {
  I2C_SIZE_OF_DEVICE_ADDRESS_7_BIT,
  I2C_SIZE_OF_DEVICE_ADDRESS_10_BIT,
  I2C_SIZE_OF_DEVICE_ADDRESS_INVALID
} t_I2C_SIZE_OF_DEVICE_ADDRESS;

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

e_ERROR_ID I2cAcquire(const int node);
void I2cRelease(const int node);

void I2cSetBitrate(const int node, const e_I2C_BITRATE bitrate);
void I2cSetSizeOfDeviceAddress(const int node, const t_I2C_SIZE_OF_DEVICE_ADDRESS size_of_device_address);

void I2cDisable(const int node);
void I2cEnable(const int node);
bool I2cIsActive(const int node);

COMPILER_INLINE bool I2C_GOT_INTERRUPT(const int node);

COMPILER_INLINE void I2C_SEND_START_CONDITION(const int node);
COMPILER_INLINE void I2C_REPEATE_START_CONDITION(const int node);
COMPILER_INLINE void I2C_SEND_STOP_CONDITION(const int node);

COMPILER_INLINE void I2C_START_TRANSMISSION(const int node, const uint8_t value);
COMPILER_INLINE bool I2C_GOT_ACKNOWLEDGE(const int node);

COMPILER_INLINE void I2C_START_RECEPTION(const int node);
COMPILER_INLINE uint8_t I2C_GET_RECEIVED(const int node);
COMPILER_INLINE void I2C_SEND_ACKNOWLEDGE(const int node, const bool positive);

/*============================================================================*/
/*  I M P L E M E N T A T I O N                                               */
/*============================================================================*/

#include "I2cImpl.h"

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
