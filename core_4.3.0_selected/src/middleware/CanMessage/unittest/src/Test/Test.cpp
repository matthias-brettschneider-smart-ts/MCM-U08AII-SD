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

#include "middleware/ICanMessage.h"
#include "middleware/IGeneric.h"

#include "gtest/gtest.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef struct {
  uint8_t length;
  uint8_t dlc;
  uint8_t length_min;
} TEST_ASSIGNMENT_DLC_FD_t;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static const TEST_ASSIGNMENT_DLC_FD_t test_assignment_dlc_fd[] = {
  { .length =  0, .dlc =  0, .length_min =  0 },
  { .length =  1, .dlc =  1, .length_min =  1 },
  { .length =  2, .dlc =  2, .length_min =  2 },
  { .length =  3, .dlc =  3, .length_min =  3 },
  { .length =  4, .dlc =  4, .length_min =  4 },
  { .length =  5, .dlc =  5, .length_min =  5 },
  { .length =  6, .dlc =  6, .length_min =  6 },
  { .length =  7, .dlc =  7, .length_min =  7 },
  { .length =  8, .dlc =  8, .length_min =  8 },
  { .length =  9, .dlc =  9, .length_min = 12 },
  { .length = 10, .dlc =  9, .length_min = 12 },
  { .length = 11, .dlc =  9, .length_min = 12 },
  { .length = 12, .dlc =  9, .length_min = 12 },
  { .length = 13, .dlc = 10, .length_min = 16 },
  { .length = 14, .dlc = 10, .length_min = 16 },
  { .length = 15, .dlc = 10, .length_min = 16 },
  { .length = 16, .dlc = 10, .length_min = 16 },
  { .length = 17, .dlc = 11, .length_min = 20 },
  { .length = 18, .dlc = 11, .length_min = 20 },
  { .length = 19, .dlc = 11, .length_min = 20 },
  { .length = 20, .dlc = 11, .length_min = 20 },
  { .length = 21, .dlc = 12, .length_min = 24 },
  { .length = 22, .dlc = 12, .length_min = 24 },
  { .length = 23, .dlc = 12, .length_min = 24 },
  { .length = 24, .dlc = 12, .length_min = 24 },
  { .length = 25, .dlc = 13, .length_min = 32 },
  { .length = 26, .dlc = 13, .length_min = 32 },
  { .length = 27, .dlc = 13, .length_min = 32 },
  { .length = 28, .dlc = 13, .length_min = 32 },
  { .length = 29, .dlc = 13, .length_min = 32 },
  { .length = 30, .dlc = 13, .length_min = 32 },
  { .length = 31, .dlc = 13, .length_min = 32 },
  { .length = 32, .dlc = 13, .length_min = 32 },
  { .length = 33, .dlc = 14, .length_min = 48 },
  { .length = 34, .dlc = 14, .length_min = 48 },
  { .length = 35, .dlc = 14, .length_min = 48 },
  { .length = 36, .dlc = 14, .length_min = 48 },
  { .length = 37, .dlc = 14, .length_min = 48 },
  { .length = 38, .dlc = 14, .length_min = 48 },
  { .length = 39, .dlc = 14, .length_min = 48 },
  { .length = 40, .dlc = 14, .length_min = 48 },
  { .length = 41, .dlc = 14, .length_min = 48 },
  { .length = 42, .dlc = 14, .length_min = 48 },
  { .length = 43, .dlc = 14, .length_min = 48 },
  { .length = 44, .dlc = 14, .length_min = 48 },
  { .length = 45, .dlc = 14, .length_min = 48 },
  { .length = 46, .dlc = 14, .length_min = 48 },
  { .length = 47, .dlc = 14, .length_min = 48 },
  { .length = 48, .dlc = 14, .length_min = 48 },
  { .length = 49, .dlc = 15, .length_min = 64 },
  { .length = 50, .dlc = 15, .length_min = 64 },
  { .length = 51, .dlc = 15, .length_min = 64 },
  { .length = 52, .dlc = 15, .length_min = 64 },
  { .length = 53, .dlc = 15, .length_min = 64 },
  { .length = 54, .dlc = 15, .length_min = 64 },
  { .length = 55, .dlc = 15, .length_min = 64 },
  { .length = 56, .dlc = 15, .length_min = 64 },
  { .length = 57, .dlc = 15, .length_min = 64 },
  { .length = 58, .dlc = 15, .length_min = 64 },
  { .length = 59, .dlc = 15, .length_min = 64 },
  { .length = 60, .dlc = 15, .length_min = 64 },
  { .length = 61, .dlc = 15, .length_min = 64 },
  { .length = 62, .dlc = 15, .length_min = 64 },
  { .length = 63, .dlc = 15, .length_min = 64 },
  { .length = 64, .dlc = 15, .length_min = 64 },
  { .length = 65, .dlc =  0, .length_min =  0 }
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

class CanMessage : public ::testing::Test
{
  protected:

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
};

TEST_F(CanMessage, MappingsOfDlcAndLengthsAreAsExpected)
{
  // IME: core - how to handle dlc/lengths > 8?

  for (uint16_t dlc = 0; dlc <= UINT8_MAX; dlc++) {
    uint8_t expected = (dlc <= 8) ? (uint8_t)dlc : 8;
    ASSERT_EQ(CanMessageGetLength(false, (uint8_t)dlc), expected);
  }

  for (uint16_t length = 0; length <= UINT8_MAX; length++) {
    uint8_t expected = (length <= 8) ? (uint8_t)length : 0;
    ASSERT_EQ(CanMessageGetDlc(false, (uint8_t)length), expected);
  }

  for (size_t index = 0; index < (sizeof(test_assignment_dlc_fd) / sizeof(TEST_ASSIGNMENT_DLC_FD_t)); index++) {
    ASSERT_EQ(CanMessageGetDlc(true, test_assignment_dlc_fd[index].length), test_assignment_dlc_fd[index].dlc);
    ASSERT_EQ(CanMessageGetLength(true, test_assignment_dlc_fd[index].dlc), test_assignment_dlc_fd[index].length_min);
  }
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
