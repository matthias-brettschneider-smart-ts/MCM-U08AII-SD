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

#include "middleware/IAssert.h"
#include "middleware/IConfig.h"
#include "middleware/IConfigInternal.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"

#ifndef CONFIG_NUMBER_OF_REGISTRATIONS
#error "error compile flags - CONFIG_NUMBER_OF_REGISTRATIONS undefined"
#elif ((CONFIG_NUMBER_OF_REGISTRATIONS <= 0) || (CONFIG_NUMBER_OF_REGISTRATIONS > 25))
#error "error compile flags - CONFIG_NUMBER_OF_REGISTRATIONS invalid"
#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef struct {
  e_CONFIG_TYPE config_type : 8;
  uint8_t size;
} t_CONFIG_SIZE;

typedef struct {
  t_CONFIG * configs;
  t_CONFIG_CALLBACK * callback;
} t_CONFIG_REGISTRATION;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static const t_CONFIG_SIZE config_sizes[] = {
    { .config_type = CONFIG_TYPE_BOOL,            .size =  1 },
    { .config_type = CONFIG_TYPE_DATE,            .size =  4 },
    { .config_type = CONFIG_TYPE_FLOAT32,         .size =  4 },
    { .config_type = CONFIG_TYPE_INT8,            .size =  1 },
    { .config_type = CONFIG_TYPE_INT16,           .size =  2 },
    { .config_type = CONFIG_TYPE_INT32,           .size =  4 },
    { .config_type = CONFIG_TYPE_UINT8,           .size =  1 },
    { .config_type = CONFIG_TYPE_UINT16,          .size =  2 },
    { .config_type = CONFIG_TYPE_UINT32,          .size =  4 },
    { .config_type = CONFIG_TYPE_VERSION,         .size =  4 },
    { .config_type = CONFIG_TYPE_RAW,             .size =  1 },
    { .config_type = CONFIG_TYPE_UNKNOWN,         .size =  0 }
};

static t_CONFIG_REGISTRATION config_registration[CONFIG_NUMBER_OF_REGISTRATIONS];

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

