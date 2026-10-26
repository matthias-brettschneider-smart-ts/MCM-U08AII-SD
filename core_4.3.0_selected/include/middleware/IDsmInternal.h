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

#include "middleware/IGeneric.h"
#include "middleware/IErrorId.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

typedef enum {
  DSM_DIRECTION_NONE = 0,
  DSM_DIRECTION_TRANSMIT = 1,
  DSM_DIRECTION_RECEIVE = 2,
  DSM_DIRECTION_TRANSCEIVE = 3
} e_DSM_DIRECTION;

typedef enum {
  DSM_FREQUENCY_1_HZ = 0,
  DSM_FREQUENCY_2_HZ = 1,
  DSM_FREQUENCY_5_HZ = 2,
  DSM_FREQUENCY_10_HZ = 3,
  DSM_FREQUENCY_20_HZ = 4,
  DSM_FREQUENCY_50_HZ = 5,
  DSM_FREQUENCY_100_HZ = 6,
  DSM_FREQUENCY_200_HZ = 7,
  DSM_FREQUENCY_500_HZ = 8,
  DSM_FREQUENCY_1000_HZ = 9,
  DSM_FREQUENCY_2000_HZ = 10,
  DSM_FREQUENCY_5000_HZ = 11,
  DSM_FREQUENCY_10000_HZ = 12,
  DSM_FREQUENCY_20000_HZ = 13,
  DSM_FREQUENCY_50000_HZ = 14,
  DSM_FREQUENCY_INVALID = 15
} e_DSM_FREQUENCY;

typedef struct {
  uint32_t block : 8;
  uint32_t offset : 16;
  uint32_t frequency : 4;
  uint32_t lane : 2;
  uint32_t direction : 2;
} t_DSM_SCHEDULER_ENTRY;

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

void DsmReset(void);

e_ERROR_ID DsmSchedule(const t_DSM_SCHEDULER_ENTRY * const scheduling);

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
