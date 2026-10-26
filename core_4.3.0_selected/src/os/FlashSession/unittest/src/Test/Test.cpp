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

#include "os/IFlashSession.h"

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

class FlashSession : public ::testing::Test
{
protected:

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
};

TEST_F(FlashSession, FlashSessionIsInactivePerDefault)
{
  ASSERT_EQ(FlashSessionIsActive(), false);
}

TEST_F(FlashSession, EraseFailsForNotEraseableAddressOrLength)
{
  ASSERT_NE(FlashSessionErase(MOCK_ADDRESS_ERASEABLE - 1, MOCK_NUMBER_OF_BYTES_ERASEABLE), ERROR_ID_SUCCESS);
  ASSERT_NE(FlashSessionErase(MOCK_ADDRESS_ERASEABLE + 1, MOCK_NUMBER_OF_BYTES_ERASEABLE), ERROR_ID_SUCCESS);
  ASSERT_NE(FlashSessionErase(MOCK_ADDRESS_ERASEABLE, MOCK_NUMBER_OF_BYTES_ERASEABLE - 1), ERROR_ID_SUCCESS);
  ASSERT_NE(FlashSessionErase(MOCK_ADDRESS_ERASEABLE, MOCK_NUMBER_OF_BYTES_ERASEABLE + 1), ERROR_ID_SUCCESS);
}

TEST_F(FlashSession, EraseIsPossibleForEraseableAddressAndLength)
{
  ASSERT_EQ(FlashSessionErase(MOCK_ADDRESS_ERASEABLE, MOCK_NUMBER_OF_BYTES_ERASEABLE), ERROR_ID_SUCCESS);
}

TEST_F(FlashSession, FlashSessionCanNotBeStartedForNotWriteableAddressOrLength)
{
  ASSERT_NE(FlashSessionStart(MOCK_ADDRESS_WRITEABLE - 1, MOCK_NUMBER_OF_BYTES_WRITEABLE), ERROR_ID_SUCCESS);
  ASSERT_NE(FlashSessionStart(MOCK_ADDRESS_WRITEABLE + 1, MOCK_NUMBER_OF_BYTES_WRITEABLE), ERROR_ID_SUCCESS);
  ASSERT_NE(FlashSessionStart(MOCK_ADDRESS_WRITEABLE, MOCK_NUMBER_OF_BYTES_WRITEABLE - 1), ERROR_ID_SUCCESS);
  ASSERT_NE(FlashSessionStart(MOCK_ADDRESS_WRITEABLE, MOCK_NUMBER_OF_BYTES_WRITEABLE + 1), ERROR_ID_SUCCESS);
}

TEST_F(FlashSession, FlashSessionCanBeStartedForWriteableAddressAndLength)
{
  ASSERT_EQ(FlashSessionIsActive(), false);
  ASSERT_EQ(FlashSessionStart(MOCK_ADDRESS_WRITEABLE, MOCK_NUMBER_OF_BYTES_WRITEABLE), ERROR_ID_SUCCESS);
  ASSERT_EQ(FlashSessionIsActive(), true);
}

TEST_F(FlashSession, FlashSessionCanBeAbortedWithAnInvalidStart)
{
  ASSERT_EQ(FlashSessionIsActive(), true);
  ASSERT_NE(FlashSessionStart(MOCK_ADDRESS_WRITEABLE + 1, MOCK_NUMBER_OF_BYTES_WRITEABLE), ERROR_ID_SUCCESS);
  ASSERT_EQ(FlashSessionIsActive(), false);
}

TEST_F(FlashSession, TransferFailsWithoutActiveSession)
{
  ASSERT_EQ(FlashSessionIsActive(), false);
  uint8_t data;
  ASSERT_EQ(FlashSessionTransfer(0, 1, &data), ERROR_ID_FLASH_NO_ACTIVE_SESSION);
  ASSERT_EQ(FlashSessionIsActive(), false);
}

TEST_F(FlashSession, ProgramFailsWithoutActiveSession)
{
  ASSERT_EQ(FlashSessionIsActive(), false);
  ASSERT_EQ(FlashSessionProgram(), ERROR_ID_FLASH_NO_ACTIVE_SESSION);
  ASSERT_EQ(FlashSessionIsActive(), false);
}

