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
  GPIO_TYPE_DIGITAL_INPUT_WITH_OPEN_DRAIN,
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
  GPIO_PIN_A5,
  GPIO_PIN_A6,
  GPIO_PIN_A7,
  GPIO_PIN_A9,
  GPIO_PIN_A10,
  GPIO_PIN_A12,
  GPIO_PIN_A13,
  GPIO_PIN_A14,
  GPIO_PIN_A15,
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
  GPIO_PIN_C1,
  GPIO_PIN_C2,
  GPIO_PIN_C3,
  GPIO_PIN_C4,
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
  GPIO_PIN_E0,
  GPIO_PIN_E1,
  GPIO_PIN_E2,
  GPIO_PIN_E3,
  GPIO_PIN_E4,
  GPIO_PIN_E5,
  GPIO_PIN_E6,
  GPIO_PIN_E7,
  GPIO_PIN_F0,
  GPIO_PIN_F1,
  GPIO_PIN_F2,
  GPIO_PIN_F3,
  GPIO_PIN_F4,
  GPIO_PIN_F5,
  GPIO_PIN_F6,
  GPIO_PIN_F7,
  GPIO_PIN_F8,
  GPIO_PIN_F12,
  GPIO_PIN_F13,
  GPIO_PIN_G0,
  GPIO_PIN_G1,
  GPIO_PIN_G2,
  GPIO_PIN_G3,
  GPIO_PIN_G6,
  GPIO_PIN_G7,
  GPIO_PIN_G8,
  GPIO_PIN_G9,
  GPIO_PIN_G12,
  GPIO_PIN_G13,
  GPIO_PIN_G14,
  GPIO_PIN_G15,
  GPIO_PIN_LAST
};

enum e_GPIO_PERIPHERY {
  GPIO_PERIPHERY_ADC1,
  GPIO_PERIPHERY_ADC2,
  GPIO_PERIPHERY_CAN1,
  GPIO_PERIPHERY_SPI1,
  GPIO_PERIPHERY_INT,
  GPIO_PERIPHERY_NONE
};

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

#define GPIO_SET_BIT(reg, bit, active) { \
    if (active) { \
      reg &= ~(1u << (bit % 16)); \
    } \
    else { \
      reg |= (1u << (bit % 16)); \
    } \
}

#define GPIO_ACTIVATE_ANALOG_CHANNEL_FOR_ADC_1(analog_channel, active) { \
    if ((analog_channel >= 0) && (analog_channel <= 15)) { \
      GPIO_SET_BIT(AD1PCFGL, analog_channel, active); \
    } \
    else if ((analog_channel >= 16) && (analog_channel <= 31)) { \
      GPIO_SET_BIT(AD1PCFGH, analog_channel, active); \
    } \
    else { \
      ASSERT_FATAL(false); \
    } \
}

#define GPIO_ACTIVATE_ANALOG_CHANNEL_FOR_ADC_2(analog_channel, active) { \
    if ((analog_channel >= 0) && (analog_channel <= 15)) { \
      GPIO_SET_BIT(AD2PCFGL, analog_channel, active); \
    } \
    else { \
      ASSERT_FATAL(false); \
    } \
}

#define GPIO_INIT_ANALOG_CHANNEL_FOR_ADC_NONE(analog_channel, active_adc_1, active_adc_2)

#define GPIO_INIT_ANALOG_CHANNEL_FOR_ADC_1(analog_channel, active_adc_1, active_adc_2) { \
    GPIO_ACTIVATE_ANALOG_CHANNEL_FOR_ADC_1(analog_channel, active_adc_1); \
}

#define GPIO_INIT_ANALOG_CHANNEL_FOR_ADC_2(analog_channel, active_adc_1, active_adc_2) { \
    GPIO_ACTIVATE_ANALOG_CHANNEL_FOR_ADC_2(analog_channel, active_adc_2); \
}

#define GPIO_INIT_ANALOG_CHANNEL_FOR_ADC_BOTH(analog_channel, active_adc_1, active_adc_2) { \
    GPIO_ACTIVATE_ANALOG_CHANNEL_FOR_ADC_1(analog_channel, active_adc_1); \
    GPIO_ACTIVATE_ANALOG_CHANNEL_FOR_ADC_2(analog_channel, active_adc_2); \
}

