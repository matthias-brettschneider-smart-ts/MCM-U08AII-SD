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

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

typedef enum {
  LABEL_ID_MODE                                                         = 0x0001u,
  LABEL_ID_CHANNEL_STATES                                               = 0x0002u,
  LABEL_ID_CHANNEL_RANGES                                               = 0x0003u,
  LABEL_ID_HARDWARE_TYPE                                                = 0x0100u,
  LABEL_ID_HARDWARE_VERSION                                             = 0x0101u,
  LABEL_ID_SERIAL_NUMBER                                                = 0x0102u,
  LABEL_ID_PRODUCTION_DATE                                              = 0x0103u,
  LABEL_ID_FIRMWARE_TYPE                                                = 0x0104u,
  LABEL_ID_FIRMWARE_VERSION                                             = 0x0105u,
  LABEL_ID_HARDWARE_VARIANT                                             = 0x0107u,
  LABEL_ID_FIRMWARE_VARIANT                                             = 0x0108u,
#if false // not in use any more
  LABEL_ID_CAN_BITRATE                                                  = 0x010Au,
  LABEL_ID_CAN_BASE_ID                                                  = 0x010Bu,
#endif
  LABEL_ID_MODULE_NUMBER                                                = 0x010Eu,
  LABEL_ID_FILTER_TYPE                                                  = 0x0112u,
  LABEL_ID_FILTER_PARAMETER                                             = 0x0113u,
#if false // not in use any more
  LABEL_ID_MEASURE_MODE                                                 = 0x0114u,
  LABEL_ID_ENCODER_SUPPLY                                               = 0x0115u,
  LABEL_ID_EDGE_TYPE                                                    = 0x0116u,
  LABEL_ID_PITCH                                                        = 0x0117u,
#endif
  LABEL_ID_COMPARATOR_LEVEL                                             = 0x0118u,
#if false // not in use any more
  LABEL_ID_CHANNEL_OUTPUT_MODE                                          = 0x0119u,
  LABEL_ID_CHANNEL_OUTPUT_STATES                                        = 0x011Au,
  LABEL_ID_CHANNEL_OUTPUT_FREQUENCY                                     = 0x011Bu,
#endif
  LABEL_ID_CHANNEL_OUTPUT_DUTY_CYCLE                                    = 0x011Cu,
#if false // not in use any more
  LABEL_ID_SHUNT                                                        = 0x011Du,
  LABEL_ID_DECIMATION                                                   = 0x011Eu,
  LABEL_ID_GPIO_MODE                                                    = 0x011Fu,
  LABEL_ID_GPIO_STATE                                                   = 0x0120u,
  LABEL_ID_RELAY_STATE                                                  = 0x0121u,
  LABEL_ID_GPIO_INPUTS                                                  = 0x0127u,
#endif
  LABEL_ID_STATIC_CHANNEL                                               = 0x0128u,
  LABEL_ID_PERIOD_LIMIT                                                 = 0x0130u,
#if false // not in use any more
  LABEL_ID_INPUT_CAPTURE_EDGES                                          = 0x0131u,
  LABEL_ID_INPUT_CAPTURE_MODES                                          = 0x0132u,
#endif
  LABEL_ID_IDENTIFICATION_CUSTOMER                                      = 0x0150u,
  LABEL_ID_IDENTIFICATION_SMART                                         = 0x0151u,
  LABEL_ID_ERROR                                                        = 0x0200u,
  LABEL_ID_ERROR_VERBOSE_LEVEL                                          = 0x0201u,
  LABEL_ID_AUTO_OPERATION                                               = 0x0202u,
  LABEL_ID_CAN_TRANSMISSION_SAMPLES                                     = 0x0203u,
#if false // not in use any more
  LABEL_ID_BLOCK_LIST                                                   = 0x0204u,
#endif
  LABEL_ID_DEVICE_ID                                                    = 0x0206u,
  LABEL_ID_DEVICE_REVISION                                              = 0x0207u,
  LABEL_ID_MODULE_TEMPERATURE                                           = 0x0208u,
  LABEL_ID_CAN_TRANSMISSION_RATE                                        = 0x0209u,
#if false // not in use any more
  LABEL_ID_CAN_MODULE_ID_OFFSET                                         = 0x020Au,
#endif
  LABEL_ID_POWER_SUPPLY                                                 = 0x020Bu,
  LABEL_ID_BLOCK_INFO                                                   = 0x020Du,
  LABEL_ID_STAY_IN_FLASHLOADER                                          = 0x020Eu,
  LABEL_ID_CABLE_BREAK_DETECTION                                        = 0x020Fu,
#if false // not in use any more
  LABEL_ID_TRANSMISSION_UART                                            = 0x0210u,
#endif
  LABEL_ID_BOOT_COUNTER                                                 = 0x0211u,
  LABEL_ID_STORAGE_INFO                                                 = 0x0212u,
  LABEL_ID_UPTIME                                                       = 0x0213u,
  LABEL_ID_GROUND_SHIFT_AVERAGE                                         = 0x0214u,
  LABEL_ID_CLOCK_MASTER                                                 = 0x0215u,
  LABEL_ID_STARTUP_DETERMINATION                                        = 0x0216u,
  LABEL_ID_STACK_INFO                                                   = 0x0217u,
  LABEL_ID_UNLOCK_MASTER_CLOCK                                          = 0x0218u,
  LABEL_ID_SEED_AND_KEY                                                 = 0x0219u,
  LABEL_ID_CHANNEL_RESOLUTION                                           = 0x021Au,
  LABEL_ID_CAN_TRANSMISSION_LAYOUT                                      = 0x021Bu,
  LABEL_ID_CHANNEL_ANTI_ALIASING_FILTER                                 = 0x021Cu,
  LABEL_ID_CHANNEL_RESISTOR                                             = 0x021Du,
  LABEL_ID_ERROR_ADD_ENTRY                                              = 0x021Eu,
  LABEL_ID_CORE_VERSION                                                 = 0x021Fu,
  LABEL_ID_HARDWARE_CONFIGURATION                                       = 0x0220u,
  LABEL_ID_CHANNEL_MODE                                                 = 0x0221u,
  LABEL_ID_CHANNEL_STATE                                                = 0x0222u,
  LABEL_ID_CHANNEL_FREQUENCY                                            = 0x0223u,
  LABEL_ID_CHANNEL_AC                                                   = 0x0224u,
  LABEL_ID_CHANNEL_FAST_START                                           = 0x0225u,
  LABEL_ID_DSM_NUMBER_OF_LANES                                          = 0x0226u,
  LABEL_ID_DSM_NUMBER_OF_SLOTS                                          = 0x0227u,
  LABEL_ID_DSM_NUMBER_OF_BLOCKS                                         = 0x0228u,
  LABEL_ID_DSM_SCHEDULING                                               = 0x0229u,
  LABEL_ID_NUMBER_OF_CHANNELS                                           = 0x0230u,
#if false // not in use any more
  LABEL_ID_FAKE_OLD_UART_VIA_DSM                                        = 0x0231u,
#endif
  LABEL_ID_BMS_FLAGS                                                    = 0x0232u,
  LABEL_ID_BMS_DIE_TEMPERATURE                                          = 0x0233u,
  LABEL_ID_BMS_SUPPLY_HIGH_VOLTAGE_PLUS                                 = 0x0234u,
  LABEL_ID_BMS_SUPPLY_VOLTAGE_PLUS                                      = 0x0235u,
  LABEL_ID_BMS_SUPPLY_DIGITAL                                           = 0x0236u,
  LABEL_ID_BMS_SUPPLY_ANALOG                                            = 0x0237u,
  LABEL_ID_BMS_SUPPLY_SECOND_REFERENCE                                  = 0x0238u,
  LABEL_ID_UNDERVOLTAGE_LIMIT                                           = 0x0239u,
  LABEL_ID_FRONTEND_SELECTION                                           = 0x023Au,
  LABEL_ID_CAN_BIT_RATE_NOMINAL_STORED                                  = 0x023Bu,
  LABEL_ID_CAN_BIT_RATE_DATA_STORED                                     = 0x023Cu,
  LABEL_ID_CAN_BASE_ID_STORED                                           = 0x023Du,
  LABEL_ID_CAN_MODULE_ID_OFFSET_STORED                                  = 0x023Eu,
  LABEL_ID_CABLE_BREAK_DETECTION_MODE                                   = 0x0240u,
  LABEL_ID_CABLE_BREAK_DETECTION_ALGORITHM                              = 0x0241u,
  LABEL_ID_CABLE_BREAK_DETECTION_CLASSIC_SENSITIVITY                    = 0x0242u,
  LABEL_ID_CABLE_BREAK_DETECTION_CLASSIC_REACTION_TIME                  = 0x0243u,
  LABEL_ID_CABLE_BREAK_DETECTION_CLASSIC_HOLD_TIME                      = 0x0244u,
  LABEL_ID_CABLE_BREAK_DETECTION_CORE_SENSITIVITY                       = 0x0245u,
  LABEL_ID_CABLE_BREAK_DETECTION_CORE_REACTION_TIME                     = 0x0246u,
  LABEL_ID_CABLE_BREAK_DETECTION_CORE_HOLD_TIME                         = 0x0247u,
  LABEL_ID_SIGNAL_EDGE                                                  = 0x0248u,
  LABEL_ID_DSM_RESET                                                    = 0x0249u,
  LABEL_ID_CHANNEL_VOLTAGE                                              = 0x024Au,
  LABEL_ID_DSM_TEST_DATA_RECEIVE                                        = 0x024Bu,
  LABEL_ID_DSM_TEST_DATA_TRANSMIT                                       = 0x024Cu,
  LABEL_ID_DSM_TEST_SYNCHRONIZE                                         = 0x024Du,
  LABEL_ID_HEAP_INFO                                                    = 0x024Eu,
  LABEL_ID_CHANNEL_HIGH_TIME                                            = 0x0250u,
  LABEL_ID_CHANNEL_HIGH_VOLTAGE                                         = 0x0251u,
  LABEL_ID_CHANNEL_LOW_TIME                                             = 0x0252u,
  LABEL_ID_CHANNEL_LOW_VOLTAGE                                          = 0x0253u,
  LABEL_ID_CHANNEL_CONTROL_MODE                                         = 0x0260u,
  LABEL_ID_CHANNEL_ON_TIME                                              = 0x0261u,
  LABEL_ID_CHANNEL_REPEAT_TIME                                          = 0x0262u,
  LABEL_ID_CHANNEL_NUMBER_OF_ACTIVATIONS                                = 0x0263u,
  LABEL_ID_CHANNEL_EXTINCTION_LEVEL                                     = 0x0264u,
  LABEL_ID_CHANNEL_SIGNAL_LOGIC                                         = 0x026Cu,
  LABEL_ID_CHANNEL_STATIC_ACTIVATION                                    = 0x026Eu,
  LABEL_ID_CHANNEL_SEQUENCE_ACTIVATION                                  = 0x026Fu,
  LABEL_ID_CHANNEL_PEAK_PHASE_MODE                                      = 0x0270u,
  LABEL_ID_CHANNEL_PEAK_PHASE_TIME_LIMIT                                = 0x0271u,
  LABEL_ID_CHANNEL_PEAK_PHASE_CURRENT                                   = 0x0272u,
  LABEL_ID_CHANNEL_PEAK_PHASE_CURRENT_LIMIT                             = 0x0273u,
  LABEL_ID_CHANNEL_HOLD_PHASE_MODE                                      = 0x0275u,
  LABEL_ID_CHANNEL_HOLD_PHASE_CURRENT                                   = 0x0276u,
  LABEL_ID_CHANNEL_HOLD_PHASE_FREQUENCY                                 = 0x0277u,
  LABEL_ID_CHANNEL_HOLD_PHASE_DUTY_CYCLE                                = 0x0278u,
  LABEL_ID_CHANNEL_PEAK_PHASE_CURRENT_LOWER                             = 0x0280u,
  LABEL_ID_CHANNEL_PEAK_PHASE_CURRENT_UPPER                             = 0x0281u,
  LABEL_ID_CHANNEL_HOLD_PHASE_CURRENT_LOWER                             = 0x0282u,
  LABEL_ID_CHANNEL_HOLD_PHASE_CURRENT_UPPER                             = 0x0283u,
#if false // not in use any more
  LABEL_ID_CHANNEL_MAPPING_SEVEN_GROUPS                                 = 0x0290u,
#endif
  LABEL_ID_CAN_TRANSMISSION_LAYOUT_EKPO                                 = 0x0291u,
  LABEL_ID_CHANNEL_MAPPING_NINE_GROUPS                                  = 0x0292u,
  // add new labels here

#if false // not in use any more
  LABEL_ID_CALIBRATION_RAW_AVERAGE                                      = 0x1000u,
#endif
  LABEL_ID_HEAD_CTRL_INDEX                                              = 0x3000u, // IME: core - rename head controller labels
  LABEL_ID_NUM_MEAS_CTRLS                                               = 0x3001u, //       |
  LABEL_ID_CAN_SUM_RATE                                                 = 0x3002u, //       |
  LABEL_ID_CAN_VALUE_RATE                                               = 0x3003u, //       |
  LABEL_ID_INITIALIZE_MEAS_CTRLS                                        = 0x300au, //       |
  LABEL_ID_MASTER_ENABLE                                                = 0x3010u, //       |
  LABEL_ID_MASTER_NUM                                                   = 0x3011u, //       |
  LABEL_ID_POLARITY_INVERSION                                           = 0x3012u, //       |
  LABEL_ID_GATEWAY_ENABLE                                               = 0x3020u, //      -+-
  LABEL_ID_ADJUSTMENT_SELECTION                                         = 0x4000u,
  LABEL_ID_ADJUSTMENT_DATE_SMART                                        = 0x4110u,
  LABEL_ID_ADJUSTMENT_COUNT_SMART                                       = 0x4120u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_FACTOR_RANGE_0                         = 0x4130u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_FACTOR_RANGE_1                         = 0x4131u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_FACTOR_RANGE_2                         = 0x4132u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_FACTOR_RANGE_3                         = 0x4133u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_FACTOR_RANGE_4                         = 0x4134u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_FACTOR_RANGE_5                         = 0x4135u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_FACTOR_RANGE_6                         = 0x4136u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_FACTOR_RANGE_7                         = 0x4137u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_OFFSET_RANGE_0                         = 0x4140u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_OFFSET_RANGE_1                         = 0x4141u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_OFFSET_RANGE_2                         = 0x4142u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_OFFSET_RANGE_3                         = 0x4143u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_OFFSET_RANGE_4                         = 0x4144u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_OFFSET_RANGE_5                         = 0x4145u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_OFFSET_RANGE_6                         = 0x4146u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_OFFSET_RANGE_7                         = 0x4147u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_GROUND_SHIFT_RANGE_0                   = 0x4150u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_GROUND_SHIFT_RANGE_1                   = 0x4151u,
  LABEL_ID_ADJUSTMENT_DATA_SMART_GROUND_SHIFT_RANGE_2                   = 0x4152u,
  LABEL_ID_ADJUSTMENT_DATE_CUSTOMER                                     = 0x4210u,
  LABEL_ID_ADJUSTMENT_COUNT_CUSTOMER                                    = 0x4220u,
  LABEL_ID_ADJUSTMENT_DATA_CUSTOMER_FACTOR_RANGE_0                      = 0x4230u,
  LABEL_ID_ADJUSTMENT_DATA_CUSTOMER_FACTOR_RANGE_1                      = 0x4231u,
  LABEL_ID_ADJUSTMENT_DATA_CUSTOMER_FACTOR_RANGE_2                      = 0x4232u,
  LABEL_ID_ADJUSTMENT_DATA_CUSTOMER_OFFSET_RANGE_0                      = 0x4240u,
  LABEL_ID_ADJUSTMENT_DATA_CUSTOMER_OFFSET_RANGE_1                      = 0x4241u,
  LABEL_ID_ADJUSTMENT_DATA_CUSTOMER_OFFSET_RANGE_2                      = 0x4242u,
  LABEL_ID_ADJUSTMENT_DATA_CUSTOMER_GROUND_SHIFT_RANGE_0                = 0x4250u,
  LABEL_ID_ADJUSTMENT_DATA_CUSTOMER_GROUND_SHIFT_RANGE_1                = 0x4251u,
  LABEL_ID_ADJUSTMENT_DATA_CUSTOMER_GROUND_SHIFT_RANGE_2                = 0x4252u,
  LABEL_ID_LAST_ENTRY                                                   = 0xFFFFu
} e_LABEL_ID;

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
