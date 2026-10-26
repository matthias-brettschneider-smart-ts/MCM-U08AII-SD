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

#define CONTROLLER_DSPIC33EP128GS804_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP508_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP506_SUPPORTED

#include "hal/ICompiler.h"
#include "hal/IController.h"
#include "hal/IOscillator.h"
#include "hal/IUart.h"
#include "middleware/IAssert.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

COMPILER_INLINE void UART_SET_PARITY(const int node, const e_UART_PARITY parity);
COMPILER_INLINE void UART_SET_NUMBER_OF_STOP_BITS(const int node, const bool two);
COMPILER_INLINE void UART_SET_BITRATE_INTERNAL(const int node, const uint16_t divisor);
COMPILER_INLINE void UART_SET_BITRATE(const int node, const uint32_t bitrate);
COMPILER_INLINE void UART_DISABLE_FLOW_CONTROL(const int node);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void UART_SET_PARITY(const int node, const e_UART_PARITY parity)
{
  ASSERT_FATAL((parity == UART_PARITY_EVEN) || (parity == UART_PARITY_ODD) || (parity == UART_PARITY_NONE));

  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: U1MODEbits.PDSEL = (parity == UART_PARITY_ODD) ? 2 : (parity == UART_PARITY_EVEN) ? 1 : 0; break;
    case 1: U2MODEbits.PDSEL = (parity == UART_PARITY_ODD) ? 2 : (parity == UART_PARITY_EVEN) ? 1 : 0; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: U1MODEbits.MOD = (parity == UART_PARITY_ODD) ? 2 : (parity == UART_PARITY_EVEN) ? 3 : 0; break;
    case 1: U2MODEbits.MOD = (parity == UART_PARITY_ODD) ? 2 : (parity == UART_PARITY_EVEN) ? 3 : 0; break;
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
COMPILER_INLINE void UART_SET_NUMBER_OF_STOP_BITS(const int node, const bool two)
{
  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: U1MODEbits.STSEL = two ? 1 : 0; break;
    case 1: U2MODEbits.STSEL = two ? 1 : 0; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: U1MODEHbits.STSEL = two ? 2 : 0; break;
    case 1: U2MODEHbits.STSEL = two ? 2 : 0; break;
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
COMPILER_INLINE void UART_SET_BITRATE_INTERNAL(const int node, const uint16_t divisor)
{
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
  const uint16_t brg = ((divisor < 4) || ((divisor % 4) != 0)) ? 0 : (divisor / 4);
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
  const uint16_t brg = divisor;
#endif

  ASSERT_FATAL(brg != 0);

  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    //      no auto baud rate,    4 clocks per bit,    tbd
    case 0: U1MODEbits.ABAUD = 0; U1MODEbits.BRGH = 1; U1BRG = brg - 1; break;
    case 1: U2MODEbits.ABAUD = 0; U2MODEbits.BRGH = 1; U2BRG = brg - 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    //      no auto baud rate,    fractional mode,         periphery clock,         tbd
    case 0: U1MODEbits.ABAUD = 0; U1MODEHbits.BCLKMOD = 1; U1MODEHbits.BCLKSEL = 0; U1BRGH = 0; U1BRG = brg; break;
    case 1: U2MODEbits.ABAUD = 0; U2MODEHbits.BCLKMOD = 1; U2MODEHbits.BCLKSEL = 0; U2BRGH = 0; U2BRG = brg; break;
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
COMPILER_INLINE void UART_SET_BITRATE(const int node, const uint32_t bitrate)
{
  const uint16_t divisor =
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
      ((bitrate == 10000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ?   4 :
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
      ((bitrate ==  1000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ?  40 :
      ((bitrate ==  1000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ?  80 :
      ((bitrate ==  1000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ? 100 :
      ((bitrate == 10000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  40000000)) ?   4 :
      ((bitrate == 10000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() ==  80000000)) ?   8 :
      ((bitrate == 10000000) && (OSCILLATOR_GET_INSTRUCTION_FREQUENCY_IN_HZ() == 100000000)) ?  10 :
#endif
      0;

  ASSERT_FATAL(divisor != 0);

  UART_SET_BITRATE_INTERNAL(node, divisor);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void UART_DISABLE_FLOW_CONTROL(const int node)
{
  switch (node) {
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
    case 0: U1MODEbits.UEN = 0; U1MODEbits.RTSMD = 1; break;
    case 1: U2MODEbits.UEN = 0; U2MODEbits.RTSMD = 1; break;
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    case 0: U1MODEHbits.FLO = 0; break;
    case 1: U2MODEHbits.FLO = 0; break;
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
void UartInit(const int node, const t_UART_SETTINGS * const settings)
{
  ASSERT_FATAL(settings != NULL);

  UART_DEACTIVATE(node);
  UART_SET_PARITY(node, settings->parity);
  UART_SET_NUMBER_OF_STOP_BITS(node, settings->two_stop_bits);
  UART_SET_BITRATE(node, settings->bitrate);
  UART_DISABLE_FLOW_CONTROL(node);
  UART_ACTIVATE(node, settings->mode);
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
