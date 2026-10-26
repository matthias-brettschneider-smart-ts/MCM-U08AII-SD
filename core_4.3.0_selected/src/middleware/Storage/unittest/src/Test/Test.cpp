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
#include "middleware/ISecurity.h"
#include "middleware/IStorage.h"
#include "middleware/IStorageInternal.h"

#include "IMock.h"

#include "gtest/gtest.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define TEST_NUMBER_OF_POSSIBLE_REGISTRATIONS                                 14

typedef struct {
  int number_of_registrations;
  uint8_t  data1;
  uint16_t data2;
  uint32_t data3;
  uint32_t data4;
} t_TEST_STORAGE_DATA;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_TEST_STORAGE_DATA test = {
    .number_of_registrations = 0,
    .data1 = 0x11,
    .data2 = 0xABCDu,
    .data3 = 0x12345678ul,
    .data4 = 0x01020304ul
};

static const t_STORAGE_ENTRY test_storage[] = {
    { .storage_id = STORAGE_ID_DATA1,     .storage_block = STORAGE_BLOCK_CORE_IDENTIFICATION,     .data_pointer = (void*)&test.data1, .size = sizeof(test.data1) },
    { .storage_id = STORAGE_ID_DATA2,     .storage_block = STORAGE_BLOCK_CORE_SETTINGS,           .data_pointer = (void*)&test.data2, .size = sizeof(test.data2) },
    { .storage_id = STORAGE_ID_DATA3,     .storage_block = STORAGE_BLOCK_MODULE_SETTINGS,         .data_pointer = (void*)&test.data3, .size = sizeof(test.data3) },
    { .storage_id = STORAGE_ID_DATA4,     .storage_block = STORAGE_BLOCK_MODULE_ADJUSTMENT_SMART, .data_pointer = (void*)&test.data4, .size = sizeof(test.data4) },
    { .storage_id = STORAGE_ID_UNDEFINED, .storage_block = STORAGE_BLOCK_UNDEFINED,               .data_pointer = NULL,               .size = 0                  }
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

class Storage : public ::testing::Test
{
  protected:

  void SetUp() override
  {
    MockSecuritySetLevel(SECURITY_LEVEL_ALL);
    ASSERT_EQ(MockErrorGetNumberAndReset(), 0);
  }

  void TearDown() override
  {
    ASSERT_EQ(MockErrorGetNumberAndReset(), 0);
  }
};

TEST_F(Storage, CanBeInitialized)
{
  StorageInit();
}

TEST_F(Storage, ConfigurationCanBeRegistered)
{
  StorageRegister(test_storage);
  test.number_of_registrations++;
}

TEST_F(Storage, LoadingDataFromBlocksThatHaveNeverBeenStoredLeadsToAnErrorEntryForEachBlock)
{
  StorageLoad(test_storage);
  ASSERT_EQ(test.data1, 0x11);
  ASSERT_EQ(test.data2, 0xABCDu);
  ASSERT_EQ(test.data3, 0x12345678u);
  ASSERT_EQ(test.data4, 0x01020304u);
  ASSERT_EQ(MockErrorGetNumberAndReset(), 4);
}

TEST_F(Storage, StoreSettingsAtSecurityLevelAllLeadsToAnErrorEntryForEachBlockThatIsNotAllowedToBeStored)
{
  StorageStoreSettings();
  ASSERT_EQ(MockErrorGetNumberAndReset(), 3);
}

TEST_F(Storage, ValuesFromTwoBlocksCanBeLoadedAtSecurityLevelAllAndTwoCanNotBeFoundBecauseTheyAreNotStoredYet)
{
  test.data1 = 0u;
  test.data2 = 0u;
  test.data3 = 0u;
  test.data4 = 0u;
  StorageLoad(test_storage);
  ASSERT_EQ(test.data1, 0u);
  ASSERT_EQ(test.data2, 0xABCDu);
  ASSERT_EQ(test.data3, 0x12345678u);
  ASSERT_EQ(test.data4, 0u);
  test.data1 = 0x11u;
  test.data4 = 0x01020304u;
  ASSERT_EQ(MockErrorGetNumberAndReset(), 2);
}

TEST_F(Storage, StoreSettingsCanBeExecutedAsManufacturer)
{
  MockSecuritySetLevel(SECURITY_LEVEL_MANUFACTURER);
  StorageStoreSettings();
}

TEST_F(Storage, ValuesFromAllBlocksCanBeLoaded)
{
  MockSecuritySetLevel(SECURITY_LEVEL_ALL);
  test.data1 = 0u;
  test.data2 = 0u;
  test.data3 = 0u;
  test.data4 = 0u;
  StorageLoad(test_storage);
  ASSERT_EQ(test.data1, 0x11u);
  ASSERT_EQ(test.data2, 0xABCDu);
  ASSERT_EQ(test.data3, 0x12345678u);
  ASSERT_EQ(test.data4, 0x01020304u);
}

TEST_F(Storage, RegisteringTheSameConfigurationTwiceLeadsToAnErrorEntryForEachRedundantAttempt)
{
  for (int registration = 0; registration < 3; registration++)
    StorageRegister(test_storage);
  ASSERT_EQ(MockErrorGetNumberAndReset(), 3);
}

TEST_F(Storage, RegisteringMoreConfigurationsThanAllowedLeadsToAnErrorEntryForEachOverflowAttempt)
{
  while (test.number_of_registrations < (TEST_NUMBER_OF_POSSIBLE_REGISTRATIONS + 5)) {
    t_STORAGE_ENTRY * empty_list = new t_STORAGE_ENTRY;
    empty_list->storage_id = STORAGE_ID_UNDEFINED;
    empty_list->storage_block = STORAGE_BLOCK_UNDEFINED;
    empty_list->data_pointer = NULL;
    empty_list->size = 0;
    StorageRegister(empty_list);
    test.number_of_registrations++;
  }
  ASSERT_EQ(test.number_of_registrations, TEST_NUMBER_OF_POSSIBLE_REGISTRATIONS + 5);
  ASSERT_EQ(MockErrorGetNumberAndReset(), 5);
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
