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

#include "middleware/IAssert.h"
#include "middleware/IConfig.h"
#include "middleware/IConfigInternal.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"

#include "IMock.h"

#include "gtest/gtest.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef enum {
  TEST_CONFIG_ID_UNKNOWN,
  TEST_CONFIG_ID_NOT_READABLE,
  TEST_CONFIG_ID_NOT_WRITEABLE,
  TEST_CONFIG_ID_INVALID_POINTER,
  TEST_CONFIG_ID_BOOL,
  TEST_CONFIG_ID_INT8,
  TEST_CONFIG_ID_INT16,
  TEST_CONFIG_ID_INT32,
  TEST_CONFIG_ID_UINT8,
  TEST_CONFIG_ID_UINT16,
  TEST_CONFIG_ID_UINT32,
  TEST_CONFIG_ID_FLOAT32,
  TEST_CONFIG_ID_VERSION,
  TEST_CONFIG_ID_DATE,
  TEST_CONFIG_ID_BOOL_ARRAY10,
  TEST_CONFIG_ID_UINT16_ARRAY5,
  TEST_CONFIG_ID_RAW12,
  TEST_CONFIG_ID_RAW20,
  TEST_CONFIG_ID_RAW32
} e_TEST_CONFIG_ID;

typedef struct {
  int callback_counter_pre;
  int callback_counter_post;
  bool test_bool;
  int8_t test_int8;
  int16_t test_int16;
  int32_t test_int32;
  uint8_t test_uint8;
  uint16_t test_uint16;
  uint32_t test_uint32;
  float test_float32;
  t_VERSION test_version;
  t_DATE test_date;
  bool test_bool_array10[10];
  uint16_t test_uint16_array5[5];
  uint8_t test_raw12[12];
  uint8_t test_raw20[20];
  uint8_t test_raw32[32];
} t_TEST;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_TEST test;

static const t_CONFIG test_config[] = {
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_NOT_READABLE,  .config_type = CONFIG_TYPE_BOOL,    .elements =  1, .readable = false, .writeable = true,  .data_pointer = (void*)&test.test_bool         },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_NOT_WRITEABLE, .config_type = CONFIG_TYPE_BOOL,    .elements =  1, .readable = true,  .writeable = false, .data_pointer = (void*)&test.test_bool         },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_BOOL,          .config_type = CONFIG_TYPE_BOOL,    .elements =  1, .readable = true,  .writeable = true,  .data_pointer = (void*)&test.test_bool         },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_INT8,          .config_type = CONFIG_TYPE_INT8,    .elements =  1, .readable = true,  .writeable = true,  .data_pointer = (void*)&test.test_int8         },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_INT16,         .config_type = CONFIG_TYPE_INT16,   .elements =  1, .readable = true,  .writeable = true,  .data_pointer = (void*)&test.test_int16        },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_INT32,         .config_type = CONFIG_TYPE_INT32,   .elements =  1, .readable = true,  .writeable = true,  .data_pointer = (void*)&test.test_int32        },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_UINT8,         .config_type = CONFIG_TYPE_UINT8,   .elements =  1, .readable = true,  .writeable = true,  .data_pointer = (void*)&test.test_uint8        },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_UINT16,        .config_type = CONFIG_TYPE_UINT16,  .elements =  1, .readable = true,  .writeable = true,  .data_pointer = (void*)&test.test_uint16       },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_UINT32,        .config_type = CONFIG_TYPE_UINT32,  .elements =  1, .readable = true,  .writeable = true,  .data_pointer = (void*)&test.test_uint32       },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_FLOAT32,       .config_type = CONFIG_TYPE_FLOAT32, .elements =  1, .readable = true,  .writeable = true,  .data_pointer = (void*)&test.test_float32      },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_VERSION,       .config_type = CONFIG_TYPE_VERSION, .elements =  1, .readable = true,  .writeable = true,  .data_pointer = (void*)&test.test_version      },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_DATE,          .config_type = CONFIG_TYPE_DATE,    .elements =  1, .readable = true,  .writeable = true,  .data_pointer = (void*)&test.test_date         },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY10,  .config_type = CONFIG_TYPE_BOOL,    .elements = 10, .readable = true,  .writeable = true,  .data_pointer = (void*)test.test_bool_array10  },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_UINT16_ARRAY5, .config_type = CONFIG_TYPE_UINT16,  .elements =  5, .readable = true,  .writeable = true,  .data_pointer = (void*)test.test_uint16_array5 },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_RAW12,         .config_type = CONFIG_TYPE_RAW,     .elements = 12, .readable = true,  .writeable = true,  .data_pointer = (void*)test.test_raw12         },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_RAW20,         .config_type = CONFIG_TYPE_RAW,     .elements = 20, .readable = true,  .writeable = true,  .data_pointer = (void*)test.test_raw20         },
    { .config_id = (e_CONFIG_ID)TEST_CONFIG_ID_RAW32,         .config_type = CONFIG_TYPE_RAW,     .elements = 32, .readable = true,  .writeable = true,  .data_pointer = (void*)test.test_raw32         },
    { .config_id = CONFIG_ID_INVALID,                         .config_type = CONFIG_TYPE_UNKNOWN, .elements =  1, .readable = true,  .writeable = true,  .data_pointer = NULL                           }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static CONFIG_CALLBACK(TestConfigCallback);
