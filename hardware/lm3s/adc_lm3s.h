/**************************************************************************//**
 * @ingroup	 hardware_lm3s
 * @defgroup lm3s_adc_api ADC
 * @{
 * @file     lm3s/adc_lm3s.h
 * @brief    LM3S ADC Header File
 * @version  V3.00
 * @date     12. March 2010
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/

#ifndef __ADC_H__
#define __ADC_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

struct ADC_SEQ
{
	__IO uint32_t SSMUX;           /**< Multiplexer select 0 register	*/
	__IO uint32_t SSCTL;           /**< Sample sequence control 0 reg.	*/
	__I  uint32_t SSFIFO;          /**< Result FIFO 0 register	*/
	__I  uint32_t SSFSTAT;         /**< FIFO 0 status register	*/
	__IO uint32_t SSOP;			   /**< ADC Sample Sequence 0 Operation	*/
	__IO uint32_t SSDC;			   /**< ADC Sample Sequence 0 Digital Comparator Select	*/
		 uint8_t  RESERVED[8];	   /**< doc! */
};
/*----------       Analog to Digital Converter (ADC)              -----------*/
// <g> Analog to Digital Converter (ADC)
struct ADC_Type
{
	__IO uint32_t ACTSS;            /**< Active sample register	*/
	__I  uint32_t RIS;              /**< Raw interrupt status register	*/
	__IO uint32_t IM;               /**< Interrupt mask register	*/
	__IO uint32_t ISC;              /**< Interrupt status/clear register	*/
	__IO uint32_t OSTAT;            /**< Overflow status register	*/
	__IO uint32_t EMUX;             /**< Event multiplexer select reg.	*/
	__IO uint32_t USTAT;            /**< Underflow status register	*/
		 uint8_t  RESERVED0[4];		/**< doc! */
	__IO uint32_t SSPRI;            /**< Channel priority register	*/
		 uint8_t  RESERVED1[4];		/**< doc! */
	__O  uint32_t PSSI;             /**< Processor sample initiate reg.	*/
		 uint8_t  RESERVED2[4];		/**< doc! */
	__IO uint32_t SAC;              /**< Sample Averaging Control reg.	*/
	__IO uint32_t DCISC;			/**< doc! */
		 uint8_t  RESERVED3[8];		/**< doc! */
	__IO uint32_t SSMUX0;           /**< Multiplexer select 0 register	*/
	__IO uint32_t SSCTL0;           /**< Sample sequence control 0 reg.	*/
	__I  uint32_t SSFIFO0;          /**< Result FIFO 0 register	*/
	__I  uint32_t SSFSTAT0;         /**< FIFO 0 status register	*/
	__IO uint32_t SSOP0;			/**< ADC Sample Sequence 0 Operation	*/
	__IO uint32_t SSDC0;			/**< ADC Sample Sequence 0 Digital Comparator Select	*/
		 uint8_t  RESERVED4[8];		/**< doc! */
	__IO uint32_t SSMUX1;           /**< Multiplexer select 0 register	*/
	__IO uint32_t SSCTL1;           /**< Sample sequence control 0 reg.	*/
	__I  uint32_t SSFIFO1;          /**< Result FIFO 0 register	*/
	__I  uint32_t SSFSTAT1;         /**< FIFO 0 status register	*/
	__IO uint32_t SSOP1;			/**< ADC Sample Sequence 0 Operation	*/
	__IO uint32_t SSDC1;			/**< ADC Sample Sequence 0 Digital Comparator Select	*/
		 uint8_t  RESERVED5[8];		/**< doc! */
	__IO uint32_t SSMUX2;           /**< Multiplexer select 0 register	*/
	__IO uint32_t SSCTL2;           /**< Sample sequence control 0 reg.	*/
	__I  uint32_t SSFIFO2;          /**< Result FIFO 0 register	*/
	__I  uint32_t SSFSTAT2;         /**< FIFO 0 status register	*/
	__IO uint32_t SSOP2;			/**< ADC Sample Sequence 0 Operation	*/
	__IO uint32_t SSDC2;			/**< ADC Sample Sequence 0 Digital Comparator Select	*/
		 uint8_t  RESERVED6[8];		/**< doc! */
	__IO uint32_t SSMUX3;           /**< Multiplexer select 0 register	*/
	__IO uint32_t SSCTL3;           /**< Sample sequence control 0 reg.	*/
	__I  uint32_t SSFIFO3;          /**< Result FIFO 0 register	*/
	__I  uint32_t SSFSTAT3;         /**< FIFO 0 status register	*/
	__IO uint32_t SSOP3;			/**< ADC Sample Sequence 0 Operation	*/
	__IO uint32_t SSDC3;			/**< ADC Sample Sequence 0 Digital Comparator Select	*/
		 uint8_t  RESERVED7[8];		/**< doc! */
	__IO uint32_t DCRIC;			/**< ADC Digital Comparator Reset Initial Conditions	*/
		uint8_t   RESERVED8[252];	/**< doc! */
		union{
			struct{
				__IO uint32_t DCCTL0;
				__IO uint32_t DCCTL1;
				__IO uint32_t DCCTL2;
				__IO uint32_t DCCTL3;
				__IO uint32_t DCCTL4;
				__IO uint32_t DCCTL5;
				__IO uint32_t DCCTL6;
				__IO uint32_t DCCTL7;
			};
			__IO uint32_t	DCCTL[8];
		};
		 uint8_t  RESERVED9[32];	/**< doc! */
		union{
			struct{
				__IO uint32_t DCCMP0;
				__IO uint32_t DCCMP1;
				__IO uint32_t DCCMP2;
				__IO uint32_t DCCMP3;
				__IO uint32_t DCCMP4;
				__IO uint32_t DCCMP5;
				__IO uint32_t DCCMP6;
				__IO uint32_t DCCMP7;
			};
			__IO uint32_t	DCCMP[8];
		};
//		 uint8_t  RESERVED9[80-8];
//	__IO uint32_t TMLB;             // Test mode loopback register

