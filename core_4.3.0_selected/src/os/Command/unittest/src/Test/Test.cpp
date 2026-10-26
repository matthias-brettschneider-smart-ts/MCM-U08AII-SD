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

#include "middleware/ICanChannel.h"
#include "middleware/ICanMessage.h"
#include "middleware/IErrorInternal.h"
#include "os/IProtocol.h"

#include "IMock.h"

#include "gtest/gtest.h"

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

// --- helpers -----------------------------------------------------------------

// --- tests -------------------------------------------------------------------

class Command : public ::testing::Test
{
  protected:

  void SetUp() override
  {
    MockModeSet(MODE_ID_CONFIG);
    MockFlushActions();
    MockCanChannelFlushTransmit();
    const char * expected = "";
    ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
    ASSERT_EQ(MockCanChannelReceiveIsPending(), false);
    ASSERT_EQ(MockCanChannelTransmitIsPending(), false);
  }

  void TearDown() override
  {
    const char * expected = "";
    ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
    ASSERT_EQ(MockCanChannelReceiveIsPending(), false);
    ASSERT_EQ(MockCanChannelTransmitIsPending(), false);
  }
};

TEST_F(Command, CanBeInitialized)
{
  ProtocolInit();
}

TEST_F(Command, HandlerCanBeExecutedWithNoRequestPending)
{
  ASSERT_EQ(MockCanChannelReceiveIsPending(), false);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), false);
  ProtocolHandler();
  ASSERT_EQ(MockCanChannelReceiveIsPending(), false);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), false);
}

TEST_F(Command, EmptyRequestIsNotAnswered)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 0,
      .data_byte = { 0, 0, 0, 0, 0, 0, 0, 0 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  ProtocolHandler();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), false);
}

TEST_F(Command, UnknownComandFailsWithError99)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x0B, 0, 0, 0, 0, 0, 0, 0 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  ProtocolHandler();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 0x0B, 0, 0, 0, 0x99, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, ErrorFrameCanBeSend)
{
  ProtocolSendError(ERROR_CATEGORY_WARNING, 0x12, 9, 0x55);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x00, 0x02, 0x00, 0x01, 0x01, 0x12, 0x09, 0x55 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, RequestWelcomeCanBeExecutedWithoutFillBytes)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 1,
      .data_byte = { 2, 0, 0, 0, 0, 0, 0, 0 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  ProtocolHandler();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0xCA, 0xFE, 0xBA, 0xBE, 0xCA, 0xFE, 0xBA, 0xBE }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, RequestWelcomeCanBeExecutedWithFillBytes)
{
  for (uint8_t length = 2; length <= 8; length++) {
    const t_CAN_MESSAGE request = {
        .identifier = CAN_MESSAGE_ID_INVALID,
        .fd = false,
        .bitrate_switching = false,
        .payload_length = length,
        .data_byte = { 2, 0, 0, 0, 0, 0, 0, 0 }
    };
    MockCanChannelPrepareMessageForReceive(&request, true);
    ProtocolHandler();
    ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
    const t_CAN_MESSAGE response = {
        .identifier = CAN_MESSAGE_ID_INVALID,
        .fd = false,
        .bitrate_switching = false,
        .payload_length = 8,
        .data_byte = { 0xCA, 0xFE, 0xBA, 0xBE, 0xCA, 0xFE, 0xBA, 0xBE }
    };
    ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
  }
}

