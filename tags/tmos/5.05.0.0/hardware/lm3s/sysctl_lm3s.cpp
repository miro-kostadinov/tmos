/**************************************************************************//**
 * @ingroup	 lm3s_sysctl
 * @{
 * @file     sysctl_lm3s.cpp
 * @brief    SYSCTL class methods
 * @version  V3.00
 * @date     15. March 2010
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/
#include <tmos.h>
#include "fam_cpp.h"

/**
 * SysCtlPeripheralValid
 * @param ulPeripheral
 * @return
 */
bool SysCtlPeripheralValid(unsigned long ulPeripheral)
{
    return((ulPeripheral == ID_PERIPH_ADC0) ||
           (ulPeripheral == ID_PERIPH_ADC1) ||
           (ulPeripheral == ID_PERIPH_CAN0) ||
           (ulPeripheral == ID_PERIPH_CAN1) ||
           (ulPeripheral == ID_PERIPH_CAN2) ||
           (ulPeripheral == ID_PERIPH_COMP0) ||
           (ulPeripheral == ID_PERIPH_COMP1) ||
           (ulPeripheral == ID_PERIPH_COMP2) ||
           (ulPeripheral == ID_PERIPH_EPI0) ||
           (ulPeripheral == ID_PERIPH_ETH) ||
           (ulPeripheral == ID_PERIPH_GPIOA) ||
           (ulPeripheral == ID_PERIPH_GPIOB) ||
           (ulPeripheral == ID_PERIPH_GPIOC) ||
           (ulPeripheral == ID_PERIPH_GPIOD) ||
           (ulPeripheral == ID_PERIPH_GPIOE) ||
           (ulPeripheral == ID_PERIPH_GPIOF) ||
           (ulPeripheral == ID_PERIPH_GPIOG) ||
           (ulPeripheral == ID_PERIPH_GPIOH) ||
           (ulPeripheral == ID_PERIPH_GPIOJ) ||
           (ulPeripheral == ID_PERIPH_HIBERNATE) ||
           (ulPeripheral == ID_PERIPH_I2C0) ||
           (ulPeripheral == ID_PERIPH_I2C1) ||
           (ulPeripheral == ID_PERIPH_I2S0) ||
           (ulPeripheral == ID_PERIPH_IEEE1588) ||
           (ulPeripheral == ID_PERIPH_MPU) ||
           (ulPeripheral == ID_PERIPH_PLL) ||
           (ulPeripheral == ID_PERIPH_PWM) ||
           (ulPeripheral == ID_PERIPH_QEI0) ||
           (ulPeripheral == ID_PERIPH_QEI1) ||
           (ulPeripheral == ID_PERIPH_SSI0) ||
           (ulPeripheral == ID_PERIPH_SSI1) ||
           (ulPeripheral == ID_PERIPH_TEMP) ||
           (ulPeripheral == ID_PERIPH_TIMER0) ||
           (ulPeripheral == ID_PERIPH_TIMER1) ||
           (ulPeripheral == ID_PERIPH_TIMER2) ||
           (ulPeripheral == ID_PERIPH_TIMER3) ||
           (ulPeripheral == ID_PERIPH_UART0) ||
           (ulPeripheral == ID_PERIPH_UART1) ||
           (ulPeripheral == ID_PERIPH_UART2) ||
           (ulPeripheral == ID_PERIPH_UDMA) ||
           (ulPeripheral == ID_PERIPH_USB0) ||
           (ulPeripheral == ID_PERIPH_WDOG0) ||
           (ulPeripheral == ID_PERIPH_WDOG1));
}

//*****************************************************************************
//
// This macro extracts the array index out of the peripheral number.
//
//*****************************************************************************
//#define ID_PERIPH_INDEX(a)  (((a) >> 28) & 0xf)

//*****************************************************************************
//
// This macro constructs the peripheral bit mask from the peripheral number.
//
//*****************************************************************************
//#define ID_PERIPH_MASK(a)   (((a) & 0xffff) << (((a) & 0x001f0000) >> 16))


/**
 * doc!
 */
static const unsigned long * g_pulDCRegs[] =
{
    (unsigned long *)SYSCTL_DC1,
    (unsigned long *)SYSCTL_DC2,
    (unsigned long *)SYSCTL_DC4,
    (unsigned long *)SYSCTL_DC1
};

/**
 * Xtals doc!
 */
static const unsigned long g_pulXtals[] =
{
    1000000,
    1843200,
    2000000,
    2457600,
    3579545,
    3686400,
    4000000,
    4096000,
    4915200,
    5000000,
    5120000,
    6000000,
    6144000,
    7372800,
    8000000,
    8192000,
    10000000,
    12000000,
    12288000,
    13560000,
    14318180,
    16000000,
    16384000
};


/**
 * @brief:	Gets the processor clock rate
 *
 * @return:	processor clock rate in Hz
 */
unsigned long SYSCTL_Type::SysCtlClockGet()
{
    unsigned long ulRCC, ulRCC2, ulPLL, ulClk;
    //
    // Read RCC and RCC2.  For Sandstorm-class devices (which do not have
    // RCC2), the RCC2 read will return 0, which indicates that RCC2 is
    // disabled (since the SYSCTL_RCC2_USERCC2 bit is clear).
    //
    ulRCC = this->RCC ;
    ulRCC2 = this->RCC2;

    //
    // Get the base clock rate.
    //
    switch((ulRCC2 & SYSCTL_RCC2_USERCC2) ?
           (ulRCC2 & SYSCTL_RCC2_OSCSRC2_M) :
           (ulRCC & SYSCTL_RCC_OSCSRC_M))
    {
        //
        // The main oscillator is the clock source.  Determine its rate from
        // the crystal setting field.
        //
        case SYSCTL_RCC_OSCSRC_MAIN:
        {
            ulClk = g_pulXtals[(ulRCC & SYSCTL_RCC_XTAL_M) >>
                               SYSCTL_RCC_XTAL_S];
            break;
        }

        //
        // The internal oscillator is the source clock.
        //
        case SYSCTL_RCC_OSCSRC_INT:
        {
            //
            // See if this is a Sandstorm-class or Fury-class device.
            //
            if(CLASS_IS_SANDSTORM)
            {
                //
                // The internal oscillator on a Sandstorm-class device is
                // 15 MHz +/- 50%.
                //
                ulClk = 15000000;
            }
            else if((CLASS_IS_FURY && REVISION_IS_A2) ||
                    (CLASS_IS_DUSTDEVIL && REVISION_IS_A0))
            {
                //
                // The internal oscillator on a rev A2 Fury-class device and a
                // rev A0 Dustdevil-class device is 12 MHz +/- 30%.
                //
                ulClk = 12000000;
            }
            else
            {
                //
                // The internal oscillator on all other devices is 16 MHz.
                //
                ulClk = 16000000;
            }
            break;
        }

        //
        // The internal oscillator divided by four is the source clock.
        //
        case SYSCTL_RCC_OSCSRC_INT4:
        {
            //
            // See if this is a Sandstorm-class or Fury-class device.
            //
            if(CLASS_IS_SANDSTORM)
            {
                //
                // The internal oscillator on a Sandstorm-class device is
                // 15 MHz +/- 50%.
                //
                ulClk = 15000000 / 4;
            }
            else if((CLASS_IS_FURY && REVISION_IS_A2) ||
                    (CLASS_IS_DUSTDEVIL && REVISION_IS_A0))
            {
                //
                // The internal oscillator on a rev A2 Fury-class device and a
                // rev A0 Dustdevil-class device is 12 MHz +/- 30%.
                //
                ulClk = 12000000 / 4;
            }
            else
            {
                //
                // The internal oscillator on a Tempest-class device is 16 MHz.
                //
                ulClk = 16000000 / 4;
            }
            break;
        }

        //
        // The internal 30 KHz oscillator is the source clock.
        //
        case SYSCTL_RCC_OSCSRC_30:
        {
            //
            // The internal 30 KHz oscillator has an accuracy of +/- 30%.
            //
            ulClk = 30000;
            break;
        }

        //
        // The 4.19 MHz clock from the hibernate module is the clock source.
        //
        case SYSCTL_RCC2_OSCSRC2_419:
        {
            ulClk = 4194304;
            break;
        }

        //
        // The 32 KHz clock from the hibernate module is the source clock.
        //
        case SYSCTL_RCC2_OSCSRC2_32:
        {
            ulClk = 32768;
            break;
        }

        //
        // An unknown setting, so return a zero clock (that is, an unknown
        // clock rate).
        //
        default:
        {
            return(0);
        }
    }

    //
    // See if the PLL is being used.
    //
    if(((ulRCC2 & SYSCTL_RCC2_USERCC2) && !(ulRCC2 & SYSCTL_RCC2_BYPASS2)) ||
       (!(ulRCC2 & SYSCTL_RCC2_USERCC2) && !(ulRCC & SYSCTL_RCC_BYPASS)))
    {
        //
        // Get the PLL configuration.
        //
        ulPLL = this->PLLCFG;

        //
        // See if this is a Sandstorm-class or Fury-class device.
        //
        if(CLASS_IS_SANDSTORM)
        {
            //
            // Compute the PLL output frequency based on its input frequency.
            // The formula for a Sandstorm-class devices is
            // "(xtal * (f + 2)) / (r + 2)".
            //
            ulClk = ((ulClk * (((ulPLL & SYSCTL_PLLCFG_F_M) >>
                                SYSCTL_PLLCFG_F_S) + 2)) /
                     (((ulPLL & SYSCTL_PLLCFG_R_M) >>
                       SYSCTL_PLLCFG_R_S) + 2));
        }
        else
        {
            //
            // Compute the PLL output frequency based on its input frequency.
            // The formula for a Fury-class device is
            // "(xtal * f) / ((r + 1) * 2)".
            //
            ulClk = ((ulClk * ((ulPLL & SYSCTL_PLLCFG_F_M) >>
                               SYSCTL_PLLCFG_F_S)) /
                     ((((ulPLL & SYSCTL_PLLCFG_R_M) >>
                        SYSCTL_PLLCFG_R_S) + 1) * 2));
        }

        //
        // See if the optional output divide by 2 is being used.
        //
        if(ulPLL & SYSCTL_PLLCFG_OD_2)
        {
            ulClk /= 2;
        }

        //
        // See if the optional output divide by 4 is being used.
        //
        if(ulPLL & SYSCTL_PLLCFG_OD_4)
        {
            ulClk /= 4;
        }

        //
        // Force the system divider to be enabled.  It is always used when
        // using the PLL, but in some cases it will not read as being enabled.
        //
        ulRCC |= SYSCTL_RCC_USESYSDIV;
    }

    //
    // See if the system divider is being used.
    //
    if(ulRCC & SYSCTL_RCC_USESYSDIV)
    {
        //
        // Adjust the clock rate by the system clock divider.
        //
        if(ulRCC2 & SYSCTL_RCC2_USERCC2)
        {
            if((ulRCC2 & SYSCTL_RCC2_DIV400) &&
               (((ulRCC2 & SYSCTL_RCC2_USERCC2) &&
                 !(ulRCC2 & SYSCTL_RCC2_BYPASS2)) ||
                (!(ulRCC2 & SYSCTL_RCC2_USERCC2) &&
                 !(ulRCC & SYSCTL_RCC_BYPASS))))

            {
                ulClk = ((ulClk * 2) / (((ulRCC2 & (SYSCTL_RCC2_SYSDIV2_M |
                                                    SYSCTL_RCC2_SYSDIV2LSB)) >>
                                         (SYSCTL_RCC2_SYSDIV2_S - 1)) + 1));
            }
            else
            {
                ulClk /= (((ulRCC2 & SYSCTL_RCC2_SYSDIV2_M) >>
                           SYSCTL_RCC2_SYSDIV2_S) + 1);
            }
        }
        else
        {
            ulClk /= (((ulRCC & SYSCTL_RCC_SYSDIV_M) >> SYSCTL_RCC_SYSDIV_S) +
                      1);
        }
    }

    //
    // Return the computed clock rate.
    //
    return(ulClk);

}

