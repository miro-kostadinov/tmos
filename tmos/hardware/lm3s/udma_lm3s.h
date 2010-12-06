/**************************************************************************//**
 * @ingroup	 lm3s_udma
 * @{
 * @file     udma_lm3s.h
 * @brief    UDMA header
 * @version  V3.00
 * @date     18. March 2010
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/

#ifndef UDMA_LM3S_H_
#define UDMA_LM3S_H_

#include <tmos_types.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*----------       DMA Controller (UDMA)                          -----------*/
// <g> DMA Controller (UDMA)
struct UDMA_Type
{
	__I  uint32_t STAT;             //!< DMA Status
	__O  uint32_t CFG;              //!< DMA Configuration
	__IO uint32_t CTLBASE;          //!< DMA Channel Control Base Pointer
	__I  uint32_t ALTBASE;          //!< DMA Alternate Channel Control Base Pointer
	__I  uint32_t WAITSTAT;         //!< DMA Channel Wait on Request Status
	__O  uint32_t SWREQ;            //!< DMA Channel Software Request
	__IO uint32_t USEBURSTSET;      //!< DMA Channel Useburst Set
	__O  uint32_t USEBURSTCLR;      //!< DMA Channel Useburst Clear
	__IO uint32_t REQMASKSET;       //!< DMA Channel Request Mask Set
	__O  uint32_t REQMASKCLR;       //!< DMA Channel Request Mask Clear
	__IO uint32_t ENASET;           //!< DMA Channel Enable Set
	__O  uint32_t ENACLR;           //!< DMA Channel Enable Clear
	__IO uint32_t ALTSET;           //!< DMA Channel Primary Alternate Set
	__O  uint32_t ALTCLR;           //!< DMA Channel Primary Alternate Clear
	__IO uint32_t PRIOSET;          //!< DMA Channel Priority Set
	__O  uint32_t PRIOCLR;          //!< DMA Channel Priority Clear
		 uint8_t  RESERVED1[12];	//!< doc!
	__IO uint32_t ERRCLR;           //!< DMA Bus Error Clear
		 uint8_t  RESERVED2[1200];	//!< doc!
	__IO uint32_t CHALT;            //!< DMA Channel Alternate Select

	void uDMAEnable(void);
	void uDMADisable(void);
	unsigned long uDMAErrorStatusGet(void);
	void uDMAErrorStatusClear(void);
	void uDMAChannelEnable(unsigned long ulChannel);
	void uDMAChannelDisable(unsigned long ulChannel);
	int uDMAChannelIsEnabled(unsigned long ulChannel);
	void uDMAControlBaseSet(void *pControlTable);
	void *uDMAControlBaseGet(void);
	void uDMAChannelRequest(unsigned long ulChannel);
	void uDMAChannelAttributeEnable(unsigned long ulChannel, unsigned long ulAttr);
	void uDMAChannelAttributeDisable(unsigned long ulChannel, unsigned long ulAttr);
	unsigned long uDMAChannelAttributeGet(unsigned long ulChannel);
	void uDMAChannelControlSet(unsigned long ulChannel, unsigned long ulControl);
	void uDMAChannelTransferSet(unsigned long ulChannel, unsigned long ulMode,
									   void *pvSrcAddr,
	                                   void *pvDstAddr,
	                                   unsigned long ulTransferSize);
	unsigned long uDMAChannelSizeGet(unsigned long ulChannel);
	unsigned long uDMAChannelModeGet(unsigned long ulChannel);
	void uDMAChannelSelectDefault(unsigned long ulDefPeriphs);
	void uDMAChannelSelectSecondary(unsigned long ulSecPeriphs);
	void * uDMAChannelDestPtrGet(unsigned long ulChannel);
};
// </g>

