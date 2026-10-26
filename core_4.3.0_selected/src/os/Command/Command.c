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

#include "hal/IReboot.h"
#include "middleware/IAssert.h"
#include "middleware/ICanChannel.h"
#include "middleware/ICanMessage.h"
#include "middleware/IDefaultInternal.h"
#include "middleware/IGeneric.h"
#include "middleware/ILabelId.h"
#include "middleware/IMemory.h"
#include "middleware/IStorageInternal.h"
#include "os/IFlashSession.h"
#include "os/ILabel.h"
#include "os/IMode.h"
#include "os/IProtocol.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef enum {
  COMMAND_ID_GET_LABEL                                                    = 0x00,
  COMMAND_ID_SET_LABEL                                                    = 0x01,
  COMMAND_ID_REQUEST_WELCOME                                              = 0x02,
  COMMAND_ID_STORE_SETTINGS                                               = 0x03,
  COMMAND_ID_FACTORY_RESET                                                = 0x04,
  COMMAND_ID_REBOOT                                                       = 0x05,
  COMMAND_ID_FLASH_ERASE                                                  = 0x06,
  COMMAND_ID_FLASH_START                                                  = 0x07,
  COMMAND_ID_FLASH_TRANSFER_DATA                                          = 0x08,
  COMMAND_ID_FLASH_PROGRAM_DATA                                           = 0x09,
  COMMAND_ID_FLASH_CHECK_MEMORY                                           = 0x0A,
  COMMAND_ID_UNDEFINED                                                    = 0x0B,  // has to be the last of regular IDs
  COMMAND_ID_NEGATIVE_RESPONSE                                            = 0x7F
} e_COMMAND;

