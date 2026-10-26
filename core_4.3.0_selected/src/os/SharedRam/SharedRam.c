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

#include "hal/ICompiler.h"
#include "middleware/IAssert.h"
#include "middleware/IConfig.h"
#include "middleware/ICrc.h"
#include "middleware/IGeneric.h"
#include "middleware/IMemory.h"
#include "os/ISharedRam.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef struct {
  uint16_t version;
  uint16_t flags;
  uint16_t boot_counter;
  uint32_t uptime_in_seconds;                // up to 136 years
  uint32_t uptime_last_stored_in_seconds;
  uint8_t  reserved[16];
  uint16_t crc;
} COMPILER_PACKED t_SHARED_RAM_BLOCK;

#define SHARED_RAM_SIZE_IN_BYTE                                               32
#define SHARED_RAM_LAYOUT_VERSION                                              1

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

COMPILER_SHARED_RAM static volatile t_SHARED_RAM_BLOCK shared_ram_block;

static const t_CONFIG shared_ram_config[] = {
    { .config_id = CONFIG_ID_BOOT_COUNTER,        .config_type = CONFIG_TYPE_UINT16,  .elements = 1, .readable = true,  .writeable = false, .data_pointer = NULL },
    { .config_id = CONFIG_ID_STAY_IN_FLASHLOADER, .config_type = CONFIG_TYPE_BOOL,    .elements = 1, .readable = true,  .writeable = true,  .data_pointer = NULL },
    { .config_id = CONFIG_ID_INVALID,             .config_type = CONFIG_TYPE_UNKNOWN, .elements = 0, .readable = false, .writeable = false, .data_pointer = NULL }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static CONFIG_CALLBACK(SharedRamConfigCallback);

static void SharedRamEnsureValidness(void);
static void SharedRamUpdateCrc(void);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
void SharedRamInit(void)
{
  ASSERT_STATIC(sizeof(shared_ram_block) == SHARED_RAM_SIZE_IN_BYTE);

  SharedRamEnsureValidness();

  ConfigRegister(shared_ram_config, SharedRamConfigCallback);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Callback routine used for factory reset.
 *
 -----------------------------------------------------------------------------*/
static CONFIG_CALLBACK(SharedRamConfigCallback)
{
  e_ERROR_ID result = ERROR_ID_SUCCESS;

  switch (config_id) {

    case CONFIG_ID_STAY_IN_FLASHLOADER:

      if ((callback_type == CONFIG_CALLBACK_TYPE_PRE) && read) {
        bool * const stay_in_flashloader = (bool *)data;
        *stay_in_flashloader = SharedRamFlagGet(SHARED_RAM_FLAG_STAY_IN_FLASHLOADER);
      }

      if ((callback_type == CONFIG_CALLBACK_TYPE_POST) && !read) {
        const bool stay_in_flashloader = *(bool *)data;
        SharedRamFlagSet(SHARED_RAM_FLAG_STAY_IN_FLASHLOADER, stay_in_flashloader);
      }

      break;

    case CONFIG_ID_BOOT_COUNTER:

      if ((callback_type == CONFIG_CALLBACK_TYPE_PRE) && read) {
        uint16_t * const boot_counter = (uint16_t *)data;
        *boot_counter = (uint16_t)SharedRamVariableGet(SHARED_RAM_VARIABLE_BOOT_COUNTER);
      }

      break;

    default:
      break;
  }

  return result;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
static void SharedRamEnsureValidness(void)
{
  const bool consistent = CrcGetCrc16((uint8_t *)&shared_ram_block, SHARED_RAM_SIZE_IN_BYTE) != 0;
  if (consistent) {
    shared_ram_block.version = SHARED_RAM_LAYOUT_VERSION;
    shared_ram_block.flags = 0;
    shared_ram_block.boot_counter = 0;
    shared_ram_block.uptime_in_seconds = 0;
    shared_ram_block.uptime_last_stored_in_seconds = 0;

    SharedRamUpdateCrc();
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
static void SharedRamUpdateCrc(void)
{
  const uint16_t crc = CrcGetCrc16((uint8_t *)&shared_ram_block, SHARED_RAM_SIZE_IN_BYTE - 2);
  shared_ram_block.crc = MEMORY_SWAP_UINT16(crc);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool SharedRamFlagGet(const e_SHARED_RAM_FLAG flag)
{
  SharedRamEnsureValidness();

  return (shared_ram_block.flags & (1u << flag)) != 0;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void SharedRamFlagSet(const e_SHARED_RAM_FLAG flag, const bool value)
{
  SharedRamEnsureValidness();

  if (value)
    shared_ram_block.flags |= (1u << flag);
  else
    shared_ram_block.flags &= ~(1u << flag);

  SharedRamUpdateCrc();
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
uint32_t SharedRamVariableGet(const e_SHARED_RAM_VARIABLE variable)
{
  SharedRamEnsureValidness();

  return (variable == SHARED_RAM_VARIABLE_BOOT_COUNTER) ? (uint32_t)shared_ram_block.boot_counter :
      (variable == SHARED_RAM_VARIABLE_UPTIME) ? shared_ram_block.uptime_in_seconds :
          (variable == SHARED_RAM_VARIABLE_UPTIME_LAST) ? shared_ram_block.uptime_last_stored_in_seconds : 0;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void SharedRamVariableSet(const e_SHARED_RAM_VARIABLE variable, const uint32_t value)
{
  SharedRamEnsureValidness();

  if (variable == SHARED_RAM_VARIABLE_BOOT_COUNTER)
    shared_ram_block.boot_counter = (uint16_t)value;
  else if (variable == SHARED_RAM_VARIABLE_UPTIME)
    shared_ram_block.uptime_in_seconds = value;
  else if (variable == SHARED_RAM_VARIABLE_UPTIME_LAST)
    shared_ram_block.uptime_last_stored_in_seconds = value;

  SharedRamUpdateCrc();
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void SharedRamVariableInc(const e_SHARED_RAM_VARIABLE variable)
{
  SharedRamEnsureValidness();

  if ((variable == SHARED_RAM_VARIABLE_BOOT_COUNTER) && (shared_ram_block.boot_counter < UINT16_MAX))
    shared_ram_block.boot_counter++;
  else if ((variable == SHARED_RAM_VARIABLE_UPTIME) && (shared_ram_block.uptime_in_seconds < UINT32_MAX))
    shared_ram_block.uptime_in_seconds++;
  else if ((variable == SHARED_RAM_VARIABLE_UPTIME_LAST) && (shared_ram_block.uptime_last_stored_in_seconds < UINT32_MAX))
    shared_ram_block.uptime_last_stored_in_seconds++;

  SharedRamUpdateCrc();
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
