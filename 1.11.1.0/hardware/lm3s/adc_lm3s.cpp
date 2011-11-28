/**************************************************************************//**
 * @ingroup	 lm3s_adc_api
 * @{
 * @file     lm3s/adc_lm3s.cpp
 * @brief    LM3S ADC functions
 * @version  V3.00
 * @date     12. March 2010
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/
#ifndef ADC_LM3S_H_
#define ADC_LM3S_H_

#include <tmos.h>
#include "fam_cpp.h"


//*****************************************************************************
//
// These defines are used by the ADC driver to simplify access to the ADC
// sequencer's registers.
//
//*****************************************************************************
/*
#define ADC_SEQ                 (ADC_O_SSMUX0)
#define ADC_SEQ_STEP            (ADC_O_SSMUX1 - ADC_O_SSMUX0)
#define ADC_SSMUX               (ADC_O_SSMUX0 - ADC_O_SSMUX0)
#define ADC_SSCTL               (ADC_O_SSCTL0 - ADC_O_SSMUX0)
#define ADC_SSFIFO              (ADC_O_SSFIFO0 - ADC_O_SSMUX0)
#define ADC_SSFSTAT             (ADC_O_SSFSTAT0 - ADC_O_SSMUX0)
#define ADC_SSOP                (ADC_O_SSOP0 - ADC_O_SSMUX0)
#define ADC_SSDC                (ADC_O_SSDC0 - ADC_O_SSMUX0)
*/
//*****************************************************************************
//
// The currently configured software oversampling factor for each of the ADC
// sequencers.
//
//*****************************************************************************
static unsigned char g_pucOversampleFactor[3];


//*****************************************************************************
//
//! Disables a sample sequence interrupt.
//!
//! \param ulSequenceNum is the sample sequence number.
//!
//! This function disables the requested sample sequence interrupt.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCIntDisable(unsigned long ulSequenceNum)
{
    ASSERT(ulSequenceNum < 4);

    this->IM &= ~(1 << ulSequenceNum);

}

//*****************************************************************************
//
//! Enables a sample sequence interrupt.
//!
//! \param ulSequenceNum is the sample sequence number.
//!
//! This function enables the requested sample sequence interrupt.  Any
//! outstanding interrupts are cleared before enabling the sample sequence
//! interrupt.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCIntEnable(unsigned long ulSequenceNum)
{
    ASSERT(ulSequenceNum < 4);
    this->ISC = 1 << ulSequenceNum;
    // Enable this sample sequence interrupt.
    this->IM |= 1 << ulSequenceNum;
}

//*****************************************************************************
//
//! Gets the current interrupt status.
//!
//! \param ulSequenceNum is the sample sequence number.
//! \param bMasked is false if the raw interrupt status is required and true if
//! the masked interrupt status is required.
//!
//! This returns the interrupt status for the specified sample sequence.
//! Either the raw interrupt status or the status of interrupts that are
//! allowed to reflect to the processor can be returned.
//!
//! \return The current raw or masked interrupt status.
//
//*****************************************************************************
unsigned long ADC_Type::ADCIntStatus(unsigned long ulSequenceNum, bool bMasked)
{
    unsigned long ulTemp;

    ASSERT(ulSequenceNum < 4);
    // Return either the interrupt status or the raw interrupt status as requested.
    if(bMasked)
        ulTemp = this->ISC & (0x10001 << ulSequenceNum);
    else
    {
        ulTemp = this->RIS & (0x10000 | (1 << ulSequenceNum));
        // If the digital comparator status bit is set, reflect it to the appropriate sequence bit.
        if(ulTemp & 0x10000)
        {
            ulTemp |= 0xF0000;
            ulTemp &= ~(0x10000 << ulSequenceNum);
        }
    }
    // Return the interrupt status
    return(ulTemp);
}

//*****************************************************************************
//
//! Clears sample sequence interrupt source.
//!
//! \param ulSequenceNum is the sample sequence number.
//!
//! The specified sample sequence interrupt is cleared, so that it no longer
//! asserts.  This must be done in the interrupt handler to keep it from being
//! called again immediately upon exit.
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
void ADC_Type::ADCIntClear(unsigned long ulSequenceNum)
{
    // Clear the interrupt.
    this->ISC = 1 << ulSequenceNum;
}

