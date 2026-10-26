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
#pragma once

/*============================================================================*/
/* I N C L U D E S                                                            */
/*============================================================================*/

#include "hal/ICompiler.h"
#include "middleware/IAssert.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

enum e_GPIO_TYPE  : unsigned int {
  GPIO_TYPE_PUSH_PULL_OUTPUT
};

enum e_GPIO_STATE  : unsigned int {
  GPIO_STATE_LOW,
  GPIO_STATE_HIGH
};

enum e_GPIO_PIN  : unsigned int {
  GPIO_PIN_GREEN
};

enum e_GPIO_PERIPHERY  : unsigned int {
  GPIO_PERIPHERY_NONE
};

static bool mock_state_green;

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

COMPILER_INLINE void GPIO_INIT(const e_GPIO_PIN gpio, const e_GPIO_TYPE type, const e_GPIO_PERIPHERY periphery, const e_GPIO_STATE state)
{
  ASSERT_FATAL(gpio == GPIO_PIN_GREEN);
  ASSERT_FATAL(type == GPIO_TYPE_PUSH_PULL_OUTPUT);
  ASSERT_FATAL(periphery == GPIO_PERIPHERY_NONE);
  ASSERT_FATAL((state == GPIO_STATE_LOW) || (state == GPIO_STATE_HIGH));

  mock_state_green = state == GPIO_STATE_LOW;
}

COMPILER_INLINE void GPIO_SET(const e_GPIO_PIN gpio, const e_GPIO_STATE state)
{
  ASSERT_FATAL(gpio == GPIO_PIN_GREEN);
  ASSERT_FATAL((state == GPIO_STATE_LOW) || (state == GPIO_STATE_HIGH));

  mock_state_green = state == GPIO_STATE_LOW;
}

COMPILER_INLINE bool MOCK_GREEN_IS_ACTIVE(void)
{
  return mock_state_green;
}

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
