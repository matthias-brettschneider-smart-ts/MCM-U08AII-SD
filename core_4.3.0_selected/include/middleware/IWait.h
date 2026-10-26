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

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

COMPILER_INLINE uint16_t WAIT_GET_TICKS_FOR_US_16(const uint16_t time_in_us);
COMPILER_INLINE uint32_t WAIT_GET_TICKS_FOR_US_32(const uint32_t time_in_us);
COMPILER_INLINE uint32_t WAIT_GET_TICKS_FOR_MS_32(const uint16_t time_in_ms);

COMPILER_INLINE uint16_t WAIT_GET_TIMER_VALUE_16(void);
COMPILER_INLINE uint32_t WAIT_GET_TIMER_VALUE_32(void);

COMPILER_INLINE bool WAIT_CHECK_TIMEOUT_US_16(const uint16_t reference_ticks, const uint16_t time_in_us);
COMPILER_INLINE bool WAIT_CHECK_TIMEOUT_US_32(const uint32_t reference_ticks, const uint32_t time_in_us);
COMPILER_INLINE bool WAIT_CHECK_TIMEOUT_MS_32(const uint32_t reference_ticks, const uint32_t time_in_ms);

COMPILER_INLINE void WAIT_US_16(const uint16_t reference_ticks, const uint16_t time_in_us);
COMPILER_INLINE void WAIT_US_32(const uint32_t reference_ticks, const uint32_t time_in_us);
COMPILER_INLINE void WAIT_MS_32(const uint32_t reference_ticks, const uint32_t time_in_ms);

COMPILER_INLINE void WAIT_US(const uint16_t time_in_us);
COMPILER_INLINE void WAIT_MS(const uint16_t time_in_ms);

/*============================================================================*/
/*  I M P L E M E N T A T I O N                                               */
/*============================================================================*/

#include "WaitImpl.h"

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