//*****************************************************************************
//
//! Enables a sample sequence.
//!
//! \param ulSequenceNum is the sample sequence number.
//!
//! Allows the specified sample sequence to be captured when its trigger is
//! detected.  A sample sequence must be configured before it is enabled.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCSequenceEnable(unsigned long ulSequenceNum)
{
    ASSERT(ulSequenceNum < 4);
    // Enable the specified sequence.
    this->ACTSS |= 1 << ulSequenceNum;
}

//*****************************************************************************
//
//! Disables a sample sequence.
//!
//! \param ulSequenceNum is the sample sequence number.
//!
//! Prevents the specified sample sequence from being captured when its trigger
//! is detected.  A sample sequence should be disabled before it is configured.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCSequenceDisable(unsigned long ulSequenceNum)
{
    ASSERT(ulSequenceNum < 4);
    // Disable the specified sequences.
    this->ACTSS &= ~(1 << ulSequenceNum);
}

//*****************************************************************************
//
//! Configures the trigger source and priority of a sample sequence.
//!
//! \param ulSequenceNum is the sample sequence number.
//! \param ulTrigger is the trigger source that initiates the sample sequence;
//! must be one of the \b ADC_TRIGGER_* values.
//! \param ulPriority is the relative priority of the sample sequence with
//! respect to the other sample sequences.
//!
//! This function configures the initiation criteria for a sample sequence.
//! Valid sample sequences range from zero to three; sequence zero will capture
//! up to eight samples, sequences one and two will capture up to four samples,
//! and sequence three will capture a single sample.  The trigger condition and
//! priority (with respect to other sample sequence execution) is set.
//!
//! The \e ulTrigger parameter can take on the following values:
//!
//! - \b ADC_TRIGGER_PROCESSOR - A trigger generated by the processor, via the
//!                              ADCProcessorTrigger() function.
//! - \b ADC_TRIGGER_COMP0 - A trigger generated by the first analog
//!                          comparator; configured with ComparatorConfigure().
//! - \b ADC_TRIGGER_COMP1 - A trigger generated by the second analog
//!                          comparator; configured with ComparatorConfigure().
//! - \b ADC_TRIGGER_COMP2 - A trigger generated by the third analog
//!                          comparator; configured with ComparatorConfigure().
//! - \b ADC_TRIGGER_EXTERNAL - A trigger generated by an input from the Port
//!                             B4 pin.
//! - \b ADC_TRIGGER_TIMER - A trigger generated by a timer; configured with
//!                          TimerControlTrigger().
//! - \b ADC_TRIGGER_PWM0 - A trigger generated by the first PWM generator;
//!                         configured with PWMGenIntTrigEnable().
//! - \b ADC_TRIGGER_PWM1 - A trigger generated by the second PWM generator;
//!                         configured with PWMGenIntTrigEnable().
//! - \b ADC_TRIGGER_PWM2 - A trigger generated by the third PWM generator;
//!                         configured with PWMGenIntTrigEnable().
//! - \b ADC_TRIGGER_PWM3 - A trigger generated by the fourth PWM generator;
//!                         configured with PWMGenIntTrigEnable().
//! - \b ADC_TRIGGER_ALWAYS - A trigger that is always asserted, causing the
//!                           sample sequence to capture repeatedly (so long as
//!                           there is not a higher priority source active).
//!
//! Note that not all trigger sources are available on all Stellaris family
//! members; consult the data sheet for the device in question to determine the
//! availability of triggers.
//!
//! The \e ulPriority parameter is a value between 0 and 3, where 0 represents
//! the highest priority and 3 the lowest.  Note that when programming the
//! priority among a set of sample sequences, each must have unique priority;
//! it is up to the caller to guarantee the uniqueness of the priorities.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCSequenceConfigure(unsigned long ulSequenceNum, unsigned long ulTrigger, unsigned long ulPriority)
{
    ASSERT(ulSequenceNum < 4);
    ASSERT((ulTrigger == ADC_TRIGGER_PROCESSOR) ||
           (ulTrigger == ADC_TRIGGER_COMP0) ||
           (ulTrigger == ADC_TRIGGER_COMP1) ||
           (ulTrigger == ADC_TRIGGER_COMP2) ||
           (ulTrigger == ADC_TRIGGER_EXTERNAL) ||
           (ulTrigger == ADC_TRIGGER_TIMER) ||
           (ulTrigger == ADC_TRIGGER_PWM0) ||
           (ulTrigger == ADC_TRIGGER_PWM1) ||
           (ulTrigger == ADC_TRIGGER_PWM2) ||
           (ulTrigger == ADC_TRIGGER_PWM3) ||
           (ulTrigger == ADC_TRIGGER_ALWAYS));
    ASSERT(ulPriority < 4);

    // Compute the shift for the bits that control this sample sequence.
    ulSequenceNum *= 4;
    // Set the trigger event for this sample sequence.
    this->EMUX = ((this->EMUX & ~(0xf << ulSequenceNum)) | ((ulTrigger & 0xf) << ulSequenceNum));
    // Set the priority for this sample sequence.
    this->SSPRI = ((this->SSPRI & ~(0xf << ulSequenceNum)) | ((ulPriority & 0x3) << ulSequenceNum));
}

