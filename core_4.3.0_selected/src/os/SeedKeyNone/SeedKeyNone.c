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

#include "middleware/IGeneric.h"
#include "os/ISeedKey.h"

#ifndef SEED_KEY_NONE_NONE_DISABLE_WARNING_FOR_TESTING
#define SEED_KEY_NONE_NONE_DISABLE_WARNING_FOR_TESTING                     false
#endif

#if (SEED_KEY_NONE_NONE_DISABLE_WARNING_FOR_TESTING == false)
#warning "No seed & key. For development purpose only!"
#endif

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

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
void SeedKeyInit(void)
{

}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Algorithm for creating new seed
 *
 -----------------------------------------------------------------------------*/
size_t SeedKeyCreateSeed(uint8_t seed[], const size_t size_of_seed)
{
  memset(seed, 0x55, size_of_seed);

  return size_of_seed;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
  * Algorithm for validating operator key.
 *
 -----------------------------------------------------------------------------*/
bool SeedKeyValidForOperator(const uint8_t seed[], const size_t size_of_seed, const uint8_t key[], const size_t size_of_key)
{
  (void)seed;
  (void)size_of_seed;
  (void)key;
  (void)size_of_key;

  return true;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Algorithm for validating maintenance key.
 *
 -----------------------------------------------------------------------------*/
bool SeedKeyValidForMaintenance(const uint8_t seed[], const size_t size_of_seed, const uint8_t key[], const size_t size_of_key)
{
  (void)seed;
  (void)size_of_seed;
  (void)key;
  (void)size_of_key;

  return true;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Algorithm for validating manufacturer key.
 *
 -----------------------------------------------------------------------------*/
bool SeedKeyValidForManufacturer(const uint8_t seed[], const size_t size_of_seed, const uint8_t key[], const size_t size_of_key)
{
  (void)seed;
  (void)size_of_seed;
  (void)key;
  (void)size_of_key;

  return true;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
