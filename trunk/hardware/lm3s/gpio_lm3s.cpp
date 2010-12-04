/**************************************************************************//**
 * @ingroup	 lm3s_gpio
 * @{
 * @file     gpio_lm3s.cpp
 * @brief    GPIO class functions
 * @version  V3.00
 * @date     12. March 2010
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/

#include <tmos.h>
#include "fam_cpp.h"

/**
 * The base addresses of all the GPIO modules.
 * Both the APB and AHB apertures are provided.
 */
static const unsigned long g_pulGPIOBaseAddrs[] =
{
    GPIO_PORTA_BASE, GPIO_PORTA_AHB_BASE,
    GPIO_PORTB_BASE, GPIO_PORTB_AHB_BASE,
    GPIO_PORTC_BASE, GPIO_PORTC_AHB_BASE,
    GPIO_PORTD_BASE, GPIO_PORTD_AHB_BASE,
    GPIO_PORTE_BASE, GPIO_PORTE_AHB_BASE,
    GPIO_PORTF_BASE, GPIO_PORTF_AHB_BASE,
    GPIO_PORTG_BASE, GPIO_PORTG_AHB_BASE,
    GPIO_PORTH_BASE, GPIO_PORTH_AHB_BASE,
    GPIO_PORTJ_BASE, GPIO_PORTJ_AHB_BASE,
};

//*****************************************************************************
//
//! \internal
//! Checks a GPIO base address.
//!
//! \param ulPort is the base address of the GPIO port.
//!
//! This function determines if a GPIO port base address is valid.
//!
//! \return Returns \b true if the base address is valid and \b false
//! otherwise.
//
//*****************************************************************************
/*
#ifndef NOTRACE
static int
GPIOBaseValid(unsigned long ulPort)
{
    return((ulPort == GPIO_PORTA_BASE) || (ulPort == GPIO_PORTA_AHB_BASE) ||
           (ulPort == GPIO_PORTB_BASE) || (ulPort == GPIO_PORTB_AHB_BASE) ||
           (ulPort == GPIO_PORTC_BASE) || (ulPort == GPIO_PORTC_AHB_BASE) ||
           (ulPort == GPIO_PORTD_BASE) || (ulPort == GPIO_PORTD_AHB_BASE) ||
           (ulPort == GPIO_PORTE_BASE) || (ulPort == GPIO_PORTE_AHB_BASE) ||
           (ulPort == GPIO_PORTF_BASE) || (ulPort == GPIO_PORTF_AHB_BASE) ||
           (ulPort == GPIO_PORTG_BASE) || (ulPort == GPIO_PORTG_AHB_BASE) ||
           (ulPort == GPIO_PORTH_BASE) || (ulPort == GPIO_PORTH_AHB_BASE) ||
           (ulPort == GPIO_PORTJ_BASE) || (ulPort == GPIO_PORTJ_AHB_BASE));
}
#endif
*/
//*****************************************************************************
//
//! Sets the direction and mode of the specified pin(s).
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//! \param ulPinIO is the pin direction and/or mode.
//!
//! This function will set the specified pin(s) on the selected GPIO port
//! as either an input or output under software control, or it will set the
//! pin to be under hardware control.
//!
//! The parameter \e ulPinIO is an enumerated data type that can be one of
//! the following values:
//!
//! - \b GPIO_DIR_MODE_IN
//! - \b GPIO_DIR_MODE_OUT
//! - \b GPIO_DIR_MODE_HW
//!
//! where \b GPIO_DIR_MODE_IN specifies that the pin will be programmed as
//! a software controlled input, \b GPIO_DIR_MODE_OUT specifies that the pin
//! will be programmed as a software controlled output, and
//! \b GPIO_DIR_MODE_HW specifies that the pin will be placed under
//! hardware control.
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note GPIOPadConfigSet() must also be used to configure the corresponding
//! pad(s) in order for them to propagate the signal to/from the GPIO.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIODirModeSet( unsigned char ucPins, unsigned long ulPinIO)
{
    ASSERT((ulPinIO == GPIO_DIR_MODE_IN) || (ulPinIO == GPIO_DIR_MODE_OUT) ||
           (ulPinIO == GPIO_DIR_MODE_HW));

    this->DIR = ((ulPinIO & 1) ?
                (this->DIR | ucPins) :
                (this->DIR & ~(ucPins)));
    this->AFSEL = ((ulPinIO & 2) ?
                  (this->AFSEL | ucPins) :
                  (this->AFSEL & ~(ucPins)));
}

