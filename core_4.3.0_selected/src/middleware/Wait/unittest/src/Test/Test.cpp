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
#include "middleware/IWait.h"

#include "IMock.h"

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

class Wait : public ::testing::Test
{
  protected:

  void SetUp() override
  {
    MockResetSystemTimer();
  }

  void TearDown() override
  {
  }
};

TEST_F(Wait, MockIsConfiguredAsExpected)
{
  ASSERT_EQ(MOCK_SYSTEM_TIMER_TICKS_PER_US, 40);
}

TEST_F(Wait, TimerAreZeroByDefault)
{
  ASSERT_EQ(WAIT_GET_TIMER_VALUE_16(), 0u);
  ASSERT_EQ(WAIT_GET_TIMER_VALUE_32(), 0u);
}

TEST_F(Wait, AbsoluteTimeIsConverted)
{
  ASSERT_EQ(WAIT_GET_TICKS_FOR_US_16(1), 40u);
  ASSERT_EQ(WAIT_GET_TICKS_FOR_US_32(1), 40u);
  ASSERT_EQ(WAIT_GET_TICKS_FOR_MS_32(1), 40000u);
}

TEST_F(Wait, TimeoutCheckIncrementsForMicrosecondsAnd16Bit)
{
  const uint16_t reference = WAIT_GET_TIMER_VALUE_16();
  ASSERT_EQ(WAIT_CHECK_TIMEOUT_US_16(reference, 1), false);
  MockIncrementSystemTimer(40 - 1);
  ASSERT_EQ(WAIT_CHECK_TIMEOUT_US_16(reference, 1), false);
  MockIncrementSystemTimer(1);
  ASSERT_EQ(WAIT_CHECK_TIMEOUT_US_16(reference, 1), true);
}

TEST_F(Wait, TimeoutCheckIncrementsForMicrosecondsAnd32Bit)
{
  const uint16_t reference = WAIT_GET_TIMER_VALUE_32();
  ASSERT_EQ(WAIT_CHECK_TIMEOUT_US_32(reference, 1), false);
  MockIncrementSystemTimer(40 - 1);
  ASSERT_EQ(WAIT_CHECK_TIMEOUT_US_32(reference, 1), false);
  MockIncrementSystemTimer(1);
  ASSERT_EQ(WAIT_CHECK_TIMEOUT_US_32(reference, 1), true);
}

TEST_F(Wait, TimeoutCheckIncrementsForMillisecondsAnd32Bit)
{
  const uint16_t reference = WAIT_GET_TIMER_VALUE_32();
  ASSERT_EQ(WAIT_CHECK_TIMEOUT_MS_32(reference, 1), false);
  MockIncrementSystemTimer(40000 - 1);
  ASSERT_EQ(WAIT_CHECK_TIMEOUT_MS_32(reference, 1), false);
  MockIncrementSystemTimer(1);
  ASSERT_EQ(WAIT_CHECK_TIMEOUT_MS_32(reference, 1), true);
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
