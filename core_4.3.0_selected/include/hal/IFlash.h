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

typedef enum {
  FLASH_DISABLE_INTERRUPTS_NEVER,
  FLASH_DISABLE_INTERRUPTS_DURING_ERASE_AND_WRITE_ONLY,
  FLASH_DISABLE_INTERRUPTS_PERMANENTLY
} e_FLASH_DISABLE_INTERRUPTS;

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

uint32_t FlashGetRowSize(void);
uint32_t FlashGetPageSize(void);

e_ERROR_ID FlashAddressesOneOrMultipleRows(const uint32_t address, const uint32_t number_of_bytes);
e_ERROR_ID FlashAddressesOneOrMultiplePages(const uint32_t address, const uint32_t number_of_bytes);

e_ERROR_ID FlashAquire(const e_FLASH_DISABLE_INTERRUPTS action);
void FlashRelease(void);

uint32_t FlashRead32Bit(const uint32_t address);
e_ERROR_ID FlashWrite32Bit(const uint32_t address, const uint32_t value);
e_ERROR_ID FlashWrite64Bit(const uint32_t address, const uint32_t value[]);

e_ERROR_ID FlashRead(const uint32_t address, const uint16_t number_of_bytes, uint8_t * const data);

e_ERROR_ID FlashBlankCheckPage(const uint32_t address);
e_ERROR_ID FlashErasePage(const uint32_t address);
e_ERROR_ID FlashWriteRow(const uint32_t address, const uint8_t data[]);

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
