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

#define CONTROLLER_DSPIC33CK256MP508_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP506_SUPPORTED

#include "hal/ICan.h"
#include "hal/ICompiler.h"
#include "hal/IController.h"
#include "hal/IOscillator.h"
#include "middleware/IAssert.h"
#include "middleware/ICanMessage.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"

#include "CanFdBitrate.h"
#include "CanFdFifo.h"
#include "CanFdFilter.h"
#include "CanFdMessages.h"

#ifndef CAN_SUPPORTS_8_MBPS
#error "error compile flags - CAN_SUPPORTS_8_MBPS undefined"
#elif ((CAN_SUPPORTS_8_MBPS != false) && (CAN_SUPPORTS_8_MBPS != true))
#error "error compile flags - CAN_SUPPORTS_8_MBPS invalid"
#endif

#ifndef CAN_NUMBER_OF_MESSAGES_TRANSMIT_PROTOCOL
#error "error compile flags - CAN_NUMBER_OF_MESSAGES_TRANSMIT_PROTOCOL undefined"
#elif ((CAN_NUMBER_OF_MESSAGES_TRANSMIT_PROTOCOL < 0) || (CAN_NUMBER_OF_MESSAGES_TRANSMIT_PROTOCOL > 32))
#error "error compile flags - CAN_NUMBER_OF_MESSAGES_TRANSMIT_PROTOCOL invalid"
#endif

#ifndef CAN_NUMBER_OF_MESSAGES_TRANSMIT_APPLICATION
#error "error compile flags - CAN_NUMBER_OF_MESSAGES_TRANSMIT_APPLICATION undefined"
#elif ((CAN_NUMBER_OF_MESSAGES_TRANSMIT_APPLICATION < 0) || (CAN_NUMBER_OF_MESSAGES_TRANSMIT_APPLICATION > 32))
#error "error compile flags - CAN_NUMBER_OF_MESSAGES_TRANSMIT_APPLICATION invalid"
#endif

#ifndef CAN_NUMBER_OF_MESSAGES_TRANSMIT_OTHER
#error "error compile flags - CAN_NUMBER_OF_MESSAGES_TRANSMIT_OTHER undefined"
#elif ((CAN_NUMBER_OF_MESSAGES_TRANSMIT_OTHER < 0) || (CAN_NUMBER_OF_MESSAGES_TRANSMIT_OTHER > 32))
#error "error compile flags - CAN_NUMBER_OF_MESSAGES_TRANSMIT_OTHER invalid"
#endif

#ifndef CAN_NUMBER_OF_MESSAGES_RECEIVE_PROTOCOL
#error "error compile flags - CAN_NUMBER_OF_MESSAGES_RECEIVE_PROTOCOL undefined"
#elif ((CAN_NUMBER_OF_MESSAGES_RECEIVE_PROTOCOL < 0) || (CAN_NUMBER_OF_MESSAGES_RECEIVE_PROTOCOL > 32))
#error "error compile flags - CAN_NUMBER_OF_MESSAGES_RECEIVE_PROTOCOL invalid"
#endif

#ifndef CAN_NUMBER_OF_MESSAGES_RECEIVE_APPLICATION
#error "error compile flags - CAN_NUMBER_OF_MESSAGES_RECEIVE_APPLICATION undefined"
#elif ((CAN_NUMBER_OF_MESSAGES_RECEIVE_APPLICATION < 0) || (CAN_NUMBER_OF_MESSAGES_RECEIVE_APPLICATION > 32))
#error "error compile flags - CAN_NUMBER_OF_MESSAGES_RECEIVE_APPLICATION invalid"
#endif

