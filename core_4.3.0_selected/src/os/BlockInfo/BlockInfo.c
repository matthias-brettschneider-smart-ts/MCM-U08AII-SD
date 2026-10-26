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

/*============================================================================*/
/* I N C L U D E S                                                            */
/*============================================================================*/

#include "middleware/IConfig.h"
#include "middleware/IGeneric.h"
#include "os/IBlockInfo.h"

#ifndef BLOCK_INFO_NUMBER_OF_BLOCKS
#error "error compile flags - BLOCK_INFO_NUMBER_OF_BLOCKS undefined"
#elif ((BLOCK_INFO_NUMBER_OF_BLOCKS < 2) && (BLOCK_INFO_NUMBER_OF_BLOCKS > 3))
#error "error compile flags - BLOCK_INFO_NUMBER_OF_BLOCKS invalid"
#endif

#ifndef BLOCK_INFO_0_TYPE
#error "error compile flags - BLOCK_INFO_0_TYPE undefined"
#endif
#ifndef BLOCK_INFO_0_START_ADDRESS
#error "error compile flags - BLOCK_INFO_0_START_ADDRESS undefined"
#endif
#ifndef BLOCK_INFO_0_LENGTH
#error "error compile flags - BLOCK_INFO_0_LENGTH undefined"
#endif
#ifndef BLOCK_INFO_0_CHECKSUM_ADDRESS
#error "error compile flags - BLOCK_INFO_0_CHECKSUM_ADDRESS undefined"
#endif

#ifndef BLOCK_INFO_1_TYPE
#error "error compile flags - BLOCK_INFO_1_TYPE undefined"
#endif
#ifndef BLOCK_INFO_1_START_ADDRESS
#error "error compile flags - BLOCK_INFO_1_START_ADDRESS undefined"
#endif
#ifndef BLOCK_INFO_1_LENGTH
#error "error compile flags - BLOCK_INFO_1_LENGTH undefined"
#endif
#ifndef BLOCK_INFO_1_CHECKSUM_ADDRESS
#error "error compile flags - BLOCK_INFO_1_CHECKSUM_ADDRESS undefined"
#endif

#if (BLOCK_INFO_NUMBER_OF_BLOCKS == 3)

#ifndef BLOCK_INFO_2_TYPE
#error "error compile flags - BLOCK_INFO_2_TYPE undefined"
#endif
#ifndef BLOCK_INFO_2_START_ADDRESS
#error "error compile flags - BLOCK_INFO_2_START_ADDRESS undefined"
#endif
#ifndef BLOCK_INFO_2_LENGTH
#error "error compile flags - BLOCK_INFO_2_LENGTH undefined"
#endif
#ifndef BLOCK_INFO_2_CHECKSUM_ADDRESS
#error "error compile flags - BLOCK_INFO_2_CHECKSUM_ADDRESS undefined"
#endif

#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static const t_BLOCK_INFO block_info[] = {
    {
        .type = BLOCK_INFO_0_TYPE,
        .start_address = BLOCK_INFO_0_START_ADDRESS,
        .end_address = BLOCK_INFO_0_START_ADDRESS + BLOCK_INFO_0_LENGTH - 1,
        .checksum_address = BLOCK_INFO_0_CHECKSUM_ADDRESS
    },
    {
        .type = BLOCK_INFO_1_TYPE,
        .start_address = BLOCK_INFO_1_START_ADDRESS,
        .end_address = BLOCK_INFO_1_START_ADDRESS + BLOCK_INFO_1_LENGTH - 1,
        .checksum_address = BLOCK_INFO_1_CHECKSUM_ADDRESS
    },
#if (BLOCK_INFO_NUMBER_OF_BLOCKS == 3)
    {
        .type = BLOCK_INFO_2_TYPE,
        .start_address = BLOCK_INFO_2_START_ADDRESS,
        .end_address = BLOCK_INFO_2_START_ADDRESS + BLOCK_INFO_2_LENGTH - 1,
        .checksum_address = BLOCK_INFO_2_CHECKSUM_ADDRESS
    },
#endif
    {
        .type = BLOCK_INFO_TYPE_INVALID,
        .start_address = 0,
        .end_address = 0,
        .checksum_address = 0
    }
};

static const uint32_t block_info_value[BLOCK_INFO_NUMBER_OF_BLOCKS * 3 + 1] = {
    BLOCK_INFO_NUMBER_OF_BLOCKS,
    (uint32_t)BLOCK_INFO_0_TYPE, BLOCK_INFO_0_START_ADDRESS, BLOCK_INFO_0_START_ADDRESS + BLOCK_INFO_0_LENGTH - 1,
    (uint32_t)BLOCK_INFO_1_TYPE, BLOCK_INFO_1_START_ADDRESS, BLOCK_INFO_1_START_ADDRESS + BLOCK_INFO_1_LENGTH - 1,
#if (BLOCK_INFO_NUMBER_OF_BLOCKS == 3)
    (uint32_t)BLOCK_INFO_2_TYPE, BLOCK_INFO_2_START_ADDRESS, BLOCK_INFO_2_START_ADDRESS + BLOCK_INFO_2_LENGTH - 1,
#endif
};

static const t_CONFIG block_info_config[] = {
#if (BLOCK_INFO_NUMBER_OF_BLOCKS == 3)
    { .config_id = CONFIG_ID_BLOCK_INFO, .config_type = CONFIG_TYPE_UINT32,  .elements = 10, .readable = true,  .writeable = false, .data_pointer = (void*)block_info_value },
#else
    { .config_id = CONFIG_ID_BLOCK_INFO, .config_type = CONFIG_TYPE_UINT32,  .elements =  7, .readable = true,  .writeable = false, .data_pointer = (void*)block_info_value },
#endif
    { .config_id = CONFIG_ID_INVALID,    .config_type = CONFIG_TYPE_UNKNOWN, .elements =  0, .readable = false, .writeable = false, .data_pointer = NULL                    }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
void BlockInfoInit(void)
{
  ConfigRegister(block_info_config, NULL);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
const t_BLOCK_INFO * BlockInfoGet(void)
{
  return block_info;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
