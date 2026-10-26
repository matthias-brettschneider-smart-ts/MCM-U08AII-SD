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

#define CONTROLLER_DSPIC33FJ256GP710_SUPPORTED
#define CONTROLLER_DSPIC33EP128GS804_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP508_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP506_SUPPORTED

#include "hal/ICompiler.h"
#include "hal/IController.h"
#include "middleware/IAssert.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool I2C_GOT_INTERRUPT(const int node)
{
  bool got_interrupt = false;
  switch (node) {
    case 0: got_interrupt = _MI2C1IF != 0; if (got_interrupt) _MI2C1IF = 0; break;
    case 1: got_interrupt = _MI2C2IF != 0; if (got_interrupt) _MI2C2IF = 0; break;
#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 2: got_interrupt = _MI2C3IF != 0; if (got_interrupt) _MI2C3IF = 0; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
  return got_interrupt;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void I2C_SEND_START_CONDITION(const int node)
{
  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: I2C1CONbits.SEN = 1; break;
    case 1: I2C2CONbits.SEN = 1; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: I2C1CONLbits.SEN = 1; break;
    case 1: I2C2CONLbits.SEN = 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: I2C1CONLbits.SEN = 1; break;
    case 1: I2C2CONLbits.SEN = 1; break;
    case 2: I2C3CONLbits.SEN = 1; break;
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
COMPILER_INLINE void I2C_REPEATE_START_CONDITION(const int node)
{
  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: I2C1CONbits.RSEN = 1; break;
    case 1: I2C2CONbits.RSEN = 1; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: I2C1CONLbits.RSEN = 1; break;
    case 1: I2C2CONLbits.RSEN = 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: I2C1CONLbits.RSEN = 1; break;
    case 1: I2C2CONLbits.RSEN = 1; break;
    case 2: I2C3CONLbits.RSEN = 1; break;
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
COMPILER_INLINE void I2C_SEND_STOP_CONDITION(const int node)
{
  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: I2C1CONbits.PEN = 1; break;
    case 1: I2C2CONbits.PEN = 1; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: I2C1CONLbits.PEN = 1; break;
    case 1: I2C2CONLbits.PEN = 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: I2C1CONLbits.PEN = 1; break;
    case 1: I2C2CONLbits.PEN = 1; break;
    case 2: I2C3CONLbits.PEN = 1; break;
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
COMPILER_INLINE void I2C_START_TRANSMISSION(const int node, const uint8_t value)
{
  switch (node) {
    case 0: I2C1TRN = (uint16_t)value; break;
    case 1: I2C2TRN = (uint16_t)value; break;
#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 2: I2C3TRN = (uint16_t)value; break;
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
COMPILER_INLINE bool I2C_GOT_ACKNOWLEDGE(const int node)
{
  bool got_acknowledge = false;
  switch (node) {
    case 0: got_acknowledge = I2C1STATbits.ACKSTAT == 0; break;
    case 1: got_acknowledge = I2C2STATbits.ACKSTAT == 0; break;
#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 2: got_acknowledge = I2C3STATbits.ACKSTAT == 0; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
  return got_acknowledge;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void I2C_START_RECEPTION(const int node)
{
  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: I2C1CONbits.RCEN = 1; break;
    case 1: I2C2CONbits.RCEN = 1; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: I2C1CONLbits.RCEN = 1; break;
    case 1: I2C2CONLbits.RCEN = 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: I2C1CONLbits.RCEN = 1; break;
    case 1: I2C2CONLbits.RCEN = 1; break;
    case 2: I2C3CONLbits.RCEN = 1; break;
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
COMPILER_INLINE uint8_t I2C_GET_RECEIVED(const int node)
{
  uint8_t value = 0;
  switch (node) {
    case 0: value = (uint8_t)I2C1RCV; break;
    case 1: value = (uint8_t)I2C2RCV; break;
#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 2: value = (uint8_t)I2C3RCV; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
  return value;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void I2C_SEND_ACKNOWLEDGE(const int node, const bool positive)
{
  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: I2C1CONbits.ACKDT = positive ? 0 : 1; I2C1CONbits.ACKEN = 1; break;
    case 1: I2C2CONbits.ACKDT = positive ? 0 : 1; I2C2CONbits.ACKEN = 1; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: I2C1CONLbits.ACKDT = positive ? 0 : 1; I2C1CONLbits.ACKEN = 1; break;
    case 1: I2C2CONLbits.ACKDT = positive ? 0 : 1; I2C2CONLbits.ACKEN = 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: I2C1CONLbits.ACKDT = positive ? 0 : 1; I2C1CONLbits.ACKEN = 1; break;
    case 1: I2C2CONLbits.ACKDT = positive ? 0 : 1; I2C2CONLbits.ACKEN = 1; break;
    case 2: I2C3CONLbits.ACKDT = positive ? 0 : 1; I2C3CONLbits.ACKEN = 1; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
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