//*****************************************************************************
//
//! Gets the direction and mode of a pin.
//!
//! \param ucPin is the pin number.
//!
//! This function gets the direction and control mode for a specified pin on
//! the selected GPIO port.  The pin can be configured as either an input or
//! output under software control, or it can be under hardware control.  The
//! type of control and direction are returned as an enumerated data type.
//!
//! \return Returns one of the enumerated data types described for
//! GPIODirModeSet().
//
//*****************************************************************************
unsigned long GPIO_Type::GPIODirModeGet( unsigned char ucPin)
{
    unsigned long ulDir, ulAFSEL;

    ASSERT(ucPin < 8);
    ucPin = 1 << ucPin;

    ulDir = this->DIR;
    ulAFSEL = this->AFSEL;
    return(((ulDir & ucPin) ? 1 : 0) | ((ulAFSEL & ucPin) ? 2 : 0));
}

//*****************************************************************************
//
//! Sets the interrupt type for the specified pin(s).
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//! \param ulIntType specifies the type of interrupt trigger mechanism.
//!
//! This function sets up the various interrupt trigger mechanisms for the
//! specified pin(s) on the selected GPIO port.
//!
//! The parameter \e ulIntType is an enumerated data type that can be one of
//! the following values:
//!
//! - \b GPIO_FALLING_EDGE
//! - \b GPIO_RISING_EDGE
//! - \b GPIO_BOTH_EDGES
//! - \b GPIO_LOW_LEVEL
//! - \b GPIO_HIGH_LEVEL
//!
//! where the different values describe the interrupt detection mechanism
//! (edge or level) and the particular triggering event (falling, rising,
//! or both edges for edge detect, low or high for level detect).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note In order to avoid any spurious interrupts, the user must
//! ensure that the GPIO inputs remain stable for the duration of
//! this function.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOIntTypeSet(unsigned char ucPins, unsigned long ulIntType)
{
    ASSERT((ulIntType == GPIO_FALLING_EDGE) ||
           (ulIntType == GPIO_RISING_EDGE) || (ulIntType == GPIO_BOTH_EDGES) ||
           (ulIntType == GPIO_LOW_LEVEL) || (ulIntType == GPIO_HIGH_LEVEL));

    this->IBE = ((ulIntType & 1) ?
			  (this->IBE | ucPins) :
			  (this->IBE & ~(ucPins)));
    this->IS = ((ulIntType & 2) ?
			  (this->IS | ucPins) :
			  (this->IS & ~(ucPins)));
    this->IEV = ((ulIntType & 4) ?
			  (this->IEV | ucPins) :
			  (this->IEV & ~(ucPins)));
}

//*****************************************************************************
//
//! Gets the interrupt type for a pin.
//!
//! \param ucPin is the pin number.
//!
//! This function gets the interrupt type for a specified pin on the selected
//! GPIO port.  The pin can be configured as a falling edge, rising edge, or
//! both edge detected interrupt, or it can be configured as a low level or
//! high level detected interrupt.  The type of interrupt detection mechanism
//! is returned as an enumerated data type.
//!
//! \return Returns one of the enumerated data types described for
//! GPIOIntTypeSet().
//
//*****************************************************************************
unsigned long GPIO_Type::GPIOIntTypeGet(unsigned char ucPin)
{
    unsigned long ulIBE, ulIS, ulIEV;

    ASSERT(ucPin < 8);

    ucPin = 1 << ucPin;

    ulIBE = this->IBE;
    ulIS = this->IS;
    ulIEV = this->IEV;
    return(((ulIBE & ucPin) ? 1 : 0) | ((ulIS & ucPin) ? 2 : 0) |
           ((ulIEV & ucPin) ? 4 : 0));
}

