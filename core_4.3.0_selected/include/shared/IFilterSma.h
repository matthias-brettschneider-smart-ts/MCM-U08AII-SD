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

#include "hal/ICompiler.h"
#include "middleware/IGeneric.h"

#ifndef FILTER_SMA_DATA_TYPE_SIGN
#error "error compile flags - FILTER_SMA_DATA_TYPE_SIGN undefined"
#elif ((FILTER_SMA_DATA_TYPE_SIGN != false) && (FILTER_SMA_DATA_TYPE_SIGN != true))
#error "error compile flags - FILTER_SMA_DATA_TYPE_SIGN invalid"
#endif

#ifndef FILTER_SMA_DATA_TYPE_WIDTH_IN_BITS
#error "error compile flags - FILTER_SMA_DATA_TYPE_WIDTH_IN_BITS undefined"
#elif (FILTER_SMA_DATA_TYPE_WIDTH_IN_BITS != 16)
#error "error compile flags - FILTER_SMA_DATA_TYPE_WIDTH_IN_BITS invalid"
#endif

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

#if (FILTER_SMA_DATA_TYPE_SIGN && (FILTER_SMA_DATA_TYPE_WIDTH_IN_BITS == 16))
typedef int16_t t_FILTER_SMA_DATA_TYPE;
#elif (!FILTER_SMA_DATA_TYPE_SIGN && (FILTER_SMA_DATA_TYPE_WIDTH_IN_BITS == 16))
typedef uint16_t t_FILTER_SMA_DATA_TYPE;
#endif

typedef struct FILTER_SMA_INTERNAL t_FILTER_SMA;

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

COMPILER_INLINE void FILTER_SMA_SET_FORCE_RESET(t_FILTER_SMA * const self);
COMPILER_INLINE void FILTER_SMA_SET_LENGTH(t_FILTER_SMA * const self, int length);
COMPILER_INLINE void FILTER_SMA_UPDATE(t_FILTER_SMA * const self, const t_FILTER_SMA_DATA_TYPE sample);
COMPILER_INLINE t_FILTER_SMA_DATA_TYPE FILTER_SMA_GET_RESULT(t_FILTER_SMA * const self);

/*============================================================================*/
/*  I M P L E M E N T A T I O N                                               */
/*============================================================================*/

#include "FilterSmaImpl.h"

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
