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

#include "hal/II2c.h"
#include "hal/II2cSequence.h"
#include "hal/ICompiler.h"
#include "middleware/IAssert.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

typedef void (t_I2C_SEQUENCE_STEP)(const int node, t_I2C_SEQUENCE * const sequence);
#define I2C_SEQUENCE_STEP(name) void (name)(const int node, t_I2C_SEQUENCE * const sequence)

typedef enum {
  I2C_SEQUENCE_STEP_STATE_INITIAL,
  I2C_SEQUENCE_STEP_STATE_CONSECUTIVE,
  I2C_SEQUENCE_STEP_STATE_SUCCEEDED,
  I2C_SEQUENCE_STEP_STATE_FAILED
} e_I2C_SEQUENCE_STEP_STATE;

struct I2C_SEQUENCE_INTERNAL {
  struct {
    uint8_t device_address;
    uint16_t data_address;
    uint16_t number_of_bytes;
    uint8_t * data;
  } config;
  struct {
    bool in_progress;
    uint16_t data_index;
  } transfer;
  struct {
    uint16_t number;
    e_I2C_SEQUENCE_STEP_STATE state;
    bool read_first;
  } step;
};

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE I2C_SEQUENCE_STEP(I2C_SEQUENCE_STEP_SEND_START_CONDITION)
{
  ASSERT_FATAL(sequence != NULL);

  if (sequence->step.state == I2C_SEQUENCE_STEP_STATE_INITIAL) {
    I2C_SEND_START_CONDITION(node);
    sequence->step.state = I2C_SEQUENCE_STEP_STATE_CONSECUTIVE;
  }
  else {
    if (I2C_GOT_INTERRUPT(node))
      sequence->step.state = I2C_SEQUENCE_STEP_STATE_SUCCEEDED;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE I2C_SEQUENCE_STEP(I2C_SEQUENCE_STEP_REPEAT_START_CONDITION)
{
  ASSERT_FATAL(sequence != NULL);

  if (sequence->step.state == I2C_SEQUENCE_STEP_STATE_INITIAL) {
    I2C_REPEATE_START_CONDITION(node);
    sequence->step.state = I2C_SEQUENCE_STEP_STATE_CONSECUTIVE;
  }
  else {
    if (I2C_GOT_INTERRUPT(node))
      sequence->step.state = I2C_SEQUENCE_STEP_STATE_SUCCEEDED;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE I2C_SEQUENCE_STEP(I2C_SEQUENCE_STEP_SEND_STOP_CONDITION)
{
  ASSERT_FATAL(sequence != NULL);

  if (sequence->step.state == I2C_SEQUENCE_STEP_STATE_INITIAL) {
    I2C_SEND_STOP_CONDITION(node);
    sequence->step.state = I2C_SEQUENCE_STEP_STATE_CONSECUTIVE;
  }
  else {
    if (I2C_GOT_INTERRUPT(node))
      sequence->step.state = I2C_SEQUENCE_STEP_STATE_SUCCEEDED;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE I2C_SEQUENCE_STEP(I2C_SEQUENCE_STEP_SEND_DEVICE_ADDRESS_FOR_READ)
{
  ASSERT_FATAL(sequence != NULL);

  if (sequence->step.state == I2C_SEQUENCE_STEP_STATE_INITIAL) {
    const uint8_t device_address_read = (sequence->config.device_address << 1) + 1;
    I2C_START_TRANSMISSION(node, device_address_read);
    sequence->step.state = I2C_SEQUENCE_STEP_STATE_CONSECUTIVE;
  }
  else {
    if (I2C_GOT_INTERRUPT(node))
      sequence->step.state = I2C_GOT_ACKNOWLEDGE(node) ? I2C_SEQUENCE_STEP_STATE_SUCCEEDED : I2C_SEQUENCE_STEP_STATE_FAILED;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE I2C_SEQUENCE_STEP(I2C_SEQUENCE_STEP_SEND_DEVICE_ADDRESS_FOR_WRITE)
{
  ASSERT_FATAL(sequence != NULL);

  if (sequence->step.state == I2C_SEQUENCE_STEP_STATE_INITIAL) {
    const uint8_t device_address_write = sequence->config.device_address << 1;
    I2C_START_TRANSMISSION(node, device_address_write);
    sequence->step.state = I2C_SEQUENCE_STEP_STATE_CONSECUTIVE;
  }
  else {
    if (I2C_GOT_INTERRUPT(node))
      sequence->step.state = I2C_GOT_ACKNOWLEDGE(node) ? I2C_SEQUENCE_STEP_STATE_SUCCEEDED : I2C_SEQUENCE_STEP_STATE_FAILED;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE I2C_SEQUENCE_STEP(I2C_SEQUENCE_STEP_SEND_DATA_ADDRESS_HIGH)
{
  ASSERT_FATAL(sequence != NULL);

  if (sequence->step.state == I2C_SEQUENCE_STEP_STATE_INITIAL) {
    const uint8_t data_address_high = (uint8_t)(sequence->config.data_address >> 8);
    I2C_START_TRANSMISSION(node, data_address_high);
    sequence->step.state = I2C_SEQUENCE_STEP_STATE_CONSECUTIVE;
  }
  else {
    if (I2C_GOT_INTERRUPT(node))
      sequence->step.state = I2C_GOT_ACKNOWLEDGE(node) ? I2C_SEQUENCE_STEP_STATE_SUCCEEDED : I2C_SEQUENCE_STEP_STATE_FAILED;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE I2C_SEQUENCE_STEP(I2C_SEQUENCE_STEP_SEND_DATA_ADDRESS_LOW)
{
  ASSERT_FATAL(sequence != NULL);

  if (sequence->step.state == I2C_SEQUENCE_STEP_STATE_INITIAL) {
    const uint8_t data_address_low = (uint8_t)sequence->config.data_address;
    I2C_START_TRANSMISSION(node, data_address_low);
    sequence->step.state = I2C_SEQUENCE_STEP_STATE_CONSECUTIVE;
  }
  else {
    if (I2C_GOT_INTERRUPT(node))
      sequence->step.state = I2C_GOT_ACKNOWLEDGE(node) ? I2C_SEQUENCE_STEP_STATE_SUCCEEDED : I2C_SEQUENCE_STEP_STATE_FAILED;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE I2C_SEQUENCE_STEP(I2C_SEQUENCE_STEP_TRANSMIT_DATA)
{
  ASSERT_FATAL(sequence != NULL);

  if (sequence->step.state == I2C_SEQUENCE_STEP_STATE_INITIAL) {
    I2C_START_TRANSMISSION(node, sequence->config.data[sequence->transfer.data_index]);
    sequence->transfer.data_index++;
    sequence->step.state = I2C_SEQUENCE_STEP_STATE_CONSECUTIVE;
  }
  else {
    if (I2C_GOT_INTERRUPT(node)) {
      const bool done = sequence->transfer.data_index >= sequence->config.number_of_bytes;
      sequence->step.state = !I2C_GOT_ACKNOWLEDGE(node) ? I2C_SEQUENCE_STEP_STATE_FAILED : done ? I2C_SEQUENCE_STEP_STATE_SUCCEEDED : I2C_SEQUENCE_STEP_STATE_INITIAL;
    }
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE I2C_SEQUENCE_STEP(I2C_SEQUENCE_STEP_RECEIVE_DATA)
{
  ASSERT_FATAL(sequence != NULL);

  if (sequence->step.state == I2C_SEQUENCE_STEP_STATE_INITIAL) {
    I2C_START_RECEPTION(node);
    sequence->step.read_first = true;
    sequence->step.state = I2C_SEQUENCE_STEP_STATE_CONSECUTIVE;
  }
  else {
    if (I2C_GOT_INTERRUPT(node)) {
      if (sequence->step.read_first) {
        sequence->step.read_first = false;
        sequence->config.data[sequence->transfer.data_index] = I2C_GET_RECEIVED(node);
        sequence->transfer.data_index++;
        const bool last = sequence->transfer.data_index >= sequence->config.number_of_bytes;
        const bool positive = last ? false : true;
        I2C_SEND_ACKNOWLEDGE(node, positive);
      }
      else {
        const bool last = sequence->transfer.data_index >= sequence->config.number_of_bytes;
        sequence->step.state = last ? I2C_SEQUENCE_STEP_STATE_SUCCEEDED : I2C_SEQUENCE_STEP_STATE_INITIAL;
      }
    }
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool I2C_SEQUENCE_UPDATE(const int node, const e_I2C_SEQUENCE_MODE mode, t_I2C_SEQUENCE * const sequence)
{
  ASSERT_FATAL(sequence != NULL);

  if (sequence->transfer.in_progress) {

    uint16_t number_of_steps = 0;

    switch (mode) {

        case I2C_SEQUENCE_MODE_READ_WITHOUT_DATA_ADDRESS:

          switch(sequence->step.number) {
            case 0: I2C_SEQUENCE_STEP_SEND_START_CONDITION(node, sequence); break;
            case 1: I2C_SEQUENCE_STEP_SEND_DEVICE_ADDRESS_FOR_READ(node, sequence); break;
            case 2: I2C_SEQUENCE_STEP_RECEIVE_DATA(node, sequence); break;
            case 3: I2C_SEQUENCE_STEP_SEND_STOP_CONDITION(node, sequence); break;
          }

          number_of_steps = 4;

          break;

        case I2C_SEQUENCE_MODE_READ_WITH_ONE_BYTE_DATA_ADDRESS:

          switch(sequence->step.number) {
            case 0: I2C_SEQUENCE_STEP_SEND_START_CONDITION(node, sequence); break;
            case 1: I2C_SEQUENCE_STEP_SEND_DEVICE_ADDRESS_FOR_WRITE(node, sequence); break;
            case 2: I2C_SEQUENCE_STEP_SEND_DATA_ADDRESS_LOW(node, sequence); break;
            case 3: I2C_SEQUENCE_STEP_REPEAT_START_CONDITION(node, sequence); break;
            case 4: I2C_SEQUENCE_STEP_SEND_DEVICE_ADDRESS_FOR_READ(node, sequence); break;
            case 5: I2C_SEQUENCE_STEP_RECEIVE_DATA(node, sequence); break;
            case 6: I2C_SEQUENCE_STEP_SEND_STOP_CONDITION(node, sequence); break;
          }

          number_of_steps = 7;

          break;

        case I2C_SEQUENCE_MODE_READ_WITH_TWO_BYTE_DATA_ADDRESS:

          switch(sequence->step.number) {
            case 0: I2C_SEQUENCE_STEP_SEND_START_CONDITION(node, sequence); break;
            case 1: I2C_SEQUENCE_STEP_SEND_DEVICE_ADDRESS_FOR_WRITE(node, sequence); break;
            case 2: I2C_SEQUENCE_STEP_SEND_DATA_ADDRESS_HIGH(node, sequence); break;
            case 3: I2C_SEQUENCE_STEP_SEND_DATA_ADDRESS_LOW(node, sequence); break;
            case 4: I2C_SEQUENCE_STEP_REPEAT_START_CONDITION(node, sequence); break;
            case 5: I2C_SEQUENCE_STEP_SEND_DEVICE_ADDRESS_FOR_READ(node, sequence); break;
            case 6: I2C_SEQUENCE_STEP_RECEIVE_DATA(node, sequence); break;
            case 7: I2C_SEQUENCE_STEP_SEND_STOP_CONDITION(node, sequence); break;
          }

          number_of_steps = 8;

          break;

        case I2C_SEQUENCE_MODE_WRITE_WITHOUT_DATA_ADDRESS:

          switch(sequence->step.number) {
            case 0: I2C_SEQUENCE_STEP_SEND_START_CONDITION(node, sequence); break;
            case 1: I2C_SEQUENCE_STEP_SEND_DEVICE_ADDRESS_FOR_WRITE(node, sequence); break;
            case 2: I2C_SEQUENCE_STEP_TRANSMIT_DATA(node, sequence); break;
            case 3: I2C_SEQUENCE_STEP_SEND_STOP_CONDITION(node, sequence); break;
          }

          number_of_steps = 4;

          break;

        case I2C_SEQUENCE_MODE_WRITE_WITH_ONE_BYTE_DATA_ADDRESS:

          switch(sequence->step.number) {
            case 0: I2C_SEQUENCE_STEP_SEND_START_CONDITION(node, sequence); break;
            case 1: I2C_SEQUENCE_STEP_SEND_DEVICE_ADDRESS_FOR_WRITE(node, sequence); break;
            case 2: I2C_SEQUENCE_STEP_SEND_DATA_ADDRESS_LOW(node, sequence); break;
            case 3: I2C_SEQUENCE_STEP_TRANSMIT_DATA(node, sequence); break;
            case 4: I2C_SEQUENCE_STEP_SEND_STOP_CONDITION(node, sequence); break;
          }

          number_of_steps = 5;

          break;

        case I2C_SEQUENCE_MODE_WRITE_WITH_TWO_BYTE_DATA_ADDRESS:

          switch(sequence->step.number) {
            case 0: I2C_SEQUENCE_STEP_SEND_START_CONDITION(node, sequence); break;
            case 1: I2C_SEQUENCE_STEP_SEND_DEVICE_ADDRESS_FOR_WRITE(node, sequence); break;
            case 2: I2C_SEQUENCE_STEP_SEND_DATA_ADDRESS_HIGH(node, sequence); break;
            case 3: I2C_SEQUENCE_STEP_SEND_DATA_ADDRESS_LOW(node, sequence); break;
            case 4: I2C_SEQUENCE_STEP_TRANSMIT_DATA(node, sequence); break;
            case 5: I2C_SEQUENCE_STEP_SEND_STOP_CONDITION(node, sequence); break;
          }

          number_of_steps = 6;

          break;
    }

    if (sequence->step.state == I2C_SEQUENCE_STEP_STATE_SUCCEEDED) {

      const bool last = sequence->step.number == number_of_steps - 1;
      if (last)
        sequence->transfer.in_progress = false;
      else {
        sequence->step.number++;
        sequence->step.state = I2C_SEQUENCE_STEP_STATE_INITIAL;
      }
    }
    else if (sequence->step.state == I2C_SEQUENCE_STEP_STATE_FAILED)
      sequence->transfer.in_progress = false;
  }

  return sequence->transfer.in_progress;
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
