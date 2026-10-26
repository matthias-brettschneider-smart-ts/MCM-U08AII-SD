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

#include "hal/IGpio.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

#define CURIOSITY_CK_LED_RED                    GPIO_PIN_E15
#define CURIOSITY_CK_LED_GREEN                  GPIO_PIN_E14
#define CURIOSITY_CK_LED_BLUE                   GPIO_PIN_E13
#define CURIOSITY_CK_LED_1                      GPIO_PIN_E6
#define CURIOSITY_CK_LED_2                      GPIO_PIN_E5

#define CURIOSITY_CK_MICRO_BUS_A_AN20           GPIO_PIN_E0
#define CURIOSITY_CK_MICRO_BUS_A_RST            GPIO_PIN_B9
#define CURIOSITY_CK_MICRO_BUS_A_CS             GPIO_PIN_D2
#define CURIOSITY_CK_MICRO_BUS_A_SCK            GPIO_PIN_D8
#define CURIOSITY_CK_MICRO_BUS_A_MISO           GPIO_PIN_D7
#define CURIOSITY_CK_MICRO_BUS_A_MOSI           GPIO_PIN_D6
#define CURIOSITY_CK_MICRO_BUS_A_PWM            GPIO_PIN_C15
#define CURIOSITY_CK_MICRO_BUS_A_INT            GPIO_PIN_E11
#define CURIOSITY_CK_MICRO_BUS_A_RX             GPIO_PIN_D15
#define CURIOSITY_CK_MICRO_BUS_A_TX             GPIO_PIN_D14
#define CURIOSITY_CK_MICRO_BUS_A_ASCL1          GPIO_PIN_C9
#define CURIOSITY_CK_MICRO_BUS_A_ASDA1          GPIO_PIN_C8

#define CURIOSITY_CK_MICRO_BUS_B_AN22           GPIO_PIN_E2
#define CURIOSITY_CK_MICRO_BUS_B_RST            GPIO_PIN_E4
#define CURIOSITY_CK_MICRO_BUS_B_CS             GPIO_PIN_E10
#define CURIOSITY_CK_MICRO_BUS_B_SCK            CURIOSITY_CK_MICRO_BUS_A_SCK
#define CURIOSITY_CK_MICRO_BUS_B_MISO           CURIOSITY_CK_MICRO_BUS_A_MISO
#define CURIOSITY_CK_MICRO_BUS_B_MOSI           CURIOSITY_CK_MICRO_BUS_A_MOSI
#define CURIOSITY_CK_MICRO_BUS_B_PWM            GPIO_PIN_C14
#define CURIOSITY_CK_MICRO_BUS_B_INT            GPIO_PIN_E12
#define CURIOSITY_CK_MICRO_BUS_B_RX             GPIO_PIN_D12
#define CURIOSITY_CK_MICRO_BUS_B_TX             GPIO_PIN_D9
#define CURIOSITY_CK_MICRO_BUS_B_ASCL1          CURIOSITY_CK_MICRO_BUS_A_ASCL1
#define CURIOSITY_CK_MICRO_BUS_B_ASDA1          CURIOSITY_CK_MICRO_BUS_A_ASDA1

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
