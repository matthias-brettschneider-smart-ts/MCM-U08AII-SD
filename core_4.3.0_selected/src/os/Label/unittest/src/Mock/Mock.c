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

#include "middleware/IConfig.h"
#include "middleware/IConfigInternal.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"
#include "os/IMode.h"

#include "IMock.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef struct {
  e_MODE mode;
  bool bool_single;
  bool bool_array_8[8];
  int8_t int8;
  int16_t int16;
  uint32_t uint32;
  t_VERSION version;
  t_DATE date;
  uint8_t raw32[32];
} t_MOCK;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_MOCK mock = {
    .mode = MODE_ID_ILLEGAL,
    .bool_single = false,
    .bool_array_8 = { false, true, true, false, false, false, false, true },
    .int8 = -57,
    .int16 = -321,
    .uint32 = 0x12345678,
    .version = { .minor = 1, .major = 2, .patch = 1000 },
    .date = { .year = 2023, .month = 2, .day = 21 },
    .raw32 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

// --- config ------------------------------------------------------------------

bool ConfigIsAvailable(const e_CONFIG_ID config_id)
{
  return
      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_BOOL) ||
      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY8) ||
      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_INT8) ||
      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_INT16) ||
      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_UINT32) ||
      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_VERSION) ||
      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_DATE) ||
      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_RAW32);
}

e_CONFIG_TYPE ConfigGetType(const e_CONFIG_ID config_id)
{
  return
      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_BOOL) ? CONFIG_TYPE_BOOL :
          (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY8) ? CONFIG_TYPE_BOOL :
              (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_INT8) ? CONFIG_TYPE_INT8 :
                  (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_INT16) ? CONFIG_TYPE_INT16 :
                      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_UINT32) ? CONFIG_TYPE_UINT32 :
                          (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_VERSION) ? CONFIG_TYPE_VERSION :
                              (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_DATE) ? CONFIG_TYPE_DATE :
                                  (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_RAW32) ? CONFIG_TYPE_RAW :
                                      CONFIG_TYPE_UNKNOWN;
}

size_t ConfigGetSize(const e_CONFIG_ID config_id)
{
  return
      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_BOOL) ? 1 :
          (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY8) ? 8 :
              (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_INT8) ? 1 :
                  (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_INT16) ? 2 :
                      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_UINT32) ? 4 :
                          (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_VERSION) ? 4 :
                              (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_DATE) ? 4 :
                                  (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_RAW32) ? 32 :
                                      0;
}

e_ERROR_ID ConfigReadValue(const e_CONFIG_ID config_id, void * const value, const size_t size)
{
  e_ERROR_ID result = ERROR_ID_SUCCESS;

  if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_BOOL) && (size == sizeof(mock.bool_single)))
    memcpy(value, &mock.bool_single, sizeof(mock.bool_single));
  else if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY8) && (size == sizeof(mock.bool_array_8)))
    memcpy(value, &mock.bool_array_8, sizeof(mock.bool_array_8));
  else if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_INT8) && (size == sizeof(mock.int8)))
    memcpy(value, &mock.int8, sizeof(mock.int8));
  else if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_INT16) && (size == sizeof(mock.int16)))
    memcpy(value, &mock.int16, sizeof(mock.int16));
  else if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_UINT32) && (size == sizeof(mock.uint32)))
    memcpy(value, &mock.uint32, sizeof(mock.uint32));
  else if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_VERSION) && (size == sizeof(mock.version)))
    memcpy(value, &mock.version, sizeof(mock.version));
  else if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_DATE) && (size == sizeof(mock.date)))
    memcpy(value, &mock.date, sizeof(mock.date));
  else if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_RAW32) && (size == sizeof(mock.raw32)))
    memcpy(value, &mock.raw32, sizeof(mock.raw32));
  else
    result = ERROR_ID_UNKNOWN_VALUE;

  return result;
}

e_ERROR_ID ConfigWriteValue(const e_CONFIG_ID config_id, const void * const value, const size_t size)
{
  e_ERROR_ID result = ERROR_ID_SUCCESS;

  if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_BOOL) && (size == sizeof(mock.bool_single)))
    memcpy(&mock.bool_single, value, sizeof(mock.bool_single));
  else if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY8) && (size == sizeof(mock.bool_array_8)))
    memcpy(&mock.bool_array_8, value, sizeof(mock.bool_array_8));
  else if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_INT8) && (size == sizeof(mock.int8)))
    memcpy(&mock.int8, value, sizeof(mock.int8));
  else if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_INT16) && (size == sizeof(mock.int16)))
    memcpy(&mock.int16, value, sizeof(mock.int16));
  else if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_UINT32) && (size == sizeof(mock.uint32)))
    memcpy(&mock.uint32, value, sizeof(mock.uint32));
  else if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_VERSION) && (size == sizeof(mock.version)))
    memcpy(&mock.version, value, sizeof(mock.version));
  else if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_DATE) && (size == sizeof(mock.date)))
    memcpy(&mock.date, value, sizeof(mock.date));
  else if ((config_id == (e_CONFIG_ID)TEST_CONFIG_ID_RAW32) && (size == sizeof(mock.raw32)))
    memcpy(&mock.raw32, value, sizeof(mock.raw32));
  else
    result = ERROR_ID_UNKNOWN_VALUE;

  return result;
}

bool MockValueAsExpected(const e_CONFIG_ID config_id, const void * const value, const size_t size)
{
  const size_t size_expected =
      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_BOOL) ? sizeof(mock.bool_single) :
          (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY8) ? sizeof(mock.bool_array_8) :
              (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_INT8) ? sizeof(mock.int8) :
                  (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_INT16) ? sizeof(mock.int16) :
                      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_UINT32) ? sizeof(mock.uint32) :
                          (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_VERSION) ? sizeof(mock.version) :
                              (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_DATE) ? sizeof(mock.date) :
                                  (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_RAW32) ? sizeof(mock.raw32) :
                                      0;

  const void * const value_expected =
      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_BOOL) ? (void *)&mock.bool_single :
          (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_BOOL_ARRAY8) ? (void *)mock.bool_array_8 :
              (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_INT8) ? (void *)&mock.int8 :
                  (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_INT16) ? (void *)&mock.int16 :
                      (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_UINT32) ? (void *)&mock.uint32 :
                          (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_VERSION) ? (void *)&mock.version :
                              (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_DATE) ? (void *)&mock.date :
                                  (config_id == (e_CONFIG_ID)TEST_CONFIG_ID_RAW32) ? (void *)mock.raw32 :
                                      NULL;

  const bool size_as_expected = size == size_expected;
  const bool content_as_expected = memcmp(value, value_expected, size) == 0;

  return size_as_expected && content_as_expected;
}

// --- mode --------------------------------------------------------------------

e_ERROR_ID MockModeSet(const e_MODE new_mode)
{
  mock.mode = new_mode;

  return ERROR_ID_SUCCESS;
}

e_MODE ModeGet(void)
{
  return mock.mode;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