	 void ADCIntDisable(unsigned long ulSequenceNum);
	 void ADCIntEnable(unsigned long ulSequenceNum);
	 unsigned long ADCIntStatus(unsigned long ulSequenceNum, bool bMasked);
	 void ADCIntClear(unsigned long ulSequenceNum);
	 void ADCSequenceEnable(unsigned long ulSequenceNum);
	 void ADCSequenceDisable(unsigned long ulSequenceNum);
	 void ADCSequenceConfigure(unsigned long ulSequenceNum, unsigned long ulTrigger, unsigned long ulPriority);
	 void ADCSequenceStepConfigure(unsigned long ulSequenceNum, unsigned long ulStep, unsigned long ulConfig);
	 unsigned long ADCSequenceOverflow(unsigned long ulSequenceNum);
	 void ADCSequenceOverflowClear(unsigned long ulSequenceNum);
	 unsigned long ADCSequenceUnderflow(unsigned long ulSequenceNum);
	 void ADCSequenceUnderflowClear(unsigned long ulSequenceNum);
	 unsigned long ADCSequenceDataGet(unsigned long ulSequenceNum, unsigned long *pulBuffer);
	 void ADCProcessorTrigger(unsigned long ulSequenceNum);
	 void ADCSoftwareOversampleConfigure(unsigned long ulSequenceNum, unsigned long ulFactor);
	 void ADCSoftwareOversampleStepConfigure(unsigned long ulSequenceNum, unsigned long ulStep, unsigned long ulConfig);
	 void ADCSoftwareOversampleDataGet(unsigned long ulSequenceNum, unsigned long *pulBuffer, unsigned long ulCount);
	 void ADCHardwareOversampleConfigure(unsigned long ulFactor);
	 void ADCComparatorConfigure(unsigned long ulComp, unsigned long ulConfig);
	 void ADCComparatorRegionSet(unsigned long ulComp, unsigned long ulLowRef, unsigned long ulHighRef);
	 void ADCComparatorReset(unsigned long ulComp,	bool bTrigger, bool bInterrupt);
	 void ADCComparatorIntDisable(unsigned long ulSequenceNum);
	 void ADCComparatorIntEnable(unsigned long ulSequenceNum);
	 unsigned long ADCComparatorIntStatus(void);
	 void ADCComparatorIntClear(unsigned long ulStatus);

};
//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSCTL0 register.
//
//*****************************************************************************
#define ADC_SSCTL0_TS7          0x80000000  /**< 8th Sample Temp Sensor Select	*/
#define ADC_SSCTL0_IE7          0x40000000  /**< 8th Sample Interrupt Enable	*/
#define ADC_SSCTL0_END7         0x20000000  /**< 8th Sample is End of Sequence	*/
#define ADC_SSCTL0_D7           0x10000000  /**< 8th Sample Diff Input Select	*/
#define ADC_SSCTL0_TS6          0x08000000  /**< 7th Sample Temp Sensor Select	*/
#define ADC_SSCTL0_IE6          0x04000000  /**< 7th Sample Interrupt Enable	*/
#define ADC_SSCTL0_END6         0x02000000  /**< 7th Sample is End of Sequence	*/
#define ADC_SSCTL0_D6           0x01000000  /**< 7th Sample Diff Input Select	*/
#define ADC_SSCTL0_TS5          0x00800000  /**< 6th Sample Temp Sensor Select	*/
#define ADC_SSCTL0_IE5          0x00400000  /**< 6th Sample Interrupt Enable	*/
#define ADC_SSCTL0_END5         0x00200000  /**< 6th Sample is End of Sequence	*/
#define ADC_SSCTL0_D5           0x00100000  /**< 6th Sample Diff Input Select	*/
#define ADC_SSCTL0_TS4          0x00080000  /**< 5th Sample Temp Sensor Select	*/
#define ADC_SSCTL0_IE4          0x00040000  /**< 5th Sample Interrupt Enable	*/
#define ADC_SSCTL0_END4         0x00020000  /**< 5th Sample is End of Sequence	*/
#define ADC_SSCTL0_D4           0x00010000  /**< 5th Sample Diff Input Select	*/
#define ADC_SSCTL0_TS3          0x00008000  /**< 4th Sample Temp Sensor Select	*/
#define ADC_SSCTL0_IE3          0x00004000  /**< 4th Sample Interrupt Enable	*/
#define ADC_SSCTL0_END3         0x00002000  /**< 4th Sample is End of Sequence	*/
#define ADC_SSCTL0_D3           0x00001000  /**< 4th Sample Diff Input Select	*/
#define ADC_SSCTL0_TS2          0x00000800  /**< 3rd Sample Temp Sensor Select	*/
#define ADC_SSCTL0_IE2          0x00000400  /**< 3rd Sample Interrupt Enable	*/
#define ADC_SSCTL0_END2         0x00000200  /**< 3rd Sample is End of Sequence	*/
#define ADC_SSCTL0_D2           0x00000100  /**< 3rd Sample Diff Input Select	*/
#define ADC_SSCTL0_TS1          0x00000080  /**< 2nd Sample Temp Sensor Select	*/
#define ADC_SSCTL0_IE1          0x00000040  /**< 2nd Sample Interrupt Enable	*/
#define ADC_SSCTL0_END1         0x00000020  /**< 2nd Sample is End of Sequence	*/
#define ADC_SSCTL0_D1           0x00000010  /**< 2nd Sample Diff Input Select	*/
#define ADC_SSCTL0_TS0          0x00000008  /**< 1st Sample Temp Sensor Select	*/
#define ADC_SSCTL0_IE0          0x00000004  /**< 1st Sample Interrupt Enable	*/
#define ADC_SSCTL0_END0         0x00000002  /**< 1st Sample is End of Sequence	*/
#define ADC_SSCTL0_D0           0x00000001  /**< 1st Sample Diff Input Select	*/
//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSFSTAT0 register.
//
//*****************************************************************************
#define ADC_SSFSTAT0_FULL       0x00001000  /**< FIFO Full	*/
#define ADC_SSFSTAT0_EMPTY      0x00000100  /**< FIFO Empty	*/
#define ADC_SSFSTAT0_HPTR_M     0x000000F0  /**< FIFO Head Pointer	*/
#define ADC_SSFSTAT0_TPTR_M     0x0000000F  /**< FIFO Tail Pointer	*/
#define ADC_SSFSTAT0_HPTR_S     4			/**< doc! */
#define ADC_SSFSTAT0_TPTR_S     0			/**< doc! */

