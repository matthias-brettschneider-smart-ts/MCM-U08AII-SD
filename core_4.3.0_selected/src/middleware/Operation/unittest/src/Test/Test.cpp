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

#include "middleware/IOperation.h"
#include "middleware/IOperationInternal.h"

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

class Operation : public ::testing::Test
{
protected:

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
};

TEST_F(Operation, IsNotRunningBeforeAndAfterInitialization)
{
  ASSERT_EQ(OperationIsRunning(), false);
  OperationInit();
  ASSERT_EQ(OperationIsRunning(), false);
}

TEST_F(Operation, CanBeStarted)
{
  ASSERT_EQ(OperationIsRunning(), false);
  OperationStart();
  ASSERT_EQ(OperationIsRunning(), true);
}

TEST_F(Operation, CanBeStopped)
{
  ASSERT_EQ(OperationIsRunning(), true);
  OperationStop();
  ASSERT_EQ(OperationIsRunning(), false);
}

TEST_F(Operation, AutoOperationIsOffPerDefault)
{
  OperationApplyAutoOperation();
  ASSERT_EQ(OperationIsRunning(), false);
}

TEST_F(Operation, AutoOperationDoesNotStopOperationIfAlreadyStarted)
{
  OperationStart();
  ASSERT_EQ(OperationIsRunning(), true);
  OperationApplyAutoOperation();
  ASSERT_EQ(OperationIsRunning(), true);
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
