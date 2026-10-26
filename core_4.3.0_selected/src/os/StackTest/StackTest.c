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

#include "hal/IStack.h"
#include "middleware/IConfig.h"
#include "middleware/IGeneric.h"
#include "os/IStackTest.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static const t_CONFIG stack_test_config[] = {
    { .config_id = CONFIG_ID_CURRENT_STACK_POINTER, .config_type = CONFIG_TYPE_UINT32,  .elements = 1, .readable = true,  .writeable = false, .data_pointer = NULL },
    { .config_id = CONFIG_ID_STACK_LIMIT,           .config_type = CONFIG_TYPE_UINT32,  .elements = 1, .readable = true,  .writeable = false, .data_pointer = NULL },
    { .config_id = CONFIG_ID_INIT_STACK_TEST,       .config_type = CONFIG_TYPE_BOOL,    .elements = 1, .readable = false, .writeable = true,  .data_pointer = NULL },
    { .config_id = CONFIG_ID_STACK_UNUSED,          .config_type = CONFIG_TYPE_UINT32,  .elements = 1, .readable = true,  .writeable = false, .data_pointer = NULL },
    { .config_id = CONFIG_ID_INVALID,               .config_type = CONFIG_TYPE_UNKNOWN, .elements = 0, .readable = false, .writeable = false, .data_pointer = NULL }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static CONFIG_CALLBACK(StackTestConfigCallback);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
void StackTestInit(void)
{
  ConfigRegister(stack_test_config, StackTestConfigCallback);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Callback routine used for configuration accesses not covered by simple read/write via pointers.
 *
 -----------------------------------------------------------------------------*/
static CONFIG_CALLBACK(StackTestConfigCallback)
{
  (void)read;

  e_ERROR_ID result = ERROR_ID_SUCCESS;

  switch (config_id) {

    case CONFIG_ID_CURRENT_STACK_POINTER:

      if (callback_type == CONFIG_CALLBACK_TYPE_PRE)
        *(uint32_t *)data = StackGetStackPointer();

      break;

    case CONFIG_ID_STACK_LIMIT:

      if (callback_type == CONFIG_CALLBACK_TYPE_PRE)
        *(uint32_t *)data = StackGetStackLimit();

      break;

    case CONFIG_ID_INIT_STACK_TEST:

      if (callback_type == CONFIG_CALLBACK_TYPE_PRE)
        if (*(bool *)data == false)
          result = ERROR_ID_VALUE_OUT_OF_RANGE;

      if (callback_type == CONFIG_CALLBACK_TYPE_POST)
        StackInitializeEmpty();

      break;

    case CONFIG_ID_STACK_UNUSED:

      if (callback_type == CONFIG_CALLBACK_TYPE_PRE)
        *(uint32_t *)data = StackIsInitializedEmpty() ? StackGetNumberOfUnusedBytes() : 0;

      break;

    default:
      break;
  }

  return result;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
