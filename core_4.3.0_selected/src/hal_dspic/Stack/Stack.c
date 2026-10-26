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

#include "hal/IController.h"
#include "hal/IStack.h"
#include "middleware/IGeneric.h"

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

#define STACK_SAFETY_OFFSET_IN_BYTE                                            4
#define STACK_ADDRESS_WORD(address)       ((uint16_t)address & (uint16_t)0xFFFE)
#define STACK_TEST_PATTERN                                                0x5555

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

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
 *
 * There are some special things to be mentioned regarding dsPIC:
 *
 * - The stack grows from lower addresses to higher addresses.
 * - Start and size the stack are defined by the linker and can not be determined by the firmware.
 *   Only a minimum size is defined with linker flags.
 *
 -----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Return an address slightly above the current stack pointer.
 * This way to determining the stack pointer is used because reading WREG15 itself results in a compiler warning.
 *
 -----------------------------------------------------------------------------*/
uint32_t StackGetStackPointer(void)
{
  volatile int stack_variable;

  return (uint32_t)(STACK_ADDRESS_WORD(&stack_variable) + STACK_ADDRESS_WORD(STACK_SAFETY_OFFSET_IN_BYTE));
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Return the last address of the stack.
 *
 -----------------------------------------------------------------------------*/
uint32_t StackGetStackLimit(void)
{
  return (uint32_t)STACK_ADDRESS_WORD(SPLIM);
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Initializes currently unused parts of the stack with a dummy pattern.
 *
 -----------------------------------------------------------------------------*/
void StackInitializeEmpty(void)
{
  // create variable on stack
  volatile uint16_t * pointer;

  // initialize empty parts of the stack with test pattern
  pointer = (uint16_t *)(uint16_t)StackGetStackPointer();
  while (STACK_ADDRESS_WORD(pointer) <= STACK_ADDRESS_WORD(SPLIM)) {
    *pointer = (uint16_t)STACK_TEST_PATTERN;
    pointer++;
  }
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Determine whether the stack has already been initialized with the dummy pattern.
 *
 -----------------------------------------------------------------------------*/
bool StackIsInitializedEmpty(void)
{
  uint16_t * const pointer = (uint16_t *)STACK_ADDRESS_WORD(SPLIM);

  return *pointer == (uint16_t)STACK_TEST_PATTERN;
}

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Checks how many bytes have never been changed.
 *
 -----------------------------------------------------------------------------*/
uint32_t StackGetNumberOfUnusedBytes(void)
{
  // create variable on stack
  volatile uint16_t * pointer;

  // get address of the highest word within the stack that has not been overwritten yet
  pointer = (uint16_t *)STACK_ADDRESS_WORD(SPLIM);
  while ((*pointer == STACK_TEST_PATTERN) && (STACK_ADDRESS_WORD(pointer) != StackGetStackPointer()))
    pointer--;

  uint16_t number_of_bytes = STACK_ADDRESS_WORD(SPLIM) - STACK_ADDRESS_WORD(pointer);

  return (uint32_t)number_of_bytes;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