#ifndef CAN_NUMBER_OF_MESSAGES_RECEIVE_OTHER
#error "error compile flags - CAN_NUMBER_OF_MESSAGES_RECEIVE_OTHER undefined"
#elif ((CAN_NUMBER_OF_MESSAGES_RECEIVE_OTHER < 0) || (CAN_NUMBER_OF_MESSAGES_RECEIVE_OTHER > 32))
#error "error compile flags - CAN_NUMBER_OF_MESSAGES_RECEIVE_OTHER invalid"
#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef struct {
  t_CAN_FD_TRANSMIT_MESSAGE transmit_1[CAN_NUMBER_OF_MESSAGES_TRANSMIT_PROTOCOL];
  t_CAN_FD_TRANSMIT_MESSAGE transmit_2[CAN_NUMBER_OF_MESSAGES_TRANSMIT_APPLICATION];
  t_CAN_FD_TRANSMIT_MESSAGE transmit_3[CAN_NUMBER_OF_MESSAGES_TRANSMIT_OTHER];
  t_CAN_FD_RECEIVE_MESSAGE receive_1[CAN_NUMBER_OF_MESSAGES_RECEIVE_PROTOCOL];
  t_CAN_FD_RECEIVE_MESSAGE receive_2[CAN_NUMBER_OF_MESSAGES_RECEIVE_APPLICATION];
  t_CAN_FD_RECEIVE_MESSAGE receive_3[CAN_NUMBER_OF_MESSAGES_RECEIVE_OTHER];
} t_CAN_FD_FIFO;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_CAN_FD_FIFO COMPILER_ALIGNED_4_BYTE can_fd_fifo;

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static void CanFdInitNode(const uint8_t node);
static void CanFdSetConfigurationMode(const uint8_t node, const bool active);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
void CanInit(void)
{
  ASSERT_STATIC(sizeof(t_CAN_FD_TRANSMIT_MESSAGE) == (8 + CAN_FD_PAYLOAD_IN_BYTES));
  ASSERT_STATIC((sizeof(t_CAN_FD_TRANSMIT_MESSAGE) % 4) == 0);
  ASSERT_STATIC(sizeof(t_CAN_FD_RECEIVE_MESSAGE) == (12 + CAN_FD_PAYLOAD_IN_BYTES));
  ASSERT_STATIC((sizeof(t_CAN_FD_RECEIVE_MESSAGE) % 4) == 0);
  ASSERT_STATIC(((uint16_t)&can_fd_fifo.transmit_1 % 4) == 0);
  ASSERT_STATIC(((uint16_t)&can_fd_fifo.transmit_2 % 4) == 0);
  ASSERT_STATIC(((uint16_t)&can_fd_fifo.transmit_3 % 4) == 0);
  ASSERT_STATIC(((uint16_t)&can_fd_fifo.receive_1 % 4) == 0);
  ASSERT_STATIC(((uint16_t)&can_fd_fifo.receive_2 % 4) == 0);
  ASSERT_STATIC(((uint16_t)&can_fd_fifo.receive_3 % 4) == 0);
  ASSERT_STATIC((sizeof(can_fd_fifo.transmit_1) % 4) == 0);
  ASSERT_STATIC((sizeof(can_fd_fifo.transmit_2) % 4) == 0);
  ASSERT_STATIC((sizeof(can_fd_fifo.transmit_3) % 4) == 0);
  ASSERT_STATIC((sizeof(can_fd_fifo.receive_1) % 4) == 0);
  ASSERT_STATIC((sizeof(can_fd_fifo.receive_2) % 4) == 0);
  ASSERT_STATIC((sizeof(can_fd_fifo.receive_3) % 4) == 0);

  CAN_FD_CONFIGURE_PERIPHERY_CLOCK();
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Initialize node.
 *
 -----------------------------------------------------------------------------*/
void CanInitNode(const uint8_t node)
{
  CanFdInitNode(node);

  CanFdSetConfigurationMode(node, true);
  {
    CAN_FD_CONFIGURE_FIFO(node, 0, true, CAN_FD_PAYLOAD_IN_BYTES, CAN_NUMBER_OF_MESSAGES_TRANSMIT_PROTOCOL);
    CAN_FD_CONFIGURE_FIFO(node, 1, true, CAN_FD_PAYLOAD_IN_BYTES, CAN_NUMBER_OF_MESSAGES_TRANSMIT_APPLICATION);
    CAN_FD_CONFIGURE_FIFO(node, 2, true, CAN_FD_PAYLOAD_IN_BYTES, CAN_NUMBER_OF_MESSAGES_TRANSMIT_OTHER);
    CAN_FD_CONFIGURE_FIFO(node, 3, false, CAN_FD_PAYLOAD_IN_BYTES, CAN_NUMBER_OF_MESSAGES_RECEIVE_PROTOCOL);
    CAN_FD_CONFIGURE_FIFO(node, 4, false, CAN_FD_PAYLOAD_IN_BYTES, CAN_NUMBER_OF_MESSAGES_RECEIVE_APPLICATION);
    CAN_FD_CONFIGURE_FIFO(node, 5, false, CAN_FD_PAYLOAD_IN_BYTES, CAN_NUMBER_OF_MESSAGES_RECEIVE_OTHER);
  }
  CanFdSetConfigurationMode(node, false);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Activate or deactivate the configuration mode.
 *
 -----------------------------------------------------------------------------*/
static void CanFdSetConfigurationMode(const uint8_t node, const bool active)
{
  ASSERT_FATAL(node < CAN_GET_NUMBER_OF_NODES());

  C1CONHbits.REQOP = active ? 4 : 0;              // reqest mode change
  while (C1CONHbits.OPMOD != (active ? 4 : 0));   // wait for mode change
  C1INTLbits.MODIF = 0;                           // clear interrupt flag for mode change
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
static void CanFdInitNode(const uint8_t node)
{
  ASSERT_FATAL(node < CAN_GET_NUMBER_OF_NODES());

  C1CONLbits.CON = 1;                             // enable CANFD module

  CanFdSetConfigurationMode(node, true);
  {
    C1CONHbits.TXQEN = 0;                         // do not reserve RAM for TXQ
    C1CONHbits.STEF = 0;                          // do not save transmitted messages in TEF
    C1CONHbits.RTXAT = 0;                         // unlimited number of retransmission attempts
    C1CONHbits.TXBWS = 0;                         // no delay for transmit bandwidth sharing // IME: core - dsPIC33C - can - transmit bandwidth sharing?
    C1CONLbits.WAKFIL = 0;                        // bus line filter is not used for wake-up
    C1CONLbits.DNCNT = 0;                         // disable data byte filtering (DeviceNet)
    C1CONLbits.BRSDIS = 0;                        // bit rate switching depends on transmit message object
    C1CONLbits.ISOCRCEN = 1;                      // enable ISO CRC in CAN FD frames // IME: core - dsPIC33C - can - ISO CRC?
    C1TDCHbits.SID11EN = 0;                       // do not use 12 bit SID
    C1TDCHbits.EDGFLTEN = 0;                      // disable edge filtering // IME: core - dsPIC33C - can - edge filtering?
    C1TDCHbits.TDCMOD = 2;                        // transmitter Delay Compensation auto // IME: core - dsPIC33C - can - transmitter delay compensation?
    C1TDCLbits.TDCO = 15;                         // transmitter Delay Compensation 15 x Tcan
    C1TDCLbits.TDCV = 0;                          // transmitter Delay Compensation0 x Tcan

    C1TSCONHbits.TBCEN = 1;                       // enable time base counter
    C1TSCONLbits.TBCPRE = 80 - 1;                 // increment time base every 80 clocks

    C1FIFOBAL = (uint16_t)&can_fd_fifo;           // base address for the FIFO followed by the message objects
  }
  CanFdSetConfigurationMode(node, false);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool CanSetNominalBitrate(const uint8_t node, const uint16_t bitrate_in_kbps)
{
  bool done = true;

  CanFdSetConfigurationMode(node, true);
  {
    done = CAN_SET_NOMINAL_BIT_RATE(node, bitrate_in_kbps);
  }
  CanFdSetConfigurationMode(node, false);

  return done;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool CanSetDataBitrate(const uint8_t node, const uint16_t bitrate_in_kbps)
{
  bool done = true;

  CanFdSetConfigurationMode(node, true);
  {
    done = CAN_SET_DATA_BIT_RATE(node, bitrate_in_kbps);
  }
  CanFdSetConfigurationMode(node, false);

  return done;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool CanSetAcceptanceFilter(const uint8_t node, const uint8_t filter, const uint16_t identifier, const uint8_t fifo)
{
  ASSERT_FATAL((identifier >= 1) && (identifier <= 0x7FF));
  ASSERT_FATAL((fifo >= 3) && (fifo <= 5));

  const bool identifier_has_to_match = fifo != 5;
  const uint16_t mask = identifier_has_to_match ? 0x7FF : 0;

  CAN_FD_CONFIGURE_FILTER(node, filter, mask, identifier, fifo);

  return true; // IME: core - dsPIC33C - can -
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool CanTransmitMessage(const uint8_t node, const uint8_t fifo, const t_CAN_MESSAGE * const message)
{
  ASSERT_FATAL(fifo <= 2);

  const bool done = CAN_FD_ADD_MESSAGE_TO_FIFO(node, fifo, message);
  return done;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool CanReceiveMessage(const uint8_t node, const uint8_t fifo, t_CAN_MESSAGE * const message)
{
  ASSERT_FATAL((fifo >= 3) && (fifo <= 5));

  if (CAN_FD_RECEIVE_FIFO_OVERFLOW(node, fifo))
    ErrorAddEntry(ERROR_ID_CAN_RX_BUFFER_OVERRUN, (node == 0) ? 0x10 : 0x20);

  const bool new_message = CAN_FD_GET_MESSAGE_FROM_FIFO(node, fifo, message);
  return new_message;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
void CanTrigger(void)
{
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
