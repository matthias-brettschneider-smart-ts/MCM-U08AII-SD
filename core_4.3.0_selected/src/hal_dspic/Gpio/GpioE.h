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
#include "hal/IController.h"
#include "hal/IGpio.h"
#include "middleware/IAssert.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

enum e_GPIO_TYPE {
  GPIO_TYPE_UNUSED,
  GPIO_TYPE_DIGITAL_INPUT,
  GPIO_TYPE_ANALOG_INPUT,
  GPIO_TYPE_PUSH_PULL_OUTPUT,
  GPIO_TYPE_OPEN_DRAIN_OUTPUT,
  GPIO_TYPE_PERIPHERY
};

enum e_GPIO_STATE {
  GPIO_STATE_UNDEFINED,
  GPIO_STATE_HIGH,
  GPIO_STATE_LOW,
  GPIO_STATE_ACTIVE,
  GPIO_STATE_INACTIVE
};

enum e_GPIO_PIN {
  GPIO_PIN_NONE,
  GPIO_PIN_A0,
  GPIO_PIN_A1,
  GPIO_PIN_A2,
  GPIO_PIN_A3,
  GPIO_PIN_A4,
  GPIO_PIN_B0,
  GPIO_PIN_B1,
  GPIO_PIN_B2,
  GPIO_PIN_B3,
  GPIO_PIN_B4,
  GPIO_PIN_B5,
  GPIO_PIN_B6,
  GPIO_PIN_B7,
  GPIO_PIN_B8,
  GPIO_PIN_B9,
  GPIO_PIN_B11,
  GPIO_PIN_B12,
  GPIO_PIN_B13,
  GPIO_PIN_B14,
  GPIO_PIN_B15,
  GPIO_PIN_C0,
  GPIO_PIN_C1,
  GPIO_PIN_C2,
  GPIO_PIN_C3,
  GPIO_PIN_C4,
  GPIO_PIN_C5,
  GPIO_PIN_C6,
  GPIO_PIN_C7,
  GPIO_PIN_C8,
  GPIO_PIN_C9,
  GPIO_PIN_C10,
  GPIO_PIN_C12,
  GPIO_PIN_C13,
  GPIO_PIN_LAST
};

enum e_GPIO_PERIPHERY {
  GPIO_PERIPHERY_CAN1RX,
  GPIO_PERIPHERY_CAN1TX,
  GPIO_PERIPHERY_CAN2RX,
  GPIO_PERIPHERY_CAN2TX,
  GPIO_PERIPHERY_INT2,
  GPIO_PERIPHERY_SPI1CLOCK,
  GPIO_PERIPHERY_SPI1DI,
  GPIO_PERIPHERY_SPI1DO,
  GPIO_PERIPHERY_UART2RX,
  GPIO_PERIPHERY_UART2TX,
  GPIO_PERIPHERY_NONE
};

#define GPIO_HARDWARE_ANS_ANALOG                                               1
#define GPIO_HARDWARE_ANS_DIGITAL                                              0
#define GPIO_HARDWARE_TRIS_INPUT                                               1
#define GPIO_HARDWARE_TRIS_OUTPUT                                              0
#define GPIO_HARDWARE_LATCH_HIGH                                               1
#define GPIO_HARDWARE_LATCH_LOW                                                0
#define GPIO_HARDWARE_LATCH_ACTIVE                                             0
#define GPIO_HARDWARE_LATCH_INACTIVE                                           1
#define GPIO_HARDWARE_PORT_HIGH                                                1
#define GPIO_HARDWARE_PORT_LOW                                                 0
#define GPIO_HARDWARE_ODC_ENABLE                                               1
#define GPIO_HARDWARE_ODC_DISABLE                                              0

#define GPIO_INIT_NO_ODC(port_pin, active)
#define GPIO_INIT_ODC(port_pin, active) { \
    _ODC##port_pin = active ? GPIO_HARDWARE_ODC_ENABLE : GPIO_HARDWARE_ODC_DISABLE; \
}