//*****************************************************************************
//
//! Configure a step of the sample sequencer.
//!
//! \param ulSequenceNum is the sample sequence number.
//! \param ulStep is the step to be configured.
//! \param ulConfig is the configuration of this step; must be a logical OR of
//! \b ADC_CTL_TS, \b ADC_CTL_IE, \b ADC_CTL_END, \b ADC_CTL_D, and one of the
//! input channel selects (\b ADC_CTL_CH0 through \b ADC_CTL_CH15).  For parts
//! with the digital comparator feature, the follow values may also be OR'd
//! into the \e ulConfig value to enable the digital comparater feature:
//! \b ADC_CTL_CE and one of the comparater selects (\b ADC_CTL_CMP0 through
//! \b ADC_CTL_CMP7).
//!
//! This function will set the configuration of the ADC for one step of a
//! sample sequence.  The ADC can be configured for single-ended or
//! differential operation (the \b ADC_CTL_D bit selects differential
//! operation when set), the channel to be sampled can be chosen (the
//! \b ADC_CTL_CH0 through \b ADC_CTL_CH15 values), and the internal
//! temperature sensor can be selected (the \b ADC_CTL_TS bit).  Additionally,
//! this step can be defined as the last in the sequence (the \b ADC_CTL_END
//! bit) and it can be configured to cause an interrupt when the step is
//! complete (the \b ADC_CTL_IE bit).  If the digital comparators are present
//! on the device, this step may also be configured send the ADC sample to
//! the selected comparator (the \b ADC_CTL_CMP0 through \b ADC_CTL_CMP7
//! values) by using the \b ADC_CTL_CE bit.  The configuration is used by the
//! ADC at the appropriate time when the trigger for this sequence occurs.
//!
//! \note If the Digitial Comparator is present and enabled using the
//! \b ADC_CTL_CE bit, the ADC sample will NOT be written into the ADC
//! sequence data FIFO.
//!
//! The \e ulStep parameter determines the order in which the samples are
//! captured by the ADC when the trigger occurs.  It can range from zero to
//! seven for the first sample sequence, from zero to three for the second and
//! third sample sequence, and can only be zero for the fourth sample sequence.
//!
//! Differential mode only works with adjacent channel pairs (for example, 0
//! and 1).  The channel select must be the number of the channel pair to
//! sample (for example, \b ADC_CTL_CH0 for 0 and 1, or \b ADC_CTL_CH1 for 2
//! and 3) or undefined results will be returned by the ADC.  Additionally, if
//! differential mode is selected when the temperature sensor is being sampled,
//! undefined results will be returned by the ADC.
//!
//! It is the responsibility of the caller to ensure that a valid configuration
//! is specified; this function does not check the validity of the specified
//! configuration.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCSequenceStepConfigure(unsigned long ulSequenceNum, unsigned long ulStep, unsigned long ulConfig)
{
    unsigned long ulTemp;
    ASSERT(ulSequenceNum < 4);
    ASSERT(((ulSequenceNum == 0) && (ulStep < 8)) ||
           ((ulSequenceNum == 1) && (ulStep < 4)) ||
           ((ulSequenceNum == 2) && (ulStep < 4)) ||
           ((ulSequenceNum == 3) && (ulStep < 1)));


    // Compute the shift for the bits that control this step.
    ulStep *= 4;
    ADC_SEQ *SEQ = (ADC_SEQ *)&this->SSMUX0;
    // = (ADC_SEQ *)&this->SSMUX0;

    // Set the analog mux value for this step.
    SEQ[ulSequenceNum].SSMUX = (SEQ[ulSequenceNum].SSMUX & ~(0x0000000f << ulStep) ) | ((ulConfig & 0x0f) << ulStep);

    // Set the control value for this step.
    SEQ[ulSequenceNum].SSCTL = (SEQ[ulSequenceNum].SSCTL & ~(0x0000000f << ulStep) ) | (((ulConfig & 0xf0) >> 4) << ulStep);

    // Enable digital comparator if specified in the ulConfig bit-fields.
    if(ulConfig & 0x000F0000)
    {
        // Program the comparator for the specified step.
        ulTemp = SEQ[ulSequenceNum].SSDC;
        ulTemp &= ~(0xF << ulStep);
        ulTemp |= (((ulConfig & 0x00070000) >> 16) << ulStep);
        SEQ[ulSequenceNum].SSDC = ulTemp;

        // Enable the comparator.
        ulTemp = SEQ[ulSequenceNum].SSOP;
        ulTemp |= (1 << ulStep);
        SEQ[ulSequenceNum].SSOP = ulTemp;
    }
    // Disable digital comparator if not specified.
    else
    {
        ulTemp = SEQ[ulSequenceNum].SSOP;
        ulTemp &= ~(1 << ulStep);
        SEQ[ulSequenceNum].SSOP = ulTemp;
    }
}

