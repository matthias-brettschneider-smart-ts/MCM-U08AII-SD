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

COMPILER_INLINE uint32_t DSP_MULTIPLY_U16_U16(const uint16_t factor_1, const uint16_t factor_2);
COMPILER_INLINE int32_t DSP_MULTIPLY_U16_S16(const uint16_t factor_1, const int16_t factor_2);
COMPILER_INLINE int32_t DSP_MULTIPLY_S16_U16(const int16_t factor_1, const uint16_t factor_2);
COMPILER_INLINE int32_t DSP_MULTIPLY_S16_S16(const int16_t factor_1, const int16_t factor_2);

COMPILER_INLINE int16_t DSP_DIVIDE_SIGNED(const int32_t dividend, const int16_t divisor);
COMPILER_INLINE int16_t DSP_DIVIDE_SIGNED_REMAINDER(const int32_t dividend, const int16_t divisor, int16_t * const my_remainder);
COMPILER_INLINE uint16_t DSP_DIVIDE_UNSIGNED(const uint32_t dividend, const uint16_t divisor);
COMPILER_INLINE uint16_t DSP_DIVIDE_UNSIGNED_REMAINDER(const uint32_t dividend, const uint16_t divisor, uint16_t * const my_remainder);

/*============================================================================*/
/*  I M P L E M E N T A T I O N                                               */
/*============================================================================*/

#include "DspImpl.h"

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
