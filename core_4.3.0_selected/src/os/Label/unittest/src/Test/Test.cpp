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

#include "middleware/IConfigInternal.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"
#include "os/ILabel.h"
#include "os/IMode.h"
#include "project/ILabelConfig.h"

#include "IMock.h"

#include "gtest/gtest.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef enum {
  TEST_LABEL_ID_UNKNOWN,
  TEST_LABEL_ID_NOT_READABLE,
  TEST_LABEL_ID_NOT_WRITEABLE,
  TEST_LABEL_ID_SINGLECAST_SET_NOT_ALLOWED,
  TEST_LABEL_ID_NOT_KNOWN_BY_CONFIG,
  TEST_LABEL_ID_BOOL,
  TEST_LABEL_ID_BOOL_ARRAY,
  TEST_LABEL_ID_INT8,
  TEST_LABEL_ID_INT16,
  TEST_LABEL_ID_UINT32_WITH_ONE_SUBLABEL,
  TEST_LABEL_ID_UINT32_WITHOUT_SUBLABEL,
  TEST_LABEL_ID_VERSION,
  TEST_LABEL_ID_DATE,
  TEST_LABEL_ID_RAW32
} e_TEST_LABEL_ID;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

const t_LABEL_CONFIG label_config[] = {
    { .label_id = TEST_LABEL_ID_NOT_READABLE,               .sublabel_id = 0, .singlecast_set_allowed = true,   .get_minimum = MODE_ID_ILLEGAL,    .set_minimum = MODE_ID_OPERATION, .label_type = LABEL_TYPE_UINT32,     .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_UINT32      },
    { .label_id = TEST_LABEL_ID_NOT_WRITEABLE,              .sublabel_id = 0, .singlecast_set_allowed = true,   .get_minimum = MODE_ID_OPERATION,  .set_minimum = MODE_ID_ILLEGAL,   .label_type = LABEL_TYPE_UINT32,     .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_UINT32      },
    { .label_id = TEST_LABEL_ID_SINGLECAST_SET_NOT_ALLOWED, .sublabel_id = 0, .singlecast_set_allowed = false,  .get_minimum = MODE_ID_OPERATION,  .set_minimum = MODE_ID_OPERATION, .label_type = LABEL_TYPE_UINT32,     .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_UINT32      },
    { .label_id = TEST_LABEL_ID_NOT_KNOWN_BY_CONFIG,        .sublabel_id = 0, .singlecast_set_allowed = true,   .get_minimum = MODE_ID_OPERATION,  .set_minimum = MODE_ID_OPERATION, .label_type = LABEL_TYPE_UINT32,     .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_UNKNOWN     },
    { .label_id = TEST_LABEL_ID_BOOL,                       .sublabel_id = 0, .singlecast_set_allowed = true,   .get_minimum = MODE_ID_OPERATION,  .set_minimum = MODE_ID_OPERATION, .label_type = LABEL_TYPE_BOOL,       .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_BOOL        },
    { .label_id = TEST_LABEL_ID_BOOL_ARRAY,                 .sublabel_id = 0, .singlecast_set_allowed = true,   .get_minimum = MODE_ID_OPERATION,  .set_minimum = MODE_ID_OPERATION, .label_type = LABEL_TYPE_BOOL_ARRAY, .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY8 },
    { .label_id = TEST_LABEL_ID_INT8,                       .sublabel_id = 0, .singlecast_set_allowed = true,   .get_minimum = MODE_ID_OPERATION,  .set_minimum = MODE_ID_OPERATION, .label_type = LABEL_TYPE_INT8,       .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_INT8        },
    { .label_id = TEST_LABEL_ID_INT16,                      .sublabel_id = 0, .singlecast_set_allowed = true,   .get_minimum = MODE_ID_OPERATION,  .set_minimum = MODE_ID_OPERATION, .label_type = LABEL_TYPE_INT16,      .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_INT16       },
    { .label_id = TEST_LABEL_ID_UINT32_WITHOUT_SUBLABEL,    .sublabel_id = 0, .singlecast_set_allowed = true,   .get_minimum = MODE_ID_OPERATION,  .set_minimum = MODE_ID_OPERATION, .label_type = LABEL_TYPE_UINT32,     .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_UINT32      },
    { .label_id = TEST_LABEL_ID_UINT32_WITH_ONE_SUBLABEL,   .sublabel_id = 0, .singlecast_set_allowed = true,   .get_minimum = MODE_ID_OPERATION,  .set_minimum = MODE_ID_OPERATION, .label_type = LABEL_TYPE_UINT32,     .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_UINT32      },
    { .label_id = TEST_LABEL_ID_UINT32_WITH_ONE_SUBLABEL,   .sublabel_id = 1, .singlecast_set_allowed = true,   .get_minimum = MODE_ID_OPERATION,  .set_minimum = MODE_ID_OPERATION, .label_type = LABEL_TYPE_UINT32,     .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_UINT32      },
    { .label_id = TEST_LABEL_ID_VERSION,                    .sublabel_id = 0, .singlecast_set_allowed = true,   .get_minimum = MODE_ID_OPERATION,  .set_minimum = MODE_ID_OPERATION, .label_type = LABEL_TYPE_VERSION,    .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_VERSION     },
    { .label_id = TEST_LABEL_ID_DATE,                       .sublabel_id = 0, .singlecast_set_allowed = true,   .get_minimum = MODE_ID_OPERATION,  .set_minimum = MODE_ID_OPERATION, .label_type = LABEL_TYPE_DATE,       .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_DATE        },
    { .label_id = TEST_LABEL_ID_RAW32,                      .sublabel_id = 0, .singlecast_set_allowed = true,   .get_minimum = MODE_ID_OPERATION,  .set_minimum = MODE_ID_OPERATION, .label_type = LABEL_TYPE_RAW,        .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_RAW32       },
    { .label_id = LABEL_ID_LAST_ENTRY,                      .sublabel_id = 0, .singlecast_set_allowed = false,  .get_minimum = MODE_ID_ILLEGAL,    .set_minimum = MODE_ID_ILLEGAL,   .label_type = LABEL_TYPE_UNKNOWN,    .config_id = CONFIG_ID_INVALID                       }
};

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

