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

#define CONTROLLER_DSPIC33FJ256GP710_SUPPORTED
#define CONTROLLER_DSPIC33EP128GS804_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP508_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP506_SUPPORTED

#include "hal/IController.h"
#include "hal/IOscillator.h"
#include "middleware/IAssert.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"

#ifndef OSCILLATOR_SOURCE_FREQUENCY_IN_HZ
#error "error compile flags - OSCILLATOR_SOURCE_FREQUENCY_IN_HZ undefined"
#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef struct {
  uint32_t source_frequency;
  uint32_t instruction_frequency;
  uint16_t pll_pre_divider;
  uint16_t pll_multiplier;
  uint16_t pll_post_divider_1;
  uint16_t pll_post_divider_2;
} t_OSCILLATOR_CONFIGURATION;

#define OSCILLATOR_CLOCK_SOURCE_FRC_WITHOUT_PLL                                0
#define OSCILLATOR_CLOCK_SOURCE_PRIMARY_OSCILLATOR_WITH_PLL                    3

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

/*
 * Frequencies:
 *
 *   Fsource  frequency of clock source (FRC or external oscillator)
 *   Fosc     system clock
 *   Fcy      instructions cycle clock
 *   Fp       peripheral clock
 *
 *   Assumption: processor clock & peripheral clock are forced to 1:1 ratio!
 *
 *   Fosc     = Fsource * PLL              CONTROLLER_DSPIC33FJ256GP710 / CONTROLLER_DSPIC33EP128GS804
 *   Fcy = Fp = Fosc / 2
 *            = Fsource * PLL / 2
 *
 *   Fosc     = Fsource * PLL / 2          CONTROLLER_DSPIC33CK256MP508 / CONTROLLER_DSPIC33CK256MP506
 *   Fcy = Fp = Fosc / 2
 *            = Fsource * PLL / 4
 *
 *
 * Startup settings determined by configuration bits (FRC with default PLL settings)
 *
 *            | Fsource  |  PLL  |     Fosc     |  Fcy = Fp
 *   ---------+----------+-------+--------------+-------------
 *   dsPIC33F | 7,37 MHz |  6,25 |  46,0625 MHz | 23,03125 MHz
 *   dsPIC33E | 7,37 MHz |  6,25 |  46,0625 MHz | 23,03125 MHz
 *   dsPIC33C | 8,00 MHz | 37,50 | 150,0000 MHz | 75,00000 MHz
 *
 *
 */
