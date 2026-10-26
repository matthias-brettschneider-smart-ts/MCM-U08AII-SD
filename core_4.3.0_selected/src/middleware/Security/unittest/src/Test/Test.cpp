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
#include "middleware/ISecurityInternal.h"

#include "IMock.h"

#include "gtest/gtest.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define TEST_SIZE_OF_SEED_IN_BYTE                                              4u
#define TEST_SIZE_OF_KEY_IN_BYTE                                               4u

#define TEST_BRUTE_FORCE_TIMEOUT_IN_MS                                      1000

typedef uint8_t TEST_SEED[TEST_SIZE_OF_SEED_IN_BYTE];
typedef uint8_t TEST_KEY[TEST_SIZE_OF_KEY_IN_BYTE];

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

class Security : public ::testing::Test
{
  protected:

  void SetUp() override
  {
    const TEST_KEY key = MOCK_KEY_INVALID;
    SecuritySetKey(key);
    ASSERT_EQ(SecuritySetLevel(SECURITY_LEVEL_ALL), ERROR_ID_SUCCESS);
    MockTimer32Increment(TEST_BRUTE_FORCE_TIMEOUT_IN_MS);
  }

  void TearDown() override
  {
  }
};

TEST_F(Security, IsConfiguredCorrectlyForThisTests)
{
  ASSERT_EQ(SecurityGetSizeOfSeed(), TEST_SIZE_OF_SEED_IN_BYTE);
  ASSERT_EQ(SecurityGetSizeOfKey(), TEST_SIZE_OF_KEY_IN_BYTE);
}

TEST_F(Security, TheInitialSecurityLevelIsAll)
{
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
}

TEST_F(Security, SettingProtectedSecurityLevelWithoutGettingSeedBeforeFailsWithNoSeedRequested)
{
  ASSERT_EQ(SecuritySetLevel(SECURITY_LEVEL_OPERATOR), ERROR_ID_NO_SEED_REQUESTED);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
  ASSERT_EQ(SecuritySetLevel(SECURITY_LEVEL_MAINTENANCE), ERROR_ID_NO_SEED_REQUESTED);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
  ASSERT_EQ(SecuritySetLevel(SECURITY_LEVEL_MANUFACTURER), ERROR_ID_NO_SEED_REQUESTED);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
}

TEST_F(Security, SettingSecurityLevelOperatorWithInvalidKeyFailsWithWrongKey)
{
  TEST_SEED seed;
  SecurityGetSeed(seed);
  ASSERT_EQ(SecuritySetLevel(SECURITY_LEVEL_OPERATOR), ERROR_ID_WRONG_KEY);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
}

TEST_F(Security, SettingSecurityLevelMaintenanceWithInvalidKeyFailsWithWrongKey)
{
  TEST_SEED seed;
  SecurityGetSeed(seed);
  ASSERT_EQ(SecuritySetLevel(SECURITY_LEVEL_MAINTENANCE), ERROR_ID_WRONG_KEY);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
}

TEST_F(Security, SettingSecurityLevelManufacturerWithInvalidKeyFailsWithWrongKey)
{
  TEST_SEED seed;
  SecurityGetSeed(seed);
  ASSERT_EQ(SecuritySetLevel(SECURITY_LEVEL_MANUFACTURER), ERROR_ID_WRONG_KEY);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
}

TEST_F(Security, ExpectedSeedIsProvidedAtTheFirstAttempt)
{
  TEST_SEED seed;
  const TEST_SEED seed_expected = MOCK_SEED_0;
  SecurityGetSeed(seed);
  ASSERT_EQ(memcmp(seed, seed_expected, TEST_SIZE_OF_SEED_IN_BYTE), 0);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
}

TEST_F(Security, SecurityLevelOperatorCanBeEnteredWithValidKeyAndCanBeLeftByExplicitLock)
{
  const TEST_KEY key = MOCK_KEY_OPERATOR;
  SecuritySetKey(key);
  ASSERT_EQ(SecuritySetLevel(SECURITY_LEVEL_OPERATOR), ERROR_ID_SUCCESS);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_OPERATOR);
  SecurityLock();
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
}

TEST_F(Security, ExpectedSeedIsProvidedAtTheSecondAttempt)
{
  TEST_SEED seed;
  const TEST_SEED seed_expected = MOCK_SEED_1;
  SecurityGetSeed(seed);
  ASSERT_EQ(memcmp(seed, seed_expected, TEST_SIZE_OF_SEED_IN_BYTE), 0);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
}

TEST_F(Security, SecurityLevelMaintenanceCanBeEnteredWithValidKeyAndCanBeLeftByExplicitLock)
{
  const TEST_KEY key = MOCK_KEY_MAINTENANCE;
  SecuritySetKey(key);
  ASSERT_EQ(SecuritySetLevel(SECURITY_LEVEL_MAINTENANCE), ERROR_ID_SUCCESS);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_MAINTENANCE);
  SecurityLock();
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
}

TEST_F(Security, ExpectedSeedIsProvidedAtTheThirdAttempt)
{
  TEST_SEED seed;
  const TEST_SEED seed_expected = MOCK_SEED_2;
  SecurityGetSeed(seed);
  ASSERT_EQ(memcmp(seed, seed_expected, TEST_SIZE_OF_SEED_IN_BYTE), 0);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
}

TEST_F(Security, SecurityLevelManufacturerCanBeEnteredWithValidKeyAndCanBeLeftByExplicitLock)
{
  const TEST_KEY key = MOCK_KEY_MANUFACTURER;
  SecuritySetKey(key);
  ASSERT_EQ(SecuritySetLevel(SECURITY_LEVEL_MANUFACTURER), ERROR_ID_SUCCESS);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_MANUFACTURER);
  SecurityLock();
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
}

TEST_F(Security, BruteForceAttackIsPreventedByTimeout)
{
  TEST_SEED seed;
  const TEST_SEED seed_expected = MOCK_SEED_3;
  SecurityGetSeed(seed);
  ASSERT_EQ(memcmp(seed, seed_expected, TEST_SIZE_OF_SEED_IN_BYTE), 0);
  ASSERT_EQ(SecuritySetLevel(SECURITY_LEVEL_MANUFACTURER), ERROR_ID_WRONG_KEY);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
  MockTimer32Increment(TEST_BRUTE_FORCE_TIMEOUT_IN_MS - 1);
  ASSERT_EQ(SecuritySetLevel(SECURITY_LEVEL_MANUFACTURER), ERROR_ID_WRONG_KEY);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_ALL);
  MockTimer32Increment(1);
  const TEST_KEY key_valid = MOCK_KEY_MANUFACTURER;
  SecuritySetKey(key_valid);
  ASSERT_EQ(SecuritySetLevel(SECURITY_LEVEL_MANUFACTURER), ERROR_ID_SUCCESS);
  ASSERT_EQ(SecurityGetCurrentLevel(), SECURITY_LEVEL_MANUFACTURER);
}

TEST_F(Security, ExpectedSeedIsProvidedAtTheFourthAttempt)
{
  TEST_SEED seed;
  const TEST_SEED seed_expected = MOCK_SEED_4;
  SecurityGetSeed(seed);
  ASSERT_EQ(memcmp(seed, seed_expected, TEST_SIZE_OF_SEED_IN_BYTE), 0);
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
