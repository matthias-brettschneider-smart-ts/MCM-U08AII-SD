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

#include "middleware/IConfigId.h"
#include "middleware/IConfigType.h"
#include "middleware/IErrorId.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

typedef struct {
  e_CONFIG_ID config_id : 8;
  e_CONFIG_TYPE config_type : 8;
  uint8_t elements;
  uint8_t readable : 1;
  uint8_t writeable : 1;
  void * data_pointer;
} t_CONFIG;

typedef enum {
  CONFIG_CALLBACK_TYPE_PRE,
  CONFIG_CALLBACK_TYPE_POST
} e_CONFIG_CALLBACK_TYPE;

typedef e_ERROR_ID (t_CONFIG_CALLBACK)(const e_CONFIG_CALLBACK_TYPE callback_type, const e_CONFIG_ID config_id, const bool read, const void * const data);
#define CONFIG_CALLBACK(name) e_ERROR_ID (name)(const e_CONFIG_CALLBACK_TYPE callback_type, const e_CONFIG_ID config_id, const bool read, const void * const data)

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

e_ERROR_ID ConfigRegister(const t_CONFIG config[], t_CONFIG_CALLBACK * const callback);

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
