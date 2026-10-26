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

#include "hal/IController.h"
#include "hal/ICompiler.h"
#include "hal/IDsp.h"
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
 * uint32_t = uint16_t * uint16_t
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint32_t DSP_MULTIPLY_U16_U16(const uint16_t factor_1, const uint16_t factor_2)
{
  return __builtin_muluu(factor_1, factor_2);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * int32_t = uint16_t * int16_t
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE int32_t DSP_MULTIPLY_U16_S16(const uint16_t factor_1, const int16_t factor_2)
{
  return __builtin_mulus(factor_1, factor_2);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * int32_t = int16_t * uint16_t
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE int32_t DSP_MULTIPLY_S16_U16(const int16_t factor_1, const uint16_t factor_2)
{
  return __builtin_mulsu(factor_1, factor_2);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * int32_t = int16_t * int16_t
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE int32_t DSP_MULTIPLY_S16_S16(const int16_t factor_1, const int16_t factor_2)
{
  return __builtin_mulss(factor_1, factor_2);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * int16_t = int32_t / int16_t
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE int16_t DSP_DIVIDE_SIGNED(const int32_t dividend, const int16_t divisor)
{
  return __builtin_divsd(dividend, divisor);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * int16_t = int32_t / int16_t with providing remainder.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE int16_t DSP_DIVIDE_SIGNED_REMAINDER(const int32_t dividend, const int16_t divisor, int16_t * const my_remainder)
{
  ASSERT_FATAL(my_remainder != NULL);

  return __builtin_divmodsd(dividend, divisor, my_remainder);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * uint16_t = uint32_t / uint16_t
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint16_t DSP_DIVIDE_UNSIGNED(const uint32_t dividend, const uint16_t divisor)
{
  return __builtin_divud(dividend, divisor);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * uint16_t = uint32_t / uint16_t with providing remainder.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint16_t DSP_DIVIDE_UNSIGNED_REMAINDER(const uint32_t dividend, const uint16_t divisor, uint16_t * const my_remainder)
{
  ASSERT_FATAL(my_remainder != NULL);

  return __builtin_divmodud(dividend, divisor, my_remainder);
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
