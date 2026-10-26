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
  SPI_CLOCK_STATE_LOW,
  SPI_CLOCK_STATE_HIGH,
  SPI_CLOCK_IDLE_INVALID
} e_SPI_CLOCK_STATE;

typedef enum {
  SPI_CLOCK_EDGE_RISING,
  SPI_CLOCK_EDGE_FALLING,
  SPI_CLOCK_EDGE_INVALID
} e_SPI_CLOCK_EDGE;

typedef struct {
  bool master;
  bool transfer_two_byte;
  bool use_clock_pin;
  bool use_sdo_pin;
  bool use_sdi_pin;
  bool use_slave_select_pin;
  e_SPI_CLOCK_STATE clock_state_idle;
  e_SPI_CLOCK_EDGE clock_edge_for_input_read;
  e_SPI_CLOCK_EDGE clock_edge_for_output_change;
  uint32_t bitrate;
} t_SPI_SETTINGS;

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

void SpiInit(const int node, const t_SPI_SETTINGS * const settings);

COMPILER_INLINE void SPI_SET_SIGNAL_LOGIC(const int node, const e_SPI_CLOCK_STATE clock_state_idle, const e_SPI_CLOCK_EDGE clock_edge_for_input_read, const e_SPI_CLOCK_EDGE clock_edge_for_output_change);
COMPILER_INLINE void SPI_SET_BITRATE(const int node, const uint32_t bitrate);
COMPILER_INLINE void SPI_SET_WORD_LENGTH(const int node, const uint16_t number_of_bits);
COMPILER_INLINE void SPI_USE_FRAMED_COMMUNICATION(const int node, const bool master, const bool high_active);

COMPILER_INLINE void SPI_ACTIVATE(const int node);
COMPILER_INLINE void SPI_DEACTIVATE(const int node);

COMPILER_INLINE bool SPI_IS_INTERRUPT_PENDING(const int node);
COMPILER_INLINE void SPI_CONFIRM_INTERRUPT(const int node);

COMPILER_INLINE bool SPI_TRANSMIT_BUFFER_IS_FULL(const int node);
COMPILER_INLINE bool SPI_RECEIVE_BUFFER_IS_FULL(const int node);

COMPILER_INLINE void SPI_FLUSH(const int node);

COMPILER_INLINE void SPI_TRANSMIT_16_BITS(const int node, const uint16_t transmit);
COMPILER_INLINE uint16_t SPI_RECEIVE_16_BITS(const int node);

/*============================================================================*/
/*  I M P L E M E N T A T I O N                                               */
/*============================================================================*/

#include "SpiImpl.h"

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
