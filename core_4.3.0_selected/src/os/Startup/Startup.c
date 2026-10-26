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
#include "hal/IDsp.h"
#include "hal/IGpio.h"
#include "hal/IPinning.h"
#include "middleware/IConfig.h"
#include "middleware/IDebug.h"
#include "middleware/IDefault.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"
#include "middleware/IModuleNumber.h"
#include "middleware/ISecurity.h"
#include "middleware/IStorage.h"
#include "middleware/IWait.h"
#include "os/IClock.h"
#include "os/ISlotDetection.h"
#include "os/IStartup.h"
#include "project/IClockCommon.h"

#ifndef STARTUP_DETERMINE_MODULE_NUMBER_VIA_SLOT_NUMBER
#define STARTUP_DETERMINE_MODULE_NUMBER_VIA_SLOT_NUMBER                       false
#elif ((STARTUP_DETERMINE_MODULE_NUMBER_VIA_SLOT_NUMBER != false) && (STARTUP_DETERMINE_MODULE_NUMBER_VIA_SLOT_NUMBER != true))
#error "error compile flags - STARTUP_DETERMINE_MODULE_NUMBER_VIA_SLOT_NUMBER invalid"
#endif

#ifndef STARTUP_DETERMINE_MODULE_NUMBER_VIA_AUTO_ENUMERATION
#define STARTUP_DETERMINE_MODULE_NUMBER_VIA_AUTO_ENUMERATION                  false
#elif ((STARTUP_DETERMINE_MODULE_NUMBER_VIA_AUTO_ENUMERATION != false) && (STARTUP_DETERMINE_MODULE_NUMBER_VIA_AUTO_ENUMERATION != true))
#error "error compile flags - STARTUP_DETERMINE_MODULE_NUMBER_VIA_AUTO_ENUMERATION invalid"
#endif

#if (STARTUP_DETERMINE_MODULE_NUMBER_VIA_SLOT_NUMBER && STARTUP_DETERMINE_MODULE_NUMBER_VIA_AUTO_ENUMERATION)
#error "error compile flags - do not set STARTUP_DETERMINE_MODULE_NUMBER_VIA_SLOT_NUMBER and STARTUP_DETERMINE_MODULE_NUMBER_VIA_AUTO_ENUMERATION together"
#endif

#if (!STARTUP_DETERMINE_MODULE_NUMBER_VIA_SLOT_NUMBER && !STARTUP_DETERMINE_MODULE_NUMBER_VIA_AUTO_ENUMERATION)
#ifndef STARTUP_FIX_MODULE_NUMBER
#error "error compile flags - configure STARTUP_FIX_MODULE_NUMBER"
#endif
#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef enum {
  STARTUP_DETERMINATION_AUTO                                                 = 0,
  STARTUP_DETERMINATION_STORED_WITH_CHECK                                    = 1,
  STARTUP_DETERMINATION_STORED_WITHOUT_CHECK                                 = 2,
  STARTUP_DETERMINATION_OFF                                                  = 3,
  STARTUP_DETERMINATION_INVALID                                              = 4
} e_STARTUP_DETERMINATION;

typedef struct {
  e_STARTUP_DETERMINATION startup_determination;
  uint8_t module_number_determined;
  uint8_t module_number_stored;
  bool clock_master_determined;
  bool clock_master_stored;
  bool clock_master;
  uint32_t start_of_synchronization_pulse;
} t_STARTUP;

// default
#define STARTUP_DETERMINATION_DEFAULT                 STARTUP_DETERMINATION_AUTO
#define STARTUP_MODULE_NUMBER_DEFAULT                      MODULE_NUMBER_INVALID
#define STARTUP_CLOCK_MASTER_DEFAULT                                       false

// startup pulse
#define STARTUP_PULSE_DURATION_PER_MODULE_IN_US                               50
#define STARTUP_PULSE_DURATION_FIRST_INTELLIPROBE_MODULE_IN_US                25
#define STARTUP_PULSE_DURATION_OTHER_INTELLIPROBE_MODULES_IN_US               10
#define STARTUP_PULSE_WAIT_FOR_ACTIVE_IN_US                                    3
#define STARTUP_PULSE_WAIT_FOR_INACTIVE_IN_US                                 10