static void TestResetCallbackCounters(void);
static bool TestCallbackHasBeenExecutedAsExpected(const int expect_pre, const int expect_post);
static void TestValidReadWriteStraight(const e_TEST_CONFIG_ID config_id, void * const real, void * const read, const void * const read_check, const void * const write, const void * const write_check, const size_t number_of_bytes);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

// --- helpers -----------------------------------------------------------------

static CONFIG_CALLBACK(TestConfigCallback)
{
  (void)config_id;
  (void)read;
  (void)data;

  e_ERROR_ID error = ERROR_ID_SUCCESS;

  if (callback_type == CONFIG_CALLBACK_TYPE_PRE)
    test.callback_counter_pre++;
  else if (callback_type == CONFIG_CALLBACK_TYPE_POST)
    test.callback_counter_post++;
  else
    ASSERT_FATAL(false);

  return error;
}

static void TestResetCallbackCounters(void)
{
  test.callback_counter_pre = 0;
  test.callback_counter_post = 0;
}

static bool TestCallbackHasBeenExecutedAsExpected(const int expect_pre, const int expect_post)
{
  return (expect_pre == test.callback_counter_pre) && (expect_post  == test.callback_counter_post);
}

static void TestValidReadWriteStraight(const e_TEST_CONFIG_ID config_id, void * const real, void * const read, const void * const read_check, const void * const write, const void * const write_check, const size_t number_of_bytes)
{
  ASSERT_EQ(ConfigReadValue((e_CONFIG_ID)config_id, read, number_of_bytes), ERROR_ID_SUCCESS);
  ASSERT_EQ(TestCallbackHasBeenExecutedAsExpected(1, 1), true);
  ASSERT_EQ(memcmp(read, read_check, number_of_bytes), 0);

  TestResetCallbackCounters();

  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)config_id, write, number_of_bytes), ERROR_ID_SUCCESS);
  ASSERT_EQ(TestCallbackHasBeenExecutedAsExpected(1, 1), true);
  ASSERT_EQ(memcmp(real, write_check, number_of_bytes), 0);

  TestResetCallbackCounters();
}

// --- tests -------------------------------------------------------------------

class Config : public ::testing::Test
{
  protected:

  void SetUp() override
  {
    TestResetCallbackCounters();
    ASSERT_EQ(MockErrorGetNumberAndReset(), 0);
  }

  void TearDown() override
  {
    ASSERT_EQ(MockErrorGetNumberAndReset(), 0);
  }
};

TEST_F(Config, ThisArchitectureUsesTheExpectedSizeOfDataTypes)
{
  ASSERT_EQ(sizeof(bool), 1u);
  ASSERT_EQ(true, 1);
  ASSERT_EQ(false, 0);
  ASSERT_EQ(sizeof(float), 4u);
  ASSERT_EQ(sizeof(t_VERSION), 4u);
  ASSERT_EQ(sizeof(t_DATE), 4u);
}

TEST_F(Config, ConfigurationTableAndCallbackFunctionCanBeRegistered)
{
  ASSERT_EQ(ConfigRegister(test_config, TestConfigCallback), ERROR_ID_SUCCESS);
}

