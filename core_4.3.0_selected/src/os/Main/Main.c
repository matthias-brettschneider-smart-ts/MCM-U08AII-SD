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

#include "hal/ICallApplication.h"
#include "hal/ICan.h" // IME: core - dsPIC33C - can - remove
#include "hal/IConfigurationBits.h"
#include "hal/IControllerIdentification.h"
#include "hal/IFlash.h"
#include "hal/IOscillator.h"
#include "hal/IReboot.h"
#include "middleware/ICanChannel.h"
#include "middleware/IDebug.h"
#include "middleware/IError.h"
#include "middleware/IErrorInternal.h"
#include "middleware/IGeneric.h"
#include "middleware/IModuleNumber.h"
#include "middleware/IOperation.h"
#include "middleware/IOperationInternal.h"
#include "middleware/IStorage.h"
#include "middleware/IStorageInternal.h"
#include "middleware/ISync.h"
#include "middleware/IWait.h"
#include "os/IBlink.h"
#include "os/IBlock.h"
#include "os/IBlockInfo.h"
#include "os/ICanBase.h"
#include "os/IClock.h"
#include "os/IInfo.h"
#include "os/IMain.h"
#include "os/IProtocol.h"
#include "os/ISeedKey.h"
#include "os/ISharedRam.h"
#include "os/IStackTest.h"
#include "os/IStartup.h"
#include "os/ISystemTimer.h"
#include "os/IUptime.h"
#include "project/IApplication.h"
#include "project/IHal.h"

#ifndef MAIN_FLASHLOADER
#error "error compile flags - MAIN_FLASHLOADER undefined"
#elif ((MAIN_FLASHLOADER != false) && (MAIN_FLASHLOADER != true))
#error "error compile flags - MAIN_FLASHLOADER invalid"
#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define MAIN_FLASHLOADER_ONLY                                   MAIN_FLASHLOADER
#define MAIN_APPLICATION_ONLY                                  !MAIN_FLASHLOADER

#define MAIN_STARTUP_TIME_FLASHLOADER_IN_MS                                  220
#define MAIN_STARTUP_TIME_APPLICATION_IN_MS                                  115
#define MAIN_STARTUP_TIME_COMPENSATION_DELAY_IN_MS                            50
#define MAIN_STARTUP_TIME_AFTER_STARTUP_PULSE_IN_MS                           25
#define MAIN_STARTUP_TIME_IN_RECOVERY_WINDOW_IN_MS                            30

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

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
 * Do startup behavior and execute background loop.
 *
 -----------------------------------------------------------------------------*/