//*****************************************************************************
//
//! Determines if a sample sequence overflow occurred.
//!
//! \param ulSequenceNum is the sample sequence number.
//!
//! This determines if a sample sequence overflow has occurred.  This will
//! happen if the captured samples are not read from the FIFO before the next
//! trigger occurs.
//!
//! \return Returns zero if there was not an overflow, and non-zero if there
//! was.
//
//*****************************************************************************
unsigned long ADC_Type::ADCSequenceOverflow(unsigned long ulSequenceNum)
{
    ASSERT(ulSequenceNum < 4);
    // Determine if there was an overflow on this sequence.
    return(this->OSTAT & (1 << ulSequenceNum));
}

//*****************************************************************************
//
//! Clears the overflow condition on a sample sequence.
//!
//! \param ulSequenceNum is the sample sequence number.
//!
//! This will clear an overflow condition on one of the sample sequences.  The
//! overflow condition must be cleared in order to detect a subsequent overflow
//! condition (it otherwise causes no harm).
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCSequenceOverflowClear(unsigned long ulSequenceNum)
{
    ASSERT(ulSequenceNum < 4);
    // Clear the overflow condition for this sequence.
    this->OSTAT = 1 << ulSequenceNum;
}

//*****************************************************************************
//
//! Determines if a sample sequence underflow occurred.
//!
//! \param ulSequenceNum is the sample sequence number.
//!
//! This determines if a sample sequence underflow has occurred.  This will
//! happen if too many samples are read from the FIFO.
//!
//! \return Returns zero if there was not an underflow, and non-zero if there
//! was.
//
//*****************************************************************************
unsigned long ADC_Type::ADCSequenceUnderflow(unsigned long ulSequenceNum)
{
    ASSERT(ulSequenceNum < 4);
    // Determine if there was an underflow on this sequence.
    return(this->USTAT & (1 << ulSequenceNum));
}

//*****************************************************************************
//
//! Clears the underflow condition on a sample sequence.
//!
//! \param ulSequenceNum is the sample sequence number.
//!
//! This will clear an underflow condition on one of the sample sequences.  The
//! underflow condition must be cleared in order to detect a subsequent
//! underflow condition (it otherwise causes no harm).
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCSequenceUnderflowClear(unsigned long ulSequenceNum)
{
    ASSERT(ulSequenceNum < 4);
    // Clear the underflow condition for this sequence.
    this->USTAT = 1 << ulSequenceNum;
}

