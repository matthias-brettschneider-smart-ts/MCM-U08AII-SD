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
#include "middleware/IError.h"
#include "middleware/IGeneric.h"
#include "middleware/ISecurity.h"
#include "middleware/ISecurityInternal.h"
#include "middleware/IWait.h"
#include "os/ISeedKey.h"

#ifndef SECURITY_SIZE_OF_SEED_IN_BYTE
#error "error compile flags - SECURITY_SIZE_OF_SEED_IN_BYTE undefined"
#elif ((SECURITY_SIZE_OF_SEED_IN_BYTE != 4) && (SECURITY_SIZE_OF_SEED_IN_BYTE != 16))
#error "error compile flags - SECURITY_SIZE_OF_SEED_IN_BYTE invalid"
#endif

#ifndef SECURITY_SIZE_OF_KEY_IN_BYTE
#error "error compile flags - SECURITY_SIZE_OF_KEY_IN_BYTE undefined"
#elif ((SECURITY_SIZE_OF_KEY_IN_BYTE != 4) && (SECURITY_SIZE_OF_KEY_IN_BYTE != 32))
#error "error compile flags - SECURITY_SIZE_OF_KEY_IN_BYTE invalid"
#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef struct {
  e_SECURITY_LEVEL level;
  bool brute_force_timeout_active;
  uint32_t brute_force_timeout_value;
  bool seed_valid;
  uint8_t seed[SECURITY_SIZE_OF_SEED_IN_BYTE];
  uint8_t key[SECURITY_SIZE_OF_KEY_IN_BYTE];
} t_SECURITY;

#define SECURITY_TIMEOUT_BRUTE_FORCE_IN_MS                                  1000

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_SECURITY security;

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
 *
 *
 -----------------------------------------------------------------------------*/
size_t SecurityGetSizeOfSeed(void)
{
  return SECURITY_SIZE_OF_SEED_IN_BYTE;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
size_t SecurityGetSizeOfKey(void)
{
  return SECURITY_SIZE_OF_KEY_IN_BYTE;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
static void SecurityStartBruteForceTimer(void)
{
  security.brute_force_timeout_value = WAIT_GET_TIMER_VALUE_32();
  security.brute_force_timeout_active = true;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
static void SecurityUpdateBruteForceTimer(void)
{
  if (security.brute_force_timeout_active)
    if (WAIT_CHECK_TIMEOUT_MS_32(security.brute_force_timeout_value, SECURITY_TIMEOUT_BRUTE_FORCE_IN_MS))
      security.brute_force_timeout_active = false;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
static bool SecurityBruteForceTimerInactive(void)
{
  return !security.brute_force_timeout_active;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Create new seed if necessary and return seed.
 *
 -----------------------------------------------------------------------------*/
void SecurityGetSeed(uint8_t seed[])
{
  ASSERT_FATAL(seed != NULL);

  if (!security.seed_valid)
    security.seed_valid = SeedKeyCreateSeed(security.seed, SECURITY_SIZE_OF_SEED_IN_BYTE) == SECURITY_SIZE_OF_SEED_IN_BYTE;

  memcpy(seed, security.seed, SECURITY_SIZE_OF_SEED_IN_BYTE);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void SecuritySetKey(const uint8_t key[])
{
  ASSERT_FATAL(key != NULL);

  memcpy(security.key, key, SECURITY_SIZE_OF_KEY_IN_BYTE);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Set security level if key is valid but prevent brute force attacks.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID SecuritySetLevel(const e_SECURITY_LEVEL security_level)
{
  e_ERROR_ID result = ERROR_ID_WRONG_KEY;

  if (security_level == SECURITY_LEVEL_ALL) {
    security.level = SECURITY_LEVEL_ALL;
    result = ERROR_ID_SUCCESS;
  }
  else if (!security.seed_valid)
    result = ERROR_ID_NO_SEED_REQUESTED;
  else {

    SecurityUpdateBruteForceTimer();
    if (SecurityBruteForceTimerInactive()) {

      bool valid = false;
      if (security_level == SECURITY_LEVEL_OPERATOR)
        valid = SeedKeyValidForOperator(security.seed, SECURITY_SIZE_OF_SEED_IN_BYTE, security.key, SECURITY_SIZE_OF_KEY_IN_BYTE);
      else if (security_level == SECURITY_LEVEL_MAINTENANCE)
        valid = SeedKeyValidForMaintenance(security.seed, SECURITY_SIZE_OF_SEED_IN_BYTE, security.key, SECURITY_SIZE_OF_KEY_IN_BYTE);
      else if (security_level == SECURITY_LEVEL_MANUFACTURER)
        valid = SeedKeyValidForManufacturer(security.seed, SECURITY_SIZE_OF_SEED_IN_BYTE, security.key, SECURITY_SIZE_OF_KEY_IN_BYTE);

      if (valid) {
        security.level = security_level;
        security.seed_valid = false;
        result = ERROR_ID_SUCCESS;
      }
      else {
        security.level = SECURITY_LEVEL_ALL;
        SecurityStartBruteForceTimer();
      }
    }
  }

  return result;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
e_SECURITY_LEVEL SecurityGetCurrentLevel(void)
{
  return security.level;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void SecurityLock(void)
{
  security.level = SECURITY_LEVEL_ALL;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
