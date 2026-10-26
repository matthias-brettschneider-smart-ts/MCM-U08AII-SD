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

#include "hal/IControllerIdentification.h"
#include "middleware/IConfig.h"
#include "middleware/IDefault.h"
#include "middleware/IGeneric.h"
#include "middleware/ISecurity.h"
#include "middleware/IStorage.h"
#include "os/IHeap.h"
#include "os/IInfo.h"
#include "ReleaseCore.h"

#ifndef INFO_FIRMWARE_TYPE
#error "error compile flags - INFO_FIRMWARE_TYPE undefined"
#elif ((INFO_FIRMWARE_TYPE < 0) || (INFO_FIRMWARE_TYPE > UINT16_MAX))
#error "error compile flags - INFO_FIRMWARE_TYPE invalid"
#endif

#ifndef INFO_FIRMWARE_VERSION_MAJOR
#error "error compile flags - INFO_FIRMWARE_VERSION_MAJOR undefined"
#elif ((INFO_FIRMWARE_VERSION_MAJOR < 0) || (INFO_FIRMWARE_VERSION_MAJOR > UINT8_MAX))
#error "error compile flags - INFO_FIRMWARE_VERSION_MAJOR invalid"
#endif

#ifndef INFO_FIRMWARE_VERSION_MINOR
#error "error compile flags - INFO_FIRMWARE_VERSION_MINOR undefined"
#elif ((INFO_FIRMWARE_VERSION_MINOR < 0) || (INFO_FIRMWARE_VERSION_MINOR > UINT8_MAX))
#error "error compile flags - INFO_FIRMWARE_VERSION_MINOR invalid"
#endif

#ifndef INFO_FIRMWARE_VERSION_PATCH
#error "error compile flags - INFO_FIRMWARE_VERSION_PATCH undefined"
#elif ((INFO_FIRMWARE_VERSION_PATCH < 0) || (INFO_FIRMWARE_VERSION_PATCH > UINT16_MAX))
#error "error compile flags - INFO_FIRMWARE_VERSION_PATCH invalid"
#endif

#ifndef INFO_FIRMWARE_VARIANT
#error "error compile flags - INFO_FIRMWARE_VARIANT undefined"
#elif ((INFO_FIRMWARE_VARIANT < 0) || (INFO_FIRMWARE_VARIANT > UINT8_MAX))
#error "error compile flags - INFO_FIRMWARE_VARIANT invalid"
#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define INFO_HARDWARE_TYPE_DEFAULT                                             0
#define INFO_HARDWARE_VERSION_MAJOR_DEFAULT                                    0
#define INFO_HARDWARE_VERSION_MINOR_DEFAULT                                    0
#define INFO_HARDWARE_VERSION_PATCH_DEFAULT                                    0
#define INFO_HARDWARE_VARIANT_DEFAULT                                        'Z'

#define INFO_SERIAL_NUMBER_DEFAULT                                             0

#define INFO_PRODUCTION_DATE_YEAR_DEFAULT                                      0
#define INFO_PRODUCTION_DATE_MONTH_DEFAULT                                     0
#define INFO_PRODUCTION_DATE_DAY_DEFAULT                                       0

typedef struct {
  uint16_t serial_number;
  t_DATE production_date;
  uint16_t hardware_type;
  t_VERSION hardware_version;
  uint8_t hardware_variant;
  uint8_t identification_smart[12];
  uint8_t identification_customer[32];
} t_INFO;

typedef struct {
  t_VERSION core_version;
  uint16_t firmware_type;
  t_VERSION firmware_version;
  uint8_t firmware_variant;
} t_INFO_CONST;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_INFO info = {
    .serial_number = INFO_SERIAL_NUMBER_DEFAULT,
    .production_date = { .year = INFO_PRODUCTION_DATE_YEAR_DEFAULT, .month = INFO_PRODUCTION_DATE_MONTH_DEFAULT, .day = INFO_PRODUCTION_DATE_DAY_DEFAULT },
    .hardware_type = INFO_HARDWARE_TYPE_DEFAULT,
    .hardware_version = { .major = INFO_HARDWARE_VERSION_MAJOR_DEFAULT, .minor = INFO_HARDWARE_VERSION_MINOR_DEFAULT, .patch = INFO_HARDWARE_VERSION_PATCH_DEFAULT },
    .hardware_variant = INFO_HARDWARE_VARIANT_DEFAULT
};

