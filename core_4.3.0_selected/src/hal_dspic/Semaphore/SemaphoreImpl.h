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

#define CONTROLLER_DSPIC33FJ256GP710_SUPPORTED
#define CONTROLLER_DSPIC33EP128GS804_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP508_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP506_SUPPORTED

#include "hal/ICompiler.h"
#include "hal/IController.h"
#include "middleware/IAssert.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE COMPILER_DO_NOT_OPTIMIZE void SEMAPHORE_ATOMIC_INCREMENT(t_SEMAPHORE_MUTEX * const mutex)
{
  t_SEMAPHORE_MUTEX * mutex_non_const = mutex;

  __asm__ volatile ("inc [%[value]], [%[value]]" : [value] "+r" (mutex_non_const) : : "memory");
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE COMPILER_DO_NOT_OPTIMIZE void SEMAPHORE_ATOMIC_DECREMENT(t_SEMAPHORE_MUTEX * const mutex)
{
  t_SEMAPHORE_MUTEX * mutex_non_const = mutex;

  __asm__ volatile ("dec [%[value]], [%[value]]" : [value] "+r" (mutex_non_const) : : "memory");
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Try to get mutex if it is free.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE bool SEMAPHORE_TRY_TO_LOCK_MUTEX(t_SEMAPHORE_MUTEX * const mutex)
{
  ASSERT_FATAL(mutex != NULL);

  bool got_mutex = false;

  SEMAPHORE_ATOMIC_INCREMENT(mutex);
  if (*mutex == 1)
    got_mutex = true;
  else
    SEMAPHORE_ATOMIC_DECREMENT(mutex);

  return got_mutex;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Release mutex.
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void SEMAPHORE_RELEASE_MUTEX(t_SEMAPHORE_MUTEX * const mutex)
{
  ASSERT_FATAL(mutex != NULL);

  *mutex = 0;
}

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
