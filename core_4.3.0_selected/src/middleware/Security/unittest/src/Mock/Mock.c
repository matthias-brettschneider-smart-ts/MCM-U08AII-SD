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

#include "middleware/ISecurity.h"
#include "os/ISeedKey.h"

#include "IMock.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define MOCK_NUMBER_OF_SEEDS                                                   5

#define MOCK_SIZE_OF_SEED_IN_BYTE                                              4
#define MOCK_SIZE_OF_KEY_IN_BYTE                                               4

typedef struct {
  int seed_number;
  uint32_t timer_in_ticks;
  uint8_t seeds[MOCK_NUMBER_OF_SEEDS][MOCK_SIZE_OF_SEED_IN_BYTE];
  uint8_t key_operator[MOCK_SIZE_OF_KEY_IN_BYTE];
  uint8_t key_maintenance[MOCK_SIZE_OF_KEY_IN_BYTE];
  uint8_t key_manufacturer[MOCK_SIZE_OF_KEY_IN_BYTE];
} t_MOCK;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_MOCK mock = {
    .seed_number = 0,
    .timer_in_ticks = 0,
    .seeds = { MOCK_SEED_0, MOCK_SEED_1, MOCK_SEED_2, MOCK_SEED_3, MOCK_SEED_4 },
    .key_operator = MOCK_KEY_OPERATOR,
    .key_maintenance = MOCK_KEY_MAINTENANCE,
    .key_manufacturer = MOCK_KEY_MANUFACTURER
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

size_t SeedKeyCreateSeed(uint8_t seed[], const size_t size_of_seed)
{
  (void)size_of_seed;

  memcpy(seed, mock.seeds[mock.seed_number], MOCK_SIZE_OF_SEED_IN_BYTE);

  if (mock.seed_number < (MOCK_NUMBER_OF_SEEDS - 1))
    mock.seed_number++;

  return MOCK_SIZE_OF_SEED_IN_BYTE;
}

bool SeedKeyValidForOperator(const uint8_t seed[], const size_t size_of_seed, const uint8_t key[], const size_t size_of_key)
{
  (void)seed;
  (void)size_of_seed;

  return memcmp(key, mock.key_operator, size_of_key) == 0;
}

bool SeedKeyValidForMaintenance(const uint8_t seed[], const size_t size_of_seed, const uint8_t key[], const size_t size_of_key)
{
  (void)seed;
  (void)size_of_seed;

  return memcmp(key, mock.key_maintenance, size_of_key) == 0;
}

bool SeedKeyValidForManufacturer(const uint8_t seed[], const size_t size_of_seed, const uint8_t key[], const size_t size_of_key)
{
  (void)seed;
  (void)size_of_seed;

  return memcmp(key, mock.key_manufacturer, size_of_key) == 0;
}

void MockTimer32Increment(const uint32_t time_in_ms)
{
  mock.timer_in_ticks += MOCK_TIMER_TICKS_PER_MS * time_in_ms;
}

uint32_t MockTimer32Get(void)
{
  return mock.timer_in_ticks;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