//*****************************************************************************
//
//! Gets the size of the SRAM.
//!
//! This function determines the size of the SRAM on the Stellaris device.
//!
//! \return The total number of bytes of SRAM.
//
//*****************************************************************************
unsigned long SYSCTL_Type::SysCtlSRAMSizeGet(void)
{
    //
    // Compute the size of the SRAM.
    //
    return(((this->DC0 & SYSCTL_DC0_SRAMSZ_M) >> 8) + 0x100);
}

//*****************************************************************************
//
//! Gets the size of the flash.
//!
//! This function determines the size of the flash on the Stellaris device.
//!
//! \return The total number of bytes of flash.
//
//*****************************************************************************
unsigned long SYSCTL_Type::SysCtlFlashSizeGet(void)
{
    //
    // Compute the size of the flash.
    //
    return(((this->DC0 & SYSCTL_DC0_FLASHSZ_M) << 11) + 0x800);
}

//*****************************************************************************
//
//! Determines if a peripheral is present.
//!
//! \param ulPeripheral is the peripheral in question.
//!
//! Determines if a particular peripheral is present in the device.  Each
//! member of the Stellaris family has a different peripheral set; this will
//! determine which are present on this device.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b ID_PERIPH_ADC0, \b ID_PERIPH_ADC1, \b ID_PERIPH_CAN0,
//! \b ID_PERIPH_CAN1, \b ID_PERIPH_CAN2, \b ID_PERIPH_COMP0,
//! \b ID_PERIPH_COMP1, \b ID_PERIPH_COMP2, \b ID_PERIPH_EPI0,
//! \b ID_PERIPH_ETH, \b ID_PERIPH_GPIOA, \b ID_PERIPH_GPIOB,
//! \b ID_PERIPH_GPIOC, \b ID_PERIPH_GPIOD, \b ID_PERIPH_GPIOE,
//! \b ID_PERIPH_GPIOF, \b ID_PERIPH_GPIOG, \b ID_PERIPH_GPIOH,
//! \b ID_PERIPH_GPIOJ, \b ID_PERIPH_HIBERNATE, \b ID_PERIPH_I2C0,
//! \b ID_PERIPH_I2C1, \b ID_PERIPH_I2S0, \b ID_PERIPH_IEEE1588,
//! \b ID_PERIPH_MPU, \b ID_PERIPH_PLL, \b ID_PERIPH_PWM,
//! \b ID_PERIPH_QEI0, \b ID_PERIPH_QEI1, \b ID_PERIPH_SSI0,
//! \b ID_PERIPH_SSI1, \b ID_PERIPH_TIMER0, \b ID_PERIPH_TIMER1,
//! \b ID_PERIPH_TIMER2, \b ID_PERIPH_TIMER3, \b ID_PERIPH_TEMP,
//! \b ID_PERIPH_UART0, \b ID_PERIPH_UART1, \b ID_PERIPH_UART2,
//! \b ID_PERIPH_UDMA, \b ID_PERIPH_USB0, \b ID_PERIPH_WDOG0, or
//! \b ID_PERIPH_WDOG1.
//!
//! \return Returns \b true if the specified peripheral is present and \b false
//! if it is not.
//
//*****************************************************************************