static t_CONFIG * ConfigGetEntry(const e_CONFIG_ID config_id, t_CONFIG_CALLBACK ** callback);
static size_t ConfigGetSizeInternal(const e_CONFIG_TYPE config_type, const size_t elements);
static bool ConfigValueIsOutOfRange(const e_CONFIG_TYPE config_type, const size_t elements, const void * const value);
static e_ERROR_ID ConfigAccessValue(const e_CONFIG_ID config_id, void * const value, const bool read, const size_t size);

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Register configuration values and optional callback function.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID ConfigRegister(const t_CONFIG config[], t_CONFIG_CALLBACK * const callback)
{
  ASSERT_STATIC(sizeof(bool) == 1);
  ASSERT_STATIC((false == 0) && (true == 1)); // false & true may be used to configure single bits
  ASSERT_STATIC(sizeof(float) == 4);
  ASSERT_STATIC(sizeof(t_DATE) == 4);
  ASSERT_STATIC(sizeof(t_VERSION) == 4);
  ASSERT_STATIC((sizeof(int) == 2) ? (sizeof(t_CONFIG) == 6) : true);
  ASSERT_STATIC(CONFIG_ID_INVALID <= 255);
  ASSERT_STATIC(CONFIG_TYPE_UNKNOWN <= 255);

  uint16_t registration = 0;
  bool already_registered = false;

  // search for empty index or already stored pointer
  while ((registration < CONFIG_NUMBER_OF_REGISTRATIONS) && (config_registration[registration].configs != NULL)) {
    if (config_registration[registration].configs == config)
      already_registered = true;
    registration++;
  }

  // register entry if not done yet and array is not full
  const bool registration_failed = (registration >= CONFIG_NUMBER_OF_REGISTRATIONS) || already_registered;
  if (registration_failed)
    ErrorAddEntry(ERROR_ID_CONFIG_REGISTRATION, 0);
  else {
    config_registration[registration].configs = (t_CONFIG *)config;
    config_registration[registration].callback = callback;
  }

  return registration_failed ? ERROR_ID_FAILED : ERROR_ID_SUCCESS;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
static t_CONFIG * ConfigGetEntry(const e_CONFIG_ID config_id, t_CONFIG_CALLBACK ** callback)
{
  t_CONFIG * config = NULL;

  int registration = 0;
  while ((registration < CONFIG_NUMBER_OF_REGISTRATIONS) && (config_registration[registration].configs != NULL) && (config == NULL)) {
    int entry = 0;
    while ((config_registration[registration].configs[entry].config_id != CONFIG_ID_INVALID) && (config == NULL)) {
      if (config_registration[registration].configs[entry].config_id == config_id) {
        config = &config_registration[registration].configs[entry];
        if (callback != NULL)
          *callback = config_registration[registration].callback;
      }
      entry++;
    }
    registration++;
  }

  return config;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
bool ConfigIsAvailable(const e_CONFIG_ID config_id)
{
  return ConfigGetEntry(config_id, NULL) != NULL;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
e_CONFIG_TYPE ConfigGetType(const e_CONFIG_ID config_id)
{
  const t_CONFIG * const config = ConfigGetEntry(config_id, NULL);
  const e_CONFIG_TYPE type = (config == NULL) ? CONFIG_TYPE_UNKNOWN : config->config_type;

  return type;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
static size_t ConfigGetSizeInternal(const e_CONFIG_TYPE config_type, const size_t elements)
{
  size_t size = 0;
  for (size_t entry = 0; entry < (sizeof(config_sizes) / sizeof(t_CONFIG_SIZE)); entry++)
    if (config_sizes[entry].config_type == config_type)
      size = (size_t)config_sizes[entry].size * elements;

  return size;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
size_t ConfigGetSize(const e_CONFIG_ID config_id)
{
  const t_CONFIG * const config = ConfigGetEntry(config_id, NULL);
  const size_t size = (config == NULL) ? 0 : ConfigGetSizeInternal(config->config_type, config->elements);

  return size;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
static bool ConfigValueIsOutOfRange(const e_CONFIG_TYPE config_type, const size_t elements, const void * const value)
{
  ASSERT_FATAL(value != NULL);

  bool out_of_range = false;

  if (config_type == CONFIG_TYPE_BOOL) {
    const uint8_t * const array = (uint8_t *)value;
    for (size_t index = 0; index < elements; index++)
      if (array[index] > 1)
        out_of_range = true;
  }

  if (config_type == CONFIG_TYPE_FLOAT32) {
    const uint32_t * const array = (uint32_t *)value;
    for (size_t index = 0; index < elements; index++)
      out_of_range |= (array[index] & 0x7F800000) == 0x7F800000;
  }

  if (config_type == CONFIG_TYPE_DATE) {
    const t_DATE * const date = (t_DATE *)value;
    out_of_range = (date->day == 0) || (date->day > 31) || (date->month == 0) || (date->month > 12) || (date->year < 2000);
  }

  return out_of_range;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Get or set specific configuration value.
 *
 -----------------------------------------------------------------------------*/
static e_ERROR_ID ConfigAccessValue(const e_CONFIG_ID config_id, void * const value, const bool read, const size_t size)
{
  ASSERT_FATAL(value != NULL);

  t_CONFIG_CALLBACK * callback = NULL;
  t_CONFIG * config = ConfigGetEntry(config_id, &callback);

  e_ERROR_ID result = ERROR_ID_SUCCESS;
  if (config == NULL)
    result = ERROR_ID_UNKNOWN_VALUE;
  else if (read && !config->readable)
    result = ERROR_ID_NOT_READABLE;
  else if (!read && !config->writeable)
    result = ERROR_ID_NOT_WRITEABLE;
  else if (ConfigGetSizeInternal(config->config_type, config->elements) != size)
    result = ERROR_ID_INTERNAL_ERROR;
  else {

    if (!read)
      if (ConfigValueIsOutOfRange(config->config_type, config->elements, value))
        result = ERROR_ID_VALUE_OUT_OF_RANGE;

    // first execution of callback (if possible)
    if ((result == ERROR_ID_SUCCESS) && (callback != NULL))
      result = (callback)(CONFIG_CALLBACK_TYPE_PRE, config_id, read, value);

    // copy data (if possible)
    if ((result == ERROR_ID_SUCCESS) && (config->data_pointer != NULL)) {
      if (read)
        memcpy(value, config->data_pointer, ConfigGetSizeInternal(config->config_type, config->elements));
      else
        memcpy(config->data_pointer, value, ConfigGetSizeInternal(config->config_type, config->elements));
    }

    // second execution of callback (if possible)
    if ((result == ERROR_ID_SUCCESS) && (callback != NULL))
      result = (callback)(CONFIG_CALLBACK_TYPE_POST, config_id, read, value);
  }

  return result;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Read specific configuration value.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID ConfigReadValue(const e_CONFIG_ID config_id, void * const value, const size_t size)
{
  return ConfigAccessValue(config_id, value, true, size);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Write specific configuration value.
 *
 -----------------------------------------------------------------------------*/
e_ERROR_ID ConfigWriteValue(const e_CONFIG_ID config_id, const void * const value, const size_t size)
{
  return ConfigAccessValue(config_id, (void *)value, false, size);
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
