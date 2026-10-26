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
#include "hal/IGpio.h"
#include "hal/IPinning.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

#define LED_STATE_ON                                           GPIO_STATE_ACTIVE
#define LED_STATE_OFF                                        GPIO_STATE_INACTIVE

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Activate the red LED to signal permanent errors.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void LED_SIGNAL_PERMANENT_ERROR(void)
{
  GPIO_INIT(PIN_STATUS_LED_GREEN, GPIO_TYPE_OPEN_DRAIN_OUTPUT, GPIO_PERIPHERY_NONE, LED_STATE_OFF);
  GPIO_INIT(PIN_STATUS_LED_RED, GPIO_TYPE_OPEN_DRAIN_OUTPUT, GPIO_PERIPHERY_NONE, LED_STATE_ON);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Activate or deactivate the green or the red LED.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void LED_SET(const e_LED_COLOR color, const e_LED_TODO todo)
{
  if (todo == LED_TODO_DEACTIVATE) {
    GPIO_SET(PIN_STATUS_LED_GREEN, LED_STATE_OFF);
    GPIO_SET(PIN_STATUS_LED_RED, LED_STATE_OFF);
  }
  else if (todo == LED_TODO_ACTIVATE) {
    if (color == LED_COLOR_RED) {
      GPIO_SET(PIN_STATUS_LED_GREEN, LED_STATE_OFF);
      GPIO_SET(PIN_STATUS_LED_RED, LED_STATE_ON);
    }
    else {
      GPIO_SET(PIN_STATUS_LED_RED, LED_STATE_OFF);
      GPIO_SET(PIN_STATUS_LED_GREEN, LED_STATE_ON);
    }
  }
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
