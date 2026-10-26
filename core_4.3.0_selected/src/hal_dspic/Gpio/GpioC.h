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
#include "middleware/IError.h"
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
  GPIO_PIN_B10,
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
  GPIO_PIN_C11,
  GPIO_PIN_C12,
  GPIO_PIN_C13,
  GPIO_PIN_C14,
  GPIO_PIN_C15,
  GPIO_PIN_D0,
  GPIO_PIN_D1,
  GPIO_PIN_D2,
  GPIO_PIN_D3,
  GPIO_PIN_D4,
  GPIO_PIN_D5,
  GPIO_PIN_D6,
  GPIO_PIN_D7,
  GPIO_PIN_D8,
  GPIO_PIN_D9,
  GPIO_PIN_D10,
  GPIO_PIN_D11,
  GPIO_PIN_D12,
  GPIO_PIN_D13,
  GPIO_PIN_D14,
  GPIO_PIN_D15,
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508)
  GPIO_PIN_E0,
  GPIO_PIN_E1,
  GPIO_PIN_E2,
  GPIO_PIN_E3,
  GPIO_PIN_E4,
  GPIO_PIN_E5,
  GPIO_PIN_E6,
  GPIO_PIN_E7,
  GPIO_PIN_E8,
  GPIO_PIN_E9,
  GPIO_PIN_E10,
  GPIO_PIN_E11,
  GPIO_PIN_E12,
  GPIO_PIN_E13,
  GPIO_PIN_E14,
  GPIO_PIN_E15,
#endif
  GPIO_PIN_LAST
};

enum e_GPIO_PERIPHERY {
  GPIO_PERIPHERY_CAN1RX,
  GPIO_PERIPHERY_CAN1TX,
  GPIO_PERIPHERY_SPI1CLOCK,
  GPIO_PERIPHERY_SPI1DI,
  GPIO_PERIPHERY_SPI1DO,
  GPIO_PERIPHERY_UART1RX,
  GPIO_PERIPHERY_UART1TX,
  GPIO_PERIPHERY_UART2RX,
  GPIO_PERIPHERY_UART2TX,
  GPIO_PERIPHERY_INT1,
  GPIO_PERIPHERY_INT2,
  GPIO_PERIPHERY_INT3,
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
    _ANSEL##port_pin = active ? GPIO_HARDWARE_ANS_ANALOG : GPIO_HARDWARE_ANS_DIGITAL; \
}

