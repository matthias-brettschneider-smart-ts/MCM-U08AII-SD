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

enum e_STORAGE_ID {

  // used by flashloader & application
  STORAGE_ID_UNDEFINED                                                    =    0,
  STORAGE_ID_AUTO_OPERATION                                               = 0x10,
  STORAGE_ID_CLOCK_MASTER_STORED                                          = 0x11,
  STORAGE_ID_MODULE_NUMBER_STORED                                         = 0x12,
  STORAGE_ID_STARTUP_DETERMINATION                                        = 0x14,
  STORAGE_ID_HARDWARE_TYPE                                                = 0x20,
  STORAGE_ID_HARDWARE_VERSION                                             = 0x21,
  STORAGE_ID_HARDWARE_VARIANT                                             = 0x22,
  STORAGE_ID_SERIAL_NUMBER                                                = 0x23,
  STORAGE_ID_PRODUCTION_DATE                                              = 0x24,
  STORAGE_ID_IDENTIFICATION_SMART                                         = 0x25,
  STORAGE_ID_IDENTIFICATION_CUSTOMER                                      = 0x26,
  STORAGE_ID_CAN_BIT_RATE_PRIMARY_NOMINAL                                 = 0x30,
  STORAGE_ID_CAN_BIT_RATE_SECONDARY_NOMINAL                               = 0x31,
  STORAGE_ID_CAN_ID_REQUEST_BROADCAST                                     = 0x32,
  STORAGE_ID_CAN_ID_BLOCK_OFFSET                                          = 0x33,
  STORAGE_ID_CAN_INIT_DETECTION                                           = 0x34,
  STORAGE_ID_CAN_BIT_RATE_PRIMARY_DATA                                    = 0x35,
  STORAGE_ID_CAN_BIT_RATE_SECONDARY_DATA                                  = 0x36,
  STORAGE_ID_ERROR_VERBOSE_LEVEL                                          = 0x40,
  STORAGE_ID_UPTIME_ONE                                                   = 0x60,
  STORAGE_ID_UPTIME_TWO                                                   = 0x61
};

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
