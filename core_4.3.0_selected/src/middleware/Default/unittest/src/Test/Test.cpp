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

#include "middleware/IDefault.h"
#include "middleware/IDefaultInternal.h"

#include "IMock.h"

#include "gtest/gtest.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static uint8_t TestCallbackCounter[3];

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static DEFAULT_CALLBACK(TestDefaultCallback0);
static DEFAULT_CALLBACK(TestDefaultCallback1);
static DEFAULT_CALLBACK(TestDefaultCallback2);
static DEFAULT_CALLBACK(TestDefaultCallback3);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

// --- helpers -----------------------------------------------------------------

static DEFAULT_CALLBACK(TestDefaultCallback0)
{
  TestCallbackCounter[0]++;
}

static DEFAULT_CALLBACK(TestDefaultCallback1)
{
  TestCallbackCounter[1]++;
}

static DEFAULT_CALLBACK(TestDefaultCallback2)
{
  TestCallbackCounter[2]++;
}

static DEFAULT_CALLBACK(TestDefaultCallback3)
{
  ASSERT_TRUE(0);
}

// --- tests -------------------------------------------------------------------

class Default : public ::testing::Test
{
protected:

  void SetUp() override
  {
    ASSERT_EQ(MockErrorGetNumberAndReset(), 0);
  }

  void TearDown() override
  {
    ASSERT_EQ(MockErrorGetNumberAndReset(), 0);
  }
};

TEST_F(Default, SubscriberCanRegisterAndIsCalledBack)
{
  DefaultRegister(TestDefaultCallback0);
  DefaultReset();
  ASSERT_EQ(MockErrorGetNumberAndReset(), 0);
  ASSERT_EQ(TestCallbackCounter[0], 1);
  ASSERT_EQ(TestCallbackCounter[1], 0);
  ASSERT_EQ(TestCallbackCounter[2], 0);
}

TEST_F(Default, SameCallbackCanNotBeRegisteredTwice)
{
  DefaultRegister(TestDefaultCallback1);
  ASSERT_EQ(MockErrorGetNumberAndReset(), 0);
  DefaultRegister(TestDefaultCallback1);
  ASSERT_GT(MockErrorGetNumberAndReset(), 0);
  DefaultReset();
  ASSERT_EQ(TestCallbackCounter[0], 2);
  ASSERT_EQ(TestCallbackCounter[1], 1);
  ASSERT_EQ(TestCallbackCounter[2], 0);
}

TEST_F(Default, AllAvailableSubscriptionsCanBeUsed)
{
  DefaultRegister(TestDefaultCallback2);
  ASSERT_EQ(MockErrorGetNumberAndReset(), 0);
  DefaultReset();
  ASSERT_EQ(TestCallbackCounter[0], 3);
  ASSERT_EQ(TestCallbackCounter[1], 2);
  ASSERT_EQ(TestCallbackCounter[2], 1);
}

TEST_F(Default, NoMoreSubscriptionThanAvailableCanBeUsed)
{
  DefaultRegister(TestDefaultCallback3);
  ASSERT_GT(MockErrorGetNumberAndReset(), 0);
  DefaultReset();
  ASSERT_EQ(TestCallbackCounter[0], 4);
  ASSERT_EQ(TestCallbackCounter[1], 3);
  ASSERT_EQ(TestCallbackCounter[2], 2);
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