// configuration signal
#define STARTUP_CONFIG_SIGNAL_DURATION_PER_MODULE_IN_US                       50
#define STARTUP_CONFIG_SIGNAL_DURATION_ROUNDING_RESERVE_IN_US                 15
#define STARTUP_CONFIG_SIGNAL_MAXIMUM_WAIT_TIME_IN_US                      10000

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_STARTUP startup = {
    .startup_determination = STARTUP_DETERMINATION_DEFAULT,
    .module_number_determined = STARTUP_MODULE_NUMBER_DEFAULT,
    .module_number_stored = STARTUP_MODULE_NUMBER_DEFAULT,
    .clock_master_determined = STARTUP_CLOCK_MASTER_DEFAULT,
    .clock_master_stored = STARTUP_CLOCK_MASTER_DEFAULT,
    .clock_master = STARTUP_CLOCK_MASTER_DEFAULT,
    .start_of_synchronization_pulse = 0
};

uint8_t startup_module_number; // variable can not be defined static because it is used by inline functions (but NOT exported beyond the interface)

static const t_STORAGE_ENTRY startup_storage[] = {
    { .storage_id = STORAGE_ID_CLOCK_MASTER_STORED,   .storage_block = STORAGE_BLOCK_CORE_SETTINGS, .data_pointer = (void*)&startup.clock_master_stored,   .size = sizeof(startup.clock_master_stored)   },
    { .storage_id = STORAGE_ID_MODULE_NUMBER_STORED,  .storage_block = STORAGE_BLOCK_CORE_SETTINGS, .data_pointer = (void*)&startup.module_number_stored,  .size = sizeof(startup.module_number_stored)  },
    { .storage_id = STORAGE_ID_STARTUP_DETERMINATION, .storage_block = STORAGE_BLOCK_CORE_SETTINGS, .data_pointer = (void*)&startup.startup_determination, .size = sizeof(startup.startup_determination) },
    { .storage_id = STORAGE_ID_UNDEFINED,             .storage_block = STORAGE_BLOCK_UNDEFINED,     .data_pointer = NULL,                                  .size = 0                                     }
};