// --- helpers -----------------------------------------------------------------

// --- tests -------------------------------------------------------------------

class Label : public ::testing::Test
{
protected:

  void SetUp() override
  {
    MockModeSet(MODE_ID_ILLEGAL);
  }

  void TearDown() override
  {
  }
};

TEST_F(Label, LabelMappingIsAvailable)
{
  uint32_t value = 0;
  MockModeSet(MODE_ID_CONFIG);
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_UINT32_WITHOUT_SUBLABEL, 0, &value, false), ERROR_ID_SUCCESS);
}

TEST_F(Label, ReadingUnknownLabelFailsWithUnknownLabel)
{
  uint32_t value = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_UNKNOWN, 0, &value, false), ERROR_ID_UNKNOWN_LABEL);
}

TEST_F(Label, ReadingKnownLabelWithUnknownSublabelFailsWithUnknownSublabel)
{
  uint32_t value = 0u;
  for (uint16_t sublabel = 1; sublabel <= 255; sublabel++)
    ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_UINT32_WITHOUT_SUBLABEL, (uint8_t)sublabel, &value, false), ERROR_ID_UNKNOWN_SUBLABEL);
}

TEST_F(Label, ReadingLabelInWrongModeFailsWithNotAllowedInThisMode)
{
  uint32_t value = 0u;
  MockModeSet(MODE_ID_ILLEGAL);
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_UINT32_WITHOUT_SUBLABEL, 0, &value, false), ERROR_ID_NOT_ALLOWED_IN_THIS_MODE);
  MockModeSet(MODE_ID_CONFIG);
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_UINT32_WITHOUT_SUBLABEL, 0, &value, false), ERROR_ID_SUCCESS);
}

TEST_F(Label, ReadingWriteOnlyLabelFailsWithNotReadable)
{
  MockModeSet(MODE_ID_OPERATION);
  uint32_t value = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_NOT_READABLE, 0, &value, false), ERROR_ID_NOT_READABLE);
}