static const t_INFO_CONST info_const = {
    .core_version = { .major = RELEASE_CORE_VERSION_MAJOR, .minor = RELEASE_CORE_VERSION_MINOR, .patch = RELEASE_CORE_VERSION_PATCH },
    .firmware_type = INFO_FIRMWARE_TYPE,
    .firmware_version = { .major = INFO_FIRMWARE_VERSION_MAJOR, .minor = INFO_FIRMWARE_VERSION_MINOR, .patch = INFO_FIRMWARE_VERSION_PATCH },
    .firmware_variant = INFO_FIRMWARE_VARIANT
};

static const t_STORAGE_ENTRY info_storage[] = {
    { .storage_id = STORAGE_ID_HARDWARE_TYPE,           .storage_block = STORAGE_BLOCK_CORE_IDENTIFICATION, .data_pointer = (void *)&info.hardware_type,          .size = sizeof(info.hardware_type)           },
    { .storage_id = STORAGE_ID_HARDWARE_VERSION,        .storage_block = STORAGE_BLOCK_CORE_IDENTIFICATION, .data_pointer = (void *)&info.hardware_version,       .size = sizeof(info.hardware_version)        },
    { .storage_id = STORAGE_ID_HARDWARE_VARIANT,        .storage_block = STORAGE_BLOCK_CORE_IDENTIFICATION, .data_pointer = (void *)&info.hardware_variant,       .size = sizeof(info.hardware_variant)        },
    { .storage_id = STORAGE_ID_SERIAL_NUMBER,           .storage_block = STORAGE_BLOCK_CORE_IDENTIFICATION, .data_pointer = (void *)&info.serial_number,          .size = sizeof(info.serial_number)           },
    { .storage_id = STORAGE_ID_PRODUCTION_DATE,         .storage_block = STORAGE_BLOCK_CORE_IDENTIFICATION, .data_pointer = (void *)&info.production_date,        .size = sizeof(info.production_date)         },
    { .storage_id = STORAGE_ID_IDENTIFICATION_SMART,    .storage_block = STORAGE_BLOCK_CORE_IDENTIFICATION, .data_pointer = (void *)info.identification_smart,    .size = sizeof(info.identification_smart)    },
    { .storage_id = STORAGE_ID_IDENTIFICATION_CUSTOMER, .storage_block = STORAGE_BLOCK_CORE_IDENTIFICATION, .data_pointer = (void *)info.identification_customer, .size = sizeof(info.identification_customer) },
    { .storage_id = STORAGE_ID_UNDEFINED,               .storage_block = STORAGE_BLOCK_UNDEFINED,           .data_pointer = NULL,                                 .size = 0                                    }
};

