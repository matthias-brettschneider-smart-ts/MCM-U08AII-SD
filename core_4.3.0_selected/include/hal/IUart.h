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
  UART_MODE_RX,
  UART_MODE_TX,
  UART_MODE_RX_AND_TX
} e_UART_MODE;

typedef enum {
  UART_PARITY_EVEN,
  UART_PARITY_ODD,
  UART_PARITY_NONE
} e_UART_PARITY;

typedef struct {
  e_UART_MODE mode;
  e_UART_PARITY parity;
  bool two_stop_bits;
  uint32_t bitrate;
} t_UART_SETTINGS;

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

void UartInit(const int node, const t_UART_SETTINGS * const settings);

COMPILER_INLINE void UART_ACTIVATE(const int node, const e_UART_MODE mode);
COMPILER_INLINE void UART_DEACTIVATE(const int node);

COMPILER_INLINE void UART_TRANSMIT_8_BITS(const int node, const uint8_t transmit);
COMPILER_INLINE uint8_t UART_RECEIVE_8_BITS(const int node);

COMPILER_INLINE bool UART_TRANSMISSION_COMPLETED(const int node);

/*============================================================================*/
/*  I M P L E M E N T A T I O N                                               */
/*============================================================================*/

#include "UartImpl.h"

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
