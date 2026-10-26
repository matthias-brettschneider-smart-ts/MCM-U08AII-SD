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
#include "os/ISystemTimer.h"
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
 * Convert absolute time into timer ticks.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint16_t WAIT_GET_TICKS_FOR_US_16(const uint16_t time_in_us)
{
  return SYSTEM_TIMER_GET_TICKS_FOR_US_16(time_in_us);
}
COMPILER_INLINE uint32_t WAIT_GET_TICKS_FOR_US_32(const uint32_t time_in_us)
{
  return SYSTEM_TIMER_GET_TICKS_FOR_US_32(time_in_us);
}
COMPILER_INLINE uint32_t WAIT_GET_TICKS_FOR_MS_32(const uint16_t time_in_ms)
{
  return SYSTEM_TIMER_GET_TICKS_FOR_MS_32(time_in_ms);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get current timer value in ticks.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint16_t WAIT_GET_TIMER_VALUE_16(void)
{
  return SYSTEM_TIMER_GET_TICKS_16();
}
COMPILER_INLINE uint32_t WAIT_GET_TIMER_VALUE_32(void)
{
  return SYSTEM_TIMER_GET_TICKS_32();
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Check weather time is over relative to the given timer value.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool WAIT_CHECK_TIMEOUT_US_16(const uint16_t reference_ticks, const uint16_t time_in_us)
{
  return (WAIT_GET_TIMER_VALUE_16() - reference_ticks) >= WAIT_GET_TICKS_FOR_US_16(time_in_us);
}
COMPILER_INLINE bool WAIT_CHECK_TIMEOUT_US_32(const uint32_t reference_ticks, const uint32_t time_in_us)
{
  return (WAIT_GET_TIMER_VALUE_32() - reference_ticks) >= WAIT_GET_TICKS_FOR_US_32(time_in_us);
}
COMPILER_INLINE bool WAIT_CHECK_TIMEOUT_MS_32(const uint32_t reference_ticks, const uint32_t time_in_ms)
{
  return (WAIT_GET_TIMER_VALUE_32() - reference_ticks) >= WAIT_GET_TICKS_FOR_MS_32(time_in_ms);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Wait time relative to the given timer value (without blocking interrupts).
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void WAIT_US_16(const uint16_t reference_ticks, const uint16_t time_in_us)
{
  while (!WAIT_CHECK_TIMEOUT_US_16(reference_ticks, time_in_us));
}
COMPILER_INLINE void WAIT_US_32(const uint32_t reference_ticks, const uint32_t time_in_us)
{
  while (!WAIT_CHECK_TIMEOUT_US_32(reference_ticks, time_in_us));
}
COMPILER_INLINE void WAIT_MS_32(const uint32_t reference_ticks, const uint32_t time_in_ms)
{
  while (!WAIT_CHECK_TIMEOUT_MS_32(reference_ticks, time_in_ms));
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Wait time relative to the function call (without blocking interrupts).
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void WAIT_US(const uint16_t time_in_us)
{
  WAIT_US_16(WAIT_GET_TIMER_VALUE_16(), time_in_us);
}
COMPILER_INLINE void WAIT_MS(const uint16_t time_in_ms)
{
  WAIT_MS_32(WAIT_GET_TIMER_VALUE_32(), time_in_ms);
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
