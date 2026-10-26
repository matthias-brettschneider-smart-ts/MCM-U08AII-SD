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

#define CONTROLLER_DSPIC33FJ256GP710_SUPPORTED
#define CONTROLLER_DSPIC33EP128GS804_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP508_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP506_SUPPORTED

#include "hal/ICompiler.h"
#include "hal/IController.h"
#include "middleware/IGeneric.h"
#include "os/ILed.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

COMPILER_INLINE void TRAP_HANDLE_COMMON(void);

// handler must be public to be found by the linker but do not have to be visible/available via interface
COMPILER_ISR void _OscillatorFail(void);
COMPILER_ISR void _AddressError(void);
COMPILER_ISR void _StackError(void);
COMPILER_ISR void _MathError(void);
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
COMPILER_ISR void _DMACError(void);
COMPILER_ISR void _ReservedTrap0(void);
COMPILER_ISR void _ReservedTrap6(void);
COMPILER_ISR void _ReservedTrap7(void);
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
COMPILER_ISR void _HardTrapError(void);
COMPILER_ISR void _SoftTrapError(void);
COMPILER_ISR void _DMACError(void);
COMPILER_ISR void _ReservedTrap7(void);
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
COMPILER_ISR void _HardTrapError(void);
COMPILER_ISR void _SoftTrapError(void);
COMPILER_ISR void _ReservedTrap5(void);
COMPILER_ISR void _ReservedTrap7(void);
#endif

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_INLINE void TRAP_HANDLE_COMMON(void)
{
  LED_SIGNAL_PERMANENT_ERROR();
  while (true);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 *
 *
 -----------------------------------------------------------------------------*/
COMPILER_ISR void _OscillatorFail(void) { TRAP_HANDLE_COMMON(); }
COMPILER_ISR void _AddressError(void)   { TRAP_HANDLE_COMMON(); }
COMPILER_ISR void _StackError(void)     { TRAP_HANDLE_COMMON(); }
COMPILER_ISR void _MathError(void)      { TRAP_HANDLE_COMMON(); }
#if (CONTROLLER_TYPE == CONTROLLER_DSPIC33FJ256GP710)
COMPILER_ISR void _DMACError(void)      { TRAP_HANDLE_COMMON(); }
COMPILER_ISR void _ReservedTrap0(void)  { TRAP_HANDLE_COMMON(); }
COMPILER_ISR void _ReservedTrap6(void)  { TRAP_HANDLE_COMMON(); }
COMPILER_ISR void _ReservedTrap7(void)  { TRAP_HANDLE_COMMON(); }
#elif (CONTROLLER_TYPE == CONTROLLER_DSPIC33EP128GS804)
COMPILER_ISR void _HardTrapError(void)  { TRAP_HANDLE_COMMON(); }
COMPILER_ISR void _SoftTrapError(void)  { TRAP_HANDLE_COMMON(); }
COMPILER_ISR void _DMACError(void)      { TRAP_HANDLE_COMMON(); }
COMPILER_ISR void _ReservedTrap7(void)  { TRAP_HANDLE_COMMON(); }
#elif ((CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP508) || (CONTROLLER_TYPE == CONTROLLER_DSPIC33CK256MP506))
COMPILER_ISR void _HardTrapError(void)  { TRAP_HANDLE_COMMON(); }
COMPILER_ISR void _SoftTrapError(void)  { TRAP_HANDLE_COMMON(); }
COMPILER_ISR void _ReservedTrap5(void)  { TRAP_HANDLE_COMMON(); }
COMPILER_ISR void _ReservedTrap7(void)  { TRAP_HANDLE_COMMON(); }
#endif

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
