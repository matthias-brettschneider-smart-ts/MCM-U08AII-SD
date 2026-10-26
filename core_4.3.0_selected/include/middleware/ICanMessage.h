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

#include "middleware/IGeneric.h"

#ifndef CAN_MESSAGE_PAYLOAD_LENGTH
#error "error compile flags - CAN_MESSAGE_PAYLOAD_LENGTH undefined"
#elif ((CAN_MESSAGE_PAYLOAD_LENGTH != 8) && (CAN_MESSAGE_PAYLOAD_LENGTH != 64))
#error "error compile flags - CAN_MESSAGE_PAYLOAD_LENGTH invalid"
#endif

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

typedef struct {
  uint16_t identifier;
  bool fd;
  bool bitrate_switching;
  uint8_t payload_length;
  union {
    uint8_t  data_byte[CAN_MESSAGE_PAYLOAD_LENGTH];
    uint16_t data_word[CAN_MESSAGE_PAYLOAD_LENGTH / 2];
    uint32_t data_long[CAN_MESSAGE_PAYLOAD_LENGTH / 4];
  };
} t_CAN_MESSAGE;

#define CAN_MESSAGE_INVALID_LENGTH_OR_DLC                                      0

#define CAN_MESSAGE_ID_INVALID                                                 0
#define CAN_MESSAGE_ID_FIRST                                                   1
#define CAN_MESSAGE_ID_LAST                                                0x7FF

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

uint8_t CanMessageGetLength(const bool can_fd, const uint8_t dlc);
uint8_t CanMessageGetDlc(const bool can_fd, const uint8_t length);

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
