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

#include "middleware/IDefault.h"
#include "middleware/IDefaultInternal.h"
#include "middleware/IError.h"
#include "middleware/IGeneric.h"

#ifndef DEFAULT_NUMBER_OF_REGISTRATIONS
#error "error compile flags - DEFAULT_NUMBER_OF_REGISTRATIONS undefined"
#elif ((DEFAULT_NUMBER_OF_REGISTRATIONS <= 0) || (DEFAULT_NUMBER_OF_REGISTRATIONS > 20))
#error "error compile flags - DEFAULT_NUMBER_OF_REGISTRATIONS invalid"
#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_DEFAULT_CALLBACK * default_pointers[DEFAULT_NUMBER_OF_REGISTRATIONS];

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Register callback function.
 *
 -----------------------------------------------------------------------------*/
void DefaultRegister(t_DEFAULT_CALLBACK * callback)
{
  int index = 0;
  bool already_registered = false;

  // search empty index or already stored pointer
  for (index = 0; (index < DEFAULT_NUMBER_OF_REGISTRATIONS) && (default_pointers[index] != NULL); index++)
    if (default_pointers[index] == callback)
      already_registered = true;

  const bool full = index >= DEFAULT_NUMBER_OF_REGISTRATIONS;

  if (full || already_registered)
    ErrorAddEntry(ERROR_ID_DEFAULT_REGISTRATION, 0);
  else
    default_pointers[index] = callback;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Execute all available callback functions sequentially.
 *
 -----------------------------------------------------------------------------*/
void DefaultReset(void)
{
  for (int index = 0; index < DEFAULT_NUMBER_OF_REGISTRATIONS; index++)
    if (default_pointers[index] != NULL)
      (default_pointers[index])();
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