TEST_F(Command, StoreSettingsCanBeExecutedWithoutFillBytes)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 1,
      .data_byte = { 3, 0, 0, 0, 0, 0, 0, 0 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  ProtocolHandler();
  const char * expected = "StoreSettings";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  MockFlushActions();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 1,
      .data_byte = { 3, 0, 0, 0, 0, 0, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, StoreSettingsCanBeExecutedWithFillBytes)
{
  for (uint8_t length = 2; length <= 8; length++) {
    const t_CAN_MESSAGE request = {
        .identifier = CAN_MESSAGE_ID_INVALID,
        .fd = false,
        .bitrate_switching = false,
        .payload_length = length,
        .data_byte = { 3, 0, 0, 0, 0, 0, 0, 0 }
    };
    MockCanChannelPrepareMessageForReceive(&request, true);
    ProtocolHandler();
    const char * expected = "StoreSettings";
    ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
    MockFlushActions();
    ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
    const t_CAN_MESSAGE response = {
        .identifier = CAN_MESSAGE_ID_INVALID,
        .fd = false,
        .bitrate_switching = false,
        .payload_length = 1,
        .data_byte = { 3, 0, 0, 0, 0, 0, 0, 0 }
    };
    ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
  }
}

TEST_F(Command, FactoryResetWithFillBytesFailesWithError99)
{
  for (uint8_t length = 2; length <= 8; length++) {
    const t_CAN_MESSAGE request = {
        .identifier = CAN_MESSAGE_ID_INVALID,
        .fd = false,
        .bitrate_switching = false,
        .payload_length = length,
        .data_byte = { 4, 0, 0, 0, 0, 0, 0, 0 }
    };
    MockCanChannelPrepareMessageForReceive(&request, true);
    ProtocolHandler();
    const char * expected = "";
    ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
    ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
    const t_CAN_MESSAGE response = {
        .identifier = CAN_MESSAGE_ID_INVALID,
        .fd = false,
        .bitrate_switching = false,
        .payload_length = 8,
        .data_byte = { 0x7F, 4, 0, 0, 0, 0x99, 0, 0 }
    };
    ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
  }
}

TEST_F(Command, FactoryResetCanBeExecuted)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 1,
      .data_byte = { 4, 0, 0, 0, 0, 0, 0, 0 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  ProtocolHandler();
  const char * expected = "FactoryReset";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  MockFlushActions();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 1,
      .data_byte = { 4, 0, 0, 0, 0, 0, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, RebootAsSinglecastFailesWithError0D)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 1,
      .data_byte = { 5, 0, 0, 0, 0, 0, 0, 0 }
  };
  MockCanChannelPrepareMessageForReceive(&request, false);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 5, 0, 0, 0, 0x0D, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, RebootWithFillBytesFailesWithError99)
{
  for (uint8_t length = 2; length <= 8; length++) {
    const t_CAN_MESSAGE request = {
        .identifier = CAN_MESSAGE_ID_INVALID,
        .fd = false,
        .bitrate_switching = false,
        .payload_length = length,
        .data_byte = { 5, 0, 0, 0, 0, 0, 0, 0 }
    };
    MockCanChannelPrepareMessageForReceive(&request, true);
    ProtocolHandler();
    const char * expected = "";
    ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
    ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
    const t_CAN_MESSAGE response = {
        .identifier = CAN_MESSAGE_ID_INVALID,
        .fd = false,
        .bitrate_switching = false,
        .payload_length = 8,
        .data_byte = { 0x7F, 5, 0, 0, 0, 0x99, 0, 0 }
    };
    ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
  }
}