TEST_F(Config, RedundantRegistrationFailesAndIsLogged)
{
  ASSERT_EQ(ConfigRegister(test_config, TestConfigCallback), ERROR_ID_FAILED);
  ASSERT_EQ(MockErrorGetNumberAndReset(), 1);
}

TEST_F(Config, GetSizeProvidesTheExpectedValues)
{
  ASSERT_EQ(ConfigGetSize((e_CONFIG_ID)TEST_CONFIG_ID_BOOL), 1u);
  ASSERT_EQ(ConfigGetSize((e_CONFIG_ID)TEST_CONFIG_ID_INT8), 1u);
  ASSERT_EQ(ConfigGetSize((e_CONFIG_ID)TEST_CONFIG_ID_INT16), 2u);
  ASSERT_EQ(ConfigGetSize((e_CONFIG_ID)TEST_CONFIG_ID_INT32), 4u);
  ASSERT_EQ(ConfigGetSize((e_CONFIG_ID)TEST_CONFIG_ID_UINT8), 1u);
  ASSERT_EQ(ConfigGetSize((e_CONFIG_ID)TEST_CONFIG_ID_UINT16), 2u);
  ASSERT_EQ(ConfigGetSize((e_CONFIG_ID)TEST_CONFIG_ID_UINT32), 4u);
  ASSERT_EQ(ConfigGetSize((e_CONFIG_ID)TEST_CONFIG_ID_FLOAT32), 4u);
  ASSERT_EQ(ConfigGetSize((e_CONFIG_ID)TEST_CONFIG_ID_VERSION), 4u);
  ASSERT_EQ(ConfigGetSize((e_CONFIG_ID)TEST_CONFIG_ID_DATE), 4u);
  ASSERT_EQ(ConfigGetSize((e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY10), 10u);
  ASSERT_EQ(ConfigGetSize((e_CONFIG_ID)TEST_CONFIG_ID_UINT16_ARRAY5), 10u);
  ASSERT_EQ(ConfigGetSize((e_CONFIG_ID)TEST_CONFIG_ID_RAW12), 12u);
  ASSERT_EQ(ConfigGetSize((e_CONFIG_ID)TEST_CONFIG_ID_RAW20), 20u);
  ASSERT_EQ(ConfigGetSize((e_CONFIG_ID)TEST_CONFIG_ID_RAW32), 32u);
}

TEST_F(Config, GetTypeProvidesTheExpectedValues)
{
  ASSERT_EQ(ConfigGetType((e_CONFIG_ID)TEST_CONFIG_ID_BOOL), CONFIG_TYPE_BOOL);
  ASSERT_EQ(ConfigGetType((e_CONFIG_ID)TEST_CONFIG_ID_INT8), CONFIG_TYPE_INT8);
  ASSERT_EQ(ConfigGetType((e_CONFIG_ID)TEST_CONFIG_ID_INT16), CONFIG_TYPE_INT16);
  ASSERT_EQ(ConfigGetType((e_CONFIG_ID)TEST_CONFIG_ID_INT32), CONFIG_TYPE_INT32);
  ASSERT_EQ(ConfigGetType((e_CONFIG_ID)TEST_CONFIG_ID_UINT8), CONFIG_TYPE_UINT8);
  ASSERT_EQ(ConfigGetType((e_CONFIG_ID)TEST_CONFIG_ID_UINT16), CONFIG_TYPE_UINT16);
  ASSERT_EQ(ConfigGetType((e_CONFIG_ID)TEST_CONFIG_ID_UINT32), CONFIG_TYPE_UINT32);
  ASSERT_EQ(ConfigGetType((e_CONFIG_ID)TEST_CONFIG_ID_FLOAT32), CONFIG_TYPE_FLOAT32);
  ASSERT_EQ(ConfigGetType((e_CONFIG_ID)TEST_CONFIG_ID_VERSION), CONFIG_TYPE_VERSION);
  ASSERT_EQ(ConfigGetType((e_CONFIG_ID)TEST_CONFIG_ID_DATE), CONFIG_TYPE_DATE);
  ASSERT_EQ(ConfigGetType((e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY10), CONFIG_TYPE_BOOL);
  ASSERT_EQ(ConfigGetType((e_CONFIG_ID)TEST_CONFIG_ID_UINT16_ARRAY5), CONFIG_TYPE_UINT16);
  ASSERT_EQ(ConfigGetType((e_CONFIG_ID)TEST_CONFIG_ID_RAW12), CONFIG_TYPE_RAW);
  ASSERT_EQ(ConfigGetType((e_CONFIG_ID)TEST_CONFIG_ID_RAW20), CONFIG_TYPE_RAW);
  ASSERT_EQ(ConfigGetType((e_CONFIG_ID)TEST_CONFIG_ID_RAW32), CONFIG_TYPE_RAW);
}

TEST_F(Config, KnownIdAreReportedAsKnown)
{
  ASSERT_EQ(ConfigIsAvailable((e_CONFIG_ID)TEST_CONFIG_ID_BOOL), true);
}

TEST_F(Config, UnknownIdAreReportedAsUnknown)
{
  ASSERT_EQ(ConfigIsAvailable((e_CONFIG_ID)TEST_CONFIG_ID_UNKNOWN), false);
}

TEST_F(Config, ReadingAnInvalidIdFailsWithUnknownValue)
{
  test.test_uint8 = 0x55;
  uint8_t read_value = 0xAA;
  ASSERT_EQ(ConfigReadValue(CONFIG_ID_INVALID, &read_value, 1), ERROR_ID_UNKNOWN_VALUE);
  ASSERT_EQ(TestCallbackHasBeenExecutedAsExpected(0, 0), true);
  ASSERT_EQ(read_value, 0xAA);
}

TEST_F(Config, ReadingWriteOnlyValueFailsWithNotReadable)
{
  test.test_uint8 = 0x55;
  uint8_t read_value = 0xAA;
  ASSERT_EQ(ConfigReadValue((e_CONFIG_ID)TEST_CONFIG_ID_NOT_READABLE, &read_value, 1), ERROR_ID_NOT_READABLE);
  ASSERT_EQ(TestCallbackHasBeenExecutedAsExpected(0, 0), true);
  ASSERT_EQ(read_value, 0xAA);
}

TEST_F(Config, WritingReadOnlyValueFailsWithNotWriteable)
{
  test.test_uint8 = 0x55;
  uint8_t value_to_write = 0xAA;
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_NOT_WRITEABLE, &value_to_write, 1), ERROR_ID_NOT_WRITEABLE);
  ASSERT_EQ(TestCallbackHasBeenExecutedAsExpected(0, 0), true);
  ASSERT_EQ(test.test_uint8, 0x55);
}

