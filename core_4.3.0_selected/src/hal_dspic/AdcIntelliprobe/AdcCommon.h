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

#define CONTROLLER_DSPIC33EP128GS804_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP508_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP506_SUPPORTED

#include "hal/ICompiler.h"
#include "hal/IController.h"
#include "hal/IOscillator.h"
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
COMPILER_INLINE void ADC_COMMON_SET_TICK_TIME_IN_NS(const uint16_t tick_time_in_ns)
{
  ASSERT_FATAL(tick_time_in_ns == 50);

  ADCON3Hbits.CLKSEL = 0; // use system clock
  const uint8_t number_of_clock_periods =
      (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 40000000) ? 2 :
          (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 80000000) ? 4 :
              (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000) ? 5 :
                  UINT8_MAX;
  ASSERT_FATAL(number_of_clock_periods != UINT8_MAX);

  const uint8_t clock_divider = number_of_clock_periods - 1;
  ASSERT_FATAL(clock_divider <= 63);
  ADCON3Hbits.CLKDIV = clock_divider;

  const uint16_t warm_time_in_ticks = 256; // 256 x 50 ns = 12,8 µs (minimum is 10 µs)
  const uint16_t warm_time = (warm_time_in_ticks == 256) ? 0b1000 : UINT16_MAX;
  ASSERT_FATAL(warm_time <= 0b1111);
  ADCON5Hbits.WARMTIME = warm_time;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void ADC_COMMON_USE_REFERENCE_AVDD_AVSS(void)
{
  ADCON3Lbits.REFSEL = 0;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void ADC_COMMON_USE_INTEGER_FORMAT(void)
{
  ADCON1Hbits.FORM = 0;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void ADC_COMMON_ACTIVATE(void)
{
  ADCON1Lbits.ADON = 1;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool ADC_COMMON_IS_ACTIVATE(void)
{
  return ADCON1Lbits.ADON == 1;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void ADC_COMMON_INIT(const uint16_t tick_time_in_ns)
{
  ADC_COMMON_SET_TICK_TIME_IN_NS(tick_time_in_ns);
  ADC_COMMON_USE_REFERENCE_AVDD_AVSS();
  ADC_COMMON_USE_INTEGER_FORMAT();
  ADC_COMMON_ACTIVATE();
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void ADC_COMMON_MEASURE_CHANNEL(const uint8_t channel)
{
  ADCON3Lbits.CNVCHSEL = channel;
  ADCON3Lbits.CNVRTCH = 1;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool ADC_COMMON_DATA_IS_AVAILABLE(const uint8_t channel)
{
  const uint16_t mask = 1u << ((channel < 16) ? channel : (channel - 16));
  const uint16_t field = (channel < 16) ? ADSTATL : ADSTATH;
  return (field && mask) != 0;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint16_t ADC_COMMON_GET_BUFFER(const uint8_t channel)
{
  uint16_t buffer = 0;

  switch (channel) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case  0: buffer = ADCBUF0; break;
//  case  1: buffer = ADCBUF1; break;
//  case  2: buffer = ADCBUF2; break;
//  case  3: buffer = ADCBUF3; break;
    case  4: buffer = ADCBUF4; break;
//  case  5: buffer = ADCBUF5; break;
    case  6: buffer = ADCBUF6; break;
    case  7: buffer = ADCBUF7; break;
    case  8: buffer = ADCBUF8; break;
    case  9: buffer = ADCBUF9; break;
    case 10: buffer = ADCBUF10; break;
    case 11: buffer = ADCBUF11; break;
//  case 12: buffer = ADCBUF12; break;
//  case 13: buffer = ADCBUF13; break;
    case 14: buffer = ADCBUF14; break;
//  case 15: buffer = ADCBUF15; break;
//  case 16: buffer = ADCBUF16; break;
    case 17: buffer = ADCBUF17; break;
    case 18: buffer = ADCBUF18; break;
    case 19: buffer = ADCBUF19; break;
    case 20: buffer = ADCBUF20; break;
    case 21: buffer = ADCBUF21; break;
//  case 22: buffer = ADCBUF22; break;
//  case 23: buffer = ADCBUF23; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508)
    case  0: buffer = ADCBUF0; break;
//  case  1: buffer = ADCBUF1; break;
    case  2: buffer = ADCBUF2; break;
    case  3: buffer = ADCBUF3; break;
    case  4: buffer = ADCBUF4; break;
    case  5: buffer = ADCBUF5; break;
    case  6: buffer = ADCBUF6; break;
    case  7: buffer = ADCBUF7; break;
    case  8: buffer = ADCBUF8; break;
    case  9: buffer = ADCBUF9; break;
    case 10: buffer = ADCBUF10; break;
    case 11: buffer = ADCBUF11; break;
    case 12: buffer = ADCBUF12; break;
    case 13: buffer = ADCBUF13; break;
    case 14: buffer = ADCBUF14; break;
    case 15: buffer = ADCBUF15; break;
    case 16: buffer = ADCBUF16; break;
    case 17: buffer = ADCBUF17; break;
    case 18: buffer = ADCBUF18; break;
    case 19: buffer = ADCBUF19; break;
    case 20: buffer = ADCBUF20; break;
    case 21: buffer = ADCBUF21; break;
    case 22: buffer = ADCBUF22; break;
    case 23: buffer = ADCBUF23; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506)
    case  0: buffer = ADCBUF0; break;
//  case  1: buffer = ADCBUF1; break;
    case  2: buffer = ADCBUF2; break;
    case  3: buffer = ADCBUF3; break;
    case  4: buffer = ADCBUF4; break;
    case  5: buffer = ADCBUF5; break;
    case  6: buffer = ADCBUF6; break;
    case  7: buffer = ADCBUF7; break;
    case  8: buffer = ADCBUF8; break;
    case  9: buffer = ADCBUF9; break;
    case 10: buffer = ADCBUF10; break;
    case 11: buffer = ADCBUF11; break;
    case 12: buffer = ADCBUF12; break;
    case 13: buffer = ADCBUF13; break;
    case 14: buffer = ADCBUF14; break;
    case 15: buffer = ADCBUF15; break;
    case 16: buffer = ADCBUF16; break;
    case 17: buffer = ADCBUF17; break;
    case 18: buffer = ADCBUF18; break;
    case 19: buffer = ADCBUF19; break;
//  case 20: buffer = ADCBUF20; break;
//  case 21: buffer = ADCBUF21; break;
//  case 22: buffer = ADCBUF22; break;
//  case 23: buffer = ADCBUF23; break;
#endif
    default: ASSERT_FATAL(false); break;
  }

  return buffer;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint16_t ADC_COMMON_ACQUIRE_CHANNEL(const uint8_t channel)
{
  ASSERT_FATAL(ADC_COMMON_IS_ACTIVATE());

  ADC_COMMON_MEASURE_CHANNEL(channel);
  while (!ADC_COMMON_DATA_IS_AVAILABLE(channel));
  return ADC_COMMON_GET_BUFFER(channel);
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
