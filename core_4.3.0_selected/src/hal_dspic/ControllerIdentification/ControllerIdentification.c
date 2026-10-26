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

#define CONTROLLER_DSPIC33FJ256GP710_SUPPORTED
#define CONTROLLER_DSPIC33EP128GS804_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP508_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP506_SUPPORTED

#include "hal/IController.h"
#include "hal/IControllerIdentification.h"
#include "hal/IFlash.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef struct {
  uint16_t id;
  uint16_t revision;
} t_CONTROLLER_IDENTIFICATION;

#define CONTROLLER_IDENTIFICATION_ADDRESS_ID                          0x01FE0000
#define CONTROLLER_IDENTIFICATION_ADDRESS_REVISION                    0x01FE0004

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static const t_CONTROLLER_IDENTIFICATION controller_supported[] = {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    { .id =                           0x00FFu, .revision =                           0x3002u }, // dsPIC33FJ256GP710 | A2
    { .id =                           0x00FFu, .revision =                           0x3004u }, // dsPIC33FJ256GP710 | A3
    { .id =                           0x00FFu, .revision =                           0x3040u }, // dsPIC33FJ256GP710 | A4
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    { .id =                           0x6C50u, .revision =                           0x4001u }, // dsPIC33EP128GS804 | A2
    { .id =                           0x6C50u, .revision =                           0x4002u }, // dsPIC33EP128GS804 | B0
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508)
    { .id =                           0x7C74u, .revision =                              0x5u }, // dsPIC33CK256MP508 | B3
    { .id =                           0x7C74u, .revision =                              0x7u }, // dsPIC33CK256MP508 | B5
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506)
    { .id =                           0x7C73u, .revision =                              0x5u }, // dsPIC33CK256MP506 | B3
    { .id =                           0x7C73u, .revision =                              0x7u }, // dsPIC33CK256MP506 | B5
#endif
    { .id = CONTROLLER_IDENTIFICATION_INVALID, .revision = CONTROLLER_IDENTIFICATION_INVALID }
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

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Read device id and device revision and stop on invalid devices, inform about unexpected revisions.
 *
 -----------------------------------------------------------------------------*/
void ControllerIdentificationCheck(void)
{
  const uint16_t id = ControllerIdentificationGetDeviceId();
  const uint16_t revision = ControllerIdentificationGetDeviceRevision();

  bool device_valid = false;
  for (int entry = 0; controller_supported[entry].id != CONTROLLER_IDENTIFICATION_INVALID; entry++)
    if (id == controller_supported[entry].id)
      device_valid = true;

  if (!device_valid)
    ErrorAddEntry(ERROR_ID_FATAL_INVALID_DEVICE, 0);

  bool revision_known = false;
  for (int entry = 0; controller_supported[entry].id != CONTROLLER_IDENTIFICATION_INVALID; entry++)
    if (id == controller_supported[entry].id)
      if (revision == controller_supported[entry].revision)
        revision_known = true;

  if (!revision_known)
    ErrorAddEntry(ERROR_ID_UNKNOWN_DEVICE_REVISION, 0);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
uint16_t ControllerIdentificationGetDeviceId(void)
{
  return (uint16_t)FlashRead32Bit(CONTROLLER_IDENTIFICATION_ADDRESS_ID);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
uint16_t ControllerIdentificationGetDeviceRevision(void)
{
  return (uint16_t)FlashRead32Bit(CONTROLLER_IDENTIFICATION_ADDRESS_REVISION);
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
