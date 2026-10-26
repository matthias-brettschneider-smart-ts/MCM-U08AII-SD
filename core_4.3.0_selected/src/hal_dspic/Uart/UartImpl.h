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

#define CONTROLLER_DSPIC33EP128GS804_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP508_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP506_SUPPORTED

#include "hal/ICompiler.h"
#include "hal/IController.h"
#include "middleware/IAssert.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Activate UART node.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void UART_ACTIVATE(const int node, const e_UART_MODE mode)
{
  ASSERT_FATAL((mode == UART_MODE_RX) || (mode == UART_MODE_TX) || (mode == UART_MODE_RX_AND_TX));

  const bool tx_enable = (mode == UART_MODE_TX) || (mode == UART_MODE_RX_AND_TX);
  const bool rx_enable = (mode == UART_MODE_RX) || (mode == UART_MODE_RX_AND_TX);

  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    // enabling TX has to be done after enabling UART itself
    case 0: U1MODEbits.UARTEN = 1; U1STAbits.UTXEN = tx_enable ? 1 : 0; (void)rx_enable; break;
    case 1: U2MODEbits.UARTEN = 1; U2STAbits.UTXEN = tx_enable ? 1 : 0; (void)rx_enable; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: U1MODEbits.UARTEN = 1; U1MODEbits.UTXEN = tx_enable ? 1 : 0; U1MODEbits.URXEN = rx_enable ? 1 : 0; break;
    case 1: U2MODEbits.UARTEN = 1; U2MODEbits.UTXEN = tx_enable ? 1 : 0; U2MODEbits.URXEN = rx_enable ? 1 : 0; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Deactivate UART node.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void UART_DEACTIVATE(const int node)
{
  switch (node) {
    case 0: U1MODEbits.UARTEN = 0; break;
    case 1: U2MODEbits.UARTEN = 0; break;
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void UART_TRANSMIT_8_BITS(const int node, const uint8_t transmit)
{
  switch (node) {
    case 0: U1TXREG = transmit; break;
    case 1: U2TXREG = transmit; break;
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint8_t UART_RECEIVE_8_BITS(const int node)
{
  uint8_t receive = 0;

  switch (node) {
    case 0: receive = U1RXREG; break;
    case 1: receive = U2RXREG; break;
    default: ASSERT_FATAL(false); break;
  }

  return receive;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool UART_TRANSMISSION_COMPLETED(const int node)
{
  switch (node) {
    case 0: return U1STAbits.TRMT == 1; break;
    case 1: return U2STAbits.TRMT == 1; break;
    default: ASSERT_FATAL(false); return false; break;
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
