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

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void CAN_FD_SET_ID_AND_MASK(const uint8_t node, const uint8_t filter, const uint16_t mask, const uint16_t identifier)
{
  ASSERT_FATAL(node < CAN_GET_NUMBER_OF_NODES());
  ASSERT_FATAL(mask <= 0x7FF);
  ASSERT_FATAL(identifier <= 0x7FF);

  // match only messages with standard identifiers
  switch (filter) {
    case  0: C1MASK0Hbits.MIDE  = 1; C1FLTOBJ0Hbits.EXIDE  = 0; break;
    case  1: C1MASK1Hbits.MIDE  = 1; C1FLTOBJ1Hbits.EXIDE  = 0; break;
    case  2: C1MASK2Hbits.MIDE  = 1; C1FLTOBJ2Hbits.EXIDE  = 0; break;
    case  3: C1MASK3Hbits.MIDE  = 1; C1FLTOBJ3Hbits.EXIDE  = 0; break;
    case  4: C1MASK4Hbits.MIDE  = 1; C1FLTOBJ4Hbits.EXIDE  = 0; break;
    case  5: C1MASK5Hbits.MIDE  = 1; C1FLTOBJ5Hbits.EXIDE  = 0; break;
    case  6: C1MASK6Hbits.MIDE  = 1; C1FLTOBJ6Hbits.EXIDE  = 0; break;
    case  7: C1MASK7Hbits.MIDE  = 1; C1FLTOBJ7Hbits.EXIDE  = 0; break;
    case  8: C1MASK8Hbits.MIDE  = 1; C1FLTOBJ8Hbits.EXIDE  = 0; break;
    case  9: C1MASK9Hbits.MIDE  = 1; C1FLTOBJ9Hbits.EXIDE  = 0; break;
    case 10: C1MASK10Hbits.MIDE = 1; C1FLTOBJ10Hbits.EXIDE = 0; break;
    case 11: C1MASK11Hbits.MIDE = 1; C1FLTOBJ11Hbits.EXIDE = 0; break;
    case 12: C1MASK12Hbits.MIDE = 1; C1FLTOBJ12Hbits.EXIDE = 0; break;
    case 13: C1MASK13Hbits.MIDE = 1; C1FLTOBJ13Hbits.EXIDE = 0; break;
    case 14: C1MASK14Hbits.MIDE = 1; C1FLTOBJ14Hbits.EXIDE = 0; break;
    case 15: C1MASK15Hbits.MIDE = 1; C1FLTOBJ15Hbits.EXIDE = 0; break;
    default: ASSERT_FATAL(false); break;
  };

  // SID
  switch (filter) {
    case  0: C1MASK0Lbits.MSID  = mask; C1FLTOBJ0Lbits.SID  = identifier; break;
    case  1: C1MASK1Lbits.MSID  = mask; C1FLTOBJ1Lbits.SID  = identifier; break;
    case  2: C1MASK2Lbits.MSID  = mask; C1FLTOBJ2Lbits.SID  = identifier; break;
    case  3: C1MASK3Lbits.MSID  = mask; C1FLTOBJ3Lbits.SID  = identifier; break;
    case  4: C1MASK4Lbits.MSID  = mask; C1FLTOBJ4Lbits.SID  = identifier; break;
    case  5: C1MASK5Lbits.MSID  = mask; C1FLTOBJ5Lbits.SID  = identifier; break;
    case  6: C1MASK6Lbits.MSID  = mask; C1FLTOBJ6Lbits.SID  = identifier; break;
    case  7: C1MASK7Lbits.MSID  = mask; C1FLTOBJ7Lbits.SID  = identifier; break;
    case  8: C1MASK8Lbits.MSID  = mask; C1FLTOBJ8Lbits.SID  = identifier; break;
    case  9: C1MASK9Lbits.MSID  = mask; C1FLTOBJ9Lbits.SID  = identifier; break;
    case 10: C1MASK10Lbits.MSID = mask; C1FLTOBJ10Lbits.SID = identifier; break;
    case 11: C1MASK11Lbits.MSID = mask; C1FLTOBJ11Lbits.SID = identifier; break;
    case 12: C1MASK12Lbits.MSID = mask; C1FLTOBJ12Lbits.SID = identifier; break;
    case 13: C1MASK13Lbits.MSID = mask; C1FLTOBJ13Lbits.SID = identifier; break;
    case 14: C1MASK14Lbits.MSID = mask; C1FLTOBJ14Lbits.SID = identifier; break;
    case 15: C1MASK15Lbits.MSID = mask; C1FLTOBJ15Lbits.SID = identifier; break;
    default: ASSERT_FATAL(false); break;
  };

  // SID11
  switch (filter) {
    case  0: C1MASK0Hbits.MSID11  = 0; C1FLTOBJ0Hbits.SID11  = 0; break;
    case  1: C1MASK1Hbits.MSID11  = 0; C1FLTOBJ1Hbits.SID11  = 0; break;
    case  2: C1MASK2Hbits.MSID11  = 0; C1FLTOBJ2Hbits.SID11  = 0; break;
    case  3: C1MASK3Hbits.MSID11  = 0; C1FLTOBJ3Hbits.SID11  = 0; break;
    case  4: C1MASK4Hbits.MSID11  = 0; C1FLTOBJ4Hbits.SID11  = 0; break;
    case  5: C1MASK5Hbits.MSID11  = 0; C1FLTOBJ5Hbits.SID11  = 0; break;
    case  6: C1MASK6Hbits.MSID11  = 0; C1FLTOBJ6Hbits.SID11  = 0; break;
    case  7: C1MASK7Hbits.MSID11  = 0; C1FLTOBJ7Hbits.SID11  = 0; break;
    case  8: C1MASK8Hbits.MSID11  = 0; C1FLTOBJ8Hbits.SID11  = 0; break;
    case  9: C1MASK9Hbits.MSID11  = 0; C1FLTOBJ9Hbits.SID11  = 0; break;
    case 10: C1MASK10Hbits.MSID11 = 0; C1FLTOBJ10Hbits.SID11 = 0; break;
    case 11: C1MASK11Hbits.MSID11 = 0; C1FLTOBJ11Hbits.SID11 = 0; break;
    case 12: C1MASK12Hbits.MSID11 = 0; C1FLTOBJ12Hbits.SID11 = 0; break;
    case 13: C1MASK13Hbits.MSID11 = 0; C1FLTOBJ13Hbits.SID11 = 0; break;
    case 14: C1MASK14Hbits.MSID11 = 0; C1FLTOBJ14Hbits.SID11 = 0; break;
    case 15: C1MASK15Hbits.MSID11 = 0; C1FLTOBJ15Hbits.SID11 = 0; break;
    default: ASSERT_FATAL(false); break;
  };

  // EID
  switch (filter) {
    case  0: C1MASK0Lbits.MEID  = 0; C1MASK0Hbits.MEID  = 0; C1FLTOBJ0Lbits.EID  = 0; C1FLTOBJ0Hbits.EID  = 0; break;
    case  1: C1MASK1Lbits.MEID  = 0; C1MASK1Hbits.MEID  = 0; C1FLTOBJ1Lbits.EID  = 0; C1FLTOBJ1Hbits.EID  = 0; break;
    case  2: C1MASK2Lbits.MEID  = 0; C1MASK2Hbits.MEID  = 0; C1FLTOBJ2Lbits.EID  = 0; C1FLTOBJ2Hbits.EID  = 0; break;
    case  3: C1MASK3Lbits.MEID  = 0; C1MASK3Hbits.MEID  = 0; C1FLTOBJ3Lbits.EID  = 0; C1FLTOBJ3Hbits.EID  = 0; break;
    case  4: C1MASK4Lbits.MEID  = 0; C1MASK4Hbits.MEID  = 0; C1FLTOBJ4Lbits.EID  = 0; C1FLTOBJ4Hbits.EID  = 0; break;
    case  5: C1MASK5Lbits.MEID  = 0; C1MASK5Hbits.MEID  = 0; C1FLTOBJ5Lbits.EID  = 0; C1FLTOBJ5Hbits.EID  = 0; break;
    case  6: C1MASK6Lbits.MEID  = 0; C1MASK6Hbits.MEID  = 0; C1FLTOBJ6Lbits.EID  = 0; C1FLTOBJ6Hbits.EID  = 0; break;
    case  7: C1MASK7Lbits.MEID  = 0; C1MASK7Hbits.MEID  = 0; C1FLTOBJ7Lbits.EID  = 0; C1FLTOBJ7Hbits.EID  = 0; break;
    case  8: C1MASK8Lbits.MEID  = 0; C1MASK8Hbits.MEID  = 0; C1FLTOBJ8Lbits.EID  = 0; C1FLTOBJ8Hbits.EID  = 0; break;
    case  9: C1MASK9Lbits.MEID  = 0; C1MASK9Hbits.MEID  = 0; C1FLTOBJ9Lbits.EID  = 0; C1FLTOBJ9Hbits.EID  = 0; break;
    case 10: C1MASK10Lbits.MEID = 0; C1MASK10Hbits.MEID = 0; C1FLTOBJ10Lbits.EID = 0; C1FLTOBJ10Hbits.EID = 0; break;
    case 11: C1MASK11Lbits.MEID = 0; C1MASK11Hbits.MEID = 0; C1FLTOBJ11Lbits.EID = 0; C1FLTOBJ11Hbits.EID = 0; break;
    case 12: C1MASK12Lbits.MEID = 0; C1MASK12Hbits.MEID = 0; C1FLTOBJ12Lbits.EID = 0; C1FLTOBJ12Hbits.EID = 0; break;
    case 13: C1MASK13Lbits.MEID = 0; C1MASK13Hbits.MEID = 0; C1FLTOBJ13Lbits.EID = 0; C1FLTOBJ13Hbits.EID = 0; break;
    case 14: C1MASK14Lbits.MEID = 0; C1MASK14Hbits.MEID = 0; C1FLTOBJ14Lbits.EID = 0; C1FLTOBJ14Hbits.EID = 0; break;
    case 15: C1MASK15Lbits.MEID = 0; C1MASK15Hbits.MEID = 0; C1FLTOBJ15Lbits.EID = 0; C1FLTOBJ15Hbits.EID = 0; break;
    default: ASSERT_FATAL(false); break;
  };
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void CAN_FD_CONFIGURE_FILTER(const uint8_t node, const uint8_t filter, const uint16_t mask, const uint16_t identifier, const uint8_t fifo)
{
  ASSERT_FATAL(node < CAN_GET_NUMBER_OF_NODES());
  ASSERT_FATAL(fifo < CAN_GET_NUMBER_OF_FIFOS());

  switch (filter) {
    case  0: C1FLTCON0Lbits.FLTEN0  = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON0Lbits.F0BP  = fifo + 1; C1FLTCON0Lbits.FLTEN0  = 1; break;
    case  1: C1FLTCON0Lbits.FLTEN1  = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON0Lbits.F1BP  = fifo + 1; C1FLTCON0Lbits.FLTEN1  = 1; break;
    case  2: C1FLTCON0Hbits.FLTEN2  = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON0Hbits.F2BP  = fifo + 1; C1FLTCON0Hbits.FLTEN2  = 1; break;
    case  3: C1FLTCON0Hbits.FLTEN3  = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON0Hbits.F3BP  = fifo + 1; C1FLTCON0Hbits.FLTEN3  = 1; break;
    case  4: C1FLTCON1Lbits.FLTEN4  = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON1Lbits.F4BP  = fifo + 1; C1FLTCON1Lbits.FLTEN4  = 1; break;
    case  5: C1FLTCON1Lbits.FLTEN5  = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON1Lbits.F5BP  = fifo + 1; C1FLTCON1Lbits.FLTEN5  = 1; break;
    case  6: C1FLTCON1Hbits.FLTEN6  = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON1Hbits.F6BP  = fifo + 1; C1FLTCON1Hbits.FLTEN6  = 1; break;
    case  7: C1FLTCON1Hbits.FLTEN7  = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON1Hbits.F7BP  = fifo + 1; C1FLTCON1Hbits.FLTEN7  = 1; break;
    case  8: C1FLTCON2Lbits.FLTEN8  = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON2Lbits.F8BP  = fifo + 1; C1FLTCON2Lbits.FLTEN8  = 1; break;
    case  9: C1FLTCON2Lbits.FLTEN9  = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON2Lbits.F9BP  = fifo + 1; C1FLTCON2Lbits.FLTEN9  = 1; break;
    case 10: C1FLTCON2Hbits.FLTEN10 = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON2Hbits.F10BP = fifo + 1; C1FLTCON2Hbits.FLTEN10 = 1; break;
    case 11: C1FLTCON2Hbits.FLTEN11 = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON2Hbits.F11BP = fifo + 1; C1FLTCON2Hbits.FLTEN11 = 1; break;
    case 12: C1FLTCON3Lbits.FLTEN12 = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON3Lbits.F12BP = fifo + 1; C1FLTCON3Lbits.FLTEN12 = 1; break;
    case 13: C1FLTCON3Lbits.FLTEN13 = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON3Lbits.F13BP = fifo + 1; C1FLTCON3Lbits.FLTEN13 = 1; break;
    case 14: C1FLTCON3Hbits.FLTEN14 = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON3Hbits.F14BP = fifo + 1; C1FLTCON3Hbits.FLTEN14 = 1; break;
    case 15: C1FLTCON3Hbits.FLTEN15 = 0; CAN_FD_SET_ID_AND_MASK(node, filter, mask, identifier); C1FLTCON3Hbits.F15BP = fifo + 1; C1FLTCON3Hbits.FLTEN15 = 1; break;
    default: ASSERT_FATAL(false); break;
  };
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
