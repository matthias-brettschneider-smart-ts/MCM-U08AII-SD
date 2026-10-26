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

#include "hal/ICan.h"
#include "middleware/IAssert.h"
#include "middleware/ICanMessage.h"
#include "middleware/IConfig.h"
#include "middleware/IDefault.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"
#include "middleware/IModuleNumber.h"
#include "middleware/IStorage.h"
#include "middleware/IWait.h"
#include "os/ICanBase.h"

#ifndef CAN_BASE_BLOCK_OFFSET_DEFAULT
#error "error compile flags - CAN_BASE_BLOCK_OFFSET_DEFAULT undefined"
#elif ((CAN_BASE_BLOCK_OFFSET_DEFAULT < CAN_MESSAGE_ID_FIRST) || (CAN_BASE_BLOCK_OFFSET_DEFAULT > (CAN_MESSAGE_ID_LAST - 1)))
#error "error compile flags - CAN_BASE_BLOCK_OFFSET_DEFAULT invalid"
#endif

#ifndef CAN_BASE_NUMBER_OF_IDENTIFIER_PER_MODULE
#error "error compile flags - CAN_BASE_NUMBER_OF_IDENTIFIER_PER_MODULE undefined"
#elif ((CAN_BASE_NUMBER_OF_IDENTIFIER_PER_MODULE < 2) || (CAN_BASE_NUMBER_OF_IDENTIFIER_PER_MODULE > 0x100))
#error "error compile flags - CAN_BASE_NUMBER_OF_IDENTIFIER_PER_MODULE invalid"
#endif

#ifndef CAN_BASE_NUMBER_OF_APPLICATION_IDENTIFIER
#error "error compile flags - CAN_BASE_NUMBER_OF_APPLICATION_IDENTIFIER undefined"
#elif ((CAN_BASE_NUMBER_OF_APPLICATION_IDENTIFIER < 0) || (CAN_BASE_NUMBER_OF_APPLICATION_IDENTIFIER > 0xFE))
#error "error compile flags - CAN_BASE_NUMBER_OF_APPLICATION_IDENTIFIER invalid"
#endif

#ifndef CAN_BASE_RECEIVE_APPLICATION_IDENTIFIER
#error "error compile flags - CAN_BASE_RECEIVE_APPLICATION_IDENTIFIER undefined"
#elif ((CAN_BASE_RECEIVE_APPLICATION_IDENTIFIER != false) && (CAN_BASE_RECEIVE_APPLICATION_IDENTIFIER != true))
#error "error compile flags - CAN_BASE_RECEIVE_APPLICATION_IDENTIFIER invalid"
#endif

#ifndef CAN_BASE_RECEIVE_OTHER_IDENTIFIER
#error "error compile flags - CAN_BASE_RECEIVE_OTHER_IDENTIFIER undefined"
#elif ((CAN_BASE_RECEIVE_OTHER_IDENTIFIER != false) && (CAN_BASE_RECEIVE_OTHER_IDENTIFIER != true))
#error "error compile flags - CAN_BASE_RECEIVE_OTHER_IDENTIFIER invalid"
#endif

#if ((CAN_BASE_NUMBER_OF_APPLICATION_IDENTIFIER + 2) > CAN_BASE_NUMBER_OF_IDENTIFIER_PER_MODULE)
#error "error compile flags - inconsistent configuration"
#elif ((CAN_BASE_RECEIVE_APPLICATION_IDENTIFIER == true) && (CAN_BASE_NUMBER_OF_APPLICATION_IDENTIFIER > 6))
#error "error compile flags - inconsistent configuration"
#elif ((CAN_BASE_RECEIVE_APPLICATION_IDENTIFIER == true) && (CAN_BASE_RECEIVE_OTHER_IDENTIFIER == true))
#error "error compile flags - inconsistent configuration"
#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define CAN_BASE_BIT_RATE_NOMINAL_DEFAULT                                   1000
#define CAN_BASE_BIT_RATE_DATA_DEFAULT                                      2000

#define CAN_BASE_ID_REQUEST_BROADCAST_DEFAULT                              0x100

#define CAN_BASE_NODE_PRIMARY                                                  0 // do not change assignemnt - use pin mapping if they have to be switched!
#define CAN_BASE_NODE_SECONDARY                                                1

#define CAN_BASE_FIFO_TRANSMIT_RESPONSE                                        0
#define CAN_BASE_FIFO_TRANSMIT_APPLICATION                                     1
#define CAN_BASE_FIFO_TRANSMIT_OTHER                                           2
#define CAN_BASE_FIFO_RECEIVE_REQUEST                                          3
#define CAN_BASE_FIFO_RECEIVE_APPLICATION                                      4
#define CAN_BASE_FIFO_RECEIVE_OTHER                                            5

