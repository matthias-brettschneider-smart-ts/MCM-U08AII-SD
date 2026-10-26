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

#include "hal/IController.h"

#ifndef COMPILER_SHARED_RAM_ADDRESS
#error "error compile flags - COMPILER_SHARED_RAM_ADDRESS undefined"
#endif

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

#define COMPILER_IMPL_INLINE                 static inline __attribute__((always_inline))
#define COMPILER_IMPL_DO_NOT_OPTIMIZE        __attribute__((optimize("-O0")))
#define COMPILER_IMPL_ISR                    __attribute__((interrupt, no_auto_psv))
#define COMPILER_IMPL_SHARED_RAM             __attribute__((persistent, address(COMPILER_SHARED_RAM_ADDRESS)))
#define COMPILER_IMPL_PACKED                 __attribute__((packed))
#define COMPILER_IMPL_ALIGNED_2_BYTE         __attribute__((aligned(2)))
#define COMPILER_IMPL_ALIGNED_4_BYTE         __attribute__((aligned(4)))

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
