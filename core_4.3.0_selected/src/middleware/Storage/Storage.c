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
#include "middleware/ICrc.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"
#include "middleware/IMemory.h"
#include "middleware/ISecurity.h"
#include "middleware/IStorage.h"
#include "middleware/IStorageInternal.h"
#include "os/IEeprom.h"

#ifndef STORAGE_MINIMUM_SIZE_PER_BLOCK_IN_BYTE
#error "error compile flags - STORAGE_MINIMUM_SIZE_PER_BLOCK_IN_BYTE undefined"
#elif ((STORAGE_MINIMUM_SIZE_PER_BLOCK_IN_BYTE < 0) || (STORAGE_MINIMUM_SIZE_PER_BLOCK_IN_BYTE > 512))
#error "error compile flags - STORAGE_MINIMUM_SIZE_PER_BLOCK_IN_BYTE invalid"
#endif

#ifndef STORAGE_MAXIMUM_SIZE_PER_BLOCK_IN_BYTE
#error "error compile flags - STORAGE_MAXIMUM_SIZE_PER_BLOCK_IN_BYTE undefined"
#elif ((STORAGE_MAXIMUM_SIZE_PER_BLOCK_IN_BYTE < 0) || (STORAGE_MAXIMUM_SIZE_PER_BLOCK_IN_BYTE > 512))
#error "error compile flags - STORAGE_MAXIMUM_SIZE_PER_BLOCK_IN_BYTE invalid"
#endif

#ifndef STORAGE_NUMBER_OF_REGISTRATIONS
#error "error compile flags - STORAGE_NUMBER_OF_REGISTRATIONS undefined"
#elif ((STORAGE_NUMBER_OF_REGISTRATIONS <= 0) || (STORAGE_NUMBER_OF_REGISTRATIONS > 20))
#error "error compile flags - STORAGE_NUMBER_OF_REGISTRATIONS invalid"
#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define STORAGE_CHECKSUM_SIZE                                                  2

#define STORAGE_BLOCK_CORE_UPTIME_FIRST_SIZE_IN_BYTE            STORAGE_MINIMUM_SIZE_PER_BLOCK_IN_BYTE
#define STORAGE_BLOCK_CORE_UPTIME_SECOND_SIZE_IN_BYTE           STORAGE_MINIMUM_SIZE_PER_BLOCK_IN_BYTE
// further block sizes are defined individually

#define STORAGE_BLOCK_CORE_UPTIME_FIRST_ADDRESS                                0
#define STORAGE_BLOCK_CORE_UPTIME_SECOND_ADDRESS                (STORAGE_BLOCK_CORE_UPTIME_FIRST_ADDRESS + STORAGE_BLOCK_CORE_UPTIME_FIRST_SIZE_IN_BYTE)
#define STORAGE_BLOCK_CORE_IDENTIFICATION_START_ADDRESS         (STORAGE_BLOCK_CORE_UPTIME_SECOND_ADDRESS + STORAGE_BLOCK_CORE_UPTIME_SECOND_SIZE_IN_BYTE)
#define STORAGE_BLOCK_CORE_SETTINGS_START_ADDRESS               (STORAGE_BLOCK_CORE_IDENTIFICATION_START_ADDRESS + STORAGE_BLOCK_CORE_IDENTIFICATION_SIZE_IN_BYTE)
#define STORAGE_BLOCK_APPL_SETTINGS_START_ADDRESS               (STORAGE_BLOCK_CORE_SETTINGS_START_ADDRESS + STORAGE_BLOCK_CORE_SETTINGS_SIZE_IN_BYTE)
#define STORAGE_BLOCK_APPL_ADJUSTMENT_SMART_START_ADDRESS       (STORAGE_BLOCK_APPL_SETTINGS_START_ADDRESS + STORAGE_BLOCK_APPL_SETTINGS_SIZE_IN_BYTE)
#define STORAGE_BLOCK_APPL_ADJUSTMENT_CUSTOMER_START_ADDRESS    (STORAGE_BLOCK_APPL_ADJUSTMENT_SMART_START_ADDRESS + STORAGE_BLOCK_APPL_ADJUSTMENT_SMART_SIZE_IN_BYTE)