int SYSCTL_Type::SysCtlPeripheralPresent(unsigned long ulPeripheral)
{

    //
    // Read the correct DC register and determine if this peripheral exists.
    //
    if(ulPeripheral == ID_PERIPH_USB0)
    {
        //
        // USB is a special case since the DC bit is missing for USB0.
        //
        if(this->DC6 && SYSCTL_DC6_USB0_M)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }
    else if( *g_pulDCRegs[ulPeripheral>>5] & (1<<(ulPeripheral&0x1f)))
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

//*****************************************************************************
//
//! Performs a software reset of a peripheral.
//!
//! \param ulPeripheral is the peripheral to reset.
//!
//! This function performs a software reset of the specified peripheral.  An
//! individual peripheral reset signal is asserted for a brief period and then
//! deasserted, leaving the peripheral in a operating state but in its reset
//! condition.
//!
//|  NOTE: It also enables the peripheral so it is recommended when the peripheral is enabled for the first time
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b ID_PERIPH_ADC0, \b ID_PERIPH_ADC1, \b ID_PERIPH_CAN0,
//! \b ID_PERIPH_CAN1, \b ID_PERIPH_CAN2, \b ID_PERIPH_COMP0,
//! \b ID_PERIPH_COMP1, \b ID_PERIPH_COMP2, \b ID_PERIPH_EPI0,
//! \b ID_PERIPH_ETH, \b ID_PERIPH_GPIOA, \b ID_PERIPH_GPIOB,
//! \b ID_PERIPH_GPIOC, \b ID_PERIPH_GPIOD, \b ID_PERIPH_GPIOE,
//! \b ID_PERIPH_GPIOF, \b ID_PERIPH_GPIOG, \b ID_PERIPH_GPIOH,
//! \b ID_PERIPH_GPIOJ, \b ID_PERIPH_HIBERNATE, \b ID_PERIPH_I2C0,
//! \b ID_PERIPH_I2C1, \b ID_PERIPH_I2S0, \b ID_PERIPH_PWM,
//! \b ID_PERIPH_QEI0, \b ID_PERIPH_QEI1, \b ID_PERIPH_SSI0,
//! \b ID_PERIPH_SSI1, \b ID_PERIPH_TIMER0, \b ID_PERIPH_TIMER1,
//! \b ID_PERIPH_TIMER2, \b ID_PERIPH_TIMER3, \b ID_PERIPH_TEMP,
//! \b ID_PERIPH_UART0, \b ID_PERIPH_UART1, \b ID_PERIPH_UART2,
//! \b ID_PERIPH_UDMA, \b ID_PERIPH_USB0, \b ID_PERIPH_WDOG0, or
//! \b ID_PERIPH_WDOG1.
//!
//! \return None.
//
//*****************************************************************************
void SysCtlPeripheralReset(unsigned long ulPeripheral)
{
	unsigned int index;
    volatile unsigned long ulDelay;

    //
    // Check the arguments.
    //
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    index = ulPeripheral>>5;
    ulPeripheral = 1<<(ulPeripheral&0x1f);

    //
    // Enable this peripheral.
    //
    SYSCTL->RCGCRegs[index] |= ulPeripheral;//&0x1f;

    //
    // Put the peripheral into the reset state.
    //
    SYSCTL->SRCRRegs[index] |= ulPeripheral;

    //
    // Delay for a little bit.
    //
    for(ulDelay = 0; ulDelay < 16; ulDelay++)
    {
    }

    //
    // Take the peripheral out of the reset state.
    //
    SYSCTL->SRCRRegs[index] &= ~ulPeripheral;
}

//*****************************************************************************
//
//! Enables a peripheral.
//!
//! \param ulPeripheral is the peripheral to enable.
//!
//! Peripherals are enabled with this function.  At power-up, all peripherals
//! are disabled; they must be enabled in order to operate or respond to
//! register reads/writes.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b ID_PERIPH_ADC0, \b ID_PERIPH_ADC1, \b ID_PERIPH_CAN0,
//! \b ID_PERIPH_CAN1, \b ID_PERIPH_CAN2, \b ID_PERIPH_COMP0,
//! \b ID_PERIPH_COMP1, \b ID_PERIPH_COMP2, \b ID_PERIPH_EPI0,
//! \b ID_PERIPH_ETH, \b ID_PERIPH_GPIOA, \b ID_PERIPH_GPIOB,
//! \b ID_PERIPH_GPIOC, \b ID_PERIPH_GPIOD, \b ID_PERIPH_GPIOE,
//! \b ID_PERIPH_GPIOF, \b ID_PERIPH_GPIOG, \b ID_PERIPH_GPIOH,
//! \b ID_PERIPH_GPIOJ, \b ID_PERIPH_HIBERNATE, \b ID_PERIPH_I2C0,
//! \b ID_PERIPH_I2C1, \b ID_PERIPH_I2S0, \b ID_PERIPH_PWM,
//! \b ID_PERIPH_QEI0, \b ID_PERIPH_QEI1, \b ID_PERIPH_SSI0,
//! \b ID_PERIPH_SSI1, \b ID_PERIPH_TIMER0, \b ID_PERIPH_TIMER1,
//! \b ID_PERIPH_TIMER2, \b ID_PERIPH_TIMER3, \b ID_PERIPH_TEMP,
//! \b ID_PERIPH_UART0, \b ID_PERIPH_UART1, \b ID_PERIPH_UART2,
//! \b ID_PERIPH_UDMA, \b ID_PERIPH_USB0, \b ID_PERIPH_WDOG0, or
//! \b ID_PERIPH_WDOG1.
//!
//! \note It takes five clock cycles after the write to enable a peripheral
//! before the the peripheral is actually enabled.  During this time, attempts
//! to access the peripheral will result in a bus fault.  Care should be taken
//! to ensure that the peripheral is not accessed during this brief time
//! period.
//!
//! \return None.
//
//*****************************************************************************
void SysCtlPeripheralEnable(unsigned long ulPeripheral)
{
    //
    // Check the arguments.
    //
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    //
    // Enable this peripheral.
    //
    SYSCTL->RCGCRegs[(ulPeripheral>>5)] |= (1<<(ulPeripheral&0x1f));
}

//*****************************************************************************
//
//! Disables a peripheral.
//!
//! \param ulPeripheral is the peripheral to disable.
//!
//! Peripherals are disabled with this function.  Once disabled, they will not
//! operate or respond to register reads/writes.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b ID_PERIPH_ADC0, \b ID_PERIPH_ADC1, \b ID_PERIPH_CAN0,
//! \b ID_PERIPH_CAN1, \b ID_PERIPH_CAN2, \b ID_PERIPH_COMP0,
//! \b ID_PERIPH_COMP1, \b ID_PERIPH_COMP2, \b ID_PERIPH_EPI0,
//! \b ID_PERIPH_ETH, \b ID_PERIPH_GPIOA, \b ID_PERIPH_GPIOB,
//! \b ID_PERIPH_GPIOC, \b ID_PERIPH_GPIOD, \b ID_PERIPH_GPIOE,
//! \b ID_PERIPH_GPIOF, \b ID_PERIPH_GPIOG, \b ID_PERIPH_GPIOH,
//! \b ID_PERIPH_GPIOJ, \b ID_PERIPH_HIBERNATE, \b ID_PERIPH_I2C0,
//! \b ID_PERIPH_I2C1, \b ID_PERIPH_I2S0, \b ID_PERIPH_PWM,
//! \b ID_PERIPH_QEI0, \b ID_PERIPH_QEI1, \b ID_PERIPH_SSI0,
//! \b ID_PERIPH_SSI1, \b ID_PERIPH_TIMER0, \b ID_PERIPH_TIMER1,
//! \b ID_PERIPH_TIMER2, \b ID_PERIPH_TIMER3, \b ID_PERIPH_TEMP,
//! \b ID_PERIPH_UART0, \b ID_PERIPH_UART1, \b ID_PERIPH_UART2,
//! \b ID_PERIPH_UDMA, \b ID_PERIPH_USB0, \b ID_PERIPH_WDOG0, or
//! \b ID_PERIPH_WDOG1.
//!
//! \return None.
//
//*****************************************************************************
void SysCtlPeripheralDisable(unsigned long ulPeripheral)
{
    //
    // Check the arguments.
    //
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    //
    // Disable this peripheral.
    //
    SYSCTL->RCGCRegs[(ulPeripheral>>5)] &= ~(1<<(ulPeripheral&0x1f));
}

//*****************************************************************************
//
//! Enables a peripheral in sleep mode.
//!
//! \param ulPeripheral is the peripheral to enable in sleep mode.
//!
//! This function allows a peripheral to continue operating when the processor
//! goes into sleep mode.  Since the clocking configuration of the device does
//! not change, any peripheral can safely continue operating while the
//! processor is in sleep mode, and can therefore wake the processor from sleep
//! mode.
//!
//! Sleep mode clocking of peripherals must be enabled via
//! SysCtlPeripheralClockGating(); if disabled, the peripheral sleep mode
//! configuration is maintained but has no effect when sleep mode is entered.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b ID_PERIPH_ADC0, \b ID_PERIPH_ADC1, \b ID_PERIPH_CAN0,
//! \b ID_PERIPH_CAN1, \b ID_PERIPH_CAN2, \b ID_PERIPH_COMP0,
//! \b ID_PERIPH_COMP1, \b ID_PERIPH_COMP2, \b ID_PERIPH_EPI0,
//! \b ID_PERIPH_ETH, \b ID_PERIPH_GPIOA, \b ID_PERIPH_GPIOB,
//! \b ID_PERIPH_GPIOC, \b ID_PERIPH_GPIOD, \b ID_PERIPH_GPIOE,
//! \b ID_PERIPH_GPIOF, \b ID_PERIPH_GPIOG, \b ID_PERIPH_GPIOH,
//! \b ID_PERIPH_GPIOJ, \b ID_PERIPH_HIBERNATE, \b ID_PERIPH_I2C0,
//! \b ID_PERIPH_I2C1, \b ID_PERIPH_I2S0, \b ID_PERIPH_PWM,
//! \b ID_PERIPH_QEI0, \b ID_PERIPH_QEI1, \b ID_PERIPH_SSI0,
//! \b ID_PERIPH_SSI1, \b ID_PERIPH_TIMER0, \b ID_PERIPH_TIMER1,
//! \b ID_PERIPH_TIMER2, \b ID_PERIPH_TIMER3, \b ID_PERIPH_TEMP,
//! \b ID_PERIPH_UART0, \b ID_PERIPH_UART1, \b ID_PERIPH_UART2,
//! \b ID_PERIPH_UDMA, \b ID_PERIPH_USB0, \b ID_PERIPH_WDOG0, or
//! \b ID_PERIPH_WDOG1.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlPeripheralSleepEnable(unsigned long ulPeripheral)
{
    //
    // Check the arguments.
    //
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    //
    // Enable this peripheral in sleep mode.
    //
    this->SCGCRegs[(ulPeripheral>>5)] |= (1<<(ulPeripheral&0x1f));
}

//*****************************************************************************
//
//! Disables a peripheral in sleep mode.
//!
//! \param ulPeripheral is the peripheral to disable in sleep mode.
//!
//! This function causes a peripheral to stop operating when the processor goes
//! into sleep mode.  Disabling peripherals while in sleep mode helps to lower
//! the current draw of the device.  If enabled (via SysCtlPeripheralEnable()),
//! the peripheral will automatically resume operation when the processor
//! leaves sleep mode, maintaining its entire state from before sleep mode was
//! entered.
//!
//! Sleep mode clocking of peripherals must be enabled via
//! SysCtlPeripheralClockGating(); if disabled, the peripheral sleep mode
//! configuration is maintained but has no effect when sleep mode is entered.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b ID_PERIPH_ADC0, \b ID_PERIPH_ADC1, \b ID_PERIPH_CAN0,
//! \b ID_PERIPH_CAN1, \b ID_PERIPH_CAN2, \b ID_PERIPH_COMP0,
//! \b ID_PERIPH_COMP1, \b ID_PERIPH_COMP2, \b ID_PERIPH_EPI0,
//! \b ID_PERIPH_ETH, \b ID_PERIPH_GPIOA, \b ID_PERIPH_GPIOB,
//! \b ID_PERIPH_GPIOC, \b ID_PERIPH_GPIOD, \b ID_PERIPH_GPIOE,
//! \b ID_PERIPH_GPIOF, \b ID_PERIPH_GPIOG, \b ID_PERIPH_GPIOH,
//! \b ID_PERIPH_GPIOJ, \b ID_PERIPH_HIBERNATE, \b ID_PERIPH_I2C0,
//! \b ID_PERIPH_I2C1, \b ID_PERIPH_I2S0, \b ID_PERIPH_PWM,
//! \b ID_PERIPH_QEI0, \b ID_PERIPH_QEI1, \b ID_PERIPH_SSI0,
//! \b ID_PERIPH_SSI1, \b ID_PERIPH_TIMER0, \b ID_PERIPH_TIMER1,
//! \b ID_PERIPH_TIMER2, \b ID_PERIPH_TIMER3, \b ID_PERIPH_TEMP,
//! \b ID_PERIPH_UART0, \b ID_PERIPH_UART1, \b ID_PERIPH_UART2,
//! \b ID_PERIPH_UDMA, \b ID_PERIPH_USB0, \b ID_PERIPH_WDOG0, or
//! \b ID_PERIPH_WDOG1.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlPeripheralSleepDisable(unsigned long ulPeripheral)
{
    //
    // Check the arguments.
    //
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    //
    // Disable this peripheral in sleep mode.
    //
    this->SCGCRegs[(ulPeripheral>>5)] &= ~(1<<(ulPeripheral&0x1f));
}

//*****************************************************************************
//
//! Enables a peripheral in deep-sleep mode.
//!
//! \param ulPeripheral is the peripheral to enable in deep-sleep mode.
//!
//! This function allows a peripheral to continue operating when the processor
//! goes into deep-sleep mode.  Since the clocking configuration of the device
//! may change, not all peripherals can safely continue operating while the
//! processor is in sleep mode.  Those that must run at a particular frequency
//! (such as a UART) will not work as expected if the clock changes.  It is the
//! responsibility of the caller to make sensible choices.
//!
//! Deep-sleep mode clocking of peripherals must be enabled via
//! SysCtlPeripheralClockGating(); if disabled, the peripheral deep-sleep mode
//! configuration is maintained but has no effect when deep-sleep mode is
//! entered.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b ID_PERIPH_ADC0, \b ID_PERIPH_ADC1, \b ID_PERIPH_CAN0,
//! \b ID_PERIPH_CAN1, \b ID_PERIPH_CAN2, \b ID_PERIPH_COMP0,
//! \b ID_PERIPH_COMP1, \b ID_PERIPH_COMP2, \b ID_PERIPH_EPI0,
//! \b ID_PERIPH_ETH, \b ID_PERIPH_GPIOA, \b ID_PERIPH_GPIOB,
//! \b ID_PERIPH_GPIOC, \b ID_PERIPH_GPIOD, \b ID_PERIPH_GPIOE,
//! \b ID_PERIPH_GPIOF, \b ID_PERIPH_GPIOG, \b ID_PERIPH_GPIOH,
//! \b ID_PERIPH_GPIOJ, \b ID_PERIPH_HIBERNATE, \b ID_PERIPH_I2C0,
//! \b ID_PERIPH_I2C1, \b ID_PERIPH_I2S0, \b ID_PERIPH_PWM,
//! \b ID_PERIPH_QEI0, \b ID_PERIPH_QEI1, \b ID_PERIPH_SSI0,
//! \b ID_PERIPH_SSI1, \b ID_PERIPH_TIMER0, \b ID_PERIPH_TIMER1,
//! \b ID_PERIPH_TIMER2, \b ID_PERIPH_TIMER3, \b ID_PERIPH_TEMP,
//! \b ID_PERIPH_UART0, \b ID_PERIPH_UART1, \b ID_PERIPH_UART2,
//! \b ID_PERIPH_UDMA, \b ID_PERIPH_USB0, \b ID_PERIPH_WDOG0, or
//! \b ID_PERIPH_WDOG1.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlPeripheralDeepSleepEnable(unsigned long ulPeripheral)
{
    //
    // Check the arguments.
    //
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    //
    // Enable this peripheral in deep-sleep mode.
    //
    this->DCGCRegs[(ulPeripheral>>5)] |= (1<<(ulPeripheral&0x1f));
}

//*****************************************************************************
//
//! Disables a peripheral in deep-sleep mode.
//!
//! \param ulPeripheral is the peripheral to disable in deep-sleep mode.
//!
//! This function causes a peripheral to stop operating when the processor goes
//! into deep-sleep mode.  Disabling peripherals while in deep-sleep mode helps
//! to lower the current draw of the device, and can keep peripherals that
//! require a particular clock frequency from operating when the clock changes
//! as a result of entering deep-sleep mode.  If enabled (via
//! SysCtlPeripheralEnable()), the peripheral will automatically resume
//! operation when the processor leaves deep-sleep mode, maintaining its entire
//! state from before deep-sleep mode was entered.
//!
//! Deep-sleep mode clocking of peripherals must be enabled via
//! SysCtlPeripheralClockGating(); if disabled, the peripheral deep-sleep mode
//! configuration is maintained but has no effect when deep-sleep mode is
//! entered.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b ID_PERIPH_ADC0, \b ID_PERIPH_ADC1, \b ID_PERIPH_CAN0,
//! \b ID_PERIPH_CAN1, \b ID_PERIPH_CAN2, \b ID_PERIPH_COMP0,
//! \b ID_PERIPH_COMP1, \b ID_PERIPH_COMP2, \b ID_PERIPH_EPI0,
//! \b ID_PERIPH_ETH, \b ID_PERIPH_GPIOA, \b ID_PERIPH_GPIOB,
//! \b ID_PERIPH_GPIOC, \b ID_PERIPH_GPIOD, \b ID_PERIPH_GPIOE,
//! \b ID_PERIPH_GPIOF, \b ID_PERIPH_GPIOG, \b ID_PERIPH_GPIOH,
//! \b ID_PERIPH_GPIOJ, \b ID_PERIPH_HIBERNATE, \b ID_PERIPH_I2C0,
//! \b ID_PERIPH_I2C1, \b ID_PERIPH_I2S0, \b ID_PERIPH_PWM,
//! \b ID_PERIPH_QEI0, \b ID_PERIPH_QEI1, \b ID_PERIPH_SSI0,
//! \b ID_PERIPH_SSI1, \b ID_PERIPH_TIMER0, \b ID_PERIPH_TIMER1,
//! \b ID_PERIPH_TIMER2, \b ID_PERIPH_TIMER3, \b ID_PERIPH_TEMP,
//! \b ID_PERIPH_UART0, \b ID_PERIPH_UART1, \b ID_PERIPH_UART2,
//! \b ID_PERIPH_UDMA, \b ID_PERIPH_USB0, \b ID_PERIPH_WDOG0, or
//! \b ID_PERIPH_WDOG1.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlPeripheralDeepSleepDisable(unsigned long ulPeripheral)
{
    //
    // Check the arguments.
    //
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    //
    // Disable this peripheral in deep-sleep mode.
    //
    this->DCGCRegs[(ulPeripheral>>5)] &= ~(1<<(ulPeripheral&0x1f));
}

//*****************************************************************************
//
//! Controls peripheral clock gating in sleep and deep-sleep mode.
//!
//! \param bEnable is a boolean that is \b true if the sleep and deep-sleep
//! peripheral configuration should be used and \b false if not.
//!
//! This function controls how peripherals are clocked when the processor goes
//! into sleep or deep-sleep mode.  By default, the peripherals are clocked the
//! same as in run mode; if peripheral clock gating is enabled they are clocked
//! according to the configuration set by SysCtlPeripheralSleepEnable(),
//! SysCtlPeripheralSleepDisable(), SysCtlPeripheralDeepSleepEnable(), and
//! SysCtlPeripheralDeepSleepDisable().
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlPeripheralClockGating(int bEnable)
{
    //
    // Enable peripheral clock gating as requested.
    //
    if(bEnable)
    {
        this->RCC |= SYSCTL_RCC_ACG;
    }
    else
    {
        this->RCC &= ~(SYSCTL_RCC_ACG);
    }
}


//*****************************************************************************
//
//! Enables individual system control interrupt sources.
//!
//! \param ulInts is a bit mask of the interrupt sources to be enabled.  Must
//! be a logical OR of \b SYSCTL_INT_PLL_LOCK, \b SYSCTL_INT_CUR_LIMIT,
//! \b SYSCTL_INT_IOSC_FAIL, \b SYSCTL_INT_MOSC_FAIL, \b SYSCTL_INT_POR,
//! \b SYSCTL_INT_BOR, and/or \b SYSCTL_INT_PLL_FAIL.
//!
//! Enables the indicated system control interrupt sources.  Only the sources
//! that are enabled can be reflected to the processor interrupt; disabled
//! sources have no effect on the processor.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlIntEnable(unsigned long ulInts)
{
    //
    // Enable the specified interrupts.
    //
    this->IMC |= ulInts;
}

//*****************************************************************************
//
//! Disables individual system control interrupt sources.
//!
//! \param ulInts is a bit mask of the interrupt sources to be disabled.  Must
//! be a logical OR of \b SYSCTL_INT_PLL_LOCK, \b SYSCTL_INT_CUR_LIMIT,
//! \b SYSCTL_INT_IOSC_FAIL, \b SYSCTL_INT_MOSC_FAIL, \b SYSCTL_INT_POR,
//! \b SYSCTL_INT_BOR, and/or \b SYSCTL_INT_PLL_FAIL.
//!
//! Disables the indicated system control interrupt sources.  Only the sources
//! that are enabled can be reflected to the processor interrupt; disabled
//! sources have no effect on the processor.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlIntDisable(unsigned long ulInts)
{
    //
    // Disable the specified interrupts.
    //
    this->IMC &= ~(ulInts);
}

//*****************************************************************************
//
//! Clears system control interrupt sources.
//!
//! \param ulInts is a bit mask of the interrupt sources to be cleared.  Must
//! be a logical OR of \b SYSCTL_INT_PLL_LOCK, \b SYSCTL_INT_CUR_LIMIT,
//! \b SYSCTL_INT_IOSC_FAIL, \b SYSCTL_INT_MOSC_FAIL, \b SYSCTL_INT_POR,
//! \b SYSCTL_INT_BOR, and/or \b SYSCTL_INT_PLL_FAIL.
//!
//! The specified system control interrupt sources are cleared, so that they no
//! longer assert.  This must be done in the interrupt handler to keep it from
//! being called again immediately upon exit.
//!
//! \note Since there is a write buffer in the Cortex-M3 processor, it may take
//! several clock cycles before the interrupt source is actually cleared.
//! Therefore, it is recommended that the interrupt source be cleared early in
//! the interrupt handler (as opposed to the very last action) to avoid
//! returning from the interrupt handler before the interrupt source is
//! actually cleared.  Failure to do so may result in the interrupt handler
//! being immediately reentered (since NVIC still sees the interrupt source
//! asserted).
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlIntClear(unsigned long ulInts)
{
    //
    // Clear the requested interrupt sources.
    //
    this->MISC = ulInts;
}

//*****************************************************************************
//
//! Gets the current interrupt status.
//!
//! \param bMasked is false if the raw interrupt status is required and true if
//! the masked interrupt status is required.
//!
//! This returns the interrupt status for the system controller.  Either the
//! raw interrupt status or the status of interrupts that are allowed to
//! reflect to the processor can be returned.
//!
//! \return The current interrupt status, enumerated as a bit field of
//! \b SYSCTL_INT_PLL_LOCK, \b SYSCTL_INT_CUR_LIMIT, \b SYSCTL_INT_IOSC_FAIL,
//! \b SYSCTL_INT_MOSC_FAIL, \b SYSCTL_INT_POR, \b SYSCTL_INT_BOR, and
//! \b SYSCTL_INT_PLL_FAIL.
//
//*****************************************************************************
unsigned long SYSCTL_Type::SysCtlIntStatus(int bMasked)
{
    //
    // Return either the interrupt status or the raw interrupt status as
    // requested.
    //
    if(bMasked)
    {
        return(this->MISC);
    }
    else
    {
        return(this->RIS);
    }
}

//*****************************************************************************
//
//! Sets the output voltage of the LDO.
//!
//! \param ulVoltage is the required output voltage from the LDO.  Must be one
//! of \b SYSCTL_LDO_2_25V, \b SYSCTL_LDO_2_30V, \b SYSCTL_LDO_2_35V,
//! \b SYSCTL_LDO_2_40V, \b SYSCTL_LDO_2_45V, \b SYSCTL_LDO_2_50V,
//! \b SYSCTL_LDO_2_55V, \b SYSCTL_LDO_2_60V, \b SYSCTL_LDO_2_65V,
//! \b SYSCTL_LDO_2_70V, or \b SYSCTL_LDO_2_75V.
//!
//! This function sets the output voltage of the LDO.  The default voltage is
//! 2.5 V; it can be adjusted +/- 10%.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlLDOSet(unsigned long ulVoltage)
{
    //
    // Check the arguments.
    //
    ASSERT((ulVoltage == SYSCTL_LDO_2_25V) ||
           (ulVoltage == SYSCTL_LDO_2_30V) ||
           (ulVoltage == SYSCTL_LDO_2_35V) ||
           (ulVoltage == SYSCTL_LDO_2_40V) ||
           (ulVoltage == SYSCTL_LDO_2_45V) ||
           (ulVoltage == SYSCTL_LDO_2_50V) ||
           (ulVoltage == SYSCTL_LDO_2_55V) ||
           (ulVoltage == SYSCTL_LDO_2_60V) ||
           (ulVoltage == SYSCTL_LDO_2_65V) ||
           (ulVoltage == SYSCTL_LDO_2_70V) ||
           (ulVoltage == SYSCTL_LDO_2_75V));

    //
    // Set the LDO voltage to the requested value.
    //
    this->LDOPCTL = ulVoltage;
}

//*****************************************************************************
//
//! Gets the output voltage of the LDO.
//!
//! This function determines the output voltage of the LDO, as specified by the
//! control register.
//!
//! \return Returns the current voltage of the LDO; will be one of
//! \b SYSCTL_LDO_2_25V, \b SYSCTL_LDO_2_30V, \b SYSCTL_LDO_2_35V,
//! \b SYSCTL_LDO_2_40V, \b SYSCTL_LDO_2_45V, \b SYSCTL_LDO_2_50V,
//! \b SYSCTL_LDO_2_55V, \b SYSCTL_LDO_2_60V, \b SYSCTL_LDO_2_65V,
//! \b SYSCTL_LDO_2_70V, or \b SYSCTL_LDO_2_75V.
//
//*****************************************************************************
unsigned long SYSCTL_Type::SysCtlLDOGet(void)
{
    //
    // Return the LDO voltage setting.
    //
    return(this->LDOPCTL);
}

//*****************************************************************************
//
//! Configures the LDO failure control.
//!
//! \param ulConfig is the required LDO failure control setting; can be either
//! \b SYSCTL_LDOCFG_ARST or \b SYSCTL_LDOCFG_NORST.
//!
//! This function allows the LDO to be configured to cause a processor reset
//! when the output voltage becomes unregulated.
//!
//! The LDO failure control is only available on Sandstorm-class devices.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlLDOConfigSet(unsigned long ulConfig)
{
    //
    // Check the arguments.
    //
    ASSERT((ulConfig == SYSCTL_LDOCFG_ARST) ||
           (ulConfig == SYSCTL_LDOCFG_NORST));

    //
    // Set the reset control as requested.
    //
    this->LDOARST = ulConfig;
}

//*****************************************************************************
//
//! Gets the reason for a reset.
//!
//! This function will return the reason(s) for a reset.  Since the reset
//! reasons are sticky until either cleared by software or an external reset,
//! multiple reset reasons may be returned if multiple resets have occurred.
//! The reset reason will be a logical OR of \b SYSCTL_CAUSE_LDO,
//! \b SYSCTL_CAUSE_SW, \b SYSCTL_CAUSE_WDOG, \b SYSCTL_CAUSE_BOR,
//! \b SYSCTL_CAUSE_POR, and/or \b SYSCTL_CAUSE_EXT.
//!
//! \return Returns the reason(s) for a reset.
//
//*****************************************************************************
unsigned long SYSCTL_Type::SysCtlResetCauseGet(void)
{
    //
    // Return the reset reasons.
    //
    return(this->RESC);
}

//*****************************************************************************
//
//! Clears reset reasons.
//!
//! \param ulCauses are the reset causes to be cleared; must be a logical OR of
//! \b SYSCTL_CAUSE_LDO, \b SYSCTL_CAUSE_SW, \b SYSCTL_CAUSE_WDOG,
//! \b SYSCTL_CAUSE_BOR, \b SYSCTL_CAUSE_POR, and/or \b SYSCTL_CAUSE_EXT.
//!
//! This function clears the specified sticky reset reasons.  Once cleared,
//! another reset for the same reason can be detected, and a reset for a
//! different reason can be distinguished (instead of having two reset causes
//! set).  If the reset reason is used by an application, all reset causes
//! should be cleared after they are retrieved with SysCtlResetCauseGet().
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlResetCauseClear(unsigned long ulCauses)
{
    //
    // Clear the given reset reasons.
    //
    this->RESC &= ~(ulCauses);
}

//*****************************************************************************
//
//! Configures the brown-out control.
//!
//! \param ulConfig is the desired configuration of the brown-out control.
//! Must be the logical OR of \b SYSCTL_BOR_RESET and/or
//! \b SYSCTL_BOR_RESAMPLE.
//! \param ulDelay is the number of internal oscillator cycles to wait before
//! resampling an asserted brown-out signal.  This value only has meaning when
//! \b SYSCTL_BOR_RESAMPLE is set and must be less than 8192.
//!
//! This function configures how the brown-out control operates.  It can detect
//! a brown-out by looking at only the brown-out output, or it can wait for it
//! to be active for two consecutive samples separated by a configurable time.
//! When it detects a brown-out condition, it can either reset the device or
//! generate a processor interrupt.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlBrownOutConfigSet(unsigned long ulConfig, unsigned long ulDelay)
{
    //
    // Check the arguments.
    //
    ASSERT(!(ulConfig & ~(SYSCTL_BOR_RESET | SYSCTL_BOR_RESAMPLE)));
    ASSERT(ulDelay < 8192);

    //
    // Configure the brown-out reset control.
    //
    this->PBORCTL = (ulDelay << SYSCTL_PBORCTL_BORTIM_S) | ulConfig;
}

//*****************************************************************************
//
//! Provides a small delay.
//!
//!
//! This function provides a means of generating a constant length delay.  It
//! is written in assembly to keep the delay consistent across tool chains,
//! avoiding the need to tune the delay based on the tool chain in use.
//!
//! The loop takes 3 cycles/loop.
//!
//! \return None.
//
//*****************************************************************************
/*
#if defined(ewarm) || defined(DOXYGEN)
void
SysCtlDelay(unsigned long ulCount)
{
    __asm("    subs    r0, #1\n"
          "    bne.n   SysCtlDelay\n"
          "    bx      lr");
}
#endif
#if defined(codered) || defined(gcc) || defined(sourcerygxx)
void __attribute__((naked))
SysCtlDelay(unsigned long ulCount)
{
    __asm("    subs    r0, #1\n"
          "    bne     SysCtlDelay\n"
          "    bx      lr");
}
#endif
#if defined(rvmdk) || defined(__ARMCC_VERSION)
__asm void
SysCtlDelay(unsigned long ulCount)
{
    subs    r0, #1;
    bne     SysCtlDelay;
    bx      lr;
}
#endif
#if defined(ccs)
volatile unsigned long g_ulInlineCCSWorkaround;
void
SysCtlDelay(unsigned long ulCount)
{
    __asm("delay?: subs    r0, #1\n"
          "    bne.n   delay?\n"
          "    bx lr\n");

    //
    // This is needed to keep TI compiler from optimizing away this code.
    //
    g_ulInlineCCSWorkaround += ulCount;
}
#endif
*/
extern "C"
void __attribute__((naked))
SysCtlDelay(unsigned long ulCount)
{
    __asm("    subs    r0, #1\n"
          "    bne     SysCtlDelay\n"
          "    bx      lr");
}

//*****************************************************************************
//
//! Sets the clocking of the device.
//!
//! \param ulConfig is the required configuration of the device clocking.
//!
//! This function configures the clocking of the device.  The input crystal
//! frequency, oscillator to be used, use of the PLL, and the system clock
//! divider are all configured with this function.
//!
//! The \e ulConfig parameter is the logical OR of several different values,
//! many of which are grouped into sets where only one can be chosen.
//!
//! The system clock divider is chosen with one of the following values:
//! \b SYSCTL_SYSDIV_1, \b SYSCTL_SYSDIV_2, \b SYSCTL_SYSDIV_3, ...
//! \b SYSCTL_SYSDIV_64.  Only \b SYSCTL_SYSDIV_1 through \b SYSCTL_SYSDIV_16
//! are valid on Sandstorm-class devices.
//!
//! The use of the PLL is chosen with either \b SYSCTL_USE_PLL or
//! \b SYSCTL_USE_OSC.
//!
//! The external crystal frequency is chosen with one of the following values:
//! \b SYSCTL_XTAL_1MHZ, \b SYSCTL_XTAL_1_84MHZ, \b SYSCTL_XTAL_2MHZ,
//! \b SYSCTL_XTAL_2_45MHZ, \b SYSCTL_XTAL_3_57MHZ, \b SYSCTL_XTAL_3_68MHZ,
//! \b SYSCTL_XTAL_4MHZ, \b SYSCTL_XTAL_4_09MHZ, \b SYSCTL_XTAL_4_91MHZ,
//! \b SYSCTL_XTAL_5MHZ, \b SYSCTL_XTAL_5_12MHZ, \b SYSCTL_XTAL_6MHZ,
//! \b SYSCTL_XTAL_6_14MHZ, \b SYSCTL_XTAL_7_37MHZ, \b SYSCTL_XTAL_8MHZ,
//! \b SYSCTL_XTAL_8_19MHZ, \b SYSCTL_XTAL_10MHZ, \b SYSCTL_XTAL_12MHZ,
//! \b SYSCTL_XTAL_12_2MHZ, \b SYSCTL_XTAL_13_5MHZ, \b SYSCTL_XTAL_14_3MHZ,
//! \b SYSCTL_XTAL_16MHZ, or \b SYSCTL_XTAL_16_3MHZ.  Values below
//! \b SYSCTL_XTAL_3_57MHZ are not valid when the PLL is in operation.  On
//! Sandstorm- and Fury-class devices, values above \b SYSCTL_XTAL_8_19MHZ are
//! not valid.
//!
//! The oscillator source is chosen with one of the following values:
//! \b SYSCTL_OSC_MAIN, \b SYSCTL_OSC_INT, \b SYSCTL_OSC_INT4,
//! \b SYSCTL_OSC_INT30, or \b SYSCTL_OSC_EXT32.  On Sandstorm-class devices,
//! \b SYSCTL_OSC_INT30 and \b SYSCTL_OSC_EXT32 are not valid.
//! \b SYSCTL_OSC_EXT32 is only available on devices with the hibernate module,
//! and then only when the hibernate module has been enabled.
//!
//! The internal and main oscillators are disabled with the
//! \b SYSCTL_INT_OSC_DIS and \b SYSCTL_MAIN_OSC_DIS flags, respectively.
//! The external oscillator must be enabled in order to use an external clock
//! source.  Note that attempts to disable the oscillator used to clock the
//! device will be prevented by the hardware.
//!
//! To clock the system from an external source (such as an external crystal
//! oscillator), use \b SYSCTL_USE_OSC \b | \b SYSCTL_OSC_MAIN.  To clock the
//! system from the main oscillator, use \b SYSCTL_USE_OSC \b |
//! \b SYSCTL_OSC_MAIN.  To clock the system from the PLL, use
//! \b SYSCTL_USE_PLL \b | \b SYSCTL_OSC_MAIN, and select the appropriate
//! crystal with one of the \b SYSCTL_XTAL_xxx values.
//!
//! \note If selecting the PLL as the system clock source (that is, via
//! \b SYSCTL_USE_PLL), this function will poll the PLL lock interrupt to
//! determine when the PLL has locked.  If an interrupt handler for the
//! system control interrupt is in place, and it responds to and clears the
//! PLL lock interrupt, this function will delay until its timeout has occurred
//! instead of completing as soon as PLL lock is achieved.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlClockSet(unsigned long ulConfig)
{
    unsigned long ulDelay, ulRCC, ulRCC2;

    //
    // See if this is a Sandstorm-class device and clocking features from newer
    // devices were requested.
    //
    if(CLASS_IS_SANDSTORM && (ulConfig & SYSCTL_RCC2_USERCC2))
    {
        //
        // Return without changing the clocking since the requested
        // configuration can not be achieved.
        //
        return;
    }

    //
    // Get the current value of the RCC and RCC2 registers.  If using a
    // Sandstorm-class device, the RCC2 register will read back as zero and the
    // writes to it from within this function will be ignored.
    //
    ulRCC = this->RCC;
    ulRCC2 = this->RCC2;

    //
    // Bypass the PLL and system clock dividers for now.
    //
    ulRCC |= SYSCTL_RCC_BYPASS;
    ulRCC &= ~(SYSCTL_RCC_USESYSDIV);
    ulRCC2 |= SYSCTL_RCC2_BYPASS2;

    //
    // Write the new RCC value.
    //
    this->RCC = ulRCC;
    this->RCC2 = ulRCC2;

    //
    // See if either oscillator needs to be enabled.
    //
    if(((ulRCC & SYSCTL_RCC_IOSCDIS) && !(ulConfig & SYSCTL_RCC_IOSCDIS)) ||
       ((ulRCC & SYSCTL_RCC_MOSCDIS) && !(ulConfig & SYSCTL_RCC_MOSCDIS)))
    {
        //
        // Make sure that the required oscillators are enabled.  For now, the
        // previously enabled oscillators must be enabled along with the newly
        // requested oscillators.
        //
        ulRCC &= (~(SYSCTL_RCC_IOSCDIS | SYSCTL_RCC_MOSCDIS) |
                  (ulConfig & (SYSCTL_RCC_IOSCDIS | SYSCTL_RCC_MOSCDIS)));

        //
        // Write the new RCC value.
        //
        this->RCC = ulRCC;

        //
        // Wait for a bit, giving the oscillator time to stabilize.  The number
        // of iterations is adjusted based on the current clock source; a
        // smaller number of iterations is required for slower clock rates.
        //
        if(((ulRCC2 & SYSCTL_RCC2_USERCC2) &&
            (((ulRCC2 & SYSCTL_RCC2_OSCSRC2_M) == SYSCTL_RCC2_OSCSRC2_30) ||
             ((ulRCC2 & SYSCTL_RCC2_OSCSRC2_M) == SYSCTL_RCC2_OSCSRC2_32))) ||
           (!(ulRCC2 & SYSCTL_RCC2_USERCC2) &&
            ((ulRCC & SYSCTL_RCC_OSCSRC_M) == SYSCTL_RCC_OSCSRC_30)))
        {
            //
            // Delay for 4096 iterations.
            //
            SysCtlDelay(4096);
        }
        else
        {
            //
            // Delay for 524,288 iterations.
            //
            SysCtlDelay(524288);
        }
    }

    //
    // Set the new crystal value, oscillator source, and PLL configuration.
    // Since the OSCSRC2 field in RCC2 overlaps the XTAL field in RCC, the
    // OSCSRC field has a special encoding within ulConfig to avoid the
    // overlap.
    //
    ulRCC &= ~(SYSCTL_RCC_XTAL_M | SYSCTL_RCC_OSCSRC_M |
               SYSCTL_RCC_PWRDN | SYSCTL_RCC_OEN);
    ulRCC |= ulConfig & (SYSCTL_RCC_XTAL_M | SYSCTL_RCC_OSCSRC_M |
                         SYSCTL_RCC_PWRDN | SYSCTL_RCC_OEN);
    ulRCC2 &= ~(SYSCTL_RCC2_USERCC2 | SYSCTL_RCC2_OSCSRC2_M |
                SYSCTL_RCC2_PWRDN2);
    ulRCC2 |= ulConfig & (SYSCTL_RCC2_USERCC2 | SYSCTL_RCC_OSCSRC_M |
                          SYSCTL_RCC2_PWRDN2);
    ulRCC2 |= (ulConfig & 0x00000008) << 3;

    //
    // Clear the PLL lock interrupt.
    //
    this->MISC = SYSCTL_INT_PLL_LOCK;

    //
    // Write the new RCC value.
    //
    if(ulRCC2 & SYSCTL_RCC2_USERCC2)
    {
        this->RCC2 = ulRCC2;
        this->RCC = ulRCC;
    }
    else
    {
        this->RCC = ulRCC;
        this->RCC2 = ulRCC2;
    }

    //
    // Wait for a bit so that new crystal value and oscillator source can take
    // effect.
    //
    SysCtlDelay(16);

    //
    // Set the requested system divider and disable the appropriate
    // oscillators.  This will not get written immediately.
    //
    ulRCC &= ~(SYSCTL_RCC_SYSDIV_M | SYSCTL_RCC_USESYSDIV |
               SYSCTL_RCC_IOSCDIS | SYSCTL_RCC_MOSCDIS);
    ulRCC |= ulConfig & (SYSCTL_RCC_SYSDIV_M | SYSCTL_RCC_USESYSDIV |
                         SYSCTL_RCC_IOSCDIS | SYSCTL_RCC_MOSCDIS);
    ulRCC2 &= ~(SYSCTL_RCC2_SYSDIV2_M);
    ulRCC2 |= ulConfig & SYSCTL_RCC2_SYSDIV2_M;
    if(ulConfig & SYSCTL_RCC2_DIV400)
    {
        ulRCC |= SYSCTL_RCC_USESYSDIV;
        ulRCC2 &= ~(SYSCTL_RCC_USESYSDIV);
        ulRCC2 |= ulConfig & (SYSCTL_RCC2_DIV400 | SYSCTL_RCC2_SYSDIV2LSB);
    }
    else
    {
        ulRCC2 &= ~(SYSCTL_RCC2_DIV400);
    }

    //
    // See if the PLL output is being used to clock the system.
    //
    if(!(ulConfig & SYSCTL_RCC_BYPASS))
    {
        //
        // Wait until the PLL has locked.
        //
        for(ulDelay = 32768; ulDelay > 0; ulDelay--)
        {
            if(this->RIS & SYSCTL_INT_PLL_LOCK)
            {
                break;
            }
        }

        //
        // Enable use of the PLL.
        //
        ulRCC &= ~(SYSCTL_RCC_BYPASS);
        ulRCC2 &= ~(SYSCTL_RCC2_BYPASS2);
    }

    //
    // Write the final RCC value.
    //
    this->RCC = ulRCC;
    this->RCC2 = ulRCC2;

    //
    // Delay for a little bit so that the system divider takes effect.
    //
    SysCtlDelay(16);
}


//*****************************************************************************
//
//! Sets the PWM clock configuration.
//!
//! \param ulConfig is the configuration for the PWM clock; it must be one of
//! \b SYSCTL_PWMDIV_1, \b SYSCTL_PWMDIV_2, \b SYSCTL_PWMDIV_4,
//! \b SYSCTL_PWMDIV_8, \b SYSCTL_PWMDIV_16, \b SYSCTL_PWMDIV_32, or
//! \b SYSCTL_PWMDIV_64.
//!
//! This function sets the rate of the clock provided to the PWM module as a
//! ratio of the processor clock.  This clock is used by the PWM module to
//! generate PWM signals; its rate forms the basis for all PWM signals.
//!
//! \note The clocking of the PWM is dependent upon the system clock rate as
//! configured by SysCtlClockSet().
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlPWMClockSet(unsigned long ulConfig)
{
    //
    // Check the arguments.
    //
    ASSERT((ulConfig == SYSCTL_PWMDIV_1) ||
           (ulConfig == SYSCTL_PWMDIV_2) ||
           (ulConfig == SYSCTL_PWMDIV_4) ||
           (ulConfig == SYSCTL_PWMDIV_8) ||
           (ulConfig == SYSCTL_PWMDIV_16) ||
           (ulConfig == SYSCTL_PWMDIV_32) ||
           (ulConfig == SYSCTL_PWMDIV_64));

    //
    // Check that there is a PWM block on this part.
    //
    ASSERT(this->DC1 & SYSCTL_DC1_PWM);

    //
    // Set the PWM clock configuration into the run-mode clock configuration
    // register.
    //
    this->RCC = ((this->RCC &
                          ~(SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_M)) |
                         ulConfig);
}

//*****************************************************************************
//
//! Gets the current PWM clock configuration.
//!
//! This function returns the current PWM clock configuration.
//!
//! \return Returns the current PWM clock configuration; will be one of
//! \b SYSCTL_PWMDIV_1, \b SYSCTL_PWMDIV_2, \b SYSCTL_PWMDIV_4,
//! \b SYSCTL_PWMDIV_8, \b SYSCTL_PWMDIV_16, \b SYSCTL_PWMDIV_32, or
//! \b SYSCTL_PWMDIV_64.
//
//*****************************************************************************
unsigned long SYSCTL_Type::SysCtlPWMClockGet(void)
{
    //
    // Check that there is a PWM block on this part.
    //
    ASSERT(this->DC1 & SYSCTL_DC1_PWM);

    //
    // Return the current PWM clock configuration.  Make sure that
    // SYSCTL_PWMDIV_1 is returned in all cases where the divider is disabled.
    //
    if(!(this->RCC & SYSCTL_RCC_USEPWMDIV))
    {
        //
        // The divider is not active so reflect this in the value we return.
        //
        return(SYSCTL_PWMDIV_1);
    }
    else
    {
        //
        // The divider is active so directly return the masked register value.
        //
        return(this->RCC &
               (SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_M));
    }
}

//*****************************************************************************
//
//! Sets the sample rate of the ADC.
//!
//! \param ulSpeed is the desired sample rate of the ADC; must be one of
//! \b SYSCTL_ADCSPEED_1MSPS, \b SYSCTL_ADCSPEED_500KSPS,
//! \b SYSCTL_ADCSPEED_250KSPS, or \b SYSCTL_ADCSPEED_125KSPS.
//!
//! This function sets the rate at which the ADC samples are captured by the
//! ADC block.  The sampling speed may be limited by the hardware, so the
//! sample rate may end up being slower than requested.  SysCtlADCSpeedGet()
//! will return the actual speed in use.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlADCSpeedSet(unsigned long ulSpeed)
{
    //
    // Check the arguments.
    //
    ASSERT((ulSpeed == SYSCTL_ADCSPEED_1MSPS) ||
           (ulSpeed == SYSCTL_ADCSPEED_500KSPS) ||
           (ulSpeed == SYSCTL_ADCSPEED_250KSPS) ||
           (ulSpeed == SYSCTL_ADCSPEED_125KSPS));

    //
    // Check that there is an ADC block on this part.
    //
    ASSERT(this->DC1 & SYSCTL_DC1_ADC0);

    //
    // Set the ADC speed in run, sleep, and deep-sleep mode.
    //
    this->RCGC0 = ((this->RCGC0 & ~(SYSCTL_RCGC0_ADCSPD_M)) |
                           ulSpeed);
    this->SCGC0 = ((this->SCGC0 & ~(SYSCTL_SCGC0_ADCSPD_M)) |
                           ulSpeed);
}

//*****************************************************************************
//
//! Gets the sample rate of the ADC.
//!
//! This function gets the current sample rate of the ADC.
//!
//! \return Returns the current ADC sample rate; will be one of
//! \b SYSCTL_ADCSPEED_1MSPS, \b SYSCTL_ADCSPEED_500KSPS,
//! \b SYSCTL_ADCSPEED_250KSPS, or \b SYSCTL_ADCSPEED_125KSPS.
//
//*****************************************************************************
unsigned long SYSCTL_Type::SysCtlADCSpeedGet(void)
{
    //
    // Check that there is an ADC block on this part.
    //
    ASSERT(this->DC1 & SYSCTL_DC1_ADC0);

    //
    // Return the current ADC speed.
    //
    return(this->RCGC0 & SYSCTL_RCGC0_ADCSPD_M);
}

//*****************************************************************************
//
//! Configures the internal oscillator verification timer.
//!
//! \param bEnable is a boolean that is \b true if the internal oscillator
//! verification timer should be enabled.
//!
//! This function allows the internal oscillator verification timer to be
//! enabled or disabled.  When enabled, an interrupt will be generated if the
//! internal oscillator ceases to operate.
//!
//! The internal oscillator verification timer is only available on
//! Sandstorm-class devices.
//!
//! \note Both oscillators (main and internal) must be enabled for this
//! verification timer to operate as the main oscillator will verify the
//! internal oscillator.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlIOSCVerificationSet(int bEnable)
{
    //
    // Enable or disable the internal oscillator verification timer as
    // requested.
    //
    if(bEnable)
    {
        this->RCC |= SYSCTL_RCC_IOSCVER;
    }
    else
    {
        this->RCC &= ~(SYSCTL_RCC_IOSCVER);
    }
}

//*****************************************************************************
//
//! Configures the main oscillator verification timer.
//!
//! \param bEnable is a boolean that is \b true if the main oscillator
//! verification timer should be enabled.
//!
//! This function allows the main oscillator verification timer to be enabled
//! or disabled.  When enabled, an interrupt will be generated if the main
//! oscillator ceases to operate.
//!
//! The main oscillator verification timer is only available on
//! Sandstorm-class devices.
//!
//! \note Both oscillators (main and internal) must be enabled for this
//! verification timer to operate as the internal oscillator will verify the
//! main oscillator.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlMOSCVerificationSet(int bEnable)
{
    //
    // Enable or disable the main oscillator verification timer as requested.
    //
    if(bEnable)
    {
        this->RCC |= SYSCTL_RCC_MOSCVER;
    }
    else
    {
        this->RCC &= ~(SYSCTL_RCC_MOSCVER);
    }
}

//*****************************************************************************
//
//! Configures the PLL verification timer.
//!
//! \param bEnable is a boolean that is \b true if the PLL verification timer
//! should be enabled.
//!
//! This function allows the PLL verification timer to be enabled or disabled.
//! When enabled, an interrupt will be generated if the PLL ceases to operate.
//!
//! The PLL verification timer is only available on Sandstorm-class devices.
//!
//! \note The main oscillator must be enabled for this verification timer to
//! operate as it is used to check the PLL.  Also, the verification timer
//! should be disabled while the PLL is being reconfigured via
//! SysCtlClockSet().
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlPLLVerificationSet(int bEnable)
{
    //
    // Enable or disable the PLL verification timer as requested.
    //
    if(bEnable)
    {
        this->RCC |= SYSCTL_RCC_PLLVER;
    }
    else
    {
        this->RCC &= ~(SYSCTL_RCC_PLLVER);
    }
}

//*****************************************************************************
//
//! Clears the clock verification status.
//!
//! This function clears the status of the clock verification timers, allowing
//! them to assert another failure if detected.
//!
//! The clock verification timers are only available on Sandstorm-class
//! devices.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlClkVerificationClear(void)
{
    //
    // Clear the clock verification.
    //
    this->PIOSCCAL = SYSCTL_CLKVCLR_VERCLR;

    //
    // The bit does not self-reset, so clear it.
    //
    this->PIOSCCAL = 0;
}

//*****************************************************************************
//
//! Enables a GPIO peripheral for access from the AHB.
//!
//! \param ulGPIOPeripheral is the GPIO peripheral to enable.
//!
//! This function is used to enable the specified GPIO peripheral to be
//! accessed from the Advanced Host Bus (AHB) instead of the legacy Advanced
//! Peripheral Bus (APB).  When a GPIO peripheral is enabled for AHB access,
//! the \b _AHB_BASE form of the base address should be used for GPIO
//! functions.  For example, instead of using \b GPIO_PORTA_BASE as the base
//! address for GPIO functions, use \b GPIO_PORTA_AHB_BASE instead.
//!
//! The \e ulGPIOPeripheral argument must be only one of the following values:
//! \b ID_PERIPH_GPIOA, \b ID_PERIPH_GPIOB, \b ID_PERIPH_GPIOC,
//! \b ID_PERIPH_GPIOD, \b ID_PERIPH_GPIOE, \b ID_PERIPH_GPIOF,
//! \b ID_PERIPH_GPIOG, or \b ID_PERIPH_GPIOH.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlGPIOAHBEnable(unsigned long ulGPIOPeripheral)
{
    //
    // Check the arguments.
    //
    ASSERT((ulGPIOPeripheral == ID_PERIPH_GPIOA) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOB) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOC) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOD) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOE) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOF) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOG) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOH) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOJ));

    //
    // Enable this GPIO for AHB access.
    //
    this->GPIOHBCTL |= ulGPIOPeripheral & 0xFFFF;
}

