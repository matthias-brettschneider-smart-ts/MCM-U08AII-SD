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
#include "hal/IOscillator.h"
#include "middleware/IAssert.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804))
#define TIMER_ISR_INTERNAL(number)                         _T##number##Interrupt
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
#define TIMER_ISR_INTERNAL(number)                         _CCT##number##Interrupt
#endif

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool TIMER_SUPPORTS_32_BIT(void)
{
  return (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Wait one tick.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void TIMER_WAIT_ONE_TICK(void)
{
  __asm__ volatile ("nop");
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Start timer.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void TIMER_START(const int timer)
{
  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 1: T1CONbits.TON = 1; break;
    case 2: T2CONbits.TON = 1; break;
    case 3: T3CONbits.TON = 1; break;
    case 4: T4CONbits.TON = 1; break;
    case 5: T5CONbits.TON = 1; break;
    case 6: T6CONbits.TON = 1; break;
    case 7: T7CONbits.TON = 1; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 1: T1CONbits.TON = 1; break;
    case 2: T2CONbits.TON = 1; break;
    case 3: T3CONbits.TON = 1; break;
    case 4: T4CONbits.TON = 1; break;
    case 5: T5CONbits.TON = 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: CCP1CON1Lbits.CCPON = 1; break;
    case 2: CCP2CON1Lbits.CCPON = 1; break;
    case 3: CCP3CON1Lbits.CCPON = 1; break;
    case 4: CCP4CON1Lbits.CCPON = 1; break;
    case 5: CCP5CON1Lbits.CCPON = 1; break;
    case 6: CCP6CON1Lbits.CCPON = 1; break;
    case 7: CCP7CON1Lbits.CCPON = 1; break;
    case 8: CCP8CON1Lbits.CCPON = 1; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Stop timer.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void TIMER_STOP(const int timer)
{
  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 1: T1CONbits.TON = 0; break;
    case 2: T2CONbits.TON = 0; break;
    case 3: T3CONbits.TON = 0; break;
    case 4: T4CONbits.TON = 0; break;
    case 5: T5CONbits.TON = 0; break;
    case 6: T6CONbits.TON = 0; break;
    case 7: T7CONbits.TON = 0; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 1: T1CONbits.TON = 0; break;
    case 2: T2CONbits.TON = 0; break;
    case 3: T3CONbits.TON = 0; break;
    case 4: T4CONbits.TON = 0; break;
    case 5: T5CONbits.TON = 0; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: CCP1CON1Lbits.CCPON = 0; break;
    case 2: CCP2CON1Lbits.CCPON = 0; break;
    case 3: CCP3CON1Lbits.CCPON = 0; break;
    case 4: CCP4CON1Lbits.CCPON = 0; break;
    case 5: CCP5CON1Lbits.CCPON = 0; break;
    case 6: CCP6CON1Lbits.CCPON = 0; break;
    case 7: CCP7CON1Lbits.CCPON = 0; break;
    case 8: CCP8CON1Lbits.CCPON = 0; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Use two 16 bit values as one 32 bit value.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void TIMER_USE_AS_32_BIT(const int timer)
{
  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 2: T2CONbits.T32 = 1; break;
    case 4: T4CONbits.T32 = 1; break;
    case 6: T6CONbits.T32 = 1; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 2: T2CONbits.T32 = 1; break;
    case 4: T4CONbits.T32 = 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: CCP1CON1Lbits.T32 = 1; break;
    case 2: CCP2CON1Lbits.T32 = 1; break;
    case 3: CCP3CON1Lbits.T32 = 1; break;
    case 4: CCP4CON1Lbits.T32 = 1; break;
    case 5: CCP5CON1Lbits.T32 = 1; break;
    case 6: CCP6CON1Lbits.T32 = 1; break;
    case 7: CCP7CON1Lbits.T32 = 1; break;
    case 8: CCP8CON1Lbits.T32 = 1; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Set timer frequency.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void TIMER_SET_FREQUENCY(const int timer, const uint32_t frequency)
{
#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
  const uint8_t clock_source =
    ((frequency ==    625000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ? 0 :
    ((frequency ==   1250000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ? 2 :
    ((frequency ==   2500000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ? 0 :
    ((frequency ==   5000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ? 2 :
    ((frequency ==  10000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ? 0 :
    ((frequency ==  20000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ? 2 :
    ((frequency ==  40000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ? 0 :
    ((frequency ==  80000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ? 2 :
    ((frequency ==   1250000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ? 0 :
    ((frequency ==   2500000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ? 2 :
    ((frequency ==   5000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ? 0 :
    ((frequency ==  10000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ? 2 :
    ((frequency ==  20000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ? 0 :
    ((frequency ==  40000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ? 2 :
    ((frequency ==  80000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ? 0 :
    ((frequency == 160000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ? 2 :
    ((frequency ==   1562500) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ? 0 :
    ((frequency ==   3125000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ? 2 :
    ((frequency ==   6250000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ? 0 :
    ((frequency ==  12500000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ? 2 :
    ((frequency ==  25000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ? 0 :
    ((frequency ==  50000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ? 2 :
    ((frequency == 100000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ? 0 :
    ((frequency == 200000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ? 2 :
    UINT8_MAX;

  ASSERT_FATAL(clock_source <= 0b111);
#endif

  const uint16_t divider =
#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804))
    ((frequency ==    156250) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ? 256 :
    ((frequency ==    625000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ?  64 :
    ((frequency ==   5000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ?   8 :
    ((frequency ==  40000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ?   1 :
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    ((frequency ==    625000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ?  64 :
    ((frequency ==   1250000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ?  64 :
    ((frequency ==   2500000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ?  16 :
    ((frequency ==   5000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ?  16 :
    ((frequency ==  10000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ?   4 :
    ((frequency ==  20000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ?   4 :
    ((frequency ==  40000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ?   1 :
    ((frequency ==  80000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ?   1 :
    ((frequency ==   1250000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ?  64 :
    ((frequency ==   2500000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ?  64 :
    ((frequency ==   5000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ?  16 :
    ((frequency ==  10000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ?  16 :
    ((frequency ==  20000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ?   4 :
    ((frequency ==  40000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ?   4 :
    ((frequency ==  80000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ?   1 :
    ((frequency == 160000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ?   1 :
    ((frequency ==   1562500) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ?  64 :
    ((frequency ==   3125000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ?  64 :
    ((frequency ==   6250000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ?  16 :
    ((frequency ==  12500000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ?  16 :
    ((frequency ==  25000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ?   4 :
    ((frequency ==  50000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ?   4 :
    ((frequency == 100000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ?   1 :
    ((frequency == 200000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ?   1 :
#endif
    0;

  const uint8_t prescaler =
#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804))
      (divider == 1) ? 0 : (divider == 8) ? 1 : (divider == 64) ? 2 : (divider == 256) ? 3 : UINT8_MAX;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
          (divider == 1) ? 0 : (divider == 4) ? 1 : (divider == 16) ? 2 : (divider == 64) ? 3 : UINT8_MAX;
#endif

  ASSERT_FATAL(prescaler <= 0b11);

  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 1: T1CONbits.TCKPS = prescaler; break;
    case 2: T2CONbits.TCKPS = prescaler; break;
    case 3: T3CONbits.TCKPS = prescaler; break;
    case 4: T4CONbits.TCKPS = prescaler; break;
    case 5: T5CONbits.TCKPS = prescaler; break;
    case 6: T6CONbits.TCKPS = prescaler; break;
    case 7: T7CONbits.TCKPS = prescaler; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 1: T1CONbits.TCKPS = prescaler; break;
    case 2: T2CONbits.TCKPS = prescaler; break;
    case 3: T3CONbits.TCKPS = prescaler; break;
    case 4: T4CONbits.TCKPS = prescaler; break;
    case 5: T5CONbits.TCKPS = prescaler; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: CCP1CON1Lbits.CLKSEL = clock_source; CCP1CON1Lbits.TMRPS = prescaler; break;
    case 2: CCP2CON1Lbits.CLKSEL = clock_source; CCP2CON1Lbits.TMRPS = prescaler; break;
    case 3: CCP3CON1Lbits.CLKSEL = clock_source; CCP3CON1Lbits.TMRPS = prescaler; break;
    case 4: CCP4CON1Lbits.CLKSEL = clock_source; CCP4CON1Lbits.TMRPS = prescaler; break;
    case 5: CCP5CON1Lbits.CLKSEL = clock_source; CCP5CON1Lbits.TMRPS = prescaler; break;
    case 6: CCP6CON1Lbits.CLKSEL = clock_source; CCP6CON1Lbits.TMRPS = prescaler; break;
    case 7: CCP7CON1Lbits.CLKSEL = clock_source; CCP7CON1Lbits.TMRPS = prescaler; break;
    case 8: CCP8CON1Lbits.CLKSEL = clock_source; CCP8CON1Lbits.TMRPS = prescaler; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Set timer period.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void TIMER_SET_PERIOD_16(const int timer, const uint16_t period)
{
  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 1: PR1 = period; break;
    case 2: PR2 = period; break;
    case 3: PR3 = period; break;
    case 4: PR4 = period; break;
    case 5: PR5 = period; break;
    case 6: PR6 = period; break;
    case 7: PR7 = period; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 1: PR1 = period; break;
    case 2: PR2 = period; break;
    case 3: PR3 = period; break;
    case 4: PR4 = period; break;
    case 5: PR5 = period; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: CCP1PRL = period; break;
    case 2: CCP2PRL = period; break;
    case 3: CCP3PRL = period; break;
    case 4: CCP4PRL = period; break;
    case 5: CCP5PRL = period; break;
    case 6: CCP6PRL = period; break;
    case 7: CCP7PRL = period; break;
    case 8: CCP8PRL = period; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Set timer period.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void TIMER_SET_PERIOD_16_UPPER(const int timer, const uint16_t period)
{
  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 2: PR3 = period; break;
    case 4: PR5 = period; break;
    case 6: PR7 = period; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 2: PR3 = period; break;
    case 4: PR5 = period; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: CCP1PRH = period; break;
    case 2: CCP2PRH = period; break;
    case 3: CCP3PRH = period; break;
    case 4: CCP4PRH = period; break;
    case 5: CCP5PRH = period; break;
    case 6: CCP6PRH = period; break;
    case 7: CCP7PRH = period; break;
    case 8: CCP8PRH = period; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get timer period.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint16_t TIMER_GET_PERIOD_16(const int timer)
{
  uint16_t period = 0;

  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 1: period = PR1; break;
    case 2: period = PR2; break;
    case 3: period = PR3; break;
    case 4: period = PR4; break;
    case 5: period = PR5; break;
    case 6: period = PR6; break;
    case 7: period = PR7; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 1: period = PR1; break;
    case 2: period = PR2; break;
    case 3: period = PR3; break;
    case 4: period = PR4; break;
    case 5: period = PR5; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: period = CCP1PRL; break;
    case 2: period = CCP2PRL; break;
    case 3: period = CCP3PRL; break;
    case 4: period = CCP4PRL; break;
    case 5: period = CCP5PRL; break;
    case 6: period = CCP6PRL; break;
    case 7: period = CCP7PRL; break;
    case 8: period = CCP8PRL; break;
#endif
    default: ASSERT_FATAL(false); break;
  }

  return period;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get timer period.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint16_t TIMER_GET_PERIOD_16_UPPER(const int timer)
{
  uint16_t period = 0;

  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 2: period = PR3; break;
    case 4: period = PR5; break;
    case 6: period = PR7; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 2: period = PR3; break;
    case 4: period = PR5; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: period = CCP1PRH; break;
    case 2: period = CCP2PRH; break;
    case 3: period = CCP3PRH; break;
    case 4: period = CCP4PRH; break;
    case 5: period = CCP5PRH; break;
    case 6: period = CCP6PRH; break;
    case 7: period = CCP7PRH; break;
    case 8: period = CCP8PRH; break;
#endif
    default: ASSERT_FATAL(false); break;
  }

  return period;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Set timer value.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void TIMER_SET_VALUE_16(const int timer, const uint16_t value)
{
  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 1: TMR1 = value; break;
    case 2: TMR2 = value; break;
    case 3: TMR3 = value; break;
    case 4: TMR4 = value; break;
    case 5: TMR5 = value; break;
    case 6: TMR6 = value; break;
    case 7: TMR7 = value; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 1: TMR1 = value; break;
    case 2: TMR2 = value; break;
    case 3: TMR3 = value; break;
    case 4: TMR4 = value; break;
    case 5: TMR5 = value; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: CCP1TMRL = value; break;
    case 2: CCP2TMRL = value; break;
    case 3: CCP3TMRL = value; break;
    case 4: CCP4TMRL = value; break;
    case 5: CCP5TMRL = value; break;
    case 6: CCP6TMRL = value; break;
    case 7: CCP7TMRL = value; break;
    case 8: CCP8TMRL = value; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Set timer value.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void TIMER_SET_VALUE_16_UPPER(const int timer, const uint16_t value)
{
  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 2: TMR3 = value; break;
    case 4: TMR5 = value; break;
    case 6: TMR7 = value; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 2: TMR3 = value; break;
    case 4: TMR5 = value; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: CCP1TMRH = value; break;
    case 2: CCP2TMRH = value; break;
    case 3: CCP3TMRH = value; break;
    case 4: CCP4TMRH = value; break;
    case 5: CCP5TMRH = value; break;
    case 6: CCP6TMRH = value; break;
    case 7: CCP7TMRH = value; break;
    case 8: CCP8TMRH = value; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get timer value.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint16_t TIMER_GET_VALUE_16(const int timer)
{
  uint16_t value = 0;

  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 1: value = TMR1; break;
    case 2: value = TMR2; break;
    case 3: value = TMR3; break;
    case 4: value = TMR4; break;
    case 5: value = TMR5; break;
    case 6: value = TMR6; break;
    case 7: value = TMR7; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 1: value = TMR1; break;
    case 2: value = TMR2; break;
    case 3: value = TMR3; break;
    case 4: value = TMR4; break;
    case 5: value = TMR5; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: value = CCP1TMRL; break;
    case 2: value = CCP2TMRL; break;
    case 3: value = CCP3TMRL; break;
    case 4: value = CCP4TMRL; break;
    case 5: value = CCP5TMRL; break;
    case 6: value = CCP6TMRL; break;
    case 7: value = CCP7TMRL; break;
    case 8: value = CCP8TMRL; break;
#endif
    default: ASSERT_FATAL(false); break;
  }

  return value;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get timer value.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint16_t TIMER_GET_VALUE_16_UPPER(const int timer)
{
  uint16_t value = 0;

  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 2: value = TMR3; break;
    case 4: value = TMR5; break;
    case 6: value = TMR7; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 2: value = TMR3; break;
    case 4: value = TMR5; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: value = CCP1TMRH; break;
    case 2: value = CCP2TMRH; break;
    case 3: value = CCP3TMRH; break;
    case 4: value = CCP4TMRH; break;
    case 5: value = CCP5TMRH; break;
    case 6: value = CCP6TMRH; break;
    case 7: value = CCP7TMRH; break;
    case 8: value = CCP8TMRH; break;
#endif
    default: ASSERT_FATAL(false); break;
  }

  return value;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Set interrupt priority.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void TIMER_SET_INTERRUPT_PRIORITY(const int timer, const int priority)
{
  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 1: _T1IP = priority; break;
    case 2: _T2IP = priority; break;
    case 3: _T3IP = priority; break;
    case 4: _T4IP = priority; break;
    case 5: _T5IP = priority; break;
    case 6: _T6IP = priority; break;
    case 7: _T7IP = priority; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 1: _T1IP = priority; break;
    case 2: _T2IP = priority; break;
    case 3: _T3IP = priority; break;
    case 4: _T4IP = priority; break;
    case 5: _T5IP = priority; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: _CCP1IP = priority; break;
    case 2: _CCP2IP = priority; break;
    case 3: _CCP3IP = priority; break;
    case 4: _CCP4IP = priority; break;
    case 5: _CCP5IP = priority; break;
    case 6: _CCP6IP = priority; break;
    case 7: _CCP7IP = priority; break;
    case 8: _CCP8IP = priority; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Enable timer interrupt.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void TIMER_ENABLE_INTERRUPT(const int timer)
{
  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 1: _T1IE = 1; break;
    case 2: _T2IE = 1; break;
    case 3: _T3IE = 1; break;
    case 4: _T4IE = 1; break;
    case 5: _T5IE = 1; break;
    case 6: _T6IE = 1; break;
    case 7: _T7IE = 1; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 1: _T1IE = 1; break;
    case 2: _T2IE = 1; break;
    case 3: _T3IE = 1; break;
    case 4: _T4IE = 1; break;
    case 5: _T5IE = 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: _CCT1IE = 1; break;
    case 2: _CCT2IE = 1; break;
    case 3: _CCT3IE = 1; break;
    case 4: _CCT4IE = 1; break;
    case 5: _CCT5IE = 1; break;
    case 6: _CCT6IE = 1; break;
    case 7: _CCT7IE = 1; break;
    case 8: _CCT8IE = 1; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Disable timer interrupt.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void TIMER_DISABLE_INTERRUPT(const int timer)
{
  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 1: _T1IE = 0; break;
    case 2: _T2IE = 0; break;
    case 3: _T3IE = 0; break;
    case 4: _T4IE = 0; break;
    case 5: _T5IE = 0; break;
    case 6: _T6IE = 0; break;
    case 7: _T7IE = 0; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 1: _T1IE = 0; break;
    case 2: _T2IE = 0; break;
    case 3: _T3IE = 0; break;
    case 4: _T4IE = 0; break;
    case 5: _T5IE = 0; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: _CCT1IE = 0; break;
    case 2: _CCT2IE = 0; break;
    case 3: _CCT3IE = 0; break;
    case 4: _CCT4IE = 0; break;
    case 5: _CCT5IE = 0; break;
    case 6: _CCT6IE = 0; break;
    case 7: _CCT7IE = 0; break;
    case 8: _CCT8IE = 0; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Confirm timer interrupt.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void TIMER_CONFIRM_INTERRUPT(const int timer)
{
  switch (timer) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 1: _T1IF = 0; break;
    case 2: _T2IF = 0; break;
    case 3: _T3IF = 0; break;
    case 4: _T4IF = 0; break;
    case 5: _T5IF = 0; break;
    case 6: _T6IF = 0; break;
    case 7: _T7IF = 0; break;
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 1: _T1IF = 0; break;
    case 2: _T2IF = 0; break;
    case 3: _T3IF = 0; break;
    case 4: _T4IF = 0; break;
    case 5: _T5IF = 0; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 1: _CCT1IF = 0; break;
    case 2: _CCT2IF = 0; break;
    case 3: _CCT3IF = 0; break;
    case 4: _CCT4IF = 0; break;
    case 5: _CCT5IF = 0; break;
    case 6: _CCT6IF = 0; break;
    case 7: _CCT7IF = 0; break;
    case 8: _CCT8IF = 0; break;
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
