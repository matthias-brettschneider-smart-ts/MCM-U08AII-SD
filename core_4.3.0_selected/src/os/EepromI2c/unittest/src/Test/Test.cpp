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
#include "os/IEeprom.h"

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

class EepromI2c : public ::testing::Test
{
  protected:

  void SetUp() override
  {
    MockI2cAvailable();
    MockI2cSequenceSucceeds();
    MockResetLog();
  }

  void TearDown() override
  {
  }
};

TEST_F(EepromI2c, IsConfiguredCorrectlyForTheFollowingTests)
{
  ASSERT_EQ(EepromGetPageSize(), 32);
  ASSERT_EQ(EepromGetEepromSize(), 4096);
}

TEST_F(EepromI2c, CanBeInitialized)
{
  ASSERT_EQ(EepromInit(), ERROR_ID_SUCCESS);
}

TEST_F(EepromI2c, EepromAccessFailsTemporarilyIfI2cIsNotAvailable)
{
  uint8_t data[32];
  memset(data, 0, sizeof(data));
  MockI2cUnavailable();
  ASSERT_EQ(EepromRead(0x0000, data, sizeof(data)), ERROR_ID_TEMPORARILY_UNAVAILABLE);
  ASSERT_EQ(EepromWrite(0x0000, data, sizeof(data)), ERROR_ID_TEMPORARILY_UNAVAILABLE);
}

TEST_F(EepromI2c, EepromAccessFailsIfI2cSequenceFails)
{
  uint8_t data[32];
  memset(data, 0, sizeof(data));
  MockI2cSequenceFails();
  ASSERT_EQ(EepromRead(0x0000, data, sizeof(data)), ERROR_ID_FAILED);
  ASSERT_EQ(EepromWrite(0x0000, data, sizeof(data)), ERROR_ID_FAILED);
}

TEST_F(EepromI2c, PageCanBeReadFromEeprom)
{
  uint8_t data[32];
  memset(data, 0, sizeof(data));
  ASSERT_EQ(EepromRead(0x0000, data, sizeof(data)), ERROR_ID_SUCCESS);
  const char expected[] = "start: device 0x57, address 0x0000, 0x0020 bytes\nupdate: read\n";
  ASSERT_EQ(MockLogIsEqual(expected, strlen(expected)), true);
}

TEST_F(EepromI2c, PageCanBeWrittenToEeprom)
{
  uint8_t data[32];
  memset(data, 0, sizeof(data));
  ASSERT_EQ(EepromWrite(0x0000, data, sizeof(data)), ERROR_ID_SUCCESS);
  const char expected[] = "start: device 0x57, address 0x0000, 0x0020 bytes\nupdate: write\n";
  ASSERT_EQ(MockLogIsEqual(expected, strlen(expected)), true);
}

TEST_F(EepromI2c, WholeEepromCanBeRead)
{
  uint8_t data[4096];
  memset(data, 0, sizeof(data));
  ASSERT_EQ(EepromRead(0x0000, data, sizeof(data)), ERROR_ID_SUCCESS);
}

TEST_F(EepromI2c, WholeEepromCanBeWritten)
{
  uint8_t data[4096];
  memset(data, 0, sizeof(data));
  ASSERT_EQ(EepromWrite(0x0000, data, sizeof(data)), ERROR_ID_SUCCESS);
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
