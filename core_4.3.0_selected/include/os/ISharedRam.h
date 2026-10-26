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

typedef enum {
  SHARED_RAM_FLAG_APPLICATION_VALID,
  SHARED_RAM_FLAG_PREVIOUS_FLASHLOADER,
  SHARED_RAM_FLAG_STAY_IN_FLASHLOADER,
  SHARED_RAM_FLAG_UDS_PROGRAMMING_SESSION_ACTIVE
} e_SHARED_RAM_FLAG;

typedef enum {
  SHARED_RAM_VARIABLE_BOOT_COUNTER,
  SHARED_RAM_VARIABLE_UPTIME,
  SHARED_RAM_VARIABLE_UPTIME_LAST
} e_SHARED_RAM_VARIABLE;

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

void SharedRamInit(void);

bool SharedRamFlagGet(const e_SHARED_RAM_FLAG flag);
void SharedRamFlagSet(const e_SHARED_RAM_FLAG flag, const bool value);

uint32_t SharedRamVariableGet(const e_SHARED_RAM_VARIABLE variable);
void SharedRamVariableSet(const e_SHARED_RAM_VARIABLE variable, const uint32_t value);
void SharedRamVariableInc(const e_SHARED_RAM_VARIABLE variable);

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