//*****************************************************************************
//
//! Gets the captured data for a sample sequence.
//!
//! \param ulSequenceNum is the sample sequence number.
//! \param pulBuffer is the address where the data is stored.
//!
//! This function copies data from the specified sample sequence output FIFO to
//! a memory resident buffer.  The number of samples available in the hardware
//! FIFO are copied into the buffer, which is assumed to be large enough to
//! hold that many samples.  This will only return the samples that are
//! presently available, which may not be the entire sample sequence if it is
//! in the process of being executed.
//!
//! \return Returns the number of samples copied to the buffer.
//
//*****************************************************************************
unsigned long ADC_Type::ADCSequenceDataGet(unsigned long ulSequenceNum, unsigned long *pulBuffer)
{
    unsigned long ulCount;
    ASSERT(ulSequenceNum < 4);
    // Read samples from the FIFO until it is empty.
    ulCount = 0;
    ADC_SEQ *SEQ = (ADC_SEQ *)&this->SSMUX0;
    while(!(SEQ[ulSequenceNum].SSFSTAT & ADC_SSFSTAT0_EMPTY) && (ulCount < 8))
    {
        // Read the FIFO and copy it to the destination.
        *pulBuffer++ = SEQ[ulSequenceNum].SSFIFO;

        // Increment the count of samples read.
        ulCount++;
    }

    // Return the number of samples read.
    return(ulCount);
}

//*****************************************************************************
//
//! Causes a processor trigger for a sample sequence.
//!
//! \param ulSequenceNum is the sample sequence number.
//!
//! This function triggers a processor-initiated sample sequence if the sample
//! sequence trigger is configured to \b ADC_TRIGGER_PROCESSOR.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCProcessorTrigger(unsigned long ulSequenceNum)
{
    ASSERT(ulSequenceNum < 4);
    // Generate a processor trigger for this sample sequence.
    this->PSSI = 1 << ulSequenceNum;
}

//*****************************************************************************
//
//! Configures the software oversampling factor of the ADC.
//!
//! \param ulSequenceNum is the sample sequence number.
//! \param ulFactor is the number of samples to be averaged.
//!
//! This function configures the software oversampling for the ADC, which can
//! be used to provide better resolution on the sampled data.  Oversampling is
//! accomplished by averaging multiple samples from the same analog input.
//! Three different oversampling rates are supported; 2x, 4x, and 8x.
//!
//! Oversampling is only supported on the sample sequencers that are more than
//! one sample in depth (that is, the fourth sample sequencer is not
//! supported).  Oversampling by 2x (for example) divides the depth of the
//! sample sequencer by two; so 2x oversampling on the first sample sequencer
//! can only provide four samples per trigger.  This also means that 8x
//! oversampling is only available on the first sample sequencer.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCSoftwareOversampleConfigure(unsigned long ulSequenceNum, unsigned long ulFactor)
{
    unsigned long ulValue;
    ASSERT(ulSequenceNum < 3);
    ASSERT(((ulFactor == 2) || (ulFactor == 4) || (ulFactor == 8)) &&
           ((ulSequenceNum == 0) || (ulFactor != 8)));

    // Convert the oversampling factor to a shift factor.
    for(ulValue = 0, ulFactor >>= 1; ulFactor; ulValue++, ulFactor >>= 1)
    {
    }

    // Save the sfiht factor.
    g_pucOversampleFactor[ulSequenceNum] = ulValue;
}

