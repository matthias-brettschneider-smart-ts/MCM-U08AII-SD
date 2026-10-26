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
#include "middleware/IWait.h"
#include "os/IClock.h"

#ifndef CLOCK_INDIVIDUAL_NUMBER_OF_CYCLES_PER_SECOND
#error "error compile flags - CLOCK_INDIVIDUAL_NUMBER_OF_CYCLES_PER_SECOND undefined"
#elif (CLOCK_INDIVIDUAL_NUMBER_OF_CYCLES_PER_SECOND == 0)
#error "error compile flags - CLOCK_INDIVIDUAL_NUMBER_OF_CYCLES_PER_SECOND invalid"
#endif

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

typedef struct {
  uint32_t cycle_start_in_ticks;
  uint16_t count_cycle;
} t_CLOCK_INDIVIDUAL_INLINE;

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

extern volatile t_CLOCK_INDIVIDUAL_INLINE clock_individual_inline;

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
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
COMPILER_INLINE uint16_t CLOCK_GET_CYCLES_PER_SECOND(void)
{
  return CLOCK_INDIVIDUAL_NUMBER_OF_CYCLES_PER_SECOND;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get start time of the current cycle.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint32_t CLOCK_GET_CYCLE_START_IN_TICKS(void)
{
  return clock_individual_inline.cycle_start_in_ticks;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get number of the current cycle.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint16_t CLOCK_GET_CYCLE_NUMBER(void)
{
  return clock_individual_inline.count_cycle;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
