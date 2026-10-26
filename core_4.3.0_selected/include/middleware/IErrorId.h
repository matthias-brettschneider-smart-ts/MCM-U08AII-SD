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

  // --- used within firmware / on CAN -----------------------------------------

  ERROR_ID_SUCCESS                                                        = 0x00,
  ERROR_ID_VALUE_OUT_OF_RANGE                                             = 0x03,
  ERROR_ID_WRONG_KEY                                                      = 0x05,
  ERROR_ID_NO_SEED_REQUESTED                                              = 0x06,
  ERROR_ID_NOT_ALLOWED_IN_THIS_MODE                                       = 0x08,
  ERROR_ID_NOT_WRITEABLE                                                  = 0x0A,
  ERROR_ID_UNKNOWN_SUBLABEL                                               = 0x0C,
  ERROR_ID_ONLY_ALLOWED_ON_BASE_ID                                        = 0x0D,
  ERROR_ID_FLASH_INVALID_ADDRESS                                          = 0x16,
  ERROR_ID_FLASH_INVALID_LENGTH                                           = 0x17,
  ERROR_ID_FLASH_EXCEED_VALID_ADDRESS_RANGE                               = 0x18,
  ERROR_ID_FLASH_NO_ACTIVE_SESSION                                        = 0x19,
  ERROR_ID_FLASH_NOT_EMPTY                                                = 0x1A,
  ERROR_ID_FLASH_UNEXPECTED_INDEX                                         = 0x1B,
  ERROR_ID_FLASH_TOO_MUCH_DATA                                            = 0x1C,
  ERROR_ID_FLASH_NOT_ENOUGH_DATA                                          = 0x1D,
  ERROR_ID_LABEL_NOT_HANDLED                                              = 0x1E,
  ERROR_ID_WRONG_CHECKSUM                                                 = 0x1F,
  ERROR_ID_UNKNOWN_LABEL                                                  = 0x20,
  ERROR_ID_NOT_READABLE                                                   = 0x21,
  ERROR_ID_TEMPORARILY_UNAVAILABLE                                        = 0x22,
  ERROR_ID_NO_ACTIVE_FLASH_PAGE                                           = 0x23,
  ERROR_ID_FLASH_EXPIRED                                                  = 0x25,
  ERROR_ID_WRITE_ERROR                                                    = 0x26,
  ERROR_ID_READ_ERROR                                                     = 0x27,
  ERROR_ID_FULL                                                           = 0x28,
  ERROR_ID_FAILED                                                         = 0x29,
  ERROR_ID_UNKNOWN_VALUE                                                  = 0x2A,
  ERROR_ID_INTERNAL_ERROR                                                 = 0x2B,
  ERROR_ID_NOT_ALLOWED                                                    = 0x2C,
  ERROR_ID_WRONG_SIZE                                                     = 0x2D,
  ERROR_ID_VALUE_INCOMPATIBLE                                             = 0x2E,
  ERROR_ID_UNKNOWN_COMMAND                                                = 0x99,

  // --- used by error log -----------------------------------------------------

  // has to be the first within consecutively enumerated values
  ERROR_ID_ZERO                                                           = 0xA0,

  // first 15 numbers are reserved for fatal errors
  ERROR_ID_FATAL_INVALID_MODULE_NUMBER                                    = 0xA1,
  ERROR_ID_FATAL_RESERVED02                                               = 0xA2,
  ERROR_ID_FATAL_RESERVED03                                               = 0xA3,
  ERROR_ID_FATAL_RESERVED04                                               = 0xA4,
  ERROR_ID_FATAL_RESERVED05                                               = 0xA5,
  ERROR_ID_FATAL_RESERVED06                                               = 0xA6,
  ERROR_ID_FATAL_RESERVED07                                               = 0xA7,
  ERROR_ID_FATAL_CLOCK_IO                                                 = 0xA8,
  ERROR_ID_FATAL_STORAGE_MISCONFIGURED                                    = 0xA9,
  ERROR_ID_FATAL_FLASHLOADER_CORRUPT                                      = 0xAA,
  ERROR_ID_FATAL_STARTUP_TOO_LONG                                         = 0xAB,
  ERROR_ID_FATAL_I2C                                                      = 0xAC,
  ERROR_ID_FATAL_INVALID_DEVICE                                           = 0xAD,
  ERROR_ID_FATAL_ENDLESS_LOOP                                             = 0xAE,
  ERROR_ID_FATAL_ASSERT                                                   = 0xAF,

  // further numbers can be used for regular errors and warnings
  ERROR_ID_STORAGE_ACCESS_FAILED_CORE_IDENTIFICATION                      = 0xB0,
  ERROR_ID_STORAGE_ACCESS_FAILED_CORE_SETTINGS                            = 0xB1,
  ERROR_ID_STORAGE_ACCESS_FAILED_MODULE_SETTINGS                          = 0xB2,
  ERROR_ID_STORAGE_ACCESS_FAILED_MODULE_ADJUSTMENT_SMART                  = 0xB3,
  ERROR_ID_STORAGE_ACCESS_FAILED_MODULE_ADJUSTMENT_CUSTOMER               = 0xB4,
  ERROR_ID_EXECUTION_TIME_EXCEEDED                                        = 0xB5,
  ERROR_ID_CLOCK_SPIKES                                                   = 0xB6,
  ERROR_ID_CLOCK_MISSING                                                  = 0xB7,
  ERROR_ID_CLOCK_UNEXPECTED                                               = 0xB8,
  ERROR_ID_CAN_TX_QUEUE_OVERRUN                                           = 0xB9,
  ERROR_ID_CAN_RX_BUFFER_OVERRUN                                          = 0xBA,
  ERROR_ID_DEFAULT_REGISTRATION                                           = 0xBB,
  ERROR_ID_STORAGE_REGISTRATION                                           = 0xBC,
  ERROR_ID_CONFIG_REGISTRATION                                            = 0xBD,
  ERROR_ID_STORAGE_READ_VALUE                                             = 0xBE,
  ERROR_ID_STORAGE_WRITE_VALUE                                            = 0xBF,
  ERROR_ID_STARTUP_DETERMINATION                                          = 0xC0,
  ERROR_ID_ADJUSTMENT_VALUE_OUT_OF_RANGE                                  = 0xC1,
  ERROR_ID_UNKNOWN_DEVICE_REVISION                                        = 0xC2,
  ERROR_ID_PHYSICAL                                                       = 0xC3,
  ERROR_ID_UPTIME_CORRUPT                                                 = 0xC4,
  ERROR_ID_APPLICATION_WARNING                                            = 0xC5,
  ERROR_ID_APPLICATION_ERROR                                              = 0xC6,
  ERROR_ID_ERROR_HANDLING                                                 = 0xC7,
  ERROR_ID_NO_FLASHLOADER                                                 = 0xC8,

  // has to be the last within consecutively enumerated values
  ERROR_ID_LAST                                                           = 0xC9

} e_ERROR_ID;

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