/*----------       DMA Channel Control Structure (UDMA_CTRL)      -----------*/
// <g> DMA Channel Control Structure (UDMA_CTRL)
typedef struct
{
	__IO uint32_t SRCENDP;          //!< DMA Channel Source Address End Pointer
	__IO uint32_t DSTENDP;          //!< DMA Channel Destination Address End Pointer
	__IO uint32_t CHCTL;            //!< DMA Channel Control Word
	__IO uint32_t unused;			//!< UNUSED
}UDMA_CTRL_Type;
// </g>
/*
typedef struct
{
    // The ending source address of the data transfer.
    volatile void *pvSrcEndAddr;
    // The ending destination address of the data transfer.
    volatile void *pvDstEndAddr;
    // The channel control mode.
    volatile unsigned long ulControl;
    // An unused location.
    volatile unsigned long ulSpare;
}
tDMAControlTable;
*/
//*****************************************************************************
//
// Flags that can be passed to uDMAChannelAttributeEnable(),
// uDMAChannelAttributeDisable(), and returned from uDMAChannelAttributeGet().
//
//*****************************************************************************
#define UDMA_ATTR_USEBURST      0x00000001	//!< doc!
#define UDMA_ATTR_ALTSELECT     0x00000002	//!< doc!
#define UDMA_ATTR_HIGH_PRIORITY 0x00000004	//!< doc!
#define UDMA_ATTR_REQMASK       0x00000008	//!< doc!
#define UDMA_ATTR_ALL           0x0000000F	//!< doc!

//*****************************************************************************
//
// DMA control modes that can be passed to uDMAModeSet() and returned
// uDMAModeGet().
//
//*****************************************************************************
#define UDMA_MODE_STOP          0x00000000	//!< doc!
#define UDMA_MODE_BASIC         0x00000001	//!< doc!
#define UDMA_MODE_AUTO          0x00000002	//!< doc!
#define UDMA_MODE_PINGPONG      0x00000003	//!< doc!
#define UDMA_MODE_MEM_SCATTER_GATHER                                          \
                                0x00000004	//!< doc!
#define UDMA_MODE_PER_SCATTER_GATHER                                          \
                                0x00000006	//!< doc!
#define UDMA_MODE_ALT_SELECT    0x00000001	//!< doc!

//*****************************************************************************
//
// Channel configuration values that can be passed to uDMAControlSet().
//
//*****************************************************************************
#define UDMA_DST_INC_8          0x00000000	//!< doc!
#define UDMA_DST_INC_16         0x40000000	//!< doc!
#define UDMA_DST_INC_32         0x80000000	//!< doc!
#define UDMA_DST_INC_NONE       0xc0000000	//!< doc!
#define UDMA_SRC_INC_8          0x00000000	//!< doc!
#define UDMA_SRC_INC_16         0x04000000	//!< doc!
#define UDMA_SRC_INC_32         0x08000000	//!< doc!
#define UDMA_SRC_INC_NONE       0x0c000000	//!< doc!
#define UDMA_SIZE_8             0x00000000	//!< doc!
#define UDMA_SIZE_16            0x11000000	//!< doc!
#define UDMA_SIZE_32            0x22000000	//!< doc!
#define UDMA_ARB_1              0x00000000	//!< doc!
#define UDMA_ARB_2              0x00004000	//!< doc!
#define UDMA_ARB_4              0x00008000	//!< doc!
#define UDMA_ARB_8              0x0000c000	//!< doc!
#define UDMA_ARB_16             0x00010000	//!< doc!
#define UDMA_ARB_32             0x00014000	//!< doc!
#define UDMA_ARB_64             0x00018000	//!< doc!
#define UDMA_ARB_128            0x0001c000	//!< doc!
#define UDMA_ARB_256            0x00020000	//!< doc!
#define UDMA_ARB_512            0x00024000	//!< doc!
#define UDMA_ARB_1024           0x00028000	//!< doc!
#define UDMA_NEXT_USEBURST      0x00000008	//!< doc!

