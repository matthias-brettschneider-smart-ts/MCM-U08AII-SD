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

#include "hal/IAdc.h"
#include "middleware/IAssert.h"
#include "middleware/IGeneric.h"
#include "AdcCommon.h"
#include "AdcCore.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
void AdcInit(void)
{
  // core tick time
  //
  //    50 ns/tick * 2 = 100 ns/tick;
  //
  // sampling time (conversion delay)
  //
  //    20 µs = 100 ns/tick * 200 ticks
  //          = 9 * RTOTAL * CHOLD
  //
  //    with CHOLD = 8 pF
  //
  //    => RTOTALmax = 20 µs / (9 x 8 pF) = 278 kOhm
  //
  // conversion time: (resolution + 1) * TADCORE // IME: core - dsPIC33C - adc - check/update calculations
  //
  //          = 8 * TCORESRC + (resolution + 2,5) * TADCORE
  //          = 8 *   50 ns  + (    12     + 2,5) * 100 ns
  //          =    0,4 µs    +            1,45 µs
  //          =             1,85 µs

  const uint16_t tick_time_in_ns = 50;
  ADC_COMMON_INIT(tick_time_in_ns);

  const uint8_t resolution = 12;
  const uint16_t conversion_delay_in_us = 20;
  ADC_CORE_INIT(0, resolution, conversion_delay_in_us); // first dedicated core
  ADC_CORE_INIT(2, resolution, conversion_delay_in_us); // shared core
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get new sample for the selected channel.
 *
 -----------------------------------------------------------------------------*/
uint16_t AdcAcquireSingleChannel(const int channel)
{
  return ADC_COMMON_ACQUIRE_CHANNEL(channel);
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
