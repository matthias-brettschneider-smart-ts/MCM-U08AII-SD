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
#define CONTROLLER_DSPIC33EP128GS804_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP508_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP506_SUPPORTED

#include "hal/ICompiler.h"
#include "hal/IController.h"
#include "middleware/IAssert.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"
#include "middleware/IWait.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

#define SPI_NUMBER_OF_NODES                                                    2

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void SPI_SET_SIGNAL_LOGIC(const int node, const e_SPI_CLOCK_STATE clock_state_idle, const e_SPI_CLOCK_EDGE clock_edge_for_input_read, const e_SPI_CLOCK_EDGE clock_edge_for_output_change)
{
  ASSERT_FATAL(node < SPI_NUMBER_OF_NODES);
  ASSERT_FATAL((clock_state_idle == SPI_CLOCK_STATE_LOW) || (clock_state_idle == SPI_CLOCK_STATE_HIGH));
  ASSERT_FATAL((clock_edge_for_input_read == SPI_CLOCK_EDGE_RISING) || (clock_edge_for_input_read == SPI_CLOCK_EDGE_FALLING));
  ASSERT_FATAL((clock_edge_for_output_change == SPI_CLOCK_EDGE_RISING) || (clock_edge_for_output_change == SPI_CLOCK_EDGE_FALLING));

  // input_sample | output change | clock idle || CKP | CKE | SMP
  // -------------+---------------+------------++-----+-----+-----
  //      R       |       R       |      L     ||  0  |  0  |  1
  //      R       |       R       |      H     ||  1  |  1  |  1
  //      R       |       F       |      L     ||  0  |  1  |  0
  //      R       |       F       |      H     ||  1  |  0  |  0
  //      F       |       R       |      L     ||  0  |  0  |  0
  //      F       |       R       |      H     ||  1  |  1  |  0
  //      F       |       F       |      L     ||  0  |  1  |  1
  //      F       |       F       |      H     ||  1  |  0  |  1
  //
  const bool clock_idle_high = clock_state_idle == SPI_CLOCK_STATE_HIGH;
  const bool change_output_from_active_to_idle =
        ((clock_state_idle == SPI_CLOCK_STATE_LOW) && (clock_edge_for_output_change == SPI_CLOCK_EDGE_FALLING)) ||
        ((clock_state_idle == SPI_CLOCK_STATE_HIGH) && (clock_edge_for_output_change == SPI_CLOCK_EDGE_RISING));
  const bool sample_input_at_the_end_of_data_output_time = clock_edge_for_input_read == clock_edge_for_output_change;

  if (node == 0) {
    SPI1CON1bits.CKP = clock_idle_high ? 1 : 0;
    SPI1CON1bits.CKE = change_output_from_active_to_idle ? 1 : 0;
    SPI1CON1bits.SMP = sample_input_at_the_end_of_data_output_time ? 1 : 0;
  }
  else {
    SPI2CON1bits.CKP = clock_idle_high ? 1 : 0;
    SPI2CON1bits.CKE = change_output_from_active_to_idle ? 1 : 0;
    SPI2CON1bits.SMP = sample_input_at_the_end_of_data_output_time ? 1 : 0;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
COMPILER_INLINE void SPI_SET_BITRATE_INTERNAL(const int node, uint8_t primary, uint8_t secondary)
{
  const uint8_t ppre = (primary == 1) ? 3 : (primary == 4) ? 2 : (primary == 16) ? 1 : (primary == 64) ? 0 : UINT8_MAX;
  const uint8_t spre = (secondary == 1) ? 7 : (secondary == 2) ? 6 : (secondary == 3) ? 5 : (secondary == 4) ? 4 : (secondary == 5) ? 3 :
      (secondary == 6) ? 2 : (secondary == 7) ? 1 : (secondary == 8) ? 0 : UINT8_MAX;

  ASSERT_FATAL(ppre != UINT8_MAX);
  ASSERT_FATAL(spre != UINT8_MAX);

  switch (node) {
    case 0: SPI1CON1bits.PPRE = ppre; SPI1CON1bits.SPRE = spre; break;
    case 1: SPI2CON1bits.PPRE = ppre; SPI2CON1bits.SPRE = spre; break;
    default: ASSERT_FATAL(false); break;
  }
}
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
COMPILER_INLINE void SPI_SET_BITRATE_INTERNAL(const int node, uint8_t divider)
{
  ASSERT_FATAL((divider % 2) == 0);
  const uint8_t divider_half = divider / 2;
  ASSERT_FATAL(divider_half >= 1);
  const uint8_t brgl = divider_half - 1;

  switch (node) {
    case 0: SPI1BRGL = brgl; break;
    case 1: SPI2BRGL = brgl; break;
    default: ASSERT_FATAL(false); break;
  }
}
#endif

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void SPI_SET_BITRATE(const int node, const uint32_t bitrate)
{
  ASSERT_FATAL(node < SPI_NUMBER_OF_NODES);

  if (bitrate == 0) {} // valid value in slave mode
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
  else if ((bitrate ==   500000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) SPI_SET_BITRATE_INTERNAL(node, 16, 5); // 16 x 5 = 80
  else if ((bitrate ==   625000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) SPI_SET_BITRATE_INTERNAL(node, 16, 4); // 16 x 4 = 64
  else if ((bitrate ==  2500000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) SPI_SET_BITRATE_INTERNAL(node,  4, 4); //  4 x 4 = 16
  else if ((bitrate ==  3333333) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) SPI_SET_BITRATE_INTERNAL(node,  4, 3); //  4 x 3 = 12
  else if ((bitrate ==  5000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) SPI_SET_BITRATE_INTERNAL(node,  4, 2); //  4 x 2 =  8
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
  else if ((bitrate ==  1000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) SPI_SET_BITRATE_INTERNAL(node,  40);
  else if ((bitrate ==  2000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) SPI_SET_BITRATE_INTERNAL(node,  20);
  else if ((bitrate ==  5000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) SPI_SET_BITRATE_INTERNAL(node,   8);
  else if ((bitrate ==  6666667) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) SPI_SET_BITRATE_INTERNAL(node,   6);
  else if ((bitrate == 10000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) SPI_SET_BITRATE_INTERNAL(node,   4);
  else if ((bitrate == 20000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) SPI_SET_BITRATE_INTERNAL(node,   2);
  else if ((bitrate ==  1000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) SPI_SET_BITRATE_INTERNAL(node,  80);
  else if ((bitrate ==  2000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) SPI_SET_BITRATE_INTERNAL(node,  40);
  else if ((bitrate ==  5000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) SPI_SET_BITRATE_INTERNAL(node,  16);
  else if ((bitrate ==  6666667) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) SPI_SET_BITRATE_INTERNAL(node,  12);
  else if ((bitrate == 10000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) SPI_SET_BITRATE_INTERNAL(node,   8);
  else if ((bitrate == 20000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) SPI_SET_BITRATE_INTERNAL(node,   4);
  else if ((bitrate == 40000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) SPI_SET_BITRATE_INTERNAL(node,   2);
  else if ((bitrate ==  1000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) SPI_SET_BITRATE_INTERNAL(node, 100);
  else if ((bitrate ==  2000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) SPI_SET_BITRATE_INTERNAL(node,  50);
  else if ((bitrate ==  5000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) SPI_SET_BITRATE_INTERNAL(node,  20);
  else if ((bitrate ==  6250000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) SPI_SET_BITRATE_INTERNAL(node,  16);
  else if ((bitrate == 10000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) SPI_SET_BITRATE_INTERNAL(node,  10);
  else if ((bitrate == 20000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) SPI_SET_BITRATE_INTERNAL(node,   5);
  else if ((bitrate == 25000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) SPI_SET_BITRATE_INTERNAL(node,   4);
  else if ((bitrate == 50000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) SPI_SET_BITRATE_INTERNAL(node,   2);
#endif
  else ASSERT_FATAL(false);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void SPI_SET_WORD_LENGTH(const int node, const uint16_t number_of_bits)
{

#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)

  // dsPIC33FJ256GP710 does not support variable symbol lengths (other than 8 vs. 16 bit)
  (void)node;
  (void)number_of_bits;
  ASSERT_FATAL(false);

#endif

  ASSERT_FATAL((number_of_bits >= 2) && (number_of_bits <= 16));

  switch (node) {
#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: SPI1CON2Lbits.WLENGTH = number_of_bits - 1; break;
    case 1: SPI2CON2Lbits.WLENGTH = number_of_bits - 1; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Configure and enable framed communication.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void SPI_USE_FRAMED_COMMUNICATION(const int node, const bool master, const bool high_active)
{
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)

  switch (node) {
    case 0: SPI1CON2bits.SPIFSD = master ? 0 : 1; SPI1CON2bits.FRMPOL = high_active ? 1 : 0; SPI1CON2bits.FRMDLY = 0; SPI1CON1bits.SSEN = 0; SPI1CON2bits.FRMEN = 1; break;
    case 1: SPI2CON2bits.SPIFSD = master ? 0 : 1; SPI2CON2bits.FRMPOL = high_active ? 1 : 0; SPI2CON2bits.FRMDLY = 0; SPI2CON1bits.SSEN = 0; SPI2CON2bits.FRMEN = 1; break;
    default: ASSERT_FATAL(false); break;
  }

#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))

  // IME: core - SPI - not implemented for dsPIC33EP128GS804 & dsPIC3CK256MP508 yet
  (void)node;
  (void)master;
  (void)high_active;
  ASSERT_FATAL(false);

#endif
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Activate SPI node.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void SPI_ACTIVATE(const int node)
{
  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: SPI1STATbits.SPIEN = 1; break;
    case 1: SPI2STATbits.SPIEN = 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: SPI1CON1bits.SPIEN = 1; break;
    case 1: SPI2CON1bits.SPIEN = 1; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Deactivate SPI node.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void SPI_DEACTIVATE(const int node)
{
  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: SPI1STATbits.SPIEN = 0; break;
    case 1: SPI2STATbits.SPIEN = 0; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: SPI1CON1bits.SPIEN = 0; break;
    case 1: SPI2CON1bits.SPIEN = 0; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Check SPI node for pending interrupt.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool SPI_IS_INTERRUPT_PENDING(const int node)
{

#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))

  // IME: core - SPI - not tested on dsPIC33EP128GS804 & dsPIC33CK256MP508 yet
  ASSERT_FATAL(false);

#endif

  bool pending = false;

  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: pending = _SPI1IF != 0; break;
    case 1: pending = _SPI2IF != 0; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: pending = (_SPI1RXIF != 0) || (_SPI1TXIF != 0); break;
    case 1: pending = (_SPI2RXIF != 0) || (_SPI2TXIF != 0); break;
#endif
    default: ASSERT_FATAL(false); break;
  }

  return pending;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Confirm pending interrupt for SPI node
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void SPI_CONFIRM_INTERRUPT(const int node)
{
  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: _SPI1IF = 0; break;
    case 1: _SPI2IF = 0; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: _SPI1RXIF = 0; _SPI1TXIF = 0; break;
    case 1: _SPI2RXIF = 0; _SPI2TXIF = 0; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void SPI_FLUSH(const int node)
{
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)

  // dummy read
  volatile uint16_t dummy = SPI_RECEIVE_16_BITS(node);
  (void)dummy;

#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))

  // IME: core - SPI - not implemented for dsPIC33EP128GS804 & dsPIC33CK256MP508 yet
  (void)node;
  ASSERT_FATAL(false);

#if false
  // clear RX FIFO
  const uint32_t endless_loop_timer = WAIT_GET_TIMER_VALUE_32();
  volatile uint16_t dumme_read;
  while (SPI1STATHbits.RXELM != 0) {
    dumme_read = SPI1BUFL;
    dumme_read = SPI1BUFH;
    if (WAIT_CHECK_TIMEOUT_MS_32(endless_loop_timer, 1000))
      ErrorAddEntry(ERROR_ID_FATAL_ENDLESS_LOOP, 0);
  }
#endif

#endif
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool SPI_TRANSMIT_BUFFER_IS_FULL(const int node)
{
  bool full = false;

  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: full = SPI1STATbits.SPITBF == 1; break;
    case 1: full = SPI2STATbits.SPITBF == 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: full = SPI1STATLbits.SPITBF == 1; break;
    case 1: full = SPI2STATLbits.SPITBF == 1; break;
#endif
    default: ASSERT_FATAL(false); break;
  }

  return full;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool SPI_RECEIVE_BUFFER_IS_FULL(const int node)
{
  bool full = false;

  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: full = SPI1STATbits.SPIRBF == 1; break;
    case 1: full = SPI2STATbits.SPIRBF == 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: full = SPI1STATLbits.SPIRBF == 1; break;
    case 1: full = SPI2STATLbits.SPIRBF == 1; break;
#endif
    default: ASSERT_FATAL(false); break;
  }

  return full;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Send word MSB first.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void SPI_TRANSMIT_16_BITS(const int node, const uint16_t transmit)
{
  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: SPI1BUF = transmit; break;
    case 1: SPI2BUF = transmit; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: SPI1BUFL = transmit; break;
    case 1: SPI2BUFL = transmit; break;
#endif
    default: ASSERT_FATAL(false); break;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Receive word MSB first.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint16_t SPI_RECEIVE_16_BITS(const int node)
{
  uint16_t receive = 0;

  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
    case 0: receive = SPI1BUF; break;
    case 1: receive = SPI2BUF; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: receive = SPI1BUFL; break;
    case 1: receive = SPI2BUFL; break;
#endif
    default: ASSERT_FATAL(false); break;
  }

  return receive;
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
