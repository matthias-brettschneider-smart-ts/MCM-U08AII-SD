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

#include "hal/ICan.h"
#include "hal/ICompiler.h"
#include "hal/IController.h"
#include "middleware/IAssert.h"
#include "middleware/ICanMessage.h"
#include "middleware/IGeneric.h"
#include "CanFdMessages.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void CAN_FD_CONFIGURE_FIFO(const uint8_t node, const uint8_t fifo, const bool tx_not_rx, const uint8_t payload_size_in_byte, const uint8_t number_of_messages)
{
  if (number_of_messages > 0) {

    ASSERT_FATAL(node < CAN_GET_NUMBER_OF_NODES());
    ASSERT_FATAL(fifo < CAN_GET_NUMBER_OF_FIFOS());
    ASSERT_FATAL(number_of_messages <= 32);
    const bool try_receiving_via_fifo_0 = (fifo == 0) && !tx_not_rx;
    ASSERT_FATAL(!try_receiving_via_fifo_0);

    const uint8_t fsize = number_of_messages - 1;
    const uint8_t plsize =
        (payload_size_in_byte == 8) ? 0 :
            (payload_size_in_byte == 12) ? 1 :
                (payload_size_in_byte == 16) ? 2 :
                    (payload_size_in_byte == 20) ? 3 :
                        (payload_size_in_byte == 24) ? 4 :
                            (payload_size_in_byte == 32) ? 5 :
                                (payload_size_in_byte == 48) ? 6 :
                                    (payload_size_in_byte == 64) ? 7 :
                                        UINT8_MAX;
    ASSERT_FATAL(plsize != UINT8_MAX);
    const uint8_t txen = tx_not_rx ? 1 : 0;
    const uint8_t rxtsen = tx_not_rx ? 0 : 1;

    switch (fifo) {
      case  0: C1FIFOCON1Hbits.FSIZE = fsize; C1FIFOCON1Hbits.PLSIZE = plsize; C1FIFOCON1Lbits.TXEN = txen; C1FIFOCON1Lbits.RXTSEN = rxtsen; break;
      case  1: C1FIFOCON2Hbits.FSIZE = fsize; C1FIFOCON2Hbits.PLSIZE = plsize; C1FIFOCON2Lbits.TXEN = txen; C1FIFOCON2Lbits.RXTSEN = rxtsen; break;
      case  2: C1FIFOCON3Hbits.FSIZE = fsize; C1FIFOCON3Hbits.PLSIZE = plsize; C1FIFOCON3Lbits.TXEN = txen; C1FIFOCON3Lbits.RXTSEN = rxtsen; break;
      case  3: C1FIFOCON4Hbits.FSIZE = fsize; C1FIFOCON4Hbits.PLSIZE = plsize; C1FIFOCON4Lbits.TXEN = txen; C1FIFOCON4Lbits.RXTSEN = rxtsen; break;
      case  4: C1FIFOCON5Hbits.FSIZE = fsize; C1FIFOCON5Hbits.PLSIZE = plsize; C1FIFOCON5Lbits.TXEN = txen; C1FIFOCON5Lbits.RXTSEN = rxtsen; break;
      case  5: C1FIFOCON6Hbits.FSIZE = fsize; C1FIFOCON6Hbits.PLSIZE = plsize; C1FIFOCON6Lbits.TXEN = txen; C1FIFOCON6Lbits.RXTSEN = rxtsen; break;
      case  6: C1FIFOCON7Hbits.FSIZE = fsize; C1FIFOCON7Hbits.PLSIZE = plsize; C1FIFOCON6Lbits.TXEN = txen; C1FIFOCON7Lbits.RXTSEN = rxtsen; break;
      default: ASSERT_FATAL(false); break;
    };
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool CAN_FD_TRANSMIT_FIFO_IS_NOT_FULL(const uint8_t node, const uint8_t fifo)
{
  ASSERT_FATAL(node < CAN_GET_NUMBER_OF_NODES());
  ASSERT_FATAL(fifo < CAN_GET_NUMBER_OF_FIFOS());

  return
      (fifo == 0) ? (C1FIFOSTA1bits.TFNRFNIF == 1) :
          (fifo == 1) ? (C1FIFOSTA2bits.TFNRFNIF == 1) :
              (fifo == 2) ? (C1FIFOSTA3bits.TFNRFNIF == 1) :
                  (fifo == 3) ? (C1FIFOSTA4bits.TFNRFNIF == 1) :
                      (fifo == 4) ? (C1FIFOSTA5bits.TFNRFNIF == 1) :
                          (fifo == 5) ? (C1FIFOSTA6bits.TFNRFNIF == 1) :
                              (fifo == 6) ? (C1FIFOSTA7bits.TFNRFNIF == 1) :
                                  false;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool CAN_FD_RECEIVE_FIFO_IS_NOT_EMPTY(const uint8_t node, const uint8_t fifo)
{
  ASSERT_FATAL(node < CAN_GET_NUMBER_OF_NODES());
  ASSERT_FATAL(fifo < CAN_GET_NUMBER_OF_FIFOS());

  return
      (fifo == 0) ? (C1FIFOSTA1bits.TFNRFNIF == 1) :
          (fifo == 1) ? (C1FIFOSTA2bits.TFNRFNIF == 1) :
              (fifo == 2) ? (C1FIFOSTA3bits.TFNRFNIF == 1) :
                  (fifo == 3) ? (C1FIFOSTA4bits.TFNRFNIF == 1) :
                      (fifo == 4) ? (C1FIFOSTA5bits.TFNRFNIF == 1) :
                          (fifo == 5) ? (C1FIFOSTA6bits.TFNRFNIF == 1) :
                              (fifo == 6) ? (C1FIFOSTA7bits.TFNRFNIF == 1) :
                                  false;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool CAN_FD_RECEIVE_FIFO_OVERFLOW(const uint8_t node, const uint8_t fifo)
{
  ASSERT_FATAL(node < CAN_GET_NUMBER_OF_NODES());
  ASSERT_FATAL(fifo < CAN_GET_NUMBER_OF_FIFOS());

  const bool overflow =
      (fifo == 0) ? (C1FIFOSTA1bits.RXOVIF == 1) :
          (fifo == 1) ? (C1FIFOSTA2bits.RXOVIF == 1) :
              (fifo == 2) ? (C1FIFOSTA3bits.RXOVIF == 1) :
                  (fifo == 3) ? (C1FIFOSTA4bits.RXOVIF == 1) :
                      (fifo == 4) ? (C1FIFOSTA5bits.RXOVIF == 1) :
                          (fifo == 5) ? (C1FIFOSTA6bits.RXOVIF == 1) :
                              (fifo == 6) ? (C1FIFOSTA7bits.RXOVIF == 1) :
                                  true;

  if (overflow) {
    switch (fifo) {
      case 0: C1FIFOSTA1bits.RXOVIF = 0; break;
      case 1: C1FIFOSTA2bits.RXOVIF = 0; break;
      case 2: C1FIFOSTA3bits.RXOVIF = 0; break;
      case 3: C1FIFOSTA4bits.RXOVIF = 0; break;
      case 4: C1FIFOSTA5bits.RXOVIF = 0; break;
      case 5: C1FIFOSTA6bits.RXOVIF = 0; break;
      case 6: C1FIFOSTA7bits.RXOVIF = 0; break;
      default: ASSERT_FATAL(false); break;
    };
  }

  return overflow;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void * CAN_FD_GET_MESSAGE_POINTER(const uint8_t node, const uint8_t fifo)
{
  ASSERT_FATAL(node < CAN_GET_NUMBER_OF_NODES());
  ASSERT_FATAL(fifo < CAN_GET_NUMBER_OF_FIFOS());

  return
      (fifo == 0) ? (void *)C1FIFOUA1L :
          (fifo == 1) ? (void *)C1FIFOUA2L :
              (fifo == 2) ? (void *)C1FIFOUA3L :
                  (fifo == 3) ? (void *)C1FIFOUA4L :
                      (fifo == 4) ? (void *)C1FIFOUA5L :
                          (fifo == 5) ? (void *)C1FIFOUA6L :
                              (fifo == 6) ? (void *)C1FIFOUA7L :
                                  NULL;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void CAN_FD_INCREMENT_FIFO(const uint8_t node, const uint8_t fifo)
{
  ASSERT_FATAL(node < CAN_GET_NUMBER_OF_NODES());
  ASSERT_FATAL(fifo < CAN_GET_NUMBER_OF_FIFOS());

  switch (fifo) {
    case 0: C1FIFOCON1Lbits.UINC = 1; break;
    case 1: C1FIFOCON2Lbits.UINC = 1; break;
    case 2: C1FIFOCON3Lbits.UINC = 1; break;
    case 3: C1FIFOCON4Lbits.UINC = 1; break;
    case 4: C1FIFOCON5Lbits.UINC = 1; break;
    case 5: C1FIFOCON6Lbits.UINC = 1; break;
    case 6: C1FIFOCON7Lbits.UINC = 1; break;
    default: ASSERT_FATAL(false); break;
  };
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void CAN_FD_TRIGGER_TX_FIFO(const uint8_t node, const uint8_t fifo)
{
  ASSERT_FATAL(node < CAN_GET_NUMBER_OF_NODES());
  ASSERT_FATAL(fifo < CAN_GET_NUMBER_OF_FIFOS());

  switch (fifo) {
    case 0: C1FIFOCON1Lbits.TXREQ = 1; break;
    case 1: C1FIFOCON2Lbits.TXREQ = 1; break;
    case 2: C1FIFOCON3Lbits.TXREQ = 1; break;
    case 3: C1FIFOCON4Lbits.TXREQ = 1; break;
    case 4: C1FIFOCON5Lbits.TXREQ = 1; break;
    case 5: C1FIFOCON6Lbits.TXREQ = 1; break;
    case 6: C1FIFOCON7Lbits.TXREQ = 1; break;
    default: ASSERT_FATAL(false); break;
  };
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool CAN_FD_ADD_MESSAGE_TO_FIFO(const uint8_t node, const uint8_t fifo, const t_CAN_MESSAGE * const message)
{
  ASSERT_FATAL(message != NULL);

  const bool done = CAN_FD_TRANSMIT_FIFO_IS_NOT_FULL(node, fifo) ? true : false;
  if (done) {

    t_CAN_FD_TRANSMIT_MESSAGE * entry = (t_CAN_FD_TRANSMIT_MESSAGE *)CAN_FD_GET_MESSAGE_POINTER(node, fifo);
    ASSERT_FATAL(entry != NULL);
    const uint8_t dlc = CanMessageGetDlc(message->fd, message->payload_length);
    const uint8_t padding_length = CanMessageGetLength(message->fd, dlc) - message->payload_length;
    entry->SID = message->identifier;
    entry->FDF = message->fd ? 1 : 0;
    entry->BRS = message->bitrate_switching ? 1 : 0;
    entry->DLC = dlc;
    memcpy(entry->byte, message->data_byte, message->payload_length);
    memset(&entry->byte[message->payload_length], 0, padding_length);
    CAN_FD_INCREMENT_FIFO(node, fifo);
    CAN_FD_TRIGGER_TX_FIFO(node, fifo);
  }

  return done;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool CAN_FD_GET_MESSAGE_FROM_FIFO(const uint8_t node, const uint8_t fifo, t_CAN_MESSAGE * const message)
{
  ASSERT_FATAL(message != NULL);

  const bool new_message = CAN_FD_RECEIVE_FIFO_IS_NOT_EMPTY(node, fifo) ? true : false;
  if (new_message) {

    t_CAN_FD_RECEIVE_MESSAGE * entry = (t_CAN_FD_RECEIVE_MESSAGE *)CAN_FD_GET_MESSAGE_POINTER(node, fifo);
    ASSERT_FATAL(entry != NULL);
    message->identifier = entry->SID;
    message->fd = entry->FDF == 1;
    message->bitrate_switching = entry->BRS == 1;
    message->payload_length = CanMessageGetLength(message->fd, (uint8_t)entry->DLC);
    memcpy(message->data_byte, entry->byte, message->payload_length);
    CAN_FD_INCREMENT_FIFO(node, fifo);

    // IME: core - dsPIC33C - can - check for too big payload
  }

  return new_message;
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