//*****************************************************************************
//
//! Configures a step of the software oversampled sequencer.
//!
//! \param ulSequenceNum is the sample sequence number.
//! \param ulStep is the step to be configured.
//! \param ulConfig is the configuration of this step.
//!
//! This function configures a step of the sample sequencer when using the
//! software oversampling feature.  The number of steps available depends on
//! the oversampling factor set by ADCSoftwareOversampleConfigure().  The value
//! of \e ulConfig is the same as defined for ADCSequenceStepConfigure().
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCSoftwareOversampleStepConfigure(unsigned long ulSequenceNum, unsigned long ulStep,unsigned long ulConfig)
{
    ASSERT(ulSequenceNum < 3);
    ASSERT(((ulSequenceNum == 0) &&
            (ulStep <  (unsigned long)(8 >> (unsigned long)g_pucOversampleFactor[ulSequenceNum]))) ||
           (ulStep <  (unsigned long)(4 >> (unsigned long)g_pucOversampleFactor[ulSequenceNum])));


    //
    // Compute the shift for the bits that control this step.
    //
    ulStep *= 4 << g_pucOversampleFactor[ulSequenceNum];

    //
    // Loop through the hardware steps that make up this step of the software
    // oversampled sequence.
    //
    ADC_SEQ *SEQ = (ADC_SEQ *)&this->SSMUX0;
    for(unsigned int val = 1 << g_pucOversampleFactor[ulSequenceNum];
        val; val--)
    {
        //
        // Set the analog mux value for this step.
        //
        SEQ[ulSequenceNum].SSMUX = ((SEQ[ulSequenceNum].SSMUX &
                                      ~(0x0000000f << ulStep)) |
                                     ((ulConfig & 0x0f) << ulStep));

        //
        // Set the control value for this step.
        //
        SEQ[ulSequenceNum].SSCTL = ((SEQ[ulSequenceNum].SSCTL &
                                      ~(0x0000000f << ulStep)) |
                                     (((ulConfig & 0xf0) >> 4) << ulStep));
        if(val != 1)
        {
        	SEQ[ulSequenceNum].SSCTL &= ~((ADC_SSCTL0_IE0 |
                                            ADC_SSCTL0_END0) << ulStep);
        }

        //
        // Go to the next hardware step.
        //
        ulStep += 4;
    }
}

//*****************************************************************************
//
//! Gets the captured data for a sample sequence using software oversampling.
//!
//! \param ulSequenceNum is the sample sequence number.
//! \param pulBuffer is the address where the data is stored.
//! \param ulCount is the number of samples to be read.
//!
//! This function copies data from the specified sample sequence output FIFO to
//! a memory resident buffer with software oversampling applied.  The requested
//! number of samples are copied into the data buffer; if there are not enough
//! samples in the hardware FIFO to satisfy this many oversampled data items
//! then incorrect results will be returned.  It is the caller's responsibility
//! to read only the samples that are available and wait until enough data is
//! available, for example as a result of receiving an interrupt.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCSoftwareOversampleDataGet(unsigned long ulSequenceNum,
                             unsigned long *pulBuffer, unsigned long ulCount)
{
    unsigned long ulIdx, ulAccum;

    ASSERT(ulSequenceNum < 3);
    ASSERT(((ulSequenceNum == 0) &&
            (ulCount <  (unsigned long)(8 >>  (unsigned long)g_pucOversampleFactor[ulSequenceNum]))) ||
           (ulCount <  (unsigned long)(4 >>  (unsigned long)g_pucOversampleFactor[ulSequenceNum])));

    //
    // Get the offset of the sequence to be read.
    //
    ADC_SEQ *SEQ = (ADC_SEQ *)&this->SSMUX0;
    // Read the samples from the FIFO until it is empty.
    while(ulCount--)
    {
        // Compute the sum of the samples.
        ulAccum = 0;
        for(ulIdx = 1 << g_pucOversampleFactor[ulSequenceNum]; ulIdx; ulIdx--)
        {
            // Read the FIFO and add it to the accumulator.
            ulAccum += SEQ[ulSequenceNum].SSFIFO;
        }
        // Write the averaged sample to the output buffer.
        *pulBuffer++ = ulAccum >> g_pucOversampleFactor[ulSequenceNum];
    }
}

//*****************************************************************************
//
//! Configures the hardware oversampling factor of the ADC.
//!
//! \param ulFactor is the number of samples to be averaged.
//!
//! This function configures the hardware oversampling for the ADC, which can
//! be used to provide better resolution on the sampled data.  Oversampling is
//! accomplished by averaging multiple samples from the same analog input.  Six
//! different oversampling rates are supported; 2x, 4x, 8x, 16x, 32x, and 64x.
//! Specifying an oversampling factor of zero will disable hardware
//! oversampling.
//!
//! Hardware oversampling applies uniformly to all sample sequencers.  It does
//! not reduce the depth of the sample sequencers like the software
//! oversampling APIs; each sample written into the sample sequence FIFO is a
//! fully oversampled analog input reading.
//!
//! Enabling hardware averaging increases the precision of the ADC at the cost
//! of throughput.  For example, enabling 4x oversampling reduces the
//! throughput of a 250 Ksps ADC to 62.5 Ksps.
//!
//! \note Hardware oversampling is available beginning with Rev C0 of the
//! Stellaris microcontroller.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCHardwareOversampleConfigure(unsigned long ulFactor)
{
    unsigned long ulValue;
    ASSERT(((ulFactor == 0) || (ulFactor == 2) || (ulFactor == 4) ||
           (ulFactor == 8) || (ulFactor == 16) || (ulFactor == 32) ||
           (ulFactor == 64)));

    // Convert the oversampling factor to a shift factor.
    for(ulValue = 0, ulFactor >>= 1; ulFactor; ulValue++, ulFactor >>= 1)
    {
    }

    // Write the shift factor to the ADC to configure the hardware oversampler.
    this->SAC = ulValue;
}

