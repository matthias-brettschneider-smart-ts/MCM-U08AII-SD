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
#include "middleware/IErrorInternal.h"
#include "middleware/IGeneric.h"

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

static bool TestLogIsEmpty(void);
static void TestRemoveOneEntry(void);
static bool TestRemoveExpectedEntry(const e_ERROR_CATEGORY category, const e_ERROR_ID id, const uint8_t occurrence_count, const uint8_t attribute);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

// --- helpers -----------------------------------------------------------------

static bool TestLogIsEmpty(void)
{
  t_ERROR_SUMMARY summary;
  ErrorGetOldestEntry(&summary);
  return summary.value == 0;
}

static void TestRemoveOneEntry(void)
{
  t_ERROR_SUMMARY summary;
  ErrorGetOldestEntry(&summary);
}

static bool TestRemoveExpectedEntry(const e_ERROR_CATEGORY category, const e_ERROR_ID id, const uint8_t occurrence_count, const uint8_t attribute)
{
  t_ERROR_SUMMARY summary;
  ErrorGetOldestEntry(&summary);
  const t_ERROR_SUMMARY expected = { .category = category, .id = id, .occurrence_count = occurrence_count, .attribute = attribute };
  return summary.value == expected.value;
}

// --- tests -------------------------------------------------------------------

class Error : public ::testing::Test
{
  protected:

  void SetUp() override
  {
    ASSERT_EQ(TestLogIsEmpty(), true);
    ASSERT_EQ(ErrorGetHighestCategory(), ERROR_CATEGORY_NO_ERROR);
  }

  void TearDown() override
  {
    ASSERT_EQ(TestLogIsEmpty(), true);
    ASSERT_EQ(ErrorGetHighestCategory(), ERROR_CATEGORY_NO_ERROR);
  }
};

TEST_F(Error, CanBeInitialized)
{
  ErrorInitFirst();
  ErrorInitSecond();
}

TEST_F(Error, EmptyLogHasHighestCategoryNoError)
{
  ASSERT_EQ(ErrorGetHighestCategory(), ERROR_CATEGORY_NO_ERROR);
}

TEST_F(Error, ReadingEmptyLogProvidesEmptySummary)
{
  ASSERT_EQ(TestRemoveExpectedEntry(ERROR_CATEGORY_NO_ERROR, ERROR_ID_SUCCESS, 0, 0), true);
}

TEST_F(Error, TryingToAddNonLogableErrorIsRecognized)
{
  ASSERT_LT(ERROR_ID_SUCCESS, ERROR_ID_ZERO);
  ASSERT_LT(ERROR_ID_VALUE_OUT_OF_RANGE, ERROR_ID_ZERO);
  ASSERT_LT(ERROR_ID_UNKNOWN_COMMAND, ERROR_ID_ZERO);
  ErrorAddEntry(ERROR_ID_SUCCESS, 0x10);
  ErrorAddEntry(ERROR_ID_VALUE_OUT_OF_RANGE, 0x11);
  ErrorAddEntry(ERROR_ID_UNKNOWN_COMMAND, 0x12);
  ASSERT_EQ(TestRemoveExpectedEntry(ERROR_CATEGORY_ERROR, ERROR_ID_ERROR_HANDLING, 3, 0x10), true);
}

TEST_F(Error, TryingToAddInternalLimitsIsRecognized)
{
  ErrorAddEntry(ERROR_ID_ZERO, 0x13);
  ErrorAddEntry(ERROR_ID_LAST, 0x14);
  ASSERT_EQ(TestRemoveExpectedEntry(ERROR_CATEGORY_ERROR, ERROR_ID_ERROR_HANDLING, 2, 0x13), true);
}

TEST_F(Error, WarningCanBeAddedAndRead)
{
  ErrorAddEntry(ERROR_ID_APPLICATION_WARNING, 0x34);
  ASSERT_EQ(TestRemoveExpectedEntry(ERROR_CATEGORY_WARNING, ERROR_ID_APPLICATION_WARNING, 1, 0x34), true);
}

TEST_F(Error, AddingWarningLeadsToHighestCategoryWarning)
{
  ErrorAddEntry(ERROR_ID_APPLICATION_WARNING, 0x34);
  ASSERT_EQ(ErrorGetHighestCategory(), ERROR_CATEGORY_WARNING);
  TestRemoveOneEntry();
}