//*****************************************************************************
//
//! Sets the pad configuration for the specified pin(s).
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//! \param ulStrength specifies the output drive strength.
//! \param ulPinType specifies the pin type.
//!
//! This function sets the drive strength and type for the specified pin(s)
//! on the selected GPIO port.  For pin(s) configured as input ports, the
//! pad is configured as requested, but the only real effect on the input
//! is the configuration of the pull-up or pull-down termination.
//!
//! The parameter \e ulStrength can be one of the following values:
//!
//! - \b GPIO_STRENGTH_2MA
//! - \b GPIO_STRENGTH_4MA
//! - \b GPIO_STRENGTH_8MA
//! - \b GPIO_STRENGTH_8MA_SC
//!
//! where \b GPIO_STRENGTH_xMA specifies either 2, 4, or 8 mA output drive
//! strength, and \b GPIO_OUT_STRENGTH_8MA_SC specifies 8 mA output drive with
//! slew control.
//!
//! The parameter \e ulPinType can be one of the following values:
//!
//! - \b GPIO_PIN_TYPE_STD
//! - \b GPIO_PIN_TYPE_STD_WPU
//! - \b GPIO_PIN_TYPE_STD_WPD
//! - \b GPIO_PIN_TYPE_OD
//! - \b GPIO_PIN_TYPE_OD_WPU
//! - \b GPIO_PIN_TYPE_OD_WPD
//! - \b GPIO_PIN_TYPE_ANALOG
//!
//! where \b GPIO_PIN_TYPE_STD* specifies a push-pull pin, \b GPIO_PIN_TYPE_OD*
//! specifies an open-drain pin, \b *_WPU specifies a weak pull-up, \b *_WPD
//! specifies a weak pull-down, and \b GPIO_PIN_TYPE_ANALOG specifies an
//! analog input (for the comparators).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPadConfigSet(unsigned char ucPins, unsigned long ulStrength, unsigned long ulPinType)
{
    ASSERT((ulStrength == GPIO_STRENGTH_2MA) ||
           (ulStrength == GPIO_STRENGTH_4MA) ||
           (ulStrength == GPIO_STRENGTH_8MA) ||
           (ulStrength == GPIO_STRENGTH_8MA_SC));
    ASSERT((ulPinType == GPIO_PIN_TYPE_STD) ||
           (ulPinType == GPIO_PIN_TYPE_STD_WPU) ||
           (ulPinType == GPIO_PIN_TYPE_STD_WPD) ||
           (ulPinType == GPIO_PIN_TYPE_OD) ||
           (ulPinType == GPIO_PIN_TYPE_OD_WPU) ||
           (ulPinType == GPIO_PIN_TYPE_OD_WPD) ||
           (ulPinType == GPIO_PIN_TYPE_ANALOG));

    // Set the output drive strength.
    this->DR2R = ((ulStrength & 1) ?
			  (this->DR2R | ucPins) :
			  (this->DR2R & ~(ucPins)));
    this->DR4R = ((ulStrength & 2) ?
			  (this->DR4R | ucPins) :
			  (this->DR4R & ~(ucPins)));
    this->DR8R = ((ulStrength & 4) ?
			  (this->DR8R | ucPins) :
			  (this->DR8R & ~(ucPins)));
    this->SLR = ((ulStrength & 8) ?
			  (this->SLR | ucPins) :
			  (this->SLR & ~(ucPins)));
    // Set the pin type.
    this->ODR = ((ulPinType & 1) ?
			  (this->ODR | ucPins) :
			  (this->ODR & ~(ucPins)));
    this->PUR = ((ulPinType & 2) ?
			  (this->PUR | ucPins) :
			  (this->PUR & ~(ucPins)));
    this->PDR = ((ulPinType & 4) ?
			  (this->PDR | ucPins) :
			  (this->PDR & ~(ucPins)));
    this->DEN = ((ulPinType & 8) ?
			  (this->DEN | ucPins) :
			  (this->DEN & ~(ucPins)));

    //
    // Set the analog mode select register.  This register only appears in
    // DustDevil-class (and later) devices, but is a harmless write on
    // Sandstorm- and Fury-class devices.
    //
    this->AMSEL = ((ulPinType == GPIO_PIN_TYPE_ANALOG) ?
         (this->AMSEL | ucPins) :
         (this->AMSEL & ~(ucPins)));
}

