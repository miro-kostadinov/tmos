/**************************************************************************//**
 * @ingroup	 lm3s_timer
 * @{
 * @file     timer_lm3s.h
 * @brief    TIMER class methods
 * @version  V3.00
 * @date     23. March 2010
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/

#ifndef TIMER_LM3S_H_
#define TIMER_LM3S_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*----------       General Purpose Timer (TIMER)                  -----------*/
// <g> General Purpose Timer (TIMER)
struct TIMER_Type
{
	__IO uint32_t CFG;              //!< Configuration register
	__IO uint32_t TAMR;             //!< TimerA mode register
	__IO uint32_t TBMR;             //!< TimerB mode register
	__IO uint32_t CTL;              //!< Control register
		 uint8_t  RESERVED0[8];		//!< doc!
	__IO uint32_t IMR;              //!< Interrupt mask register
	__I  uint32_t RIS;              //!< Interrupt status register
	__I  uint32_t MIS;              //!< Masked interrupt status reg
	__O  uint32_t ICR;              //!< Interrupt clear register
	__IO uint32_t TAILR;            //!< TimerA interval load register
	__IO uint32_t TBILR;            //!< TimerB interval load register
	__IO uint32_t TAMATCHR;         //!< TimerA match register
	__IO uint32_t TBMATCHR;         //!< TimerB match register
	__IO uint32_t TAPR;             //!< TimerA prescale register
	__IO uint32_t TBPR;             //!< TimerB prescale register
	__IO uint32_t TAPMR;            //!< TimerA prescale match register
	__IO uint32_t TBPMR;            //!< TimerB prescale match register
	__I  uint32_t TAR;              //!< TimerA register
	__I  uint32_t TBR;              //!< TimerB register
	__I  uint32_t TAV;              //!< GPTM Timer A Value
	__I  uint32_t TBV;              //!< GPTM Timer B Value

	unsigned long  GetPeripheral(void);
	void Reset(void);
	void Enable(unsigned long ulTimer);
	void Disable(unsigned long ulTimer);
	void Configure(unsigned long ulConfig);
	void ControlLevel(unsigned long ulTimer,int bInvert);
	void ControlTrigger(unsigned long ulTimer,int bEnable);
	void ControlEvent(unsigned long ulTimer,unsigned long ulEvent);
	void ControlStall(unsigned long ulTimer,int bStall);
	void RTCEnable(void);
	void RTCDisable(void);
	void PrescaleSet(unsigned long ulTimer,unsigned long ulValue);
	unsigned long PrescaleGet(unsigned long ulTimer);
	void LoadSet(unsigned long ulTimer,unsigned long ulValue);
	unsigned long LoadGet(unsigned long ulTimer);
	unsigned long ValueGet(unsigned long ulTimer);
	void MatchSet(unsigned long ulTimer,unsigned long ulValue);
	unsigned long MatchGet(unsigned long ulTimer);
	void IntEnable(unsigned long ulIntFlags);
	void IntDisable(unsigned long ulIntFlags);
	unsigned long IntStatus(int bMasked);
	void IntClear(unsigned long ulIntFlags);

};
// </g>

//*****************************************************************************
//
// Values that can be passed to TimerConfigure as the ulConfig parameter.
//
//*****************************************************************************
#define TIMER_CFG_32_BIT_OS     0x00000001  //!< 32-bit one-shot timer
#define TIMER_CFG_32_BIT_PER    0x00000002  //!< 32-bit periodic timer
#define TIMER_CFG_32_RTC        0x01000000  //!< 32-bit RTC timer
#define TIMER_CFG_16_BIT_PAIR   0x04000000  //!< Two 16-bit timers
#define TIMER_CFG_A_ONE_SHOT    0x00000001  //!< Timer A one-shot timer
#define TIMER_CFG_A_PERIODIC    0x00000002  //!< Timer A periodic timer
#define TIMER_CFG_A_CAP_COUNT   0x00000003  //!< Timer A event counter
#define TIMER_CFG_A_CAP_TIME    0x00000007  //!< Timer A event timer
#define TIMER_CFG_A_PWM         0x0000000A  //!< Timer A PWM output
#define TIMER_CFG_B_ONE_SHOT    0x00000100  //!< Timer B one-shot timer
#define TIMER_CFG_B_PERIODIC    0x00000200  //!< Timer B periodic timer
#define TIMER_CFG_B_CAP_COUNT   0x00000300  //!< Timer B event counter
#define TIMER_CFG_B_CAP_TIME    0x00000700  //!< Timer B event timer
#define TIMER_CFG_B_PWM         0x00000A00  //!< Timer B PWM output

//*****************************************************************************
//
// Values that can be passed to TimerIntEnable, TimerIntDisable, and
// TimerIntClear as the ulIntFlags parameter, and returned from TimerIntStatus.
//
//*****************************************************************************
#define TIMER_CAPB_EVENT        0x00000400  //!< CaptureB event interrupt
#define TIMER_CAPB_MATCH        0x00000200  //!< CaptureB match interrupt
#define TIMER_TIMB_TIMEOUT      0x00000100  //!< TimerB time out interrupt
#define TIMER_RTC_MATCH         0x00000008  //!< RTC interrupt mask
#define TIMER_CAPA_EVENT        0x00000004  //!< CaptureA event interrupt
#define TIMER_CAPA_MATCH        0x00000002  //!< CaptureA match interrupt
#define TIMER_TIMA_TIMEOUT      0x00000001  //!< TimerA time out interrupt

//*****************************************************************************
//
// Values that can be passed to TimerControlEvent as the ulEvent parameter.
//
//*****************************************************************************
#define TIMER_EVENT_POS_EDGE    0x00000000  //!< Count positive edges
#define TIMER_EVENT_NEG_EDGE    0x00000404  //!< Count negative edges
#define TIMER_EVENT_BOTH_EDGES  0x00000C0C  //!< Count both edges

//*****************************************************************************
//
// Values that can be passed to most of the timer APIs as the ulTimer
// parameter.
//
//*****************************************************************************
#define TIMER_A                 0x000000ff  //!< Timer A
#define TIMER_B                 0x0000ff00  //!< Timer B
#define TIMER_BOTH              0x0000ffff  //!< Timer Both

#define TIMER0    ((TIMER_Type *)TIMER0_BASE)	//!< TIMER 0
#define TIMER1    ((TIMER_Type *)TIMER1_BASE)	//!< TIMER 1
#define TIMER2    ((TIMER_Type *)TIMER2_BASE)	//!< TIMER 2
#define TIMER3    ((TIMER_Type *)TIMER3_BASE)	//!< TIMER 3

#ifdef __cplusplus
}
#endif

#endif // TIMER_LM3S_H_
/** @} ingroup	 lm3s_timer */
