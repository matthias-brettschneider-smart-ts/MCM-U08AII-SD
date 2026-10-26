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

#include "hal/II2cSequence.h"
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

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Initialize sequence for an upcoming transmission.
 *
 -----------------------------------------------------------------------------*/
void I2cSequenceStart(t_I2C_SEQUENCE * const sequence, const uint8_t device_address, const uint16_t data_address, const uint16_t number_of_bytes, uint8_t data[])
{
  ASSERT_FATAL(sequence != NULL);

  const bool valid = (device_address <= 0x7F) && (number_of_bytes > 0) && (data != NULL);
  if (valid) {

    sequence->config.device_address = device_address;
    sequence->config.data_address = data_address;
    sequence->config.number_of_bytes = number_of_bytes;
    sequence->config.data = data;

    sequence->transfer.data_index = 0;

    sequence->step.number = 0;
    sequence->step.state = I2C_SEQUENCE_STEP_STATE_INITIAL;
    sequence->step.read_first = false;
  }

  sequence->transfer.in_progress = valid ? true : false;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Determine sequence state, either during (interrupted) transmissions or after it finished.
 *
 -----------------------------------------------------------------------------*/
e_I2C_SEQUENCE_STATE I2cSequenceGetState(t_I2C_SEQUENCE * const sequence)
{
  ASSERT_FATAL(sequence != NULL);

  return
      sequence->transfer.in_progress ? I2C_SEQUENCE_STATE_IN_PROGRESS :
          (sequence->step.state == I2C_SEQUENCE_STEP_STATE_SUCCEEDED) ? I2C_SEQUENCE_STATE_SUCCEEDED :
              I2C_SEQUENCE_STATE_FAILED;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