//*****************************************************************************
//
// Channel numbers to be passed to API functions that require a channel number
// ID.
//
//*****************************************************************************
#define UDMA_CHANNEL_USBEP1RX   0	//!< doc!
#define UDMA_CHANNEL_USBEP1TX   1	//!< doc!
#define UDMA_CHANNEL_USBEP2RX   2	//!< doc!
#define UDMA_CHANNEL_USBEP2TX   3	//!< doc!
#define UDMA_CHANNEL_USBEP3RX   4	//!< doc!
#define UDMA_CHANNEL_USBEP3TX   5	//!< doc!
#define UDMA_CHANNEL_ETH0RX     6	//!< doc!
#define UDMA_CHANNEL_ETH0TX     7	//!< doc!
#define UDMA_CHANNEL_UART0RX    8	//!< doc!
#define UDMA_CHANNEL_UART0TX    9	//!< doc!
#define UDMA_CHANNEL_SSI0RX     10	//!< doc!
#define UDMA_CHANNEL_SSI0TX     11	//!< doc!
#define UDMA_CHANNEL_UART2RX	12 //!< alternate
#define UDMA_CHANNEL_UART2TX	13 //!< alternate
#define UDMA_CHANNEL_ADC0       14	//!< doc!
#define UDMA_CHANNEL_ADC1       15	//!< doc!
#define UDMA_CHANNEL_ADC2       16	//!< doc!
#define UDMA_CHANNEL_ADC3       17	//!< doc!
#define UDMA_CHANNEL_TMR0A      18	//!< doc!
#define UDMA_CHANNEL_TMR0B      19	//!< doc!
#define UDMA_CHANNEL_TMR1A      20	//!< doc!
#define UDMA_CHANNEL_TMR1B      21	//!< doc!
#define UDMA_CHANNEL_UART1RX    22	//!< doc!
#define UDMA_CHANNEL_UART1TX    23	//!< doc!
#define UDMA_CHANNEL_SSI1RX     24	//!< doc!
#define UDMA_CHANNEL_SSI1TX     25	//!< doc!
#define UDMA_CHANNEL_I2S0RX     28	//!< doc!
#define UDMA_CHANNEL_I2S0TX     29	//!< doc!
#define UDMA_CHANNEL_SW         30	//!< doc!

//*****************************************************************************
//
// Flags to be OR'd with the channel ID to indicate if the primary or alternate
// control structure should be used.
//
//*****************************************************************************
#define UDMA_PRI_SELECT         0x00000000	//!< doc!
#define UDMA_ALT_SELECT         0x00000020	//!< doc!

//*****************************************************************************
//
// uDMA interrupt sources, to be passed to uDMAIntRegister() and
// uDMAIntUnregister().
//
//*****************************************************************************
#define UDMA_INT_SW             62	//!< doc!
#define UDMA_INT_ERR            63	//!< doc!

//*****************************************************************************
//
// Channel numbers to be passed to API functions that require a channel number
// ID.  These are for secondary peripheral assignments.
//
//*****************************************************************************
#define UDMA_SEC_CHANNEL_UART2RX_0	0	//!< doc!
#define UDMA_SEC_CHANNEL_UART2TX_1  1	//!< doc!
#define UDMA_SEC_CHANNEL_TMR3A  	2	//!< doc!
#define UDMA_SEC_CHANNEL_TMR3B  	3	//!< doc!
#define UDMA_SEC_CHANNEL_TMR2A_4    4	//!< doc!
#define UDMA_SEC_CHANNEL_TMR2B_5    5	//!< doc!
#define UDMA_SEC_CHANNEL_TMR2A_6    6	//!< doc!
#define UDMA_SEC_CHANNEL_TMR2B_7    7	//!< doc!
#define UDMA_SEC_CHANNEL_UART1RX    8	//!< doc!
#define UDMA_SEC_CHANNEL_UART1TX    9	//!< doc!
#define UDMA_SEC_CHANNEL_SSI1RX 	10	//!< doc!
#define UDMA_SEC_CHANNEL_SSI1TX 	11	//!< doc!
#define UDMA_SEC_CHANNEL_UART2RX_12 12	//!< doc!
#define UDMA_SEC_CHANNEL_UART2TX_13 13	//!< doc!
#define UDMA_SEC_CHANNEL_TMR2A_14   14	//!< doc!
#define UDMA_SEC_CHANNEL_TMR2B_15   15	//!< doc!
#define UDMA_SEC_CHANNEL_TMR1A  	18	//!< doc!
#define UDMA_SEC_CHANNEL_TMR1B  	19	//!< doc!
#define UDMA_SEC_CHANNEL_EPI0RX 	20	//!< doc!
#define UDMA_SEC_CHANNEL_EPI0TX 	21	//!< doc!
#define UDMA_SEC_CHANNEL_ADC10  	24	//!< doc!
#define UDMA_SEC_CHANNEL_ADC11  	25	//!< doc!
#define UDMA_SEC_CHANNEL_ADC12  	26	//!< doc!
#define UDMA_SEC_CHANNEL_ADC13  	27	//!< doc!
#define UDMA_SEC_CHANNEL_SW     	30	//!< doc!