TEST_F(Config, ReadingWithWrongSizeFailsWithInternalError)
{
  bool read = false;
  ASSERT_EQ(ConfigReadValue((e_CONFIG_ID)TEST_CONFIG_ID_BOOL, &read, sizeof(read) - 1), ERROR_ID_INTERNAL_ERROR);
  ASSERT_EQ(ConfigReadValue((e_CONFIG_ID)TEST_CONFIG_ID_BOOL, &read, sizeof(read) + 1), ERROR_ID_INTERNAL_ERROR);
}

TEST_F(Config, WritingWithWrongSizeFailsWithInternalError)
{
  const bool write = false;
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_BOOL, &write, sizeof(write) - 1), ERROR_ID_INTERNAL_ERROR);
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_BOOL, &write, sizeof(write) + 1), ERROR_ID_INTERNAL_ERROR);
}

TEST_F(Config, BoolDoesNotAcceptValuesOtherThanZeroOrOneAtWriting)
{
  const uint8_t write = 2;
  ASSERT_EQ(sizeof(bool), sizeof(write));
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_BOOL, &write, sizeof(write)), ERROR_ID_VALUE_OUT_OF_RANGE);
}

TEST_F(Config, FloatDoesNotAcceptInfinityAtWriting)
{
  uint32_t infinity_positive = 0x7F800000;
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_FLOAT32, &infinity_positive, sizeof(uint32_t)), ERROR_ID_VALUE_OUT_OF_RANGE);
  uint32_t negative_infinity = 0xFF800000;
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_FLOAT32, &negative_infinity, sizeof(uint32_t)), ERROR_ID_VALUE_OUT_OF_RANGE);
}

