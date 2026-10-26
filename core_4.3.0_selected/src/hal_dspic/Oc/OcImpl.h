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
#pragma once

/*============================================================================*/
/* I N C L U D E S                                                            */
/*============================================================================*/

#define CONTROLLER_DSPIC33FJ256GP710_SUPPORTED

#include "hal/ICompiler.h"
#include "hal/IController.h"
#include "middleware/IAssert.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

typedef enum {
  OCM_MODE_OFF                                                               = 0,
  OCM_MODE_PWM_WITHOUT_FAULT_PROTECT                                         = 6
} e_OCM_MODE;

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void OC_DISABLE(const int channel)
{
  switch (channel) {
    case 0: OC1CONbits.OCM = OCM_MODE_OFF; break;
    case 1: OC2CONbits.OCM = OCM_MODE_OFF; break;
    case 2: OC3CONbits.OCM = OCM_MODE_OFF; break;
    case 3: OC4CONbits.OCM = OCM_MODE_OFF; break;
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void OC_ENABLE_FOR_PWM_GENERATION(const int channel)
{
  switch (channel) {
    case 0: OC1CONbits.OCM = OCM_MODE_PWM_WITHOUT_FAULT_PROTECT; break;
    case 1: OC2CONbits.OCM = OCM_MODE_PWM_WITHOUT_FAULT_PROTECT; break;
    case 2: OC3CONbits.OCM = OCM_MODE_PWM_WITHOUT_FAULT_PROTECT; break;
    case 3: OC4CONbits.OCM = OCM_MODE_PWM_WITHOUT_FAULT_PROTECT; break;
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool OC_IS_ENABLED(const int channel)
{
  bool active = false;

  switch (channel) {
    case 0: active = OC1CONbits.OCM == OCM_MODE_PWM_WITHOUT_FAULT_PROTECT; break;
    case 1: active = OC2CONbits.OCM == OCM_MODE_PWM_WITHOUT_FAULT_PROTECT; break;
    case 2: active = OC3CONbits.OCM == OCM_MODE_PWM_WITHOUT_FAULT_PROTECT; break;
    case 3: active = OC4CONbits.OCM == OCM_MODE_PWM_WITHOUT_FAULT_PROTECT; break;
    default: ASSERT_FATAL(false); break;
  }

  return active;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void OC_SELECT_TIMER(const int channel, const int timer)
{
  switch (channel) {
    case 0: OC1CONbits.OCTSEL = timer; break;
    case 1: OC2CONbits.OCTSEL = timer; break;
    case 2: OC3CONbits.OCTSEL = timer; break;
    case 3: OC4CONbits.OCTSEL = timer; break;
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void OC_SET_INIT_VALUE(const int channel, const uint16_t value)
{
  switch (channel) {
    case 0: OC1RS = value; break;
    case 1: OC2RS = value; break;
    case 2: OC3RS = value; break;
    case 3: OC4RS = value; break;
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void OC_APPLY_INIT_VALUE(const int channel)
{
  switch (channel) {
    case 0: OC1R = OC1RS; break;
    case 1: OC2R = OC2RS; break;
    case 2: OC3R = OC3RS; break;
    case 3: OC4R = OC4RS; break;
    default: ASSERT_FATAL(false); break;
  }
}

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
