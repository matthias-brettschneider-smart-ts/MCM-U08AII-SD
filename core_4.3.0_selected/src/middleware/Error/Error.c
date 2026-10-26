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

#include "hal/IInterrupt.h"
#include "hal/ISemaphore.h"
#include "middleware/IAssert.h"
#include "middleware/IConfig.h"
#include "middleware/IDefault.h"
#include "middleware/IError.h"
#include "middleware/IErrorInternal.h"
#include "middleware/IGeneric.h"
#include "middleware/IMemory.h"
#include "middleware/IStorage.h"
#include "os/IBlink.h"
#include "os/ILed.h"
#include "os/IProtocol.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define ERROR_VERBOSE_LEVEL_DEFAULT                                            1

typedef struct {
  uint8_t id;
  uint8_t category;
} t_ERROR_ASSIGNMENT;

typedef struct {
  uint8_t unread : 1;
  uint8_t occurrence_count : 7;
  uint8_t attribute;
  uint16_t occurence_order;
} t_ERROR_ENTRY;

typedef struct {
  uint8_t verbose_level;
  uint16_t occurence_order;
  t_SEMAPHORE_MUTEX write_mutex;
  t_ERROR_ENTRY log[ERROR_ID_LAST - ERROR_ID_ZERO];
} t_ERROR;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static const t_ERROR_ASSIGNMENT error_assignment[] = {
    { .id = ERROR_ID_ZERO,                                              .category = ERROR_CATEGORY_INVALID     },
    { .id = ERROR_ID_FATAL_INVALID_MODULE_NUMBER,                       .category = ERROR_CATEGORY_FATAL_ERROR },
    { .id = ERROR_ID_FATAL_RESERVED02,                                  .category = ERROR_CATEGORY_FATAL_ERROR },
    { .id = ERROR_ID_FATAL_RESERVED03,                                  .category = ERROR_CATEGORY_FATAL_ERROR },
    { .id = ERROR_ID_FATAL_RESERVED04,                                  .category = ERROR_CATEGORY_FATAL_ERROR },
    { .id = ERROR_ID_FATAL_RESERVED05,                                  .category = ERROR_CATEGORY_FATAL_ERROR },
    { .id = ERROR_ID_FATAL_RESERVED06,                                  .category = ERROR_CATEGORY_FATAL_ERROR },
    { .id = ERROR_ID_FATAL_RESERVED07,                                  .category = ERROR_CATEGORY_FATAL_ERROR },
    { .id = ERROR_ID_FATAL_CLOCK_IO,                                    .category = ERROR_CATEGORY_FATAL_ERROR },
    { .id = ERROR_ID_FATAL_STORAGE_MISCONFIGURED,                       .category = ERROR_CATEGORY_FATAL_ERROR },
    { .id = ERROR_ID_FATAL_FLASHLOADER_CORRUPT,                         .category = ERROR_CATEGORY_FATAL_ERROR },
    { .id = ERROR_ID_FATAL_STARTUP_TOO_LONG,                            .category = ERROR_CATEGORY_FATAL_ERROR },
    { .id = ERROR_ID_FATAL_I2C,                                         .category = ERROR_CATEGORY_FATAL_ERROR },
    { .id = ERROR_ID_FATAL_INVALID_DEVICE,                              .category = ERROR_CATEGORY_FATAL_ERROR },
    { .id = ERROR_ID_FATAL_ENDLESS_LOOP,                                .category = ERROR_CATEGORY_FATAL_ERROR },
    { .id = ERROR_ID_FATAL_ASSERT,                                      .category = ERROR_CATEGORY_FATAL_ERROR },
    { .id = ERROR_ID_STORAGE_ACCESS_FAILED_CORE_IDENTIFICATION,         .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_STORAGE_ACCESS_FAILED_CORE_SETTINGS,               .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_STORAGE_ACCESS_FAILED_MODULE_SETTINGS,             .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_STORAGE_ACCESS_FAILED_MODULE_ADJUSTMENT_SMART,     .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_STORAGE_ACCESS_FAILED_MODULE_ADJUSTMENT_CUSTOMER,  .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_EXECUTION_TIME_EXCEEDED,                           .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_CLOCK_SPIKES,                                      .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_CLOCK_MISSING,                                     .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_CLOCK_UNEXPECTED,                                  .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_CAN_TX_QUEUE_OVERRUN,                              .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_CAN_RX_BUFFER_OVERRUN,                             .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_DEFAULT_REGISTRATION,                              .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_STORAGE_REGISTRATION,                              .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_CONFIG_REGISTRATION,                               .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_STORAGE_READ_VALUE,                                .category = ERROR_CATEGORY_WARNING     },
    { .id = ERROR_ID_STORAGE_WRITE_VALUE,                               .category = ERROR_CATEGORY_WARNING     },
    { .id = ERROR_ID_STARTUP_DETERMINATION,                             .category = ERROR_CATEGORY_WARNING     },
    { .id = ERROR_ID_ADJUSTMENT_VALUE_OUT_OF_RANGE,                     .category = ERROR_CATEGORY_WARNING     },
    { .id = ERROR_ID_UNKNOWN_DEVICE_REVISION,                           .category = ERROR_CATEGORY_WARNING     },
    { .id = ERROR_ID_PHYSICAL,                                          .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_UPTIME_CORRUPT,                                    .category = ERROR_CATEGORY_WARNING     },
    { .id = ERROR_ID_APPLICATION_WARNING,                               .category = ERROR_CATEGORY_WARNING     },
    { .id = ERROR_ID_APPLICATION_ERROR,                                 .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_ERROR_HANDLING,                                    .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_NO_FLASHLOADER,                                    .category = ERROR_CATEGORY_ERROR       },
    { .id = ERROR_ID_LAST,                                              .category = ERROR_CATEGORY_INVALID     }
};

