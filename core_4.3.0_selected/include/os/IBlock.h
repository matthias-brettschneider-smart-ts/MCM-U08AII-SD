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

#include "middleware/IErrorId.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

e_ERROR_ID BlockRangeIsAvailableForErase(const uint32_t address, const uint32_t number_of_bytes);
e_ERROR_ID BlockErase(uint32_t address, const uint32_t number_of_bytes);

e_ERROR_ID BlockRangeIsAvailableForWrite(const uint32_t address, const uint32_t number_of_bytes);
e_ERROR_ID BlockWrite(uint32_t address, uint32_t number_of_bytes, uint8_t data[]);

e_ERROR_ID BlockRangeIsAvailableForCheck(const uint32_t address, const uint32_t number_of_bytes);
e_ERROR_ID BlockCheck(uint32_t address, uint32_t number_of_bytes, uint16_t * const checksum);

e_ERROR_ID BlockCheckFlashloaderBlocks(void);
e_ERROR_ID BlockCheckApplicationBlocks(void);

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