#define STORAGE_ERROR_ATTRIBUTE_BLOCK_NOT_CONFIGURED                           1
#define STORAGE_ERROR_ATTRIBUTE_BLOCK_NOT_VALID                                2
#define STORAGE_ERROR_ATTRIBUTE_BLOCK_OUT_OF_SPACE                             3
#define STORAGE_ERROR_ATTRIBUTE_BLOCK_NOT_ALLOWED                              4
#define STORAGE_ERROR_ATTRIBUTE_BLOCK_NOT_READABLE                             5
#define STORAGE_ERROR_ATTRIBUTE_BLOCK_NOT_WRITEABLE                            6

typedef struct {
  e_STORAGE_BLOCK block;
  e_SECURITY_LEVEL required_security_level;
  bool store_settings;
  uint16_t block_start_address;
  uint16_t block_number_of_bytes;
  uint32_t * free_bytes;
  e_ERROR_ID error_id;
} t_STORAGE_BLOCK;

typedef struct {
  uint32_t number_of_blocks;
  uint32_t free_bytes_core_identification;
  uint32_t free_bytes_core_settings;
  uint32_t free_bytes_module_settings;
  uint32_t free_bytes_module_adjustment_smart;
  uint32_t free_bytes_module_adjustment_customer;
} t_STORAGE_INFO;

typedef struct {
  bool initialized;
  t_STORAGE_INFO info;
  t_STORAGE_ENTRY * entry_pointers[STORAGE_NUMBER_OF_REGISTRATIONS];
} t_STORAGE;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_STORAGE storage;

static const t_STORAGE_BLOCK storage_blocks[] = {
    {
        .block                   = STORAGE_BLOCK_CORE_UPTIME_FIRST,
        .required_security_level = SECURITY_LEVEL_ALL,
        .store_settings          = false,
        .block_start_address     = STORAGE_BLOCK_CORE_UPTIME_FIRST_ADDRESS,
        .block_number_of_bytes   = STORAGE_BLOCK_CORE_UPTIME_FIRST_SIZE_IN_BYTE,
        .free_bytes              = NULL,
        .error_id                = ERROR_ID_UPTIME_CORRUPT
    },
    {
        .block                   = STORAGE_BLOCK_CORE_UPTIME_SECOND,
        .required_security_level = SECURITY_LEVEL_ALL,
        .store_settings          = false,
        .block_start_address     = STORAGE_BLOCK_CORE_UPTIME_SECOND_ADDRESS,
        .block_number_of_bytes   = STORAGE_BLOCK_CORE_UPTIME_SECOND_SIZE_IN_BYTE,
        .free_bytes              = NULL,
        .error_id                = ERROR_ID_UPTIME_CORRUPT
    },
    {
        .block                   = STORAGE_BLOCK_CORE_IDENTIFICATION,
        .required_security_level = SECURITY_LEVEL_MANUFACTURER,
        .store_settings          = true,
        .block_start_address     = STORAGE_BLOCK_CORE_IDENTIFICATION_START_ADDRESS,
        .block_number_of_bytes   = STORAGE_BLOCK_CORE_IDENTIFICATION_SIZE_IN_BYTE,
        .free_bytes              = &storage.info.free_bytes_core_identification,
        .error_id                = ERROR_ID_STORAGE_ACCESS_FAILED_CORE_IDENTIFICATION
    },
    {
        .block                   = STORAGE_BLOCK_CORE_SETTINGS,
        .required_security_level = SECURITY_LEVEL_ALL,
        .store_settings          = true,
        .block_start_address     = STORAGE_BLOCK_CORE_SETTINGS_START_ADDRESS,
        .block_number_of_bytes   = STORAGE_BLOCK_CORE_SETTINGS_SIZE_IN_BYTE,
        .free_bytes              = &storage.info.free_bytes_core_settings,
        .error_id                = ERROR_ID_STORAGE_ACCESS_FAILED_CORE_SETTINGS
    },
    {
        .block                   = STORAGE_BLOCK_MODULE_SETTINGS,
        .required_security_level = SECURITY_LEVEL_ALL,
        .store_settings          = true,
        .block_start_address     = STORAGE_BLOCK_APPL_SETTINGS_START_ADDRESS,
        .block_number_of_bytes   = STORAGE_BLOCK_APPL_SETTINGS_SIZE_IN_BYTE,
        .free_bytes              = &storage.info.free_bytes_module_settings,
        .error_id                = ERROR_ID_STORAGE_ACCESS_FAILED_MODULE_SETTINGS
    },
    {
        .block                   = STORAGE_BLOCK_MODULE_ADJUSTMENT_SMART,
        .required_security_level = SECURITY_LEVEL_MANUFACTURER,
        .store_settings          = true,
        .block_start_address     = STORAGE_BLOCK_APPL_ADJUSTMENT_SMART_START_ADDRESS,
        .block_number_of_bytes   = STORAGE_BLOCK_APPL_ADJUSTMENT_SMART_SIZE_IN_BYTE,
        .free_bytes              = &storage.info.free_bytes_module_adjustment_smart,
        .error_id                = ERROR_ID_STORAGE_ACCESS_FAILED_MODULE_ADJUSTMENT_SMART
    },
    {
        .block                   = STORAGE_BLOCK_MODULE_ADJUSTMENT_CUSTOMER,
        .required_security_level = SECURITY_LEVEL_MAINTENANCE,
        .store_settings          = true,
        .block_start_address     = STORAGE_BLOCK_APPL_ADJUSTMENT_CUSTOMER_START_ADDRESS,
        .block_number_of_bytes   = STORAGE_BLOCK_APPL_ADJUSTMENT_CUSTOMER_SIZE_IN_BYTE,
        .free_bytes              = &storage.info.free_bytes_module_adjustment_customer,
        .error_id                = ERROR_ID_STORAGE_ACCESS_FAILED_MODULE_ADJUSTMENT_CUSTOMER
    }
};

