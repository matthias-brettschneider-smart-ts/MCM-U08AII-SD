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

#include "middleware/ICanMessage.h"
#include "os/IMode.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

void MockModeSet(const e_MODE mode);

void MockFlushActions(void);
bool MockActionIsEqual(const char expected[], const size_t number_of_characters);

bool MockCanChannelReceiveIsPending(void);
void MockCanChannelPrepareMessageForReceive(const t_CAN_MESSAGE * const message, const bool broadcast);
bool MockCanChannelReceiveRequest(t_CAN_MESSAGE * const message, bool * const broadcast);

bool MockCanChannelTransmitResponse(t_CAN_MESSAGE * const message, const bool suppress_error_entry);
bool MockCanChannelTransmitIsPending(void);
bool MockCanChannelMessageWasTransmitted(const t_CAN_MESSAGE * const message);
void MockCanChannelFlushTransmit(void);

void MockSetFlashSessionStatus(const bool active);

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