//*****************************************************************************
//
//! Configures an ADC digital comparator.
//!
//! \param ulComp is the index of the comparator to configure.
//! \param ulConfig is the configuration of the comparator.
//!
//! This function will configure a comparator.  The \e ulConfig parameter is
//! the result of a logical OR operation between the \b ADC_COMP_TRIG_xxx, and
//! \b ADC_COMP_INT_xxx values.
//!
//! The \b ADC_COMP_TRIG_xxx term can take on the following values:
//!
//! - \b ADC_COMP_TRIG_NONE to never trigger PWM fault condition.
//! - \b ADC_COMP_TRIG_LOW_ALWAYS to always trigger PWM fault condition when
//! ADC output is in the low-band.
//! - \b ADC_COMP_TRIG_LOW_ONCE to trigger PWM fault condition once when ADC
//! output transitions into the low-band.
//! - \b ADC_COMP_TRIG_LOW_HALWAYS to always trigger PWM fault condition when
//! ADC output is in the low-band only if ADC output has been in the high-band
//! since the last trigger output.
//! - \b ADC_COMP_TRIG_LOW_HONCE to trigger PWM fault condition once when ADC
//! output transitions into low-band only if ADC output has been in the
//! high-band since the last trigger output.
//! - \b ADC_COMP_TRIG_MID_ALWAYS to always trigger PWM fault condition when
//! ADC output is in the mid-band.
//! - \b ADC_COMP_TRIG_MID_ONCE to trigger PWM fault condition once when ADC
//! output transitions into the mid-band.
//! - \b ADC_COMP_TRIG_HIGH_ALWAYS to always trigger PWM fault condition when
//! ADC output is in the high-band.
//! - \b ADC_COMP_TRIG_HIGH_ONCE to trigger PWM fault condition once when ADC
//! output transitions into the high-band.
//! - \b ADC_COMP_TRIG_HIGH_HALWAYS to always trigger PWM fault condition when
//! ADC output is in the high-band only if ADC output has been in the low-band
//! since the last trigger output.
//! - \b ADC_COMP_TRIG_HIGH_HONCE to trigger PWM fault condition once when ADC
//! output transitions into high-band only if ADC output has been in the
//! low-band since the last trigger output.
//!
//! The \b ADC_COMP_INT_xxx term can take on the following values:
//!
//! - \b ADC_COMP_INT_NONE to never generate ADC interrupt.
//! - \b ADC_COMP_INT_LOW_ALWAYS to always generate ADC interrupt when ADC
//! output is in the low-band.
//! - \b ADC_COMP_INT_LOW_ONCE to generate ADC interrupt once when ADC output
//! transitions into the low-band.
//! - \b ADC_COMP__INT_LOW_HALWAYS to always generate ADC interrupt when ADC
//! output is in the low-band only if ADC output has been in the high-band
//! since the last trigger output.
//! - \b ADC_COMP_INT_LOW_HONCE to generate ADC interrupt once when ADC output
//! transitions into low-band only if ADC output has been in the high-band
//! since the last trigger output.
//! - \b ADC_COMP_INT_MID_ALWAYS to always generate ADC interrupt when ADC
//! output is in the mid-band.
//! - \b ADC_COMP_INT_MID_ONCE to generate ADC interrupt once when ADC output
//! transitions into the mid-band.
//! - \b ADC_COMP_INT_HIGH_ALWAYS to always generate ADC interrupt when ADC
//! output is in the high-band.
//! - \b ADC_COMP_INT_HIGH_ONCE to generate ADC interrupt once when ADC output
//! transitions into the high-band.
//! - \b ADC_COMP_INT_HIGH_HALWAYS to always generate ADC interrupt when ADC
//! output is in the high-band only if ADC output has been in the low-band
//! since the last trigger output.
//! - \b ADC_COMP_INT_HIGH_HONCE to generate ADC interrupt once when ADC output
//! transitions into high-band only if ADC output has been in the low-band
//! since the last trigger output.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCComparatorConfigure(unsigned long ulComp, unsigned long ulConfig)
{
    ASSERT(ulComp < 8);
    // Save the new setting.
    this->DCCTL[ulComp] = ulConfig;
}