#define GPIO_INIT_NO_ANALOG(port_pin, active)
#define GPIO_INIT_ANALOG(port_pin, active) { \
    _ANS##port_pin = active ? GPIO_HARDWARE_ANS_ANALOG : GPIO_HARDWARE_ANS_DIGITAL; \
}

#define GPIO_INIT_PERIPHERY(periphery, rp) { \
    if (periphery == GPIO_PERIPHERY_INT2)            { _INT2R = rp; } \
    else if (periphery == GPIO_PERIPHERY_CAN1RX)     { _C1RXR = rp; } \
    else if (periphery == GPIO_PERIPHERY_CAN1TX)     { _RP##rp##R = _RPOUT_C1TX; } \
    else if (periphery == GPIO_PERIPHERY_CAN2RX)     { _C2RXR = rp; } \
    else if (periphery == GPIO_PERIPHERY_CAN2TX)     { _RP##rp##R = _RPOUT_C2TX; } \
    else if (periphery == GPIO_PERIPHERY_SPI1CLOCK)  { _RP##rp##R = _RPOUT_SCK1; } \
    else if (periphery == GPIO_PERIPHERY_SPI1DI)     { _SDI1R = rp; } \
    else if (periphery == GPIO_PERIPHERY_SPI1DO)     { _RP##rp##R = _RPOUT_SDO1; } \
    else if (periphery == GPIO_PERIPHERY_UART2RX)    { _U2RXR = rp; } \
    else if (periphery == GPIO_PERIPHERY_UART2TX)    { _RP##rp##R = _RPOUT_U2TX; } \
    else { \
      ASSERT_FATAL(false); \
    } \
}

#define MACRO_SELECT_INTERNAL(a, ...)                             a##__VA_ARGS__
#define MACRO_SELECT(macro, variant, ...)          MACRO_SELECT_INTERNAL(macro, variant)(__VA_ARGS__)

#define GPIO_INIT_MATCHING(gpio, port_pin, rp, supports_open_drain, supports_analog, type, periphery, state) { \
    if (gpio == GPIO_PIN_##port_pin) { \
      if ((type == GPIO_TYPE_ANALOG_INPUT) && (periphery == GPIO_PERIPHERY_NONE) && (state == GPIO_STATE_UNDEFINED)) { \
        _TRIS##port_pin = GPIO_HARDWARE_TRIS_INPUT; \
        MACRO_SELECT(GPIO_INIT_, supports_open_drain, port_pin, false); \
        MACRO_SELECT(GPIO_INIT_, supports_analog, port_pin, true); \
      } \
      else if ((type == GPIO_TYPE_DIGITAL_INPUT) && (periphery == GPIO_PERIPHERY_NONE) && (state == GPIO_STATE_UNDEFINED)) { \
        _TRIS##port_pin = GPIO_HARDWARE_TRIS_INPUT; \
        MACRO_SELECT(GPIO_INIT_, supports_open_drain, port_pin, false); \
        MACRO_SELECT(GPIO_INIT_, supports_analog, port_pin, false); \
      } \
      else if ((type == GPIO_TYPE_PUSH_PULL_OUTPUT) && (periphery == GPIO_PERIPHERY_NONE) && ((state == GPIO_STATE_HIGH) || (state == GPIO_STATE_LOW))) { \
        MACRO_SELECT(GPIO_INIT_, supports_analog, port_pin, false); \
        _LAT##port_pin = (state == GPIO_STATE_HIGH) ? GPIO_HARDWARE_LATCH_HIGH : GPIO_HARDWARE_LATCH_LOW; \
        MACRO_SELECT(GPIO_INIT_, supports_open_drain, port_pin, false); \
        _TRIS##port_pin = GPIO_HARDWARE_TRIS_OUTPUT; \
      } \
      else if ((type == GPIO_TYPE_OPEN_DRAIN_OUTPUT) && (periphery == GPIO_PERIPHERY_NONE) && ((state == GPIO_STATE_ACTIVE) || (state == GPIO_STATE_INACTIVE))) { \
        MACRO_SELECT(GPIO_INIT_, supports_analog, port_pin, false); \
        _LAT##port_pin = (state == GPIO_STATE_ACTIVE) ? GPIO_HARDWARE_LATCH_ACTIVE : GPIO_HARDWARE_LATCH_INACTIVE; \
        MACRO_SELECT(GPIO_INIT_, supports_open_drain, port_pin, true); \
        _TRIS##port_pin = GPIO_HARDWARE_TRIS_OUTPUT; \
      } \
      else if ((type == GPIO_TYPE_PERIPHERY) && (periphery != GPIO_PERIPHERY_NONE)) { \
        MACRO_SELECT(GPIO_INIT_, supports_analog, port_pin, false); \
        GPIO_INIT_PERIPHERY(periphery, rp); \
      } \
      else { \
        ASSERT_FATAL(false); \
      } \
    } \
}

#define GPIO_GET_MATCHING(gpio, port_pin, state) { \
    if (gpio == GPIO_PIN_##port_pin) { \
      state = (_R##port_pin == GPIO_HARDWARE_PORT_HIGH) ? GPIO_STATE_HIGH : GPIO_STATE_LOW; \
    } \
}

#define GPIO_SET_MATCHING(gpio, port_pin, state) { \
    if (gpio == GPIO_PIN_##port_pin) { \
      if ((state == GPIO_STATE_HIGH) || (state == GPIO_STATE_LOW)) { \
        _LAT##port_pin = (state == GPIO_STATE_HIGH) ? GPIO_HARDWARE_LATCH_HIGH : GPIO_HARDWARE_LATCH_LOW; \
      } \
      else if ((state == GPIO_STATE_ACTIVE) || (state == GPIO_STATE_INACTIVE)) { \
        _LAT##port_pin = (state == GPIO_STATE_ACTIVE) ? GPIO_HARDWARE_LATCH_ACTIVE : GPIO_HARDWARE_LATCH_INACTIVE; \
      } \
      else { \
        ASSERT_FATAL(false); \
      } \
    } \
}

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Initialize GPIO pin.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void GPIO_INIT(const e_GPIO_PIN gpio, const e_GPIO_TYPE type, const e_GPIO_PERIPHERY periphery, const e_GPIO_STATE state)
{
  ASSERT_FATAL(gpio < GPIO_PIN_LAST);

  GPIO_INIT_MATCHING(gpio,  A0, 16, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A1, 17, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A2, 18, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A3, 19, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A4, 20, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B0, 32, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B1, 33, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B2, 34, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B3, 35, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B4, 36, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B5, 37, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B6, 38, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B7, 39, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B8, 40, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B9, 41, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B11, 43, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B12, 44, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B13, 45, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B14, 46, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B15, 47, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C0, 48, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C1, 49, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C2, 50, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C3, 51, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C4, 52, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C5, 53, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C6, 54, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C7, 55, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C8, 56, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C9, 57, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, C10, 58, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, C12, 60, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, C13, 61, ODC, NO_ANALOG, type, periphery, state);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get current state of GPIO pin.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE e_GPIO_STATE GPIO_GET(const e_GPIO_PIN gpio)
{
  ASSERT_FATAL(gpio < GPIO_PIN_LAST);

  e_GPIO_STATE state = GPIO_STATE_UNDEFINED;

  GPIO_GET_MATCHING(gpio, A0, state);
  GPIO_GET_MATCHING(gpio, A1, state);
  GPIO_GET_MATCHING(gpio, A2, state);
  GPIO_GET_MATCHING(gpio, A3, state);
  GPIO_GET_MATCHING(gpio, A4, state);
  GPIO_GET_MATCHING(gpio, B0, state);
  GPIO_GET_MATCHING(gpio, B1, state);
  GPIO_GET_MATCHING(gpio, B2, state);
  GPIO_GET_MATCHING(gpio, B3, state);
  GPIO_GET_MATCHING(gpio, B4, state);
  GPIO_GET_MATCHING(gpio, B5, state);
  GPIO_GET_MATCHING(gpio, B6, state);
  GPIO_GET_MATCHING(gpio, B7, state);
  GPIO_GET_MATCHING(gpio, B8, state);
  GPIO_GET_MATCHING(gpio, B9, state);
  GPIO_GET_MATCHING(gpio, B11, state);
  GPIO_GET_MATCHING(gpio, B12, state);
  GPIO_GET_MATCHING(gpio, B13, state);
  GPIO_GET_MATCHING(gpio, B14, state);
  GPIO_GET_MATCHING(gpio, B15, state);
  GPIO_GET_MATCHING(gpio, C0, state);
  GPIO_GET_MATCHING(gpio, C1, state);
  GPIO_GET_MATCHING(gpio, C2, state);
  GPIO_GET_MATCHING(gpio, C3, state);
  GPIO_GET_MATCHING(gpio, C4, state);
  GPIO_GET_MATCHING(gpio, C5, state);
  GPIO_GET_MATCHING(gpio, C6, state);
  GPIO_GET_MATCHING(gpio, C7, state);
  GPIO_GET_MATCHING(gpio, C8, state);
  GPIO_GET_MATCHING(gpio, C9, state);
  GPIO_GET_MATCHING(gpio, C10, state);
  GPIO_GET_MATCHING(gpio, C12, state);
  GPIO_GET_MATCHING(gpio, C13, state);

  return state;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Set state of GPIO pin.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void GPIO_SET(const e_GPIO_PIN gpio, const e_GPIO_STATE state)
{
  ASSERT_FATAL(gpio < GPIO_PIN_LAST);

  GPIO_SET_MATCHING(gpio, A0, state);
  GPIO_SET_MATCHING(gpio, A1, state);
  GPIO_SET_MATCHING(gpio, A2, state);
  GPIO_SET_MATCHING(gpio, A3, state);
  GPIO_SET_MATCHING(gpio, A4, state);
  GPIO_SET_MATCHING(gpio, B0, state);
  GPIO_SET_MATCHING(gpio, B1, state);
  GPIO_SET_MATCHING(gpio, B2, state);
  GPIO_SET_MATCHING(gpio, B3, state);
  GPIO_SET_MATCHING(gpio, B4, state);
  GPIO_SET_MATCHING(gpio, B5, state);
  GPIO_SET_MATCHING(gpio, B6, state);
  GPIO_SET_MATCHING(gpio, B7, state);
  GPIO_SET_MATCHING(gpio, B8, state);
  GPIO_SET_MATCHING(gpio, B9, state);
  GPIO_SET_MATCHING(gpio, B11, state);
  GPIO_SET_MATCHING(gpio, B12, state);
  GPIO_SET_MATCHING(gpio, B13, state);
  GPIO_SET_MATCHING(gpio, B14, state);
  GPIO_SET_MATCHING(gpio, B15, state);
  GPIO_SET_MATCHING(gpio, C0, state);
  GPIO_SET_MATCHING(gpio, C1, state);
  GPIO_SET_MATCHING(gpio, C2, state);
  GPIO_SET_MATCHING(gpio, C3, state);
  GPIO_SET_MATCHING(gpio, C4, state);
  GPIO_SET_MATCHING(gpio, C5, state);
  GPIO_SET_MATCHING(gpio, C6, state);
  GPIO_SET_MATCHING(gpio, C7, state);
  GPIO_SET_MATCHING(gpio, C8, state);
  GPIO_SET_MATCHING(gpio, C9, state);
  GPIO_SET_MATCHING(gpio, C10, state);
  GPIO_SET_MATCHING(gpio, C12, state);
  GPIO_SET_MATCHING(gpio, C13, state);
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