static const t_CONFIG storage_config[] = {
    { .config_id = CONFIG_ID_STORAGE_INFO, .config_type = CONFIG_TYPE_UINT32,  .elements = 6, .readable = true,  .writeable = false, .data_pointer = (void*)&storage.info },
    { .config_id = CONFIG_ID_INVALID,      .config_type = CONFIG_TYPE_UNKNOWN, .elements = 0, .readable = false, .writeable = false, .data_pointer = NULL                 }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static t_STORAGE_BLOCK * StorageGetBlockConfig(const e_STORAGE_BLOCK block);
static e_ERROR_ID StorageLoadFromBlock(const t_STORAGE_BLOCK * const block_config, const t_STORAGE_ENTRY entries[]);
static e_ERROR_ID StorageCreateBlockDump(const t_STORAGE_BLOCK * const block_config, uint8_t dump[]);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
void StorageInit(void)
{
  // do some minimal check of the block configuration
  const size_t number_of_blocks = sizeof(storage_blocks) / sizeof(t_STORAGE_BLOCK);
  for (size_t block_index = 0; block_index < number_of_blocks; block_index++) {
    const uint16_t start_address = storage_blocks[block_index].block_start_address;
    const uint16_t number_of_bytes = storage_blocks[block_index].block_number_of_bytes;
    const bool invalid_start_address = (start_address % EepromGetPageSize()) != 0;
    const bool invalid_number_of_bytes = (number_of_bytes > STORAGE_MAXIMUM_SIZE_PER_BLOCK_IN_BYTE) || ((number_of_bytes % EepromGetPageSize()) != 0);
    const bool overflow = (start_address + number_of_bytes) > EepromGetEepromSize();
    if (invalid_start_address || invalid_number_of_bytes || overflow)
      ErrorAddEntry(ERROR_ID_FATAL_STORAGE_MISCONFIGURED, 0);
  }

  EepromInit();
  ASSERT_FATAL(EepromGetPageSize() == STORAGE_MINIMUM_SIZE_PER_BLOCK_IN_BYTE);

  ConfigRegister(storage_config, NULL);

  storage.info.number_of_blocks = sizeof(t_STORAGE_INFO) / sizeof(uint32_t) - 1;
  storage.initialized = true;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Register new storage IDs.
 *
 -----------------------------------------------------------------------------*/
void StorageRegister(const t_STORAGE_ENTRY entries[])
{
  uint16_t table_index = 0;
  bool already_registered = false;

  // search empty index or already stored pointer
  while ((table_index <= (STORAGE_NUMBER_OF_REGISTRATIONS - 1)) && (storage.entry_pointers[table_index] != NULL)) {
    if (storage.entry_pointers[table_index] == entries)
      already_registered = true;
    table_index++;
  }

  const bool no_free_entry = table_index >= STORAGE_NUMBER_OF_REGISTRATIONS;
  if (no_free_entry || already_registered)
    ErrorAddEntry(ERROR_ID_STORAGE_REGISTRATION, 0);
  else
    storage.entry_pointers[table_index] = (t_STORAGE_ENTRY *)entries;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
static t_STORAGE_BLOCK * StorageGetBlockConfig(const e_STORAGE_BLOCK block)
{
  t_STORAGE_BLOCK * block_config = NULL;

  const size_t number_of_blocks = sizeof(storage_blocks) / sizeof(t_STORAGE_BLOCK);
  for (size_t block_index = 0; block_index < number_of_blocks; block_index++)
    if (storage_blocks[block_index].block == block)
      block_config = (t_STORAGE_BLOCK *)&storage_blocks[block_index];

  return block_config;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
static e_ERROR_ID StorageLoadFromBlock(const t_STORAGE_BLOCK * const block_config, const t_STORAGE_ENTRY entries[])
{
  bool one_or_more_values_not_available = false;

  const uint16_t block_size = block_config->block_number_of_bytes;

  uint8_t dump[STORAGE_MAXIMUM_SIZE_PER_BLOCK_IN_BYTE];
  bool block_valid = EepromRead(block_config->block_start_address, dump, block_size) == ERROR_ID_SUCCESS;
  if (block_valid)
    block_valid = CrcGetCrc16(dump, block_size) == 0;

  if (!block_valid) {
    ErrorAddEntry(block_config->error_id, STORAGE_ERROR_ATTRIBUTE_BLOCK_NOT_VALID);
    one_or_more_values_not_available = true;
  }
  else {

    for (uint16_t entry = 0; entries[entry].storage_id != STORAGE_ID_UNDEFINED; entry++) {
      if (entries[entry].storage_block == block_config->block) {

        uint8_t * data = dump;
        bool value_not_found = true;

        // parse block till we reached its end or found the required value
        while (value_not_found && (data < (dump + block_size))) {

          const uint8_t id = *data++;
          const uint8_t length = *data++;

          const bool skip_value = (id != entries[entry].storage_id) || (length != entries[entry].size);
          if (skip_value)
            data += length;
          else {
            memcpy(entries[entry].data_pointer, data, entries[entry].size);
            value_not_found = false;
          }
        }

        // may be OK in case of firmware up- or down-grades but the user should be informed
        if (value_not_found) {
          ErrorAddEntry(ERROR_ID_STORAGE_READ_VALUE, 0);
          one_or_more_values_not_available = true;
        }
      }
    }
  }

  return one_or_more_values_not_available ? ERROR_ID_STORAGE_READ_VALUE : ERROR_ID_SUCCESS;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Load values for the given list of storage IDs if available.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID StorageLoad(const t_STORAGE_ENTRY entries[])
{
  ASSERT_FATAL(entries != NULL);
  ASSERT_FATAL(storage.initialized);

  e_ERROR_ID result = ERROR_ID_SUCCESS;

  const size_t number_of_blocks = sizeof(storage_blocks) / sizeof(t_STORAGE_BLOCK);
  for (size_t block_index = 0; block_index < number_of_blocks; block_index++) {

    bool block_required = false;
    for (uint16_t entry = 0; entries[entry].storage_id != STORAGE_ID_UNDEFINED; entry++)
      if (entries[entry].storage_block == storage_blocks[block_index].block)
        block_required = true;

    if (block_required) {

      if (storage_blocks[block_index].block_number_of_bytes == 0) {
        ErrorAddEntry(storage_blocks[block_index].error_id, STORAGE_ERROR_ATTRIBUTE_BLOCK_NOT_CONFIGURED);
        result = ERROR_ID_STORAGE_READ_VALUE;
      }
      else if (StorageLoadFromBlock(&storage_blocks[block_index], entries) != ERROR_ID_SUCCESS)
        result = ERROR_ID_STORAGE_READ_VALUE;
    }
  }

  return result;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
static e_ERROR_ID StorageCreateBlockDump(const t_STORAGE_BLOCK * const block_config, uint8_t dump[])
{
  ASSERT_FATAL(block_config != NULL);
  ASSERT_FATAL(dump != NULL);

  e_ERROR_ID result = ERROR_ID_SUCCESS;

  // process zero size blocks as well to determine and log invalid configurations
  uint16_t free_bytes = (block_config->block_number_of_bytes != 0) ? (block_config->block_number_of_bytes - STORAGE_CHECKSUM_SIZE) : 0;
  uint8_t * data = dump;

  for (uint16_t table = 0; (table < STORAGE_NUMBER_OF_REGISTRATIONS) && (storage.entry_pointers[table] != NULL); table++) {

    t_STORAGE_ENTRY * entry_pointer = storage.entry_pointers[table];

    while (entry_pointer->storage_id != STORAGE_ID_UNDEFINED) {

      const bool value_required = entry_pointer->storage_block == block_config->block;
      if (value_required) {

        const bool out_of_space = free_bytes < ((uint16_t)entry_pointer->size + 2u);
        if (out_of_space) {
          ErrorAddEntry(block_config->error_id, STORAGE_ERROR_ATTRIBUTE_BLOCK_OUT_OF_SPACE);
          result = ERROR_ID_FAILED;
        }
        else {
          *data++ = (uint8_t)entry_pointer->storage_id;
          *data++ = entry_pointer->size;
          memcpy(data, entry_pointer->data_pointer, entry_pointer->size);
          data += entry_pointer->size;
          free_bytes -= ((uint16_t)entry_pointer->size + 2u);
        }
      }

      entry_pointer++;
    }
  }

  if (block_config->block_number_of_bytes == 0)
    result = ERROR_ID_FAILED;

  if (result == ERROR_ID_SUCCESS) {
    const uint16_t crc_swapped = MEMORY_SWAP_UINT16(CrcGetCrc16(dump, block_config->block_number_of_bytes - STORAGE_CHECKSUM_SIZE));
    uint8_t * const checksum = &dump[block_config->block_number_of_bytes - STORAGE_CHECKSUM_SIZE];
    memcpy(checksum, &crc_swapped, STORAGE_CHECKSUM_SIZE);
  }

  if (block_config->free_bytes != NULL)
    *(block_config->free_bytes) = free_bytes;

  return result;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Update specific block if its configured and writable at the current security level
 *
 -----------------------------------------------------------------------------*/
void StorageStoreBlock(const e_STORAGE_BLOCK block)
{
  ASSERT_FATAL(storage.initialized);

  uint8_t dump_current[STORAGE_MAXIMUM_SIZE_PER_BLOCK_IN_BYTE];
  uint8_t dump_new[STORAGE_MAXIMUM_SIZE_PER_BLOCK_IN_BYTE];
  memset(dump_new, 0, sizeof(dump_new));

  t_STORAGE_BLOCK * const block_config = StorageGetBlockConfig(block);

  if (block_config == NULL)
    ErrorAddEntry(ERROR_ID_STORAGE_WRITE_VALUE, 0);
  else if (StorageCreateBlockDump(block_config, dump_new) != ERROR_ID_SUCCESS)
    {} // nothing to do - potential errors are already detected during dump creation
  else if (EepromRead(block_config->block_start_address, dump_current, block_config->block_number_of_bytes) != ERROR_ID_SUCCESS)
    ErrorAddEntry(block_config->error_id, STORAGE_ERROR_ATTRIBUTE_BLOCK_NOT_READABLE);
  else if (memcmp(dump_new, dump_current, block_config->block_number_of_bytes) == 0)
    {}// nothing to do - block has not changed
  else if (SecurityGetCurrentLevel() < block_config->required_security_level)
    ErrorAddEntry(block_config->error_id, STORAGE_ERROR_ATTRIBUTE_BLOCK_NOT_ALLOWED);
  else if (EepromWrite(block_config->block_start_address, dump_new, block_config->block_number_of_bytes) != ERROR_ID_SUCCESS)
    ErrorAddEntry(block_config->error_id, STORAGE_ERROR_ATTRIBUTE_BLOCK_NOT_WRITEABLE);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Try to update all blocks that are allowed to be processed at store settings.
 *
 -----------------------------------------------------------------------------*/
void StorageStoreSettings(void)
{
  const size_t number_of_blocks = sizeof(storage_blocks) / sizeof(t_STORAGE_BLOCK);
  for (size_t block_index = 0; block_index < number_of_blocks; block_index++)
    if (storage_blocks[block_index].store_settings)
      StorageStoreBlock(storage_blocks[block_index].block);
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