#define GPIO_INIT_PERIPHERY(gpio, port_pin, analog_channel, periphery) { \
    if (((periphery == GPIO_PERIPHERY_CAN1) && ((gpio == GPIO_PIN_F0) || (gpio == GPIO_PIN_F1))) || \
        ((periphery == GPIO_PERIPHERY_INT) && (gpio == GPIO_PIN_A14)) || \
        ((periphery == GPIO_PERIPHERY_SPI1) && ((gpio == GPIO_PIN_F6) || (gpio == GPIO_PIN_F7) || (gpio == GPIO_PIN_F8))) || \
        ((periphery == GPIO_PERIPHERY_ADC1) && \
            ((gpio == GPIO_PIN_A6) || (gpio == GPIO_PIN_A7) || (gpio == GPIO_PIN_A12) || (gpio == GPIO_PIN_A13) || \
            (gpio == GPIO_PIN_B0) || (gpio == GPIO_PIN_B1) || (gpio == GPIO_PIN_B2) || (gpio == GPIO_PIN_B3) || \
            (gpio == GPIO_PIN_B4) || (gpio == GPIO_PIN_B5) || (gpio == GPIO_PIN_B6) || (gpio == GPIO_PIN_B7) || \
            (gpio == GPIO_PIN_B8) || (gpio == GPIO_PIN_B9) || (gpio == GPIO_PIN_B10) || (gpio == GPIO_PIN_B11) || \
            (gpio == GPIO_PIN_B12) || (gpio == GPIO_PIN_B13) || (gpio == GPIO_PIN_B14) || (gpio == GPIO_PIN_B15) || \
            (gpio == GPIO_PIN_C1) || (gpio == GPIO_PIN_C2) || (gpio == GPIO_PIN_C3) || (gpio == GPIO_PIN_C4) || \
            (gpio == GPIO_PIN_E0) || (gpio == GPIO_PIN_E1) || (gpio == GPIO_PIN_E2) || (gpio == GPIO_PIN_E3) || \
            (gpio == GPIO_PIN_E4) || (gpio == GPIO_PIN_E5) || (gpio == GPIO_PIN_E6) || (gpio == GPIO_PIN_E7))) || \
        ((periphery == GPIO_PERIPHERY_ADC2) && \
            ((gpio == GPIO_PIN_B0) || (gpio == GPIO_PIN_B1) || (gpio == GPIO_PIN_B2) || (gpio == GPIO_PIN_B3) || \
            (gpio == GPIO_PIN_B4) || (gpio == GPIO_PIN_B5) || (gpio == GPIO_PIN_B6) || (gpio == GPIO_PIN_B7) || \
            (gpio == GPIO_PIN_B8) || (gpio == GPIO_PIN_B9) || (gpio == GPIO_PIN_B10) || (gpio == GPIO_PIN_B11) || \
            (gpio == GPIO_PIN_B12) || (gpio == GPIO_PIN_B13) || (gpio == GPIO_PIN_B14) || (gpio == GPIO_PIN_B15)))) { \
      _TRIS##port_pin = GPIO_HARDWARE_TRIS_INPUT; \
    } \
    else { \
      ASSERT_FATAL(false); \
    } \
}

#define MACRO_SELECT_INTERNAL(a, ...)                             a##__VA_ARGS__
#define MACRO_SELECT(macro, variant, ...)          MACRO_SELECT_INTERNAL(macro, variant)(__VA_ARGS__)