typedef struct {
  struct {
    bool storage_test;
    bool has_valid_settings;
    bool has_valid_identifier;
    struct {
      uint16_t primary_nominal;
      uint16_t primary_data;
      uint16_t secondary_nominal;
      uint16_t secondary_data;
    } bit_rate;
    struct {
      uint16_t request_broadcast;
      uint16_t block_offset;
    } id;
  } init;
  struct {
    bool application_transmissions_are_allowed;
    struct {
      uint16_t request_broadcast;
      uint16_t request_singlecast;
      uint16_t response;
      uint16_t first_application;
    } id;
  } runtime;
} t_CAN_BASE;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_CAN_BASE can_base;

static const t_STORAGE_ENTRY can_base_storage[] = {
    { .storage_id = STORAGE_ID_CAN_BIT_RATE_PRIMARY_NOMINAL,     .storage_block = STORAGE_BLOCK_CORE_SETTINGS, .data_pointer = (void *)&can_base.init.bit_rate.primary_nominal,   .size = sizeof(can_base.init.bit_rate.primary_nominal)   },
    { .storage_id = STORAGE_ID_CAN_BIT_RATE_PRIMARY_DATA,        .storage_block = STORAGE_BLOCK_CORE_SETTINGS, .data_pointer = (void *)&can_base.init.bit_rate.primary_data,      .size = sizeof(can_base.init.bit_rate.primary_data)      },
    { .storage_id = STORAGE_ID_CAN_BIT_RATE_SECONDARY_NOMINAL,   .storage_block = STORAGE_BLOCK_CORE_SETTINGS, .data_pointer = (void *)&can_base.init.bit_rate.secondary_nominal, .size = sizeof(can_base.init.bit_rate.secondary_nominal) },
    { .storage_id = STORAGE_ID_CAN_BIT_RATE_SECONDARY_DATA,      .storage_block = STORAGE_BLOCK_CORE_SETTINGS, .data_pointer = (void *)&can_base.init.bit_rate.secondary_data,    .size = sizeof(can_base.init.bit_rate.secondary_data)    },
    { .storage_id = STORAGE_ID_CAN_ID_REQUEST_BROADCAST,         .storage_block = STORAGE_BLOCK_CORE_SETTINGS, .data_pointer = (void *)&can_base.init.id.request_broadcast,       .size = sizeof(can_base.init.id.request_broadcast)       },
    { .storage_id = STORAGE_ID_CAN_ID_BLOCK_OFFSET,              .storage_block = STORAGE_BLOCK_CORE_SETTINGS, .data_pointer = (void *)&can_base.init.id.block_offset,            .size = sizeof(can_base.init.id.block_offset)            },
    { .storage_id = STORAGE_ID_UNDEFINED,                        .storage_block = STORAGE_BLOCK_UNDEFINED,     .data_pointer = NULL,                                              .size = 0                                                }
};

static const t_STORAGE_ENTRY can_base_storage_test[] = {
    { .storage_id = STORAGE_ID_CAN_INIT_DETECTION, .storage_block = STORAGE_BLOCK_CORE_IDENTIFICATION, .data_pointer = (void *)&can_base.init.storage_test, .size = sizeof(can_base.init.storage_test) },
    { .storage_id = STORAGE_ID_UNDEFINED,          .storage_block = STORAGE_BLOCK_UNDEFINED,           .data_pointer = NULL,                                .size = 0                                  }
};