//*****************************************************************************
//
//! Gets the pad configuration for a pin.
//!
//! \param ucPin is the pin number.
//! \param pulStrength is a pointer to storage for the output drive strength.
//! \param pulPinType is a pointer to storage for the output drive type.
//!
//! This function gets the pad configuration for a specified pin on the
//! selected GPIO port.  The values returned in \e pulStrength and
//! \e pulPinType correspond to the values used in GPIOPadConfigSet().  This
//! function also works for pin(s) configured as input pin(s); however, the
//! only meaningful data returned is whether the pin is terminated with a
//! pull-up or down resistor.
//!
//! \return None
//
//*****************************************************************************
void GPIO_Type::GPIOPadConfigGet(unsigned char ucPin, unsigned long *pulStrength, unsigned long *pulPinType)
{
    unsigned long ulTemp1, ulTemp2, ulTemp3, ulTemp4;
    ASSERT(ucPin < 8);
    ucPin = (1 << ucPin);
    // Get the drive strength for this pin.
    ulTemp1 = this->DR2R;
    ulTemp2 = this->DR4R;
    ulTemp3 = this->DR8R;
    ulTemp4 = this->SLR;
    *pulStrength = (((ulTemp1 & ucPin) ? 1 : 0) | ((ulTemp2 & ucPin) ? 2 : 0) |
                    ((ulTemp3 & ucPin) ? 4 : 0) | ((ulTemp4 & ucPin) ? 8 : 0));

    // Get the pin type.
    ulTemp1 = this->ODR;
    ulTemp2 = this->PUR;
    ulTemp3 = this->PDR;
    ulTemp4 = this->DEN;
    *pulPinType = (((ulTemp1 & ucPin) ? 1 : 0) | ((ulTemp2 & ucPin) ? 2 : 0) |
                   ((ulTemp3 & ucPin) ? 4 : 0) | ((ulTemp4 & ucPin) ? 8 : 0));
}

//*****************************************************************************
//
//! Enables interrupts for the specified pin(s).
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! Unmasks the interrupt for the specified pin(s).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinIntEnable(unsigned char ucPins)
{
    this->IM |= ucPins;
}

//*****************************************************************************
//
//! Disables interrupts for the specified pin(s).
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! Masks the interrupt for the specified pin(s).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinIntDisable( unsigned char ucPins )
{
    this->IM &= ~(ucPins);
}

//*****************************************************************************
//
//! Gets interrupt status for the specified GPIO port.
//!
//! \param bMasked specifies whether masked or raw interrupt status is
//! returned.
//!
//! If \e bMasked is set as \b true, then the masked interrupt status is
//! returned; otherwise, the raw interrupt status will be returned.
//!
//! \return Returns a bit-packed byte, where each bit that is set identifies
//! an active masked or raw interrupt, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//! Bits 31:8 should be ignored.
//
//*****************************************************************************
long GPIO_Type::GPIOPinIntStatus(int bMasked)
{
    if(bMasked)
        return(this->MIS);
    return(this->RIS);
}

//*****************************************************************************
//
//! Clears the interrupt for the specified pin(s).
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! Clears the interrupt for the specified pin(s).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
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
void GPIO_Type::GPIOPinIntClear(unsigned char ucPins)
{
    this->ICR = ucPins;
}