#define GPIO_INIT_MATCHING(gpio, port_pin, supports_open_drain, supports_analog, analog_channel, type, periphery, state) { \
    if (gpio == GPIO_PIN_##port_pin) { \
      if ((type == GPIO_TYPE_DIGITAL_INPUT) && (periphery == GPIO_PERIPHERY_NONE) && (state == GPIO_STATE_UNDEFINED)) { \
        MACRO_SELECT(GPIO_INIT_ANALOG_CHANNEL_FOR_, supports_analog, analog_channel, false, false); \
        MACRO_SELECT(GPIO_INIT_, supports_open_drain, port_pin, false); \
        _TRIS##port_pin = GPIO_HARDWARE_TRIS_INPUT; \
      } \
      else if ((type == GPIO_TYPE_DIGITAL_INPUT_WITH_OPEN_DRAIN) && (periphery == GPIO_PERIPHERY_NONE) && (state == GPIO_STATE_UNDEFINED)) { \
        MACRO_SELECT(GPIO_INIT_ANALOG_CHANNEL_FOR_, supports_analog, analog_channel, false, false); \
        MACRO_SELECT(GPIO_INIT_, supports_open_drain, port_pin, true); \
        _TRIS##port_pin = GPIO_HARDWARE_TRIS_INPUT; \
      } \
      else if ((type == GPIO_TYPE_PUSH_PULL_OUTPUT) && (periphery == GPIO_PERIPHERY_NONE) && ((state == GPIO_STATE_HIGH) || (state == GPIO_STATE_LOW))) { \
        MACRO_SELECT(GPIO_INIT_ANALOG_CHANNEL_FOR_, supports_analog, analog_channel, false, false); \
        MACRO_SELECT(GPIO_INIT_, supports_open_drain, port_pin, false); \
        _LAT##port_pin = (state == GPIO_STATE_HIGH) ? GPIO_HARDWARE_LATCH_HIGH : GPIO_HARDWARE_LATCH_LOW; \
        _TRIS##port_pin = GPIO_HARDWARE_TRIS_OUTPUT; \
      } \
      else if ((type == GPIO_TYPE_OPEN_DRAIN_OUTPUT) && (periphery == GPIO_PERIPHERY_NONE) && ((state == GPIO_STATE_ACTIVE) || (state == GPIO_STATE_INACTIVE))) { \
        MACRO_SELECT(GPIO_INIT_ANALOG_CHANNEL_FOR_, supports_analog, analog_channel, false, false); \
        MACRO_SELECT(GPIO_INIT_, supports_open_drain, port_pin, true); \
        _LAT##port_pin = (state == GPIO_STATE_ACTIVE) ? GPIO_HARDWARE_LATCH_ACTIVE : GPIO_HARDWARE_LATCH_INACTIVE; \
        _TRIS##port_pin = GPIO_HARDWARE_TRIS_OUTPUT; \
      } \
      else if ((type == GPIO_TYPE_PERIPHERY) && (periphery != GPIO_PERIPHERY_NONE)) { \
        GPIO_INIT_PERIPHERY(gpio, port_pin, analog_channel, periphery); \
        if (periphery == GPIO_PERIPHERY_ADC1) { \
          MACRO_SELECT(GPIO_INIT_ANALOG_CHANNEL_FOR_, supports_analog, analog_channel, true, false); \
        } \
        else if (periphery == GPIO_PERIPHERY_ADC2) { \
          MACRO_SELECT(GPIO_INIT_ANALOG_CHANNEL_FOR_, supports_analog, analog_channel, false, true); \
        } \
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

  GPIO_INIT_MATCHING(gpio,  A0,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A1,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A2,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A3,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A4,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A5,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A6, NO_ODC,    ADC_1, 22, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A7, NO_ODC,    ADC_1, 23, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  A9, NO_ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, A10, NO_ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, A12, NO_ODC,    ADC_1, 20, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, A13, NO_ODC,    ADC_1, 21, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, A14,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, A15,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B0, NO_ODC, ADC_BOTH,  0, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B1, NO_ODC, ADC_BOTH,  1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B2, NO_ODC, ADC_BOTH,  2, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B3, NO_ODC, ADC_BOTH,  3, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B4, NO_ODC, ADC_BOTH,  4, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B5, NO_ODC, ADC_BOTH,  5, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B6, NO_ODC, ADC_BOTH,  6, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B7, NO_ODC, ADC_BOTH,  7, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B8, NO_ODC, ADC_BOTH,  8, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  B9, NO_ODC, ADC_BOTH,  9, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B10, NO_ODC, ADC_BOTH, 10, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B11, NO_ODC, ADC_BOTH, 11, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B12, NO_ODC, ADC_BOTH, 12, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B13, NO_ODC, ADC_BOTH, 13, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B14, NO_ODC, ADC_BOTH, 14, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, B15, NO_ODC, ADC_BOTH, 15, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C1, NO_ODC,    ADC_1, 16, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C2, NO_ODC,    ADC_1, 17, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C3, NO_ODC,    ADC_1, 18, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  C4, NO_ODC,    ADC_1, 19, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, C12, NO_ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, C13, NO_ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, C14, NO_ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, C15, NO_ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D0,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D1,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D2,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D3,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D4,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D5,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D6,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D7,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D8,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  D9,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, D10,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, D11,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, D12,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, D13,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, D14,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, D15,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E0, NO_ODC,    ADC_1, 24, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E1, NO_ODC,    ADC_1, 25, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E2, NO_ODC,    ADC_1, 26, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E3, NO_ODC,    ADC_1, 27, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E4, NO_ODC,    ADC_1, 28, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E5, NO_ODC,    ADC_1, 29, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E6, NO_ODC,    ADC_1, 30, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  E7, NO_ODC,    ADC_1, 31, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  F0,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  F1,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  F2,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  F3,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  F4,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  F5,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  F6,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  F7,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  F8,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, F12,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, F13,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  G0,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  G1,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  G2,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  G3,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  G6,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  G7,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  G8,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio,  G9,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, G12,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, G13,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, G14,    ODC, ADC_NONE, -1, type, periphery, state);
  GPIO_INIT_MATCHING(gpio, G15,    ODC, ADC_NONE, -1, type, periphery, state);
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
  GPIO_GET_MATCHING(gpio, A5, state);
  GPIO_GET_MATCHING(gpio, A6, state);
  GPIO_GET_MATCHING(gpio, A7, state);
  GPIO_GET_MATCHING(gpio, A9, state);
  GPIO_GET_MATCHING(gpio, A10, state);
  GPIO_GET_MATCHING(gpio, A12, state);
  GPIO_GET_MATCHING(gpio, A13, state);
  GPIO_GET_MATCHING(gpio, A14, state);
  GPIO_GET_MATCHING(gpio, A15, state);
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
  GPIO_GET_MATCHING(gpio, B10, state);
  GPIO_GET_MATCHING(gpio, B11, state);
  GPIO_GET_MATCHING(gpio, B12, state);
  GPIO_GET_MATCHING(gpio, B13, state);
  GPIO_GET_MATCHING(gpio, B14, state);
  GPIO_GET_MATCHING(gpio, B15, state);
  GPIO_GET_MATCHING(gpio, C1, state);
  GPIO_GET_MATCHING(gpio, C2, state);
  GPIO_GET_MATCHING(gpio, C3, state);
  GPIO_GET_MATCHING(gpio, C4, state);
  GPIO_GET_MATCHING(gpio, C12, state);
  GPIO_GET_MATCHING(gpio, C13, state);
  GPIO_GET_MATCHING(gpio, C14, state);
  GPIO_GET_MATCHING(gpio, C15, state);
  GPIO_GET_MATCHING(gpio, D0, state);
  GPIO_GET_MATCHING(gpio, D1, state);
  GPIO_GET_MATCHING(gpio, D2, state);
  GPIO_GET_MATCHING(gpio, D3, state);
  GPIO_GET_MATCHING(gpio, D4, state);
  GPIO_GET_MATCHING(gpio, D5, state);
  GPIO_GET_MATCHING(gpio, D6, state);
  GPIO_GET_MATCHING(gpio, D7, state);
  GPIO_GET_MATCHING(gpio, D8, state);
  GPIO_GET_MATCHING(gpio, D9, state);
  GPIO_GET_MATCHING(gpio, D10, state);
  GPIO_GET_MATCHING(gpio, D11, state);
  GPIO_GET_MATCHING(gpio, D12, state);
  GPIO_GET_MATCHING(gpio, D13, state);
  GPIO_GET_MATCHING(gpio, D14, state);
  GPIO_GET_MATCHING(gpio, D15, state);
  GPIO_GET_MATCHING(gpio, E0, state);
  GPIO_GET_MATCHING(gpio, E1, state);
  GPIO_GET_MATCHING(gpio, E2, state);
  GPIO_GET_MATCHING(gpio, E3, state);
  GPIO_GET_MATCHING(gpio, E4, state);
  GPIO_GET_MATCHING(gpio, E5, state);
  GPIO_GET_MATCHING(gpio, E6, state);
  GPIO_GET_MATCHING(gpio, E7, state);
  GPIO_GET_MATCHING(gpio, F0, state);
  GPIO_GET_MATCHING(gpio, F1, state);
  GPIO_GET_MATCHING(gpio, F2, state);
  GPIO_GET_MATCHING(gpio, F3, state);
  GPIO_GET_MATCHING(gpio, F4, state);
  GPIO_GET_MATCHING(gpio, F5, state);
  GPIO_GET_MATCHING(gpio, F6, state);
  GPIO_GET_MATCHING(gpio, F7, state);
  GPIO_GET_MATCHING(gpio, F8, state);
  GPIO_GET_MATCHING(gpio, F12, state);
  GPIO_GET_MATCHING(gpio, F13, state);
  GPIO_GET_MATCHING(gpio, G0, state);
  GPIO_GET_MATCHING(gpio, G1, state);
  GPIO_GET_MATCHING(gpio, G2, state);
  GPIO_GET_MATCHING(gpio, G3, state);
  GPIO_GET_MATCHING(gpio, G6, state);
  GPIO_GET_MATCHING(gpio, G7, state);
  GPIO_GET_MATCHING(gpio, G8, state);
  GPIO_GET_MATCHING(gpio, G9, state);
  GPIO_GET_MATCHING(gpio, G12, state);
  GPIO_GET_MATCHING(gpio, G13, state);
  GPIO_GET_MATCHING(gpio, G14, state);
  GPIO_GET_MATCHING(gpio, G15, state);

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
  GPIO_SET_MATCHING(gpio, A5, state);
  GPIO_SET_MATCHING(gpio, A6, state);
  GPIO_SET_MATCHING(gpio, A7, state);
  GPIO_SET_MATCHING(gpio, A9, state);
  GPIO_SET_MATCHING(gpio, A10, state);
  GPIO_SET_MATCHING(gpio, A12, state);
  GPIO_SET_MATCHING(gpio, A13, state);
  GPIO_SET_MATCHING(gpio, A14, state);
  GPIO_SET_MATCHING(gpio, A15, state);
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
  GPIO_SET_MATCHING(gpio, B10, state);
  GPIO_SET_MATCHING(gpio, B11, state);
  GPIO_SET_MATCHING(gpio, B12, state);
  GPIO_SET_MATCHING(gpio, B13, state);
  GPIO_SET_MATCHING(gpio, B14, state);
  GPIO_SET_MATCHING(gpio, B15, state);
  GPIO_SET_MATCHING(gpio, C1, state);
  GPIO_SET_MATCHING(gpio, C2, state);
  GPIO_SET_MATCHING(gpio, C3, state);
  GPIO_SET_MATCHING(gpio, C4, state);
  GPIO_SET_MATCHING(gpio, C12, state);
  GPIO_SET_MATCHING(gpio, C13, state);
  GPIO_SET_MATCHING(gpio, C14, state);
  GPIO_SET_MATCHING(gpio, C15, state);
  GPIO_SET_MATCHING(gpio, D0, state);
  GPIO_SET_MATCHING(gpio, D1, state);
  GPIO_SET_MATCHING(gpio, D2, state);
  GPIO_SET_MATCHING(gpio, D3, state);
  GPIO_SET_MATCHING(gpio, D4, state);
  GPIO_SET_MATCHING(gpio, D5, state);
  GPIO_SET_MATCHING(gpio, D6, state);
  GPIO_SET_MATCHING(gpio, D7, state);
  GPIO_SET_MATCHING(gpio, D8, state);
  GPIO_SET_MATCHING(gpio, D9, state);
  GPIO_SET_MATCHING(gpio, D10, state);
  GPIO_SET_MATCHING(gpio, D11, state);
  GPIO_SET_MATCHING(gpio, D12, state);
  GPIO_SET_MATCHING(gpio, D13, state);
  GPIO_SET_MATCHING(gpio, D14, state);
  GPIO_SET_MATCHING(gpio, D15, state);
  GPIO_SET_MATCHING(gpio, E0, state);
  GPIO_SET_MATCHING(gpio, E1, state);
  GPIO_SET_MATCHING(gpio, E2, state);
  GPIO_SET_MATCHING(gpio, E3, state);
  GPIO_SET_MATCHING(gpio, E4, state);
  GPIO_SET_MATCHING(gpio, E5, state);
  GPIO_SET_MATCHING(gpio, E6, state);
  GPIO_SET_MATCHING(gpio, E7, state);
  GPIO_SET_MATCHING(gpio, F0, state);
  GPIO_SET_MATCHING(gpio, F1, state);
  GPIO_SET_MATCHING(gpio, F2, state);
  GPIO_SET_MATCHING(gpio, F3, state);
  GPIO_SET_MATCHING(gpio, F4, state);
  GPIO_SET_MATCHING(gpio, F5, state);
  GPIO_SET_MATCHING(gpio, F6, state);
  GPIO_SET_MATCHING(gpio, F7, state);
  GPIO_SET_MATCHING(gpio, F8, state);
  GPIO_SET_MATCHING(gpio, F12, state);
  GPIO_SET_MATCHING(gpio, F13, state);
  GPIO_SET_MATCHING(gpio, G0, state);
  GPIO_SET_MATCHING(gpio, G1, state);
  GPIO_SET_MATCHING(gpio, G2, state);
  GPIO_SET_MATCHING(gpio, G3, state);
  GPIO_SET_MATCHING(gpio, G6, state);
  GPIO_SET_MATCHING(gpio, G7, state);
  GPIO_SET_MATCHING(gpio, G8, state);
  GPIO_SET_MATCHING(gpio, G9, state);
  GPIO_SET_MATCHING(gpio, G12, state);
  GPIO_SET_MATCHING(gpio, G13, state);
  GPIO_SET_MATCHING(gpio, G14, state);
  GPIO_SET_MATCHING(gpio, G15, state);
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
