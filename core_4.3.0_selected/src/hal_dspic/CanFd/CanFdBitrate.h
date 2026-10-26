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
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

#define CAN_FD_PERIPHERAL_FREQUENCY_IN_HZ                               80000000

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void CAN_FD_CONFIGURE_PERIPHERY_CLOCK(void)
{
  ASSERT_FATAL((OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 40000000) || (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 80000000) || (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000));

  // configure peripheral clock independent from current instruction frequency
  ASSERT_STATIC(CAN_FD_PERIPHERAL_FREQUENCY_IN_HZ == 80000000);

  CANCLKCONbits.CANCLKSEL =
      (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000) ? 2 : // use Fpllo = 4 x Fcy = 160 MHz
      (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000) ? 2 : // use Fpllo = 4 x Fcy = 320 MHz
      (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000) ? 4 : // use Fvco / 3        = 400 MHz
      0;
  ASSERT_FATAL(CANCLKCONbits.CANCLKSEL != 0);

  CANCLKCONbits.CANCLKDIV =
      (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000) ? (2 - 1) : // 160 MHz / 2 = 80 MHz
      (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000) ? (4 - 1) : // 320 MHz / 4 = 80 MHz
      (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000) ? (5 - 1) : // 400 MHz / 5 = 80 MHz
      0;
  ASSERT_FATAL(CANCLKCONbits.CANCLKDIV != 0);

  CANCLKCONbits.CANCLKEN = 1; // enable CAN clock generation circuit
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool CAN_FD_SET_NOMINAL_BIT_TIMING(const uint8_t node, const uint16_t brp, const uint16_t tseg1, const uint16_t tseg2, const uint16_t sjw)
{
  ASSERT_FATAL(node < CAN_GET_NUMBER_OF_NODES());
  ASSERT_FATAL((brp >= 1) && (brp <= 256));
  ASSERT_FATAL((tseg1 >= 2) && (tseg1 <= 256));
  ASSERT_FATAL((tseg2 >= 1) && (tseg2 <= 128));
  ASSERT_FATAL((sjw >= 1) && (sjw <= 128));
  ASSERT_FATAL(sjw <= tseg1);
  ASSERT_FATAL(sjw <= tseg2);

  const uint16_t before = 1 + tseg1;
  const uint16_t after = tseg2;
  const bool sample_point_is_80_percent = before == (after * 4);
  ASSERT_FATAL(sample_point_is_80_percent);

  const bool change =
      (C1NBTCFGHbits.BRP != brp - 1) ||
      (C1NBTCFGLbits.SJW != sjw - 1) ||
      (C1NBTCFGHbits.TSEG1 != tseg1 - 1) ||
      (C1NBTCFGLbits.TSEG2 != tseg2 - 1);

  if (change) {
    C1NBTCFGHbits.BRP = brp - 1;
    C1NBTCFGLbits.SJW = sjw - 1;
    C1NBTCFGHbits.TSEG1 = tseg1 - 1;
    C1NBTCFGLbits.TSEG2 = tseg2 - 1;
  }

  return change;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool CAN_SET_NOMINAL_BIT_RATE(const uint8_t node, const uint16_t bitrate_in_kbps)
{
  bool done = false;

  ASSERT_STATIC(CAN_FD_PERIPHERAL_FREQUENCY_IN_HZ == 80000000);
  switch (bitrate_in_kbps) {
    case  125: done = CAN_FD_SET_NOMINAL_BIT_TIMING(node, 2, 255, 64, 64); break;
    case  250: done = CAN_FD_SET_NOMINAL_BIT_TIMING(node, 1, 255, 64, 64); break;
    case  500: done = CAN_FD_SET_NOMINAL_BIT_TIMING(node, 1, 127, 32, 32); break;
    case 1000: done = CAN_FD_SET_NOMINAL_BIT_TIMING(node, 1,  63, 16, 16); break;
    default: break;
  }

  return done;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool CAN_FD_SET_DATA_BIT_TIMING(const uint8_t node, const uint16_t brp, const uint16_t tseg1, const uint16_t tseg2, const uint16_t sjw)
{
  ASSERT_FATAL(node < CAN_GET_NUMBER_OF_NODES());
  ASSERT_FATAL((brp >= 1) && (brp <= 256));
  ASSERT_FATAL((tseg1 >= 1) && (tseg1 <= 32));
  ASSERT_FATAL((tseg2 >= 1) && (tseg2 <= 16));
  ASSERT_FATAL((sjw >= 1) && (sjw <= 16));
  ASSERT_FATAL(sjw <= tseg1);
  ASSERT_FATAL(sjw <= tseg2);

  const uint16_t before = 1 + tseg1;
  const uint16_t after = tseg2;
  const bool sample_point_is_80_percent = (before * 1) == (after * 4);
  const bool sample_point_is_81_point_25_percent = (before * 3) == (after * 13);
  ASSERT_FATAL(sample_point_is_80_percent || sample_point_is_81_point_25_percent);

  const bool change =
      (C1DBTCFGHbits.BRP != brp - 1) ||
      (C1DBTCFGLbits.SJW != sjw - 1) ||
      (C1DBTCFGHbits.TSEG1 != tseg1 - 1) ||
      (C1DBTCFGLbits.TSEG2 != tseg2 - 1);

  if (change) {
    C1DBTCFGHbits.BRP = brp - 1;
    C1DBTCFGLbits.SJW = sjw - 1;
    C1DBTCFGHbits.TSEG1 = tseg1 - 1;
    C1DBTCFGLbits.TSEG2 = tseg2 - 1;
  }

  return change;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool CAN_SET_DATA_BIT_RATE(const uint8_t node, const uint16_t bitrate_in_kbps)
{
  bool done = false;

  ASSERT_STATIC(CAN_FD_PERIPHERAL_FREQUENCY_IN_HZ == 80000000);
  switch (bitrate_in_kbps) {
    case 2000: done = CAN_FD_SET_DATA_BIT_TIMING(node, 1, 31,  8,  8); break;
    case 4000: done = CAN_FD_SET_DATA_BIT_TIMING(node, 1, 15,  4,  4); break;
    case 5000: done = CAN_FD_SET_DATA_BIT_TIMING(node, 1, 12,  3,  3); break;
    case 8000: done = CAN_FD_SET_DATA_BIT_TIMING(node, 1,  7,  2,  2); break;
    default: break;
  }

  return done;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
