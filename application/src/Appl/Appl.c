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

#include "middleware/IDebug.h"
#include "middleware/IGeneric.h"
#include "project/IApplication.h"

#ifndef APPL_DEBUG_DO_TIMING_ANALYSES
#error "error compile flags - APPL_DEBUG_DO_TIMING_ANALYSES undefined"
#elif ((APPL_DEBUG_DO_TIMING_ANALYSES != false) && (APPL_DEBUG_DO_TIMING_ANALYSES != true))
#error "error compile flags - APPL_DEBUG_DO_TIMING_ANALYSES invalid"
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
void ApplInitOther(void)
{
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
void ApplInitFinal(void)
{
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Application task called each cycle.
 *
 -----------------------------------------------------------------------------*/
void ApplOperateInterrupt(const uint16_t cycle_number, const bool discontinuous_call)
{
  (void)cycle_number;
  (void)discontinuous_call;

  DEBUG_ACTIVATE_DEBUG_PIN(APPL_DEBUG_DO_TIMING_ANALYSES);

  DEBUG_DEACTIVATE_DEBUG_PIN(APPL_DEBUG_DO_TIMING_ANALYSES);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Application task called irregularly in remaining processor time.
 * There is not guarantee for being executed once and only once at each cycle.
 *
 -----------------------------------------------------------------------------*/
void ApplOperateNonInterrupt(const uint16_t cycle_number)
{
  (void)cycle_number;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
