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

#include "middleware/IError.h"
#include "middleware/IGeneric.h"
#include "os/IBlock.h"

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

class Block : public ::testing::Test
{
protected:

  void SetUp() override
  {
    MockResetCounters();
  }

  void TearDown() override
  {
  }
};

TEST_F(Block, ApplicationCanNotOverwriteItself)
{
  ASSERT_EQ(BlockRangeIsAvailableForErase(0xA000, 0x21000), ERROR_ID_FLASH_EXCEED_VALID_ADDRESS_RANGE);
  ASSERT_EQ(BlockErase(0xA000, 0x21000), ERROR_ID_FLASH_EXCEED_VALID_ADDRESS_RANGE);
  uint8_t dummy;
  ASSERT_EQ(BlockWrite(0xA000, 0x21000, &dummy), ERROR_ID_FLASH_EXCEED_VALID_ADDRESS_RANGE);
}

TEST_F(Block, ApplicationCanWriteWholeFlashloaderRangeButNotBeyond)
{
  ASSERT_EQ(BlockRangeIsAvailableForErase(0, 0xA000), ERROR_ID_SUCCESS);
  ASSERT_EQ(BlockRangeIsAvailableForErase(1, 0xA000), ERROR_ID_FLASH_EXCEED_VALID_ADDRESS_RANGE);
  ASSERT_EQ(BlockRangeIsAvailableForErase(0, 0xA001), ERROR_ID_FLASH_EXCEED_VALID_ADDRESS_RANGE);
}

TEST_F(Block, ApplicationWritesWholeFlashloader)
{
  ASSERT_EQ(BlockRangeIsAvailableForErase(0, 0xA000), ERROR_ID_SUCCESS);
  ASSERT_EQ(BlockErase(0, 0xA000), ERROR_ID_SUCCESS);
  ASSERT_EQ(MockGetFlashEraseCounter(), 20u);
  ASSERT_EQ(MockGetFlashWriteCounter(), 0u);
  uint8_t dummy;
  ASSERT_EQ(BlockWrite(0, 0xA000, &dummy), ERROR_ID_SUCCESS);
  ASSERT_EQ(MockGetFlashEraseCounter(), 20u);
  ASSERT_EQ(MockGetFlashWriteCounter(), 160u);
}

TEST_F(Block, ApplicationCanCheckFlashloader)
{
  ASSERT_EQ(BlockRangeIsAvailableForCheck(0, 0xA000), ERROR_ID_SUCCESS);
}

TEST_F(Block, ApplicationCanCheckApplication)
{
  ASSERT_EQ(BlockRangeIsAvailableForCheck(0xA000, 0x21000), ERROR_ID_SUCCESS);
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
