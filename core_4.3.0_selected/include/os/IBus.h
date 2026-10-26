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
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

typedef enum {
  BUS_ACTION_NONE,
  BUS_ACTION_TRANSMIT,
  BUS_ACTION_RECEIVE,
  BUS_ACTION_TRANSCEIVE
} t_BUS_ACTION;

typedef struct {
  t_BUS_ACTION action;
  uint8_t lane;
  uint8_t slot_within_cycle;
  void * buffer_receive;
  void * buffer_transmit;
} t_BUS_PREPARE;

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

void BusInit(const uint16_t block_size_in_bytes);

COMPILER_INLINE uint8_t BUS_GET_NUMBER_OF_LANES(void);
COMPILER_INLINE uint16_t BUS_GET_NUMBER_OF_SLOTS_PER_S(void);

COMPILER_INLINE bool BUS_SUPPORTS_ACTION(const t_BUS_ACTION action);
COMPILER_INLINE bool BUS_MUST_BE_PREPARED_AHEAD(void);

void BusPrepare(const t_BUS_PREPARE * const bus_prepare);

/*============================================================================*/
/*  I M P L E M E N T A T I O N                                               */
/*============================================================================*/

#include "BusImpl.h"

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
