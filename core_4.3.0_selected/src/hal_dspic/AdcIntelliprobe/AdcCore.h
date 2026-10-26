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
COMPILER_INLINE void ADC_CORE_ACTIVATE_POWER(const uint8_t core)
{
  switch (core) {
    case 0: ADCON5Lbits.C0PWR = 1; while (ADCON5Lbits.C0RDY == 0); break;
    case 1: ADCON5Lbits.C1PWR = 1; while (ADCON5Lbits.C1RDY == 0); break;
    case 2: ADCON5Lbits.SHRPWR = 1; while (ADCON5Lbits.SHRRDY == 0); break;
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void ADC_CORE_ENABLE(const uint8_t core)
{
  switch (core) {
    case 0: ADCON3Hbits.C0EN = 1; break;
    case 1: ADCON3Hbits.C1EN = 1; break;
    case 2: ADCON3Hbits.SHREN = 1; break;
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void ADC_CORE_SET_RESOLUTION(const uint8_t core, const uint8_t resolution)
{
  const uint16_t register_value = (resolution == 6) ? 0 : (resolution == 8) ? 1 : (resolution == 10) ? 2 : (resolution == 12) ? 3 : UINT8_MAX;
  ASSERT_FATAL(register_value != UINT8_MAX);

  switch (core) {
    case 0: ADCORE0Hbits.RES = register_value; break;
    case 1: ADCORE1Hbits.RES = register_value; break;
    case 2: ADCON1Hbits.SHRRES = register_value; break;
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void ADC_CORE_SET_TICK_TIME_IN_TICKS(const uint8_t core, const uint16_t tick_time_in_ticks)
{
  const uint16_t register_value = tick_time_in_ticks / 2;
  ASSERT_FATAL((register_value * 2) == tick_time_in_ticks);
  ASSERT_FATAL((register_value >= 1) && (register_value <= 127));

  switch (core) {
    case 0: ADCORE0Hbits.ADCS = register_value; break;
    case 1: ADCORE1Hbits.ADCS = register_value; break;
    case 2: ADCON2Lbits.SHRADCS = register_value; break;
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void ADC_CORE_SET_CONVERSION_DELAY_IN_TICKS(const uint8_t core, const uint16_t conversion_delay_in_ticks)
{
  const uint16_t register_value = conversion_delay_in_ticks - 2;
  ASSERT_FATAL(register_value <= 1023);

  switch (core) {
    case 0: ADCORE0Lbits.SAMC = register_value; ADCON4Lbits.SAMC0EN = 1; break;
    case 1: ADCORE1Lbits.SAMC = register_value; ADCON4Lbits.SAMC1EN = 1; break;
    case 2: ADCON2Hbits.SHRSAMC = register_value; break;
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void ADC_CORE_CALIBRATE(const uint8_t core)
{
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)

  switch (core) {
    //      enable calibration,     single-ended input mode,  start,                   wait,                             disable calibration
    case 0: ADCAL0Lbits.CAL0EN = 1; ADCAL0Lbits.CAL0DIFF = 0; ADCAL0Lbits.CAL0RUN = 1; while (ADCAL0Lbits.CAL0RDY != 1); ADCAL0Lbits.CAL0EN = 0; break;
    case 1: ADCAL0Lbits.CAL1EN = 1; ADCAL0Lbits.CAL1DIFF = 0; ADCAL0Lbits.CAL1RUN = 1; while (ADCAL0Lbits.CAL1RDY != 1); ADCAL0Lbits.CAL1EN = 0; break;
    case 2: ADCAL1Hbits.CSHREN = 1; ADCAL1Hbits.CSHRDIFF = 0; ADCAL1Hbits.CSHRRUN = 1; while (ADCAL1Hbits.CSHRRDY != 1); ADCAL1Hbits.CSHREN = 0; break;
    default: ASSERT_FATAL(false); break;
  }

#else

  (void)core; // nothing to do here

#endif
}


/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void ADC_CORE_INIT(const uint8_t core, const uint8_t resolution, const uint16_t conversion_delay_in_us)
{
  ASSERT_FATAL(resolution == 12);
  ASSERT_FATAL(conversion_delay_in_us == 20);

  ADC_CORE_SET_TICK_TIME_IN_TICKS(core, 2);
  ADC_CORE_SET_CONVERSION_DELAY_IN_TICKS(core, 200);
  ADC_CORE_SET_RESOLUTION(core, resolution);
  ADC_CORE_ACTIVATE_POWER(core);
  ADC_CORE_ENABLE(core);
  ADC_CORE_CALIBRATE(core);
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
