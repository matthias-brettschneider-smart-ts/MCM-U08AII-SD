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

#ifndef CONFIGURATION_BITS_ICD_CHANNEL
#error "error compile flags - CONFIGURATION_BITS_ICD_CHANNEL undefined"
#elif ((CONFIGURATION_BITS_ICD_CHANNEL < 1) || (CONFIGURATION_BITS_ICD_CHANNEL > 3))
#error "error compile flags - CONFIGURATION_BITS_ICD_CHANNEL invalid"
#endif

#ifndef CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C1
#error "error compile flags - CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C1 undefined"
#elif ((CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C1 != false) && (CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C1 != true))
#error "error compile flags - CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C1 invalid"
#endif

#ifndef CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C2
#error "error compile flags - CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C2 undefined"
#elif ((CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C2 != false) && (CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C2 != true))
#error "error compile flags - CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C2 invalid"
#endif

#ifndef CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C3
#error "error compile flags - CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C3 undefined"
#elif ((CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C3 != false) && (CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C3 != true))
#error "error compile flags - CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C3 invalid"
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

// FBSLIM
#pragma config BSLIM = 0x1FFF           // Boot Segment Flash Page Address Limit bits (Enter Hexadecimal value) // IME: core - dsPIC33C - configuration bits - check setting

// FSIGN

// FOSCSEL
#pragma config FNOSC = FRCPLL           // Oscillator Source Selection (Fast RC Oscillator with divide-by-N with PLL module (FRCPLL) )
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FOSC
#pragma config POSCMD = XT              // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config FCKSM = CSECME           // IME: core - dsPIC33C - configuration bits - check setting
#pragma config PLLKEN = OFF             // IME: core - dsPIC33C - configuration bits - check setting
#pragma config XTCFG = G1               // XT Config (8-16 MHz crystals)
#pragma config XTBST = ENABLE           // XT Boost (Boost the kick-start)

// FWDT
// RWDTPS = No Setting
#pragma config RCLKSEL = LPRC           // Watchdog Timer Clock Select bits (Always use LPRC)
#pragma config WINDIS = ON              // Watchdog Timer Window Enable bit (Watchdog Timer operates in Non-Window mode)
#pragma config WDTWIN = WIN25           // Watchdog Timer Window Select bits (WDT Window is 25% of WDT period)
// SWDTPS = No Setting
#pragma config FWDTEN = ON_SW           // Watchdog Timer Enable bit (WDT controlled via SW, use WDTCON.ON bit)

// FPOR
#pragma config BISTDIS = DISABLED       // Memory BIST Feature Disable (mBIST on reset feature disabled)

// FICD
#if (CONFIGURATION_BITS_ICD_CHANNEL == 1)
#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGC1 and PGD1)
#elif (CONFIGURATION_BITS_ICD_CHANNEL == 2)
#pragma config ICS = PGD2               // ICD Communication Channel Select bits (Communicate on PGC2 and PGD2)
#elif (CONFIGURATION_BITS_ICD_CHANNEL == 3)
#pragma config ICS = PGD3               // ICD Communication Channel Select bits (Communicate on PGC3 and PGD3)
#endif
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)
#pragma config NOBTSWP = ENABLED        // BOOTSWP instruction disable bit (BOOTSWP instruction is enabled) // IME: core - dsPIC33C - configuration bits - enable BOOTSWP for flashloader only?

// FDMTIVTL
#pragma config DMTIVTL = 0xFFFF         // Dead Man Timer Interval low word (Enter Hexadecimal value)

// FDMTIVTH
#pragma config DMTIVTH = 0xFFFF         // Dead Man Timer Interval high word (Enter Hexadecimal value)

// FDMTCNTL
#pragma config DMTCNTL = 0xFFFF         // Lower 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF) (Enter Hexadecimal value)

// FDMTCNTH
#pragma config DMTCNTH = 0xFFFF         // Upper 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF) (Enter Hexadecimal value)

// FDMT
#pragma config DMTDIS = OFF             // Dead Man Timer Disable bit (Dead Man Timer is Disabled and can be enabled by software)

// FDEVOPT
#if (CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C1)
#pragma config ALTI2C1 = ON             // Alternate I2C1 Pin bit (I2C1 mapped to ASDA1/ASCL1 pins)
#else
#pragma config ALTI2C1 = OFF            // Alternate I2C1 Pin bit (I2C1 mapped to SDA1/SCL1 pins)
#endif
#if (CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C2)
#pragma config ALTI2C2 = ON             // Alternate I2C2 Pin bit (I2C2 mapped to ASDA2/ASCL2 pins)
#else
#pragma config ALTI2C2 = OFF            // Alternate I2C2 Pin bit (I2C2 mapped to SDA2/SCL2 pins)
#endif
#if (CONFIGURATION_BITS_USE_ALTERNATE_PINS_FOR_I2C3)
#pragma config ALTI2C3 = ON             // Alternate I2C3 Pin bit (I2C3 mapped to ASDA3/ASCL3 pins)
#else
#pragma config ALTI2C3 = OFF            // Alternate I2C3 Pin bit (I2C3 mapped to SDA3/SCL3 pins)
#endif
#pragma config SMBEN = SMBUS            // SM Bus Enable (SMBus input threshold is enabled)
#pragma config SPI2PIN = PPS            // SPI2 Pin Select bit (SPI2 uses I/O remap (PPS) pins)

// FALTREG
#pragma config CTXT1 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits (Not Assigned)
#pragma config CTXT2 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 2 bits (Not Assigned)
#pragma config CTXT3 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 3 bits (Not Assigned)
#pragma config CTXT4 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 4 bits (Not Assigned)

// FBTSEQ
#if (CONFIGURATION_BITS_FLASHLOADER)
#pragma config BSEQ = 0x200             // Relative value defining which partition will be active after device Reset; the partition containing a lower boot number will be active (Enter Hexadecimal value)
#pragma config IBSEQ = 0xDFF            // The one's complement of BSEQ; must be calculated by the user and written during device programming. (Enter Hexadecimal value)
#else
#pragma config BSEQ = 0x500             // Relative value defining which partition will be active after device Reset; the partition containing a lower boot number will be active (Enter Hexadecimal value)
#pragma config IBSEQ = 0xAFF            // The one's complement of BSEQ; must be calculated by the user and written during device programming. (Enter Hexadecimal value)
#endif

// FBOOT (will NOT be reprogrammed at firmware updates)
#pragma config BTMODE = DUAL            // Device Boot Mode Configuration (Device is in Dual Partition mode)

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
