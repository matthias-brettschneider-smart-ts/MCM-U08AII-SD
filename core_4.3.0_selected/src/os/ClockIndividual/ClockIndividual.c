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

#include "hal/ICompiler.h"
#include "hal/ITimer.h"
#include "middleware/IGeneric.h"
#include "middleware/IWait.h"
#include "os/IClock.h"
#include "os/IProtocol.h"
#include "project/IApplication.h"

#ifndef CLOCK_INDIVIDUAL_TIMER_NUMBER
#error "error compile flags - CLOCK_INDIVIDUAL_TIMER_NUMBER undefined"
#elif ((CLOCK_INDIVIDUAL_TIMER_NUMBER < 1) || (CLOCK_INDIVIDUAL_TIMER_NUMBER > 9))
#error "error compile flags - CLOCK_INDIVIDUAL_TIMER_NUMBER invalid"
#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define CLOCK_INDIVIDUAL_INTERRUPT_PRIORITY_GENERATION                         3
#define CLOCK_INDIVIDUAL_INTERRUPT_PRIORITY_APPLICATION                        2

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

// variable can not be defined static because it is used by inline functions (but NOT exported beyond the interface)
volatile t_CLOCK_INDIVIDUAL_INLINE clock_individual_inline;

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

COMPILER_ISR TIMER_ISR(CLOCK_INDIVIDUAL_TIMER_NUMBER);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Start signal detection.
 *
 -----------------------------------------------------------------------------*/
void ClockStartSignalReception(void)
{
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Start clock generation.
 *
 -----------------------------------------------------------------------------*/
void ClockStartSignalGeneration(void)
{
  TIMER_STOP(CLOCK_INDIVIDUAL_TIMER_NUMBER);
  TIMER_SET_VALUE_16(CLOCK_INDIVIDUAL_TIMER_NUMBER, 0);
  TIMER_SET_FREQUENCY(CLOCK_INDIVIDUAL_TIMER_NUMBER, 625000);
  ASSERT_STATIC(CLOCK_INDIVIDUAL_NUMBER_OF_CYCLES_PER_SECOND > 0);
  ASSERT_STATIC((625000 % CLOCK_INDIVIDUAL_NUMBER_OF_CYCLES_PER_SECOND) == 0);
  ASSERT_STATIC((625000 / CLOCK_INDIVIDUAL_NUMBER_OF_CYCLES_PER_SECOND) <= UINT16_MAX);
  const uint16_t period_in_ticks = 625000 / CLOCK_INDIVIDUAL_NUMBER_OF_CYCLES_PER_SECOND;
  TIMER_SET_PERIOD_16(CLOCK_INDIVIDUAL_TIMER_NUMBER, period_in_ticks - 1);
  TIMER_SET_INTERRUPT_PRIORITY(CLOCK_INDIVIDUAL_TIMER_NUMBER, CLOCK_INDIVIDUAL_INTERRUPT_PRIORITY_GENERATION);
  TIMER_CONFIRM_INTERRUPT(CLOCK_INDIVIDUAL_TIMER_NUMBER);
  TIMER_ENABLE_INTERRUPT(CLOCK_INDIVIDUAL_TIMER_NUMBER);
  TIMER_START(CLOCK_INDIVIDUAL_TIMER_NUMBER);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Stop clock generation.
 *
 -----------------------------------------------------------------------------*/
void ClockStopSignalGeneration(void)
{
  TIMER_STOP(CLOCK_INDIVIDUAL_TIMER_NUMBER);
  TIMER_CONFIRM_INTERRUPT(CLOCK_INDIVIDUAL_TIMER_NUMBER);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Manual check for missing clock.
 *
 -----------------------------------------------------------------------------*/
void ClockCheckState(void)
{
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * A signal edge has to be created.
 *
 -----------------------------------------------------------------------------*/
COMPILER_ISR TIMER_ISR(CLOCK_INDIVIDUAL_TIMER_NUMBER)
{
  clock_individual_inline.cycle_start_in_ticks = WAIT_GET_TIMER_VALUE_32();
  clock_individual_inline.count_cycle++;
  if (clock_individual_inline.count_cycle == CLOCK_GET_CYCLES_PER_SECOND())
    clock_individual_inline.count_cycle = 0;

  if (!ProtocolIsProgrammingInProgress())
    ApplOperateInterrupt(clock_individual_inline.count_cycle, false);

  TIMER_CONFIRM_INTERRUPT(CLOCK_INDIVIDUAL_TIMER_NUMBER);
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