//*****************************************************************************
//
// uDMA default/secondary peripheral selections, to be passed to
// uDMAChannelSelectSecondary() and uDMAChannelSelectDefault().
//
//*****************************************************************************
#define UDMA_DEF_USBEP1RX_SEC_UART2RX                                         \
                                0x00000001	//!< doc!
#define UDMA_DEF_USBEP1TX_SEC_UART2TX                                         \
                                0x00000002	//!< doc!
#define UDMA_DEF_USBEP2RX_SEC_TMR3A                                           \
                                0x00000004	//!< doc!
#define UDMA_DEF_USBEP2TX_SEC_TMR3B                                           \
                                0x00000008	//!< doc!
#define UDMA_DEF_USBEP3RX_SEC_TMR2A                                           \
                                0x00000010	//!< doc!
#define UDMA_DEF_USBEP3TX_SEC_TMR2B                                           \
                                0x00000020	//!< doc!
#define UDMA_DEF_ETH0RX_SEC_TMR2A                                             \
                                0x00000040	//!< doc!
#define UDMA_DEF_ETH0TX_SEC_TMR2B                                             \
                                0x00000080	//!< doc!
#define UDMA_DEF_UART0RX_SEC_UART1RX                                          \
                                0x00000100	//!< doc!
#define UDMA_DEF_UART0TX_SEC_UART1TX                                          \
                                0x00000200	//!< doc!
#define UDMA_DEF_SSI0RX_SEC_SSI1RX                                            \
                                0x00000400	//!< doc!
#define UDMA_DEF_SSI0TX_SEC_SSI1TX                                            \
                                0x00000800	//!< doc!
#define UDMA_DEF_RESERVED_SEC_UART2RX                                         \
                                0x00001000	//!< doc!
#define UDMA_DEF_RESERVED_SEC_UART2TX                                         \
                                0x00002000	//!< doc!
#define UDMA_DEF_ADC00_SEC_TMR2A                                              \
                                0x00004000	//!< doc!
#define UDMA_DEF_ADC01_SEC_TMR2B                                              \
                                0x00008000	//!< doc!
#define UDMA_DEF_ADC02_SEC_RESERVED                                           \
                                0x00010000	//!< doc!
#define UDMA_DEF_ADC03_SEC_RESERVED                                           \
                                0x00020000	//!< doc!
#define UDMA_DEF_TMR0A_SEC_TMR1A                                              \
                                0x00040000	//!< doc!
#define UDMA_DEF_TMR0B_SEC_TMR1B                                              \
                                0x00080000	//!< doc!
#define UDMA_DEF_TMR1A_SEC_EPI0RX                                             \
                                0x00100000	//!< doc!
#define UDMA_DEF_TMR1B_SEC_EPI0TX                                             \
                                0x00200000	//!< doc!
#define UDMA_DEF_UART1RX_SEC_RESERVED                                         \
                                0x00400000	//!< doc!
#define UDMA_DEF_UART1TX_SEC_RESERVED                                         \
                                0x00800000	//!< doc!
#define UDMA_DEF_SSI1RX_SEC_ADC10                                             \
                                0x01000000	//!< doc!
#define UDMA_DEF_SSI1TX_SEC_ADC11                                             \
                                0x02000000	//!< doc!
#define UDMA_DEF_RESERVED_SEC_ADC12                                           \
                                0x04000000	//!< doc!
#define UDMA_DEF_RESERVED_SEC_ADC13                                           \
                                0x08000000	//!< doc!
#define UDMA_DEF_I2S0RX_SEC_RESERVED                                          \
                                0x10000000	//!< doc!
#define UDMA_DEF_I2S0TX_SEC_RESERVED                                          \
                                0x20000000	//!< doc!

#define UDMA     ((UDMA_Type *)UDMA_BASE)	//!< doc!

#ifdef __cplusplus
}
#endif

#endif //  UDMA_LM3S_H_
/** @} ingroup	 lm3s_udma */