// </g>
//*****************************************************************************
//
// Values that can be passed to ADCSequenceConfigure as the ulTrigger
// parameter.
//
//*****************************************************************************
#define ADC_TRIGGER_PROCESSOR   0x00000000  /**< Processor event	*/
#define ADC_TRIGGER_COMP0       0x00000001  /**< Analog comparator 0 event	*/
#define ADC_TRIGGER_COMP1       0x00000002  /**< Analog comparator 1 event	*/
#define ADC_TRIGGER_COMP2       0x00000003  /**< Analog comparator 2 event	*/
#define ADC_TRIGGER_EXTERNAL    0x00000004  /**< External event	*/
#define ADC_TRIGGER_TIMER       0x00000005  /**< Timer event	*/
#define ADC_TRIGGER_PWM0        0x00000006  /**< PWM0 event	*/
#define ADC_TRIGGER_PWM1        0x00000007  /**< PWM1 event	*/
#define ADC_TRIGGER_PWM2        0x00000008  /**< PWM2 event	*/
#define ADC_TRIGGER_PWM3        0x00000009  /**< PWM3 event	*/
#define ADC_TRIGGER_ALWAYS      0x0000000F  /**< Always event	*/

//*****************************************************************************
//
// Values that can be passed to ADCSequenceStepConfigure as the ulConfig
// parameter.
//
//*****************************************************************************
#define ADC_CTL_TS              0x00000080  /**< Temperature sensor select	*/
#define ADC_CTL_IE              0x00000040  /**< Interrupt enable	*/
#define ADC_CTL_END             0x00000020  /**< Sequence end select	*/
#define ADC_CTL_D               0x00000010  /**< Differential select	*/
#define ADC_CTL_CH0             0x00000000  /**< Input channel 0	*/
#define ADC_CTL_CH1             0x00000001  /**< Input channel 1	*/
#define ADC_CTL_CH2             0x00000002  /**< Input channel 2	*/
#define ADC_CTL_CH3             0x00000003  /**< Input channel 3	*/
#define ADC_CTL_CH4             0x00000004  /**< Input channel 4	*/
#define ADC_CTL_CH5             0x00000005  /**< Input channel 5	*/
#define ADC_CTL_CH6             0x00000006  /**< Input channel 6	*/
#define ADC_CTL_CH7             0x00000007  /**< Input channel 7	*/
#define ADC_CTL_CH8             0x00000008  /**< Input channel 8	*/
#define ADC_CTL_CH9             0x00000009  /**< Input channel 9	*/
#define ADC_CTL_CH10            0x0000000A  /**< Input channel 10	*/
#define ADC_CTL_CH11            0x0000000B  /**< Input channel 11	*/
#define ADC_CTL_CH12            0x0000000C  /**< Input channel 12	*/
#define ADC_CTL_CH13            0x0000000D  /**< Input channel 13	*/
#define ADC_CTL_CH14            0x0000000E  /**< Input channel 14	*/
#define ADC_CTL_CH15            0x0000000F  /**< Input channel 15	*/
#define ADC_CTL_CMP0            0x00080000  /**< Select Comparator 0	*/
#define ADC_CTL_CMP1            0x00090000  /**< Select Comparator 1	*/
#define ADC_CTL_CMP2            0x000A0000  /**< Select Comparator 2	*/
#define ADC_CTL_CMP3            0x000B0000  /**< Select Comparator 3	*/
#define ADC_CTL_CMP4            0x000C0000  /**< Select Comparator 4	*/
#define ADC_CTL_CMP5            0x000D0000  /**< Select Comparator 5	*/
#define ADC_CTL_CMP6            0x000E0000  /**< Select Comparator 6	*/
#define ADC_CTL_CMP7            0x000F0000  /**< Select Comparator 7	*/

