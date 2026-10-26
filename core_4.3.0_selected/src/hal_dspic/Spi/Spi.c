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

#define CONTROLLER_DSPIC33FJ256GP710_SUPPORTED
#define CONTROLLER_DSPIC33EP128GS804_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP508_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP506_SUPPORTED

#include "hal/ICompiler.h"
#include "hal/IController.h"
#include "hal/IOscillator.h"
#include "hal/ISpi.h"
#include "middleware/IAssert.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

COMPILER_INLINE void SPI_SET_BASIC_CONFIGURATION(const int node, const bool master, const bool transfer_two_byte);
COMPILER_INLINE void SPI_CONFIGURE_PINS_TO_USE(const int node, const bool use_clock_pin, const bool use_sdo_pin, const bool use_sdi_pin, const bool use_slave_select_pin);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Initialize SPI node.
 *
 -----------------------------------------------------------------------------*/
void SpiInit(const int node, const t_SPI_SETTINGS * const settings)
{
  ASSERT_FATAL(node < SPI_NUMBER_OF_NODES);
  ASSERT_FATAL(settings != NULL);
  ASSERT_FATAL(!(settings->master && (settings->bitrate == 0)));

  SPI_DEACTIVATE(node);
  SPI_CONFIRM_INTERRUPT(node);
  SPI_SET_BASIC_CONFIGURATION(node, settings->master, settings->transfer_two_byte);
  SPI_CONFIGURE_PINS_TO_USE(node, settings->use_clock_pin, settings->use_sdo_pin, settings->use_sdi_pin, settings->use_slave_select_pin);
  SPI_SET_SIGNAL_LOGIC(node, settings->clock_state_idle, settings->clock_edge_for_input_read, settings->clock_edge_for_output_change);
  SPI_SET_BITRATE(node, settings->bitrate);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Set basic node configuration.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void SPI_SET_BASIC_CONFIGURATION(const int node, const bool master, const bool transfer_two_byte)
{
  ASSERT_FATAL(node < SPI_NUMBER_OF_NODES);

  if (node == 0) {
    SPI1CON1bits.MSTEN = master ? 1 : 0;
    SPI1CON1bits.MODE16 = transfer_two_byte ? 1 : 0;
#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    SPI1CON1Lbits.MCLKEN = 0;
#endif
  }
  else {
    SPI2CON1bits.MSTEN = master ? 1 : 0;
    SPI2CON1bits.MODE16 = transfer_two_byte ? 1 : 0;
#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
    SPI2CON1Lbits.MCLKEN = 0;
#endif
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Configure which pins shall be handled by the periphery.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void SPI_CONFIGURE_PINS_TO_USE(const int node, const bool use_clock_pin, const bool use_sdo_pin, const bool use_sdi_pin, const bool use_slave_select_pin)
{
  ASSERT_FATAL(node < SPI_NUMBER_OF_NODES);
  ASSERT_FATAL(!((CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710) && !use_sdi_pin)); // not supported by dsPIC33F
  ASSERT_FATAL(!((CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710) && use_slave_select_pin)); // not available on dsPIC33F because of silicon bug

  if (node == 0) {
    SPI1CON1bits.DISSCK = use_clock_pin ? 0 : 1;
    SPI1CON1bits.DISSDO = use_sdo_pin ? 0 : 1;
    SPI1CON1bits.SSEN = use_slave_select_pin ? 1 : 0;
  }
  else {
    SPI2CON1bits.DISSCK = use_clock_pin ? 0 : 1;
    SPI2CON1bits.DISSDO = use_sdo_pin ? 0 : 1;
    SPI2CON1bits.SSEN = use_slave_select_pin ? 1 : 0;
  }

#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
  if (node == 0)
    SPI1CON1bits.DISSDI = use_sdi_pin ? 0 : 1;
  else
    SPI2CON1bits.DISSDI = use_sdi_pin ? 0 : 1;
#endif
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
