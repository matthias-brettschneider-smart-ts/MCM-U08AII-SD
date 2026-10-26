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

#include "middleware/IGeneric.h"
#include "shared/INtc.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define NTC_TABLE_START_TEMPERATURE_IN_A10TH_DEGREE_CELCIUS                 -400
#define NTC_TABLE_STEP_WIDTH_IN_A10TH_KELVIN                                  50

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

// ratio of a NTCG103JF103FT1S in series with 10 kOhm, normalized to uint16_t
static const uint16_t ntc_table[] = {
     3302,  4247,  5403,  6784,  8424, 10327, 12495, 14918, // -40 °C ..  -5 °C
    17579, 20435, 23439, 26532, 29667, 32768, 35792, 38687, //   0 °C ..  35 °C
    41410, 43951, 46288, 48415, 50338, 52061, 53599, 54961, //  40 °C ..  75 °C
    56166, 57231, 58165, 58987, 59713, 60345, 60906, 61397, //  80 °C .. 115 °C
    61831, 62213, 62545, 62845, 63105, 63343, 63546         // 120 °C .. 150 °C
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static int16_t NtcBinarySearch(uint16_t value);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Performs a binary search within the lookup table.
 *
 * (The highest index of the table must not be returned because of the later on interpolation.
 *  Therefore, the exactly very last entry will not be found.)
 *
 -----------------------------------------------------------------------------*/
static int16_t NtcBinarySearch(uint16_t value)
{
  int16_t index_left = 0;
  int16_t index_right = (sizeof(ntc_table) / sizeof(uint16_t)) - 1;
  int16_t index;

  // search till index_left and index_right describe the correct interval or maybe point to the exact same index
  while ((index_right - index_left) > 1) {
    index = (index_left + index_right) / 2;
    if (value < ntc_table[index])
      index_right = index;
    else if (value >= ntc_table[index])
      index_left = index;
  }
  index = index_left;

  const bool found = (value >= ntc_table[index]) && (value < ntc_table[index + 1]);
  return found ? index : NTC_NOT_SUPPORTED;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Calculates the temperature for an normalized 16 bit raw value.
 *
 -----------------------------------------------------------------------------*/
int16_t NtcTemperatureGet(uint16_t raw)
{
  int16_t temperature_in_a10th_degree_celcius = NTC_NOT_SUPPORTED;

  // parse table and calculate linear interpolation if valid section was found
  const int16_t index = NtcBinarySearch(raw);
  if (index != NTC_NOT_SUPPORTED) {
    const uint16_t raw_1 = ntc_table[index];
    const uint16_t raw_2 = ntc_table[index + 1];
    const int16_t temperature_start = NTC_TABLE_START_TEMPERATURE_IN_A10TH_DEGREE_CELCIUS + (index * NTC_TABLE_STEP_WIDTH_IN_A10TH_KELVIN);
    const uint16_t temperature_offset = (raw - raw_1) * NTC_TABLE_STEP_WIDTH_IN_A10TH_KELVIN / (raw_2 - raw_1);
    temperature_in_a10th_degree_celcius = temperature_start + temperature_offset;
  }

  return temperature_in_a10th_degree_celcius;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
