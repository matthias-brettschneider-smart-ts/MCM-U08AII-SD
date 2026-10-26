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

#include "middleware/IMemory.h"

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

class Memory : public ::testing::Test
{
protected:

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
};

TEST_F(Memory, MemoryCanBeCopiedInReverseByteOrder)
{
  const uint8_t from[3] = { 0x11, 0x12, 0x13 };
  uint8_t to[sizeof(from)];
  memset(to, 0, sizeof(to));
  ASSERT_EQ(to[0], 0);
  ASSERT_EQ(to[1], 0);
  ASSERT_EQ(to[2], 0);
  MemoryCopyReverse(to, from, sizeof(to));
  ASSERT_EQ(to[0], 0x13u);
  ASSERT_EQ(to[1], 0x12u);
  ASSERT_EQ(to[2], 0x11u);
}

TEST_F(Memory, ReverseCopyDoesNotAffectMemoryBeyondExpectedRange)
{
  const uint8_t from[] = { 0x11, 0x12, 0x13 };
  uint8_t to[] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
  MemoryCopyReverse(&to[1], from, sizeof(from));
  ASSERT_EQ(to[0], 0xAAu);
  ASSERT_EQ(to[1], 0x13u);
  ASSERT_EQ(to[2], 0x12u);
  ASSERT_EQ(to[3], 0x11u);
  ASSERT_EQ(to[4], 0xAAu);
}

TEST_F(Memory, BiggerChunkCanBeCopiedInReverseByteOrder)
{
  uint8_t from[256];
  uint8_t expected[256];
  for (size_t i = 0; i < sizeof(from); i++) {
    from[i] = i;
    expected[i] = 255 - i;
  }

  uint8_t to[256];
  memset(to, 0, sizeof(to));

  MemoryCopyReverse(to, from, sizeof(from));
  ASSERT_EQ(memcmp(to, expected, sizeof(expected)), 0);
}

TEST_F(Memory, Uint16CanBeSwapped)
{
  ASSERT_EQ(MEMORY_SWAP_UINT16(0x1234), 0x3412u);
}

TEST_F(Memory, Uint16CanBeRead)
{
  const uint8_t from[] = { 0xAA, 0xBB };
  ASSERT_EQ(MEMORY_GET_UINT16(from), 0xBBAAu);
}

TEST_F(Memory, Uint16CanBeReadFromOddAddress)
{
  const uint8_t from[] = { 0xAA, 0xBB, 0xCC, 0xDD };
  ASSERT_EQ(MEMORY_GET_UINT16(&from[1]), 0xCCBBu);
}

TEST_F(Memory, Uint16CanBeReadReverse)
{
  const uint8_t from[] = { 0xAA, 0xBB };
  ASSERT_EQ(MEMORY_GET_UINT16_REVERSE(from), 0xAABBu);
}

TEST_F(Memory, Uint16CanBeReadReverseFromOddAddress)
{
  const uint8_t from[] = { 0xAA, 0xBB, 0xCC, 0xDD };
  ASSERT_EQ(MEMORY_GET_UINT16_REVERSE(&from[1]), 0xBBCCu);
}

TEST_F(Memory, Uint32CanBeSwapped)
{
  ASSERT_EQ(MEMORY_SWAP_UINT32(0x12345678), 0x78563412u);
}

TEST_F(Memory, Uint32CanBeRead)
{
  const uint8_t from[] = { 0xAA, 0xBB, 0xCC, 0xDD };
  ASSERT_EQ(MEMORY_GET_UINT32(from), 0xDDCCBBAAu);
}

TEST_F(Memory, Uint32CanBeReadFromOddAddress)
{
  const uint8_t from[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
  ASSERT_EQ(MEMORY_GET_UINT32(&from[1]), 0xEEDDCCBBu);
}

TEST_F(Memory, Uint32CanBeReadReverse)
{
  const uint8_t from[] = { 0xAA, 0xBB, 0xCC, 0xDD };
  ASSERT_EQ(MEMORY_GET_UINT32_REVERSE(from), 0xAABBCCDDu);
}

TEST_F(Memory, Uint32CanBeReadReverseFromOddAddress)
{
  const uint8_t from[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
  ASSERT_EQ(MEMORY_GET_UINT32_REVERSE(&from[1]), 0xBBCCDDEEu);
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