TEST_F(Label, WritingReadOnlyLabelFailsWithNotWriteable)
{
  MockModeSet(MODE_ID_OPERATION);
  uint32_t value = 0u;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_NOT_WRITEABLE, 0, &value, false), ERROR_ID_NOT_WRITEABLE);
}

TEST_F(Label, LabelNotAllowedToBeSetViaSinglecastCanNotBeWrittenViaSinglecastButViaBroadcast)
{
  MockModeSet(MODE_ID_OPERATION);
  uint32_t value = 0u;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_SINGLECAST_SET_NOT_ALLOWED, 0, &value, false), ERROR_ID_ONLY_ALLOWED_ON_BASE_ID);
}

TEST_F(Label, LabelNotAllowedToBeSetViaSinglecastCanBeWrittenViaBroadcast)
{
  MockModeSet(MODE_ID_OPERATION);
  uint32_t value = 0u;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_SINGLECAST_SET_NOT_ALLOWED, 0, &value, true), ERROR_ID_SUCCESS);
}

TEST_F(Label, LabelNotAllowedToBeSetViaSinglecastCanBeReadViaSinglecast)
{
  MockModeSet(MODE_ID_OPERATION);
  uint32_t value = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_SINGLECAST_SET_NOT_ALLOWED, 0, &value, false), ERROR_ID_SUCCESS);
}

TEST_F(Label, ReadingLabelNotKnownByConfigFailsWithUnknownValue)
{
  MockModeSet(MODE_ID_OPERATION);
  uint32_t value = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_NOT_KNOWN_BY_CONFIG, 0, &value, false), ERROR_ID_UNKNOWN_VALUE);
}

TEST_F(Label, TypeBoolCanBeRead)
{
  MockModeSet(MODE_ID_CONFIG);
  uint32_t value = 0x44444444u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_BOOL, 0, &value, false), ERROR_ID_SUCCESS);
  ASSERT_EQ(value, 0u);
  bool expected = true;
  ASSERT_EQ(ConfigReadValue((e_CONFIG_ID)TEST_CONFIG_ID_BOOL, &expected, sizeof(expected)), ERROR_ID_SUCCESS);
  ASSERT_EQ(expected, false);
}

TEST_F(Label, TypeBoolCanBeWritten)
{
  MockModeSet(MODE_ID_CONFIG);
  uint32_t before = 0x55555555u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_BOOL, 0, &before, false), ERROR_ID_SUCCESS);
  ASSERT_EQ(before, 0u);
  uint32_t value = 1u;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_BOOL, 0, &value, false), ERROR_ID_SUCCESS);
  uint32_t after = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_BOOL, 0, &after, false), ERROR_ID_SUCCESS);
  ASSERT_EQ(after, 1u);
}

TEST_F(Label, TypeBoolCanNotBeSetToValueOtherThanZeroOrOne)
{
  MockModeSet(MODE_ID_CONFIG);
  for (uint32_t value = 2; value < 0x200000u; value++)
    ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_BOOL, 0, &value, false), ERROR_ID_VALUE_OUT_OF_RANGE);
  const uint32_t value_1 = UINT32_MAX;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_BOOL, 0, &value_1, false), ERROR_ID_VALUE_OUT_OF_RANGE);
}

TEST_F(Label, TypeBoolArrayCanBeRead)
{
  MockModeSet(MODE_ID_CONFIG);
  const bool expected[8] = { false, true, true, false, false, false, false, true };
  ASSERT_EQ(MockValueAsExpected((e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY8, expected, sizeof(expected)), true);
  for (uint8_t sublabel = 0; sublabel < 8; sublabel++) {
    uint32_t value = 5u;
    ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_BOOL_ARRAY, sublabel, &value, false), ERROR_ID_SUCCESS);
    ASSERT_EQ(value, expected[sublabel] ? 1u : 0u);
  }
}

