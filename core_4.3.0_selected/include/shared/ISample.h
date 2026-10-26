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
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

#define SAMPLE_ID_CABLE_BREAK_16                                (INT16_MAX -  0)
#define SAMPLE_ID_CHANNEL_DISABLED_16                           (INT16_MAX -  1)
#define SAMPLE_ID_RESERVED_FOR_HEAD_CONTROLLER_16               (INT16_MAX -  2)
#define SAMPLE_ID_INVALID_SETTINGS_16                           (INT16_MAX -  3)
#define SAMPLE_ID_NOT_AVAILABLE_16                              (INT16_MAX -  4)
#define SAMPLE_ID_UNDER_VOLTAGE_16                              (INT16_MAX -  5)
#define SAMPLE_ID_RESERVED_1_16                                 (INT16_MAX -  6)
#define SAMPLE_ID_RESERVED_2_16                                 (INT16_MAX -  7)
#define SAMPLE_ID_RESERVED_3_16                                 (INT16_MAX -  8)
#define SAMPLE_ID_RESERVED_4_16                                 (INT16_MAX -  9)
#define SAMPLE_ID_OUT_OF_RANGE_POSITIVE_16                      (INT16_MAX - 10)
// values in between are used for valid data
#define SAMPLE_ID_OUT_OF_RANGE_NEGATIVE_16                             INT16_MIN

#define SAMPLE_ID_CABLE_BREAK_32                                (INT32_MAX -  0)
#define SAMPLE_ID_CHANNEL_DISABLED_32                           (INT32_MAX -  1)
#define SAMPLE_ID_RESERVED_FOR_HEAD_CONTROLLER_32               (INT32_MAX -  2)
#define SAMPLE_ID_INVALID_SETTINGS_32                           (INT32_MAX -  3)
#define SAMPLE_ID_NOT_AVAILABLE_32                              (INT32_MAX -  4)
#define SAMPLE_ID_UNDER_VOLTAGE_32                              (INT32_MAX -  5)
#define SAMPLE_ID_RESERVED_1_32                                 (INT32_MAX -  6)
#define SAMPLE_ID_RESERVED_2_32                                 (INT32_MAX -  7)
#define SAMPLE_ID_RESERVED_3_32                                 (INT32_MAX -  8)
#define SAMPLE_ID_RESERVED_4_32                                 (INT32_MAX -  9)
#define SAMPLE_ID_OUT_OF_RANGE_POSITIVE_32                      (INT32_MAX - 10)
// values in between are used for valid data
#define SAMPLE_ID_OUT_OF_RANGE_NEGATIVE_32                             INT32_MIN

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

COMPILER_INLINE int16_t SAMPLE_CONVERT_TO_16_BIT(const int32_t value);

/*============================================================================*/
/*  I M P L E M E N T A T I O N                                               */
/*============================================================================*/

#include "SampleImpl.h"

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
