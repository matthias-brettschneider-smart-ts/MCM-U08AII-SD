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
#include "middleware/IOperation.h"
#include "middleware/IOperationInternal.h"
#include "middleware/ISecurity.h"
#include "middleware/ISecurityInternal.h"
#include "os/IMode.h"

/*
 * Mode is a wrapper for security and operation to support the SMART protocol:
 *
 * ----------------------------------+-----------------------------+-------------+
 * mode                              | security                    | operation   |
 * ----------------------------------+-----------------------------+-------------+
 * MODE_ID_ILLEGAL                   |                             | not running |
 * ----------------------------------+                             +-------------+
 * MODE_ID_OPERATION                 | SECURITY_LEVEL_ALL          | running     |
 * ----------------------------------+                             +-------------+
 * MODE_ID_CONFIG                    |                             |             |
 * ----------------------------------+-----------------------------+             +
 * MODE_ID_ADVANCED_CUSTOMER         | SECURITY_LEVEL_MAINTENANCE  | not running |
 * ----------------------------------+-----------------------------+             +
 * MODE_ID_ADVANCED_SMART            | SECURITY_LEVEL_MANUFACTURER |             |
 * ----------------------------------+-----------------------------+-------------+
 *
 */

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static const t_CONFIG mode_config[] = {
    { .config_id = CONFIG_ID_SEED,    .config_type = CONFIG_TYPE_UINT32,  .elements = 1, .readable = true,  .writeable = false, .data_pointer = NULL },
    { .config_id = CONFIG_ID_KEY,     .config_type = CONFIG_TYPE_UINT32,  .elements = 1, .readable = false, .writeable = true,  .data_pointer = NULL },
    { .config_id = CONFIG_ID_MODE,    .config_type = CONFIG_TYPE_UINT8,   .elements = 1, .readable = true,  .writeable = true,  .data_pointer = NULL },
    { .config_id = CONFIG_ID_INVALID, .config_type = CONFIG_TYPE_UNKNOWN, .elements = 0, .readable = false, .writeable = false, .data_pointer = NULL }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static CONFIG_CALLBACK(ModeConfigCallback);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
void ModeInit(void)
{
  ASSERT_FATAL(SecurityGetSizeOfSeed() == 4);
  ASSERT_FATAL(SecurityGetSizeOfKey() == 4);

  ConfigRegister(mode_config, ModeConfigCallback);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Callback routine used for configuration accesses not covered by simple read/write via pointers.
 *
 -----------------------------------------------------------------------------*/
static CONFIG_CALLBACK(ModeConfigCallback)
{
  e_ERROR_ID result = ERROR_ID_SUCCESS;

  switch (config_id) {

    case CONFIG_ID_SEED:

      if ((callback_type == CONFIG_CALLBACK_TYPE_PRE) && read)
        SecurityGetSeed((uint8_t *)data);

      break;

    case CONFIG_ID_KEY:

      if ((callback_type == CONFIG_CALLBACK_TYPE_PRE) && !read)
        SecuritySetKey((uint8_t *)data);

      break;

    case CONFIG_ID_MODE:

      if (callback_type == CONFIG_CALLBACK_TYPE_PRE) {
        if (read)
          *(uint8_t *)data = (uint8_t)ModeGet();
        else
          result = ModeSet((e_MODE)*(uint8_t *)data);
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
 * Get the current mode.
 *
 -----------------------------------------------------------------------------*/
e_MODE ModeGet(void)
{
  const e_SECURITY_LEVEL security_level = SecurityGetCurrentLevel();

  return
      OperationIsRunning() ? MODE_ID_OPERATION :
          (security_level == SECURITY_LEVEL_MAINTENANCE) ? MODE_ID_ADVANCED_CUSTOMER :
              (security_level == SECURITY_LEVEL_MANUFACTURER) ? MODE_ID_ADVANCED_SMART :
                  MODE_ID_CONFIG;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Set the given mode if allowed and start/stop operation.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID ModeSet(const e_MODE mode)
{
  e_ERROR_ID result = ERROR_ID_SUCCESS;

  if ((mode < MODE_ID_OPERATION) || (mode > MODE_ID_ADVANCED_SMART))
    result = ERROR_ID_VALUE_OUT_OF_RANGE;
  else {

    const e_SECURITY_LEVEL required =
        (mode == MODE_ID_ADVANCED_CUSTOMER) ? SECURITY_LEVEL_MAINTENANCE :
            (mode == MODE_ID_ADVANCED_SMART) ? SECURITY_LEVEL_MANUFACTURER :
                SECURITY_LEVEL_ALL;

    result = SecuritySetLevel(required);

    if (result == ERROR_ID_SUCCESS) {
      if (mode == MODE_ID_OPERATION)
        OperationStart();
      else
        OperationStop();
    }
  }

  return result;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