static const t_CONFIG info_config[] = {
    { .config_id = CONFIG_ID_CORE_VERSION,            .config_type = CONFIG_TYPE_VERSION, .elements =  1, .readable = true,  .writeable = false, .data_pointer = (void *)&info_const.core_version     },
    { .config_id = CONFIG_ID_DEVICE_ID,               .config_type = CONFIG_TYPE_UINT16,  .elements =  1, .readable = true,  .writeable = false, .data_pointer = NULL                                 },
    { .config_id = CONFIG_ID_DEVICE_REVISION,         .config_type = CONFIG_TYPE_UINT16,  .elements =  1, .readable = true,  .writeable = false, .data_pointer = NULL                                 },
    { .config_id = CONFIG_ID_FIRMWARE_TYPE,           .config_type = CONFIG_TYPE_UINT16,  .elements =  1, .readable = true,  .writeable = false, .data_pointer = (void *)&info_const.firmware_type    },
    { .config_id = CONFIG_ID_FIRMWARE_VERSION,        .config_type = CONFIG_TYPE_VERSION, .elements =  1, .readable = true,  .writeable = false, .data_pointer = (void *)&info_const.firmware_version },
    { .config_id = CONFIG_ID_FIRMWARE_VARIANT,        .config_type = CONFIG_TYPE_UINT8,   .elements =  1, .readable = true,  .writeable = false, .data_pointer = (void *)&info_const.firmware_variant },
    { .config_id = CONFIG_ID_HARDWARE_TYPE,           .config_type = CONFIG_TYPE_UINT16,  .elements =  1, .readable = true,  .writeable = true,  .data_pointer = (void *)&info.hardware_type          },
    { .config_id = CONFIG_ID_HARDWARE_VERSION,        .config_type = CONFIG_TYPE_VERSION, .elements =  1, .readable = true,  .writeable = true,  .data_pointer = (void *)&info.hardware_version       },
    { .config_id = CONFIG_ID_HARDWARE_VARIANT,        .config_type = CONFIG_TYPE_UINT8,   .elements =  1, .readable = true,  .writeable = true,  .data_pointer = (void *)&info.hardware_variant       },
    { .config_id = CONFIG_ID_HEAP_INFO,               .config_type = CONFIG_TYPE_UINT32,  .elements =  1, .readable = true,  .writeable = false, .data_pointer = NULL                                 },
    { .config_id = CONFIG_ID_PRODUCTION_DATE,         .config_type = CONFIG_TYPE_DATE,    .elements =  1, .readable = true,  .writeable = true,  .data_pointer = (void *)&info.production_date        },
    { .config_id = CONFIG_ID_SERIAL_NUMBER,           .config_type = CONFIG_TYPE_UINT16,  .elements =  1, .readable = true,  .writeable = true,  .data_pointer = (void *)&info.serial_number          },
    { .config_id = CONFIG_ID_IDENTIFICATION_SMART,    .config_type = CONFIG_TYPE_RAW,     .elements = 12, .readable = true,  .writeable = true,  .data_pointer = (void *)info.identification_smart    },
    { .config_id = CONFIG_ID_IDENTIFICATION_CUSTOMER, .config_type = CONFIG_TYPE_RAW,     .elements = 32, .readable = true,  .writeable = true,  .data_pointer = (void *)info.identification_customer },
    { .config_id = CONFIG_ID_INVALID,                 .config_type = CONFIG_TYPE_UNKNOWN, .elements =  1, .readable = false, .writeable = false, .data_pointer = NULL                                 }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static DEFAULT_CALLBACK(InfoDefaultCallback);
static CONFIG_CALLBACK(InfoConfigCallback);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
void InfoInit(void)
{
  StorageRegister(info_storage);
  StorageLoad(info_storage);

  ConfigRegister(info_config, InfoConfigCallback);

  DefaultRegister(InfoDefaultCallback);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Callback routine used for factory reset.
 *
 -----------------------------------------------------------------------------*/
static DEFAULT_CALLBACK(InfoDefaultCallback)
{
  if (SecurityGetCurrentLevel() >= SECURITY_LEVEL_MAINTENANCE)
    memset(info.identification_customer, 0, sizeof(info.identification_customer));

  if (SecurityGetCurrentLevel() >= SECURITY_LEVEL_MANUFACTURER) {
    info.serial_number = INFO_SERIAL_NUMBER_DEFAULT;
    info.production_date.year = INFO_PRODUCTION_DATE_YEAR_DEFAULT;
    info.production_date.month = INFO_PRODUCTION_DATE_MONTH_DEFAULT;
    info.production_date.day = INFO_PRODUCTION_DATE_DAY_DEFAULT;
    info.hardware_type = INFO_HARDWARE_TYPE_DEFAULT;
    info.hardware_version.major = INFO_HARDWARE_VERSION_MAJOR_DEFAULT;
    info.hardware_version.minor = INFO_HARDWARE_VERSION_MINOR_DEFAULT;
    info.hardware_version.patch = INFO_HARDWARE_VERSION_PATCH_DEFAULT;
    info.hardware_variant = INFO_HARDWARE_VARIANT_DEFAULT;
    memset(info.identification_smart, 0, sizeof(info.identification_smart));
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Callback routine used for configuration accesses not covered by simple read/write via pointers.
 *
 -----------------------------------------------------------------------------*/
static CONFIG_CALLBACK(InfoConfigCallback)
{
  e_ERROR_ID result = ERROR_ID_SUCCESS;

  switch (config_id) {

    case CONFIG_ID_DEVICE_ID:

      if ((callback_type == CONFIG_CALLBACK_TYPE_PRE) && read)
        *(uint16_t *)data = ControllerIdentificationGetDeviceId();

      break;

    case CONFIG_ID_DEVICE_REVISION:

      if ((callback_type == CONFIG_CALLBACK_TYPE_PRE) && read)
        *(uint16_t *)data = ControllerIdentificationGetDeviceRevision();

      break;

    case CONFIG_ID_HEAP_INFO:

      if ((callback_type == CONFIG_CALLBACK_TYPE_PRE) && read)
#ifdef HEAP_SIZE_IN_BYTE // IME: core - remove workaround
        *(uint32_t *)data = (uint32_t)HeapGetNumberOfBytesAvailable();
#else
        *(uint32_t *)data = 0;
#endif

      break;

    default:
      break;
  }

  return result;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