TEST_F(Command, RebootCanBeExecutedAsBroadcast)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 1,
      .data_byte = { 5, 0, 0, 0, 0, 0, 0, 0 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  ProtocolHandler();
  const char * expected = "Reboot";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  MockFlushActions();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 1,
      .data_byte = { 5, 0, 0, 0, 0, 0, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, LabelGetCanBeExecutedAsSinglecast)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0, 0x47, 0x11, 0x55, 0, 0, 0, 0 }
  };
  MockCanChannelPrepareMessageForReceive(&request, false);
  MockModeSet(MODE_ID_CONFIG);
  ProtocolHandler();
  const char * expected = "LabelGet_singlecast_4711_55";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  MockFlushActions();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0, 0x47, 0x11, 0x55, 0x64, 0x63, 0x62, 0x61 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, LabelGetCanBeExecutedAsBroadcast)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0, 0x47, 0x11, 0x55, 0, 0, 0, 0 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_CONFIG);
  ProtocolHandler();
  const char * expected = "LabelGet_broadcast_4711_55";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  MockFlushActions();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0, 0x47, 0x11, 0x55, 0x64, 0x63, 0x62, 0x61 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, LabelSetCanBeExecutedAsSinglecast)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 1, 0x08, 0x15, 0x66, 0x74, 0x73, 0x72, 0x71 }
  };
  MockCanChannelPrepareMessageForReceive(&request, false);
  MockModeSet(MODE_ID_CONFIG);
  ProtocolHandler();
  const char * expected = "LabelSet_singlecast_0815_66_74737271";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  MockFlushActions();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 1, 0x08, 0x15, 0x66, 0x74, 0x73, 0x72, 0x71 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, LabelSetCanBeExecutedAsBroadcast)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 1, 0x08, 0x15, 0x66, 0x74, 0x73, 0x72, 0x71 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_CONFIG);
  ProtocolHandler();
  const char * expected = "LabelSet_broadcast_0815_66_74737271";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  MockFlushActions();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 1, 0x08, 0x15, 0x66, 0x74, 0x73, 0x72, 0x71 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashEraseExecutedInModeOperationFailsWithError08)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 6, 0x13, 0x12, 0x11, 0x24, 0x23, 0x22, 0x21 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_OPERATION);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 6, 0, 0, 0, 0x08, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashEraseExecutedInModeConfigFailsWithError08)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 6, 0x13, 0x12, 0x11, 0x24, 0x23, 0x22, 0x21 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_CONFIG);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 6, 0, 0, 0, 0x08, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashEraseExecutedInModeAdvancedCustomerFailsWithError08)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 6, 0x13, 0x12, 0x11, 0x24, 0x23, 0x22, 0x21 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_ADVANCED_CUSTOMER);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 6, 0, 0, 0, 0x08, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashEraseExecutedInModeAdvancedSmartSucceeds)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 6, 0x13, 0x12, 0x11, 0x24, 0x23, 0x22, 0x21 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_ADVANCED_SMART);
  ProtocolHandler();
  const char * expected = "FlashErase_0x24232221_0x00131211";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  MockFlushActions();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 6, 0x13, 0x12, 0x11, 0x24, 0x23, 0x22, 0x21 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashStartExecutedInModeOperationFailsWithError08)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 7, 0x13, 0x12, 0x11, 0x24, 0x23, 0x22, 0x21 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_OPERATION);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 7, 0, 0, 0, 0x08, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashStartExecutedInModeConfigFailsWithError08)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 7, 0x13, 0x12, 0x11, 0x24, 0x23, 0x22, 0x21 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_CONFIG);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 7, 0, 0, 0, 0x08, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashStartExecutedInModeAdvancedCustomerFailsWithError08)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 7, 0x13, 0x12, 0x11, 0x24, 0x23, 0x22, 0x21 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_ADVANCED_CUSTOMER);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 7, 0, 0, 0, 0x08, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashStartExecutedInModeAdvancedSmartSucceeds)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 7, 0x13, 0x12, 0x11, 0x24, 0x23, 0x22, 0x21 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_ADVANCED_SMART);
  ProtocolHandler();
  const char * expected = "FlashStart_0x24232221_0x00131211";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  MockFlushActions();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 7, 0x13, 0x12, 0x11, 0x24, 0x23, 0x22, 0x21 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashTransferDataExecutedInModeOperationSucceeds)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 8, 0x77, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_OPERATION);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 8, 0, 0, 0, 0x08, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashTransferDataExecutedInModeConfigSucceeds)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 8, 0x77, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_CONFIG);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 8, 0, 0, 0, 0x08, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashTransferDataExecutedInModeAdvancedCustomerSucceeds)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 8, 0x77, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_ADVANCED_CUSTOMER);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 8, 0, 0, 0, 0x08, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashTransferDataExecutedInModeAdvancedSmartSucceeds)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 8, 0x77, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_ADVANCED_SMART);
  ProtocolHandler();
  const char * expected = "FlashTransferData_6_77_515253545556";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  MockFlushActions();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), false);
}

