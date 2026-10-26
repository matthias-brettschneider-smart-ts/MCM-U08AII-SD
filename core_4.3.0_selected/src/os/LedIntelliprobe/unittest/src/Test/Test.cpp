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

#include "os/ILed.h"

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

class LedIntelliprobe : public ::testing::Test
{
  protected:

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
};

TEST_F(LedIntelliprobe, BothLedsAreInactiveByDefault)
{
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), false);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
}

TEST_F(LedIntelliprobe, GreenLedCanBeActivatedAndDeactivated)
{
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), false);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
  LED_SET(LED_COLOR_GREEN, LED_TODO_ACTIVATE);
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), true);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
  LED_SET(LED_COLOR_GREEN, LED_TODO_DEACTIVATE);
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), false);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
}

TEST_F(LedIntelliprobe, RedLedCanBeActivatedAndDeactivated)
{
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), false);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
  LED_SET(LED_COLOR_RED, LED_TODO_ACTIVATE);
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), false);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), true);
  LED_SET(LED_COLOR_RED, LED_TODO_DEACTIVATE);
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), false);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
}

TEST_F(LedIntelliprobe, LedsToggleEachOtherAndCanNotBeActivatedSimultaneously)
{
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), false);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
  LED_SET(LED_COLOR_GREEN, LED_TODO_ACTIVATE);
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), true);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
  LED_SET(LED_COLOR_RED, LED_TODO_ACTIVATE);
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), false);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), true);
  LED_SET(LED_COLOR_GREEN, LED_TODO_ACTIVATE);
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), true);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
  LED_SET(LED_COLOR_GREEN, LED_TODO_DEACTIVATE);
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), false);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
}

TEST_F(LedIntelliprobe, DoNothingDoesNotActivateOrDeactivateLeds)
{
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), false);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
  LED_SET(LED_COLOR_GREEN, LED_TODO_NOTHING);
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), false);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
  LED_SET(LED_COLOR_RED, LED_TODO_NOTHING);
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), false);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
  LED_SET(LED_COLOR_GREEN, LED_TODO_ACTIVATE);
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), true);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
  LED_SET(LED_COLOR_GREEN, LED_TODO_NOTHING);
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), true);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
  LED_SET(LED_COLOR_RED, LED_TODO_NOTHING);
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), true);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
  LED_SET(LED_COLOR_GREEN, LED_TODO_DEACTIVATE);
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), false);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
}

TEST_F(LedIntelliprobe, PermanentErrorActivatesRedLed)
{
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), false);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), false);
  LED_SIGNAL_PERMANENT_ERROR();
  ASSERT_EQ(MOCK_GREEN_IS_ACTIVE(), false);
  ASSERT_EQ(MOCK_RED_IS_ACTIVE(), true);
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
