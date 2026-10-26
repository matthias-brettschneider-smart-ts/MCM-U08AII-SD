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

#include "hal/II2cSequence.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"

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

class I2cSequence : public ::testing::Test
{
  protected:

  void SetUp() override
  {
    MockResetTransmission();
  }

  void TearDown() override
  {
  }
};

TEST_F(I2cSequence, CanReadWithoutDataAddress)
{
  t_I2C_SEQUENCE sequence;
  uint8_t buffer[1];
  I2cSequenceStart(&sequence, 0x57, 0x0000, sizeof(buffer), buffer);
  while (I2C_SEQUENCE_UPDATE(0, I2C_SEQUENCE_MODE_READ_WITHOUT_DATA_ADDRESS, &sequence));
  ASSERT_EQ(I2cSequenceGetState(&sequence), I2C_SEQUENCE_STATE_SUCCEEDED);
  const char * expected = "[.>AF.+<.55-.].";
  ASSERT_EQ(MockTransmissionIsEqual(expected, strlen(expected)), true);
}

TEST_F(I2cSequence, CanReadWithOneByteDataAddress)
{
  t_I2C_SEQUENCE sequence;
  uint8_t buffer[1];
  I2cSequenceStart(&sequence, 0x57, 0x0034, sizeof(buffer), buffer);
  while (I2C_SEQUENCE_UPDATE(0, I2C_SEQUENCE_MODE_READ_WITH_ONE_BYTE_DATA_ADDRESS, &sequence));
  ASSERT_EQ(I2cSequenceGetState(&sequence), I2C_SEQUENCE_STATE_SUCCEEDED);
  const char * expected = "[.>AE.+>34.+|.>AF.+<.55-.].";
  ASSERT_EQ(MockTransmissionIsEqual(expected, strlen(expected)), true);
}

TEST_F(I2cSequence, CanReadWithTwoByteDataAddress)
{
  t_I2C_SEQUENCE sequence;
  uint8_t buffer[1];
  I2cSequenceStart(&sequence, 0x57, 0x1234, sizeof(buffer), buffer);
  while (I2C_SEQUENCE_UPDATE(0, I2C_SEQUENCE_MODE_READ_WITH_TWO_BYTE_DATA_ADDRESS, &sequence));
  ASSERT_EQ(I2cSequenceGetState(&sequence), I2C_SEQUENCE_STATE_SUCCEEDED);
  const char * expected = "[.>AE.+>12.+>34.+|.>AF.+<.55-.].";
  ASSERT_EQ(MockTransmissionIsEqual(expected, strlen(expected)), true);
}

TEST_F(I2cSequence, CanWriteWithoutDataAddress)
{
  t_I2C_SEQUENCE sequence;
  uint8_t buffer[1];
  I2cSequenceStart(&sequence, 0x57, 0x0000, sizeof(buffer), buffer);
  while (I2C_SEQUENCE_UPDATE(0, I2C_SEQUENCE_MODE_WRITE_WITHOUT_DATA_ADDRESS, &sequence));
  ASSERT_EQ(I2cSequenceGetState(&sequence), I2C_SEQUENCE_STATE_SUCCEEDED);
  const char * expected = "[.>AE.+>00.+].";
  ASSERT_EQ(MockTransmissionIsEqual(expected, strlen(expected)), true);
}

TEST_F(I2cSequence, CanWriteWithOneByteDataAddress)
{
  t_I2C_SEQUENCE sequence;
  uint8_t buffer[1];
  I2cSequenceStart(&sequence, 0x57, 0x0034, sizeof(buffer), buffer);
  while (I2C_SEQUENCE_UPDATE(0, I2C_SEQUENCE_MODE_WRITE_WITH_ONE_BYTE_DATA_ADDRESS, &sequence));
  ASSERT_EQ(I2cSequenceGetState(&sequence), I2C_SEQUENCE_STATE_SUCCEEDED);
  const char * expected = "[.>AE.+>34.+>00.+].";
  ASSERT_EQ(MockTransmissionIsEqual(expected, strlen(expected)), true);
}

TEST_F(I2cSequence, CanWriteWithTwoByteDataAddress)
{
  t_I2C_SEQUENCE sequence;
  uint8_t buffer[1];
  I2cSequenceStart(&sequence, 0x57, 0x1234, sizeof(buffer), buffer);
  while (I2C_SEQUENCE_UPDATE(0, I2C_SEQUENCE_MODE_WRITE_WITH_TWO_BYTE_DATA_ADDRESS, &sequence));
  ASSERT_EQ(I2cSequenceGetState(&sequence), I2C_SEQUENCE_STATE_SUCCEEDED);
  const char * expected = "[.>AE.+>12.+>34.+>00.+].";
  ASSERT_EQ(MockTransmissionIsEqual(expected, strlen(expected)), true);
}