static const t_OSCILLATOR_CONFIGURATION oscillator_configuration[] = {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    { .source_frequency =  8000000, .instruction_frequency =  40000000, .pll_pre_divider = 2, .pll_multiplier =  40, .pll_post_divider_1 = 2, .pll_post_divider_2 = 1 }  // PLL: x 10
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    { .source_frequency = 10000000, .instruction_frequency =  40000000, .pll_pre_divider = 2, .pll_multiplier =  32, .pll_post_divider_1 = 2, .pll_post_divider_2 = 1 }, // PLL: x  8
    { .source_frequency = 10000000, .instruction_frequency =  60000000, .pll_pre_divider = 2, .pll_multiplier =  48, .pll_post_divider_1 = 2, .pll_post_divider_2 = 1 }  // PLL: x 12
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    { .source_frequency =  8000000, .instruction_frequency =  40000000, .pll_pre_divider = 1, .pll_multiplier = 160, .pll_post_divider_1 = 4, .pll_post_divider_2 = 2 }, // PLL: x 20
    { .source_frequency =  8000000, .instruction_frequency =  80000000, .pll_pre_divider = 1, .pll_multiplier = 160, .pll_post_divider_1 = 4, .pll_post_divider_2 = 1 }, // PLL: x 40
    { .source_frequency =  8000000, .instruction_frequency = 100000000, .pll_pre_divider = 1, .pll_multiplier = 150, .pll_post_divider_1 = 3, .pll_post_divider_2 = 1 }, // PLL: x 50
    { .source_frequency = 10000000, .instruction_frequency =  40000000, .pll_pre_divider = 1, .pll_multiplier = 128, .pll_post_divider_1 = 4, .pll_post_divider_2 = 2 }, // PLL: x 16
    { .source_frequency = 10000000, .instruction_frequency =  80000000, .pll_pre_divider = 1, .pll_multiplier = 128, .pll_post_divider_1 = 4, .pll_post_divider_2 = 1 }, // PLL: x 32
    { .source_frequency = 10000000, .instruction_frequency = 100000000, .pll_pre_divider = 1, .pll_multiplier = 120, .pll_post_divider_1 = 3, .pll_post_divider_2 = 1 }  // PLL: x 40
#endif
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static bool OscillatorConfigurationIsValid(const t_OSCILLATOR_CONFIGURATION * const configuration);
static void OscillatorSwitchClockSource(const uint8_t new_source);
static void OscillatorConfigurePll(const t_OSCILLATOR_CONFIGURATION * const configuration);
static void OscillatorWaitForPll(void);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Check the limits of the individual settings as well as all frequency limits within the signal chain.
 *
 -----------------------------------------------------------------------------*/
static bool OscillatorConfigurationIsValid(const t_OSCILLATOR_CONFIGURATION * const configuration)
{
  ASSERT_FATAL(configuration != NULL);

  bool valid = false;

#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804))

  valid =
      ((configuration->pll_multiplier >= 2) && (configuration->pll_multiplier <= 513)) &&
      ((configuration->pll_pre_divider >= 2) && (configuration->pll_pre_divider <= 33)) &&
      ((configuration->pll_post_divider_1 == 2) || (configuration->pll_post_divider_1 == 4) || (configuration->pll_post_divider_1 == 8)) &&
      (configuration->pll_post_divider_2 == 1);

  if (valid) {

    const uint32_t pfd_input_frequency = configuration->source_frequency / configuration->pll_pre_divider;
    const uint32_t vco_output_frequency = pfd_input_frequency * configuration->pll_multiplier;
    const uint32_t pll_output_frequency = vco_output_frequency / configuration->pll_post_divider_1;

    valid =
        ((pfd_input_frequency >= 800000) && (pfd_input_frequency <= 8000000)) &&
        ((vco_output_frequency >= 120000000) && (vco_output_frequency <= 340000000)) &&
        (pll_output_frequency <= 120000000);
  }

#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))

  valid =
      ((configuration->pll_multiplier >= 16) && (configuration->pll_multiplier <= 200)) &&
      ((configuration->pll_pre_divider >= 1) && (configuration->pll_pre_divider <= 8)) &&
      ((configuration->pll_post_divider_1 >= 1) && (configuration->pll_post_divider_1 <= 7)) &&
      ((configuration->pll_post_divider_2 >= 1) && (configuration->pll_post_divider_2 <= 7));

  if (valid) {

    const uint32_t pfd_input_frequency = configuration->source_frequency / configuration->pll_pre_divider;
    const uint32_t vco_output_frequency = pfd_input_frequency * configuration->pll_multiplier;
    const uint32_t pfd_input_frequency_limit = vco_output_frequency / 16;

    valid =
        ((configuration->source_frequency >= 8000000) && (configuration->source_frequency <= 64000000)) &&
        ((pfd_input_frequency >= 8000000) && (pfd_input_frequency <= pfd_input_frequency_limit)) &&
        ((vco_output_frequency >= 400000000) && (vco_output_frequency <= 1600000000));
  }

#endif

  return valid;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Set new clock source, requests oscillator switch and wait till it's done.
 *
 -----------------------------------------------------------------------------*/
static void OscillatorSwitchClockSource(const uint8_t new_source)
{
  __builtin_write_OSCCONH(new_source);
  __builtin_write_OSCCONL(OSCCON | 1);
  while (OSCCONbits.OSWEN != 0);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Apply new PLL settings.
 *
 -----------------------------------------------------------------------------*/
static void OscillatorConfigurePll(const t_OSCILLATOR_CONFIGURATION * const configuration)
{
  ASSERT_FATAL(configuration != NULL);

#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804))

  PLLFBDbits.PLLDIV = configuration->pll_multiplier - 2;
  CLKDIVbits.PLLPRE = configuration->pll_pre_divider - 2;
  CLKDIVbits.PLLPOST = (configuration->pll_post_divider_1 - 1) / 2;

#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))

  PLLFBDbits.PLLFBDIV = configuration->pll_multiplier;
  CLKDIVbits.PLLPRE = configuration->pll_pre_divider;
  PLLDIVbits.POST1DIV = configuration->pll_post_divider_1;
  PLLDIVbits.POST2DIV = configuration->pll_post_divider_2;

#endif
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Wait till PLL is locked (again).
 *
 -----------------------------------------------------------------------------*/
static void OscillatorWaitForPll(void)
{
  while (OSCCONbits.LOCK != 1);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Configure the PLL and switch to the primary oscillator.
 *
 -----------------------------------------------------------------------------*/
void OscillatorInit(void)
{
  uint8_t index = UINT8_MAX;
  const uint8_t number_of_entries = (uint8_t)(sizeof(oscillator_configuration) / sizeof(t_OSCILLATOR_CONFIGURATION));
  for (uint8_t entry = 0; entry < number_of_entries; entry++)
    if ((oscillator_configuration[entry].source_frequency == OSCILLATOR_SOURCE_FREQUENCY_IN_HZ) &&
        (oscillator_configuration[entry].instruction_frequency == OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ()))
      index = entry;

  const bool configuration_found = index != UINT8_MAX;
  const bool configuration_valid = configuration_found ? OscillatorConfigurationIsValid(&oscillator_configuration[index]) : false;
  ASSERT_FATAL(configuration_valid); // IME: core - oscillator - use specific fatal error?
  if (configuration_valid) {

    // configure the PLL (while not using it), switch to the primary oscillator and wait till the PLL is stable
    OscillatorSwitchClockSource(OSCILLATOR_CLOCK_SOURCE_FRC_WITHOUT_PLL);
    OscillatorConfigurePll(&oscillator_configuration[index]);
    OscillatorSwitchClockSource(OSCILLATOR_CLOCK_SOURCE_PRIMARY_OSCILLATOR_WITH_PLL);
    OscillatorWaitForPll();
  }
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
