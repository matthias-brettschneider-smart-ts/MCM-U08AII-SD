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

#include "middleware/IAssert.h"
#include "middleware/IConfig.h"
#include "middleware/IGeneric.h"
#include "middleware/IStorage.h"
#include "middleware/IStorageInternal.h"
#include "middleware/IWait.h"
#include "os/ISharedRam.h"
#include "os/IUptime.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

// Uptime and lifetime are counted in seconds and stored as uint32_t:
//
// => overrun every ~ 136 years
//
// Lifetime is stored in memory with limited erase/write cycles:
//
// - Physical EEPROM
//
//   1.000.000 erase per eeprom page
//   1 page erase/write / counter update
//   2 eeprom pages in use
//
//   => 2.000.000 possible updates
//   => 22,8 years when updating each 6 minutes
//
// - EEPROM emulation
//
//   2 flash pages / eeprom page
//   2 eeprom pages in use
//   => 36.000 flash erase possible
//
//   1022 data byte / flash page ((512 instructions / flash page - 1 instruction for page status) * 2 byte / instruction)
//   6 data byte / counter update (4 byte value + 2 byte crc)
//
//   byte / eeprom page                 |       64        |      128         |       256
//   -----------------------------------+-----------------+------------------+-----------------
//   byte for initial eeprom page       |       64        |      128         |       256
//   bytes for updates / flash page     | 1022 - 64 = 958 | 1022 - 128 = 894 | 1022 - 256 = 766
//   counter updates / flash erase      |  958 / 6 = 159  |  894 / 6 = 149   |  766 / 6 = 127
//   possible updates                   |    5.724.000    |    5.364.000     |    4.572.000
//   years when updating each 6 minutes |      65,3       |       61,0       |       52,2
//
//   (Duration is divides in half if the device is restarted each time after its first lifetime update.)

#define UPTIME_NUMBER_OF_STORAGE_BLOCKS                                        2

#define UPTIME_UPDATE_INTERVAL_FIRST_IN_SECONDS                              180
#define UPTIME_UPDATE_INTERVAL_REGULAR_IN_SECONDS                            360

typedef struct {
  bool warm_start;
  uint32_t ticks_last;
  int older_data_set;
  uint32_t lifetime_in_seconds;
  uint32_t lifetime_stored_in_seconds[UPTIME_NUMBER_OF_STORAGE_BLOCKS];
} t_UPTIME;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_UPTIME uptime;

static const t_STORAGE_ENTRY uptime_storage_first[] = {
    { .storage_id = STORAGE_ID_UPTIME_ONE, .storage_block = STORAGE_BLOCK_CORE_UPTIME_FIRST,  .data_pointer = (void*)&uptime.lifetime_stored_in_seconds[0], .size = sizeof(uptime.lifetime_stored_in_seconds[0]) },
    { .storage_id = STORAGE_ID_UNDEFINED,  .storage_block = STORAGE_BLOCK_UNDEFINED,          .data_pointer = NULL,                                         .size = 0                                            }
};

static const t_STORAGE_ENTRY uptime_storage_second[] = {
    { .storage_id = STORAGE_ID_UPTIME_TWO, .storage_block = STORAGE_BLOCK_CORE_UPTIME_SECOND, .data_pointer = (void*)&uptime.lifetime_stored_in_seconds[1], .size = sizeof(uptime.lifetime_stored_in_seconds[1]) },
    { .storage_id = STORAGE_ID_UNDEFINED,  .storage_block = STORAGE_BLOCK_UNDEFINED,          .data_pointer = NULL,                                         .size = 0                                            }
};

