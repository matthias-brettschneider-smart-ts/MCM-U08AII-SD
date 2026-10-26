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
#include "middleware/IDefaultInternal.h"
#include "middleware/IStorageInternal.h"
#include "os/IFlashSession.h"
#include "os/ILabel.h"
#include "os/IMode.h"

#include "IMock.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef struct {
  e_MODE mode;
  bool flash_session_active;
  struct {
    struct {
      bool request_singlecast_available;
      bool request_broadcast_available;
      t_CAN_MESSAGE request_message;
    } receive;
    bool transmit_available;
    t_CAN_MESSAGE transmit_message;
  } can_channel;
  struct {
    char actions[1024];
    size_t index;
  } action;
} t_MOCK;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_MOCK mock;

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

#define MOCK_ADD_TO_ACTIONS(...) \
  mock.action.index += snprintf(&mock.action.actions[mock.action.index], sizeof(mock.action.actions) - mock.action.index, __VA_ARGS__)

void MockFlushActions(void)
{
  mock.action.index = 0;
}

bool MockActionIsEqual(const char expected[], const size_t number_of_characters)
{
  bool equal = number_of_characters == mock.action.index;
  if (equal)
    equal = memcmp(expected, mock.action.actions, number_of_characters) == 0;

  mock.action.actions[mock.action.index] = 0;

#if false // for debugging
  if (!equal) {
    printf("EXPECTED: %3d - %s\n", (int)number_of_characters, expected);
    printf("GOT:      %3d - %s\n", (int)mock.action.index, mock.action.actions);
  }
#endif

  return equal;
}

// --- can channel -------------------------------------------------------------

bool MockCanChannelReceiveIsPending(void)
{
  return mock.can_channel.receive.request_broadcast_available || mock.can_channel.receive.request_singlecast_available;
}

void MockCanChannelPrepareMessageForReceive(const t_CAN_MESSAGE * const message, const bool broadcast)
{
  ASSERT_FATAL(!mock.can_channel.receive.request_broadcast_available);
  ASSERT_FATAL(!mock.can_channel.receive.request_singlecast_available);

  memcpy(&mock.can_channel.receive.request_message, message, sizeof(t_CAN_MESSAGE));

  if (broadcast)
    mock.can_channel.receive.request_broadcast_available = true;
  else
    mock.can_channel.receive.request_singlecast_available = true;
}

bool MockCanChannelReceiveRequest(t_CAN_MESSAGE * const message, bool * const broadcast)
{
  const bool available = mock.can_channel.receive.request_broadcast_available || mock.can_channel.receive.request_singlecast_available;
  if (available) {

    memcpy(message, &mock.can_channel.receive.request_message, sizeof(t_CAN_MESSAGE));

    if (mock.can_channel.receive.request_broadcast_available) {
      *broadcast = true;
      mock.can_channel.receive.request_broadcast_available = false;
    }
    else {
      *broadcast = false;
      mock.can_channel.receive.request_singlecast_available = false;
    }
  }

  return available;
}

bool MockCanChannelTransmitResponse(t_CAN_MESSAGE * const message, const bool suppress_error_entry)
{
  (void)suppress_error_entry;

  memcpy(&mock.can_channel.transmit_message, message, sizeof(mock.can_channel.transmit_message));
  mock.can_channel.transmit_available = true;

#if false // for debugging
  printf("id: 0x%03X, length: %d, data: ", message->identifier, message->payload_length);
  for (uint8_t byte = 0; byte < message->payload_length; byte++)
    printf("0x%02X ", message->data_byte[byte]);
  printf("\n");
#endif

  return true;
}

bool MockCanChannelTransmitIsPending(void)
{
  return mock.can_channel.transmit_available;
}

