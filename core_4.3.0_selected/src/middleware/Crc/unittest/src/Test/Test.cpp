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

#include "middleware/ICrc.h"

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

class Crc : public ::testing::Test
{
protected:

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
};

TEST_F(Crc, Crc16CanBeCalculatedFor1Byte)
{
  const uint8_t data[] = { 0x55 };
  ASSERT_EQ(CrcGetCrc16(data, sizeof(data)), 0xEBA0);
}

TEST_F(Crc, Crc16CanBeCalculatedFor16Byte)
{
  const uint8_t data[] = { 0x12, 0x34, 0x00, 0x78, 0x00, 0x34, 0x55, 0x7a, 0x12, 0x57, 0x00, 0xbb, 0xab, 0x34, 0x55, 0x7a };
  ASSERT_EQ(CrcGetCrc16(data, sizeof(data)), 0xA537);
}

TEST_F(Crc, Crc16CanBeCalculatedByteWiseAsWell)
{
  const uint8_t data[] = { 0x12, 0x34, 0x00, 0x78, 0x00, 0x34, 0x55, 0x7a, 0x12, 0x57, 0x00, 0xbb, 0xab, 0x34, 0x55, 0x7a };
  uint16_t crc_block = CrcGetCrc16(data, sizeof(data));
  uint16_t crc_single = 0;
  CrcInitCrc16(&crc_single);
  for (size_t index = 0; index < sizeof(data); index++)
    CrcUpdateCrc16(&data[index], 1, &crc_single);
  ASSERT_EQ(crc_single, crc_block);
}

TEST_F(Crc, Crc16CanBeCalculatedByteWiseFor100kByte)
{
  const uint8_t data[] = { 0 };
  uint16_t crc = 0;
  CrcInitCrc16(&crc);
  for (size_t byte = 0; byte < (100 * 1024); byte++)
    CrcUpdateCrc16(data, 1, &crc);
  ASSERT_EQ(crc, 0x8919);
}

TEST_F(Crc, Crc16WithCrcIncludedResultsInZero)
{
  const uint8_t data[] = { 0x12, 0x34, 0x00, 0x78, 0x00, 0x34, 0x55, 0x7a, 0x12, 0x57, 0x00, 0xbb, 0xab, 0x34, 0x55, 0x7a, 0xA5, 0x37 };
  ASSERT_EQ(CrcGetCrc16(data, sizeof(data)), 0);
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