//*****************************************************************************
//
//! Disables a GPIO peripheral for access from the AHB.
//!
//! \param ulGPIOPeripheral is the GPIO peripheral to disable.
//!
//! This function disables the specified GPIO peripheral for access from the
//! Advanced Host Bus (AHB).  Once disabled, the GPIO peripheral is accessed
//! from the legacy Advanced Peripheral Bus (AHB).
//!
//! The \b ulGPIOPeripheral argument must be only one of the following values:
//! \b ID_PERIPH_GPIOA, \b ID_PERIPH_GPIOB, \b ID_PERIPH_GPIOC,
//! \b ID_PERIPH_GPIOD, \b ID_PERIPH_GPIOE, \b ID_PERIPH_GPIOF,
//! \b ID_PERIPH_GPIOG, or \b ID_PERIPH_GPIOH.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlGPIOAHBDisable(unsigned long ulGPIOPeripheral)
{
    //
    // Check the arguments.
    //
    ASSERT((ulGPIOPeripheral == ID_PERIPH_GPIOA) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOB) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOC) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOD) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOE) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOF) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOG) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOH) ||
           (ulGPIOPeripheral == ID_PERIPH_GPIOJ));

    //
    // Disable this GPIO for AHB access.
    //
    this->GPIOHBCTL &= ~(ulGPIOPeripheral & 0xFFFF);
}

