/**************************************************************************//**
 * @ingroup	 lm3s_timer
 * @{
 * @file     timer_lm3s.cpp
 * @brief    TIMER class methods
 * @version  V3.00
 * @date     23. March 2010
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/

#include <tmos.h>
#include "fam_cpp.h"

/**
 *
 *  Get TIMER peripheral value
 *
 *  \return values that can be passed to the
 *			SysCtlPeripheralPresent(), SysCtlPeripheralEnable(),
 *			SysCtlPeripheralDisable(), and SysCtlPeripheralReset() APIs as the
 *			ulPeripheral parameter.
 *
 */
unsigned long  TIMER_Type::GetPeripheral(void)
{
	if( this == TIMER0 )
		return SYSCTL_PERIPH_TIMER0;
	if( this == TIMER1 )
		return SYSCTL_PERIPH_TIMER1;
	if( this == TIMER2 )
		return SYSCTL_PERIPH_TIMER2;
	if( this == TIMER3 )
		return SYSCTL_PERIPH_TIMER3;
	return 0;
}

/**
 *
 *  Reset TIMER peripheral
 *
 */
void TIMER_Type::Reset(void)
{
	SysCtlPeripheralReset(this->GetPeripheral());
}

//*****************************************************************************
//
//! Enables the timer(s).
//!
//! \param ulTimer specifies the timer(s) to enable; must be one of \b TIMER_A,
//! \b TIMER_B, or \b TIMER_BOTH.
//!
//! This will enable operation of the timer module.  The timer must be
//! configured before it is enabled.
//!
//! \return None.
//
//*****************************************************************************
void TIMER_Type::Enable(unsigned long ulTimer)
{
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));
    // Enable the timer(s) module.
    this->CTL |= ulTimer & (TIMER_CTL_TAEN | TIMER_CTL_TBEN);
}

//*****************************************************************************
//
//! Disables the timer(s).
//!
//! \param ulTimer specifies the timer(s) to disable; must be one of
//! \b TIMER_A, \b TIMER_B, or \b TIMER_BOTH.
//!
//! This will disable operation of the timer module.
//!
//! \return None.
//
//*****************************************************************************
void TIMER_Type::Disable(unsigned long ulTimer)
{
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));
    // Disable the timer module.
    this->CTL &= ~(ulTimer & (TIMER_CTL_TAEN | TIMER_CTL_TBEN));
}

//*****************************************************************************
//
//! Configures the timer(s).
//!
//! \param ulConfig is the configuration for the timer.
//!
//! This function configures the operating mode of the timer(s).  The timer
//! module is disabled before being configured, and is left in the disabled
//! state.  The configuration is specified in \e ulConfig as one of the
//! following values:
//!
//! - \b TIMER_CFG_32_BIT_OS - 32-bit one shot timer
//! - \b TIMER_CFG_32_BIT_PER - 32-bit periodic timer
//! - \b TIMER_CFG_32_RTC - 32-bit real time clock timer
//! - \b TIMER_CFG_16_BIT_PAIR - Two 16-bit timers
//!
//! When configured for a pair of 16-bit timers, each timer is separately
//! configured.  The first timer is configured by setting \e ulConfig to
//! the result of a logical OR operation between one of the following values
//! and \e ulConfig:
//!
//! - \b TIMER_CFG_A_ONE_SHOT - 16-bit one shot timer
//! - \b TIMER_CFG_A_PERIODIC - 16-bit periodic timer
//! - \b TIMER_CFG_A_CAP_COUNT - 16-bit edge count capture
//! - \b TIMER_CFG_A_CAP_TIME - 16-bit edge time capture
//! - \b TIMER_CFG_A_PWM - 16-bit PWM output
//!
//! Similarly, the second timer is configured by setting \e ulConfig to
//! the result of a logical OR operation between one of the corresponding
//! \b TIMER_CFG_B_* values and \e ulConfig.
//!
//! \return None.
//
//*****************************************************************************
void TIMER_Type::Configure(unsigned long ulConfig)
{
    ASSERT((ulConfig == TIMER_CFG_32_BIT_OS) ||
           (ulConfig == TIMER_CFG_32_BIT_PER) ||
           (ulConfig == TIMER_CFG_32_RTC) ||
           ((ulConfig & 0xff000000) == TIMER_CFG_16_BIT_PAIR));
    ASSERT(((ulConfig & 0xff000000) != TIMER_CFG_16_BIT_PAIR) ||
           ((((ulConfig & 0x000000ff) == TIMER_CFG_A_ONE_SHOT) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_PERIODIC) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_CAP_COUNT) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_CAP_TIME) ||
             ((ulConfig & 0x000000ff) == TIMER_CFG_A_PWM)) &&
            (((ulConfig & 0x0000ff00) == TIMER_CFG_B_ONE_SHOT) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_PERIODIC) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_CAP_COUNT) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_CAP_TIME) ||
             ((ulConfig & 0x0000ff00) == TIMER_CFG_B_PWM))));
    // Disable the timers.
    this->CTL &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN);
    // Set the global timer configuration.
    this->CFG = ulConfig >> 24;
    // Set the configuration of the A and B timers.  Note that the B timer
    // configuration is ignored by the hardware in 32-bit modes.
    this->TAMR = ulConfig & 255;
    this->TBMR = (ulConfig >> 8) & 255;
}

