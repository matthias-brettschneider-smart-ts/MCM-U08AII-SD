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

#include "middleware/IConfig.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"
#include "middleware/IOperation.h"
#include "middleware/IOperationInternal.h"
#include "middleware/ISecurity.h"
#include "os/IMode.h"

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

class Mode : public ::testing::Test
{
protected:

  void SetUp() override
  {
    MockResetSecurityLevel();
    MockRejectKey();
    OperationStop();
    ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
    ASSERT_EQ(OperationIsRunning(), false);
  }

  void TearDown() override
  {
  }
};

TEST_F(Mode, ModeCanNotBeSetToIllegal)
{
  ASSERT_EQ(ModeGet(), MODE_ID_CONFIG);
  ASSERT_EQ(ModeSet(MODE_ID_ILLEGAL), ERROR_ID_VALUE_OUT_OF_RANGE);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
  ASSERT_EQ(OperationIsRunning(), false);
}

TEST_F(Mode, ModeCanNotBeChangedToAdvancedCustomerIfKeyIsRejected)
{
  ASSERT_EQ(ModeGet(), MODE_ID_CONFIG);
  ASSERT_EQ(ModeSet(MODE_ID_ADVANCED_CUSTOMER), ERROR_ID_WRONG_KEY);
  ASSERT_EQ(ModeGet(), MODE_ID_CONFIG);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
  ASSERT_EQ(OperationIsRunning(), false);
}

TEST_F(Mode, ModeCanNotBeChangedToAdvancedSmartIfKeyIsRejected)
{
  ASSERT_EQ(ModeGet(), MODE_ID_CONFIG);
  ASSERT_EQ(ModeSet(MODE_ID_ADVANCED_SMART), ERROR_ID_WRONG_KEY);
  ASSERT_EQ(ModeGet(), MODE_ID_CONFIG);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
  ASSERT_EQ(OperationIsRunning(), false);
}

TEST_F(Mode, ModeCanBeChangedToOperation)
{
  MockAcceptKey();
  ASSERT_EQ(ModeGet(), MODE_ID_CONFIG);
  ASSERT_EQ(ModeSet(MODE_ID_OPERATION), ERROR_ID_SUCCESS);
  ASSERT_EQ(ModeGet(), MODE_ID_OPERATION);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
  ASSERT_EQ(OperationIsRunning(), true);
}

TEST_F(Mode, ModeCanBeChangedToConfig)
{
  MockAcceptKey();
  ASSERT_EQ(ModeGet(), MODE_ID_CONFIG);
  ASSERT_EQ(ModeSet(MODE_ID_CONFIG), ERROR_ID_SUCCESS);
  ASSERT_EQ(ModeGet(), MODE_ID_CONFIG);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
  ASSERT_EQ(OperationIsRunning(), false);
}

TEST_F(Mode, ModeCanBeChangedToAdvancedCustomerIfKeyIsAccepted)
{
  MockAcceptKey();
  ASSERT_EQ(ModeSet(MODE_ID_ADVANCED_CUSTOMER), ERROR_ID_SUCCESS);
  ASSERT_EQ(ModeGet(), MODE_ID_ADVANCED_CUSTOMER);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_MAINTENANCE);
  ASSERT_EQ(OperationIsRunning(), false);
}

TEST_F(Mode, ModeCanBeChangedToAdvancedSmartIfKeyIsAccepted)
{
  MockAcceptKey();
  ASSERT_EQ(ModeSet(MODE_ID_ADVANCED_SMART), ERROR_ID_SUCCESS);
  ASSERT_EQ(ModeGet(), MODE_ID_ADVANCED_SMART);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_MANUFACTURER);
  ASSERT_EQ(OperationIsRunning(), false);
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
