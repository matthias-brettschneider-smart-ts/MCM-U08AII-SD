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
#include "middleware/IConfig.h"
#include "middleware/IDefault.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"
#include "middleware/IStorage.h"
#include "os/IBlink.h"
#include "os/IProtocol.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

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

// --- config ------------------------------------------------------------------

e_ERROR_ID ConfigRegister(const t_CONFIG config[], t_CONFIG_CALLBACK * const callback)
{
  (void)config;
  (void)callback;

  return ERROR_ID_SUCCESS;
}

// --- default -----------------------------------------------------------------

void DefaultRegister(t_DEFAULT_CALLBACK * callback)
{
  (void)callback;
}

// --- storage -----------------------------------------------------------------

void StorageRegister(const t_STORAGE_ENTRY entries[])
{
  (void)entries;
}

e_ERROR_ID StorageLoad(const t_STORAGE_ENTRY entries[])
{
  (void)entries;

  return ERROR_ID_SUCCESS;
}

// --- blink -------------------------------------------------------------------

void BlinkUpdate(const uint16_t cycle, const bool fatal_error_active, const uint16_t fatal_error_pulses)
{
  (void)cycle;
  (void)fatal_error_active;
  (void)fatal_error_pulses;
}

// --- protocol ----------------------------------------------------------------

void ProtocolSendError(const uint8_t category, const uint8_t id, const uint8_t occurance_count, const uint8_t attribute)
{
  (void)category;
  (void)id;
  (void)occurance_count;
  (void)attribute;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