int main(void)
{
  //----------------------------------------------------------------------------
  // count start and branch to application
  // (do not initialize any hardware before)
  //----------------------------------------------------------------------------

  // done early to catch as many unexpected reboots as possible
  SharedRamVariableInc(SHARED_RAM_VARIABLE_BOOT_COUNTER);

  // valid application detected at last startup so call it
  if (MAIN_FLASHLOADER_ONLY) {
    if (SharedRamFlagGet(SHARED_RAM_FLAG_APPLICATION_VALID)) {
      SharedRamFlagSet(SHARED_RAM_FLAG_APPLICATION_VALID, false);
      SharedRamFlagSet(SHARED_RAM_FLAG_PREVIOUS_FLASHLOADER, true);
      CallApplicationNow();
    }
  }

  //----------------------------------------------------------------------------
  // basic initialization of common used stuff
  // (do not do further things before or in between / think twice before changing)
  //----------------------------------------------------------------------------

  // very basic hardware initialization (use correct instruction clock, get time base and make LED available)
  OscillatorInit();
  SystemTimerInit();
  HalInitLed();

  // make error handler available (there is no CAN communication at this time, just the log itself and the LED if available)
  ErrorInitFirst();

  // check device id and revision (basic error handler has to be available already)
  ControllerIdentificationCheck();

  // initialize pins (device identification has to be done already)
  HalInitPins();
  DEBUG_ACTIVATE_DEBUG_PIN(MAIN_DEBUG_DO_TIMING_ANALYSES_STARTUP);

  // do self-check of flashloader
  if (MAIN_FLASHLOADER_ONLY)
    if (BlockCheckFlashloaderBlocks() != ERROR_ID_SUCCESS)
      ErrorAddEntry(ERROR_ID_FATAL_FLASHLOADER_CORRUPT, 0);

  // bring up storage system (pin initialization and basic error handler have to be available already)
  StorageInit();

  // finalize initialization of error handler (storage system has to be available already)
  ErrorInitSecond();

  //----------------------------------------------------------------------------
  // initialization
  //----------------------------------------------------------------------------

  // project specific hardware initialization
  HalInitOther();

  ConfigurationBitsEnsureConsideration();
  BlockInfoInit();
  StackTestInit();
  SharedRamInit();
  UptimeInit();
  InfoInit();
  CanBaseInit();
  SeedKeyInit();
  ProtocolInit();
  OperationInit();
  StartupInit();

  // project specific initialization
  if (MAIN_APPLICATION_ONLY)
    ApplInitOther();

  //----------------------------------------------------------------------------
  // recovery window
  //
  // handle incoming messages within defined time window
  // there are no responses because module number and therefore physical CAN IDs are not available
  //----------------------------------------------------------------------------

  const uint32_t entering_recovery_window_in_ticks = WAIT_GET_TIMER_VALUE_32();
  DEBUG_INTERRUPT_DEBUG_PIN(MAIN_DEBUG_DO_TIMING_ANALYSES_STARTUP);
  while (!WAIT_CHECK_TIMEOUT_MS_32(entering_recovery_window_in_ticks, MAIN_STARTUP_TIME_IN_RECOVERY_WINDOW_IN_MS))
    ProtocolHandler();
  DEBUG_INTERRUPT_DEBUG_PIN(MAIN_DEBUG_DO_TIMING_ANALYSES_STARTUP);

  //----------------------------------------------------------------------------
  // make reset (for branch to application) or stay in flashloader
  //----------------------------------------------------------------------------

  if (MAIN_FLASHLOADER_ONLY) {

    const bool stay_in_flashloader = SharedRamFlagGet(SHARED_RAM_FLAG_STAY_IN_FLASHLOADER);
    SharedRamFlagSet(SHARED_RAM_FLAG_STAY_IN_FLASHLOADER, false);
    if (!stay_in_flashloader) {

      const bool application_valid = BlockCheckApplicationBlocks() == ERROR_ID_SUCCESS;
      if (application_valid) {
        SharedRamFlagSet(SHARED_RAM_FLAG_APPLICATION_VALID, true);

        // wait till end of minimum time in flashloader and reboot
        if (WAIT_CHECK_TIMEOUT_MS_32(0, MAIN_STARTUP_TIME_FLASHLOADER_IN_MS))
          ErrorAddEntry(ERROR_ID_FATAL_STARTUP_TOO_LONG, 0);
        DEBUG_DEACTIVATE_DEBUG_PIN(MAIN_DEBUG_DO_TIMING_ANALYSES_STARTUP);
        WAIT_MS_32(0, MAIN_STARTUP_TIME_FLASHLOADER_IN_MS);
        RebootExecute();
      }
    }
  }

  const bool previous_flashloader = SharedRamFlagGet(SHARED_RAM_FLAG_PREVIOUS_FLASHLOADER);
  SharedRamFlagSet(SHARED_RAM_FLAG_PREVIOUS_FLASHLOADER, false);

  //----------------------------------------------------------------------------
  // startup synchronization, module number and clock master
  //----------------------------------------------------------------------------

  const uint16_t time_till_startup_pulse_in_ms =
      MAIN_STARTUP_TIME_APPLICATION_IN_MS +
      ((MAIN_FLASHLOADER_ONLY || !previous_flashloader) ? MAIN_STARTUP_TIME_FLASHLOADER_IN_MS : 0);
  StartupSynchronize(time_till_startup_pulse_in_ms, MAIN_STARTUP_TIME_COMPENSATION_DELAY_IN_MS);
  StartupSetModuleNumberAndClockMaster();

  if (MODULE_NUMBER_GET() == MODULE_NUMBER_INVALID)
    ErrorAddEntry(ERROR_ID_FATAL_INVALID_MODULE_NUMBER, 0);

  ClockStartSignalReception();

  //----------------------------------------------------------------------------
  // final steps till operation
  //----------------------------------------------------------------------------

  // set CAN IDs (module number is known now) and send welcome message
  CanBaseUpdateIdentifier();
  ProtocolSendWelcome();
  CanTrigger();

  if (MAIN_APPLICATION_ONLY) {
    if (!previous_flashloader)
      ErrorAddEntry(ERROR_ID_NO_FLASHLOADER, 0);
    ApplInitFinal();
    OperationApplyAutoOperation();
  }

  if (StartupPostSynchronizationExceeded(MAIN_STARTUP_TIME_AFTER_STARTUP_PULSE_IN_MS))
    ErrorAddEntry(ERROR_ID_FATAL_STARTUP_TOO_LONG, 0);

  DEBUG_DEACTIVATE_DEBUG_PIN(MAIN_DEBUG_DO_TIMING_ANALYSES_STARTUP);
  if (StartupTimeToStartClockGeneration(MAIN_STARTUP_TIME_AFTER_STARTUP_PULSE_IN_MS))
    ClockStartSignalGeneration();

  //----------------------------------------------------------------------------
  // background loop
  //----------------------------------------------------------------------------

  while (true) {

    BlinkUpdate(SYNC_GET_CYCLE_NUMBER(), false, 0);

    ProtocolHandler();

    CanTrigger();

    ClockCheckState();

    if (!ProtocolIsProgrammingInProgress()) {

      UptimeUpdate();

      if (MAIN_APPLICATION_ONLY)
        ApplOperateNonInterrupt(SYNC_GET_CYCLE_NUMBER());
    }
  }

  return 0;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