//*****************************************************************************
//
//! Reads the values present of the specified pin(s).
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! The values at the specified pin(s) are read, as specified by \e ucPins.
//! Values are returned for both input and output pin(s), and the value
//! for pin(s) that are not specified by \e ucPins are set to 0.
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \return Returns a bit-packed byte providing the state of the specified
//! pin, where bit 0 of the byte represents GPIO port pin 0, bit 1 represents
//! GPIO port pin 1, and so on.  Any bit that is not specified by \e ucPins
//! is returned as a 0.  Bits 31:8 should be ignored.
//
//*****************************************************************************
long GPIO_Type::GPIOPinRead(unsigned char ucPins)
{
    return(this->DATA_Bits[ucPins]);
}

//*****************************************************************************
//
//! Writes a value to the specified pin(s).
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//! \param ucVal is the value to write to the pin(s).
//!
//! Writes the corresponding bit values to the output pin(s) specified by
//! \e ucPins.  Writing to a pin configured as an input pin has no effect.
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinWrite(unsigned char ucPins, unsigned char ucVal)
{
     this->DATA_Bits[ucPins]= ucVal;
}

//*****************************************************************************
//
//! Configures pin(s) for use as analog-to-digital converter inputs.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! The analog-to-digital converter input pins must be properly configured
//! to function correctly on DustDevil-class devices.  This function provides
//! the proper configuration for those pin(s).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note This cannot be used to turn any pin into an ADC input; it only
//! configures an ADC input pin for proper operation.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypeADC(unsigned char ucPins)
{
    // Make the pin(s) be inputs.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_IN);
    // Set the pad(s) for analog operation.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_ANALOG);
}

//*****************************************************************************
//
//! Configures pin(s) for use as a CAN device.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! The CAN pins must be properly configured for the CAN peripherals to
//! function correctly.  This function provides a typical configuration for
//! those pin(s); other configurations may work as well depending upon the
//! board setup (for example, using the on-chip pull-ups).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note This cannot be used to turn any pin into a CAN pin; it only
//! configures a CAN pin for proper operation.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypeCAN(unsigned char ucPins)
{
    // Make the pin(s) be inputs.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_HW);
    // Set the pad(s) for standard push-pull operation.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
}

//*****************************************************************************
//
//! Configures pin(s) for use as an analog comparator input.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! The analog comparator input pins must be properly configured for the analog
//! comparator to function correctly.  This function provides the proper
//! configuration for those pin(s).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note This cannot be used to turn any pin into an analog comparator input;
//! it only configures an analog comparator pin for proper operation.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypeComparator(unsigned char ucPins)
{
    // Make the pin(s) be inputs.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_IN);
    // Set the pad(s) for analog operation.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_ANALOG);
}

//*****************************************************************************
//
//! Configures pin(s) for use as GPIO inputs.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! The GPIO pins must be properly configured in order to function correctly as
//! GPIO inputs; this is especially true of Fury-class devices where the
//! digital input enable is turned off by default.  This function provides the
//! proper configuration for those pin(s).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypeGPIOInput(unsigned char ucPins)
{
    // Make the pin(s) be inputs.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_IN);
    // Set the pad(s) for standard push-pull operation.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
}

//*****************************************************************************
//
//! Configures pin(s) for use as GPIO outputs.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! The GPIO pins must be properly configured in order to function correctly as
//! GPIO outputs; this is especially true of Fury-class devices where the
//! digital input enable is turned off by default.  This function provides the
//! proper configuration for those pin(s).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypeGPIOOutput(unsigned char ucPins)
{
    // Make the pin(s) be outputs.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_OUT);
    // Set the pad(s) for standard push-pull operation.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
}

//*****************************************************************************
//
//! Configures pin(s) for use as GPIO open drain outputs.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! The GPIO pins must be properly configured in order to function correctly as
//! GPIO outputs; this is especially true of Fury-class devices where the
//! digital input enable is turned off by default.  This function provides the
//! proper configuration for those pin(s).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypeGPIOOutputOD(unsigned char ucPins)
{
    // Make the pin(s) be outputs.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_OUT);
    // Set the pad(s) for standard push-pull operation.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD);
}

