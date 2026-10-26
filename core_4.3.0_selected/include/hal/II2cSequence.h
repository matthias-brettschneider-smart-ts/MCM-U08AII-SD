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
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

typedef enum {
  I2C_SEQUENCE_MODE_READ_WITHOUT_DATA_ADDRESS,
  I2C_SEQUENCE_MODE_READ_WITH_ONE_BYTE_DATA_ADDRESS,
  I2C_SEQUENCE_MODE_READ_WITH_TWO_BYTE_DATA_ADDRESS,
  I2C_SEQUENCE_MODE_WRITE_WITHOUT_DATA_ADDRESS,
  I2C_SEQUENCE_MODE_WRITE_WITH_ONE_BYTE_DATA_ADDRESS,
  I2C_SEQUENCE_MODE_WRITE_WITH_TWO_BYTE_DATA_ADDRESS
} e_I2C_SEQUENCE_MODE;

typedef enum {
  I2C_SEQUENCE_STATE_IN_PROGRESS,
  I2C_SEQUENCE_STATE_SUCCEEDED,
  I2C_SEQUENCE_STATE_FAILED
} e_I2C_SEQUENCE_STATE;

typedef struct I2C_SEQUENCE_INTERNAL t_I2C_SEQUENCE;

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

void I2cSequenceStart(t_I2C_SEQUENCE * const sequence, const uint8_t device_address, const uint16_t data_address, const uint16_t number_of_bytes, uint8_t data[]);

COMPILER_INLINE bool I2C_SEQUENCE_UPDATE(const int node, const e_I2C_SEQUENCE_MODE mode, t_I2C_SEQUENCE * const sequence);

e_I2C_SEQUENCE_STATE I2cSequenceGetState(t_I2C_SEQUENCE * const sequence);

/*============================================================================*/
/*  I M P L E M E N T A T I O N                                               */
/*============================================================================*/

#include "I2cSequenceImpl.h"

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