TEST_F(Config, FloatDoesNotAcceptNanAtWriting)
{
  for (uint32_t nan_signaling_positive = 0x7F800001; nan_signaling_positive <= 0x7FBFFFFF; nan_signaling_positive++)
    ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_FLOAT32, &nan_signaling_positive, sizeof(uint32_t)), ERROR_ID_VALUE_OUT_OF_RANGE);
  for (uint32_t nan_signaling_negative = 0xFF800001; nan_signaling_negative <= 0xFFBFFFFF; nan_signaling_negative++)
    ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_FLOAT32, &nan_signaling_negative, sizeof(uint32_t)), ERROR_ID_VALUE_OUT_OF_RANGE);
  for (uint32_t nan_quiet_positive = 0x7FC00000; nan_quiet_positive <= 0x7FFFFFFF; nan_quiet_positive++)
    ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_FLOAT32, &nan_quiet_positive, sizeof(uint32_t)), ERROR_ID_VALUE_OUT_OF_RANGE);
  for (uint32_t nan_quiet_negative = 0xFFC00000; nan_quiet_negative < 0xFFFFFFFF; nan_quiet_negative++)
    ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_FLOAT32, &nan_quiet_negative, sizeof(uint32_t)), ERROR_ID_VALUE_OUT_OF_RANGE);
  uint32_t nan_quiet_negative = 0xFFFFFFFF;
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_FLOAT32, &nan_quiet_negative, sizeof(uint32_t)), ERROR_ID_VALUE_OUT_OF_RANGE);
}

TEST_F(Config, WritingDateWithInvalidDaysFailsWithOutOfRange)
{
  const t_DATE write_1 = { .year = 2000, .month = 12, .day = 0 };
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_DATE, &write_1, sizeof(test.test_date)), ERROR_ID_VALUE_OUT_OF_RANGE);
  const t_DATE write_2 = { .year = 2000, .month = 12, .day = 32 };
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_DATE, &write_2, sizeof(test.test_date)), ERROR_ID_VALUE_OUT_OF_RANGE);
}

TEST_F(Config, WritingDateWithInvalidMonthsFailsWithOutOfRange)
{
  const t_DATE write_3 = { .year = 2000, .month = 0, .day = 32 };
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_DATE, &write_3, sizeof(test.test_date)), ERROR_ID_VALUE_OUT_OF_RANGE);
  const t_DATE write_4 = { .year = 2000, .month = 13, .day = 32 };
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_DATE, &write_4, sizeof(test.test_date)), ERROR_ID_VALUE_OUT_OF_RANGE);
}

TEST_F(Config, WritingDateWithYearsBefore2000FailsWithOutOfRange)
{
  const t_DATE write_5 = { .year = 1999, .month = 12, .day = 32 };
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_DATE, &write_5, sizeof(test.test_date)), ERROR_ID_VALUE_OUT_OF_RANGE);
}

TEST_F(Config, BoolArrayDoesNotAcceptValuesOtherThanZeroOrOneAtWriting)
{
  const uint8_t write_1[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  ASSERT_EQ(sizeof(write_1), 10u);
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY10, &write_1, sizeof(test.test_bool_array10)), ERROR_ID_SUCCESS);
  const uint8_t write_2[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
  ASSERT_EQ(sizeof(write_2), 10u);
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY10, &write_2, sizeof(test.test_bool_array10)), ERROR_ID_SUCCESS);
  const uint8_t write_3[10] = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
  ASSERT_EQ(sizeof(write_3), 10u);
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY10, &write_3, sizeof(test.test_bool_array10)), ERROR_ID_VALUE_OUT_OF_RANGE);
  const uint8_t write_4[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 255 };
  ASSERT_EQ(sizeof(write_4), 10u);
  ASSERT_EQ(ConfigWriteValue((e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY10, &write_4, sizeof(test.test_bool_array10)), ERROR_ID_VALUE_OUT_OF_RANGE);
}

TEST_F(Config, ReadingAndWritingSucceedsAndResultsInTheExpectedValueForBool)
{
  test.test_bool = true;
  bool read = false;
  bool read_check = true;
  bool write = false;
  bool write_check = false;
  TestValidReadWriteStraight(TEST_CONFIG_ID_BOOL, &test.test_bool, &read, &read_check, &write, &write_check, sizeof(test.test_bool));
}

