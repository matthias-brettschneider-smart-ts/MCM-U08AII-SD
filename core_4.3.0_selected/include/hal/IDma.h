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
  DMA_PERIPHERY_CAN1_TX,
  DMA_PERIPHERY_CAN1_RX,
  DMA_PERIPHERY_CAN2_TX,
  DMA_PERIPHERY_CAN2_RX,
  DMA_PERIPHERY_UART1_TX,
  DMA_PERIPHERY_UART1_RX,
  DMA_PERIPHERY_UART2_TX,
  DMA_PERIPHERY_UART2_RX,
  DMA_PERIPHERY_SPI1,
  DMA_PERIPHERY_SPI2,
  DMA_PERIPHERY_UNKNOWN
} e_DMA_PERIPHERY;

typedef enum {
  DMA_ADDRESSING_REGISTER_INDIRECT_WITH_POST_INCREMENT,
  DMA_ADDRESSING_REGISTER_INDIRECT_WITHOUT_POST_INCREMENT,
  DMA_ADDRESSING_PERIPHERAL_INDIRECT
} e_DMA_ADDRESSING;

typedef struct {
  bool transfer_to_periphery;
  bool transfer_continuous;
  bool two_byte_transfer;
  uint16_t number_of_bytes_per_transfer;
  e_DMA_PERIPHERY periphery;
  e_DMA_ADDRESSING transfer_addressing;
  void * ram_address;
} t_DMA_SETTINGS;

#define DMA_RAM                              DMA_RAM_INTERNAL

#define DMA_ISR(number)                      void DMA_ISR_INTERNAL(number)(void)

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

void DmaConfigureChannel(const int channel, const t_DMA_SETTINGS * settings);

COMPILER_INLINE void DMA_ACTIVATE_CHANNEL(const int channel);
COMPILER_INLINE void DMA_DEACTIVATE_CHANNEL(const int channel);
COMPILER_INLINE void DMA_SET_RAM_ADDRESS(const int channel, const void * const address);
COMPILER_INLINE void DMA_ACTIVATE_INTERRUPT(const int channel, const int priority);
COMPILER_INLINE bool DMA_IS_INTERRUPT_PENDING(const int channel);
COMPILER_INLINE void DMA_CONFIRM_INTERRUPT(const int channel);
COMPILER_INLINE void DMA_TRIGGER_CHANNEL(const int channel);

/*============================================================================*/
/*  I M P L E M E N T A T I O N                                               */
/*============================================================================*/

#include "DmaImpl.h"

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
