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

#include "hal/ICompiler.h"
#include "middleware/IAssert.h"
#include "middleware/IConfigInternal.h"
#include "middleware/IGeneric.h"
#include "os/ILabel.h"
#include "os/IMode.h"
#include "project/ILabelConfig.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define LABEL_MAXIMUM_NUMBER_OF_SUBLABELS                                     32

typedef struct {
  e_LABEL_TYPE label_type : 8;
  e_CONFIG_TYPE config_type : 4;
  bool is_array_or_raw : 1;
  uint8_t bytes_per_sublabel : 3;
} t_LABEL_DEFINITION;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static const t_LABEL_DEFINITION label_definition[] = {

    // individual
    { .label_type = LABEL_TYPE_BOOL,          .config_type = CONFIG_TYPE_BOOL,    .is_array_or_raw = false, .bytes_per_sublabel = 1 },
    { .label_type = LABEL_TYPE_DATE,          .config_type = CONFIG_TYPE_DATE,    .is_array_or_raw = false, .bytes_per_sublabel = 4 },
    { .label_type = LABEL_TYPE_FLOAT32,       .config_type = CONFIG_TYPE_FLOAT32, .is_array_or_raw = false, .bytes_per_sublabel = 4 },
    { .label_type = LABEL_TYPE_INT8,          .config_type = CONFIG_TYPE_INT8,    .is_array_or_raw = false, .bytes_per_sublabel = 1 },
    { .label_type = LABEL_TYPE_INT16,         .config_type = CONFIG_TYPE_INT16,   .is_array_or_raw = false, .bytes_per_sublabel = 2 },
    { .label_type = LABEL_TYPE_INT32,         .config_type = CONFIG_TYPE_INT32,   .is_array_or_raw = false, .bytes_per_sublabel = 4 },
    { .label_type = LABEL_TYPE_UINT8,         .config_type = CONFIG_TYPE_UINT8,   .is_array_or_raw = false, .bytes_per_sublabel = 1 },
    { .label_type = LABEL_TYPE_UINT16,        .config_type = CONFIG_TYPE_UINT16,  .is_array_or_raw = false, .bytes_per_sublabel = 2 },
    { .label_type = LABEL_TYPE_UINT32,        .config_type = CONFIG_TYPE_UINT32,  .is_array_or_raw = false, .bytes_per_sublabel = 4 },
    { .label_type = LABEL_TYPE_VERSION,       .config_type = CONFIG_TYPE_VERSION, .is_array_or_raw = false, .bytes_per_sublabel = 4 },

    // array
    { .label_type = LABEL_TYPE_BOOL_ARRAY,    .config_type = CONFIG_TYPE_BOOL,    .is_array_or_raw = true,  .bytes_per_sublabel = 1 },
    { .label_type = LABEL_TYPE_FLOAT32_ARRAY, .config_type = CONFIG_TYPE_FLOAT32, .is_array_or_raw = true,  .bytes_per_sublabel = 4 },
    { .label_type = LABEL_TYPE_INT8_ARRAY,    .config_type = CONFIG_TYPE_INT8,    .is_array_or_raw = true,  .bytes_per_sublabel = 1 },
    { .label_type = LABEL_TYPE_INT16_ARRAY,   .config_type = CONFIG_TYPE_INT16,   .is_array_or_raw = true,  .bytes_per_sublabel = 2 },
    { .label_type = LABEL_TYPE_INT32_ARRAY,   .config_type = CONFIG_TYPE_INT32,   .is_array_or_raw = true,  .bytes_per_sublabel = 4 },
    { .label_type = LABEL_TYPE_UINT8_ARRAY,   .config_type = CONFIG_TYPE_UINT8,   .is_array_or_raw = true,  .bytes_per_sublabel = 1 },
    { .label_type = LABEL_TYPE_UINT16_ARRAY,  .config_type = CONFIG_TYPE_UINT16,  .is_array_or_raw = true,  .bytes_per_sublabel = 2 },
    { .label_type = LABEL_TYPE_UINT32_ARRAY,  .config_type = CONFIG_TYPE_UINT32,  .is_array_or_raw = true,  .bytes_per_sublabel = 4 },

    // raw
    { .label_type = LABEL_TYPE_RAW,           .config_type = CONFIG_TYPE_RAW,     .is_array_or_raw = true,  .bytes_per_sublabel = 4 }
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

COMPILER_INLINE e_ERROR_ID LABEL_GET_ENTRY(const t_LABEL_CONFIG ** entry, const uint16_t label, const uint8_t sublabel);
COMPILER_INLINE uint8_t LABEL_GET_BYTES_PER_SUBLABEL(const e_LABEL_TYPE label_type);
static bool LabelTypeIsArrayOrRaw(const e_LABEL_TYPE label_type);
COMPILER_INLINE bool LABEL_TYPE_IS_COMPATIBLE(const e_LABEL_TYPE label_type, const e_CONFIG_TYPE config_type);
COMPILER_INLINE bool LABEL_VALUE_IS_WITHIN_RANGE(const e_LABEL_TYPE label_type, const uint32_t value);

static e_ERROR_ID LabelAccess(const uint16_t label, const uint8_t sublabel, uint32_t * const value, const bool read, const bool broadcast);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get the configuration entry for the given label/sublabel.
 *
 * For arrays and raws, only the label has to match.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE e_ERROR_ID LABEL_GET_ENTRY(const t_LABEL_CONFIG ** entry, const uint16_t label, const uint8_t sublabel)
{
  e_ERROR_ID result = ERROR_ID_UNKNOWN_LABEL;

  uint16_t index = 0;
  while ((label_config[index].label_id != LABEL_ID_LAST_ENTRY) && (result != ERROR_ID_SUCCESS)) {
    if (label_config[index].label_id == label) {
      result = ERROR_ID_UNKNOWN_SUBLABEL;
      e_LABEL_TYPE label_type = label_config[index].label_type;
      bool matches_sublabel = label_config[index].sublabel_id == sublabel;
      if (matches_sublabel || LabelTypeIsArrayOrRaw(label_type)) {
        *entry = &label_config[index];
        result = ERROR_ID_SUCCESS;
      }
    }
    index++;
  }

  return result;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE uint8_t LABEL_GET_BYTES_PER_SUBLABEL(const e_LABEL_TYPE label_type)
{
  uint8_t bytes_per_sublabel = 0;
  for (size_t type = 0; type < (sizeof(label_definition) / sizeof(t_LABEL_DEFINITION)); type++)
    if (label_type == label_definition[type].label_type)
      bytes_per_sublabel = label_definition[type].bytes_per_sublabel;

  return bytes_per_sublabel;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
static bool LabelTypeIsArrayOrRaw(const e_LABEL_TYPE label_type)
{
  bool is_array_or_raw = false;
  for (size_t type = 0; type < (sizeof(label_definition) / sizeof(t_LABEL_DEFINITION)); type++)
    if (label_type == label_definition[type].label_type)
      is_array_or_raw = label_definition[type].is_array_or_raw;

  return is_array_or_raw;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool LABEL_TYPE_IS_COMPATIBLE(const e_LABEL_TYPE label_type, const e_CONFIG_TYPE config_type)
{
  bool compatible = false;
  if (config_type != CONFIG_TYPE_UNKNOWN)
    for (size_t type = 0; type < (sizeof(label_definition) / sizeof(t_LABEL_DEFINITION)); type++)
      if ((label_type == label_definition[type].label_type) && (config_type == label_definition[type].config_type))
        compatible = true;

  return compatible;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool LABEL_VALUE_IS_WITHIN_RANGE(const e_LABEL_TYPE label_type, const uint32_t value)
{
  bool in_range = true;
  if ((label_type == LABEL_TYPE_BOOL) || (label_type == LABEL_TYPE_BOOL_ARRAY))
    in_range = value <= 1;
  else if ((label_type == LABEL_TYPE_INT8) || (label_type == LABEL_TYPE_INT8_ARRAY))
    in_range = ((int32_t)value >= (int32_t)INT8_MIN) && ((int32_t)value <= (int32_t)INT8_MAX);
  else if ((label_type == LABEL_TYPE_INT16) || (label_type == LABEL_TYPE_INT16_ARRAY))
    in_range = ((int32_t)value >= (int32_t)INT16_MIN) && ((int32_t)value <= (int32_t)INT16_MAX);
  else if ((label_type == LABEL_TYPE_UINT8) || (label_type == LABEL_TYPE_UINT8_ARRAY))
    in_range = value <= (uint32_t)UINT8_MAX;
  else if ((label_type == LABEL_TYPE_UINT16) || (label_type == LABEL_TYPE_UINT16_ARRAY))
    in_range = value <= (uint32_t)UINT16_MAX;

  return in_range;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get or set label.
 *
 -----------------------------------------------------------------------------*/
static e_ERROR_ID LabelAccess(const uint16_t label, const uint8_t sublabel, uint32_t * const value, const bool read, const bool broadcast)
{
  ASSERT_STATIC(sizeof(bool) == 1);
  ASSERT_STATIC(false == 0);
  ASSERT_STATIC(true == 1);
  ASSERT_STATIC(sizeof(float) == 4);
  ASSERT_STATIC(sizeof(t_DATE) == 4);
  ASSERT_STATIC(sizeof(t_VERSION) == 4);
  ASSERT_STATIC(sizeof(LABEL_TYPE_UNKNOWN) <= 255);
  ASSERT_STATIC(sizeof(CONFIG_TYPE_UNKNOWN) <= 15);
  ASSERT_STATIC((sizeof(int) == 2) ? (sizeof(t_LABEL_DEFINITION) == 2) : true);
  ASSERT_STATIC((sizeof(int) == 2) ? (sizeof(t_LABEL_CONFIG) == 6) : true);
  ASSERT_FATAL(value != NULL);

  const t_LABEL_CONFIG * entry = NULL;
  e_ERROR_ID result = LABEL_GET_ENTRY(&entry, label, sublabel);
  if (result == ERROR_ID_SUCCESS) {

    ASSERT_FATAL(entry != NULL);
    const bool array_or_raw = LabelTypeIsArrayOrRaw(entry->label_type);
    const uint8_t bytes_per_sublabel = LABEL_GET_BYTES_PER_SUBLABEL(entry->label_type);
    const size_t config_minimum_size = array_or_raw ? (bytes_per_sublabel * ((size_t)sublabel + 1)) : bytes_per_sublabel;
    const e_CONFIG_TYPE config_type = ConfigGetType(entry->config_id);
    const size_t config_size = ConfigGetSize(entry->config_id);
    const e_MODE mode = ModeGet();

    if (read && (mode < entry->get_minimum))
      result = ERROR_ID_NOT_ALLOWED_IN_THIS_MODE;
    else if (!read && (mode < entry->set_minimum))
      result = ERROR_ID_NOT_ALLOWED_IN_THIS_MODE;
    else if (!entry->singlecast_set_allowed && !broadcast && !read)
      result = ERROR_ID_ONLY_ALLOWED_ON_BASE_ID;
    else if (read && (entry->get_minimum == MODE_ID_ILLEGAL))
      result = ERROR_ID_NOT_READABLE;
    else if (!read && (entry->set_minimum == MODE_ID_ILLEGAL))
      result = ERROR_ID_NOT_WRITEABLE;
    else if (entry->label_type == LABEL_TYPE_UNKNOWN)
      result = ERROR_ID_INTERNAL_ERROR;
    else if (array_or_raw && (entry->sublabel_id != 0))
      result = ERROR_ID_INTERNAL_ERROR;
    else if (!ConfigIsAvailable(entry->config_id))
      result = ERROR_ID_UNKNOWN_VALUE;
    else if (!LABEL_TYPE_IS_COMPATIBLE(entry->label_type, config_type))
      result = ERROR_ID_INTERNAL_ERROR;
    else if (sublabel >= LABEL_MAXIMUM_NUMBER_OF_SUBLABELS)
      result = ERROR_ID_UNKNOWN_SUBLABEL;
    else if (config_size < config_minimum_size)
      result = ERROR_ID_UNKNOWN_SUBLABEL;
    else if (!read && !LABEL_VALUE_IS_WITHIN_RANGE(entry->label_type, *value))
      result = ERROR_ID_VALUE_OUT_OF_RANGE;
    else {

      // perform read, write or read-modify-write

      uint8_t array[LABEL_MAXIMUM_NUMBER_OF_SUBLABELS * 4];

      const bool read_first = read || array_or_raw;
      if (read_first) {
        result = ConfigReadValue(entry->config_id, array, config_size);

        const bool failed_to_access_for_read_modify_write = !read && (result == ERROR_ID_NOT_READABLE);
        if (failed_to_access_for_read_modify_write)
          result = ERROR_ID_INTERNAL_ERROR;
      }

      if (result == ERROR_ID_SUCCESS) {

        const int index = array_or_raw ? (sublabel * (int)bytes_per_sublabel) : 0;

        if (read) {
          uint32_t temp = 0;
          memcpy(&temp, &array[index], bytes_per_sublabel);
          *value =
              (entry->label_type == LABEL_TYPE_INT8) ? (uint32_t)(int32_t)(int8_t)temp :
                  (entry->label_type == LABEL_TYPE_INT16) ? (uint32_t)(int32_t)(int16_t)temp :
                      temp;
        }
        else {
          memcpy(&array[index], value, bytes_per_sublabel);
          result = ConfigWriteValue(entry->config_id, array, config_size);
        }
      }
    }
  }

  return result;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Wrapper for getting label.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID LabelGetValue(const uint16_t label, const uint8_t sublabel, uint32_t * const value, const bool broadcast)
{
  return LabelAccess(label, sublabel, value, true, broadcast);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Wrapper for setting label.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID LabelSetValue(const uint16_t label, const uint8_t sublabel, const uint32_t * const value, const bool broadcast)
{
  return LabelAccess(label, sublabel, (uint32_t *)value, false, broadcast);
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
