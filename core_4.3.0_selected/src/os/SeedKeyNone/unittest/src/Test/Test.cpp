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

#include "os/ISeedKey.h"

#include "gtest/gtest.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define MOCK_SEED_4 { 0x20, 0x21, 0x22, 0x23 }
#define MOCK_KEY_4_1 { 0x30, 0x31, 0x32, 0x33 }
#define MOCK_KEY_4_2 { 0x40, 0x41, 0x42, 0x43 }

#define MOCK_SEED_16 { \
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F }

#define MOCK_KEY_32_1 { \
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, \
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F }

#define MOCK_KEY_32_2 { \
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, \
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F }

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

class SeedKeyNone : public ::testing::Test
{
  protected:

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
};

TEST_F(SeedKeyNone, CanBeInitialized)
{
  SeedKeyInit();
}

TEST_F(SeedKeyNone, DummySeedCanBeCreatedWith4Byte)
{
  const uint8_t seed_expected[] = { 0x55, 0x55, 0x55, 0x55 };
  uint8_t seed[sizeof(seed_expected)];
  ASSERT_EQ(sizeof(seed), sizeof(seed_expected));
  ASSERT_EQ(SeedKeyCreateSeed(seed, sizeof(seed)), sizeof(seed));
  ASSERT_EQ(memcmp(seed, seed_expected, sizeof(seed)), 0);
}

TEST_F(SeedKeyNone, DummySeedCanBeCreatedWith16Byte)
{
  const uint8_t seed_expected[] = { 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55 };
  uint8_t seed[sizeof(seed_expected)];
  ASSERT_EQ(sizeof(seed), sizeof(seed_expected));
  ASSERT_EQ(SeedKeyCreateSeed(seed, sizeof(seed)), sizeof(seed));
  ASSERT_EQ(memcmp(seed, seed_expected, sizeof(seed)), 0);
}

TEST_F(SeedKeyNone, AllCombinationsAreValidForOperator)
{
  const uint8_t seed_4[] = MOCK_SEED_4;
  const uint8_t key_4_1[] = MOCK_KEY_4_1;
  const uint8_t key_4_2[] = MOCK_KEY_4_2;
  ASSERT_EQ(SeedKeyValidForOperator(seed_4, sizeof(seed_4), key_4_1, sizeof(key_4_1)), true);
  ASSERT_EQ(SeedKeyValidForOperator(seed_4, sizeof(seed_4), key_4_2, sizeof(key_4_2)), true);
  const uint8_t seed_32[] = MOCK_SEED_16;
  const uint8_t key_32_1[] = MOCK_KEY_32_1;
  const uint8_t key_32_2[] = MOCK_KEY_32_2;
  ASSERT_EQ(SeedKeyValidForOperator(seed_32, sizeof(seed_32), key_32_1, sizeof(key_32_1)), true);
  ASSERT_EQ(SeedKeyValidForOperator(seed_32, sizeof(seed_32), key_32_2, sizeof(key_32_2)), true);
}

TEST_F(SeedKeyNone, AllCombinationsAreValidForMaintenance)
{
  const uint8_t seed_4[] = MOCK_SEED_4;
  const uint8_t key_4_1[] = MOCK_KEY_4_1;
  const uint8_t key_4_2[] = MOCK_KEY_4_2;
  ASSERT_EQ(SeedKeyValidForMaintenance(seed_4, sizeof(seed_4), key_4_1, sizeof(key_4_1)), true);
  ASSERT_EQ(SeedKeyValidForMaintenance(seed_4, sizeof(seed_4), key_4_2, sizeof(key_4_2)), true);
  const uint8_t seed_32[] = MOCK_SEED_16;
  const uint8_t key_32_1[] = MOCK_KEY_32_1;
  const uint8_t key_32_2[] = MOCK_KEY_32_2;
  ASSERT_EQ(SeedKeyValidForMaintenance(seed_32, sizeof(seed_32), key_32_1, sizeof(key_32_1)), true);
  ASSERT_EQ(SeedKeyValidForMaintenance(seed_32, sizeof(seed_32), key_32_2, sizeof(key_32_2)), true);
}

TEST_F(SeedKeyNone, AllCombinationsAreValidForManufacturer)
{
  const uint8_t seed_4[] = MOCK_SEED_4;
  const uint8_t key_4_1[] = MOCK_KEY_4_1;
  const uint8_t key_4_2[] = MOCK_KEY_4_2;
  ASSERT_EQ(SeedKeyValidForManufacturer(seed_4, sizeof(seed_4), key_4_1, sizeof(key_4_1)), true);
  ASSERT_EQ(SeedKeyValidForManufacturer(seed_4, sizeof(seed_4), key_4_2, sizeof(key_4_2)), true);
  const uint8_t seed_32[] = MOCK_SEED_16;
  const uint8_t key_32_1[] = MOCK_KEY_32_1;
  const uint8_t key_32_2[] = MOCK_KEY_32_2;
  ASSERT_EQ(SeedKeyValidForManufacturer(seed_32, sizeof(seed_32), key_32_1, sizeof(key_32_1)), true);
  ASSERT_EQ(SeedKeyValidForManufacturer(seed_32, sizeof(seed_32), key_32_2, sizeof(key_32_2)), true);
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
