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

#define TIMER_ISR(number)                  void TIMER_ISR_INTERNAL(number)(void)

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

COMPILER_INLINE bool TIMER_SUPPORTS_32_BIT(void);

COMPILER_INLINE void TIMER_WAIT_ONE_TICK(void);

COMPILER_INLINE void TIMER_START(const int timer);
COMPILER_INLINE void TIMER_STOP(const int timer);

COMPILER_INLINE void TIMER_USE_AS_32_BIT(const int timer);

COMPILER_INLINE void TIMER_SET_FREQUENCY(const int timer, const uint32_t frequency);

COMPILER_INLINE void TIMER_SET_PERIOD_16(const int timer, const uint16_t period);
COMPILER_INLINE void TIMER_SET_PERIOD_16_UPPER(const int timer, const uint16_t period);
COMPILER_INLINE uint16_t TIMER_GET_PERIOD_16(const int timer);
COMPILER_INLINE uint16_t TIMER_GET_PERIOD_16_UPPER(const int timer);

COMPILER_INLINE void TIMER_SET_VALUE_16(const int timer, const uint16_t value);
COMPILER_INLINE void TIMER_SET_VALUE_16_UPPER(const int timer, const uint16_t value);
COMPILER_INLINE uint16_t TIMER_GET_VALUE_16(const int timer);
COMPILER_INLINE uint16_t TIMER_GET_VALUE_16_UPPER(const int timer);

COMPILER_INLINE void TIMER_SET_INTERRUPT_PRIORITY(const int timer, const int priority);
COMPILER_INLINE void TIMER_ENABLE_INTERRUPT(const int timer);
COMPILER_INLINE void TIMER_DISABLE_INTERRUPT(const int timer);
COMPILER_INLINE void TIMER_CONFIRM_INTERRUPT(const int timer);

/*============================================================================*/
/*  I M P L E M E N T A T I O N                                               */
/*============================================================================*/

#include "TimerImpl.h"

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