static const t_CONFIG can_base_config[] = {
    { .config_id = CONFIG_ID_CAN_BIT_RATE_PRIMARY_NOMINAL_STORED,   .config_type = CONFIG_TYPE_UINT16,  .elements = 1, .readable = true,  .writeable = true,  .data_pointer = (void*)&can_base.init.bit_rate.primary_nominal   },
    { .config_id = CONFIG_ID_CAN_BIT_RATE_PRIMARY_DATA_STORED,      .config_type = CONFIG_TYPE_UINT16,  .elements = 1, .readable = true,  .writeable = true,  .data_pointer = (void*)&can_base.init.bit_rate.primary_data      },
    { .config_id = CONFIG_ID_CAN_BIT_RATE_SECONDARY_NOMINAL_STORED, .config_type = CONFIG_TYPE_UINT16,  .elements = 1, .readable = true,  .writeable = true,  .data_pointer = (void*)&can_base.init.bit_rate.secondary_nominal },
    { .config_id = CONFIG_ID_CAN_BIT_RATE_SECONDARY_DATA_STORED,    .config_type = CONFIG_TYPE_UINT16,  .elements = 1, .readable = true,  .writeable = true,  .data_pointer = (void*)&can_base.init.bit_rate.secondary_data    },
    { .config_id = CONFIG_ID_CAN_ID_REQUEST_BROADCAST_STORED,       .config_type = CONFIG_TYPE_UINT16,  .elements = 1, .readable = true,  .writeable = true,  .data_pointer = (void*)&can_base.init.id.request_broadcast       },
    { .config_id = CONFIG_ID_CAN_ID_BLOCK_OFFSET_STORED,            .config_type = CONFIG_TYPE_UINT16,  .elements = 1, .readable = true,  .writeable = true,  .data_pointer = (void*)&can_base.init.id.block_offset            },
    { .config_id = CONFIG_ID_INVALID,                               .config_type = CONFIG_TYPE_UNKNOWN, .elements = 0, .readable = false, .writeable = false, .data_pointer = NULL                                             }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static void CanBaseReset(void);

static DEFAULT_CALLBACK(CanBaseDefaultCallback);
static CONFIG_CALLBACK(CanBaseConfigCallback);

static uint16_t CanBaseCalculateFirstModuleIdentifier(const uint16_t id_request_broadcast, const uint16_t id_block_offset);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
void CanBaseInit(void)
{
  can_base.init.has_valid_identifier = false;
  CanBaseReset();
  CanInit();

  const bool has_second_node = CAN_GET_NUMBER_OF_NODES() > 1;
  const bool supports_fd = CAN_SUPPORTS_FD();

  StorageRegister(can_base_storage_test);
  StorageRegister(can_base_storage);
  const bool settings_not_stored_before = StorageLoad(can_base_storage_test) != ERROR_ID_SUCCESS;
  const bool settings_readable = StorageLoad(can_base_storage) == ERROR_ID_SUCCESS;
  can_base.init.has_valid_settings = settings_readable || settings_not_stored_before; // IME: core - can - use result to prevent transmissions

  CanInitNode(CAN_BASE_NODE_PRIMARY);
  CanSetNominalBitrate(CAN_BASE_NODE_PRIMARY, can_base.init.bit_rate.primary_nominal);
  if (supports_fd)
    CanSetDataBitrate(CAN_BASE_NODE_PRIMARY, can_base.init.bit_rate.primary_data);

  if (has_second_node) {
    CanInitNode(CAN_BASE_NODE_SECONDARY);
    CanSetNominalBitrate(CAN_BASE_NODE_SECONDARY, can_base.init.bit_rate.secondary_nominal);
    if (supports_fd)
      CanSetDataBitrate(CAN_BASE_NODE_SECONDARY, can_base.init.bit_rate.secondary_data);
  }

  CanBaseUpdateIdentifier();

  DefaultRegister(CanBaseDefaultCallback);
  ConfigRegister(can_base_config, CanBaseConfigCallback);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Callback routine used for factory reset.
 *
 -----------------------------------------------------------------------------*/
static DEFAULT_CALLBACK(CanBaseDefaultCallback)
{
  CanBaseReset();
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
static void CanBaseReset(void)
{
  can_base.init.bit_rate.primary_nominal = CAN_BASE_BIT_RATE_NOMINAL_DEFAULT;
  can_base.init.bit_rate.primary_data = CAN_BASE_BIT_RATE_DATA_DEFAULT;
  can_base.init.bit_rate.secondary_nominal = CAN_BASE_BIT_RATE_NOMINAL_DEFAULT;
  can_base.init.bit_rate.secondary_data = CAN_BASE_BIT_RATE_DATA_DEFAULT;

  can_base.init.id.request_broadcast = CAN_BASE_ID_REQUEST_BROADCAST_DEFAULT;
  can_base.init.id.block_offset = CAN_BASE_BLOCK_OFFSET_DEFAULT;

  can_base.runtime.application_transmissions_are_allowed = true;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Callback routine used for configuration accesses not covered by simple read/write via pointers.
 *
 -----------------------------------------------------------------------------*/
static CONFIG_CALLBACK(CanBaseConfigCallback)
{
  e_ERROR_ID result = ERROR_ID_SUCCESS;

  switch (config_id) {

    case CONFIG_ID_CAN_BIT_RATE_PRIMARY_NOMINAL_STORED:
    case CONFIG_ID_CAN_BIT_RATE_PRIMARY_DATA_STORED:
    case CONFIG_ID_CAN_BIT_RATE_SECONDARY_NOMINAL_STORED:
    case CONFIG_ID_CAN_BIT_RATE_SECONDARY_DATA_STORED:

      if ((callback_type == CONFIG_CALLBACK_TYPE_PRE) && !read) {

        const uint16_t bitrate_in_kbps = *(uint16_t *)data;

        const bool for_secondary_can =
            (config_id == CONFIG_ID_CAN_BIT_RATE_SECONDARY_NOMINAL_STORED) ||
            (config_id == CONFIG_ID_CAN_BIT_RATE_SECONDARY_DATA_STORED);
        const bool is_nominal_bit_rate =
            (config_id == CONFIG_ID_CAN_BIT_RATE_PRIMARY_NOMINAL_STORED) ||
            (config_id == CONFIG_ID_CAN_BIT_RATE_SECONDARY_NOMINAL_STORED);
        const bool is_data_bit_rate = !is_nominal_bit_rate;
        const bool value_is_valid_for_nominal_bit_rate = CAN_SUPPORTS_BITRATE(false, bitrate_in_kbps);
        const bool value_is_valid_for_data_bit_rate = CAN_SUPPORTS_BITRATE(true, bitrate_in_kbps);
        const bool value_is_out_of_range =
            (is_nominal_bit_rate && !value_is_valid_for_nominal_bit_rate) ||
            (is_data_bit_rate && !value_is_valid_for_data_bit_rate);
        const bool node_is_not_supported = for_secondary_can && (CAN_GET_NUMBER_OF_NODES() < 2);
        const bool fd_is_not_supported = is_data_bit_rate && !CAN_SUPPORTS_FD();

        result =
            (node_is_not_supported || fd_is_not_supported) ? ERROR_ID_UNKNOWN_VALUE :
                value_is_out_of_range ? ERROR_ID_VALUE_OUT_OF_RANGE :
                    result;
      }

      break;

    case CONFIG_ID_CAN_ID_REQUEST_BROADCAST_STORED:
    case CONFIG_ID_CAN_ID_BLOCK_OFFSET_STORED:

      if ((callback_type == CONFIG_CALLBACK_TYPE_PRE) && !read) {
        const uint16_t id = *(uint16_t *)data;
        const uint16_t id_request_broadcast = (config_id == CONFIG_ID_CAN_ID_REQUEST_BROADCAST_STORED) ? id : can_base.init.id.request_broadcast;
        const uint16_t id_block_offset = (config_id == CONFIG_ID_CAN_ID_BLOCK_OFFSET_STORED) ? id : can_base.init.id.block_offset;
        if (CanBaseCalculateFirstModuleIdentifier(id_request_broadcast, id_block_offset) == CAN_MESSAGE_ID_INVALID)
          result = ERROR_ID_VALUE_OUT_OF_RANGE;
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
 * Calculate the first module identifier if settings are allowed in this combination.
 *
 -----------------------------------------------------------------------------*/
static uint16_t CanBaseCalculateFirstModuleIdentifier(const uint16_t id_request_broadcast, const uint16_t id_block_offset)
{
  uint16_t identifier = CAN_MESSAGE_ID_INVALID;

  const uint16_t identifier_per_module = CAN_BASE_NUMBER_OF_IDENTIFIER_PER_MODULE;
  const uint8_t module_number = MODULE_NUMBER_GET();
  const bool valid = (id_request_broadcast != CAN_MESSAGE_ID_INVALID) && (id_block_offset > 0) && (module_number > 0) && (identifier_per_module <= 0x102);

  if (valid) {
    const uint32_t first_id = (uint32_t)id_request_broadcast + (uint32_t)id_block_offset + (uint32_t)identifier_per_module * (module_number - 1);
    const uint32_t last_id = first_id + identifier_per_module - 1;
    const bool fits = last_id <= CAN_MESSAGE_ID_LAST;
    if (fits)
      identifier = (uint16_t)first_id;
  }

  return identifier;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Update CAN IDs.
 *
 -----------------------------------------------------------------------------*/
void CanBaseUpdateIdentifier(void)
{
  CanSetAcceptanceFilter(CAN_BASE_NODE_PRIMARY, 0, can_base.init.id.request_broadcast, 3);
  can_base.runtime.id.request_broadcast = can_base.init.id.request_broadcast;

  const uint16_t id_request_singlecast = CanBaseCalculateFirstModuleIdentifier(can_base.init.id.request_broadcast, can_base.init.id.block_offset);
  if (id_request_singlecast != CAN_MESSAGE_ID_INVALID) {
    can_base.runtime.id.request_singlecast = id_request_singlecast;
    can_base.runtime.id.response = id_request_singlecast + 1;
    can_base.runtime.id.first_application = id_request_singlecast + 2;
    CanSetAcceptanceFilter(CAN_BASE_NODE_PRIMARY, 1, can_base.runtime.id.request_singlecast, 3);

    if (CAN_BASE_RECEIVE_APPLICATION_IDENTIFIER) {
      const uint8_t number_of_application_identifier = CAN_BASE_NUMBER_OF_APPLICATION_IDENTIFIER;
      for (uint8_t number = 0; number < number_of_application_identifier; number++) {
        const uint8_t filter = 2 + number;
        const uint16_t identifier = can_base.runtime.id.first_application + number;
        CanSetAcceptanceFilter(CAN_BASE_NODE_PRIMARY, filter, identifier, 4);
      }
    }
    else if (CAN_BASE_RECEIVE_OTHER_IDENTIFIER) {
      CanSetAcceptanceFilter(CAN_BASE_NODE_PRIMARY, 2, 1, 5);
    }

    can_base.init.has_valid_identifier = true;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void CanBaseAllowApplicationTransmissions(const bool allow)
{
  can_base.runtime.application_transmissions_are_allowed = allow;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool CanBaseReceiveRequest(t_CAN_MESSAGE * const message, bool * const broadcast)
{
  ASSERT_FATAL(message != NULL);
  ASSERT_FATAL(broadcast != NULL);

  bool new_message = CanReceiveMessage(CAN_BASE_NODE_PRIMARY, CAN_BASE_FIFO_RECEIVE_REQUEST, message);
  if (new_message) {
    if (message->identifier == can_base.runtime.id.request_broadcast)
      *broadcast = true;
    else if (message->identifier == can_base.runtime.id.request_singlecast)
      *broadcast = false;
    else
      new_message = false; // prevent processing messages if acceptance filter is behaving wrong
  }

  return new_message;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool CanBaseTransmitResponse(t_CAN_MESSAGE * const message, const bool suppress_error_entry)
{
  ASSERT_FATAL(message != NULL);

  bool message_handled = false;

  if (can_base.init.has_valid_identifier) {

    message->identifier = can_base.runtime.id.response;
    message_handled = CanTransmitMessage(CAN_BASE_NODE_PRIMARY, CAN_BASE_FIFO_TRANSMIT_RESPONSE, message);
    if (!message_handled && !suppress_error_entry)
      ErrorAddEntry(ERROR_ID_CAN_TX_QUEUE_OVERRUN, 0x10);
  }

  return message_handled;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool CanBaseReceiveApplication(t_CAN_MESSAGE * const message)
{
  ASSERT_FATAL(message != NULL);

  bool new_message = CanReceiveMessage(CAN_BASE_NODE_PRIMARY, CAN_BASE_FIFO_RECEIVE_APPLICATION, message);
  if (new_message) {

    if ((message->identifier >= can_base.runtime.id.first_application) &&
        (message->identifier < (can_base.runtime.id.first_application + CAN_BASE_NUMBER_OF_APPLICATION_IDENTIFIER)))
      message->identifier -= can_base.runtime.id.first_application;
    else
      new_message = false; // prevent processing messages if acceptance filter is behaving wrong
  }

  return new_message;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool CanBaseTransmitApplication(t_CAN_MESSAGE * const message, const bool suppress_error_entry)
{
  ASSERT_FATAL(message != NULL);

  bool message_handled = false;

  const uint16_t number_of_application_identifier = CAN_BASE_NUMBER_OF_APPLICATION_IDENTIFIER;

  const bool possible =
      can_base.init.has_valid_identifier &&
      can_base.runtime.application_transmissions_are_allowed &&
      (message->identifier < number_of_application_identifier);
  if (possible) {

    message->identifier += can_base.runtime.id.first_application;
    message_handled = CanTransmitMessage(CAN_BASE_NODE_PRIMARY, CAN_BASE_FIFO_TRANSMIT_APPLICATION, message);
    message->identifier -= can_base.runtime.id.first_application; // IME: core - dsPIC33C - can - better solution?
    if (!message_handled && !suppress_error_entry)
      ErrorAddEntry(ERROR_ID_CAN_TX_QUEUE_OVERRUN, 0x11);
  }

  return message_handled;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
