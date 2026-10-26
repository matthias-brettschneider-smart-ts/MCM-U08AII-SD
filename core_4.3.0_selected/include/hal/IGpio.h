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

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

#ifdef __cplusplus

enum e_GPIO_TYPE : unsigned int;
enum e_GPIO_STATE : unsigned int;
enum e_GPIO_PIN : unsigned int;
enum e_GPIO_PERIPHERY : unsigned int;

#else

typedef enum e_GPIO_TYPE e_GPIO_TYPE;
typedef enum e_GPIO_STATE e_GPIO_STATE;
typedef enum e_GPIO_PIN e_GPIO_PIN;
typedef enum e_GPIO_PERIPHERY e_GPIO_PERIPHERY;

#endif

/*============================================================================*/
/* I N L I N E   F U N C T I O N S                                            */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   E X T E R N                                    */
/*============================================================================*/

/*============================================================================*/
/* D E C L A R A T I O N   -   P R O T O T Y P E S                            */
/*============================================================================*/

COMPILER_INLINE void GPIO_INIT(const e_GPIO_PIN gpio, const e_GPIO_TYPE type, const e_GPIO_PERIPHERY periphery, const e_GPIO_STATE state);
COMPILER_INLINE e_GPIO_STATE GPIO_GET(const e_GPIO_PIN gpio);
COMPILER_INLINE void GPIO_SET(const e_GPIO_PIN gpio, const e_GPIO_STATE state);

/*============================================================================*/
/*  I M P L E M E N T A T I O N                                               */
/*============================================================================*/

#include "GpioImpl.h"

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
