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
#include "hal/IOscillator.h"
#include "hal/ITimer.h"
#include "middleware/IAssert.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

#define SYSTEM_TIMER                                                           4

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint32_t SYSTEM_TIMER_GET_FREQUENCY_IN_HZ(void)
{
  const uint32_t system_timer_get_frequency_in_hz =
      (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000) ? 40000000 :
      (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000) ? 40000000 :
      (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000) ? 50000000 :
      0;

  ASSERT_FATAL(system_timer_get_frequency_in_hz != 0);

  return system_timer_get_frequency_in_hz;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Convert absolute time into timer ticks.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint16_t SYSTEM_TIMER_GET_TICKS_FOR_US_16(const uint16_t us)
{
  ASSERT_FATAL((SYSTEM_TIMER_GET_FREQUENCY_IN_HZ() % 1000000) == 0);

  return (SYSTEM_TIMER_GET_FREQUENCY_IN_HZ() / 1000000) * us;
}
COMPILER_INLINE uint32_t SYSTEM_TIMER_GET_TICKS_FOR_US_32(const uint32_t us)
{
  ASSERT_FATAL((SYSTEM_TIMER_GET_FREQUENCY_IN_HZ() % 1000000) == 0);

  return (SYSTEM_TIMER_GET_FREQUENCY_IN_HZ() / 1000000) * us;
}
COMPILER_INLINE uint32_t SYSTEM_TIMER_GET_TICKS_FOR_MS_32(const uint16_t ms)
{
  ASSERT_FATAL((SYSTEM_TIMER_GET_FREQUENCY_IN_HZ() % 1000) == 0);

  return (SYSTEM_TIMER_GET_FREQUENCY_IN_HZ() / 1000) * (uint32_t)ms;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get current timer value without blocking interrupts.
 *
 * (Reading 32 bit timer only works if execution is not interrupted for too long.)
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint16_t SYSTEM_TIMER_GET_TICKS_16(void)
{
  return TIMER_GET_VALUE_16(SYSTEM_TIMER);
}
COMPILER_INLINE uint32_t SYSTEM_TIMER_GET_TICKS_32(void)
{
  typedef union {
    uint16_t u16[2];
    uint32_t u32;
  } t_SYSTEM_TIMER;
  ASSERT_STATIC(sizeof(t_SYSTEM_TIMER) == 4);

  t_SYSTEM_TIMER timer;
  timer.u16[0] = TIMER_GET_VALUE_16(SYSTEM_TIMER);
  timer.u16[1] = TIMER_GET_VALUE_16_UPPER(SYSTEM_TIMER);

  // lower bits have been read before they ran over but upper bits have been read after their increment -> fix them
  if (TIMER_GET_VALUE_16(SYSTEM_TIMER) < timer.u16[0])
    if (TIMER_GET_VALUE_16_UPPER(SYSTEM_TIMER) == timer.u16[1])
      timer.u16[1]--;

  return timer.u32;
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