//*****************************************************************************
//
//! Defines the ADC digital comparator regions.
//!
//! \param ulComp is the index of the comparator to configure.
//! \param ulLowRef is the reference point for the low/mid band threshold.
//! \param ulHighRef is the reference point for the mid/high band threshold.
//!
//! The ADC digital comparator operation is based on three ADC value regions:
//! - \b low-band is defined as any ADC value less than or equal to the
//! \e ulLowRef value.
//! - \b mid-band is defined as any ADC value greater than the \e ulLowRef
//! value but less than or equal to the \e ulHighRef value.
//! - \b high-band is defined as any ADC value greater than the \e ulHighRef
//! value.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCComparatorRegionSet(unsigned long ulComp, unsigned long ulLowRef, unsigned long ulHighRef)
{
    ASSERT(ulComp < 8);
    ASSERT((ulLowRef < 1024) && (ulLowRef <= ulHighRef));
    ASSERT(ulHighRef < 1024);

    // Save the new region settings.
    this->DCCMP[ulComp] = (ulHighRef << 16) | ulLowRef;
}

//*****************************************************************************
//
//! Resets the current ADC digital comparator conditions.
//!
//! \param ulComp is the index of the comparator.
//! \param bTrigger is the flag to indicate reset of Trigger conditions.
//! \param bInterrupt is the flag to indicate reset of Interrupt conditions.
//!
//! Because the digital comparator uses current and previous ADC values, this
//! function is provide to allow the comparator to be reset to its initial
//! value to prevent stale data from being used when a sequence is enabled.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCComparatorReset(unsigned long ulComp,
                   bool bTrigger, bool bInterrupt)
{
    unsigned long ulTemp = 0;

    ASSERT(ulComp < 8);

    // Set the appropriate bits to reset the trigger and/or interrupt
    // comparator conditions.
    if(bTrigger)
    {
        ulTemp |= (1 << (16 + ulComp));
    }
    if(bInterrupt)
    {
        ulTemp |= (1 << ulComp);
    }

    this->DCRIC = ulTemp;
}

//*****************************************************************************
//
//! Disables a sample sequence comparator interrupt.
//!
//! \param ulSequenceNum is the sample sequence number.
//!
//! This function disables the requested sample sequence comparator interrupt.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCComparatorIntDisable(unsigned long ulSequenceNum)
{
    ASSERT(ulSequenceNum < 4);

    // Disable this sample sequence comparator interrupt.
    this->IM &= ~(0x10000 << ulSequenceNum);
}

//*****************************************************************************
//
//! Enables a sample sequence comparator interrupt.
//!
//! \param ulSequenceNum is the sample sequence number.
//!
//! This function enables the requested sample sequence comparator interrupt.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCComparatorIntEnable(unsigned long ulSequenceNum)
{
    ASSERT(ulSequenceNum < 4);

    // Enable this sample sequence interrupt.
    this->IM |= 0x10000 << ulSequenceNum;
}

//*****************************************************************************
//
//! Gets the current comparator interrupt status.
//!
//!
//! This returns the digitial comparator interrupt status bits.  This status
//! is sequence agnostic.
//!
//! \return The current comparator interrupt status.
//
//*****************************************************************************
unsigned long ADC_Type::ADCComparatorIntStatus(void)
{
    // Return the digitial comparator interrupt status.
    return(this->DCISC);
}

//*****************************************************************************
//
//! Clears sample sequence comparator interrupt source.
//!
//! \param ulStatus is the bit-mapped interrupts status to clear.
//!
//! The specified interrupt status is cleared.
//!
//! \return None.
//
//*****************************************************************************
void ADC_Type::ADCComparatorIntClear(unsigned long ulStatus)
{
    // Clear the interrupt.
    this->DCISC = ulStatus;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
#endif /* ADC_LM3S_H_ */