//*****************************************************************************
//
//! Controls the output level.
//!
//! \param ulTimer specifies the timer(s) to adjust; must be one of \b TIMER_A,
//! \b TIMER_B, or \b TIMER_BOTH.
//! \param bInvert specifies the output level.
//!
//! This function sets the PWM output level for the specified timer.  If the
//! \e bInvert parameter is \b true, then the timer's output will be made
//! active low; otherwise, it will be made active high.
//!
//! \return None.
//
//*****************************************************************************
void TIMER_Type::ControlLevel(unsigned long ulTimer, int bInvert)
{
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));
    // Set the output levels as requested.
    ulTimer &= TIMER_CTL_TAPWML | TIMER_CTL_TBPWML;
    this->CTL = (bInvert ?(this->CTL | ulTimer) : (this->CTL & ~(ulTimer)));
}

//*****************************************************************************
//
//! Enables or disables the trigger output.
//!
//! \param ulTimer specifies the timer to adjust; must be one of \b TIMER_A,
//! \b TIMER_B, or \b TIMER_BOTH.
//! \param bEnable specifies the desired trigger state.
//!
//! This function controls the trigger output for the specified timer.  If the
//! \e bEnable parameter is \b true, then the timer's output trigger is
//! enabled; otherwise it is disabled.
//!
//! \return None.
//
//*****************************************************************************
void TIMER_Type::ControlTrigger(unsigned long ulTimer, int bEnable)
{
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));
    // Set the trigger output as requested.
    ulTimer &= TIMER_CTL_TAOTE | TIMER_CTL_TBOTE;
    this->CTL = (bEnable ? (this->CTL | ulTimer) : (this->CTL & ~(ulTimer)));
}

//*****************************************************************************
//
//! Controls the event type.
//!
//! \param ulTimer specifies the timer(s) to be adjusted; must be one of
//! \b TIMER_A, \b TIMER_B, or \b TIMER_BOTH.
//! \param ulEvent specifies the type of event; must be one of
//! \b TIMER_EVENT_POS_EDGE, \b TIMER_EVENT_NEG_EDGE, or
//! \b TIMER_EVENT_BOTH_EDGES.
//!
//! This function sets the signal edge(s) that will trigger the timer when in
//! capture mode.
//!
//! \return None.
//
//*****************************************************************************
void TIMER_Type::ControlEvent(unsigned long ulTimer, unsigned long ulEvent)
{
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));
    // Set the event type.
    ulEvent &= ulTimer & (TIMER_CTL_TAEVENT_M | TIMER_CTL_TBEVENT_M);
    this->CTL = ((this->CTL & ~(TIMER_CTL_TAEVENT_M | TIMER_CTL_TBEVENT_M)) | ulEvent);
}

//*****************************************************************************
//
//! Controls the stall handling.
//!
//! \param ulTimer specifies the timer(s) to be adjusted; must be one of
//! \b TIMER_A, \b TIMER_B, or \b TIMER_BOTH.
//! \param bStall specifies the response to a stall signal.
//!
//! This function controls the stall response for the specified timer.  If the
//! \e bStall parameter is \b true, then the timer will stop counting if the
//! processor enters debug mode; otherwise the timer will keep running while in
//! debug mode.
//!
//! \return None.
//
//*****************************************************************************
void TIMER_Type::ControlStall(unsigned long ulTimer, int bStall)
{
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));
    // Set the stall mode.
    ulTimer &= TIMER_CTL_TASTALL | TIMER_CTL_TBSTALL;
    this->CTL = (bStall ? (this->CTL | ulTimer) : (this->CTL & ~(ulTimer)));
}

