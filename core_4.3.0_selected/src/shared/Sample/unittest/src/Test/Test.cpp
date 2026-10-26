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
#include "shared/ISample.h"

#include "gtest/gtest.h"

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

// --- helpers -----------------------------------------------------------------

// --- tests -------------------------------------------------------------------

class Sample : public ::testing::Test
{
  protected:

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
};

TEST_F(Sample, ProvidesExpectedValuesForCableBreak)
{
  ASSERT_EQ(SAMPLE_ID_CABLE_BREAK_16, 0x7FFF);
  ASSERT_EQ(SAMPLE_ID_CABLE_BREAK_32, 0x7FFFFFFF);
}

TEST_F(Sample, ProvidesExpectedValuesForChannelDisabled)
{
  ASSERT_EQ(SAMPLE_ID_CHANNEL_DISABLED_16, 0x7FFE);
  ASSERT_EQ(SAMPLE_ID_CHANNEL_DISABLED_32, 0x7FFFFFFE);
}

TEST_F(Sample, ProvidesExpectedValuesForInvalidSettings)
{
  ASSERT_EQ(SAMPLE_ID_INVALID_SETTINGS_16, 0x7FFC);
  ASSERT_EQ(SAMPLE_ID_INVALID_SETTINGS_32, 0x7FFFFFFC);
}

TEST_F(Sample, ProvidesExpectedValuesForNotAvailable)
{
  ASSERT_EQ(SAMPLE_ID_NOT_AVAILABLE_16, 0x7FFB);
  ASSERT_EQ(SAMPLE_ID_NOT_AVAILABLE_32, 0x7FFFFFFB);
}

TEST_F(Sample, ProvidesExpectedValuesForUnderVoltage)
{
  ASSERT_EQ(SAMPLE_ID_UNDER_VOLTAGE_16, 0x7FFA);
  ASSERT_EQ(SAMPLE_ID_UNDER_VOLTAGE_32, 0x7FFFFFFA);
}

TEST_F(Sample, ConversionHandlesNegativeClippingCorrectly)
{
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(0xFFFF8001), (int16_t)0x8001); // most negative
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(0xFFFF8000), (int16_t)0x8000); // clipping
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(0xFFFF7FFF), (int16_t)0x8000); // below
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(INT32_MIN), (int16_t)0x8000);
}

TEST_F(Sample, ConversionHandlesPositiveClippingCorrectly)
{
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(0x00007FF4), (int16_t)0x7FF4); // most positive
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(0x00007FF5), (int16_t)0x7FF5); // clipping
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(0x00007FF6), (int16_t)0x7FF5); // above
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(0x00007FF7), (int16_t)0x7FF5);
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(0x00007FF8), (int16_t)0x7FF5);
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(0x00007FF9), (int16_t)0x7FF5);
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(0x00007FFA), (int16_t)0x7FF5);
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(0x00007FFB), (int16_t)0x7FF5);
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(0x00007FFC), (int16_t)0x7FF5);
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(0x00007FFD), (int16_t)0x7FF5);
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(0x00007FFE), (int16_t)0x7FF5);
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(0x00007FFF), (int16_t)0x7FF5);
  ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT(INT32_MAX), (int16_t)0x7FF5);
}

TEST_F(Sample, ConversionProvidesEqualValuesWhenInRange)
{
  for (int16_t sample = (INT16_MIN + 1); sample <= (INT16_MAX - 11); sample++)
    ASSERT_EQ(SAMPLE_CONVERT_TO_16_BIT((int32_t)sample), sample);
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