//*****************************************************************************
//
// Values that can be passed to ADCComparatorConfigure as part of the
// ulConfig parameter.
//
//*****************************************************************************
#define ADC_COMP_TRIG_NONE      0x00000000  /**< Trigger Disabled	*/
#define ADC_COMP_TRIG_LOW_ALWAYS \
                                0x00001000  /**< Trigger Low Always	*/
#define ADC_COMP_TRIG_LOW_ONCE  0x00001100  /**< Trigger Low Once	*/
#define ADC_COMP_TRIG_LOW_HALWAYS \
                                0x00001200  /**< Trigger Low Always (Hysteresis)	*/
#define ADC_COMP_TRIG_LOW_HONCE 0x00001300  /**< Trigger Low Once (Hysteresis)	*/
#define ADC_COMP_TRIG_MID_ALWAYS \
                                0x00001400  /**< Trigger Mid Always	*/
#define ADC_COMP_TRIG_MID_ONCE  0x00001500  /**< Trigger Mid Once	*/
#define ADC_COMP_TRIG_HIGH_ALWAYS \
                                0x00001C00  /**< Trigger High Always	*/
#define ADC_COMP_TRIG_HIGH_ONCE 0x00001D00  /**< Trigger High Once	*/
#define ADC_COMP_TRIG_HIGH_HALWAYS \
                                0x00001E00  /**< Trigger High Always (Hysteresis)	*/
