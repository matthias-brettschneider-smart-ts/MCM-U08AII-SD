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

#include "gtest/gtest.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef struct {
  uint16_t raw;
  int16_t temp_expected;
} t_TEST;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static const t_TEST test[] = {
    { .raw =          0, .temp_expected = NTC_NOT_SUPPORTED },
    { .raw =       3301, .temp_expected = NTC_NOT_SUPPORTED },
    { .raw =       3302, .temp_expected =             - 400 },
    { .raw =       4247, .temp_expected =             - 350 },
    { .raw =       5403, .temp_expected =             - 300 },
    { .raw =       6784, .temp_expected =             - 250 },
    { .raw =       8424, .temp_expected =             - 200 },
    { .raw =      10327, .temp_expected =             - 150 },
    { .raw =      12495, .temp_expected =             - 100 },
    { .raw =      14918, .temp_expected =             -  50 },
    { .raw =      17579, .temp_expected =                 0 },
    { .raw =      20435, .temp_expected =                50 },
    { .raw =      23439, .temp_expected =               100 },
    { .raw =      26532, .temp_expected =               150 },
    { .raw =      29667, .temp_expected =               200 },
    { .raw =      30288, .temp_expected =               210 },
    { .raw =      30908, .temp_expected =               220 },
    { .raw =      31528, .temp_expected =               230 },
    { .raw =      32148, .temp_expected =               240 },
    { .raw =      32768, .temp_expected =               250 },
    { .raw =      35792, .temp_expected =               300 },
    { .raw =      38687, .temp_expected =               350 },
    { .raw =      41410, .temp_expected =               400 },
    { .raw =      43951, .temp_expected =               450 },
    { .raw =      46288, .temp_expected =               500 },
    { .raw =      48415, .temp_expected =               550 },
    { .raw =      50338, .temp_expected =               600 },
    { .raw =      52061, .temp_expected =               650 },
    { .raw =      53599, .temp_expected =               700 },
    { .raw =      54961, .temp_expected =               750 },
    { .raw =      56166, .temp_expected =               800 },
    { .raw =      57231, .temp_expected =               850 },
    { .raw =      58165, .temp_expected =               900 },
    { .raw =      58987, .temp_expected =               950 },
    { .raw =      59713, .temp_expected =              1000 },
    { .raw =      60345, .temp_expected =              1050 },
    { .raw =      60906, .temp_expected =              1100 },
    { .raw =      61397, .temp_expected =              1150 },
    { .raw =      61831, .temp_expected =              1200 },
    { .raw =      62213, .temp_expected =              1250 },
    { .raw =      62545, .temp_expected =              1300 },
    { .raw =      62845, .temp_expected =              1350 },
    { .raw =      63105, .temp_expected =              1400 },
    { .raw =      63343, .temp_expected =              1450 },
    { .raw =      63545, .temp_expected =              1499 },
    { .raw =      63546, .temp_expected = NTC_NOT_SUPPORTED },
    { .raw = UINT16_MAX, .temp_expected = NTC_NOT_SUPPORTED },
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

// --- helpers -----------------------------------------------------------------

// --- tests -------------------------------------------------------------------

class Ntc : public ::testing::Test
{
  protected:

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
};

TEST_F(Ntc, ConversionKnowsTheValueRangeItCanHandle)
{
  for (uint16_t raw = 0; raw <= 3301; raw++) {
    int16_t temp_below = NtcTemperatureGet(raw);
    ASSERT_EQ(temp_below, NTC_NOT_SUPPORTED);
  }

  for (uint16_t raw = 3302; raw <= 63545; raw++) {
    int16_t temp = NtcTemperatureGet(raw);
    ASSERT_NE(temp, NTC_NOT_SUPPORTED);
  }

  for (uint32_t raw = 63546; raw <= UINT16_MAX; raw++) {
    int16_t temp_above = NtcTemperatureGet((uint16_t)raw);
    ASSERT_EQ(temp_above, NTC_NOT_SUPPORTED);
  }
}

TEST_F(Ntc, ConversionProvidesExpectedResultsForSpecificValues)
{
  int entries = sizeof(test) / sizeof(t_TEST);
  for (int entry = 0; entry < entries; entry++) {
    int16_t temp = NtcTemperatureGet(test[entry].raw);
    ASSERT_EQ(temp, test[entry].temp_expected);
  }
}

TEST_F(Ntc, ConversionResultsAreMonotonouslyIncreasingWithNoStepBiggerThanOneLsb)
{
  int16_t temp_old = -401;
  for (uint32_t raw = 0; raw <= UINT16_MAX; raw++) {
    int16_t temp_new = NtcTemperatureGet((uint16_t)raw);
    if (temp_new != NTC_NOT_SUPPORTED) {
      ASSERT_GE(temp_new, temp_old);
      ASSERT_LE(temp_new - temp_old, 1);
      temp_old = temp_new;
    }
  }

  ASSERT_EQ(temp_old, 1499);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 -----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