TEST_F(Command, FlashTransferDataWithoutPayloadFailesWithError99)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 2,
      .data_byte = { 8, 0x77, 0, 0, 0, 0, 0, 0 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_ADVANCED_SMART);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 8, 0, 0, 0, 0x99, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashProgramExecutedInModeOperationFailsWithError08)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 1,
      .data_byte = { 9, 0, 0, 0, 0, 0, 0, 0 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_OPERATION);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 9, 0, 0, 0, 0x08, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashProgramExecutedInModeConfigFailsWithError08)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 1,
      .data_byte = { 9, 0, 0, 0, 0, 0, 0, 0 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_CONFIG);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 9, 0, 0, 0, 0x08, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashProgramExecutedInModeAdvancedCustomerFailsWithError08)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 1,
      .data_byte = { 9, 0, 0, 0, 0, 0, 0, 0 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_ADVANCED_CUSTOMER);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 9, 0, 0, 0, 0x08, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashProgramExecutedInModeAdvancedSmartSucceeds)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 1,
      .data_byte = { 9, 0, 0, 0, 0, 0, 0, 0 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_ADVANCED_SMART);
  ProtocolHandler();
  const char * expected = "FlashProgram";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  MockFlushActions();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 1,
      .data_byte = { 9, 0, 0, 0, 0, 0, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashCheckMemoryExecutedInModeOperationFailsWithError08)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x0A, 0x13, 0x12, 0x11, 0x24, 0x23, 0x22, 0x21 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_OPERATION);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  MockFlushActions();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 0x0A, 0, 0, 0, 0x08, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashCheckMemoryExecutedInModeConfigFailsWithError08)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x0A, 0x13, 0x12, 0x11, 0x24, 0x23, 0x22, 0x21 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_CONFIG);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  MockFlushActions();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 0x0A, 0, 0, 0, 0x08, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashCheckMemoryExecutedInModeAdvancedCustomerFailsWithError08)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x0A, 0x13, 0x12, 0x11, 0x24, 0x23, 0x22, 0x21 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_ADVANCED_CUSTOMER);
  ProtocolHandler();
  const char * expected = "";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  MockFlushActions();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x7F, 0x0A, 0, 0, 0, 0x08, 0, 0 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashCheckMemoryExecutedInModeAdvancedSmartSucceeds)
{
  const t_CAN_MESSAGE request = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x0A, 0x13, 0x12, 0x11, 0x24, 0x23, 0x22, 0x21 }
  };
  MockCanChannelPrepareMessageForReceive(&request, true);
  MockModeSet(MODE_ID_ADVANCED_SMART);
  ProtocolHandler();
  const char * expected = "FlashCheck_0x24232221_0x00131211";
  ASSERT_EQ(MockActionIsEqual(expected, strlen(expected)), true);
  MockFlushActions();
  ASSERT_EQ(MockCanChannelTransmitIsPending(), true);
  const t_CAN_MESSAGE response = {
      .identifier = CAN_MESSAGE_ID_INVALID,
      .fd = false,
      .bitrate_switching = false,
      .payload_length = 8,
      .data_byte = { 0x0A, 0, 0, 0, 0, 0, 0x47, 0x11 }
  };
  ASSERT_EQ(MockCanChannelMessageWasTransmitted(&response), true);
}

TEST_F(Command, FlashSessionStatusCanBeReadViaProtocolInterface)
{
  MockSetFlashSessionStatus(false);
  ASSERT_EQ(ProtocolIsProgrammingInProgress(), false);
  MockSetFlashSessionStatus(true);
  ASSERT_EQ(ProtocolIsProgrammingInProgress(), true);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 -----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
