/*---------------------------------------------------------------------------+
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

#include "project/ILabelConfig.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

const t_LABEL_CONFIG label_config[] = {
    { .label_id = LABEL_ID_AUTO_OPERATION,              .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_CONFIG,            .label_type = LABEL_TYPE_BOOL,         .config_id = CONFIG_ID_AUTO_OPERATION                      },
    { .label_id = LABEL_ID_BOOT_COUNTER,                .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UINT16,       .config_id = CONFIG_ID_BOOT_COUNTER                        },
    { .label_id = LABEL_ID_CAN_BIT_RATE_NOMINAL_STORED, .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_CONFIG,            .label_type = LABEL_TYPE_UINT16,       .config_id = CONFIG_ID_CAN_BIT_RATE_PRIMARY_NOMINAL_STORED },
    { .label_id = LABEL_ID_CAN_BIT_RATE_DATA_STORED,    .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_CONFIG,            .label_type = LABEL_TYPE_UINT16,       .config_id = CONFIG_ID_CAN_BIT_RATE_PRIMARY_DATA_STORED    },
    { .label_id = LABEL_ID_CAN_BASE_ID_STORED,          .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_CONFIG,            .label_type = LABEL_TYPE_UINT16,       .config_id = CONFIG_ID_CAN_ID_REQUEST_BROADCAST_STORED     },
    { .label_id = LABEL_ID_CAN_MODULE_ID_OFFSET_STORED, .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_CONFIG,            .label_type = LABEL_TYPE_UINT16,       .config_id = CONFIG_ID_CAN_ID_BLOCK_OFFSET_STORED          },
    { .label_id = LABEL_ID_CLOCK_MASTER,                .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ADVANCED_SMART,    .label_type = LABEL_TYPE_BOOL,         .config_id = CONFIG_ID_CLOCK_MASTER_ACTIVE                 },
    { .label_id = LABEL_ID_CLOCK_MASTER,                .sublabel_id =  1, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_BOOL,         .config_id = CONFIG_ID_CLOCK_MASTER_DETERMINED             },
    { .label_id = LABEL_ID_CLOCK_MASTER,                .sublabel_id =  2, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ADVANCED_CUSTOMER, .label_type = LABEL_TYPE_BOOL,         .config_id = CONFIG_ID_CLOCK_MASTER_STORED                 },
    { .label_id = LABEL_ID_CORE_VERSION,                .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_VERSION,      .config_id = CONFIG_ID_CORE_VERSION                        },
    { .label_id = LABEL_ID_DEVICE_ID,                   .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UINT16,       .config_id = CONFIG_ID_DEVICE_ID                           },
    { .label_id = LABEL_ID_DEVICE_REVISION,             .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UINT16,       .config_id = CONFIG_ID_DEVICE_REVISION                     },
    { .label_id = LABEL_ID_ERROR,                       .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UINT32,       .config_id = CONFIG_ID_ERROR_ENTRY                         },
    { .label_id = LABEL_ID_ERROR_VERBOSE_LEVEL,         .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_OPERATION,         .label_type = LABEL_TYPE_UINT8,        .config_id = CONFIG_ID_ERROR_VERBOSE_LEVEL                 },
    { .label_id = LABEL_ID_ERROR_ADD_ENTRY,             .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_ILLEGAL,        .set_minimum = MODE_ID_ADVANCED_SMART,    .label_type = LABEL_TYPE_UINT16,       .config_id = CONFIG_ID_ADD_ERROR_ENTRY                     },
    { .label_id = LABEL_ID_FIRMWARE_TYPE,               .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UINT16,       .config_id = CONFIG_ID_FIRMWARE_TYPE                       },
    { .label_id = LABEL_ID_FIRMWARE_VERSION,            .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_VERSION,      .config_id = CONFIG_ID_FIRMWARE_VERSION                    },
    { .label_id = LABEL_ID_FIRMWARE_VARIANT,            .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UINT8,        .config_id = CONFIG_ID_FIRMWARE_VARIANT                    },
    { .label_id = LABEL_ID_BLOCK_INFO,                  .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UINT32_ARRAY, .config_id = CONFIG_ID_BLOCK_INFO                          },
    { .label_id = LABEL_ID_HARDWARE_TYPE,               .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ADVANCED_SMART,    .label_type = LABEL_TYPE_UINT16,       .config_id = CONFIG_ID_HARDWARE_TYPE                       },
    { .label_id = LABEL_ID_HARDWARE_VERSION,            .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ADVANCED_SMART,    .label_type = LABEL_TYPE_VERSION,      .config_id = CONFIG_ID_HARDWARE_VERSION                    },
    { .label_id = LABEL_ID_HARDWARE_VARIANT,            .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ADVANCED_SMART,    .label_type = LABEL_TYPE_UINT8,        .config_id = CONFIG_ID_HARDWARE_VARIANT                    },
    { .label_id = LABEL_ID_IDENTIFICATION_SMART,        .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ADVANCED_SMART,    .label_type = LABEL_TYPE_RAW,          .config_id = CONFIG_ID_IDENTIFICATION_SMART                },
    { .label_id = LABEL_ID_IDENTIFICATION_CUSTOMER,     .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ADVANCED_SMART,    .label_type = LABEL_TYPE_RAW,          .config_id = CONFIG_ID_IDENTIFICATION_CUSTOMER             },
    { .label_id = LABEL_ID_MODE,                        .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_OPERATION,         .label_type = LABEL_TYPE_UINT8,        .config_id = CONFIG_ID_MODE                                },
    { .label_id = LABEL_ID_MODULE_NUMBER,               .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UINT8,        .config_id = CONFIG_ID_MODULE_NUMBER_ACTIVE                },
    { .label_id = LABEL_ID_MODULE_NUMBER,               .sublabel_id =  1, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UINT8,        .config_id = CONFIG_ID_MODULE_NUMBER_DETERMINED            },
    { .label_id = LABEL_ID_MODULE_NUMBER,               .sublabel_id =  2, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ADVANCED_CUSTOMER, .label_type = LABEL_TYPE_UINT8,        .config_id = CONFIG_ID_MODULE_NUMBER_STORED                },
    { .label_id = LABEL_ID_PRODUCTION_DATE,             .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ADVANCED_SMART,    .label_type = LABEL_TYPE_DATE,         .config_id = CONFIG_ID_PRODUCTION_DATE                     },
    { .label_id = LABEL_ID_SERIAL_NUMBER,               .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ADVANCED_SMART,    .label_type = LABEL_TYPE_UINT16,       .config_id = CONFIG_ID_SERIAL_NUMBER                       },
    { .label_id = LABEL_ID_SEED_AND_KEY,                .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UINT32,       .config_id = CONFIG_ID_SEED                                },
    { .label_id = LABEL_ID_SEED_AND_KEY,                .sublabel_id =  1, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_ILLEGAL,        .set_minimum = MODE_ID_OPERATION,         .label_type = LABEL_TYPE_UINT32,       .config_id = CONFIG_ID_KEY                                 },
    { .label_id = LABEL_ID_STACK_INFO,                  .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_ADVANCED_SMART, .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UINT32,       .config_id = CONFIG_ID_CURRENT_STACK_POINTER               },
    { .label_id = LABEL_ID_STACK_INFO,                  .sublabel_id =  1, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_ADVANCED_SMART, .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UINT32,       .config_id = CONFIG_ID_STACK_LIMIT                         },
    { .label_id = LABEL_ID_STACK_INFO,                  .sublabel_id =  2, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_ILLEGAL,        .set_minimum = MODE_ID_ADVANCED_SMART,    .label_type = LABEL_TYPE_BOOL,         .config_id = CONFIG_ID_INIT_STACK_TEST                     },
    { .label_id = LABEL_ID_STACK_INFO,                  .sublabel_id =  3, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_ADVANCED_SMART, .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UINT32,       .config_id = CONFIG_ID_STACK_UNUSED                        },
    { .label_id = LABEL_ID_STARTUP_DETERMINATION,       .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ADVANCED_CUSTOMER, .label_type = LABEL_TYPE_UINT8,        .config_id = CONFIG_ID_STARTUP_DETERMINATION               },
    { .label_id = LABEL_ID_STARTUP_DETERMINATION,       .sublabel_id =  1, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_ILLEGAL,        .set_minimum = MODE_ID_ADVANCED_CUSTOMER, .label_type = LABEL_TYPE_BOOL,         .config_id = CONFIG_ID_SET_MODULE_NUMBER_AND_CLOCK_MASTER  },
    { .label_id = LABEL_ID_STAY_IN_FLASHLOADER,         .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_CONFIG,         .set_minimum = MODE_ID_CONFIG,            .label_type = LABEL_TYPE_BOOL,         .config_id = CONFIG_ID_STAY_IN_FLASHLOADER                 },
    { .label_id = LABEL_ID_STORAGE_INFO,                .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UINT32_ARRAY, .config_id = CONFIG_ID_STORAGE_INFO                        },
    { .label_id = LABEL_ID_UPTIME,                      .sublabel_id =  0, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UINT32,       .config_id = CONFIG_ID_UPTIME                              },
    { .label_id = LABEL_ID_UPTIME,                      .sublabel_id =  1, .singlecast_set_allowed = true,  .get_minimum = MODE_ID_OPERATION,      .set_minimum = MODE_ID_ADVANCED_SMART,    .label_type = LABEL_TYPE_UINT32,       .config_id = CONFIG_ID_LIFETIME                            },
    { .label_id = LABEL_ID_LAST_ENTRY,                  .sublabel_id =  0, .singlecast_set_allowed = false, .get_minimum = MODE_ID_ILLEGAL,        .set_minimum = MODE_ID_ILLEGAL,           .label_type = LABEL_TYPE_UNKNOWN,      .config_id = CONFIG_ID_INVALID                             }
};

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/


/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