TEST_F(Label, TypeBoolArrayCanBeWritten)
{
  MockModeSet(MODE_ID_CONFIG);
  const uint32_t label_false = 0u;
  const uint32_t label_true = 1u;
  ASSERT_EQ(sizeof(bool), 1u);
  const uint8_t expected_0[8] = { 0, 1, 1, 0, 0, 0, 0, 1 };
  ASSERT_EQ(MockValueAsExpected((e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY8, expected_0, sizeof(expected_0)), true);
  const uint8_t expected_1[8] = { 0, 1, 0, 0, 0, 0, 0, 1 };
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_BOOL_ARRAY, 2, &label_false, false), ERROR_ID_SUCCESS);
  ASSERT_EQ(MockValueAsExpected((e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY8, expected_1, sizeof(expected_1)), true);
  const uint8_t expected_2[8] = { 0, 1, 0, 0, 0, 0, 0, 0 };
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_BOOL_ARRAY, 7, &label_false, false), ERROR_ID_SUCCESS);
  ASSERT_EQ(MockValueAsExpected((e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY8, expected_2, sizeof(expected_2)), true);
  const uint8_t expected_3[8] = { 1, 1, 0, 0, 0, 0, 0, 0 };
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_BOOL_ARRAY, 0, &label_true, false), ERROR_ID_SUCCESS);
  ASSERT_EQ(MockValueAsExpected((e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY8, expected_3, sizeof(expected_3)), true);
  const uint8_t expected_4[8] = { 1, 1, 0, 0, 0, 0, 1, 0 };
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_BOOL_ARRAY, 6, &label_true, false), ERROR_ID_SUCCESS);
  ASSERT_EQ(MockValueAsExpected((e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY8, expected_4, sizeof(expected_4)), true);
}

TEST_F(Label, TypeBoolArrayCanNotBeSetToValueOtherThanZeroOrOne)
{
  MockModeSet(MODE_ID_CONFIG);
  uint32_t value = 2u;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_BOOL_ARRAY, 0, &value, false), ERROR_ID_VALUE_OUT_OF_RANGE);
  value = UINT8_MAX;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_BOOL_ARRAY, 0, &value, false), ERROR_ID_VALUE_OUT_OF_RANGE);
  value = UINT32_MAX;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_BOOL_ARRAY, 0, &value, false), ERROR_ID_VALUE_OUT_OF_RANGE);
}

TEST_F(Label, TypeInt8CanBeRead)
{
  MockModeSet(MODE_ID_CONFIG);
  uint32_t value = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_INT8, 0, &value, false), ERROR_ID_SUCCESS);
  int8_t expected = 1;
  ASSERT_NE(value, (uint32_t)(int32_t)expected);
  ASSERT_EQ(ConfigReadValue((e_CONFIG_ID)TEST_CONFIG_ID_INT8, &expected, sizeof(expected)), ERROR_ID_SUCCESS);
  ASSERT_EQ(value, (uint32_t)(int32_t)expected);
}

TEST_F(Label, TypeInt8CanBeWritten)
{
  MockModeSet(MODE_ID_CONFIG);
  uint32_t before = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_INT8, 0, &before, false), ERROR_ID_SUCCESS);
  ASSERT_NE(before, 0u);
  uint32_t value = (uint32_t)(int32_t)-17;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_INT8, 0, &value, false), ERROR_ID_SUCCESS);
  uint32_t after = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_INT8, 0, &after, false), ERROR_ID_SUCCESS);
  ASSERT_EQ(after, (uint32_t)(int32_t)-17);
}

TEST_F(Label, TypeInt8CanNotBeSetToValueBeyondItsRange)
{
  MockModeSet(MODE_ID_CONFIG);
  const int32_t value_1 = INT32_MIN;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_INT8, 0, (uint32_t *)&value_1, false), ERROR_ID_VALUE_OUT_OF_RANGE);
  const int32_t value_2 = INT8_MIN - 1;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_INT8, 0, (uint32_t *)&value_2, false), ERROR_ID_VALUE_OUT_OF_RANGE);
  const int32_t value_3 = INT8_MIN;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_INT8, 0, (uint32_t *)&value_3, false), ERROR_ID_SUCCESS);
  const int32_t value_4 = INT8_MAX;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_INT8, 0, (uint32_t *)&value_4, false), ERROR_ID_SUCCESS);
  const int32_t value_5 = INT8_MAX + 1;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_INT8, 0, (uint32_t *)&value_5, false), ERROR_ID_VALUE_OUT_OF_RANGE);
  const int32_t value_6 = INT32_MAX;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_INT8, 0, (uint32_t *)&value_6, false), ERROR_ID_VALUE_OUT_OF_RANGE);
}