TEST_F(Config, ReadingAndWritingSucceedsAndResultsInTheExpectedValueForInt8)
{
  test.test_int8 = 0x12;
  int8_t read = 0;
  int8_t read_check = 0x12;
  int8_t write = 0x23;
  int8_t write_check = 0x23;
  TestValidReadWriteStraight(TEST_CONFIG_ID_INT8, &test.test_int8, &read, &read_check, &write, &write_check, sizeof(test.test_int8));
}

TEST_F(Config, ReadingAndWritingSucceedsAndResultsInTheExpectedValueForInt16)
{
  test.test_int16 = 0x1234;
  int16_t read = 0;
  int16_t read_check = 0x1234;
  int16_t write = 0x2345;
  int16_t write_check = 0x2345;
  TestValidReadWriteStraight(TEST_CONFIG_ID_INT16, &test.test_int16, &read, &read_check, &write, &write_check, sizeof(test.test_int16));
}

TEST_F(Config, ReadingAndWritingSucceedsAndResultsInTheExpectedValueForInt32)
{
  test.test_int32 = 0x12345678;
  int32_t read = 0;
  int32_t read_check = 0x12345678;
  int32_t write = 0x23456789;
  int32_t write_check = 0x23456789;
  TestValidReadWriteStraight(TEST_CONFIG_ID_INT32, &test.test_int32, &read, &read_check, &write, &write_check, sizeof(test.test_int32));
}

TEST_F(Config, ReadingAndWritingSucceedsAndResultsInTheExpectedValueForUint8)
{
  test.test_uint8 = 0x21;
  uint8_t read = 0;
  uint8_t read_check = 0x21;
  uint8_t write = 0x32;
  uint8_t write_check = 0x32;
  TestValidReadWriteStraight(TEST_CONFIG_ID_UINT8, &test.test_uint8, &read, &read_check, &write, &write_check, sizeof(test.test_uint8));
}

TEST_F(Config, ReadingAndWritingSucceedsAndResultsInTheExpectedValueForUint16)
{
  test.test_uint16 = 0x4321;
  uint16_t read = 0;
  uint16_t read_check = 0x4321;
  uint16_t write = 0x5432;
  uint16_t write_check = 0x5432;
  TestValidReadWriteStraight(TEST_CONFIG_ID_UINT16, &test.test_uint16, &read, &read_check, &write, &write_check, sizeof(test.test_uint16));
}

TEST_F(Config, ReadingAndWritingSucceedsAndResultsInTheExpectedValueForUint32)
{
  test.test_uint32 = 0x87654321;
  uint32_t read = 0;
  uint32_t read_check = 0x87654321;
  uint32_t write = 0x98765432;
  uint32_t write_check = 0x98765432;
  TestValidReadWriteStraight(TEST_CONFIG_ID_UINT32, &test.test_uint32, &read, &read_check, &write, &write_check, sizeof(test.test_uint32));
}

TEST_F(Config, ReadingAndWritingSucceedsAndResultsInTheExpectedValueForFloat32)
{
  test.test_float32 = 1234.5678f;
  float read = 0.0f;
  float read_check = 1234.5678f;
  float write = 9876.5432f;
  float write_check = 9876.5432f;
  TestValidReadWriteStraight(TEST_CONFIG_ID_FLOAT32, &test.test_float32, &read, &read_check, &write, &write_check, sizeof(test.test_float32));
}

TEST_F(Config, ReadingAndWritingSucceedsAndResultsInTheExpectedValueForVersion)
{
  test.test_version.major = 1;
  test.test_version.minor = 2;
  test.test_version.patch = 3;
  t_VERSION read;
  memset(&read, 0, sizeof(read));
  t_VERSION read_check;
  memcpy(&read_check, &test.test_version, sizeof(read_check));
  t_VERSION write = { .minor = 8, .major = 9, .patch = 7 };
  t_VERSION write_check;
  memcpy(&write_check, &write, sizeof(write_check));
  TestValidReadWriteStraight(TEST_CONFIG_ID_VERSION, &test.test_version, &read, &read_check, &write, &write_check, sizeof(test.test_version));
}

