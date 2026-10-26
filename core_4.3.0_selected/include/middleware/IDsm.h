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

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

typedef bool (t_DSM_CALLBACK)(const uint8_t dsm_cycle, const uint8_t block);
#define DSM_CALLBACK(name) bool (name)(const uint8_t dsm_cycle, const uint8_t block)

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

void DsmInit(t_DSM_CALLBACK * const callback);

uint16_t DsmGetPeriodInCycles(void);
uint8_t DsmGetNumberOfBlocks(void);
size_t DsmGetSizeOfPayload(void);

void DsmPrepareBlockForTransmission(const uint8_t block, const uint8_t payload[]);
bool DsmCheckReceiveBlockAndGetData(const uint8_t block, uint8_t payload[]);

void DsmPrepareBus(const uint16_t cycle_number);

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