#define ADC_COMP_TRIG_HIGH_HONCE \
                                0x00001F00  /**< Trigger High Once (Hysteresis)	*/

#define ADC_COMP_INT_NONE       0x00000000  /**< Interrupt Disabled	*/
#define ADC_COMP_INT_LOW_ALWAYS \
                                0x00000010  /**< Interrupt Low Always	*/
#define ADC_COMP_INT_LOW_ONCE   0x00000011  /**< Interrupt Low Once	*/
#define ADC_COMP_INT_LOW_HALWAYS \
                                0x00000012  /**< Interrupt Low Always	*/
                                            /**< (Hysteresis)	*/
#define ADC_COMP_INT_LOW_HONCE  0x00000013  /**< Interrupt Low Once (Hysteresis)	*/
#define ADC_COMP_INT_MID_ALWAYS \
                                0x00000014  /**< Interrupt Mid Always	*/
#define ADC_COMP_INT_MID_ONCE   0x00000015  /**< Interrupt Mid Once	*/
#define ADC_COMP_INT_HIGH_ALWAYS \
                                0x0000001C  /**< Interrupt High Always	*/
#define ADC_COMP_INT_HIGH_ONCE  0x0000001D  /**< Interrupt High Once	*/
#define ADC_COMP_INT_HIGH_HALWAYS \
                                0x0000001E  /**< Interrupt High Always	*/
                                            /**< (Hysteresis)	*/
#define ADC_COMP_INT_HIGH_HONCE \
                                0x0000001F  /**< Interrupt High Once (Hysteresis)	*/

#define ADC   ((ADC_Type *)ADC_BASE)		/**< ADC Controller (only one?) */
//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __ADC_H__
/** @} defgroup lm3s_adc_api */