TEST_F(Label, TypeInt16CanBeRead)
{
  MockModeSet(MODE_ID_CONFIG);
  uint32_t value = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_INT16, 0, &value, false), ERROR_ID_SUCCESS);
  int16_t expected = 1;
  ASSERT_NE(value, (uint32_t)(int32_t)expected);
  ASSERT_EQ(ConfigReadValue((e_CONFIG_ID)TEST_CONFIG_ID_INT16, &expected, sizeof(expected)), ERROR_ID_SUCCESS);
  ASSERT_EQ(value, (uint32_t)(int32_t)expected);
}

TEST_F(Label, TypeInt16CanBeWritten)
{
  MockModeSet(MODE_ID_CONFIG);
  uint32_t before = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_INT16, 0, &before, false), ERROR_ID_SUCCESS);
  ASSERT_NE(before, 0x98765432);
  uint32_t value = (uint32_t)(int32_t)-12345;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_INT16, 0, &value, false), ERROR_ID_SUCCESS);
  uint32_t after = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_INT16, 0, &after, false), ERROR_ID_SUCCESS);
  ASSERT_EQ(after, (uint32_t)(int32_t)-12345);
}

TEST_F(Label, TypeInt16CanNotBeSetToValueBeyondItsRange)
{
  MockModeSet(MODE_ID_CONFIG);
  const int32_t value_1 = INT32_MIN;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_INT16, 0, (uint32_t *)&value_1, false), ERROR_ID_VALUE_OUT_OF_RANGE);
  const int32_t value_2 = INT16_MIN - 1;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_INT16, 0, (uint32_t *)&value_2, false), ERROR_ID_VALUE_OUT_OF_RANGE);
  const int32_t value_3 = INT16_MIN;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_INT16, 0, (uint32_t *)&value_3, false), ERROR_ID_SUCCESS);
  const int32_t value_4 = INT16_MAX;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_INT16, 0, (uint32_t *)&value_4, false), ERROR_ID_SUCCESS);
  const int32_t value_5 = INT16_MAX + 1;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_INT16, 0, (uint32_t *)&value_5, false), ERROR_ID_VALUE_OUT_OF_RANGE);
  const int32_t value_6 = INT32_MAX;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_INT16, 0, (uint32_t *)&value_6, false), ERROR_ID_VALUE_OUT_OF_RANGE);
}

TEST_F(Label, TypeUint32CanBeRead)
{
  MockModeSet(MODE_ID_CONFIG);
  uint32_t value = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_UINT32_WITHOUT_SUBLABEL, 0, &value, false), ERROR_ID_SUCCESS);
  uint32_t expected = 1u;
  ASSERT_NE(memcmp(&value, &expected, sizeof(expected)), 0);
  ASSERT_EQ(ConfigReadValue((e_CONFIG_ID)TEST_CONFIG_ID_UINT32, &expected, sizeof(expected)), ERROR_ID_SUCCESS);
  ASSERT_EQ(memcmp(&value, &expected, sizeof(expected)), 0);
}

TEST_F(Label, TypeUint32CanBeReadFromSublabel)
{
  MockModeSet(MODE_ID_CONFIG);
  uint32_t value = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_UINT32_WITH_ONE_SUBLABEL, 1, &value, false), ERROR_ID_SUCCESS);
  uint32_t expected = 1u;
  ASSERT_NE(memcmp(&value, &expected, sizeof(expected)), 0);
  ASSERT_EQ(ConfigReadValue((e_CONFIG_ID)TEST_CONFIG_ID_UINT32, &expected, sizeof(expected)), ERROR_ID_SUCCESS);
  ASSERT_EQ(memcmp(&value, &expected, sizeof(expected)), 0);
}

