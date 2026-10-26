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

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

typedef enum {

  // 0xF100..0xF17F - vehicle manufacturer specific identification options
  IDENTIFIER_ID_FINGERPRINT_VFLASH                                      = 0xF15A,

  // 0xFD00..0xFEFF - system supplier specific
  IDENTIFIER_ID_ADD_ERROR_ENTRY                                         = 0xFE00,
  IDENTIFIER_ID_ADJUSTMENT_COUNT_SMART                                  = 0xFE01,
  IDENTIFIER_ID_ADJUSTMENT_DATA_SMART_FACTOR_RANGE_0                    = 0xFE02,
  IDENTIFIER_ID_ADJUSTMENT_DATA_SMART_GROUND_SHIFT_RANGE_0              = 0xFE03,
  IDENTIFIER_ID_ADJUSTMENT_DATA_SMART_OFFSET_RANGE_0                    = 0xFE04,
  IDENTIFIER_ID_ADJUSTMENT_DATE_SMART                                   = 0xFE05,
  IDENTIFIER_ID_AUTO_OPERATION                                          = 0xFE06,
  IDENTIFIER_ID_BLOCK_INFO                                              = 0xFE07,
  IDENTIFIER_ID_BOOT_COUNTER                                            = 0xFE08,
  IDENTIFIER_ID_CABLE_BREAK_DETECTION_MODE                              = 0xFE09,
  IDENTIFIER_ID_CABLE_BREAK_DETECTION_CORE_HOLD_TIME                    = 0xFE0A,
  IDENTIFIER_ID_CABLE_BREAK_DETECTION_CORE_REACTION_TIME                = 0xFE0B,
  IDENTIFIER_ID_CABLE_BREAK_DETECTION_CORE_SENSITIVITY                  = 0xFE0C,
  IDENTIFIER_ID_CAN_BIT_RATE_PRIMARY                                    = 0xFE0D,
  IDENTIFIER_ID_CAN_BIT_RATE_SECONDARY                                  = 0xFE0E,
  IDENTIFIER_ID_CAN_ID_BLOCK_OFFSET                                     = 0xFE0F,
  IDENTIFIER_ID_CAN_ID_REQUEST_BROADCAST                                = 0xFE10,
  IDENTIFIER_ID_CAN_TRANSMISSION_DATA_RATE                              = 0xFE11,
  IDENTIFIER_ID_CAN_TRANSMISSION_LAYOUT                                 = 0xFE12,
  IDENTIFIER_ID_CAN_TRANSMISSION_SAMPLES                                = 0xFE13,
  IDENTIFIER_ID_CHANNEL_RANGE                                           = 0xFE14,
  IDENTIFIER_ID_CHANNEL_RESOLUTION                                      = 0xFE15,
  IDENTIFIER_ID_CHANNEL_STATE                                           = 0xFE16,
  IDENTIFIER_ID_CLOCK_MASTER_ACTIVE                                     = 0xFE17,
  IDENTIFIER_ID_CLOCK_MASTER_DETERMINED                                 = 0xFE18,
  IDENTIFIER_ID_CLOCK_MASTER_STORED                                     = 0xFE19,
  IDENTIFIER_ID_CORE_VERSION                                            = 0xFE1A,
  IDENTIFIER_ID_CURRENT_STACK_POINTER                                   = 0xFE1B,
  IDENTIFIER_ID_DEVICE_ID                                               = 0xFE1C,
  IDENTIFIER_ID_DEVICE_REVISION                                         = 0xFE1D,
  IDENTIFIER_ID_ERROR_ENTRY                                             = 0xFE1E,
  IDENTIFIER_ID_ERROR_VERBOSE_LEVEL                                     = 0xFE1F,
  IDENTIFIER_ID_FILTER_PARAMETER                                        = 0xFE20,
  IDENTIFIER_ID_FILTER_TYPE                                             = 0xFE21,
  IDENTIFIER_ID_FIRMWARE_TYPE                                           = 0xFE22,
  IDENTIFIER_ID_FIRMWARE_VARIANT                                        = 0xFE23,
  IDENTIFIER_ID_FIRMWARE_VERSION                                        = 0xFE24,
  IDENTIFIER_ID_GROUND_SHIFT_AVERAGE                                    = 0xFE25,
  IDENTIFIER_ID_HARDWARE_TYPE                                           = 0xFE26,
  IDENTIFIER_ID_HARDWARE_VARIANT                                        = 0xFE27,
  IDENTIFIER_ID_HARDWARE_VERSION                                        = 0xFE28,
  IDENTIFIER_ID_IDENTIFICATION_CUSTOMER                                 = 0xFE29,
  IDENTIFIER_ID_IDENTIFICATION_SMART                                    = 0xFE2A,
  IDENTIFIER_ID_INIT_STACK_TEST                                         = 0xFE2B,
  IDENTIFIER_ID_LIFETIME                                                = 0xFE2C,
  IDENTIFIER_ID_MODULE_NUMBER_ACTIVE                                    = 0xFE2D,
  IDENTIFIER_ID_MODULE_NUMBER_DETERMINED                                = 0xFE2E,
  IDENTIFIER_ID_MODULE_NUMBER_STORED                                    = 0xFE2F,
  IDENTIFIER_ID_MODULE_TEMPERATURE                                      = 0xFE30,
  IDENTIFIER_ID_POWER_SUPPLY                                            = 0xFE31,
  IDENTIFIER_ID_PRODUCTION_DATE                                         = 0xFE32,
  IDENTIFIER_ID_PUBLIC_KEY_OPERATOR                                     = 0xFE33,
  IDENTIFIER_ID_PUBLIC_KEY_MAINTENANCE                                  = 0xFE34,
  IDENTIFIER_ID_PUBLIC_KEY_MANUFACTURER                                 = 0xFE35,
  IDENTIFIER_ID_SERIAL_NUMBER                                           = 0xFE36,
  IDENTIFIER_ID_SET_MODULE_NUMBER_AND_CLOCK_MASTER                      = 0xFE37,
  IDENTIFIER_ID_STACK_LIMIT                                             = 0xFE38,
  IDENTIFIER_ID_STACK_TEST_RESULT                                       = 0xFE39,
  IDENTIFIER_ID_STARTUP_DETERMINATION                                   = 0xFE3A,
  IDENTIFIER_ID_STATIC_CHANNEL                                          = 0xFE3B,
  IDENTIFIER_ID_STORAGE_INFO                                            = 0xFE3C,
  IDENTIFIER_ID_UART_TRANSMISSION                                       = 0xFE3D,
  IDENTIFIER_ID_UNLOCK_MASTER_CLOCK                                     = 0xFE3E,
  IDENTIFIER_ID_UPTIME                                                  = 0xFE3F,
  IDENTIFIER_ID_STAY_IN_FLASHLOADER                                     = 0xFE40,
  IDENTIFIER_ID_CLOCK_INDIVIDUAL                                        = 0xFE41,
  IDENTIFIER_ID_CABLE_BREAK_DETECTION_ALGORITHM                         = 0xFE42,
  IDENTIFIER_ID_CABLE_BREAK_DETECTION_CLASSIC_HOLD_TIME                 = 0xFE43,
  IDENTIFIER_ID_CABLE_BREAK_DETECTION_CLASSIC_REACTION_TIME             = 0xFE44,
  IDENTIFIER_ID_CABLE_BREAK_DETECTION_CLASSIC_SENSITIVITY               = 0xFE45,
  IDENTIFIER_ID_PROGRAMMING_SESSION_APPLICATION                         = 0xFE46,
  IDENTIFIER_ID_FRONTEND_SELECTION                                      = 0xFE47,
  IDENTIFIER_ID_NUMBER_OF_CHANNELS                                      = 0xFE48,
  IDENTIFIER_ID_UNDERVOLTAGE_LIMIT_LOWER                                = 0xFE49,
  IDENTIFIER_ID_UNDERVOLTAGE_LIMIT_UPPER                                = 0xFE4A,
  IDENTIFIER_ID_BMS_FLAGS                                               = 0xFE4B,
  IDENTIFIER_ID_BMS_DIE_TEMPERATURE                                     = 0xFE4C,
  IDENTIFIER_ID_BMS_SUPPLY_ANALOG                                       = 0xFE4D,
  IDENTIFIER_ID_BMS_SUPPLY_DIGITAL                                      = 0xFE4E,
  IDENTIFIER_ID_BMS_SUPPLY_HIGH_VOLTAGE_PLUS                            = 0xFE4F,
  IDENTIFIER_ID_BMS_SUPPLY_SECOND_REFERENCE                             = 0xFE50,
  IDENTIFIER_ID_BMS_SUPPLY_VOLTAGE_PLUS                                 = 0xFE51,
  IDENTIFIER_ID_CAN_BIT_RATE_PRIMARY_NOMINAL_STORED                     = 0xFE52,
  IDENTIFIER_ID_CAN_BIT_RATE_PRIMARY_DATA_STORED                        = 0xFE53,
  IDENTIFIER_ID_CAN_BIT_RATE_SECONDARY_NOMINAL_STORED                   = 0xFE54,
  IDENTIFIER_ID_CAN_BIT_RATE_SECONDARY_DATA_STORED                      = 0xFE55,
  IDENTIFIER_ID_CAN_ID_REQUEST_BROADCAST_STORED                         = 0xFE56,
  IDENTIFIER_ID_CAN_ID_BLOCK_OFFSET_STORED                              = 0xFE57,

  // last
  IDENTIFIER_ID_INVALID                                                 = 0xFFFF
} e_IDENTIFIER_ID;

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
