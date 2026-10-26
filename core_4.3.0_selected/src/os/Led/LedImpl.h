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

#ifndef LED_VARIANT
#error "error compile flags - LED_VARIANT undefined"
#elif ((LED_VARIANT < 1) || (LED_VARIANT > 7))
#error "error compile flags - LED_VARIANT invalid"
#endif

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

#define LED_VARIANT_NONE                                                      1
#define LED_VARIANT_MONO_PUSH_PULL                                            2
#define LED_VARIANT_MONO_PUSH_PULL_INVERTED                                   3
#define LED_VARIANT_MONO_OPEN_DRAIN                                           4
#define LED_VARIANT_DUAL_PUSH_PULL                                            5
#define LED_VARIANT_DUAL_PUSH_PULL_INVERTED                                   6
#define LED_VARIANT_DUAL_OPEN_DRAIN                                           7

#define LED_MONO    ((LED_VARIANT == LED_VARIANT_MONO_PUSH_PULL) || (LED_VARIANT == LED_VARIANT_MONO_PUSH_PULL_INVERTED) || (LED_VARIANT == LED_VARIANT_MONO_OPEN_DRAIN))
#define LED_DUAL    ((LED_VARIANT == LED_VARIANT_DUAL_PUSH_PULL) || (LED_VARIANT == LED_VARIANT_DUAL_PUSH_PULL_INVERTED) || (LED_VARIANT == LED_VARIANT_DUAL_OPEN_DRAIN))

#define LED_PUSH_PULL                  ((LED_VARIANT == LED_VARIANT_MONO_PUSH_PULL) || (LED_VARIANT == LED_VARIANT_DUAL_PUSH_PULL))
#define LED_PUSH_PULL_INVERTED         ((LED_VARIANT == LED_VARIANT_MONO_PUSH_PULL_INVERTED) || (LED_VARIANT == LED_VARIANT_DUAL_PUSH_PULL_INVERTED))
#define LED_OPEN_DRAIN                 ((LED_VARIANT == LED_VARIANT_MONO_OPEN_DRAIN) || (LED_VARIANT == LED_VARIANT_DUAL_OPEN_DRAIN))

#if (LED_PUSH_PULL)
#define LED_TYPE                                      GPIO_TYPE_PUSH_PULL_OUTPUT
#define LED_STATE_ON                                             GPIO_STATE_HIGH
#define LED_STATE_OFF                                             GPIO_STATE_LOW
#elif (LED_PUSH_PULL_INVERTED)
#define LED_TYPE                                      GPIO_TYPE_PUSH_PULL_OUTPUT
#define LED_STATE_ON                                              GPIO_STATE_LOW
#define LED_STATE_OFF                                            GPIO_STATE_HIGH
#elif (LED_OPEN_DRAIN)
#define LED_TYPE                                     GPIO_TYPE_OPEN_DRAIN_OUTPUT
#define LED_STATE_ON                                           GPIO_STATE_ACTIVE
#define LED_STATE_OFF                                        GPIO_STATE_INACTIVE
#endif

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Activate the LED(s) to signal permanent errors.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void LED_SIGNAL_PERMANENT_ERROR(void)
{
#if (LED_MONO)

  GPIO_INIT(PIN_STATUS_LED_GREEN, LED_TYPE, GPIO_PERIPHERY_NONE, LED_STATE_ON);

#elif (LED_DUAL)

  GPIO_INIT(PIN_STATUS_LED_GREEN, LED_TYPE, GPIO_PERIPHERY_NONE, LED_STATE_OFF);
  GPIO_INIT(PIN_STATUS_LED_RED, LED_TYPE, GPIO_PERIPHERY_NONE, LED_STATE_ON);

#endif
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Activate or deactivate the LED(s).
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void LED_SET(const e_LED_COLOR color, const e_LED_TODO todo)
{
#if (LED_MONO)

  (void)color;

  if (todo == LED_TODO_DEACTIVATE)
    GPIO_SET(PIN_STATUS_LED_GREEN, LED_STATE_OFF);
  else if (todo == LED_TODO_ACTIVATE)
    GPIO_SET(PIN_STATUS_LED_GREEN, LED_STATE_ON);

#elif (LED_DUAL)

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

#else

  (void)color;
  (void)todo;

#endif
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