TEST_F(Label, TypeUint32CanBeWritten)
{
  MockModeSet(MODE_ID_CONFIG);
  uint32_t before = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_UINT32_WITHOUT_SUBLABEL, 0, &before, false), ERROR_ID_SUCCESS);
  ASSERT_NE(before, 0x98765432u);
  uint32_t value = 0x98765432u;
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_UINT32_WITHOUT_SUBLABEL, 0, &value, false), ERROR_ID_SUCCESS);
  uint32_t after = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_UINT32_WITHOUT_SUBLABEL, 0, &after, false), ERROR_ID_SUCCESS);
  ASSERT_EQ(after, 0x98765432u);
}

TEST_F(Label, TypeVersionCanBeRead)
{
  MockModeSet(MODE_ID_CONFIG);
  t_VERSION value = { 0u, 0u, 0u };
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_VERSION, 0, (uint32_t *)&value, false), ERROR_ID_SUCCESS);
  t_VERSION expected { 1u, 1u, 1u };
  ASSERT_NE(memcmp(&value, &expected, sizeof(expected)), 0);
  ASSERT_EQ(ConfigReadValue((e_CONFIG_ID)TEST_CONFIG_ID_VERSION, &expected, sizeof(expected)), ERROR_ID_SUCCESS);
  ASSERT_EQ(memcmp(&value, &expected, sizeof(expected)), 0);
}

TEST_F(Label, TypeDateCanBeRead)
{
  MockModeSet(MODE_ID_CONFIG);
  t_DATE value = { 0u, 0u, 0u };
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_DATE, 0, (uint32_t *)&value, false), ERROR_ID_SUCCESS);
  t_DATE expected { 1u, 1u, 1u };
  ASSERT_NE(memcmp(&value, &expected, sizeof(expected)), 0);
  ASSERT_EQ(ConfigReadValue((e_CONFIG_ID)TEST_CONFIG_ID_DATE, &expected, sizeof(expected)), ERROR_ID_SUCCESS);
  ASSERT_EQ(memcmp(&value, &expected, sizeof(expected)), 0);
}

TEST_F(Label, TypeRawCanBeRead)
{
  MockModeSet(MODE_ID_CONFIG);
  const uint8_t expected[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
  uint32_t value = 0u;
  for (uint8_t sublabel = 0; sublabel < 8; sublabel++) {
    ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_RAW32, sublabel, &value, false), ERROR_ID_SUCCESS);
    ASSERT_EQ(memcmp(&value, &expected[sublabel * 4], 4), 0);
  }
}

TEST_F(Label, TypeRawCanBeWritten)
{
  MockModeSet(MODE_ID_CONFIG);
  uint32_t value = 0;
  const uint8_t before[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
  const uint8_t write[32] = { 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82 };
  for (uint8_t sublabel = 0; sublabel < 8; sublabel++) {
    value = 0u;
    ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_RAW32, sublabel, &value, false), ERROR_ID_SUCCESS);
    ASSERT_NE(value, 0u);
    ASSERT_EQ(memcmp(&value, &before[sublabel * 4], 4), 0);
    memcpy(&value, &write[sublabel * 4], 4);
    ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_RAW32, sublabel, &value, false), ERROR_ID_SUCCESS);
    value = 0u;
    ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_RAW32, sublabel, &value, false), ERROR_ID_SUCCESS);
    ASSERT_NE(value, 0u);
    ASSERT_EQ(memcmp(&value, &write[sublabel * 4], 4), 0);
  }
}

TEST_F(Label, ReadingAndWritingBeyondRawFailsWithUnknownSublabel)
{
  MockModeSet(MODE_ID_CONFIG);
  uint32_t value = 0u;
  ASSERT_EQ(LabelGetValue(TEST_LABEL_ID_RAW32, 8, &value, false), ERROR_ID_UNKNOWN_SUBLABEL);
  ASSERT_EQ(LabelSetValue(TEST_LABEL_ID_RAW32, 8, &value, false), ERROR_ID_UNKNOWN_SUBLABEL);
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
