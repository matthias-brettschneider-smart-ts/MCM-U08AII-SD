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

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

// can not be defined as an enumeration because they are used for conditional compilation
#define CONTROLLER_NOT_SUPPORTED                                               0
#define CONTROLLER_PC                                                          1
#define CONTROLLER_DSPIC33FJ256GP710                                           2
#define CONTROLLER_DSPIC33EP128GS804                                           3
#define CONTROLLER_DSPIC33CK256MP508                                           4
#define CONTROLLER_DSPIC33CK256MP506                                           5

#if !defined(CONTROLLER_TYPE)
#error "error compile flags - CONTROLLER_TYPE undefined"
#elif ((CONTROLLER_TYPE == CONTROLLER_PC) && !defined(CONTROLLER_PC_SUPPORTED))
#error "error compile flags - PC not supported"
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710) && !defined(CONTROLLER_DSPIC33FJ256GP710_SUPPORTED))
#error "error compile flags - dsPIC33FJ256GP710 not supported"
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) && !defined(CONTROLLER_DSPIC33EP128GS804_SUPPORTED))
#error "error compile flags - dsPIC33EP128GS804 not supported"
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) && !defined(CONTROLLER_DSPIC33CK256MP508_SUPPORTED))
#error "error compile flags - dsPIC33CK256MP508 not supported"
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506) && !defined(CONTROLLER_DSPIC33CK256MP506_SUPPORTED))
#error "error compile flags - dsPIC33CK256MP506 not supported"
#endif

#undef CONTROLLER_PC_SUPPORTED
#undef CONTROLLER_DSPIC33FJ256GP710_SUPPORTED
#undef CONTROLLER_DSPIC33EP128GS804_SUPPORTED
#undef CONTROLLER_DSPIC33CK256MP508_SUPPORTED
#undef CONTROLLER_DSPIC33CK256MP506_SUPPORTED

#if ((CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
#include <xc.h>
#include <dsp.h>
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

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
