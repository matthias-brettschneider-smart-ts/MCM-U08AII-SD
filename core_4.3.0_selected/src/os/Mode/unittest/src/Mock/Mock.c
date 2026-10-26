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
#include "middleware/IGeneric.h"
#include "middleware/IOperation.h"
#include "middleware/IOperationInternal.h"
#include "middleware/ISecurity.h"
#include "middleware/ISecurityInternal.h"

#include "IMock.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define MOCK_SIZE_OF_SEED                                                      4
#define MOCK_SIZE_OF_KEY                                                       4

typedef struct {
  bool is_running;
  e_SECURITY_LEVEL security_level;
  bool accept_key;
} t_MOCK;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_MOCK mock;

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

// --- operation ---------------------------------------------------------------

void OperationStart(void)
{
  mock.is_running = true;
}

void OperationStop(void)
{
  mock.is_running = false;
}

bool OperationIsRunning(void)
{
  return mock.is_running;
}

// --- security ----------------------------------------------------------------

size_t SecurityGetSizeOfSeed(void)
{
  return MOCK_SIZE_OF_SEED;
}

size_t SecurityGetSizeOfKey(void)
{
  return MOCK_SIZE_OF_KEY;
}

void SecurityGetSeed(uint8_t seed[])
{
  (void)seed;
}

void SecuritySetKey(const uint8_t key[])
{
  (void)key;
}

e_ERROR_ID SecuritySetLevel(const e_SECURITY_LEVEL security_level)
{
  if (mock.accept_key)
    mock.security_level = security_level;

  return mock.accept_key ? ERROR_ID_SUCCESS : ERROR_ID_WRONG_KEY;
}

e_SECURITY_LEVEL SecurityGetCurrentLevel(void)
{
  return mock.security_level;
}

void MockResetSecurityLevel(void)
{
  mock.security_level = SECURITY_LEVEL_ALL;
}

void MockAcceptKey(void)
{
  mock.accept_key = true;
}

void MockRejectKey(void)
{
  mock.accept_key = false;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
