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

#define INT_ISR_INTERNAL(number)                         _INT##number##Interrupt

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void INT_ENABLE(const int interrupt)
{
  switch (interrupt) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: _INT0IE = 1; break;
    case 1: _INT1IE = 1; break;
    case 2: _INT2IE = 1; break;
    case 3: _INT3IE = 1; break;
    case 4: _INT4IE = 1; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: _INT0IE = 1; break;
    case 1: _INT1IE = 1; break;
    case 2: _INT2IE = 1; break;
    case 4: _INT4IE = 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: _INT0IE = 1; break;
    case 1: _INT1IE = 1; break;
    case 2: _INT2IE = 1; break;
    case 3: _INT3IE = 1; break;
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
COMPILER_INLINE void INT_DISABLE(const int interrupt)
{
  switch (interrupt) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: _INT0IE = 0; break;
    case 1: _INT1IE = 0; break;
    case 2: _INT2IE = 0; break;
    case 3: _INT3IE = 0; break;
    case 4: _INT4IE = 0; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: _INT0IE = 0; break;
    case 1: _INT1IE = 0; break;
    case 2: _INT2IE = 0; break;
    case 4: _INT4IE = 0; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: _INT0IE = 0; break;
    case 1: _INT1IE = 0; break;
    case 2: _INT2IE = 0; break;
    case 3: _INT3IE = 0; break;
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
COMPILER_INLINE void INT_SET_POLARITY(const int interrupt, const bool high_active)
{
  switch (interrupt) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: _INT0EP = high_active ? 0 : 1; break;
    case 1: _INT1EP = high_active ? 0 : 1; break;
    case 2: _INT2EP = high_active ? 0 : 1; break;
    case 3: _INT3EP = high_active ? 0 : 1; break;
    case 4: _INT4EP = high_active ? 0 : 1; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: _INT0EP = high_active ? 0 : 1; break;
    case 1: _INT1EP = high_active ? 0 : 1; break;
    case 2: _INT2EP = high_active ? 0 : 1; break;
    case 4: _INT4EP = high_active ? 0 : 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: _INT0EP = high_active ? 0 : 1; break;
    case 1: _INT1EP = high_active ? 0 : 1; break;
    case 2: _INT2EP = high_active ? 0 : 1; break;
    case 3: _INT3EP = high_active ? 0 : 1; break;
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
COMPILER_INLINE void INT_SET_PRIORITY(const int interrupt, const uint8_t priority)
{
  switch (interrupt) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: _INT0IP = priority; break;
    case 1: _INT1IP = priority; break;
    case 2: _INT2IP = priority; break;
    case 3: _INT3IP = priority; break;
    case 4: _INT4IP = priority; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: _INT0IP = priority; break;
    case 1: _INT1IP = priority; break;
    case 2: _INT2IP = priority; break;
    case 4: _INT4IP = priority; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: _INT0IP = priority; break;
    case 1: _INT1IP = priority; break;
    case 2: _INT2IP = priority; break;
    case 3: _INT3IP = priority; break;
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
COMPILER_INLINE void INT_CONFIRM(const int interrupt)
{
  switch (interrupt) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: _INT0IF = 0; break;
    case 1: _INT1IF = 0; break;
    case 2: _INT2IF = 0; break;
    case 3: _INT3IF = 0; break;
    case 4: _INT4IF = 0; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: _INT0IF = 0; break;
    case 1: _INT1IF = 0; break;
    case 2: _INT2IF = 0; break;
    case 4: _INT4IF = 0; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: _INT0IF = 0; break;
    case 1: _INT1IF = 0; break;
    case 2: _INT2IF = 0; break;
    case 3: _INT3IF = 0; break;
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