//*****************************************************************************
//
//! Enable RTC counting.
//!
//! This function causes the timer to start counting when in RTC mode.  If not
//! configured for RTC mode, this will do nothing.
//!
//! \return None.
//
//*****************************************************************************
void TIMER_Type::RTCEnable(void)
{
    // Enable RTC counting.
    this->CTL |= TIMER_CTL_RTCEN;
}

//*****************************************************************************
//
//! Disable RTC counting.
//!
//! This function causes the timer to stop counting when in RTC mode.
//!
//! \return None.
//
//*****************************************************************************
void TIMER_Type::RTCDisable(void)
{
    // Disable RTC counting.
    this->CTL &= ~(TIMER_CTL_RTCEN);
}

//*****************************************************************************
//
//! Set the timer prescale value.
//!
//! \param ulTimer specifies the timer(s) to adjust; must be one of \b TIMER_A,
//! \b TIMER_B, or \b TIMER_BOTH.
//! \param ulValue is the timer prescale value; must be between 0 and 255,
//! inclusive.
//!
//! This function sets the value of the input clock prescaler.  The prescaler
//! is only operational when in 16-bit mode and is used to extend the range of
//! the 16-bit timer modes.
//!
//! \return None.
//
//*****************************************************************************
void TIMER_Type::PrescaleSet(unsigned long ulTimer, unsigned long ulValue)
{
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));
    ASSERT(ulValue < 256);
    // Set the timer A prescaler if requested.
    if(ulTimer & TIMER_A)
        this->TAPR = ulValue;
    // Set the timer B prescaler if requested.
    if(ulTimer & TIMER_B)
        this->TBPR = ulValue;
}

//*****************************************************************************
//
//! Get the timer prescale value.
//!
//! \param ulTimer specifies the timer; must be one of \b TIMER_A or
//! \b TIMER_B.
//!
//! This function gets the value of the input clock prescaler.  The prescaler
//! is only operational when in 16-bit mode and is used to extend the range of
//! the 16-bit timer modes.
//!
//! \return The value of the timer prescaler.
//
//*****************************************************************************
unsigned long TIMER_Type::PrescaleGet(unsigned long ulTimer)
{
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));
    // Return the appropriate prescale value.
    return((ulTimer == TIMER_A) ? this->TAPR : this->TBPR);
}

//*****************************************************************************
//
//! Sets the timer load value.
//!
//! \param ulTimer specifies the timer(s) to adjust; must be one of \b TIMER_A,
//! \b TIMER_B, or \b TIMER_BOTH.  Only \b TIMER_A should be used when the
//! timer is configured for 32-bit operation.
//! \param ulValue is the load value.
//!
//! This function sets the timer load value; if the timer is running then the
//! value will be immediately loaded into the timer.
//!
//! \return None.
//
//*****************************************************************************
void TIMER_Type::LoadSet(unsigned long ulTimer, unsigned long ulValue)
{
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));
    // Set the timer A load value if requested.
    if(ulTimer & TIMER_A)
        this->TAILR = ulValue;
    // Set the timer B load value if requested.
    if(ulTimer & TIMER_B)
        this->TBILR = ulValue;
}

//*****************************************************************************
//
//! Gets the timer load value.
//!
//! \param ulTimer specifies the timer; must be one of \b TIMER_A or
//! \b TIMER_B.  Only \b TIMER_A should be used when the timer is configured
//! for 32-bit operation.
//!
//! This function gets the currently programmed interval load value for the
//! specified timer.
//!
//! \return Returns the load value for the timer.
//
//*****************************************************************************
unsigned long TIMER_Type::LoadGet(unsigned long ulTimer)
{
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B));
    // Return the appropriate load value.
    return((ulTimer == TIMER_A) ? this->TAILR : this->TBILR);
}

//*****************************************************************************
//
//! Gets the current timer value.
//!
//! \param ulTimer specifies the timer; must be one of \b TIMER_A or
//! \b TIMER_B.  Only \b TIMER_A should be used when the timer is configured
//! for 32-bit operation.
//!
//! This function reads the current value of the specified timer.
//!
//! \return Returns the current value of the timer.
//
//*****************************************************************************
unsigned long TIMER_Type::ValueGet(unsigned long ulTimer)
{
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B));
    // Return the appropriate timer value.
    return((ulTimer == TIMER_A) ? this->TAR : this->TBR);
}