static const t_CONFIG startup_config[] = {
    { .config_id = CONFIG_ID_CLOCK_MASTER_ACTIVE,                .config_type = CONFIG_TYPE_BOOL,    .elements = 1, .readable = true,  .writeable = true,  .data_pointer = (void*)&startup.clock_master             },
    { .config_id = CONFIG_ID_CLOCK_MASTER_DETERMINED,            .config_type = CONFIG_TYPE_BOOL,    .elements = 1, .readable = true,  .writeable = false, .data_pointer = (void*)&startup.clock_master_determined  },
    { .config_id = CONFIG_ID_CLOCK_MASTER_STORED,                .config_type = CONFIG_TYPE_BOOL,    .elements = 1, .readable = true,  .writeable = true,  .data_pointer = (void*)&startup.clock_master_stored      },
    { .config_id = CONFIG_ID_MODULE_NUMBER_ACTIVE,               .config_type = CONFIG_TYPE_UINT8,   .elements = 1, .readable = true,  .writeable = false, .data_pointer = (void*)&startup_module_number            },
    { .config_id = CONFIG_ID_MODULE_NUMBER_DETERMINED,           .config_type = CONFIG_TYPE_UINT8,   .elements = 1, .readable = true,  .writeable = false, .data_pointer = (void*)&startup.module_number_determined },
    { .config_id = CONFIG_ID_MODULE_NUMBER_STORED,               .config_type = CONFIG_TYPE_UINT8,   .elements = 1, .readable = true,  .writeable = true,  .data_pointer = (void*)&startup.module_number_stored     },
    { .config_id = CONFIG_ID_STARTUP_DETERMINATION,              .config_type = CONFIG_TYPE_UINT8,   .elements = 1, .readable = true,  .writeable = true,  .data_pointer = NULL                                     },
    { .config_id = CONFIG_ID_SET_MODULE_NUMBER_AND_CLOCK_MASTER, .config_type = CONFIG_TYPE_BOOL,    .elements = 1, .readable = false, .writeable = true,  .data_pointer = NULL                                     },
    { .config_id = CONFIG_ID_INVALID,                            .config_type = CONFIG_TYPE_UNKNOWN, .elements = 0, .readable = false, .writeable = false, .data_pointer = NULL                                     }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static DEFAULT_CALLBACK(StartupDefaultCallback);
static CONFIG_CALLBACK(StartupConfigCallback);

COMPILER_INLINE uint16_t STARTUP_DETERMINE_STARTUP_PULSE_WITH(void);

COMPILER_INLINE void STARTUP_SLOT_DETECTION(void);
COMPILER_INLINE void STARTUP_AUTO_ENUMERATION(void);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
void StartupInit(void)
{
  startup_module_number = STARTUP_MODULE_NUMBER_DEFAULT;

  DefaultRegister(StartupDefaultCallback);

  StorageRegister(startup_storage);
  StorageLoad(startup_storage);

  ConfigRegister(startup_config, StartupConfigCallback);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Callback routine used for factory reset.
 *
 -----------------------------------------------------------------------------*/
static DEFAULT_CALLBACK(StartupDefaultCallback)
{
  if (SecurityGetCurrentLevel() >= SECURITY_LEVEL_MAINTENANCE) {
    startup.startup_determination = STARTUP_DETERMINATION_DEFAULT;
    startup.module_number_stored = STARTUP_MODULE_NUMBER_DEFAULT;
    startup.clock_master_stored = STARTUP_CLOCK_MASTER_DEFAULT;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Callback routine used for configuration accesses not covered by simple read/write via pointers.
 *
 -----------------------------------------------------------------------------*/
static CONFIG_CALLBACK(StartupConfigCallback)
{
  e_ERROR_ID result = ERROR_ID_SUCCESS;

  switch (config_id) {

    case CONFIG_ID_CLOCK_MASTER_ACTIVE:

      if (!read) {

        const bool active = *(bool *)data;

        if (callback_type == CONFIG_CALLBACK_TYPE_PRE)
          if (active)
            result = ERROR_ID_VALUE_OUT_OF_RANGE;

        if (callback_type == CONFIG_CALLBACK_TYPE_POST)
          ClockStopSignalGeneration();
      }

      break;

    case CONFIG_ID_MODULE_NUMBER_STORED:

      if ((callback_type == CONFIG_CALLBACK_TYPE_PRE) && !read)
        if (*(uint8_t *)data == MODULE_NUMBER_INVALID)
          result = ERROR_ID_VALUE_OUT_OF_RANGE;

      break;

    case CONFIG_ID_STARTUP_DETERMINATION:

      if (callback_type == CONFIG_CALLBACK_TYPE_POST) {
        if (read) {
          uint8_t * const startup_determination = (uint8_t *)data;
          *startup_determination = (uint8_t)startup.startup_determination;
        }
        else {
          const uint8_t startup_determination = *(uint8_t *)data;
          if (startup_determination >= STARTUP_DETERMINATION_INVALID)
            result = ERROR_ID_VALUE_OUT_OF_RANGE;
          else if ((startup_determination != STARTUP_DETERMINATION_AUTO) && (startup.module_number_stored == MODULE_NUMBER_INVALID))
            result = ERROR_ID_VALUE_INCOMPATIBLE;
          else
            startup.startup_determination = (e_STARTUP_DETERMINATION)startup_determination;
        }
      }

      break;

    case CONFIG_ID_SET_MODULE_NUMBER_AND_CLOCK_MASTER:

      if (callback_type == CONFIG_CALLBACK_TYPE_PRE) {
        const bool set = *(bool *)data;
        if (!set)
          result = ERROR_ID_VALUE_OUT_OF_RANGE;
        else if (startup.module_number_determined == MODULE_NUMBER_INVALID)
          result = ERROR_ID_VALUE_INCOMPATIBLE;
        else {
          startup.module_number_stored = startup.module_number_determined;
          startup.clock_master_stored = startup.clock_master_determined;
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
 * Determine target specific startup pulse width.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint16_t STARTUP_DETERMINE_STARTUP_PULSE_WITH(void)
{
  uint16_t pulse_width_in_us = 0;

#if (STARTUP_DETERMINE_MODULE_NUMBER_VIA_SLOT_NUMBER)
    pulse_width_in_us = startup.module_number_determined * STARTUP_PULSE_DURATION_PER_MODULE_IN_US;
#elif (STARTUP_DETERMINE_MODULE_NUMBER_VIA_AUTO_ENUMERATION)
    pulse_width_in_us = (GPIO_GET(PIN_CONFIG_IN) == GPIO_STATE_HIGH) ? STARTUP_PULSE_DURATION_FIRST_INTELLIPROBE_MODULE_IN_US : STARTUP_PULSE_DURATION_OTHER_INTELLIPROBE_MODULES_IN_US;
#else
    pulse_width_in_us = STARTUP_PULSE_DURATION_OTHER_INTELLIPROBE_MODULES_IN_US;
#endif

  return pulse_width_in_us;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void STARTUP_SLOT_DETECTION(void)
{
  const uint8_t slot_number = SlotDetectionGet();

  if (slot_number == SLOT_DETECTION_INVALID)
    ErrorAddEntry(ERROR_ID_FATAL_INVALID_MODULE_NUMBER, 0);
  else
    startup.module_number_determined = slot_number;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void STARTUP_AUTO_ENUMERATION(void)
{
#if defined(PIN_CONFIG_IN) && defined(PIN_CONFIG_OUT)

  const bool first_module_in_stack = GPIO_GET(PIN_CONFIG_IN) == GPIO_STATE_HIGH;
  if (first_module_in_stack)
    startup.module_number_determined = 1;
  else {

    // wait for receiving configuration signal from previous module or running into timeout
    // and calculate module number based on delay if we did not ran into timeout
    while ((GPIO_GET(PIN_CONFIG_IN) == GPIO_STATE_LOW) && !WAIT_CHECK_TIMEOUT_US_32(startup.start_of_synchronization_pulse, STARTUP_CONFIG_SIGNAL_MAXIMUM_WAIT_TIME_IN_US)) {;}
    uint32_t config_signal_delay = WAIT_GET_TIMER_VALUE_32() - startup.start_of_synchronization_pulse;
    if (config_signal_delay < WAIT_GET_TICKS_FOR_US_32(STARTUP_CONFIG_SIGNAL_MAXIMUM_WAIT_TIME_IN_US)) {
      config_signal_delay += WAIT_GET_TICKS_FOR_US_32(STARTUP_CONFIG_SIGNAL_DURATION_ROUNDING_RESERVE_IN_US);
      startup.module_number_determined = DSP_DIVIDE_UNSIGNED(config_signal_delay, WAIT_GET_TICKS_FOR_US_32(STARTUP_CONFIG_SIGNAL_DURATION_PER_MODULE_IN_US)) + 1;
    }
  }

  // wait for end of time depending on module number before activating configuration pin for next module
  if (startup.module_number_determined != MODULE_NUMBER_INVALID) {
    WAIT_US_32(startup.start_of_synchronization_pulse, startup.module_number_determined * STARTUP_CONFIG_SIGNAL_DURATION_PER_MODULE_IN_US);
    GPIO_SET(PIN_CONFIG_OUT, GPIO_STATE_HIGH);
  }

#endif
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void StartupSynchronize(const uint16_t time_till_startup_pulse_in_ms, const uint16_t compensation_delay_in_ms)
{
  // do startup synchronization if requested
  if (startup.startup_determination == STARTUP_DETERMINATION_OFF)
    startup.start_of_synchronization_pulse = WAIT_GET_TIMER_VALUE_32();
  else {

    if (STARTUP_DETERMINE_MODULE_NUMBER_VIA_SLOT_NUMBER)
      STARTUP_SLOT_DETECTION();

    // determine startup pulse width
    const uint16_t startup_pulse_width_in_us = STARTUP_DETERMINE_STARTUP_PULSE_WITH();

    // determine and monitor time till startup pulse
    if (WAIT_CHECK_TIMEOUT_MS_32(0, time_till_startup_pulse_in_ms))
      ErrorAddEntry(ERROR_ID_FATAL_STARTUP_TOO_LONG, 0);

    // synchronize all modules via first edge on CLOCK (wait for receiving first edge or timeout, set CLOCK active and remember time)
    // to get synchronization done correctly, clock must not be active already or stay inactive afterwards
    DEBUG_DEACTIVATE_DEBUG_PIN(MAIN_DEBUG_DO_TIMING_ANALYSES_STARTUP);
    if (CLOCK_COMMON_IS_ACTIVE())
      ErrorAddEntry(ERROR_ID_FATAL_CLOCK_IO, 0);
    while (!CLOCK_COMMON_IS_ACTIVE() && !WAIT_CHECK_TIMEOUT_MS_32(0, time_till_startup_pulse_in_ms + compensation_delay_in_ms)) {;}
    DEBUG_ACTIVATE_DEBUG_PIN(MAIN_DEBUG_DO_TIMING_ANALYSES_STARTUP);
    CLOCK_COMMON_SET_ACTIVE();
    startup.start_of_synchronization_pulse = WAIT_GET_TIMER_VALUE_32();
    WAIT_US(STARTUP_PULSE_WAIT_FOR_ACTIVE_IN_US);
    if (!CLOCK_COMMON_IS_ACTIVE())
      ErrorAddEntry(ERROR_ID_FATAL_CLOCK_IO, 0);

    // monitor startup pulse timing, do pulse duration and give pin time to become inactive again
    if (WAIT_CHECK_TIMEOUT_MS_32(startup.start_of_synchronization_pulse, startup_pulse_width_in_us > 0 ? startup_pulse_width_in_us - 1 : 0))
      ErrorAddEntry(ERROR_ID_FATAL_STARTUP_TOO_LONG, 0);
    WAIT_US_32(startup.start_of_synchronization_pulse, startup_pulse_width_in_us);
    CLOCK_COMMON_SET_INACTIVE();
    WAIT_US(STARTUP_PULSE_WAIT_FOR_INACTIVE_IN_US);

    // clock became inactive -> we may have to generate clock signal later
    startup.clock_master_determined = !CLOCK_COMMON_IS_ACTIVE();

    if (STARTUP_DETERMINE_MODULE_NUMBER_VIA_AUTO_ENUMERATION)
      STARTUP_AUTO_ENUMERATION();

#if (!STARTUP_DETERMINE_MODULE_NUMBER_VIA_SLOT_NUMBER && !STARTUP_DETERMINE_MODULE_NUMBER_VIA_AUTO_ENUMERATION)
      startup.module_number_determined = STARTUP_FIX_MODULE_NUMBER;
      WAIT_US_32(startup.start_of_synchronization_pulse, startup.module_number_determined * STARTUP_CONFIG_SIGNAL_DURATION_PER_MODULE_IN_US);
#endif
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Use correct module number and clock master settings and report inconsistency if requested.
 *
 -----------------------------------------------------------------------------*/
void StartupSetModuleNumberAndClockMaster(void)
{
  const bool use_determined_settings = startup.startup_determination == STARTUP_DETERMINATION_AUTO;
  startup_module_number = use_determined_settings ? startup.module_number_determined : startup.module_number_stored;
  startup.clock_master = use_determined_settings ? startup.clock_master_determined : startup.clock_master_stored;

  if (startup.startup_determination == STARTUP_DETERMINATION_STORED_WITH_CHECK) {
    if (startup.clock_master_determined != startup.clock_master_stored)
      ErrorAddEntry(ERROR_ID_STARTUP_DETERMINATION, 1);
    if (startup.module_number_determined != startup.module_number_stored)
      ErrorAddEntry(ERROR_ID_STARTUP_DETERMINATION, 2);
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool StartupPostSynchronizationExceeded(const uint32_t time_after_startup_pulse_in_ms)
{
  return WAIT_CHECK_TIMEOUT_MS_32(startup.start_of_synchronization_pulse, time_after_startup_pulse_in_ms);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool StartupTimeToStartClockGeneration(const uint32_t time_after_startup_pulse_in_ms)
{
  const bool waited = startup.clock_master;
  if (waited)
    WAIT_MS_32(startup.start_of_synchronization_pulse, time_after_startup_pulse_in_ms);

  return waited;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