typedef struct {
  e_COMMAND command;
  uint8_t minimum_length;
  uint8_t maximum_length;
  e_MODE minimum_mode;
} t_COMMANDS;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static const t_COMMANDS commands[] = {
    { .command = COMMAND_ID_GET_LABEL,           .minimum_length = 8, .maximum_length = 8, .minimum_mode = MODE_ID_OPERATION      },
    { .command = COMMAND_ID_SET_LABEL,           .minimum_length = 8, .maximum_length = 8, .minimum_mode = MODE_ID_OPERATION      },
    { .command = COMMAND_ID_REQUEST_WELCOME,     .minimum_length = 1, .maximum_length = 8, .minimum_mode = MODE_ID_OPERATION      },
    { .command = COMMAND_ID_STORE_SETTINGS,      .minimum_length = 1, .maximum_length = 8, .minimum_mode = MODE_ID_CONFIG         },
    { .command = COMMAND_ID_FACTORY_RESET,       .minimum_length = 1, .maximum_length = 1, .minimum_mode = MODE_ID_CONFIG         },
    { .command = COMMAND_ID_REBOOT,              .minimum_length = 1, .maximum_length = 1, .minimum_mode = MODE_ID_OPERATION      },
    { .command = COMMAND_ID_FLASH_ERASE,         .minimum_length = 8, .maximum_length = 8, .minimum_mode = MODE_ID_ADVANCED_SMART },
    { .command = COMMAND_ID_FLASH_START,         .minimum_length = 8, .maximum_length = 8, .minimum_mode = MODE_ID_ADVANCED_SMART },
    { .command = COMMAND_ID_FLASH_TRANSFER_DATA, .minimum_length = 3, .maximum_length = 8, .minimum_mode = MODE_ID_ADVANCED_SMART },
    { .command = COMMAND_ID_FLASH_PROGRAM_DATA,  .minimum_length = 1, .maximum_length = 1, .minimum_mode = MODE_ID_ADVANCED_SMART },
    { .command = COMMAND_ID_FLASH_CHECK_MEMORY,  .minimum_length = 8, .maximum_length = 8, .minimum_mode = MODE_ID_ADVANCED_SMART },
    { .command = COMMAND_ID_UNDEFINED,           .minimum_length = 0, .maximum_length = 0, .minimum_mode = MODE_ID_ILLEGAL        }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static void ProtocolSendWelcomeInternal(const bool fd, const bool bitrate_switching);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Startup initialization of this firmware module.
 *
 -----------------------------------------------------------------------------*/
void ProtocolInit(void)
{
  ModeInit();
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Wrapper for proactive welcome messages (CAN classic).
 *
 -----------------------------------------------------------------------------*/
void ProtocolSendWelcome(void)
{
  ProtocolSendWelcomeInternal(false, false);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Send standard welcome message to allow identifying of available modules.
 *
 -----------------------------------------------------------------------------*/
static void ProtocolSendWelcomeInternal(const bool fd, const bool bitrate_switching)
{
  t_CAN_MESSAGE message;

  message.fd = fd;
  message.bitrate_switching = bitrate_switching;
  message.identifier = CAN_MESSAGE_ID_INVALID;
  message.payload_length = 8;
  message.data_byte[0] = 0xCA;
  message.data_byte[1] = 0xFE;
  message.data_byte[2] = 0xBA;
  message.data_byte[3] = 0xBE;
  message.data_byte[4] = 0xCA;
  message.data_byte[5] = 0xFE;
  message.data_byte[6] = 0xBA;
  message.data_byte[7] = 0xBE;

  CanChannelTransmitResponse(&message, false);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Send error message simulating a label access.
 *
 -----------------------------------------------------------------------------*/
void ProtocolSendError(const uint8_t category, const uint8_t id, const uint8_t occurance_count, const uint8_t attribute)
{
  t_CAN_MESSAGE message;

  message.fd = false;
  message.bitrate_switching = false;
  message.identifier = CAN_MESSAGE_ID_INVALID;
  message.payload_length = 8;
  message.data_byte[0] = COMMAND_ID_GET_LABEL;
  message.data_byte[1] = (uint8_t)((uint16_t)LABEL_ID_ERROR >> 8);
  message.data_byte[2] = (uint8_t)((uint16_t)LABEL_ID_ERROR & 0x00FFu);
  message.data_byte[3] = 1;
  message.data_byte[4] = category;
  message.data_byte[5] = id;
  message.data_byte[6] = occurance_count;
  message.data_byte[7] = attribute;

  // don't allow further error entries to prevent recursive loop
  CanChannelTransmitResponse(&message, true);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool ProtocolIsProgrammingInProgress(void)
{
  return FlashSessionIsActive();
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Handle incoming CAN requests.
 *
 -----------------------------------------------------------------------------*/
void ProtocolHandler(void)
{
  e_ERROR_ID error = ERROR_ID_SUCCESS;

  t_CAN_MESSAGE message;
  bool broadcast = false;
  const bool new_message = CanChannelReceiveRequest(&message, &broadcast);
  const bool handle_message = new_message && (message.payload_length > 0);
  if (handle_message) {

    typedef union {
      struct {
        uint8_t command;
        uint8_t unused[7];
      } generic;
      struct {
        uint32_t command : 8;
        uint32_t label : 16;
        uint32_t sublabel : 8;
        uint32_t value;
      } label;
      struct {
        uint32_t command : 8;
        uint32_t length : 24;
        uint32_t address;
      } flash;
      struct {
        uint8_t command;
        uint8_t index;
        uint8_t data[6];
      } flash_transfer_data;
    } t_COMMAND_PROTOCOL;

    // get payload for command interpretation
    t_COMMAND_PROTOCOL protocol;
    memcpy((void *)&protocol, message.data_byte, 8);

    bool suppress_response = false;

    if (protocol.generic.command >= COMMAND_ID_UNDEFINED)
      error = ERROR_ID_UNKNOWN_COMMAND;
    else if ((message.payload_length < commands[protocol.generic.command].minimum_length) || (message.payload_length > commands[protocol.generic.command].maximum_length))
      error = ERROR_ID_UNKNOWN_COMMAND;
    else if (ModeGet() < commands[protocol.generic.command].minimum_mode)
      error = ERROR_ID_NOT_ALLOWED_IN_THIS_MODE;
    else {

      switch ((e_COMMAND)protocol.generic.command) {

        case COMMAND_ID_GET_LABEL:
          protocol.label.value = 0;
          error = LabelGetValue(MEMORY_SWAP_UINT16(protocol.label.label), protocol.label.sublabel, &protocol.label.value, broadcast);
          message.data_long[1] = MEMORY_SWAP_UINT32(protocol.label.value);
          break;

        case COMMAND_ID_SET_LABEL:
          protocol.label.value = MEMORY_SWAP_UINT32(protocol.label.value);
          error = LabelSetValue(MEMORY_SWAP_UINT16(protocol.label.label), protocol.label.sublabel, &protocol.label.value, broadcast);
          break;

        case COMMAND_ID_STORE_SETTINGS:
          StorageStoreSettings();
          // send positive response containing just command byte
          message.payload_length = 1;
          break;

        case COMMAND_ID_REQUEST_WELCOME:
          ProtocolSendWelcomeInternal(message.fd, message.bitrate_switching);
          // do not send response in addition to welcome message
          suppress_response = true;
          break;

        case COMMAND_ID_FACTORY_RESET:
          DefaultReset();
          break;

        case COMMAND_ID_REBOOT:
          if (!broadcast)
            error = ERROR_ID_ONLY_ALLOWED_ON_BASE_ID;
          else
            RebootExecute();
          break;

        case COMMAND_ID_FLASH_ERASE:
          error = FlashSessionErase(MEMORY_SWAP_UINT32(protocol.flash.address), (MEMORY_SWAP_UINT32((uint32_t)protocol.flash.length) >> 8));
          break;

        case COMMAND_ID_FLASH_START:
          error = FlashSessionStart(MEMORY_SWAP_UINT32(protocol.flash.address), (MEMORY_SWAP_UINT32((uint32_t)protocol.flash.length) >> 8));
          break;

        case COMMAND_ID_FLASH_TRANSFER_DATA:
          error = FlashSessionTransfer(protocol.flash_transfer_data.index, (uint8_t)(message.payload_length - 2), protocol.flash_transfer_data.data);
          suppress_response = true;
          break;

        case COMMAND_ID_FLASH_PROGRAM_DATA:
          error = FlashSessionProgram();
          break;

        case COMMAND_ID_FLASH_CHECK_MEMORY:
        {
          uint16_t checksum = 0;
          error = FlashSessionCheck(MEMORY_SWAP_UINT32(protocol.flash.address), (MEMORY_SWAP_UINT32((uint32_t)protocol.flash.length) >> 8), &checksum);
          if (error == ERROR_ID_SUCCESS) {
            message.data_byte[1] = 0;
            message.data_word[1] = 0;
            message.data_word[2] = 0;
            message.data_word[3] = MEMORY_SWAP_UINT16(checksum);
          }
        }
        break;

        default:
          error = ERROR_ID_UNKNOWN_COMMAND;
          break;
      }
    }

    if (!suppress_response) {

      const bool negative_response = error != ERROR_ID_SUCCESS;
      if (negative_response) {

        message.payload_length = 8;

        const bool return_labels = (protocol.generic.command == COMMAND_ID_GET_LABEL) || (protocol.generic.command == COMMAND_ID_SET_LABEL);
        message.data_byte[4] = return_labels ? message.data_byte[3] : 0;
        message.data_byte[3] = return_labels ? message.data_byte[2] : 0;
        message.data_byte[2] = return_labels ? message.data_byte[1] : 0;

        // move command and add negative response and error code
        message.data_byte[1] = message.data_byte[0];
        message.data_byte[0] = COMMAND_ID_NEGATIVE_RESPONSE;
        message.data_byte[5] = (uint8_t)error;

        message.data_byte[6] = 0;
        message.data_byte[7] = 0;
      }

      message.identifier = CAN_MESSAGE_ID_INVALID;
      CanChannelTransmitResponse(&message, false);
    }
  }
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
