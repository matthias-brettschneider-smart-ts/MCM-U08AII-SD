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

#include "hal/IController.h"
#include "middleware/IGeneric.h"

#ifndef CONFIGURATION_BITS_FLASHLOADER
#error "error compile flags - CONFIGURATION_BITS_FLASHLOADER undefined"
#elif ((CONFIGURATION_BITS_FLASHLOADER != false) && (CONFIGURATION_BITS_FLASHLOADER != true))
#error "error compile flags - CONFIGURATION_BITS_FLASHLOADER invalid"
#endif

#ifndef CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C1
#error "error compile flags - CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C1 undefined"
#elif ((CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C1 != false) && (CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C1 != true))
#error "error compile flags - CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C1 invalid"
#endif

#if (defined(ON) || defined(OFF))
#error "ON and OFF must not be defined in project to prevent wrong setting of configuration bits!"
#endif

/*============================================================================*/
/* D E F I N E S  /  T Y P E D E F S                                          */
/*============================================================================*/

 // FSEC
#pragma config BWRP = OFF               // Boot Segment Write-Protect bit (Boot Segment may be written)
#pragma config BSS = DISABLED           // Boot Segment Code-Protect Level bits (No Protection (other than BWRP))
#pragma config BSEN = OFF               // Boot Segment Control bit (No Boot Segment)
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GSS = DISABLED           // General Segment Code-Protect Level bits (No Protection (other than GWRP))
#pragma config CWRP = OFF               // Configuration Segment Write-Protect bit (Configuration Segment may be written)
#pragma config CSS = DISABLED           // Configuration Segment Code-Protect Level bits (No Protection (other than CWRP))
#pragma config AIVTDIS = OFF            // Alternate Interrupt Vector Table bit (Disabled AIVT)

// FSIGN
// MPLAB does not generate code for this instruction
// see linker file for setting the "reserved bit" properly

// FBSLIM
#pragma config BSLIM = 0x1FFF           // Boot Segment Flash Page Address Limit bits (Enter Hexadecimal value)

// FOSCSEL
#pragma config FNOSC = FRCPLL           // Oscillator Source Selection (Fast RC Oscillator with divide-by-N with PLL module (FRCPLL) )
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FOSC
#pragma config POSCMD = XT              // Primary Oscillator Source (XT Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = OFF            // Peripheral pin select configuration bit (Allow multiple reconfigurations)
#pragma config FCKSM = CSECMD           // Clock Switching Mode bits (Clock switching is enabled, Fail-safe Clock Monitor is disabled)
#pragma config PLLKEN = ON              // PLL Lock Enable Bit (Clock switch to PLL source will wait until the PLL lock signal is valid)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config WDTEN = OFF              // Watchdog Timer Enable bits (WDT and SWDTEN disabled)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config WDTWIN = WIN25           // Watchdog Timer Window Select bits (WDT Window is 25% of WDT period)

// FICD
#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)
#pragma config BTSWP = ON               // BOOTSWP Instruction Enable/Disable bit (BOOTSWP instruction is enabled)

// FDEVOPT
#pragma config PWMLOCK = ON             // PWMx Lock Enable bit (Certain PWM registers may only be written after key sequency)
#if (CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C1)
#pragma config ALTI2C1 = ON             // Alternate I2C1 Pin bit (I2C1 mapped to ASDA1/ASCL1 pins)
#else
#pragma config ALTI2C1 = OFF            // Alternate I2C1 Pin bit (I2C1 mapped to SDA1/SCL1 pins)
#endif
#pragma config ALTI2C2 = OFF            // Alternate I2C2 Pin bit (I2C2 mapped to SDA2/SCL2 pins)
#pragma config DBCC = OFF               // DACx Output Cross Connection bit (No Cross Connection between DAC outputs)

// FALTREG
#pragma config CTXT1 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits (Not Assigned)
#pragma config CTXT2 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 2 bits (Not Assigned)
#pragma config CTXT3 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 2 bits (Not Assigned)
#pragma config CTXT4 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 2 bits (Not Assigned)

// FBTSEQ
#if (CONFIGURATION_BITS_FLASHLOADER)
#pragma config BSEQ = 0x200             // Relative value defining which partition will be active after device Reset; the partition containing a lower boot number will be active (Enter Hexadecimal value)
#pragma config IBSEQ = 0xDFF            // The one's complement of BSEQ; must be calculated by the user and written during device programming. (Enter Hexadecimal value)
#else
#pragma config BSEQ = 0x500             // Relative value defining which partition will be active after device Reset; the partition containing a lower boot number will be active (Enter Hexadecimal value)
#pragma config IBSEQ = 0xAFF            // The one's complement of BSEQ; must be calculated by the user and written during device programming. (Enter Hexadecimal value)
#endif

// FBOOT (will NOT be reprogrammed at firmware updates)
#pragma config BTMODE = DUAL            // Boot Mode Configuration Bits (Device is in Dual Partition mode)

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
