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

#define CONTROLLER_DSPIC33EP128GS804_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP508_SUPPORTED
#define CONTROLLER_DSPIC33CK256MP506_SUPPORTED

#include "hal/ICallApplication.h"
#include "hal/IController.h"

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

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Execute a partition swap and branch to the reset vector on the new partition.
 *
 -----------------------------------------------------------------------------*/
void CallApplicationNow(void)
{
  __asm__ volatile (
      "clr     [w15++]     \n" // push a 0x000000 return address and 0x00 SR byte onto the stack (used by retfie later on)
      "clr     [w15++]     \n"
      "mov     #0x0055, w0 \n" // unlock NVM to allow bootswp
      "mov     w0, NVMKEY  \n"
      "sl      w0, #1, w0  \n"
      "mov     w0, NVMKEY  \n"
      ".pword  0xFE2000    \n" // do the partition swap while executing the very next instruction
      "retfie              \n" // modify the program counter with a single 24 bit instruction to branch to address 0 on the other partition
      :
      :
      : "memory"
  );

  // never reaching this point
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