//*****************************************************************************
//
//! Sets the timer match value.
//!
//! \param ulTimer specifies the timer(s) to adjust; must be one of \b TIMER_A,
//! \b TIMER_B, or \b TIMER_BOTH.  Only \b TIMER_A should be used when the
//! timer is configured for 32-bit operation.
//! \param ulValue is the match value.
//!
//! This function sets the match value for a timer.  This is used in capture
//! count mode to determine when to interrupt the processor and in PWM mode to
//! determine the duty cycle of the output signal.
//!
//! \return None.
//
//*****************************************************************************
void TIMER_Type::MatchSet(unsigned long ulTimer, unsigned long ulValue)
{
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));
    // Set the timer A match value if requested.
    if(ulTimer & TIMER_A)
        this->TAMATCHR = ulValue;
    // Set the timer B match value if requested.
    if(ulTimer & TIMER_B)
        this->TBMATCHR = ulValue;
}

//*****************************************************************************
//
//! Gets the timer match value.
//!
//! \param ulTimer specifies the timer; must be one of \b TIMER_A or
//! \b TIMER_B.  Only \b TIMER_A should be used when the timer is configured
//! for 32-bit operation.
//!
//! This function gets the match value for the specified timer.
//!
//! \return Returns the match value for the timer.
//
//*****************************************************************************
unsigned long TIMER_Type::MatchGet(unsigned long ulTimer)
{
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B));
    // Return the appropriate match value.
    return((ulTimer == TIMER_A) ? this->TAMATCHR : this->TBMATCHR);
}

//*****************************************************************************
//
//! Enables individual timer interrupt sources.
//!
//! \param ulIntFlags is the bit mask of the interrupt sources to be enabled.
//!
//! Enables the indicated timer interrupt sources.  Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor.
//!
//! The \e ulIntFlags parameter must be the logical OR of any combination of
//! the following:
//!
//! - \b TIMER_CAPB_EVENT  - Capture B event interrupt
//! - \b TIMER_CAPB_MATCH  - Capture B match interrupt
//! - \b TIMER_TIMB_TIMEOUT  - Timer B timeout interrupt
//! - \b TIMER_RTC_MATCH  - RTC interrupt mask
//! - \b TIMER_CAPA_EVENT  - Capture A event interrupt
//! - \b TIMER_CAPA_MATCH  - Capture A match interrupt
//! - \b TIMER_TIMA_TIMEOUT  - Timer A timeout interrupt
//!
//! \return None.
//
//*****************************************************************************
void TIMER_Type::IntEnable(unsigned long ulIntFlags)
{
    // Enable the specified interrupts.
    this->IMR |= ulIntFlags;
}

//*****************************************************************************
//
//! Disables individual timer interrupt sources.
//!
//! \param ulIntFlags is the bit mask of the interrupt sources to be disabled.
//!
//! Disables the indicated timer interrupt sources.  Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor.
//!
//! The \e ulIntFlags parameter has the same definition as the \e ulIntFlags
//! parameter to TimerIntEnable().
//!
//! \return None.
//
//*****************************************************************************
void TIMER_Type::IntDisable(unsigned long ulIntFlags)
{
    // Disable the specified interrupts.
    this->IMR &= ~(ulIntFlags);
}

//*****************************************************************************
//
//! Gets the current interrupt status.
//!
//! \param bMasked is false if the raw interrupt status is required and true if
//! the masked interrupt status is required.
//!
//! This returns the interrupt status for the timer module.  Either the raw
//! interrupt status or the status of interrupts that are allowed to reflect to
//! the processor can be returned.
//!
//! \return The current interrupt status, enumerated as a bit field of
//! values described in TimerIntEnable().
//
//*****************************************************************************
unsigned long TIMER_Type::IntStatus(int bMasked)
{
    // Return either the interrupt status or the raw interrupt status as
    // requested.
    return(bMasked ? this->MIS : this->RIS);
}

//*****************************************************************************
//
//! Clears timer interrupt sources.
//!
//! \param ulIntFlags is a bit mask of the interrupt sources to be cleared.
//!
//! The specified timer interrupt sources are cleared, so that they no longer
//! assert.  This must be done in the interrupt handler to keep it from being
//! called again immediately upon exit.
//!
//! The \e ulIntFlags parameter has the same definition as the \e ulIntFlags
//! parameter to TimerIntEnable().
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
void TIMER_Type::IntClear(unsigned long ulIntFlags)
{
    // Clear the requested interrupt sources.
    this->ICR = ulIntFlags;
}

/** @} ingroup	 lm3s_timer */
