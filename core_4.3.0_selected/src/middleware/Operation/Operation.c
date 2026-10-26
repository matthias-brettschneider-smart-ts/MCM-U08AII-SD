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

#include "middleware/IConfig.h"
#include "middleware/IDefault.h"
#include "middleware/IGeneric.h"
#include "middleware/IOperation.h"
#include "middleware/IOperationInternal.h"
#include "middleware/IStorage.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef struct {
  bool active;
  bool auto_operation;
} t_OPERATION;

#define OPERATION_AUTO_OPERATION_DEFAULT                                   false

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_OPERATION operation;

static const t_STORAGE_ENTRY operation_storage[] = {
    { .storage_id = STORAGE_ID_AUTO_OPERATION, .storage_block = STORAGE_BLOCK_CORE_SETTINGS, .data_pointer = (void*)&operation.auto_operation, .size = sizeof(operation.auto_operation) },
    { .storage_id = STORAGE_ID_UNDEFINED,      .storage_block = STORAGE_BLOCK_UNDEFINED,     .data_pointer = NULL,                             .size = 0                                }
};

static const t_CONFIG operation_config[] = {
    { .config_id = CONFIG_ID_AUTO_OPERATION, .config_type = CONFIG_TYPE_BOOL,    .elements = 1, .readable = true,  .writeable = true,  .data_pointer = (void*)&operation.auto_operation },
    { .config_id = CONFIG_ID_INVALID,        .config_type = CONFIG_TYPE_UNKNOWN, .elements = 0, .readable = false, .writeable = false, .data_pointer = NULL                             }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static DEFAULT_CALLBACK(OperationDefaultCallback);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
void OperationInit(void)
{
  DefaultRegister(OperationDefaultCallback);

  StorageRegister(operation_storage);
  StorageLoad(operation_storage);

  ConfigRegister(operation_config, NULL);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Callback routine used for factory reset.
 *
 -----------------------------------------------------------------------------*/
static DEFAULT_CALLBACK(OperationDefaultCallback)
{
  operation.auto_operation = OPERATION_AUTO_OPERATION_DEFAULT;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void OperationStart(void)
{
  operation.active = true;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void OperationStop(void)
{
  operation.active = false;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void OperationApplyAutoOperation(void)
{
  if (operation.auto_operation)
    operation.active = true;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool OperationIsRunning(void)
{
  return operation.active;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