static volatile t_ERROR error;

static const t_STORAGE_ENTRY error_storage[] = {
    { .storage_id = STORAGE_ID_ERROR_VERBOSE_LEVEL, .storage_block = STORAGE_BLOCK_CORE_SETTINGS, .data_pointer = (void*)&error.verbose_level, .size = sizeof(error.verbose_level) },
    { .storage_id = STORAGE_ID_UNDEFINED,           .storage_block = STORAGE_BLOCK_UNDEFINED,     .data_pointer = NULL,                        .size = 0                           }
};

static const t_CONFIG error_config[] = {
    { .config_id = CONFIG_ID_ERROR_ENTRY,         .config_type = CONFIG_TYPE_UINT32,  .elements = 1, .readable = true,  .writeable = false, .data_pointer = NULL                        },
    { .config_id = CONFIG_ID_ERROR_VERBOSE_LEVEL, .config_type = CONFIG_TYPE_UINT8,   .elements = 1, .readable = true,  .writeable = true,  .data_pointer = (void*)&error.verbose_level },
    { .config_id = CONFIG_ID_ADD_ERROR_ENTRY,     .config_type = CONFIG_TYPE_UINT16,  .elements = 1, .readable = false, .writeable = true,  .data_pointer = NULL                        },
    { .config_id = CONFIG_ID_INVALID,             .config_type = CONFIG_TYPE_UNKNOWN, .elements = 0, .readable = false, .writeable = false, .data_pointer = NULL                        }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static DEFAULT_CALLBACK(ErrorDefaultCallback);
static CONFIG_CALLBACK(ErrorConfigCallback);

COMPILER_INLINE int ERROR_GET_NUMBER_OF_IDS(void);
COMPILER_INLINE bool ERROR_CAN_BE_HANDLED(const e_ERROR_ID id);
COMPILER_INLINE int ERROR_GET_INTERNAL_INDEX(const e_ERROR_ID id);
COMPILER_INLINE e_ERROR_ID ERROR_GET_ID(const int index);
COMPILER_INLINE e_ERROR_CATEGORY ERROR_GET_CATEGORY(const e_ERROR_ID id);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module (1/2)
 *
 -----------------------------------------------------------------------------*/
void ErrorInitFirst(void)
{
  ASSERT_STATIC(sizeof(t_ERROR_ASSIGNMENT) == 2);
  ASSERT_STATIC(sizeof(t_ERROR_ENTRY) == 4);
  ASSERT_STATIC(sizeof(t_ERROR_SUMMARY) == 4);

  SEMAPHORE_RELEASE_MUTEX(&error.write_mutex);
  error.verbose_level = ERROR_VERBOSE_LEVEL_DEFAULT;

  // check all assignments for incorrect index numbers
  const size_t number_of_entries = sizeof(error_assignment) / sizeof(t_ERROR_ASSIGNMENT);
  for (size_t index = 0; index < number_of_entries; index++)
    ASSERT_FATAL(error_assignment[index].id == (ERROR_ID_ZERO + (e_ERROR_ID)index));

  ConfigRegister(error_config, ErrorConfigCallback);

  DefaultRegister(ErrorDefaultCallback);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module (2/2).
 *
 * This can not be done within the regular module initialization because storage is not available at this time.
 *
 -----------------------------------------------------------------------------*/
void ErrorInitSecond(void)
{
  StorageRegister(error_storage);
  StorageLoad(error_storage);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Callback routine used for factory reset.
 *
 -----------------------------------------------------------------------------*/
static DEFAULT_CALLBACK(ErrorDefaultCallback)
{
  error.verbose_level = ERROR_VERBOSE_LEVEL_DEFAULT;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Callback routine used for configuration accesses not covered by simple read/write via pointers.
 *
 -----------------------------------------------------------------------------*/
static CONFIG_CALLBACK(ErrorConfigCallback)
{
  e_ERROR_ID result = ERROR_ID_SUCCESS;

  switch (config_id) {

    case CONFIG_ID_ERROR_VERBOSE_LEVEL:

      if ((callback_type == CONFIG_CALLBACK_TYPE_PRE) && !read) {
        const uint8_t verbose_level = *(uint8_t *)data;
        if (verbose_level > 2)
          result = ERROR_ID_VALUE_OUT_OF_RANGE;
      }

      break;

    case CONFIG_ID_ERROR_ENTRY:

      if (callback_type == CONFIG_CALLBACK_TYPE_PRE) {
        t_ERROR_SUMMARY summary;
        ErrorGetOldestEntry(&summary);
        *(uint32_t *)data = MEMORY_SWAP_UINT32(summary.value);
      }

      break;

    case CONFIG_ID_ADD_ERROR_ENTRY:

      if (callback_type == CONFIG_CALLBACK_TYPE_PRE) {
        const uint8_t * value = (uint8_t *)data;
        const e_ERROR_ID id = (e_ERROR_ID)value[1];
        if (!ERROR_CAN_BE_HANDLED(id))
          result = ERROR_ID_VALUE_OUT_OF_RANGE;
        else {
          const uint8_t attribute = value[0];
          ErrorAddEntry(id, attribute);
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
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE int ERROR_GET_NUMBER_OF_IDS(void)
{
  return (int)ERROR_ID_LAST - (int)ERROR_ID_ZERO;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool ERROR_CAN_BE_HANDLED(const e_ERROR_ID id)
{
  return (id > ERROR_ID_ZERO) && (id < ERROR_ID_LAST);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE int ERROR_GET_INTERNAL_INDEX(const e_ERROR_ID id)
{
  return (int)id - (int)ERROR_ID_ZERO;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE e_ERROR_ID ERROR_GET_ID(const int index)
{
  return (e_ERROR_ID)(index + (int)ERROR_ID_ZERO);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Determine the category of a particular error ID.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE e_ERROR_CATEGORY ERROR_GET_CATEGORY(const e_ERROR_ID id)
{
  return ERROR_CAN_BE_HANDLED(id) ? (e_ERROR_CATEGORY)error_assignment[id - ERROR_ID_ZERO].category : ERROR_CATEGORY_NO_ERROR;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Add a new entry to the error log (or just increment the error counter if this particular error has already been added).
 *
 * Routine can be called from interrupt context, but interleaving calls may get lost.
 *
 -----------------------------------------------------------------------------*/
void ErrorAddEntry(e_ERROR_ID id, const uint8_t attribute)
{
  const e_ERROR_ID id_to_add = ERROR_CAN_BE_HANDLED(id) ? id : ERROR_ID_ERROR_HANDLING;

  const e_ERROR_CATEGORY category = ERROR_GET_CATEGORY(id_to_add);
  const int index = ERROR_GET_INTERNAL_INDEX(id_to_add);

  if (category == ERROR_CATEGORY_FATAL_ERROR) {

    // just handle the LED till the end of days
    INTERRUPT_DISABLE();
    while (1)
      BlinkUpdate(0, true, (uint16_t)index);
  }
  else {

    uint8_t occurance_count_within_mutex = 0;

    if (SEMAPHORE_TRY_TO_LOCK_MUTEX(&error.write_mutex)) {

      const bool new_or_read = error.log[index].unread == 0;
      if (new_or_read) {
        error.log[index].occurrence_count = 0;
        error.log[index].occurence_order = error.occurence_order;
        error.log[index].attribute = attribute;
        error.log[index].unread = 1;
        error.occurence_order++; // warp-around allowed
      }

      const bool limit_not_reached = error.log[index].occurrence_count < 0x7F;
      if (limit_not_reached)
        error.log[index].occurrence_count++;

      occurance_count_within_mutex = error.log[index].occurrence_count;

      SEMAPHORE_RELEASE_MUTEX(&error.write_mutex);
    }

    const bool send_error_message = ((error.verbose_level == 1) && (occurance_count_within_mutex == 1)) || (error.verbose_level == 2);
    if (send_error_message)
      ProtocolSendError(category, id_to_add, occurance_count_within_mutex, attribute);
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Determine category of the highest entry that is active at the moment.
 *
 * (Determining value each time it is required is easier than maintaining state.)
 *
 -----------------------------------------------------------------------------*/
e_ERROR_CATEGORY ErrorGetHighestCategory(void)
{
  e_ERROR_CATEGORY highest_category = ERROR_CATEGORY_NO_ERROR;

  for (int id_internal = 0; id_internal < ERROR_GET_NUMBER_OF_IDS(); id_internal++)
    if (error.log[id_internal].unread == 1)
      if ((e_ERROR_CATEGORY)error_assignment[id_internal].category > highest_category)
        highest_category = (e_ERROR_CATEGORY)error_assignment[id_internal].category;

  return highest_category;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Provide summary for oldest, unread entry and remove it from the log.
 *
 -----------------------------------------------------------------------------*/
void ErrorGetOldestEntry(t_ERROR_SUMMARY * const summary)
{
  ASSERT_FATAL(summary != NULL);

  // get current counter value once to have a common reference for all entries -> future entries will be ignored
  const uint16_t occurence_order = error.occurence_order;

  uint16_t highest_age = 0;
  int oldest_index = 0;
  for (int index = 0; index < ERROR_GET_NUMBER_OF_IDS(); index++) {
    if (error.log[index].unread == 1) {

      // get real age of current entry (handles problems with meanwhile incremented error_occurence_order)
      uint16_t current_age = occurence_order - error.log[index].occurence_order;
      if (current_age > 0x8000u)
        current_age = 0;

      const bool current_is_older = current_age > highest_age;
      if (current_is_older) {
        highest_age = current_age;
        oldest_index = index;
      }
    }
  }

  const bool empty = oldest_index == 0;
  if (empty)
    summary->value = 0;
  else {
    const e_ERROR_ID id = ERROR_GET_ID(oldest_index);
    summary->category = (uint8_t)ERROR_GET_CATEGORY(id);
    summary->id = (uint8_t)id;
    summary->occurrence_count = error.log[oldest_index].occurrence_count;
    summary->attribute = error.log[oldest_index].attribute;
    error.log[oldest_index].unread = 0;
  }
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