TEST_F(Error, ErrorCanBeAddedAndRead)
{
  ErrorAddEntry(ERROR_ID_APPLICATION_ERROR, 0x56);
  ASSERT_EQ(TestRemoveExpectedEntry(ERROR_CATEGORY_ERROR, ERROR_ID_APPLICATION_ERROR, 1, 0x56), true);
}

TEST_F(Error, AddingErrorLeadsToHighestCategoryError)
{
  ErrorAddEntry(ERROR_ID_APPLICATION_ERROR, 0x56);
  ASSERT_EQ(ErrorGetHighestCategory(), ERROR_CATEGORY_ERROR);
  TestRemoveOneEntry();
}

TEST_F(Error, AddingTheSameTypeMultipleTimesIsReadWithCorrectCounterAndAttribute)
{
  ErrorAddEntry(ERROR_ID_APPLICATION_WARNING, 0x22);
  ErrorAddEntry(ERROR_ID_APPLICATION_WARNING, 0x33);
  ErrorAddEntry(ERROR_ID_APPLICATION_WARNING, 0x44);
  ASSERT_EQ(TestRemoveExpectedEntry(ERROR_CATEGORY_WARNING, ERROR_ID_APPLICATION_WARNING, 3, 0x22), true);
}

TEST_F(Error, SameErrorCanBeAddedUpTo127Times)
{
  for (int run = 1; run < 127; run++) {
    for (int add = 0; add < run; add++)
      ErrorAddEntry(ERROR_ID_APPLICATION_WARNING, 0x22);
    ASSERT_EQ(TestRemoveExpectedEntry(ERROR_CATEGORY_WARNING, ERROR_ID_APPLICATION_WARNING, run, 0x22), true);
    ASSERT_EQ(TestLogIsEmpty(), true);
  }
  for (int add = 0; add < 128; add++)
    ErrorAddEntry(ERROR_ID_APPLICATION_WARNING, 0x22);
  ASSERT_EQ(TestRemoveExpectedEntry(ERROR_CATEGORY_WARNING, ERROR_ID_APPLICATION_WARNING, 127, 0x22), true);
}

TEST_F(Error, EntriesAreReadInOrderOfTheirAppearance)
{
  ErrorAddEntry(ERROR_ID_APPLICATION_ERROR, 0x47);
  ErrorAddEntry(ERROR_ID_APPLICATION_WARNING, 0x11);
  ASSERT_EQ(TestRemoveExpectedEntry(ERROR_CATEGORY_ERROR, ERROR_ID_APPLICATION_ERROR, 1, 0x47), true);
  ASSERT_EQ(TestRemoveExpectedEntry(ERROR_CATEGORY_WARNING, ERROR_ID_APPLICATION_WARNING, 1, 0x11), true);
}

TEST_F(Error, HighestCategoryIsNotDecreasedByAddingEntryWithLowerCategory)
{
  ErrorAddEntry(ERROR_ID_APPLICATION_ERROR, 0);
  ASSERT_EQ(ErrorGetHighestCategory(), ERROR_CATEGORY_ERROR);
  ErrorAddEntry(ERROR_ID_APPLICATION_WARNING, 0);
  ASSERT_EQ(ErrorGetHighestCategory(), ERROR_CATEGORY_ERROR);
  TestRemoveOneEntry();
  TestRemoveOneEntry();
}

TEST_F(Error, HighestCategoryIsDecreasedByRemovingEntryWithHighestCategory)
{
  ErrorAddEntry(ERROR_ID_APPLICATION_ERROR, 0);
  ErrorAddEntry(ERROR_ID_APPLICATION_WARNING, 0);
  ASSERT_EQ(ErrorGetHighestCategory(), ERROR_CATEGORY_ERROR);
  TestRemoveOneEntry();
  ASSERT_EQ(ErrorGetHighestCategory(), ERROR_CATEGORY_WARNING);
  TestRemoveOneEntry();
  ASSERT_EQ(ErrorGetHighestCategory(), ERROR_CATEGORY_NO_ERROR);
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
