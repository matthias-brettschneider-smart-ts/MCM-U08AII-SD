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
  GPIO_TYPE_OPEN_DRAIN_OUTPUT
};

enum e_GPIO_STATE  : unsigned int {
  GPIO_STATE_ACTIVE,
  GPIO_STATE_INACTIVE
};

enum e_GPIO_PIN  : unsigned int {
  GPIO_PIN_GREEN,
  GPIO_PIN_RED
};

enum e_GPIO_PERIPHERY  : unsigned int {
  GPIO_PERIPHERY_NONE
};

static bool mock_state_green;
static bool mock_state_red;

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

COMPILER_INLINE void GPIO_INIT(const e_GPIO_PIN gpio, const e_GPIO_TYPE type, const e_GPIO_PERIPHERY periphery, const e_GPIO_STATE state)
{
  ASSERT_FATAL((gpio == GPIO_PIN_GREEN) || (gpio == GPIO_PIN_RED));
  ASSERT_FATAL(type == GPIO_TYPE_OPEN_DRAIN_OUTPUT);
  ASSERT_FATAL(periphery == GPIO_PERIPHERY_NONE);
  ASSERT_FATAL((state == GPIO_STATE_ACTIVE) || (state == GPIO_STATE_INACTIVE));

  if (gpio == GPIO_PIN_GREEN)
    mock_state_green = state == GPIO_STATE_ACTIVE;
  if (gpio == GPIO_PIN_RED)
    mock_state_red = state == GPIO_STATE_ACTIVE;
}

COMPILER_INLINE void GPIO_SET(const e_GPIO_PIN gpio, const e_GPIO_STATE state)
{
  ASSERT_FATAL((gpio == GPIO_PIN_GREEN) || (gpio == GPIO_PIN_RED));
  ASSERT_FATAL((state == GPIO_STATE_ACTIVE) || (state == GPIO_STATE_INACTIVE));

  if (gpio == GPIO_PIN_GREEN)
    mock_state_green = state == GPIO_STATE_ACTIVE;
  if (gpio == GPIO_PIN_RED)
    mock_state_red = state == GPIO_STATE_ACTIVE;
}

COMPILER_INLINE bool MOCK_GREEN_IS_ACTIVE(void)
{
  return mock_state_green;
}

COMPILER_INLINE bool MOCK_RED_IS_ACTIVE(void)
{
  return mock_state_red;
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