static const t_CONFIG uptime_config[] = {
    { .config_id = CONFIG_ID_UPTIME,   .config_type = CONFIG_TYPE_UINT32,  .elements = 1, .readable = true,  .writeable = false, .data_pointer = NULL },
    { .config_id = CONFIG_ID_LIFETIME, .config_type = CONFIG_TYPE_UINT32,  .elements = 1, .readable = true,  .writeable = true,  .data_pointer = NULL },
    { .config_id = CONFIG_ID_INVALID,  .config_type = CONFIG_TYPE_UNKNOWN, .elements = 0, .readable = false, .writeable = false, .data_pointer = NULL }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static CONFIG_CALLBACK(UptimeConfigCallback);

static uint32_t UptimeGetLifetime(void);
static bool UptimeSetLifetime(const uint32_t lifetime_in_minutes);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/


/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
void UptimeInit(void)
{
  StorageRegister(uptime_storage_first);
  if (StorageLoad(uptime_storage_first) != ERROR_ID_SUCCESS)
    StorageStoreBlock(STORAGE_BLOCK_CORE_UPTIME_FIRST);

  StorageRegister(uptime_storage_second);
  if (StorageLoad(uptime_storage_second) != ERROR_ID_SUCCESS)
    StorageStoreBlock(STORAGE_BLOCK_CORE_UPTIME_SECOND);

  ConfigRegister(uptime_config, UptimeConfigCallback);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Callback routine used for configuration accesses not covered by simple read/write via pointers.
 *
 -----------------------------------------------------------------------------*/
static CONFIG_CALLBACK(UptimeConfigCallback)
{
  e_ERROR_ID result = ERROR_ID_SUCCESS;

  switch (config_id) {

    case CONFIG_ID_UPTIME:

      if (callback_type == CONFIG_CALLBACK_TYPE_PRE)
        *(uint32_t *)data = SharedRamVariableGet(SHARED_RAM_VARIABLE_UPTIME);

      break;

    case CONFIG_ID_LIFETIME:

      if (callback_type == CONFIG_CALLBACK_TYPE_PRE) {
        if (read)
          *(uint32_t *)data = UptimeGetLifetime();
        else {
          const bool done = UptimeSetLifetime(*(uint32_t *)data);
          if (!done)
            result = ERROR_ID_VALUE_OUT_OF_RANGE;
        }
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
 * Get the current value of the overall runtime counter in minutes.
 *
 -----------------------------------------------------------------------------*/
static uint32_t UptimeGetLifetime(void)
{
  return uptime.lifetime_in_seconds / 60;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Set the overall runtime counter to a specific value in minutes.
 *
 -----------------------------------------------------------------------------*/
static bool UptimeSetLifetime(const uint32_t lifetime_in_minutes)
{
  const bool valid = lifetime_in_minutes <= (UINT32_MAX / 60);
  if (valid) {

    uptime.lifetime_in_seconds = lifetime_in_minutes * 60;

    uptime.lifetime_stored_in_seconds[0] = uptime.lifetime_in_seconds;
    StorageStoreBlock(STORAGE_BLOCK_CORE_UPTIME_FIRST);
    uptime.lifetime_stored_in_seconds[1] = uptime.lifetime_in_seconds;
    StorageStoreBlock(STORAGE_BLOCK_CORE_UPTIME_SECOND);

    SharedRamVariableSet(SHARED_RAM_VARIABLE_UPTIME_LAST, SharedRamVariableGet(SHARED_RAM_VARIABLE_UPTIME));
  }

  return valid;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Initialize or update counters in shared RAM and storage if necessary.
 *
 -----------------------------------------------------------------------------*/
void UptimeUpdate(void)
{
  if (!uptime.warm_start) {
    uptime.ticks_last = WAIT_GET_TIMER_VALUE_32();

    // determine older data set and initialize active data set with newer values
    // - if one set has been invalid during load, it kept its default values and thereby automatically becomes the older data set
    // - if both sets have been lost, there is no data to fall back onto
    uptime.older_data_set = (uptime.lifetime_stored_in_seconds[0] < uptime.lifetime_stored_in_seconds[1]) ? 0 : 1;
    uptime.lifetime_in_seconds = uptime.lifetime_stored_in_seconds[(uptime.older_data_set == 0) ? 1 : 0];

    uptime.warm_start = true;
  }

  // update uptime
  while ((WAIT_GET_TIMER_VALUE_32() - uptime.ticks_last) >= WAIT_GET_TICKS_FOR_MS_32(1000)) {
    uptime.ticks_last += WAIT_GET_TICKS_FOR_MS_32(1000);
    SharedRamVariableInc(SHARED_RAM_VARIABLE_UPTIME);
  }

  // update lifetime (if neccssary and possible)
  const uint32_t stored_last = SharedRamVariableGet(SHARED_RAM_VARIABLE_UPTIME_LAST);
  const uint32_t store_interval = (stored_last == 0) ? UPTIME_UPDATE_INTERVAL_FIRST_IN_SECONDS : UPTIME_UPDATE_INTERVAL_REGULAR_IN_SECONDS;
  const bool time_to_store = (SharedRamVariableGet(SHARED_RAM_VARIABLE_UPTIME) - stored_last) >= store_interval;
  if (time_to_store) {

    const bool no_overrun = (UINT32_MAX - uptime.lifetime_in_seconds) >= store_interval;
    if (no_overrun) {

      // update & store lifetime, toggle data set and remember store time
      uptime.lifetime_in_seconds += store_interval;
      uptime.lifetime_stored_in_seconds[uptime.older_data_set] = uptime.lifetime_in_seconds;
      StorageStoreBlock((uptime.older_data_set == 0) ? STORAGE_BLOCK_CORE_UPTIME_FIRST : STORAGE_BLOCK_CORE_UPTIME_SECOND);
      uptime.older_data_set = (uptime.older_data_set == 0) ? 1 : 0;
      SharedRamVariableSet(SHARED_RAM_VARIABLE_UPTIME_LAST, stored_last + store_interval);
    }
  }
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