TEST_F(Config, ReadingAndWritingSucceedsAndResultsInTheExpectedValueForDate)
{
  test.test_date.day = 17;
  test.test_date.month = 11;
  test.test_date.year = 2055;
  t_DATE read;
  memset(&read, 0, sizeof(read));
  t_DATE read_check;
  memcpy(&read_check, &test.test_date, sizeof(read_check));
  t_DATE write = { .year = 2000, .month = 2, .day = 1 };
  t_DATE write_check;
  memcpy(&write_check, &write, sizeof(write_check));
  TestValidReadWriteStraight(TEST_CONFIG_ID_DATE, &test.test_date, &read, &read_check, &write, &write_check, sizeof(test.test_date));
}

TEST_F(Config, ReadingAndWritingSucceedsAndResultsInTheExpectedValueForBoolArray10)
{
  const bool data[10] = { false, true, false, true, false, true, false, true, false, true };
  ASSERT_EQ(sizeof(data), sizeof(test.test_bool_array10));
  memcpy(test.test_bool_array10, data, sizeof(test.test_bool_array10));
  bool read[10] = { false, false, false, false, false, false, false, false, false, false };
  const bool read_check[10] = { false, true, false, true, false, true, false, true, false, true };
  const bool write[10] = { true, false, true, false, true, false, true, false, true, false };
  const bool write_check[10] = { true, false, true, false, true, false, true, false, true, false };
  TestValidReadWriteStraight(TEST_CONFIG_ID_BOOL_ARRAY10, test.test_bool_array10, read, read_check, write, write_check, sizeof(test.test_bool_array10));
}

TEST_F(Config, ReadingAndWritingSucceedsAndResultsInTheExpectedValueForUint16Array5)
{
  const uint16_t data[5] = { 0x2211, 0x4433, 0x6655, 0x8877, 0xAA99 };
  ASSERT_EQ(sizeof(data), sizeof(test.test_uint16_array5));
  memcpy(test.test_uint16_array5, data, sizeof(test.test_uint16_array5));
  uint16_t read[5] = { 0, 0, 0, 0, 0 };
  const uint16_t read_check[5] = { 0x2211, 0x4433, 0x6655, 0x8877, 0xAA99 };
  const uint16_t write[5] = { 0x4433, 0x6655, 0x8877, 0xAA99, 0xCCBB };
  const uint16_t write_check[5] = { 0x4433, 0x6655, 0x8877, 0xAA99, 0xCCBB };
  TestValidReadWriteStraight(TEST_CONFIG_ID_UINT16_ARRAY5, test.test_uint16_array5, read, read_check, write, write_check, sizeof(test.test_uint16_array5));
}

TEST_F(Config, ReadingAndWritingSucceedsAndResultsInTheExpectedValueForRaw12)
{
  const uint8_t data[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
  memcpy(test.test_raw12, data, sizeof(test.test_raw12));
  uint8_t read[12];
  memset(read, 0, sizeof(read));
  const uint8_t read_check[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
  const uint8_t write[12] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
  const uint8_t write_check[12] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
  TestValidReadWriteStraight(TEST_CONFIG_ID_RAW12, test.test_raw12, read, read_check, write, write_check, sizeof(test.test_raw12));
}

TEST_F(Config, ReadingAndWritingSucceedsAndResultsInTheExpectedValueForRaw20)
{
  const uint8_t data[20] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
  memcpy(test.test_raw20, data, sizeof(test.test_raw20));
  uint8_t read[20];
  memset(read, 0, sizeof(read));
  const uint8_t read_check[20] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
  const uint8_t write[20] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 };
  const uint8_t write_check[20] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 };
  TestValidReadWriteStraight(TEST_CONFIG_ID_RAW20, test.test_raw20, read, read_check, write, write_check, sizeof(test.test_raw20));
}

TEST_F(Config, ReadingAndWritingSucceedsAndResultsInTheExpectedValueForRaw32)
{
  const uint8_t data[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
  memcpy(test.test_raw32, data, sizeof(test.test_raw32));
  uint8_t read[32];
  memset(read, 0, sizeof(read));
  const uint8_t read_check[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
  const uint8_t write[32] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33 };
  const uint8_t write_check[32] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33 };
  TestValidReadWriteStraight(TEST_CONFIG_ID_RAW32, test.test_raw32, read, read_check, write, write_check, sizeof(test.test_raw32));
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