//*****************************************************************************
//
//! Configures pin(s) for use by the I2C peripheral.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! The I2C pins must be properly configured for the I2C peripheral to function
//! correctly.  This function provides the proper configuration for those
//! pin(s).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note This cannot be used to turn any pin into an I2C pin; it only
//! configures an I2C pin for proper operation.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypeI2C(unsigned char ucPins)
{
    // Make the pin(s) be peripheral controlled.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_HW);
    // Set the pad(s) for open-drain operation with a weak pull-up.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD_WPU);
}

//*****************************************************************************
//
//! Configures pin(s) for use by the PWM peripheral.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! The PWM pins must be properly configured for the PWM peripheral to function
//! correctly.  This function provides a typical configuration for those
//! pin(s); other configurations may work as well depending upon the board
//! setup (for example, using the on-chip pull-ups).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note This cannot be used to turn any pin into a PWM pin; it only
//! configures a PWM pin for proper operation.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypePWM(unsigned char ucPins)
{
    // Make the pin(s) be peripheral controlled.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_HW);
    // Set the pad(s) for standard push-pull operation.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
}

//*****************************************************************************
//
//! Configures pin(s) for use by the QEI peripheral.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! The QEI pins must be properly configured for the QEI peripheral to function
//! correctly.  This function provides a typical configuration for those
//! pin(s); other configurations may work as well depending upon the board
//! setup (for example, not using the on-chip pull-ups).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note This cannot be used to turn any pin into a QEI pin; it only
//! configures a QEI pin for proper operation.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypeQEI(unsigned char ucPins)
{
    // Make the pin(s) be peripheral controlled.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_HW);
    // Set the pad(s) for standard push-pull operation with a weak pull-up.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

//*****************************************************************************
//
//! Configures pin(s) for use by the SSI peripheral.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! The SSI pins must be properly configured for the SSI peripheral to function
//! correctly.  This function provides a typical configuration for those
//! pin(s); other configurations may work as well depending upon the board
//! setup (for example, using the on-chip pull-ups).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note This cannot be used to turn any pin into a SSI pin; it only
//! configures a SSI pin for proper operation.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypeSSI(unsigned char ucPins)
{
    // Make the pin(s) be peripheral controlled.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_HW);
    // Set the pad(s) for standard push-pull operation.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
}

//*****************************************************************************
//
//! Configures pin(s) for use by the Timer peripheral.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! The CCP pins must be properly configured for the timer peripheral to
//! function correctly.  This function provides a typical configuration for
//! those pin(s); other configurations may work as well depending upon the
//! board setup (for example, using the on-chip pull-ups).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note This cannot be used to turn any pin into a timer pin; it only
//! configures a timer pin for proper operation.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypeTimer(unsigned char ucPins)
{
    // Make the pin(s) be peripheral controlled.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_HW);
    // Set the pad(s) for standard push-pull operation.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
}

//*****************************************************************************
//
//! Configures pin(s) for use by the UART peripheral.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! The UART pins must be properly configured for the UART peripheral to
//! function correctly.  This function provides a typical configuration for
//! those pin(s); other configurations may work as well depending upon the
//! board setup (for example, using the on-chip pull-ups).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note This cannot be used to turn any pin into a UART pin; it only
//! configures a UART pin for proper operation.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypeUART(unsigned char ucPins)
{
    // Make the pin(s) be peripheral controlled.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_HW);
    // Set the pad(s) for standard push-pull operation.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
}

//*****************************************************************************
//
//! Configures pin(s) for use by the USB peripheral.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! Some USB digital pins must be properly configured for the USB peripheral to
//! function correctly.  This function provides a typical configuration for
//! the digital USB pin(s); other configurations may work as well depending
//! upon the board setup (for example, using the on-chip pull-ups).
//!
//! This function should only be used with EPEN and PFAULT pins as all other
//! USB pins are analog in nature or are not used in devices without OTG
//! functionality.
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note This cannot be used to turn any pin into a USB pin; it only
//! configures a USB pin for proper operation.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypeUSBDigital(unsigned char ucPins)
{
    // Make the pin(s) be peripheral controlled.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_HW);
    // Set the pad(s) for standard push-pull operation.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
}

//*****************************************************************************
//
//! Configures pin(s) for use by the USB peripheral.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! Some USB analog pins must be properly configured for the USB peripheral to
//! function correctly.  This function provides the proper configuration for
//! any USB pin(s).  This can also be used to configure the EPEN and PFAULT pins
//! so that they are no longer used by the USB controller.
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note This cannot be used to turn any pin into a USB pin; it only
//! configures a USB pin for proper operation.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypeUSBAnalog(unsigned char ucPins)
{
    // Make the pin(s) be inputs.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_IN);
    // Set the pad(s) for analog operation.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_ANALOG);
}

//*****************************************************************************
//
//! Configures pin(s) for use by the I2S peripheral.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! Some I2S pins must be properly configured for the I2S peripheral to
//! function correctly.  This function provides a typical configuration for
//! the digital I2S pin(s); other configurations may work as well depending
//! upon the board setup (for example, using the on-chip pull-ups).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note This cannot be used to turn any pin into a I2S pin; it only
//! configures a I2S pin for proper operation.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypeI2S(unsigned char ucPins)
{
    // Make the pin(s) be peripheral controlled.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_HW);
    // Set the pad(s) for standard push-pull operation.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
}

//*****************************************************************************
//
//! Configures pin(s) for use by the Ethernet peripheral as LED signals.
//!
//! \param ucPins is the bit-packed representation of the pin(s).
//!
//! The Ethernet peripheral provides two signals that can be used to drive
//! an LED (e.g. for link status/activity).  This function provides a typical
//! configuration for the pins.
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note This cannot be used to turn any pin into an Ethernet LED pin; it only
//! configures an Ethernet LED pin for proper operation.
//!
//! \return None.
//
//*****************************************************************************
void GPIO_Type::GPIOPinTypeEthernetLED(unsigned char ucPins)
{
    // Make the pin(s) be peripheral controlled.
    this->GPIODirModeSet(ucPins, GPIO_DIR_MODE_HW);
    // Set the pad(s) for standard push-pull operation.
    this->GPIOPadConfigSet(ucPins, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
}


//*****************************************************************************
//
//! Configures the alternate function of a GPIO pin.
//!
//! \param ulPinConfig is the pin configuration value, specified as one of the
//! \b GPIO_P??_??? values.
//!
//! This function configures the pin mux that selects the peripheral function
//! associated with a particular GPIO pin.  Only one peripheral function at a
//! time can be associated with a GPIO pin, and each peripheral function should
//! only be associated with a single GPIO pin at a time (despite the fact that
//! many of them can be associated with more than one GPIO pin).
//!
//! \note This function is only valid on Tempest-class devices.
//!
//! \return None.
//
//*****************************************************************************

/*
void GPIOPinConfigure(unsigned long Pin)
{
	*(unsigned int *)&test_pin = Pin;
	unsigned int port_num, bit_pattern, shift_num=0;
	unsigned int mux_val;
	GPIO_Type *Port_Base;
	port_num = GET_PORT(Pin);
    if(SYSCTL->GPIOHSCTL & (1 << port_num))
        Port_Base = (GPIO_Type *)g_pulGPIOBaseAddrs[(port_num << 1) + 1];
    else
        Port_Base = (GPIO_Type *)g_pulGPIOBaseAddrs[port_num << 1];
    bit_pattern = GET_PIN(Pin);
    Port_Base->GPIOPadConfigSet(bit_pattern,GET_PAD_STRENGTH(Pin),GET_PAD_TYPE(Pin));
    Port_Base->GPIODirModeSet(bit_pattern,GET_PIN_DIR(Pin));
    Port_Base->GPIOIntTypeSet(bit_pattern,GET_PIN_INT(Pin));
    mux_val = GET_MUX(Pin);
    if(mux_val)
    {
		while(!(bit_pattern&1))
		{
			shift_num +=4;
			bit_pattern>>=1;
		}
		Port_Base->PCTL = ( (Port_Base->PCTL &~(0xf << shift_num)) | (mux_val << shift_num) );
    }
}
*/

/** @} ingroup lm3s_gpio */