TEST_F(FlashSession, TransferFailsAndSessionAbortsWithUnexpectedIndex)
{
  ASSERT_EQ(FlashSessionIsActive(), false);
  ASSERT_EQ(FlashSessionStart(MOCK_ADDRESS_WRITEABLE, MOCK_NUMBER_OF_BYTES_WRITEABLE), ERROR_ID_SUCCESS);
  ASSERT_EQ(FlashSessionIsActive(), true);
  uint8_t data;
  ASSERT_EQ(FlashSessionTransfer(0, 1, &data), ERROR_ID_SUCCESS);
  ASSERT_EQ(FlashSessionTransfer(1, 1, &data), ERROR_ID_SUCCESS);
  ASSERT_EQ(FlashSessionTransfer(2, 1, &data), ERROR_ID_SUCCESS);
  ASSERT_EQ(FlashSessionIsActive(), true);
  ASSERT_EQ(FlashSessionTransfer(4, 1, &data), ERROR_ID_FLASH_UNEXPECTED_INDEX);
  ASSERT_EQ(FlashSessionIsActive(), false);
}

TEST_F(FlashSession, TransferFailsAndSessionAbortsIfTooMuchDataIsTransfered)
{
  ASSERT_EQ(FlashSessionIsActive(), false);
  ASSERT_EQ(FlashSessionStart(MOCK_ADDRESS_WRITEABLE, MOCK_NUMBER_OF_BYTES_WRITEABLE), ERROR_ID_SUCCESS);
  ASSERT_EQ(FlashSessionIsActive(), true);
  uint8_t data;
  uint16_t byte = 0;
  for (byte = 0; byte < MOCK_NUMBER_OF_BYTES_WRITEABLE; byte++)
    ASSERT_EQ(FlashSessionTransfer((uint8_t)byte, 1, &data), ERROR_ID_SUCCESS);
  ASSERT_EQ(FlashSessionIsActive(), true);
  ASSERT_EQ(FlashSessionTransfer((uint8_t)byte, 1, &data), ERROR_ID_FLASH_TOO_MUCH_DATA);
  ASSERT_EQ(FlashSessionIsActive(), false);
}

TEST_F(FlashSession, SessionAbortsAndProgramFailsIfNotEnoughDataHasBeenTransfered)
{
  ASSERT_EQ(FlashSessionIsActive(), false);
  ASSERT_EQ(FlashSessionStart(MOCK_ADDRESS_WRITEABLE, MOCK_NUMBER_OF_BYTES_WRITEABLE), ERROR_ID_SUCCESS);
  ASSERT_EQ(FlashSessionIsActive(), true);
  uint8_t data;
  for (uint16_t byte = 0; byte < (MOCK_NUMBER_OF_BYTES_WRITEABLE - 1); byte++)
    ASSERT_EQ(FlashSessionTransfer((uint8_t)byte, 1, &data), ERROR_ID_SUCCESS);
  ASSERT_EQ(FlashSessionIsActive(), true);
  ASSERT_EQ(FlashSessionProgram(), ERROR_ID_FLASH_NOT_ENOUGH_DATA);
  ASSERT_EQ(FlashSessionIsActive(), false);
}

TEST_F(FlashSession, FlashSessionCanBeCompletedWithRightAmountOfData)
{
  ASSERT_EQ(FlashSessionIsActive(), false);
  ASSERT_EQ(FlashSessionStart(MOCK_ADDRESS_WRITEABLE, MOCK_NUMBER_OF_BYTES_WRITEABLE), ERROR_ID_SUCCESS);
  ASSERT_EQ(FlashSessionIsActive(), true);
  uint8_t data;
  for (uint16_t byte = 0; byte < MOCK_NUMBER_OF_BYTES_WRITEABLE; byte++)
    ASSERT_EQ(FlashSessionTransfer((uint8_t)byte, 1, &data), ERROR_ID_SUCCESS);
  ASSERT_EQ(FlashSessionIsActive(), true);
  ASSERT_EQ(FlashSessionProgram(), ERROR_ID_SUCCESS);
  ASSERT_EQ(FlashSessionIsActive(), false);
}

TEST_F(FlashSession, CheckIsPossibleForCheckableAddressAndLength)
{
  uint16_t checksum = 0;
  ASSERT_EQ(FlashSessionCheck(MOCK_ADDRESS_CHECKABLE, MOCK_NUMBER_OF_BYTES_CHECKABLE, &checksum), ERROR_ID_SUCCESS);
  ASSERT_EQ(checksum, MOCK_CHECKSUM);
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