//*****************************************************************************
//
//! Powers up the USB PLL.
//!
//! This function will enable the USB controller's PLL which is used by it's
//! physical layer.  This call is necessary before connecting to any external
//! devices.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlUSBPLLEnable(void)
{
    //
    // Turn on the USB PLL.
    //
    this->RCC2 &= ~SYSCTL_RCC2_USBPWRDN;
}

//*****************************************************************************
//
//! Powers down the USB PLL.
//!
//! This function will disable the USB controller's PLL which is used by it's
//! physical layer.  The USB registers are still accessible, but the physical
//! layer will no longer function.
//!
//! \return None.
//
//*****************************************************************************
void SYSCTL_Type::SysCtlUSBPLLDisable(void)
{
    //
    // Turn of USB PLL.
    //
    this->RCC2 |= SYSCTL_RCC2_USBPWRDN;
}

//*****************************************************************************
//
//! Sets the MCLK frequency provided to the I2S module.
//!
//! \param ulInputClock is the input clock to the MCLK divider.  If this is
//! zero, the value is computed from the current PLL configuration.
//! \param ulMClk is the desired MCLK frequency.  If this is zero, MCLK output
//! is disabled.
//!
//! This function sets the dividers to provide MCLK to the I2S module.  A MCLK
//! divider will be chosen that produces the MCLK frequency that is the closest
//! possible to the requested frequency, which may be above or below the
//! requested frequency.
//!
//! The actual MCLK frequency will be returned.  It is the responsibility of
//! the application to determine if the selected MCLK is acceptable; in general
//! the human ear can not discern the frequency difference if it is within 0.3%
//! of the desired frequency (though there is a very small percentage of the
//! population that can discern lower frequency deviations).
//!
//! \return Returns the actual MCLK frequency.
//
//*****************************************************************************
unsigned long SYSCTL_Type::SysCtlI2SMClkSet(unsigned long ulInputClock, unsigned long ulMClk)
{
    unsigned long ulDivInt, ulDivFrac, ulPLL;

    //
    // See if the I2S MCLK should be disabled.
    //
    if(ulMClk == 0)
    {
        //
        // Disable the I2S MCLK and return.
        //
        this->I2SMCLKCFG = 0;
        return(0);
    }

    //
    // See if the input clock was specified.
    //
    if(ulInputClock == 0)
    {
        //
        // The input clock was not specified, so compute the output frequency
        // of the PLL.  Get the current PLL configuration.
        //
        ulPLL = this->PLLCFG;

        //
        // Get the frequency of the crystal in use.
        //
        ulInputClock = g_pulXtals[(this->RCC & SYSCTL_RCC_XTAL_M) >>
                                  SYSCTL_RCC_XTAL_S];

        //
        // Calculate the PLL output frequency.
        //
        ulInputClock = ((ulInputClock * ((ulPLL & SYSCTL_PLLCFG_F_M) >>
                                         SYSCTL_PLLCFG_F_S)) /
                        ((((ulPLL & SYSCTL_PLLCFG_R_M) >>
                           SYSCTL_PLLCFG_R_S) + 1)));

        //
        // See if the optional output divide by 2 is being used.
        //
        if(ulPLL & SYSCTL_PLLCFG_OD_2)
        {
            ulInputClock /= 2;
        }

        //
        // See if the optional output divide by 4 is being used.
        //
        if(ulPLL & SYSCTL_PLLCFG_OD_4)
        {
            ulInputClock /= 4;
        }
    }

    //
    // Verify that the requested MCLK frequency is attainable.
    //
    ASSERT(ulMClk < ulInputClock);

    //
    // Add a rounding factor to the input clock, so that the MCLK frequency
    // that is closest to the desire value is selected.
    //
    ulInputClock += (ulMClk / 32) - 1;

    //
    // Compute the integer portion of the MCLK divider.
    //
    ulDivInt = ulInputClock / ulMClk;

    //
    // If the divisor is too large, then simply use the maximum divisor.
    //
    if(CLASS_IS_TEMPEST && REVISION_IS_B1 && (ulDivInt > 255))
    {
        ulDivInt = 255;
        ulDivFrac = 15;
    }
    else if(ulDivInt > 1023)
    {
        ulDivInt = 1023;
        ulDivFrac = 15;
    }
    else
    {
        //
        // Compute the fractional portion of the MCLK divider.
        //
        ulDivFrac = ((ulInputClock - (ulDivInt * ulMClk)) * 16) / ulMClk;
    }

    //
    // Set the divisor for the Tx and Rx MCLK generators and enable the clocks.
    //
    this->I2SMCLKCFG = (SYSCTL_I2SMCLKCFG_RXEN |
                                (ulDivInt << SYSCTL_I2SMCLKCFG_RXI_S) |
                                (ulDivFrac << SYSCTL_I2SMCLKCFG_RXF_S) |
                                SYSCTL_I2SMCLKCFG_TXEN |
                                (ulDivInt << SYSCTL_I2SMCLKCFG_TXI_S) |
                                (ulDivFrac << SYSCTL_I2SMCLKCFG_TXF_S));

    //
    // Return the actual MCLK frequency.
    //
    ulInputClock -= (ulMClk / 32) - 1;
    ulDivInt = (ulDivInt * 16) + ulDivFrac;
    ulMClk = (ulInputClock / ulDivInt) * 16;
    ulMClk += ((ulInputClock - ((ulMClk / 16) * ulDivInt)) * 16) / ulDivInt;
    return(ulMClk);
}

/** @} ingroup	 lm3s_sysctl */
