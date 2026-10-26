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

#include "IMock.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

COMPILER_INLINE bool I2C_GOT_INTERRUPT(const int node)
{
  (void)node;
  MOCK_ADD_TO_TRANSMISSION(".");
  return true;
}

COMPILER_INLINE void I2C_SEND_START_CONDITION(const int node)
{
  (void)node;
  MOCK_ADD_TO_TRANSMISSION("[");
}

COMPILER_INLINE void I2C_REPEATE_START_CONDITION(const int node)
{
  (void)node;
  MOCK_ADD_TO_TRANSMISSION("|");
}

COMPILER_INLINE void I2C_SEND_STOP_CONDITION(const int node)
{
  (void)node;
  MOCK_ADD_TO_TRANSMISSION("]");
}

COMPILER_INLINE void I2C_START_TRANSMISSION(const int node, const uint8_t value)
{
  (void)node;
  MOCK_ADD_TO_TRANSMISSION(">%02X", value);
}

COMPILER_INLINE bool I2C_GOT_ACKNOWLEDGE(const int node)
{
  (void)node;
  MOCK_ADD_TO_TRANSMISSION("+");
  return true;
}

COMPILER_INLINE void I2C_START_RECEPTION(const int node)
{
  (void)node;
  MOCK_ADD_TO_TRANSMISSION("<");
}

COMPILER_INLINE uint8_t I2C_GET_RECEIVED(const int node)
{
  (void)node;
  const uint8_t data = 0x55;
  MOCK_ADD_TO_TRANSMISSION("%02X", data);
  return data;
}

COMPILER_INLINE void I2C_SEND_ACKNOWLEDGE(const int node, const bool positive)
{
  (void)node;
  MOCK_ADD_TO_TRANSMISSION("%s", positive ? "+" : "-");
}

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