bool MockCanChannelMessageWasTransmitted(const t_CAN_MESSAGE * const message)
{
  const bool available = mock.can_channel.transmit_available;

  const bool identical =
      (mock.can_channel.transmit_message.identifier == message->identifier) &&
      (mock.can_channel.transmit_message.fd == message->fd) &&
      (mock.can_channel.transmit_message.payload_length == message->payload_length) &&
      ((message->payload_length < 1) || (mock.can_channel.transmit_message.data_byte[0] == message->data_byte[0])) &&
      ((message->payload_length < 2) || (mock.can_channel.transmit_message.data_byte[1] == message->data_byte[1])) &&
      ((message->payload_length < 3) || (mock.can_channel.transmit_message.data_byte[2] == message->data_byte[2])) &&
      ((message->payload_length < 4) || (mock.can_channel.transmit_message.data_byte[3] == message->data_byte[3])) &&
      ((message->payload_length < 5) || (mock.can_channel.transmit_message.data_byte[4] == message->data_byte[4])) &&
      ((message->payload_length < 6) || (mock.can_channel.transmit_message.data_byte[5] == message->data_byte[5])) &&
      ((message->payload_length < 7) || (mock.can_channel.transmit_message.data_byte[6] == message->data_byte[6])) &&
      ((message->payload_length < 8) || (mock.can_channel.transmit_message.data_byte[7] == message->data_byte[7]));

  mock.can_channel.transmit_available = false;

  return available && identical;
}

void MockCanChannelFlushTransmit(void)
{
  mock.can_channel.transmit_available = false;
}

// --- reboot ------------------------------------------------------------------

void RebootExecute(void)
{
  MOCK_ADD_TO_ACTIONS("Reboot");
}

// --- default -----------------------------------------------------------------

void DefaultReset(void)
{
  MOCK_ADD_TO_ACTIONS("FactoryReset");
}

// --- mode --------------------------------------------------------------------

void ModeInit(void)
{

}

void MockModeSet(const e_MODE mode)
{
  mock.mode = mode;
}

e_MODE ModeGet(void)
{
  return mock.mode;
}

// --- label -------------------------------------------------------------------

e_ERROR_ID LabelGetValue(const uint16_t label, const uint8_t sublabel, uint32_t * const value, const bool broadcast)
{
  ASSERT_FATAL(value != NULL);
  MOCK_ADD_TO_ACTIONS("LabelGet_%s_%04X_%02X", broadcast ? "broadcast" : "singlecast", label, sublabel);
  *value = 0x64636261;

  return ERROR_ID_SUCCESS;
}

e_ERROR_ID LabelSetValue(const uint16_t label, const uint8_t sublabel, const uint32_t * const value, const bool broadcast)
{
  ASSERT_FATAL(value != NULL);
  uint32_t value_copy = 0;
  memcpy(&value_copy, value, sizeof(value_copy));
  MOCK_ADD_TO_ACTIONS("LabelSet_%s_%04X_%02X_%08X", broadcast ? "broadcast" : "singlecast", label, sublabel, value_copy);

  return ERROR_ID_SUCCESS;
}

// --- storage -----------------------------------------------------------------

void StorageStoreSettings(void)
{
  MOCK_ADD_TO_ACTIONS("StoreSettings");
}

// --- flash session -----------------------------------------------------------

e_ERROR_ID FlashSessionErase(const uint32_t address, const uint32_t number_of_bytes)
{
  MOCK_ADD_TO_ACTIONS("FlashErase_0x%08X_0x%08X", address, number_of_bytes);

  return ERROR_ID_SUCCESS;
}

void MockSetFlashSessionStatus(const bool active)
{
  mock.flash_session_active = active;
}

bool FlashSessionIsActive(void)
{
  return mock.flash_session_active;
}

e_ERROR_ID FlashSessionStart(const uint32_t address, const uint32_t number_of_bytes)
{
  MOCK_ADD_TO_ACTIONS("FlashStart_0x%08X_0x%08X", address, number_of_bytes);

  return ERROR_ID_SUCCESS;
}

e_ERROR_ID FlashSessionTransfer(const uint8_t index, const uint8_t number_of_bytes, const uint8_t data[])
{
  MOCK_ADD_TO_ACTIONS("FlashTransferData_%d_%02X_", number_of_bytes, index);
  for (uint8_t byte = 0; byte < number_of_bytes; byte++)
    MOCK_ADD_TO_ACTIONS("%02X", data[byte]);

  return ERROR_ID_SUCCESS;
}

e_ERROR_ID FlashSessionProgram(void)
{
  MOCK_ADD_TO_ACTIONS("FlashProgram");

  return ERROR_ID_SUCCESS;
}

e_ERROR_ID FlashSessionCheck(const uint32_t address, const uint32_t number_of_bytes, uint16_t * const checksum)
{
  ASSERT_FATAL(checksum != NULL);

  MOCK_ADD_TO_ACTIONS("FlashCheck_0x%08X_0x%08X", address, number_of_bytes);

  *checksum = 0x4711;

  return ERROR_ID_SUCCESS;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