TEST_F(I2cSequence, CanReadOneByte)
{
  t_I2C_SEQUENCE sequence;
  uint8_t buffer[1];
  I2cSequenceStart(&sequence, 0x57, 0x1234, sizeof(buffer), buffer);
  while (I2C_SEQUENCE_UPDATE(0, I2C_SEQUENCE_MODE_READ_WITH_TWO_BYTE_DATA_ADDRESS, &sequence));
  ASSERT_EQ(I2cSequenceGetState(&sequence), I2C_SEQUENCE_STATE_SUCCEEDED);
  const char * expected = "[.>AE.+>12.+>34.+|.>AF.+<.55-.].";
  ASSERT_EQ(MockTransmissionIsEqual(expected, strlen(expected)), true);
}

TEST_F(I2cSequence, CanReadThreeByte)
{
  t_I2C_SEQUENCE sequence;
  uint8_t buffer[3];
  I2cSequenceStart(&sequence, 0x57, 0x1234, sizeof(buffer), buffer);
  while (I2C_SEQUENCE_UPDATE(0, I2C_SEQUENCE_MODE_READ_WITH_TWO_BYTE_DATA_ADDRESS, &sequence));
  ASSERT_EQ(I2cSequenceGetState(&sequence), I2C_SEQUENCE_STATE_SUCCEEDED);
  const char * expected = "[.>AE.+>12.+>34.+|.>AF.+<.55+.<.55+.<.55-.].";
  ASSERT_EQ(MockTransmissionIsEqual(expected, strlen(expected)), true);
}

TEST_F(I2cSequence, CanRead32Byte)
{
  t_I2C_SEQUENCE sequence;
  uint8_t buffer[32];
  I2cSequenceStart(&sequence, 0x57, 0x1234, sizeof(buffer), buffer);
  while (I2C_SEQUENCE_UPDATE(0, I2C_SEQUENCE_MODE_READ_WITH_TWO_BYTE_DATA_ADDRESS, &sequence));
  ASSERT_EQ(I2cSequenceGetState(&sequence), I2C_SEQUENCE_STATE_SUCCEEDED);
  const char * expected = "[.>AE.+>12.+>34.+|.>AF.+<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55+.<.55-.].";
  ASSERT_EQ(MockTransmissionIsEqual(expected, strlen(expected)), true);
}

TEST_F(I2cSequence, CanWriteOneByte)
{
  t_I2C_SEQUENCE sequence;
  uint8_t buffer[1] = { 0xA0 };
  I2cSequenceStart(&sequence, 0x57, 0x1234, sizeof(buffer), buffer);
  while (I2C_SEQUENCE_UPDATE(0, I2C_SEQUENCE_MODE_WRITE_WITH_TWO_BYTE_DATA_ADDRESS, &sequence));
  ASSERT_EQ(I2cSequenceGetState(&sequence), I2C_SEQUENCE_STATE_SUCCEEDED);
  const char * expected = "[.>AE.+>12.+>34.+>A0.+].";
  ASSERT_EQ(MockTransmissionIsEqual(expected, strlen(expected)), true);
}

TEST_F(I2cSequence, CanWriteThreeByte)
{
  t_I2C_SEQUENCE sequence;
  uint8_t buffer[3] = { 0xA0, 0xA1, 0xA2 };
  I2cSequenceStart(&sequence, 0x57, 0x1234, sizeof(buffer), buffer);
  while (I2C_SEQUENCE_UPDATE(0, I2C_SEQUENCE_MODE_WRITE_WITH_TWO_BYTE_DATA_ADDRESS, &sequence));
  ASSERT_EQ(I2cSequenceGetState(&sequence), I2C_SEQUENCE_STATE_SUCCEEDED);
  const char * expected = "[.>AE.+>12.+>34.+>A0.+>A1.+>A2.+].";
  ASSERT_EQ(MockTransmissionIsEqual(expected, strlen(expected)), true);
}

TEST_F(I2cSequence, CanWrite32Byte)
{
  t_I2C_SEQUENCE sequence;
  uint8_t buffer[32] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF };
  I2cSequenceStart(&sequence, 0x57, 0x1234, sizeof(buffer), buffer);
  while (I2C_SEQUENCE_UPDATE(0, I2C_SEQUENCE_MODE_WRITE_WITH_TWO_BYTE_DATA_ADDRESS, &sequence));
  ASSERT_EQ(I2cSequenceGetState(&sequence), I2C_SEQUENCE_STATE_SUCCEEDED);
  const char * expected = "[.>AE.+>12.+>34.+>A0.+>A1.+>A2.+>A3.+>A4.+>A5.+>A6.+>A7.+>A8.+>A9.+>AA.+>AB.+>AC.+>AD.+>AE.+>AF.+>B0.+>B1.+>B2.+>B3.+>B4.+>B5.+>B6.+>B7.+>B8.+>B9.+>BA.+>BB.+>BC.+>BD.+>BE.+>BF.+].";
  ASSERT_EQ(MockTransmissionIsEqual(expected, strlen(expected)), true);
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