#define GPIO_INIT_NO_RP(gpio, port_pin, rp, periphery)
#define GPIO_INIT_RP(gpio, port_pin, rp, periphery) { \
    if (periphery == GPIO_PERIPHERY_INT1)            { _INT1R = rp; } \
    else if (periphery == GPIO_PERIPHERY_INT2)       { _INT2R = rp; } \
    else if (periphery == GPIO_PERIPHERY_INT3)       { _INT3R = rp; } \
    else if (periphery == GPIO_PERIPHERY_CAN1RX)     { _CAN1RXR = rp; } \
    else if (periphery == GPIO_PERIPHERY_CAN1TX)     { _RP##rp##R = _RPOUT_CAN1TX; } \
    else if (periphery == GPIO_PERIPHERY_SPI1CLOCK)  { _RP##rp##R = _RPOUT_SCK1OUT; } \
    else if (periphery == GPIO_PERIPHERY_SPI1DI)     { _SDI1R = rp; } \
    else if (periphery == GPIO_PERIPHERY_SPI1DO)     { _RP##rp##R = _RPOUT_SDO1; } \
    else if (periphery == GPIO_PERIPHERY_UART1RX)    { _U1RXR = rp; } \
    else if (periphery == GPIO_PERIPHERY_UART1TX)    { _RP##rp##R = _RPOUT_U1TX; } \
    else if (periphery == GPIO_PERIPHERY_UART2RX)    { _U2RXR = rp; } \
    else if (periphery == GPIO_PERIPHERY_UART2TX)    { _RP##rp##R = _RPOUT_U2TX; } \
    else { \
      ASSERT_FATAL(false); \
    } \
}

#define MACRO_SELECT_INTERNAL(a, ...)                             a##__VA_ARGS__
#define MACRO_SELECT(macro, variant, ...)          MACRO_SELECT_INTERNAL(macro, variant)(__VA_ARGS__)

#define GPIO_INIT_MATCHING(gpio, port_pin, supports_rp, rp, supports_open_drain, supports_analog, type, periphery, state) { \
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
        MACRO_SELECT(GPIO_INIT_, supports_rp, gpio, port_pin, rp, periphery); \
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

  GPIO_INIT_MATCHING(gpio,  A0, NO_RP, -1, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A1, NO_RP, -1, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A2, NO_RP, -1, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A3, NO_RP, -1, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A4, NO_RP, -1, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B0,    RP, 32, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B1,    RP, 33, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B2,    RP, 34, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B3,    RP, 35, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B4,    RP, 36, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B5,    RP, 37, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B6,    RP, 38, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B7,    RP, 39, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B8,    RP, 40, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B9,    RP, 41, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B10,    RP, 42, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B11,    RP, 43, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B12,    RP, 44, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B13,    RP, 45, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B14,    RP, 46, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B15,    RP, 47, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C0,    RP, 48, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C1,    RP, 49, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C2,    RP, 50, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C3,    RP, 51, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C4,    RP, 52, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C5,    RP, 53, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C6,    RP, 54, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C7,    RP, 55, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C8,    RP, 56, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C9,    RP, 57, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, C10,    RP, 58, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, C11,    RP, 59, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, C12,    RP, 60, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, C13,    RP, 61, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, C14,    RP, 62, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, C15,    RP, 63, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D0,    RP, 64, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D1,    RP, 65, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D2,    RP, 66, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D3,    RP, 67, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D4,    RP, 68, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D5,    RP, 69, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D6,    RP, 70, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D7,    RP, 71, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D8,    RP, 72, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D9,    RP, 73, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, D10,    RP, 74, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, D11,    RP, 75, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, D12,    RP, 76, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, D13,    RP, 77, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, D14,    RP, 78, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, D15,    RP, 79, ODC, NO_ANALOG, type, periphery, state);
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508)
  GPIO_INIT_MATCHING(gpio,  E0, NO_RP, -1, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E1, NO_RP, -1, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E2, NO_RP, -1, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E3, NO_RP, -1, ODC,    ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E4, NO_RP, -1, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E5, NO_RP, -1, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E6, NO_RP, -1, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E7, NO_RP, -1, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E8, NO_RP, -1, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E9, NO_RP, -1, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, E10, NO_RP, -1, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, E11, NO_RP, -1, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, E12, NO_RP, -1, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, E13, NO_RP, -1, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, E14, NO_RP, -1, ODC, NO_ANALOG, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, E15, NO_RP, -1, ODC, NO_ANALOG, type, periphery, state);
#endif
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

  GPIO_GET_MATCHING(gpio,  A0, state);
  GPIO_GET_MATCHING(gpio,  A1, state);
  GPIO_GET_MATCHING(gpio,  A2, state);
  GPIO_GET_MATCHING(gpio,  A3, state);
  GPIO_GET_MATCHING(gpio,  A4, state);
  GPIO_GET_MATCHING(gpio,  B0, state);
  GPIO_GET_MATCHING(gpio,  B1, state);
  GPIO_GET_MATCHING(gpio,  B2, state);
  GPIO_GET_MATCHING(gpio,  B3, state);
  GPIO_GET_MATCHING(gpio,  B4, state);
  GPIO_GET_MATCHING(gpio,  B5, state);
  GPIO_GET_MATCHING(gpio,  B6, state);
  GPIO_GET_MATCHING(gpio,  B7, state);
  GPIO_GET_MATCHING(gpio,  B8, state);
  GPIO_GET_MATCHING(gpio,  B9, state);
  GPIO_GET_MATCHING(gpio, B10, state);
  GPIO_GET_MATCHING(gpio, B11, state);
  GPIO_GET_MATCHING(gpio, B12, state);
  GPIO_GET_MATCHING(gpio, B13, state);
  GPIO_GET_MATCHING(gpio, B14, state);
  GPIO_GET_MATCHING(gpio, B15, state);
  GPIO_GET_MATCHING(gpio,  C0, state);
  GPIO_GET_MATCHING(gpio,  C1, state);
  GPIO_GET_MATCHING(gpio,  C2, state);
  GPIO_GET_MATCHING(gpio,  C3, state);
  GPIO_GET_MATCHING(gpio,  C4, state);
  GPIO_GET_MATCHING(gpio,  C5, state);
  GPIO_GET_MATCHING(gpio,  C6, state);
  GPIO_GET_MATCHING(gpio,  C7, state);
  GPIO_GET_MATCHING(gpio,  C8, state);
  GPIO_GET_MATCHING(gpio,  C9, state);
  GPIO_GET_MATCHING(gpio, C10, state);
  GPIO_GET_MATCHING(gpio, C11, state);
  GPIO_GET_MATCHING(gpio, C12, state);
  GPIO_GET_MATCHING(gpio, C13, state);
  GPIO_GET_MATCHING(gpio, C14, state);
  GPIO_GET_MATCHING(gpio, C15, state);
  GPIO_GET_MATCHING(gpio,  D0, state);
  GPIO_GET_MATCHING(gpio,  D1, state);
  GPIO_GET_MATCHING(gpio,  D2, state);
  GPIO_GET_MATCHING(gpio,  D3, state);
  GPIO_GET_MATCHING(gpio,  D4, state);
  GPIO_GET_MATCHING(gpio,  D5, state);
  GPIO_GET_MATCHING(gpio,  D6, state);
  GPIO_GET_MATCHING(gpio,  D7, state);
  GPIO_GET_MATCHING(gpio,  D8, state);
  GPIO_GET_MATCHING(gpio,  D9, state);
  GPIO_GET_MATCHING(gpio, D10, state);
  GPIO_GET_MATCHING(gpio, D11, state);
  GPIO_GET_MATCHING(gpio, D12, state);
  GPIO_GET_MATCHING(gpio, D13, state);
  GPIO_GET_MATCHING(gpio, D14, state);
  GPIO_GET_MATCHING(gpio, D15, state);
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508)
  GPIO_GET_MATCHING(gpio,  E0, state);
  GPIO_GET_MATCHING(gpio,  E1, state);
  GPIO_GET_MATCHING(gpio,  E2, state);
  GPIO_GET_MATCHING(gpio,  E3, state);
  GPIO_GET_MATCHING(gpio,  E4, state);
  GPIO_GET_MATCHING(gpio,  E5, state);
  GPIO_GET_MATCHING(gpio,  E6, state);
  GPIO_GET_MATCHING(gpio,  E7, state);
  GPIO_GET_MATCHING(gpio,  E8, state);
  GPIO_GET_MATCHING(gpio,  E9, state);
  GPIO_GET_MATCHING(gpio, E10, state);
  GPIO_GET_MATCHING(gpio, E11, state);
  GPIO_GET_MATCHING(gpio, E12, state);
  GPIO_GET_MATCHING(gpio, E13, state);
  GPIO_GET_MATCHING(gpio, E14, state);
  GPIO_GET_MATCHING(gpio, E15, state);
#endif

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

  GPIO_SET_MATCHING(gpio,  A0, state);
  GPIO_SET_MATCHING(gpio,  A1, state);
  GPIO_SET_MATCHING(gpio,  A2, state);
  GPIO_SET_MATCHING(gpio,  A3, state);
  GPIO_SET_MATCHING(gpio,  A4, state);
  GPIO_SET_MATCHING(gpio,  B0, state);
  GPIO_SET_MATCHING(gpio,  B1, state);
  GPIO_SET_MATCHING(gpio,  B2, state);
  GPIO_SET_MATCHING(gpio,  B3, state);
  GPIO_SET_MATCHING(gpio,  B4, state);
  GPIO_SET_MATCHING(gpio,  B5, state);
  GPIO_SET_MATCHING(gpio,  B6, state);
  GPIO_SET_MATCHING(gpio,  B7, state);
  GPIO_SET_MATCHING(gpio,  B8, state);
  GPIO_SET_MATCHING(gpio,  B9, state);
  GPIO_SET_MATCHING(gpio, B10, state);
  GPIO_SET_MATCHING(gpio, B11, state);
  GPIO_SET_MATCHING(gpio, B12, state);
  GPIO_SET_MATCHING(gpio, B13, state);
  GPIO_SET_MATCHING(gpio, B14, state);
  GPIO_SET_MATCHING(gpio, B15, state);
  GPIO_SET_MATCHING(gpio,  C0, state);
  GPIO_SET_MATCHING(gpio,  C1, state);
  GPIO_SET_MATCHING(gpio,  C2, state);
  GPIO_SET_MATCHING(gpio,  C3, state);
  GPIO_SET_MATCHING(gpio,  C4, state);
  GPIO_SET_MATCHING(gpio,  C5, state);
  GPIO_SET_MATCHING(gpio,  C6, state);
  GPIO_SET_MATCHING(gpio,  C7, state);
  GPIO_SET_MATCHING(gpio,  C8, state);
  GPIO_SET_MATCHING(gpio,  C9, state);
  GPIO_SET_MATCHING(gpio, C10, state);
  GPIO_SET_MATCHING(gpio, C11, state);
  GPIO_SET_MATCHING(gpio, C12, state);
  GPIO_SET_MATCHING(gpio, C13, state);
  GPIO_SET_MATCHING(gpio, C14, state);
  GPIO_SET_MATCHING(gpio, C15, state);
  GPIO_SET_MATCHING(gpio,  D0, state);
  GPIO_SET_MATCHING(gpio,  D1, state);
  GPIO_SET_MATCHING(gpio,  D2, state);
  GPIO_SET_MATCHING(gpio,  D3, state);
  GPIO_SET_MATCHING(gpio,  D4, state);
  GPIO_SET_MATCHING(gpio,  D5, state);
  GPIO_SET_MATCHING(gpio,  D6, state);
  GPIO_SET_MATCHING(gpio,  D7, state);
  GPIO_SET_MATCHING(gpio,  D8, state);
  GPIO_SET_MATCHING(gpio,  D9, state);
  GPIO_SET_MATCHING(gpio, D10, state);
  GPIO_SET_MATCHING(gpio, D11, state);
  GPIO_SET_MATCHING(gpio, D12, state);
  GPIO_SET_MATCHING(gpio, D13, state);
  GPIO_SET_MATCHING(gpio, D14, state);
  GPIO_SET_MATCHING(gpio, D15, state);
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508)
  GPIO_SET_MATCHING(gpio,  E0, state);
  GPIO_SET_MATCHING(gpio,  E1, state);
  GPIO_SET_MATCHING(gpio,  E2, state);
  GPIO_SET_MATCHING(gpio,  E3, state);
  GPIO_SET_MATCHING(gpio,  E4, state);
  GPIO_SET_MATCHING(gpio,  E5, state);
  GPIO_SET_MATCHING(gpio,  E6, state);
  GPIO_SET_MATCHING(gpio,  E7, state);
  GPIO_SET_MATCHING(gpio,  E8, state);
  GPIO_SET_MATCHING(gpio,  E9, state);
  GPIO_SET_MATCHING(gpio, E10, state);
  GPIO_SET_MATCHING(gpio, E11, state);
  GPIO_SET_MATCHING(gpio, E12, state);
  GPIO_SET_MATCHING(gpio, E13, state);
  GPIO_SET_MATCHING(gpio, E14, state);
  GPIO_SET_MATCHING(gpio, E15, state);
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
