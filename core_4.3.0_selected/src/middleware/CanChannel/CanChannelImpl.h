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

#include "hal/ICompiler.h"
#include "middleware/ICanChannel.h"
#include "middleware/ICanMessage.h"
#include "middleware/IGeneric.h"
#include "os/ICanBase.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Receive & transmit functions for protocol & application related CAN frames.
 *
 * (Functions are just name wrappers for non-inline functions. Therefore, names
 * are written in pascal case... despite being inline functions technically.)
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool CanChannelReceiveRequest(t_CAN_MESSAGE * const message, bool * const broadcast)
{
  return CanBaseReceiveRequest(message, broadcast);
}
COMPILER_INLINE bool CanChannelTransmitResponse(t_CAN_MESSAGE * const message, const bool suppress_error_entry)
{
  return CanBaseTransmitResponse(message, suppress_error_entry);
}
COMPILER_INLINE bool CanChannelReceiveApplication(t_CAN_MESSAGE * const message)
{
  return CanBaseReceiveApplication(message);
}
COMPILER_INLINE bool CanChannelTransmitApplication(t_CAN_MESSAGE * const message, const bool suppress_error_entry)
{
  return CanBaseTransmitApplication(message, suppress_error_entry);
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
