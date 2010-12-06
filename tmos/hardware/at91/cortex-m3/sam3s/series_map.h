/**************************************************************************//**
 * @ingroup	 hardware_sam3s
 * @{
 * @file     hardware/at91/cortex-m3/sam3s/series_map.h
 * @brief    SAM3S series CPP header
 * @version  V3.00
 * @date     28. November 2010
 * @author	 Miroslav Kostadinov
 *
 * @}
 *
 ******************************************************************************/

#ifndef SERIES_MAP_H_
#define SERIES_MAP_H_

/* ************************************************************************** */
/*   REGISTER ACCESS DEFINITIONS FOR SAM3S */
/* ************************************************************************** */
/**
 * @addtogroup SAM3S_reg Registers Access Definitions
 * @ingroup	 hardware_sam3s
 * @{
 */

/* ========== Register definition for HSMCI peripheral ========== */
#define REG_HSMCI_CR        REG_ACCESS(WoReg, 0x40000000U) //!< (HSMCI) Control Register */
#define REG_HSMCI_MR        REG_ACCESS(RwReg, 0x40000004U) //!< (HSMCI) Mode Register */
#define REG_HSMCI_DTOR      REG_ACCESS(RwReg, 0x40000008U) //!< (HSMCI) Data Timeout Register */
#define REG_HSMCI_SDCR      REG_ACCESS(RwReg, 0x4000000CU) //!< (HSMCI) SD/SDIO Card Register */
#define REG_HSMCI_ARGR      REG_ACCESS(RwReg, 0x40000010U) //!< (HSMCI) Argument Register */
#define REG_HSMCI_CMDR      REG_ACCESS(WoReg, 0x40000014U) //!< (HSMCI) Command Register */
#define REG_HSMCI_BLKR      REG_ACCESS(RwReg, 0x40000018U) //!< (HSMCI) Block Register */
#define REG_HSMCI_CSTOR     REG_ACCESS(RwReg, 0x4000001CU) //!< (HSMCI) Completion Signal Timeout Register */
#define REG_HSMCI_RSPR      REG_ACCESS(RoReg, 0x40000020U) //!< (HSMCI) Response Register */
#define REG_HSMCI_RDR       REG_ACCESS(RoReg, 0x40000030U) //!< (HSMCI) Receive Data Register */
#define REG_HSMCI_TDR       REG_ACCESS(WoReg, 0x40000034U) //!< (HSMCI) Transmit Data Register */
#define REG_HSMCI_SR        REG_ACCESS(RoReg, 0x40000040U) //!< (HSMCI) Status Register */
#define REG_HSMCI_IER       REG_ACCESS(WoReg, 0x40000044U) //!< (HSMCI) Interrupt Enable Register */
#define REG_HSMCI_IDR       REG_ACCESS(WoReg, 0x40000048U) //!< (HSMCI) Interrupt Disable Register */
#define REG_HSMCI_IMR       REG_ACCESS(RoReg, 0x4000004CU) //!< (HSMCI) Interrupt Mask Register */
#define REG_HSMCI_CFG       REG_ACCESS(RwReg, 0x40000054U) //!< (HSMCI) Configuration Register */
#define REG_HSMCI_WPMR      REG_ACCESS(RwReg, 0x400000E4U) //!< (HSMCI) Write Protection Mode Register */
#define REG_HSMCI_WPSR      REG_ACCESS(RoReg, 0x400000E8U) //!< (HSMCI) Write Protection Status Register */
#define REG_HSMCI_RPR       REG_ACCESS(RwReg, 0x40000100U) //!< (HSMCI) Receive Pointer Register */
#define REG_HSMCI_RCR       REG_ACCESS(RwReg, 0x40000104U) //!< (HSMCI) Receive Counter Register */
#define REG_HSMCI_TPR       REG_ACCESS(RwReg, 0x40000108U) //!< (HSMCI) Transmit Pointer Register */
#define REG_HSMCI_TCR       REG_ACCESS(RwReg, 0x4000010CU) //!< (HSMCI) Transmit Counter Register */
#define REG_HSMCI_RNPR      REG_ACCESS(RwReg, 0x40000110U) //!< (HSMCI) Receive Next Pointer Register */
#define REG_HSMCI_RNCR      REG_ACCESS(RwReg, 0x40000114U) //!< (HSMCI) Receive Next Counter Register */
#define REG_HSMCI_TNPR      REG_ACCESS(RwReg, 0x40000118U) //!< (HSMCI) Transmit Next Pointer Register */
#define REG_HSMCI_TNCR      REG_ACCESS(RwReg, 0x4000011CU) //!< (HSMCI) Transmit Next Counter Register */
#define REG_HSMCI_PTCR      REG_ACCESS(WoReg, 0x40000120U) //!< (HSMCI) Transfer Control Register */
#define REG_HSMCI_PTSR      REG_ACCESS(RoReg, 0x40000124U) //!< (HSMCI) Transfer Status Register */
#define REG_HSMCI_FIFO      REG_ACCESS(RwReg, 0x40000200U) //!< (HSMCI) FIFO Memory Aperture0 */

/* ========== Register definition for SSC peripheral ========== */
#define REG_SSC_CR          REG_ACCESS(WoReg, 0x40004000U) //!< (SSC) Control Register
#define REG_SSC_CMR         REG_ACCESS(RwReg, 0x40004004U) //!< (SSC) Clock Mode Register
#define REG_SSC_RCMR        REG_ACCESS(RwReg, 0x40004010U) //!< (SSC) Receive Clock Mode Register
#define REG_SSC_RFMR        REG_ACCESS(RwReg, 0x40004014U) //!< (SSC) Receive Frame Mode Register
#define REG_SSC_TCMR        REG_ACCESS(RwReg, 0x40004018U) //!< (SSC) Transmit Clock Mode Register
#define REG_SSC_TFMR        REG_ACCESS(RwReg, 0x4000401CU) //!< (SSC) Transmit Frame Mode Register
#define REG_SSC_RHR         REG_ACCESS(RoReg, 0x40004020U) //!< (SSC) Receive Holding Register
#define REG_SSC_THR         REG_ACCESS(WoReg, 0x40004024U) //!< (SSC) Transmit Holding Register
#define REG_SSC_RSHR        REG_ACCESS(RoReg, 0x40004030U) //!< (SSC) Receive Sync. Holding Register
#define REG_SSC_TSHR        REG_ACCESS(RwReg, 0x40004034U) //!< (SSC) Transmit Sync. Holding Register
#define REG_SSC_RC0R        REG_ACCESS(RwReg, 0x40004038U) //!< (SSC) Receive Compare 0 Register
#define REG_SSC_RC1R        REG_ACCESS(RwReg, 0x4000403CU) //!< (SSC) Receive Compare 1 Register
#define REG_SSC_SR          REG_ACCESS(RoReg, 0x40004040U) //!< (SSC) Status Register
#define REG_SSC_IER         REG_ACCESS(WoReg, 0x40004044U) //!< (SSC) Interrupt Enable Register
#define REG_SSC_IDR         REG_ACCESS(WoReg, 0x40004048U) //!< (SSC) Interrupt Disable Register
#define REG_SSC_IMR         REG_ACCESS(RoReg, 0x4000404CU) //!< (SSC) Interrupt Mask Register
#define REG_SSC_WPMR        REG_ACCESS(RwReg, 0x400040E4U) //!< (SSC) Write Protect Mode Register
#define REG_SSC_WPSR        REG_ACCESS(RoReg, 0x400040E8U) //!< (SSC) Write Protect Status Register
#define REG_SSC_RPR         REG_ACCESS(RwReg, 0x40004100U) //!< (SSC) Receive Pointer Register
#define REG_SSC_RCR         REG_ACCESS(RwReg, 0x40004104U) //!< (SSC) Receive Counter Register
#define REG_SSC_TPR         REG_ACCESS(RwReg, 0x40004108U) //!< (SSC) Transmit Pointer Register
#define REG_SSC_TCR         REG_ACCESS(RwReg, 0x4000410CU) //!< (SSC) Transmit Counter Register
#define REG_SSC_RNPR        REG_ACCESS(RwReg, 0x40004110U) //!< (SSC) Receive Next Pointer Register
#define REG_SSC_RNCR        REG_ACCESS(RwReg, 0x40004114U) //!< (SSC) Receive Next Counter Register
#define REG_SSC_TNPR        REG_ACCESS(RwReg, 0x40004118U) //!< (SSC) Transmit Next Pointer Register
#define REG_SSC_TNCR        REG_ACCESS(RwReg, 0x4000411CU) //!< (SSC) Transmit Next Counter Register
#define REG_SSC_PTCR        REG_ACCESS(WoReg, 0x40004120U) //!< (SSC) Transfer Control Register
#define REG_SSC_PTSR        REG_ACCESS(RoReg, 0x40004124U) //!< (SSC) Transfer Status Register

/* ========== Register definition for SPI peripheral ========== */
#define REG_SPI_CR          REG_ACCESS(WoReg, 0x40008000U) //!< (SPI) Control Register */
#define REG_SPI_MR          REG_ACCESS(RwReg, 0x40008004U) //!< (SPI) Mode Register */
#define REG_SPI_RDR         REG_ACCESS(RoReg, 0x40008008U) //!< (SPI) Receive Data Register */
#define REG_SPI_TDR         REG_ACCESS(WoReg, 0x4000800CU) //!< (SPI) Transmit Data Register */
#define REG_SPI_SR          REG_ACCESS(RoReg, 0x40008010U) //!< (SPI) Status Register */
#define REG_SPI_IER         REG_ACCESS(WoReg, 0x40008014U) //!< (SPI) Interrupt Enable Register */
#define REG_SPI_IDR         REG_ACCESS(WoReg, 0x40008018U) //!< (SPI) Interrupt Disable Register */
#define REG_SPI_IMR         REG_ACCESS(RoReg, 0x4000801CU) //!< (SPI) Interrupt Mask Register */
#define REG_SPI_CSR         REG_ACCESS(RwReg, 0x40008030U) //!< (SPI) Chip Select Register */
#define REG_SPI_WPMR        REG_ACCESS(RwReg, 0x400080E4U) //!< (SPI) Write Protection Control Register */
#define REG_SPI_WPSR        REG_ACCESS(RoReg, 0x400080E8U) //!< (SPI) Write Protection Status Register */
#define REG_SPI_RPR         REG_ACCESS(RwReg, 0x40008100U) //!< (SPI) Receive Pointer Register */
#define REG_SPI_RCR         REG_ACCESS(RwReg, 0x40008104U) //!< (SPI) Receive Counter Register */
#define REG_SPI_TPR         REG_ACCESS(RwReg, 0x40008108U) //!< (SPI) Transmit Pointer Register */
#define REG_SPI_TCR         REG_ACCESS(RwReg, 0x4000810CU) //!< (SPI) Transmit Counter Register */
#define REG_SPI_RNPR        REG_ACCESS(RwReg, 0x40008110U) //!< (SPI) Receive Next Pointer Register */
#define REG_SPI_RNCR        REG_ACCESS(RwReg, 0x40008114U) //!< (SPI) Receive Next Counter Register */
#define REG_SPI_TNPR        REG_ACCESS(RwReg, 0x40008118U) //!< (SPI) Transmit Next Pointer Register */
#define REG_SPI_TNCR        REG_ACCESS(RwReg, 0x4000811CU) //!< (SPI) Transmit Next Counter Register */
#define REG_SPI_PTCR        REG_ACCESS(WoReg, 0x40008120U) //!< (SPI) Transfer Control Register */
#define REG_SPI_PTSR        REG_ACCESS(RoReg, 0x40008124U) //!< (SPI) Transfer Status Register */

/* ========== Register definition for TC0 peripheral ========== */
#define REG_TC0_CCR0        REG_ACCESS(WoReg, 0x40010000U) //!< (TC0) Channel Control Register (channel = 0) */
#define REG_TC0_CMR0        REG_ACCESS(RwReg, 0x40010004U) //!< (TC0) Channel Mode Register (channel = 0) */
#define REG_TC0_SMMR0       REG_ACCESS(RwReg, 0x40010008U) //!< (TC0) Stepper Motor Mode Register (channel = 0) */
#define REG_TC0_CV0         REG_ACCESS(RoReg, 0x40010010U) //!< (TC0) Counter Value (channel = 0) */
#define REG_TC0_RA0         REG_ACCESS(RwReg, 0x40010014U) //!< (TC0) Register A (channel = 0) */
#define REG_TC0_RB0         REG_ACCESS(RwReg, 0x40010018U) //!< (TC0) Register B (channel = 0) */
#define REG_TC0_RC0         REG_ACCESS(RwReg, 0x4001001CU) //!< (TC0) Register C (channel = 0) */
#define REG_TC0_SR0         REG_ACCESS(RoReg, 0x40010020U) //!< (TC0) Status Register (channel = 0) */
#define REG_TC0_IER0        REG_ACCESS(WoReg, 0x40010024U) //!< (TC0) Interrupt Enable Register (channel = 0) */
#define REG_TC0_IDR0        REG_ACCESS(WoReg, 0x40010028U) //!< (TC0) Interrupt Disable Register (channel = 0) */
#define REG_TC0_IMR0        REG_ACCESS(RoReg, 0x4001002CU) //!< (TC0) Interrupt Mask Register (channel = 0) */
#define REG_TC0_CCR1        REG_ACCESS(WoReg, 0x40010040U) //!< (TC0) Channel Control Register (channel = 1) */
#define REG_TC0_CMR1        REG_ACCESS(RwReg, 0x40010044U) //!< (TC0) Channel Mode Register (channel = 1) */
#define REG_TC0_SMMR1       REG_ACCESS(RwReg, 0x40010048U) //!< (TC0) Stepper Motor Mode Register (channel = 1) */
#define REG_TC0_CV1         REG_ACCESS(RoReg, 0x40010050U) //!< (TC0) Counter Value (channel = 1) */
#define REG_TC0_RA1         REG_ACCESS(RwReg, 0x40010054U) //!< (TC0) Register A (channel = 1) */
#define REG_TC0_RB1         REG_ACCESS(RwReg, 0x40010058U) //!< (TC0) Register B (channel = 1) */
#define REG_TC0_RC1         REG_ACCESS(RwReg, 0x4001005CU) //!< (TC0) Register C (channel = 1) */
#define REG_TC0_SR1         REG_ACCESS(RoReg, 0x40010060U) //!< (TC0) Status Register (channel = 1) */
#define REG_TC0_IER1        REG_ACCESS(WoReg, 0x40010064U) //!< (TC0) Interrupt Enable Register (channel = 1) */
#define REG_TC0_IDR1        REG_ACCESS(WoReg, 0x40010068U) //!< (TC0) Interrupt Disable Register (channel = 1) */
#define REG_TC0_IMR1        REG_ACCESS(RoReg, 0x4001006CU) //!< (TC0) Interrupt Mask Register (channel = 1) */
#define REG_TC0_CCR2        REG_ACCESS(WoReg, 0x40010080U) //!< (TC0) Channel Control Register (channel = 2) */
#define REG_TC0_CMR2        REG_ACCESS(RwReg, 0x40010084U) //!< (TC0) Channel Mode Register (channel = 2) */
#define REG_TC0_SMMR2       REG_ACCESS(RwReg, 0x40010088U) //!< (TC0) Stepper Motor Mode Register (channel = 2) */
#define REG_TC0_CV2         REG_ACCESS(RoReg, 0x40010090U) //!< (TC0) Counter Value (channel = 2) */
#define REG_TC0_RA2         REG_ACCESS(RwReg, 0x40010094U) //!< (TC0) Register A (channel = 2) */
#define REG_TC0_RB2         REG_ACCESS(RwReg, 0x40010098U) //!< (TC0) Register B (channel = 2) */
#define REG_TC0_RC2         REG_ACCESS(RwReg, 0x4001009CU) //!< (TC0) Register C (channel = 2) */
#define REG_TC0_SR2         REG_ACCESS(RoReg, 0x400100A0U) //!< (TC0) Status Register (channel = 2) */
#define REG_TC0_IER2        REG_ACCESS(WoReg, 0x400100A4U) //!< (TC0) Interrupt Enable Register (channel = 2) */
#define REG_TC0_IDR2        REG_ACCESS(WoReg, 0x400100A8U) //!< (TC0) Interrupt Disable Register (channel = 2) */
#define REG_TC0_IMR2        REG_ACCESS(RoReg, 0x400100ACU) //!< (TC0) Interrupt Mask Register (channel = 2) */
#define REG_TC0_BCR         REG_ACCESS(WoReg, 0x400100C0U) //!< (TC0) Block Control Register */
#define REG_TC0_BMR         REG_ACCESS(RwReg, 0x400100C4U) //!< (TC0) Block Mode Register */
#define REG_TC0_QIER        REG_ACCESS(WoReg, 0x400100C8U) //!< (TC0) QDEC Interrupt Enable Register */
#define REG_TC0_QIDR        REG_ACCESS(WoReg, 0x400100CCU) //!< (TC0) QDEC Interrupt Disable Register */
#define REG_TC0_QIMR        REG_ACCESS(RoReg, 0x400100D0U) //!< (TC0) QDEC Interrupt Mask Register */
#define REG_TC0_QISR        REG_ACCESS(RoReg, 0x400100D4U) //!< (TC0) QDEC Interrupt Status Register */
#define REG_TC0_FMR         REG_ACCESS(RwReg, 0x400100D8U) //!< (TC0) Fault Mode Register */
#define REG_TC0_WPMR        REG_ACCESS(RwReg, 0x400100E4U) //!< (TC0) Write Protect Mode Register */
/* ========== Register definition for TC1 peripheral ========== */
#define REG_TC1_CCR0        REG_ACCESS(WoReg, 0x40014000U) //!< (TC1) Channel Control Register (channel = 0) */
#define REG_TC1_CMR0        REG_ACCESS(RwReg, 0x40014004U) //!< (TC1) Channel Mode Register (channel = 0) */
#define REG_TC1_SMMR0       REG_ACCESS(RwReg, 0x40014008U) //!< (TC1) Stepper Motor Mode Register (channel = 0) */
#define REG_TC1_CV0         REG_ACCESS(RoReg, 0x40014010U) //!< (TC1) Counter Value (channel = 0) */
#define REG_TC1_RA0         REG_ACCESS(RwReg, 0x40014014U) //!< (TC1) Register A (channel = 0) */
#define REG_TC1_RB0         REG_ACCESS(RwReg, 0x40014018U) //!< (TC1) Register B (channel = 0) */
#define REG_TC1_RC0         REG_ACCESS(RwReg, 0x4001401CU) //!< (TC1) Register C (channel = 0) */
#define REG_TC1_SR0         REG_ACCESS(RoReg, 0x40014020U) //!< (TC1) Status Register (channel = 0) */
#define REG_TC1_IER0        REG_ACCESS(WoReg, 0x40014024U) //!< (TC1) Interrupt Enable Register (channel = 0) */
#define REG_TC1_IDR0        REG_ACCESS(WoReg, 0x40014028U) //!< (TC1) Interrupt Disable Register (channel = 0) */
#define REG_TC1_IMR0        REG_ACCESS(RoReg, 0x4001402CU) //!< (TC1) Interrupt Mask Register (channel = 0) */
#define REG_TC1_CCR1        REG_ACCESS(WoReg, 0x40014040U) //!< (TC1) Channel Control Register (channel = 1) */
#define REG_TC1_CMR1        REG_ACCESS(RwReg, 0x40014044U) //!< (TC1) Channel Mode Register (channel = 1) */
#define REG_TC1_SMMR1       REG_ACCESS(RwReg, 0x40014048U) //!< (TC1) Stepper Motor Mode Register (channel = 1) */
#define REG_TC1_CV1         REG_ACCESS(RoReg, 0x40014050U) //!< (TC1) Counter Value (channel = 1) */
#define REG_TC1_RA1         REG_ACCESS(RwReg, 0x40014054U) //!< (TC1) Register A (channel = 1) */
#define REG_TC1_RB1         REG_ACCESS(RwReg, 0x40014058U) //!< (TC1) Register B (channel = 1) */
#define REG_TC1_RC1         REG_ACCESS(RwReg, 0x4001405CU) //!< (TC1) Register C (channel = 1) */
#define REG_TC1_SR1         REG_ACCESS(RoReg, 0x40014060U) //!< (TC1) Status Register (channel = 1) */
#define REG_TC1_IER1        REG_ACCESS(WoReg, 0x40014064U) //!< (TC1) Interrupt Enable Register (channel = 1) */
#define REG_TC1_IDR1        REG_ACCESS(WoReg, 0x40014068U) //!< (TC1) Interrupt Disable Register (channel = 1) */
#define REG_TC1_IMR1        REG_ACCESS(RoReg, 0x4001406CU) //!< (TC1) Interrupt Mask Register (channel = 1) */
#define REG_TC1_CCR2        REG_ACCESS(WoReg, 0x40014080U) //!< (TC1) Channel Control Register (channel = 2) */
#define REG_TC1_CMR2        REG_ACCESS(RwReg, 0x40014084U) //!< (TC1) Channel Mode Register (channel = 2) */
#define REG_TC1_SMMR2       REG_ACCESS(RwReg, 0x40014088U) //!< (TC1) Stepper Motor Mode Register (channel = 2) */
#define REG_TC1_CV2         REG_ACCESS(RoReg, 0x40014090U) //!< (TC1) Counter Value (channel = 2) */
#define REG_TC1_RA2         REG_ACCESS(RwReg, 0x40014094U) //!< (TC1) Register A (channel = 2) */
#define REG_TC1_RB2         REG_ACCESS(RwReg, 0x40014098U) //!< (TC1) Register B (channel = 2) */
#define REG_TC1_RC2         REG_ACCESS(RwReg, 0x4001409CU) //!< (TC1) Register C (channel = 2) */
#define REG_TC1_SR2         REG_ACCESS(RoReg, 0x400140A0U) //!< (TC1) Status Register (channel = 2) */
#define REG_TC1_IER2        REG_ACCESS(WoReg, 0x400140A4U) //!< (TC1) Interrupt Enable Register (channel = 2) */
#define REG_TC1_IDR2        REG_ACCESS(WoReg, 0x400140A8U) //!< (TC1) Interrupt Disable Register (channel = 2) */
#define REG_TC1_IMR2        REG_ACCESS(RoReg, 0x400140ACU) //!< (TC1) Interrupt Mask Register (channel = 2) */
#define REG_TC1_BCR         REG_ACCESS(WoReg, 0x400140C0U) //!< (TC1) Block Control Register */
#define REG_TC1_BMR         REG_ACCESS(RwReg, 0x400140C4U) //!< (TC1) Block Mode Register */
#define REG_TC1_QIER        REG_ACCESS(WoReg, 0x400140C8U) //!< (TC1) QDEC Interrupt Enable Register */
#define REG_TC1_QIDR        REG_ACCESS(WoReg, 0x400140CCU) //!< (TC1) QDEC Interrupt Disable Register */
#define REG_TC1_QIMR        REG_ACCESS(RoReg, 0x400140D0U) //!< (TC1) QDEC Interrupt Mask Register */
#define REG_TC1_QISR        REG_ACCESS(RoReg, 0x400140D4U) //!< (TC1) QDEC Interrupt Status Register */
#define REG_TC1_FMR         REG_ACCESS(RwReg, 0x400140D8U) //!< (TC1) Fault Mode Register */
#define REG_TC1_WPMR        REG_ACCESS(RwReg, 0x400140E4U) //!< (TC1) Write Protect Mode Register */
/* ========== Register definition for TWI0 peripheral ========== */
#define REG_TWI0_CR         REG_ACCESS(WoReg, 0x40018000U) //!< (TWI0) Control Register */
#define REG_TWI0_MMR        REG_ACCESS(RwReg, 0x40018004U) //!< (TWI0) Master Mode Register */
#define REG_TWI0_SMR        REG_ACCESS(RwReg, 0x40018008U) //!< (TWI0) Slave Mode Register */
#define REG_TWI0_IADR       REG_ACCESS(RwReg, 0x4001800CU) //!< (TWI0) Internal Address Register */
#define REG_TWI0_CWGR       REG_ACCESS(RwReg, 0x40018010U) //!< (TWI0) Clock Waveform Generator Register */
#define REG_TWI0_SR         REG_ACCESS(RoReg, 0x40018020U) //!< (TWI0) Status Register */
#define REG_TWI0_IER        REG_ACCESS(WoReg, 0x40018024U) //!< (TWI0) Interrupt Enable Register */
#define REG_TWI0_IDR        REG_ACCESS(WoReg, 0x40018028U) //!< (TWI0) Interrupt Disable Register */
#define REG_TWI0_IMR        REG_ACCESS(RoReg, 0x4001802CU) //!< (TWI0) Interrupt Mask Register */
#define REG_TWI0_RHR        REG_ACCESS(RoReg, 0x40018030U) //!< (TWI0) Receive Holding Register */
#define REG_TWI0_THR        REG_ACCESS(WoReg, 0x40018034U) //!< (TWI0) Transmit Holding Register */
#define REG_TWI0_RPR        REG_ACCESS(RwReg, 0x40018100U) //!< (TWI0) Receive Pointer Register */
#define REG_TWI0_RCR        REG_ACCESS(RwReg, 0x40018104U) //!< (TWI0) Receive Counter Register */
#define REG_TWI0_TPR        REG_ACCESS(RwReg, 0x40018108U) //!< (TWI0) Transmit Pointer Register */
#define REG_TWI0_TCR        REG_ACCESS(RwReg, 0x4001810CU) //!< (TWI0) Transmit Counter Register */
#define REG_TWI0_RNPR       REG_ACCESS(RwReg, 0x40018110U) //!< (TWI0) Receive Next Pointer Register */
#define REG_TWI0_RNCR       REG_ACCESS(RwReg, 0x40018114U) //!< (TWI0) Receive Next Counter Register */
#define REG_TWI0_TNPR       REG_ACCESS(RwReg, 0x40018118U) //!< (TWI0) Transmit Next Pointer Register */
#define REG_TWI0_TNCR       REG_ACCESS(RwReg, 0x4001811CU) //!< (TWI0) Transmit Next Counter Register */
#define REG_TWI0_PTCR       REG_ACCESS(WoReg, 0x40018120U) //!< (TWI0) Transfer Control Register */
#define REG_TWI0_PTSR       REG_ACCESS(RoReg, 0x40018124U) //!< (TWI0) Transfer Status Register */
/* ========== Register definition for TWI1 peripheral ========== */
#define REG_TWI1_CR         REG_ACCESS(WoReg, 0x4001C000U) //!< (TWI1) Control Register */
#define REG_TWI1_MMR        REG_ACCESS(RwReg, 0x4001C004U) //!< (TWI1) Master Mode Register */
#define REG_TWI1_SMR        REG_ACCESS(RwReg, 0x4001C008U) //!< (TWI1) Slave Mode Register */
#define REG_TWI1_IADR       REG_ACCESS(RwReg, 0x4001C00CU) //!< (TWI1) Internal Address Register */
#define REG_TWI1_CWGR       REG_ACCESS(RwReg, 0x4001C010U) //!< (TWI1) Clock Waveform Generator Register */
#define REG_TWI1_SR         REG_ACCESS(RoReg, 0x4001C020U) //!< (TWI1) Status Register */
#define REG_TWI1_IER        REG_ACCESS(WoReg, 0x4001C024U) //!< (TWI1) Interrupt Enable Register */
#define REG_TWI1_IDR        REG_ACCESS(WoReg, 0x4001C028U) //!< (TWI1) Interrupt Disable Register */
#define REG_TWI1_IMR        REG_ACCESS(RoReg, 0x4001C02CU) //!< (TWI1) Interrupt Mask Register */
#define REG_TWI1_RHR        REG_ACCESS(RoReg, 0x4001C030U) //!< (TWI1) Receive Holding Register */
#define REG_TWI1_THR        REG_ACCESS(WoReg, 0x4001C034U) //!< (TWI1) Transmit Holding Register */
#define REG_TWI1_RPR        REG_ACCESS(RwReg, 0x4001C100U) //!< (TWI1) Receive Pointer Register */
#define REG_TWI1_RCR        REG_ACCESS(RwReg, 0x4001C104U) //!< (TWI1) Receive Counter Register */
#define REG_TWI1_TPR        REG_ACCESS(RwReg, 0x4001C108U) //!< (TWI1) Transmit Pointer Register */
#define REG_TWI1_TCR        REG_ACCESS(RwReg, 0x4001C10CU) //!< (TWI1) Transmit Counter Register */
#define REG_TWI1_RNPR       REG_ACCESS(RwReg, 0x4001C110U) //!< (TWI1) Receive Next Pointer Register */
#define REG_TWI1_RNCR       REG_ACCESS(RwReg, 0x4001C114U) //!< (TWI1) Receive Next Counter Register */
#define REG_TWI1_TNPR       REG_ACCESS(RwReg, 0x4001C118U) //!< (TWI1) Transmit Next Pointer Register */
#define REG_TWI1_TNCR       REG_ACCESS(RwReg, 0x4001C11CU) //!< (TWI1) Transmit Next Counter Register */
#define REG_TWI1_PTCR       REG_ACCESS(WoReg, 0x4001C120U) //!< (TWI1) Transfer Control Register */
#define REG_TWI1_PTSR       REG_ACCESS(RoReg, 0x4001C124U) //!< (TWI1) Transfer Status Register */
/* ========== Register definition for PWM peripheral ========== */
#define REG_PWM_CLK         REG_ACCESS(RwReg, 0x40020000U) //!< (PWM) PWM Clock Register */
#define REG_PWM_ENA         REG_ACCESS(WoReg, 0x40020004U) //!< (PWM) PWM Enable Register */
#define REG_PWM_DIS         REG_ACCESS(WoReg, 0x40020008U) //!< (PWM) PWM Disable Register */
#define REG_PWM_SR          REG_ACCESS(RoReg, 0x4002000CU) //!< (PWM) PWM Status Register */
#define REG_PWM_IER1        REG_ACCESS(WoReg, 0x40020010U) //!< (PWM) PWM Interrupt Enable Register 1 */
#define REG_PWM_IDR1        REG_ACCESS(WoReg, 0x40020014U) //!< (PWM) PWM Interrupt Disable Register 1 */
#define REG_PWM_IMR1        REG_ACCESS(RoReg, 0x40020018U) //!< (PWM) PWM Interrupt Mask Register 1 */
#define REG_PWM_ISR1        REG_ACCESS(RoReg, 0x4002001CU) //!< (PWM) PWM Interrupt Status Register 1 */
#define REG_PWM_SCM         REG_ACCESS(RwReg, 0x40020020U) //!< (PWM) PWM Sync Channels Mode Register */
#define REG_PWM_SCUC        REG_ACCESS(RwReg, 0x40020028U) //!< (PWM) PWM Sync Channels Update Control Register */
#define REG_PWM_SCUP        REG_ACCESS(RwReg, 0x4002002CU) //!< (PWM) PWM Sync Channels Update Period Register */
#define REG_PWM_SCUPUPD     REG_ACCESS(WoReg, 0x40020030U) //!< (PWM) PWM Sync Channels Update Period Update Register */
#define REG_PWM_IER2        REG_ACCESS(WoReg, 0x40020034U) //!< (PWM) PWM Interrupt Enable Register 2 */
#define REG_PWM_IDR2        REG_ACCESS(WoReg, 0x40020038U) //!< (PWM) PWM Interrupt Disable Register 2 */
#define REG_PWM_IMR2        REG_ACCESS(RoReg, 0x4002003CU) //!< (PWM) PWM Interrupt Mask Register 2 */
#define REG_PWM_ISR2        REG_ACCESS(RoReg, 0x40020040U) //!< (PWM) PWM Interrupt Status Register 2 */
#define REG_PWM_OOV         REG_ACCESS(RwReg, 0x40020044U) //!< (PWM) PWM Output Override Value Register */
#define REG_PWM_OS          REG_ACCESS(RwReg, 0x40020048U) //!< (PWM) PWM Output Selection Register */
#define REG_PWM_OSS         REG_ACCESS(WoReg, 0x4002004CU) //!< (PWM) PWM Output Selection Set Register */
#define REG_PWM_OSC         REG_ACCESS(WoReg, 0x40020050U) //!< (PWM) PWM Output Selection Clear Register */
#define REG_PWM_OSSUPD      REG_ACCESS(WoReg, 0x40020054U) //!< (PWM) PWM Output Selection Set Update Register */
#define REG_PWM_OSCUPD      REG_ACCESS(WoReg, 0x40020058U) //!< (PWM) PWM Output Selection Clear Update Register */
#define REG_PWM_FMR         REG_ACCESS(RwReg, 0x4002005CU) //!< (PWM) PWM Fault Mode Register */
#define REG_PWM_FSR         REG_ACCESS(RoReg, 0x40020060U) //!< (PWM) PWM Fault Status Register */
#define REG_PWM_FCR         REG_ACCESS(WoReg, 0x40020064U) //!< (PWM) PWM Fault Clear Register */
#define REG_PWM_FPV         REG_ACCESS(RwReg, 0x40020068U) //!< (PWM) PWM Fault Protection Value Register */
#define REG_PWM_FPE         REG_ACCESS(RwReg, 0x4002006CU) //!< (PWM) PWM Fault Protection Enable Register */
#define REG_PWM_EL0MR       REG_ACCESS(RwReg, 0x4002007CU) //!< (PWM) PWM Event Line 0 Mode Register */
#define REG_PWM_EL1MR       REG_ACCESS(RwReg, 0x40020080U) //!< (PWM) PWM Event Line 1 Mode Register */
#define REG_PWM_SMMR        REG_ACCESS(RwReg, 0x400200B0U) //!< (PWM) PWM Stepper Motor Mode Register */
#define REG_PWM_WPCR        REG_ACCESS(WoReg, 0x400200E4U) //!< (PWM) PWM Write Protect Control Register */
#define REG_PWM_WPSR        REG_ACCESS(RoReg, 0x400200E8U) //!< (PWM) PWM Write Protect Status Register */
#define REG_PWM_RPR         REG_ACCESS(RwReg, 0x40020100U) //!< (PWM) Receive Pointer Register */
#define REG_PWM_RCR         REG_ACCESS(RwReg, 0x40020104U) //!< (PWM) Receive Counter Register */
#define REG_PWM_TPR         REG_ACCESS(RwReg, 0x40020108U) //!< (PWM) Transmit Pointer Register */
#define REG_PWM_TCR         REG_ACCESS(RwReg, 0x4002010CU) //!< (PWM) Transmit Counter Register */
#define REG_PWM_RNPR        REG_ACCESS(RwReg, 0x40020110U) //!< (PWM) Receive Next Pointer Register */
#define REG_PWM_RNCR        REG_ACCESS(RwReg, 0x40020114U) //!< (PWM) Receive Next Counter Register */
#define REG_PWM_TNPR        REG_ACCESS(RwReg, 0x40020118U) //!< (PWM) Transmit Next Pointer Register */
#define REG_PWM_TNCR        REG_ACCESS(RwReg, 0x4002011CU) //!< (PWM) Transmit Next Counter Register */
#define REG_PWM_PTCR        REG_ACCESS(WoReg, 0x40020120U) //!< (PWM) Transfer Control Register */
#define REG_PWM_PTSR        REG_ACCESS(RoReg, 0x40020124U) //!< (PWM) Transfer Status Register */
#define REG_PWM_CMP0V       REG_ACCESS(RwReg, 0x40020130U) //!< (PWM) PWM Comparison 0 Value Register */
#define REG_PWM_CMP0VUPD    REG_ACCESS(WoReg, 0x40020134U) //!< (PWM) PWM Comparison 0 Value Update Register */
#define REG_PWM_CMP0M       REG_ACCESS(RwReg, 0x40020138U) //!< (PWM) PWM Comparison 0 Mode Register */
#define REG_PWM_CMP0MUPD    REG_ACCESS(WoReg, 0x4002013CU) //!< (PWM) PWM Comparison 0 Mode Update Register */
#define REG_PWM_CMP1V       REG_ACCESS(RwReg, 0x40020140U) //!< (PWM) PWM Comparison 1 Value Register */
#define REG_PWM_CMP1VUPD    REG_ACCESS(WoReg, 0x40020144U) //!< (PWM) PWM Comparison 1 Value Update Register */
#define REG_PWM_CMP1M       REG_ACCESS(RwReg, 0x40020148U) //!< (PWM) PWM Comparison 1 Mode Register */
#define REG_PWM_CMP1MUPD    REG_ACCESS(WoReg, 0x4002014CU) //!< (PWM) PWM Comparison 1 Mode Update Register */
#define REG_PWM_CMP2V       REG_ACCESS(RwReg, 0x40020150U) //!< (PWM) PWM Comparison 2 Value Register */
#define REG_PWM_CMP2VUPD    REG_ACCESS(WoReg, 0x40020154U) //!< (PWM) PWM Comparison 2 Value Update Register */
#define REG_PWM_CMP2M       REG_ACCESS(RwReg, 0x40020158U) //!< (PWM) PWM Comparison 2 Mode Register */
#define REG_PWM_CMP2MUPD    REG_ACCESS(WoReg, 0x4002015CU) //!< (PWM) PWM Comparison 2 Mode Update Register */
#define REG_PWM_CMP3V       REG_ACCESS(RwReg, 0x40020160U) //!< (PWM) PWM Comparison 3 Value Register */
#define REG_PWM_CMP3VUPD    REG_ACCESS(WoReg, 0x40020164U) //!< (PWM) PWM Comparison 3 Value Update Register */
#define REG_PWM_CMP3M       REG_ACCESS(RwReg, 0x40020168U) //!< (PWM) PWM Comparison 3 Mode Register */
#define REG_PWM_CMP3MUPD    REG_ACCESS(WoReg, 0x4002016CU) //!< (PWM) PWM Comparison 3 Mode Update Register */
#define REG_PWM_CMP4V       REG_ACCESS(RwReg, 0x40020170U) //!< (PWM) PWM Comparison 4 Value Register */
#define REG_PWM_CMP4VUPD    REG_ACCESS(WoReg, 0x40020174U) //!< (PWM) PWM Comparison 4 Value Update Register */
#define REG_PWM_CMP4M       REG_ACCESS(RwReg, 0x40020178U) //!< (PWM) PWM Comparison 4 Mode Register */
#define REG_PWM_CMP4MUPD    REG_ACCESS(WoReg, 0x4002017CU) //!< (PWM) PWM Comparison 4 Mode Update Register */
#define REG_PWM_CMP5V       REG_ACCESS(RwReg, 0x40020180U) //!< (PWM) PWM Comparison 5 Value Register */
#define REG_PWM_CMP5VUPD    REG_ACCESS(WoReg, 0x40020184U) //!< (PWM) PWM Comparison 5 Value Update Register */
#define REG_PWM_CMP5M       REG_ACCESS(RwReg, 0x40020188U) //!< (PWM) PWM Comparison 5 Mode Register */
#define REG_PWM_CMP5MUPD    REG_ACCESS(WoReg, 0x4002018CU) //!< (PWM) PWM Comparison 5 Mode Update Register */
#define REG_PWM_CMP6V       REG_ACCESS(RwReg, 0x40020190U) //!< (PWM) PWM Comparison 6 Value Register */
#define REG_PWM_CMP6VUPD    REG_ACCESS(WoReg, 0x40020194U) //!< (PWM) PWM Comparison 6 Value Update Register */
#define REG_PWM_CMP6M       REG_ACCESS(RwReg, 0x40020198U) //!< (PWM) PWM Comparison 6 Mode Register */
#define REG_PWM_CMP6MUPD    REG_ACCESS(WoReg, 0x4002019CU) //!< (PWM) PWM Comparison 6 Mode Update Register */
#define REG_PWM_CMP7V       REG_ACCESS(RwReg, 0x400201A0U) //!< (PWM) PWM Comparison 7 Value Register */
#define REG_PWM_CMP7VUPD    REG_ACCESS(WoReg, 0x400201A4U) //!< (PWM) PWM Comparison 7 Value Update Register */
#define REG_PWM_CMP7M       REG_ACCESS(RwReg, 0x400201A8U) //!< (PWM) PWM Comparison 7 Mode Register */
#define REG_PWM_CMP7MUPD    REG_ACCESS(WoReg, 0x400201ACU) //!< (PWM) PWM Comparison 7 Mode Update Register */
#define REG_PWM_CMR0        REG_ACCESS(RwReg, 0x40020200U) //!< (PWM) PWM Channel Mode Register (ch_num = 0) */
#define REG_PWM_CDTY0       REG_ACCESS(RwReg, 0x40020204U) //!< (PWM) PWM Channel Duty Cycle Register (ch_num = 0) */
#define REG_PWM_CDTYUPD0    REG_ACCESS(WoReg, 0x40020208U) //!< (PWM) PWM Channel Duty Cycle Update Register (ch_num = 0) */
#define REG_PWM_CPRD0       REG_ACCESS(RwReg, 0x4002020CU) //!< (PWM) PWM Channel Period Register (ch_num = 0) */
#define REG_PWM_CPRDUPD0    REG_ACCESS(WoReg, 0x40020210U) //!< (PWM) PWM Channel Period Update Register (ch_num = 0) */
#define REG_PWM_CCNT0       REG_ACCESS(RoReg, 0x40020214U) //!< (PWM) PWM Channel Counter Register (ch_num = 0) */
#define REG_PWM_DT0         REG_ACCESS(RwReg, 0x40020218U) //!< (PWM) PWM Channel Dead Time Register (ch_num = 0) */
#define REG_PWM_DTUPD0      REG_ACCESS(WoReg, 0x4002021CU) //!< (PWM) PWM Channel Dead Time Update Register (ch_num = 0) */
#define REG_PWM_CMR1        REG_ACCESS(RwReg, 0x40020220U) //!< (PWM) PWM Channel Mode Register (ch_num = 1) */
#define REG_PWM_CDTY1       REG_ACCESS(RwReg, 0x40020224U) //!< (PWM) PWM Channel Duty Cycle Register (ch_num = 1) */
#define REG_PWM_CDTYUPD1    REG_ACCESS(WoReg, 0x40020228U) //!< (PWM) PWM Channel Duty Cycle Update Register (ch_num = 1) */
#define REG_PWM_CPRD1       REG_ACCESS(RwReg, 0x4002022CU) //!< (PWM) PWM Channel Period Register (ch_num = 1) */
#define REG_PWM_CPRDUPD1    REG_ACCESS(WoReg, 0x40020230U) //!< (PWM) PWM Channel Period Update Register (ch_num = 1) */
#define REG_PWM_CCNT1       REG_ACCESS(RoReg, 0x40020234U) //!< (PWM) PWM Channel Counter Register (ch_num = 1) */
#define REG_PWM_DT1         REG_ACCESS(RwReg, 0x40020238U) //!< (PWM) PWM Channel Dead Time Register (ch_num = 1) */
#define REG_PWM_DTUPD1      REG_ACCESS(WoReg, 0x4002023CU) //!< (PWM) PWM Channel Dead Time Update Register (ch_num = 1) */
#define REG_PWM_CMR2        REG_ACCESS(RwReg, 0x40020240U) //!< (PWM) PWM Channel Mode Register (ch_num = 2) */
#define REG_PWM_CDTY2       REG_ACCESS(RwReg, 0x40020244U) //!< (PWM) PWM Channel Duty Cycle Register (ch_num = 2) */
#define REG_PWM_CDTYUPD2    REG_ACCESS(WoReg, 0x40020248U) //!< (PWM) PWM Channel Duty Cycle Update Register (ch_num = 2) */
#define REG_PWM_CPRD2       REG_ACCESS(RwReg, 0x4002024CU) //!< (PWM) PWM Channel Period Register (ch_num = 2) */
#define REG_PWM_CPRDUPD2    REG_ACCESS(WoReg, 0x40020250U) //!< (PWM) PWM Channel Period Update Register (ch_num = 2) */
#define REG_PWM_CCNT2       REG_ACCESS(RoReg, 0x40020254U) //!< (PWM) PWM Channel Counter Register (ch_num = 2) */
#define REG_PWM_DT2         REG_ACCESS(RwReg, 0x40020258U) //!< (PWM) PWM Channel Dead Time Register (ch_num = 2) */
#define REG_PWM_DTUPD2      REG_ACCESS(WoReg, 0x4002025CU) //!< (PWM) PWM Channel Dead Time Update Register (ch_num = 2) */
#define REG_PWM_CMR3        REG_ACCESS(RwReg, 0x40020260U) //!< (PWM) PWM Channel Mode Register (ch_num = 3) */
#define REG_PWM_CDTY3       REG_ACCESS(RwReg, 0x40020264U) //!< (PWM) PWM Channel Duty Cycle Register (ch_num = 3) */
#define REG_PWM_CDTYUPD3    REG_ACCESS(WoReg, 0x40020268U) //!< (PWM) PWM Channel Duty Cycle Update Register (ch_num = 3) */
#define REG_PWM_CPRD3       REG_ACCESS(RwReg, 0x4002026CU) //!< (PWM) PWM Channel Period Register (ch_num = 3) */
#define REG_PWM_CPRDUPD3    REG_ACCESS(WoReg, 0x40020270U) //!< (PWM) PWM Channel Period Update Register (ch_num = 3) */
#define REG_PWM_CCNT3       REG_ACCESS(RoReg, 0x40020274U) //!< (PWM) PWM Channel Counter Register (ch_num = 3) */
#define REG_PWM_DT3         REG_ACCESS(RwReg, 0x40020278U) //!< (PWM) PWM Channel Dead Time Register (ch_num = 3) */
#define REG_PWM_DTUPD3      REG_ACCESS(WoReg, 0x4002027CU) //!< (PWM) PWM Channel Dead Time Update Register (ch_num = 3) */
/* ========== Register definition for USART0 peripheral ========== */
#define REG_USART0_CR       REG_ACCESS(WoReg, 0x40024000U) //!< (USART0) Control Register */
#define REG_USART0_MR       REG_ACCESS(RwReg, 0x40024004U) //!< (USART0) Mode Register */
#define REG_USART0_IER      REG_ACCESS(WoReg, 0x40024008U) //!< (USART0) Interrupt Enable Register */
#define REG_USART0_IDR      REG_ACCESS(WoReg, 0x4002400CU) //!< (USART0) Interrupt Disable Register */
#define REG_USART0_IMR      REG_ACCESS(RoReg, 0x40024010U) //!< (USART0) Interrupt Mask Register */
#define REG_USART0_CSR      REG_ACCESS(RoReg, 0x40024014U) //!< (USART0) Channel Status Register */
#define REG_USART0_RHR      REG_ACCESS(RoReg, 0x40024018U) //!< (USART0) Receiver Holding Register */
#define REG_USART0_THR      REG_ACCESS(WoReg, 0x4002401CU) //!< (USART0) Transmitter Holding Register */
#define REG_USART0_BRGR     REG_ACCESS(RwReg, 0x40024020U) //!< (USART0) Baud Rate Generator Register */
#define REG_USART0_RTOR     REG_ACCESS(RwReg, 0x40024024U) //!< (USART0) Receiver Time-out Register */
#define REG_USART0_TTGR     REG_ACCESS(RwReg, 0x40024028U) //!< (USART0) Transmitter Timeguard Register */
#define REG_USART0_FIDI     REG_ACCESS(RwReg, 0x40024040U) //!< (USART0) FI DI Ratio Register */
#define REG_USART0_NER      REG_ACCESS(RoReg, 0x40024044U) //!< (USART0) Number of Errors Register */
#define REG_USART0_IF       REG_ACCESS(RwReg, 0x4002404CU) //!< (USART0) IrDA Filter Register */
#define REG_USART0_MAN      REG_ACCESS(RwReg, 0x40024050U) //!< (USART0) Manchester Encoder Decoder Register */
#define REG_USART0_WPMR     REG_ACCESS(RwReg, 0x400240E4U) //!< (USART0) Write Protect Mode Register */
#define REG_USART0_WPSR     REG_ACCESS(RoReg, 0x400240E8U) //!< (USART0) Write Protect Status Register */
#define REG_USART0_RPR      REG_ACCESS(RwReg, 0x40024100U) //!< (USART0) Receive Pointer Register */
#define REG_USART0_RCR      REG_ACCESS(RwReg, 0x40024104U) //!< (USART0) Receive Counter Register */
#define REG_USART0_TPR      REG_ACCESS(RwReg, 0x40024108U) //!< (USART0) Transmit Pointer Register */
#define REG_USART0_TCR      REG_ACCESS(RwReg, 0x4002410CU) //!< (USART0) Transmit Counter Register */
#define REG_USART0_RNPR     REG_ACCESS(RwReg, 0x40024110U) //!< (USART0) Receive Next Pointer Register */
#define REG_USART0_RNCR     REG_ACCESS(RwReg, 0x40024114U) //!< (USART0) Receive Next Counter Register */
#define REG_USART0_TNPR     REG_ACCESS(RwReg, 0x40024118U) //!< (USART0) Transmit Next Pointer Register */
#define REG_USART0_TNCR     REG_ACCESS(RwReg, 0x4002411CU) //!< (USART0) Transmit Next Counter Register */
#define REG_USART0_PTCR     REG_ACCESS(WoReg, 0x40024120U) //!< (USART0) Transfer Control Register */
#define REG_USART0_PTSR     REG_ACCESS(RoReg, 0x40024124U) //!< (USART0) Transfer Status Register */
/* ========== Register definition for USART1 peripheral ========== */
#define REG_USART1_CR       REG_ACCESS(WoReg, 0x40028000U) //!< (USART1) Control Register */
#define REG_USART1_MR       REG_ACCESS(RwReg, 0x40028004U) //!< (USART1) Mode Register */
#define REG_USART1_IER      REG_ACCESS(WoReg, 0x40028008U) //!< (USART1) Interrupt Enable Register */
#define REG_USART1_IDR      REG_ACCESS(WoReg, 0x4002800CU) //!< (USART1) Interrupt Disable Register */
#define REG_USART1_IMR      REG_ACCESS(RoReg, 0x40028010U) //!< (USART1) Interrupt Mask Register */
#define REG_USART1_CSR      REG_ACCESS(RoReg, 0x40028014U) //!< (USART1) Channel Status Register */
#define REG_USART1_RHR      REG_ACCESS(RoReg, 0x40028018U) //!< (USART1) Receiver Holding Register */
#define REG_USART1_THR      REG_ACCESS(WoReg, 0x4002801CU) //!< (USART1) Transmitter Holding Register */
#define REG_USART1_BRGR     REG_ACCESS(RwReg, 0x40028020U) //!< (USART1) Baud Rate Generator Register */
#define REG_USART1_RTOR     REG_ACCESS(RwReg, 0x40028024U) //!< (USART1) Receiver Time-out Register */
#define REG_USART1_TTGR     REG_ACCESS(RwReg, 0x40028028U) //!< (USART1) Transmitter Timeguard Register */
#define REG_USART1_FIDI     REG_ACCESS(RwReg, 0x40028040U) //!< (USART1) FI DI Ratio Register */
#define REG_USART1_NER      REG_ACCESS(RoReg, 0x40028044U) //!< (USART1) Number of Errors Register */
#define REG_USART1_IF       REG_ACCESS(RwReg, 0x4002804CU) //!< (USART1) IrDA Filter Register */
#define REG_USART1_MAN      REG_ACCESS(RwReg, 0x40028050U) //!< (USART1) Manchester Encoder Decoder Register */
#define REG_USART1_WPMR     REG_ACCESS(RwReg, 0x400280E4U) //!< (USART1) Write Protect Mode Register */
#define REG_USART1_WPSR     REG_ACCESS(RoReg, 0x400280E8U) //!< (USART1) Write Protect Status Register */
#define REG_USART1_RPR      REG_ACCESS(RwReg, 0x40028100U) //!< (USART1) Receive Pointer Register */
#define REG_USART1_RCR      REG_ACCESS(RwReg, 0x40028104U) //!< (USART1) Receive Counter Register */
#define REG_USART1_TPR      REG_ACCESS(RwReg, 0x40028108U) //!< (USART1) Transmit Pointer Register */
#define REG_USART1_TCR      REG_ACCESS(RwReg, 0x4002810CU) //!< (USART1) Transmit Counter Register */
#define REG_USART1_RNPR     REG_ACCESS(RwReg, 0x40028110U) //!< (USART1) Receive Next Pointer Register */
#define REG_USART1_RNCR     REG_ACCESS(RwReg, 0x40028114U) //!< (USART1) Receive Next Counter Register */
#define REG_USART1_TNPR     REG_ACCESS(RwReg, 0x40028118U) //!< (USART1) Transmit Next Pointer Register */
#define REG_USART1_TNCR     REG_ACCESS(RwReg, 0x4002811CU) //!< (USART1) Transmit Next Counter Register */
#define REG_USART1_PTCR     REG_ACCESS(WoReg, 0x40028120U) //!< (USART1) Transfer Control Register */
#define REG_USART1_PTSR     REG_ACCESS(RoReg, 0x40028124U) //!< (USART1) Transfer Status Register */

/* ========== Register definition for UDP peripheral ========== */
#define REG_UDP_FRM_NUM     REG_ACCESS(RoReg, 0x40034000U) //!< (UDP) Frame Number Register */
#define REG_UDP_GLB_STAT    REG_ACCESS(RwReg, 0x40034004U) //!< (UDP) Global State Register */
#define REG_UDP_FADDR       REG_ACCESS(RwReg, 0x40034008U) //!< (UDP) Function Address Register */
#define REG_UDP_IER         REG_ACCESS(WoReg, 0x40034010U) //!< (UDP) Interrupt Enable Register */
#define REG_UDP_IDR         REG_ACCESS(WoReg, 0x40034014U) //!< (UDP) Interrupt Disable Register */
#define REG_UDP_IMR         REG_ACCESS(RoReg, 0x40034018U) //!< (UDP) Interrupt Mask Register */
#define REG_UDP_ISR         REG_ACCESS(RoReg, 0x4003401CU) //!< (UDP) Interrupt Status Register */
#define REG_UDP_ICR         REG_ACCESS(WoReg, 0x40034020U) //!< (UDP) Interrupt Clear Register */
#define REG_UDP_RST_EP      REG_ACCESS(RwReg, 0x40034028U) //!< (UDP) Reset Endpoint Register */
#define REG_UDP_CSR         REG_ACCESS(RwReg, 0x40034030U) //!< (UDP) Endpoint Control and Status Register */
#define REG_UDP_FDR         REG_ACCESS(RwReg, 0x40034050U) //!< (UDP) Endpoint FIFO Data Register */
#define REG_UDP_TXVC        REG_ACCESS(RwReg, 0x40034074U) //!< (UDP) Transceiver Control Register */

/* ========== Register definition for ADC peripheral ========== */
#define REG_ADC_CR          REG_ACCESS(WoReg, 0x40038000U) //!< (ADC) Control Register */
#define REG_ADC_MR          REG_ACCESS(RwReg, 0x40038004U) //!< (ADC) Mode Register */
#define REG_ADC_SEQR1       REG_ACCESS(RwReg, 0x40038008U) //!< (ADC) Channel Sequence Register 1 */
#define REG_ADC_SEQR2       REG_ACCESS(RwReg, 0x4003800CU) //!< (ADC) Channel Sequence Register 2 */
#define REG_ADC_CHER        REG_ACCESS(WoReg, 0x40038010U) //!< (ADC) Channel Enable Register */
#define REG_ADC_CHDR        REG_ACCESS(WoReg, 0x40038014U) //!< (ADC) Channel Disable Register */
#define REG_ADC_CHSR        REG_ACCESS(RoReg, 0x40038018U) //!< (ADC) Channel Status Register */
#define REG_ADC_LCDR        REG_ACCESS(RoReg, 0x40038020U) //!< (ADC) Last Converted Data Register */
#define REG_ADC_IER         REG_ACCESS(WoReg, 0x40038024U) //!< (ADC) Interrupt Enable Register */
#define REG_ADC_IDR         REG_ACCESS(WoReg, 0x40038028U) //!< (ADC) Interrupt Disable Register */
#define REG_ADC_IMR         REG_ACCESS(RoReg, 0x4003802CU) //!< (ADC) Interrupt Mask Register */
#define REG_ADC_ISR         REG_ACCESS(RoReg, 0x40038030U) //!< (ADC) Interrupt Status Register */
#define REG_ADC_OVER        REG_ACCESS(RoReg, 0x4003803CU) //!< (ADC) Overrun Status Register */
#define REG_ADC_EMR         REG_ACCESS(RwReg, 0x40038040U) //!< (ADC) Extended Mode Register */
#define REG_ADC_CWR         REG_ACCESS(RwReg, 0x40038044U) //!< (ADC) Compare Window Register */
#define REG_ADC_CGR         REG_ACCESS(RwReg, 0x40038048U) //!< (ADC) Channel Gain Register */
#define REG_ADC_COR         REG_ACCESS(RwReg, 0x4003804CU) //!< (ADC) Channel Offset Register */
#define REG_ADC_CDR         REG_ACCESS(RoReg, 0x40038050U) //!< (ADC) Channel Data Register */
#define REG_ADC_ACR         REG_ACCESS(RwReg, 0x40038094U) //!< (ADC) Analog Control Register */
#define REG_ADC_WPMR        REG_ACCESS(RwReg, 0x400380E4U) //!< (ADC) Write Protect Mode Register */
#define REG_ADC_WPSR        REG_ACCESS(RoReg, 0x400380E8U) //!< (ADC) Write Protect Status Register */
#define REG_ADC_RPR         REG_ACCESS(RwReg, 0x40038100U) //!< (ADC) Receive Pointer Register */
#define REG_ADC_RCR         REG_ACCESS(RwReg, 0x40038104U) //!< (ADC) Receive Counter Register */
#define REG_ADC_TPR         REG_ACCESS(RwReg, 0x40038108U) //!< (ADC) Transmit Pointer Register */
#define REG_ADC_TCR         REG_ACCESS(RwReg, 0x4003810CU) //!< (ADC) Transmit Counter Register */
#define REG_ADC_RNPR        REG_ACCESS(RwReg, 0x40038110U) //!< (ADC) Receive Next Pointer Register */
#define REG_ADC_RNCR        REG_ACCESS(RwReg, 0x40038114U) //!< (ADC) Receive Next Counter Register */
#define REG_ADC_TNPR        REG_ACCESS(RwReg, 0x40038118U) //!< (ADC) Transmit Next Pointer Register */
#define REG_ADC_TNCR        REG_ACCESS(RwReg, 0x4003811CU) //!< (ADC) Transmit Next Counter Register */
#define REG_ADC_PTCR        REG_ACCESS(WoReg, 0x40038120U) //!< (ADC) Transfer Control Register */
#define REG_ADC_PTSR        REG_ACCESS(RoReg, 0x40038124U) //!< (ADC) Transfer Status Register */

/* ========== Register definition for DACC peripheral ========== */
#define REG_DACC_CR         REG_ACCESS(WoReg, 0x4003C000U) //!< (DACC) Control Register
#define REG_DACC_MR         REG_ACCESS(RwReg, 0x4003C004U) //!< (DACC) Mode Register
#define REG_DACC_CHER       REG_ACCESS(WoReg, 0x4003C010U) //!< (DACC) Channel Enable Register
#define REG_DACC_CHDR       REG_ACCESS(WoReg, 0x4003C014U) //!< (DACC) Channel Disable Register
#define REG_DACC_CHSR       REG_ACCESS(RoReg, 0x4003C018U) //!< (DACC) Channel Status Register
#define REG_DACC_CDR        REG_ACCESS(WoReg, 0x4003C020U) //!< (DACC) Conversion Data Register
#define REG_DACC_IER        REG_ACCESS(WoReg, 0x4003C024U) //!< (DACC) Interrupt Enable Register
#define REG_DACC_IDR        REG_ACCESS(WoReg, 0x4003C028U) //!< (DACC) Interrupt Disable Register
#define REG_DACC_IMR        REG_ACCESS(RoReg, 0x4003C02CU) //!< (DACC) Interrupt Mask Register
#define REG_DACC_ISR        REG_ACCESS(RoReg, 0x4003C030U) //!< (DACC) Interrupt Status Register
#define REG_DACC_ACR        REG_ACCESS(RwReg, 0x4003C094U) //!< (DACC) Analog Current Register
#define REG_DACC_WPMR       REG_ACCESS(RwReg, 0x4003C0E4U) //!< (DACC) Write Protect Mode register
#define REG_DACC_WPSR       REG_ACCESS(RoReg, 0x4003C0E8U) //!< (DACC) Write Protect Status register
#define REG_DACC_RPR        REG_ACCESS(RwReg, 0x4003C100U) //!< (DACC) Receive Pointer Register
#define REG_DACC_RCR        REG_ACCESS(RwReg, 0x4003C104U) //!< (DACC) Receive Counter Register
#define REG_DACC_TPR        REG_ACCESS(RwReg, 0x4003C108U) //!< (DACC) Transmit Pointer Register
#define REG_DACC_TCR        REG_ACCESS(RwReg, 0x4003C10CU) //!< (DACC) Transmit Counter Register
#define REG_DACC_RNPR       REG_ACCESS(RwReg, 0x4003C110U) //!< (DACC) Receive Next Pointer Register
#define REG_DACC_RNCR       REG_ACCESS(RwReg, 0x4003C114U) //!< (DACC) Receive Next Counter Register
#define REG_DACC_TNPR       REG_ACCESS(RwReg, 0x4003C118U) //!< (DACC) Transmit Next Pointer Register
#define REG_DACC_TNCR       REG_ACCESS(RwReg, 0x4003C11CU) //!< (DACC) Transmit Next Counter Register
#define REG_DACC_PTCR       REG_ACCESS(WoReg, 0x4003C120U) //!< (DACC) Transfer Control Register
#define REG_DACC_PTSR       REG_ACCESS(RoReg, 0x4003C124U) //!< (DACC) Transfer Status Register

/* ========== Register definition for ACC peripheral ========== */
#define REG_ACC_CR          REG_ACCESS(WoReg, 0x40040000U) //!< (ACC) Control Register */
#define REG_ACC_MR          REG_ACCESS(RwReg, 0x40040004U) //!< (ACC) Mode Register */
#define REG_ACC_IER         REG_ACCESS(WoReg, 0x40040024U) //!< (ACC) Interrupt Enable Register */
#define REG_ACC_IDR         REG_ACCESS(WoReg, 0x40040028U) //!< (ACC) Interrupt Disable Register */
#define REG_ACC_IMR         REG_ACCESS(RoReg, 0x4004002CU) //!< (ACC) Interrupt Mask Register */
#define REG_ACC_ISR         REG_ACCESS(RoReg, 0x40040030U) //!< (ACC) Interrupt Status Register */
#define REG_ACC_ACR         REG_ACCESS(RwReg, 0x40040094U) //!< (ACC) Analog Control Register */
#define REG_ACC_WPMR        REG_ACCESS(RwReg, 0x400400E4U) //!< (ACC) Write Protect Mode Register */
#define REG_ACC_WPSR        REG_ACCESS(RoReg, 0x400400E8U) //!< (ACC) Write Protect Status Register */
/* ========== Register definition for CRCCU peripheral ========== */
#define REG_CRCCU_DSCR      REG_ACCESS(RwReg, 0x40044000U) //!< (CRCCU) CRCCU Descriptor Base Register */
#define REG_CRCCU_DMA_EN    REG_ACCESS(WoReg, 0x40044008U) //!< (CRCCU) CRCCU DMA Enable Register */
#define REG_CRCCU_DMA_DIS   REG_ACCESS(WoReg, 0x4004400CU) //!< (CRCCU) CRCCU DMA Disable Register */
#define REG_CRCCU_DMA_SR    REG_ACCESS(RoReg, 0x40044010U) //!< (CRCCU) CRCCU DMA Status Register */
#define REG_CRCCU_DMA_IER   REG_ACCESS(WoReg, 0x40044014U) //!< (CRCCU) CRCCU DMA Interrupt Enable Register */
#define REG_CRCCU_DMA_IDR   REG_ACCESS(WoReg, 0x40044018U) //!< (CRCCU) CRCCU DMA Interrupt Disable Register */
#define REG_CRCCU_DMA_IMR   REG_ACCESS(RoReg, 0x4004401CU) //!< (CRCCU) CRCCU DMA Interrupt Mask Register */
#define REG_CRCCU_DMA_ISR   REG_ACCESS(RoReg, 0x40044020U) //!< (CRCCU) CRCCU DMA Interrupt Status Register */
#define REG_CRCCU_CR        REG_ACCESS(WoReg, 0x40044034U) //!< (CRCCU) CRCCU Control Register */
#define REG_CRCCU_MR        REG_ACCESS(RwReg, 0x40044038U) //!< (CRCCU) CRCCU Mode Register */
#define REG_CRCCU_SR        REG_ACCESS(RoReg, 0x4004403CU) //!< (CRCCU) CRCCU Status Register */
#define REG_CRCCU_IER       REG_ACCESS(WoReg, 0x40044040U) //!< (CRCCU) CRCCU Interrupt Enable Register */
#define REG_CRCCU_IDR       REG_ACCESS(WoReg, 0x40044044U) //!< (CRCCU) CRCCU Interrupt Disable Register */
#define REG_CRCCU_IMR       REG_ACCESS(RoReg, 0x40044048U) //!< (CRCCU) CRCCU Interrupt Mask Register */
#define REG_CRCCU_ISR       REG_ACCESS(RoReg, 0x4004404CU) //!< (CRCCU) CRCCU Interrupt Status Register */
/* ========== Register definition for SMC peripheral ========== */
#define REG_SMC_SETUP0      REG_ACCESS(RwReg, 0x400E0000U) //!< (SMC) SMC Setup Register (CS_number = 0) */
#define REG_SMC_PULSE0      REG_ACCESS(RwReg, 0x400E0004U) //!< (SMC) SMC Pulse Register (CS_number = 0) */
#define REG_SMC_CYCLE0      REG_ACCESS(RwReg, 0x400E0008U) //!< (SMC) SMC Cycle Register (CS_number = 0) */
#define REG_SMC_MODE0       REG_ACCESS(RwReg, 0x400E000CU) //!< (SMC) SMC Mode Register (CS_number = 0) */
#define REG_SMC_SETUP1      REG_ACCESS(RwReg, 0x400E0010U) //!< (SMC) SMC Setup Register (CS_number = 1) */
#define REG_SMC_PULSE1      REG_ACCESS(RwReg, 0x400E0014U) //!< (SMC) SMC Pulse Register (CS_number = 1) */
#define REG_SMC_CYCLE1      REG_ACCESS(RwReg, 0x400E0018U) //!< (SMC) SMC Cycle Register (CS_number = 1) */
#define REG_SMC_MODE1       REG_ACCESS(RwReg, 0x400E001CU) //!< (SMC) SMC Mode Register (CS_number = 1) */
#define REG_SMC_SETUP2      REG_ACCESS(RwReg, 0x400E0020U) //!< (SMC) SMC Setup Register (CS_number = 2) */
#define REG_SMC_PULSE2      REG_ACCESS(RwReg, 0x400E0024U) //!< (SMC) SMC Pulse Register (CS_number = 2) */
#define REG_SMC_CYCLE2      REG_ACCESS(RwReg, 0x400E0028U) //!< (SMC) SMC Cycle Register (CS_number = 2) */
#define REG_SMC_MODE2       REG_ACCESS(RwReg, 0x400E002CU) //!< (SMC) SMC Mode Register (CS_number = 2) */
#define REG_SMC_SETUP3      REG_ACCESS(RwReg, 0x400E0030U) //!< (SMC) SMC Setup Register (CS_number = 3) */
#define REG_SMC_PULSE3      REG_ACCESS(RwReg, 0x400E0034U) //!< (SMC) SMC Pulse Register (CS_number = 3) */
#define REG_SMC_CYCLE3      REG_ACCESS(RwReg, 0x400E0038U) //!< (SMC) SMC Cycle Register (CS_number = 3) */
#define REG_SMC_MODE3       REG_ACCESS(RwReg, 0x400E003CU) //!< (SMC) SMC Mode Register (CS_number = 3) */
#define REG_SMC_SETUP4      REG_ACCESS(RwReg, 0x400E0040U) //!< (SMC) SMC Setup Register (CS_number = 4) */
#define REG_SMC_PULSE4      REG_ACCESS(RwReg, 0x400E0044U) //!< (SMC) SMC Pulse Register (CS_number = 4) */
#define REG_SMC_CYCLE4      REG_ACCESS(RwReg, 0x400E0048U) //!< (SMC) SMC Cycle Register (CS_number = 4) */
#define REG_SMC_MODE4       REG_ACCESS(RwReg, 0x400E004CU) //!< (SMC) SMC Mode Register (CS_number = 4) */
#define REG_SMC_OCMS        REG_ACCESS(RwReg, 0x400E0080U) //!< (SMC) SMC OCMS MODE Register */
#define REG_SMC_KEY1        REG_ACCESS(WoReg, 0x400E0084U) //!< (SMC) SMC OCMS KEY1 Register */
#define REG_SMC_KEY2        REG_ACCESS(WoReg, 0x400E0088U) //!< (SMC) SMC OCMS KEY2 Register */
#define REG_SMC_WPMR        REG_ACCESS(RwReg, 0x400E00E4U) //!< (SMC) SMC Write Protect Mode Register */
#define REG_SMC_WPSR        REG_ACCESS(RoReg, 0x400E00E8U) //!< (SMC) SMC Write Protect Status Register */
/* ========== Register definition for MATRIX peripheral ========== */
#define REG_MATRIX_MCFG     REG_ACCESS(RwReg, 0x400E0200U) //!< (MATRIX) Master Configuration Register */
#define REG_MATRIX_SCFG     REG_ACCESS(RwReg, 0x400E0240U) //!< (MATRIX) Slave Configuration Register */
#define REG_MATRIX_PRAS0    REG_ACCESS(RwReg, 0x400E0280U) //!< (MATRIX) Priority Register A for Slave 0 */
#define REG_MATRIX_PRAS1    REG_ACCESS(RwReg, 0x400E0288U) //!< (MATRIX) Priority Register A for Slave 1 */
#define REG_MATRIX_PRAS2    REG_ACCESS(RwReg, 0x400E0290U) //!< (MATRIX) Priority Register A for Slave 2 */
#define REG_MATRIX_PRAS3    REG_ACCESS(RwReg, 0x400E0298U) //!< (MATRIX) Priority Register A for Slave 3 */
#define REG_MATRIX_PRAS4    REG_ACCESS(RwReg, 0x400E02A0U) //!< (MATRIX) Priority Register A for Slave 4 */
#define REG_MATRIX_SYSIO    REG_ACCESS(RwReg, 0x400E0314U) //!< (MATRIX) System I/O Configuration register */
#define REG_MATRIX_SMCNFCS  REG_ACCESS(RwReg, 0x400E031CU) //!< (MATRIX) SMC Chip Select NAND Flash Assignment Register */
#define REG_MATRIX_WPMR     REG_ACCESS(RwReg, 0x400E03E4U) //!< (MATRIX) Write Protect Mode Register */
#define REG_MATRIX_WPSR     REG_ACCESS(RoReg, 0x400E03E8U) //!< (MATRIX) Write Protect Status Register */
/* ========== Register definition for PMC peripheral ========== */
#define REG_PMC_SCER        REG_ACCESS(WoReg, 0x400E0400U) //!< @brief (PMC) System Clock Enable Register
#define REG_PMC_SCDR        REG_ACCESS(WoReg, 0x400E0404U) //!< @brief (PMC) System Clock Disable Register
#define REG_PMC_SCSR        REG_ACCESS(RoReg, 0x400E0408U) //!< @brief (PMC) System Clock Status Register
#define REG_PMC_PCER0       REG_ACCESS(WoReg, 0x400E0410U) //!< @brief (PMC) Peripheral Clock Enable Register 0
#define REG_PMC_PCDR0       REG_ACCESS(WoReg, 0x400E0414U) //!< @brief (PMC) Peripheral Clock Disable Register 0
#define REG_PMC_PCSR0       REG_ACCESS(RoReg, 0x400E0418U) //!< (PMC) Peripheral Clock Status Register 0
#define REG_PMC_MOR         REG_ACCESS(RwReg, 0x400E0420U) //!< (PMC) Main Oscillator Register
#define REG_PMC_MCFR        REG_ACCESS(RoReg, 0x400E0424U) //!< (PMC) Main Clock Frequency Register
#define REG_PMC_PLLAR       REG_ACCESS(RwReg, 0x400E0428U) //!< (PMC) PLLA Register
#define REG_PMC_PLLBR       REG_ACCESS(RwReg, 0x400E042CU) //!< (PMC) PLLB Register
#define REG_PMC_MCKR        REG_ACCESS(RwReg, 0x400E0430U) //!< (PMC) Master Clock Register
#define REG_PMC_USB         REG_ACCESS(RwReg, 0x400E0438U) //!< (PMC) USB Clock Register
#define REG_PMC_PCK         REG_ACCESS(RwReg, 0x400E0440U) //!< (PMC) Programmable Clock 0 Register
#define REG_PMC_IER         REG_ACCESS(WoReg, 0x400E0460U) //!< (PMC) Interrupt Enable Register
#define REG_PMC_IDR         REG_ACCESS(WoReg, 0x400E0464U) //!< (PMC) Interrupt Disable Register
#define REG_PMC_SR          REG_ACCESS(RoReg, 0x400E0468U) //!< (PMC) Status Register
#define REG_PMC_IMR         REG_ACCESS(RoReg, 0x400E046CU) //!< (PMC) Interrupt Mask Register
#define REG_PMC_FSMR        REG_ACCESS(RwReg, 0x400E0470U) //!< (PMC) Fast Startup Mode Register
#define REG_PMC_FSPR        REG_ACCESS(RwReg, 0x400E0474U) //!< (PMC) Fast Startup Polarity Register
#define REG_PMC_FOCR        REG_ACCESS(WoReg, 0x400E0478U) //!< (PMC) Fault Output Clear Register
#define REG_PMC_WPMR        REG_ACCESS(RwReg, 0x400E04E4U) //!< (PMC) Write Protect Mode Register
#define REG_PMC_WPSR        REG_ACCESS(RoReg, 0x400E04E8U) //!< (PMC) Write Protect Status Register
#define REG_PMC_PCER1       REG_ACCESS(WoReg, 0x400E0500U) //!< (PMC) Peripheral Clock Enable Register 1
#define REG_PMC_PCDR1       REG_ACCESS(WoReg, 0x400E0504U) //!< (PMC) Peripheral Clock Disable Register 1
#define REG_PMC_PCSR1       REG_ACCESS(RoReg, 0x400E0508U) //!< (PMC) Peripheral Clock Status Register 1
#define REG_PMC_OCR         REG_ACCESS(RwReg, 0x400E0510U) //!< (PMC) Oscillator Calibration Register
/* ========== Register definition for UART0 peripheral ========== */
#define REG_UART0_CR        REG_ACCESS(WoReg, 0x400E0600U) //!< (UART0) Control Register */
#define REG_UART0_MR        REG_ACCESS(RwReg, 0x400E0604U) //!< (UART0) Mode Register */
#define REG_UART0_IER       REG_ACCESS(WoReg, 0x400E0608U) //!< (UART0) Interrupt Enable Register */
#define REG_UART0_IDR       REG_ACCESS(WoReg, 0x400E060CU) //!< (UART0) Interrupt Disable Register */
#define REG_UART0_IMR       REG_ACCESS(RoReg, 0x400E0610U) //!< (UART0) Interrupt Mask Register */
#define REG_UART0_SR        REG_ACCESS(RoReg, 0x400E0614U) //!< (UART0) Status Register */
#define REG_UART0_RHR       REG_ACCESS(RoReg, 0x400E0618U) //!< (UART0) Receive Holding Register */
#define REG_UART0_THR       REG_ACCESS(WoReg, 0x400E061CU) //!< (UART0) Transmit Holding Register */
#define REG_UART0_BRGR      REG_ACCESS(RwReg, 0x400E0620U) //!< (UART0) Baud Rate Generator Register */
#define REG_UART0_RPR       REG_ACCESS(RwReg, 0x400E0700U) //!< (UART0) Receive Pointer Register */
#define REG_UART0_RCR       REG_ACCESS(RwReg, 0x400E0704U) //!< (UART0) Receive Counter Register */
#define REG_UART0_TPR       REG_ACCESS(RwReg, 0x400E0708U) //!< (UART0) Transmit Pointer Register */
#define REG_UART0_TCR       REG_ACCESS(RwReg, 0x400E070CU) //!< (UART0) Transmit Counter Register */
#define REG_UART0_RNPR      REG_ACCESS(RwReg, 0x400E0710U) //!< (UART0) Receive Next Pointer Register */
#define REG_UART0_RNCR      REG_ACCESS(RwReg, 0x400E0714U) //!< (UART0) Receive Next Counter Register */
#define REG_UART0_TNPR      REG_ACCESS(RwReg, 0x400E0718U) //!< (UART0) Transmit Next Pointer Register */
#define REG_UART0_TNCR      REG_ACCESS(RwReg, 0x400E071CU) //!< (UART0) Transmit Next Counter Register */
#define REG_UART0_PTCR      REG_ACCESS(WoReg, 0x400E0720U) //!< (UART0) Transfer Control Register */
#define REG_UART0_PTSR      REG_ACCESS(RoReg, 0x400E0724U) //!< (UART0) Transfer Status Register */
/* ========== Register definition for CHIPID peripheral ========== */
#define REG_CHIPID_CIDR     REG_ACCESS(RoReg, 0x400E0740U) //!< (CHIPID) Chip ID Register */
#define REG_CHIPID_EXID     REG_ACCESS(RoReg, 0x400E0744U) //!< (CHIPID) Chip ID Extension Register */
/* ========== Register definition for UART1 peripheral ========== */
#define REG_UART1_CR        REG_ACCESS(WoReg, 0x400E0800U) //!< (UART1) Control Register */
#define REG_UART1_MR        REG_ACCESS(RwReg, 0x400E0804U) //!< (UART1) Mode Register */
#define REG_UART1_IER       REG_ACCESS(WoReg, 0x400E0808U) //!< (UART1) Interrupt Enable Register */
#define REG_UART1_IDR       REG_ACCESS(WoReg, 0x400E080CU) //!< (UART1) Interrupt Disable Register */
#define REG_UART1_IMR       REG_ACCESS(RoReg, 0x400E0810U) //!< (UART1) Interrupt Mask Register */
#define REG_UART1_SR        REG_ACCESS(RoReg, 0x400E0814U) //!< (UART1) Status Register */
#define REG_UART1_RHR       REG_ACCESS(RoReg, 0x400E0818U) //!< (UART1) Receive Holding Register */
#define REG_UART1_THR       REG_ACCESS(WoReg, 0x400E081CU) //!< (UART1) Transmit Holding Register */
#define REG_UART1_BRGR      REG_ACCESS(RwReg, 0x400E0820U) //!< (UART1) Baud Rate Generator Register */
#define REG_UART1_RPR       REG_ACCESS(RwReg, 0x400E0900U) //!< (UART1) Receive Pointer Register */
#define REG_UART1_RCR       REG_ACCESS(RwReg, 0x400E0904U) //!< (UART1) Receive Counter Register */
#define REG_UART1_TPR       REG_ACCESS(RwReg, 0x400E0908U) //!< (UART1) Transmit Pointer Register */
#define REG_UART1_TCR       REG_ACCESS(RwReg, 0x400E090CU) //!< (UART1) Transmit Counter Register */
#define REG_UART1_RNPR      REG_ACCESS(RwReg, 0x400E0910U) //!< (UART1) Receive Next Pointer Register */
#define REG_UART1_RNCR      REG_ACCESS(RwReg, 0x400E0914U) //!< (UART1) Receive Next Counter Register */
#define REG_UART1_TNPR      REG_ACCESS(RwReg, 0x400E0918U) //!< (UART1) Transmit Next Pointer Register */
#define REG_UART1_TNCR      REG_ACCESS(RwReg, 0x400E091CU) //!< (UART1) Transmit Next Counter Register */
#define REG_UART1_PTCR      REG_ACCESS(WoReg, 0x400E0920U) //!< (UART1) Transfer Control Register */
#define REG_UART1_PTSR      REG_ACCESS(RoReg, 0x400E0924U) //!< (UART1) Transfer Status Register */
/* ========== Register definition for EFC peripheral ========== */
#define REG_EFC_FMR         REG_ACCESS(RwReg, 0x400E0A00U) //!< (EFC) EEFC Flash Mode Register */
#define REG_EFC_FCR         REG_ACCESS(WoReg, 0x400E0A04U) //!< (EFC) EEFC Flash Command Register */
#define REG_EFC_FSR         REG_ACCESS(RoReg, 0x400E0A08U) //!< (EFC) EEFC Flash Status Register */
#define REG_EFC_FRR         REG_ACCESS(RoReg, 0x400E0A0CU) //!< (EFC) EEFC Flash Result Register */
/* ========== Register definition for PIOA peripheral ========== */
#define REG_PIOA_PER        REG_ACCESS(WoReg, 0x400E0E00U) //!< (PIOA) PIO Enable Register */
#define REG_PIOA_PDR        REG_ACCESS(WoReg, 0x400E0E04U) //!< (PIOA) PIO Disable Register */
#define REG_PIOA_PSR        REG_ACCESS(RoReg, 0x400E0E08U) //!< (PIOA) PIO Status Register */
#define REG_PIOA_OER        REG_ACCESS(WoReg, 0x400E0E10U) //!< (PIOA) Output Enable Register */
#define REG_PIOA_ODR        REG_ACCESS(WoReg, 0x400E0E14U) //!< (PIOA) Output Disable Register */
#define REG_PIOA_OSR        REG_ACCESS(RoReg, 0x400E0E18U) //!< (PIOA) Output Status Register */
#define REG_PIOA_IFER       REG_ACCESS(WoReg, 0x400E0E20U) //!< (PIOA) Glitch Input Filter Enable Register */
#define REG_PIOA_IFDR       REG_ACCESS(WoReg, 0x400E0E24U) //!< (PIOA) Glitch Input Filter Disable Register */
#define REG_PIOA_IFSR       REG_ACCESS(RoReg, 0x400E0E28U) //!< (PIOA) Glitch Input Filter Status Register */
#define REG_PIOA_SODR       REG_ACCESS(WoReg, 0x400E0E30U) //!< (PIOA) Set Output Data Register */
#define REG_PIOA_CODR       REG_ACCESS(WoReg, 0x400E0E34U) //!< (PIOA) Clear Output Data Register */
#define REG_PIOA_ODSR       REG_ACCESS(RwReg, 0x400E0E38U) //!< (PIOA) Output Data Status Register */
#define REG_PIOA_PDSR       REG_ACCESS(RoReg, 0x400E0E3CU) //!< (PIOA) Pin Data Status Register */
#define REG_PIOA_IER        REG_ACCESS(WoReg, 0x400E0E40U) //!< (PIOA) Interrupt Enable Register */
#define REG_PIOA_IDR        REG_ACCESS(WoReg, 0x400E0E44U) //!< (PIOA) Interrupt Disable Register */
#define REG_PIOA_IMR        REG_ACCESS(RoReg, 0x400E0E48U) //!< (PIOA) Interrupt Mask Register */
#define REG_PIOA_ISR        REG_ACCESS(RoReg, 0x400E0E4CU) //!< (PIOA) Interrupt Status Register */
#define REG_PIOA_MDER       REG_ACCESS(WoReg, 0x400E0E50U) //!< (PIOA) Multi-driver Enable Register */
#define REG_PIOA_MDDR       REG_ACCESS(WoReg, 0x400E0E54U) //!< (PIOA) Multi-driver Disable Register */
#define REG_PIOA_MDSR       REG_ACCESS(RoReg, 0x400E0E58U) //!< (PIOA) Multi-driver Status Register */
#define REG_PIOA_PUDR       REG_ACCESS(WoReg, 0x400E0E60U) //!< (PIOA) Pull-up Disable Register */
#define REG_PIOA_PUER       REG_ACCESS(WoReg, 0x400E0E64U) //!< (PIOA) Pull-up Enable Register */
#define REG_PIOA_PUSR       REG_ACCESS(RoReg, 0x400E0E68U) //!< (PIOA) Pad Pull-up Status Register */
#define REG_PIOA_ABCDSR     REG_ACCESS(RwReg, 0x400E0E70U) //!< (PIOA) Peripheral Select Register */
#define REG_PIOA_IFSCDR     REG_ACCESS(WoReg, 0x400E0E80U) //!< (PIOA) Input Filter Slow Clock Disable Register */
#define REG_PIOA_IFSCER     REG_ACCESS(WoReg, 0x400E0E84U) //!< (PIOA) Input Filter Slow Clock Enable Register */
#define REG_PIOA_IFSCSR     REG_ACCESS(RoReg, 0x400E0E88U) //!< (PIOA) Input Filter Slow Clock Status Register */
#define REG_PIOA_SCDR       REG_ACCESS(RwReg, 0x400E0E8CU) //!< (PIOA) Slow Clock Divider Debouncing Register */
#define REG_PIOA_PPDDR      REG_ACCESS(WoReg, 0x400E0E90U) //!< (PIOA) Pad Pull-down Disable Register */
#define REG_PIOA_PPDER      REG_ACCESS(WoReg, 0x400E0E94U) //!< (PIOA) Pad Pull-down Enable Register */
#define REG_PIOA_PPDSR      REG_ACCESS(RoReg, 0x400E0E98U) //!< (PIOA) Pad Pull-down Status Register */
#define REG_PIOA_OWER       REG_ACCESS(WoReg, 0x400E0EA0U) //!< (PIOA) Output Write Enable */
#define REG_PIOA_OWDR       REG_ACCESS(WoReg, 0x400E0EA4U) //!< (PIOA) Output Write Disable */
#define REG_PIOA_OWSR       REG_ACCESS(RoReg, 0x400E0EA8U) //!< (PIOA) Output Write Status Register */
#define REG_PIOA_AIMER      REG_ACCESS(WoReg, 0x400E0EB0U) //!< (PIOA) Additional Interrupt Modes Enable Register */
#define REG_PIOA_AIMDR      REG_ACCESS(WoReg, 0x400E0EB4U) //!< (PIOA) Additional Interrupt Modes Disables Register */
#define REG_PIOA_AIMMR      REG_ACCESS(RoReg, 0x400E0EB8U) //!< (PIOA) Additional Interrupt Modes Mask Register */
#define REG_PIOA_ESR        REG_ACCESS(WoReg, 0x400E0EC0U) //!< (PIOA) Edge Select Register */
#define REG_PIOA_LSR        REG_ACCESS(WoReg, 0x400E0EC4U) //!< (PIOA) Level Select Register */
#define REG_PIOA_ELSR       REG_ACCESS(RoReg, 0x400E0EC8U) //!< (PIOA) Edge/Level Status Register */
#define REG_PIOA_FELLSR     REG_ACCESS(WoReg, 0x400E0ED0U) //!< (PIOA) Falling Edge/Low Level Select Register */
#define REG_PIOA_REHLSR     REG_ACCESS(WoReg, 0x400E0ED4U) //!< (PIOA) Rising Edge/ High Level Select Register */
#define REG_PIOA_FRLHSR     REG_ACCESS(RoReg, 0x400E0ED8U) //!< (PIOA) Fall/Rise - Low/High Status Register */
#define REG_PIOA_LOCKSR     REG_ACCESS(RoReg, 0x400E0EE0U) //!< (PIOA) Lock Status */
#define REG_PIOA_WPMR       REG_ACCESS(RwReg, 0x400E0EE4U) //!< (PIOA) Write Protect Mode Register */
#define REG_PIOA_WPSR       REG_ACCESS(RoReg, 0x400E0EE8U) //!< (PIOA) Write Protect Status Register */
#define REG_PIOA_SCHMITT    REG_ACCESS(RwReg, 0x400E0F00U) //!< (PIOA) Schmitt Trigger Register */
#define REG_PIOA_PCMR       REG_ACCESS(RwReg, 0x400E0F50U) //!< (PIOA) Parallel Capture Mode Register */
#define REG_PIOA_PCIER      REG_ACCESS(WoReg, 0x400E0F54U) //!< (PIOA) Parallel Capture Interrupt Enable Register */
#define REG_PIOA_PCIDR      REG_ACCESS(WoReg, 0x400E0F58U) //!< (PIOA) Parallel Capture Interrupt Disable Register */
#define REG_PIOA_PCIMR      REG_ACCESS(RoReg, 0x400E0F5CU) //!< (PIOA) Parallel Capture Interrupt Mask Register */
#define REG_PIOA_PCISR      REG_ACCESS(RoReg, 0x400E0F60U) //!< (PIOA) Parallel Capture Interrupt Status Register */
#define REG_PIOA_PCRHR      REG_ACCESS(RoReg, 0x400E0F64U) //!< (PIOA) Parallel Capture Reception Holding Register */
#define REG_PIOA_RPR        REG_ACCESS(RwReg, 0x400E0F68U) //!< (PIOA) Receive Pointer Register */
#define REG_PIOA_RCR        REG_ACCESS(RwReg, 0x400E0F6CU) //!< (PIOA) Receive Counter Register */
#define REG_PIOA_TPR        REG_ACCESS(RwReg, 0x400E0F70U) //!< (PIOA) Transmit Pointer Register */
#define REG_PIOA_TCR        REG_ACCESS(RwReg, 0x400E0F74U) //!< (PIOA) Transmit Counter Register */
#define REG_PIOA_RNPR       REG_ACCESS(RwReg, 0x400E0F78U) //!< (PIOA) Receive Next Pointer Register */
#define REG_PIOA_RNCR       REG_ACCESS(RwReg, 0x400E0F7CU) //!< (PIOA) Receive Next Counter Register */
#define REG_PIOA_TNPR       REG_ACCESS(RwReg, 0x400E0F80U) //!< (PIOA) Transmit Next Pointer Register */
#define REG_PIOA_TNCR       REG_ACCESS(RwReg, 0x400E0F84U) //!< (PIOA) Transmit Next Counter Register */
#define REG_PIOA_PTCR       REG_ACCESS(WoReg, 0x400E0F88U) //!< (PIOA) Transfer Control Register */
#define REG_PIOA_PTSR       REG_ACCESS(RoReg, 0x400E0F8CU) //!< (PIOA) Transfer Status Register */
/* ========== Register definition for PIOB peripheral ========== */
#define REG_PIOB_PER        REG_ACCESS(WoReg, 0x400E1000U) //!< (PIOB) PIO Enable Register */
#define REG_PIOB_PDR        REG_ACCESS(WoReg, 0x400E1004U) //!< (PIOB) PIO Disable Register */
#define REG_PIOB_PSR        REG_ACCESS(RoReg, 0x400E1008U) //!< (PIOB) PIO Status Register */
#define REG_PIOB_OER        REG_ACCESS(WoReg, 0x400E1010U) //!< (PIOB) Output Enable Register */
#define REG_PIOB_ODR        REG_ACCESS(WoReg, 0x400E1014U) //!< (PIOB) Output Disable Register */
#define REG_PIOB_OSR        REG_ACCESS(RoReg, 0x400E1018U) //!< (PIOB) Output Status Register */
#define REG_PIOB_IFER       REG_ACCESS(WoReg, 0x400E1020U) //!< (PIOB) Glitch Input Filter Enable Register */
#define REG_PIOB_IFDR       REG_ACCESS(WoReg, 0x400E1024U) //!< (PIOB) Glitch Input Filter Disable Register */
#define REG_PIOB_IFSR       REG_ACCESS(RoReg, 0x400E1028U) //!< (PIOB) Glitch Input Filter Status Register */
#define REG_PIOB_SODR       REG_ACCESS(WoReg, 0x400E1030U) //!< (PIOB) Set Output Data Register */
#define REG_PIOB_CODR       REG_ACCESS(WoReg, 0x400E1034U) //!< (PIOB) Clear Output Data Register */
#define REG_PIOB_ODSR       REG_ACCESS(RwReg, 0x400E1038U) //!< (PIOB) Output Data Status Register */
#define REG_PIOB_PDSR       REG_ACCESS(RoReg, 0x400E103CU) //!< (PIOB) Pin Data Status Register */
#define REG_PIOB_IER        REG_ACCESS(WoReg, 0x400E1040U) //!< (PIOB) Interrupt Enable Register */
#define REG_PIOB_IDR        REG_ACCESS(WoReg, 0x400E1044U) //!< (PIOB) Interrupt Disable Register */
#define REG_PIOB_IMR        REG_ACCESS(RoReg, 0x400E1048U) //!< (PIOB) Interrupt Mask Register */
#define REG_PIOB_ISR        REG_ACCESS(RoReg, 0x400E104CU) //!< (PIOB) Interrupt Status Register */
#define REG_PIOB_MDER       REG_ACCESS(WoReg, 0x400E1050U) //!< (PIOB) Multi-driver Enable Register */
#define REG_PIOB_MDDR       REG_ACCESS(WoReg, 0x400E1054U) //!< (PIOB) Multi-driver Disable Register */
#define REG_PIOB_MDSR       REG_ACCESS(RoReg, 0x400E1058U) //!< (PIOB) Multi-driver Status Register */
#define REG_PIOB_PUDR       REG_ACCESS(WoReg, 0x400E1060U) //!< (PIOB) Pull-up Disable Register */
#define REG_PIOB_PUER       REG_ACCESS(WoReg, 0x400E1064U) //!< (PIOB) Pull-up Enable Register */
#define REG_PIOB_PUSR       REG_ACCESS(RoReg, 0x400E1068U) //!< (PIOB) Pad Pull-up Status Register */
#define REG_PIOB_ABCDSR     REG_ACCESS(RwReg, 0x400E1070U) //!< (PIOB) Peripheral Select Register */
#define REG_PIOB_IFSCDR     REG_ACCESS(WoReg, 0x400E1080U) //!< (PIOB) Input Filter Slow Clock Disable Register */
#define REG_PIOB_IFSCER     REG_ACCESS(WoReg, 0x400E1084U) //!< (PIOB) Input Filter Slow Clock Enable Register */
#define REG_PIOB_IFSCSR     REG_ACCESS(RoReg, 0x400E1088U) //!< (PIOB) Input Filter Slow Clock Status Register */
#define REG_PIOB_SCDR       REG_ACCESS(RwReg, 0x400E108CU) //!< (PIOB) Slow Clock Divider Debouncing Register */
#define REG_PIOB_PPDDR      REG_ACCESS(WoReg, 0x400E1090U) //!< (PIOB) Pad Pull-down Disable Register */
#define REG_PIOB_PPDER      REG_ACCESS(WoReg, 0x400E1094U) //!< (PIOB) Pad Pull-down Enable Register */
#define REG_PIOB_PPDSR      REG_ACCESS(RoReg, 0x400E1098U) //!< (PIOB) Pad Pull-down Status Register */
#define REG_PIOB_OWER       REG_ACCESS(WoReg, 0x400E10A0U) //!< (PIOB) Output Write Enable */
#define REG_PIOB_OWDR       REG_ACCESS(WoReg, 0x400E10A4U) //!< (PIOB) Output Write Disable */
#define REG_PIOB_OWSR       REG_ACCESS(RoReg, 0x400E10A8U) //!< (PIOB) Output Write Status Register */
#define REG_PIOB_AIMER      REG_ACCESS(WoReg, 0x400E10B0U) //!< (PIOB) Additional Interrupt Modes Enable Register */
#define REG_PIOB_AIMDR      REG_ACCESS(WoReg, 0x400E10B4U) //!< (PIOB) Additional Interrupt Modes Disables Register */
#define REG_PIOB_AIMMR      REG_ACCESS(RoReg, 0x400E10B8U) //!< (PIOB) Additional Interrupt Modes Mask Register */
#define REG_PIOB_ESR        REG_ACCESS(WoReg, 0x400E10C0U) //!< (PIOB) Edge Select Register */
#define REG_PIOB_LSR        REG_ACCESS(WoReg, 0x400E10C4U) //!< (PIOB) Level Select Register */
#define REG_PIOB_ELSR       REG_ACCESS(RoReg, 0x400E10C8U) //!< (PIOB) Edge/Level Status Register */
#define REG_PIOB_FELLSR     REG_ACCESS(WoReg, 0x400E10D0U) //!< (PIOB) Falling Edge/Low Level Select Register */
#define REG_PIOB_REHLSR     REG_ACCESS(WoReg, 0x400E10D4U) //!< (PIOB) Rising Edge/ High Level Select Register */
#define REG_PIOB_FRLHSR     REG_ACCESS(RoReg, 0x400E10D8U) //!< (PIOB) Fall/Rise - Low/High Status Register */
#define REG_PIOB_LOCKSR     REG_ACCESS(RoReg, 0x400E10E0U) //!< (PIOB) Lock Status */
#define REG_PIOB_WPMR       REG_ACCESS(RwReg, 0x400E10E4U) //!< (PIOB) Write Protect Mode Register */
#define REG_PIOB_WPSR       REG_ACCESS(RoReg, 0x400E10E8U) //!< (PIOB) Write Protect Status Register */
#define REG_PIOB_SCHMITT    REG_ACCESS(RwReg, 0x400E1100U) //!< (PIOB) Schmitt Trigger Register */
#define REG_PIOB_PCMR       REG_ACCESS(RwReg, 0x400E1150U) //!< (PIOB) Parallel Capture Mode Register */
#define REG_PIOB_PCIER      REG_ACCESS(WoReg, 0x400E1154U) //!< (PIOB) Parallel Capture Interrupt Enable Register */
#define REG_PIOB_PCIDR      REG_ACCESS(WoReg, 0x400E1158U) //!< (PIOB) Parallel Capture Interrupt Disable Register */
#define REG_PIOB_PCIMR      REG_ACCESS(RoReg, 0x400E115CU) //!< (PIOB) Parallel Capture Interrupt Mask Register */
#define REG_PIOB_PCISR      REG_ACCESS(RoReg, 0x400E1160U) //!< (PIOB) Parallel Capture Interrupt Status Register */
#define REG_PIOB_PCRHR      REG_ACCESS(RoReg, 0x400E1164U) //!< (PIOB) Parallel Capture Reception Holding Register */
/* ========== Register definition for PIOC peripheral ========== */
#define REG_PIOC_PER        REG_ACCESS(WoReg, 0x400E1200U) //!< (PIOC) PIO Enable Register */
#define REG_PIOC_PDR        REG_ACCESS(WoReg, 0x400E1204U) //!< (PIOC) PIO Disable Register */
#define REG_PIOC_PSR        REG_ACCESS(RoReg, 0x400E1208U) //!< (PIOC) PIO Status Register */
#define REG_PIOC_OER        REG_ACCESS(WoReg, 0x400E1210U) //!< (PIOC) Output Enable Register */
#define REG_PIOC_ODR        REG_ACCESS(WoReg, 0x400E1214U) //!< (PIOC) Output Disable Register */
#define REG_PIOC_OSR        REG_ACCESS(RoReg, 0x400E1218U) //!< (PIOC) Output Status Register */
#define REG_PIOC_IFER       REG_ACCESS(WoReg, 0x400E1220U) //!< (PIOC) Glitch Input Filter Enable Register */
#define REG_PIOC_IFDR       REG_ACCESS(WoReg, 0x400E1224U) //!< (PIOC) Glitch Input Filter Disable Register */
#define REG_PIOC_IFSR       REG_ACCESS(RoReg, 0x400E1228U) //!< (PIOC) Glitch Input Filter Status Register */
#define REG_PIOC_SODR       REG_ACCESS(WoReg, 0x400E1230U) //!< (PIOC) Set Output Data Register */
#define REG_PIOC_CODR       REG_ACCESS(WoReg, 0x400E1234U) //!< (PIOC) Clear Output Data Register */
#define REG_PIOC_ODSR       REG_ACCESS(RwReg, 0x400E1238U) //!< (PIOC) Output Data Status Register */
#define REG_PIOC_PDSR       REG_ACCESS(RoReg, 0x400E123CU) //!< (PIOC) Pin Data Status Register */
#define REG_PIOC_IER        REG_ACCESS(WoReg, 0x400E1240U) //!< (PIOC) Interrupt Enable Register */
#define REG_PIOC_IDR        REG_ACCESS(WoReg, 0x400E1244U) //!< (PIOC) Interrupt Disable Register */
#define REG_PIOC_IMR        REG_ACCESS(RoReg, 0x400E1248U) //!< (PIOC) Interrupt Mask Register */
#define REG_PIOC_ISR        REG_ACCESS(RoReg, 0x400E124CU) //!< (PIOC) Interrupt Status Register */
#define REG_PIOC_MDER       REG_ACCESS(WoReg, 0x400E1250U) //!< (PIOC) Multi-driver Enable Register */
#define REG_PIOC_MDDR       REG_ACCESS(WoReg, 0x400E1254U) //!< (PIOC) Multi-driver Disable Register */
#define REG_PIOC_MDSR       REG_ACCESS(RoReg, 0x400E1258U) //!< (PIOC) Multi-driver Status Register */
#define REG_PIOC_PUDR       REG_ACCESS(WoReg, 0x400E1260U) //!< (PIOC) Pull-up Disable Register */
#define REG_PIOC_PUER       REG_ACCESS(WoReg, 0x400E1264U) //!< (PIOC) Pull-up Enable Register */
#define REG_PIOC_PUSR       REG_ACCESS(RoReg, 0x400E1268U) //!< (PIOC) Pad Pull-up Status Register */
#define REG_PIOC_ABCDSR     REG_ACCESS(RwReg, 0x400E1270U) //!< (PIOC) Peripheral Select Register */
#define REG_PIOC_IFSCDR     REG_ACCESS(WoReg, 0x400E1280U) //!< (PIOC) Input Filter Slow Clock Disable Register */
#define REG_PIOC_IFSCER     REG_ACCESS(WoReg, 0x400E1284U) //!< (PIOC) Input Filter Slow Clock Enable Register */
#define REG_PIOC_IFSCSR     REG_ACCESS(RoReg, 0x400E1288U) //!< (PIOC) Input Filter Slow Clock Status Register */
#define REG_PIOC_SCDR       REG_ACCESS(RwReg, 0x400E128CU) //!< (PIOC) Slow Clock Divider Debouncing Register */
#define REG_PIOC_PPDDR      REG_ACCESS(WoReg, 0x400E1290U) //!< (PIOC) Pad Pull-down Disable Register */
#define REG_PIOC_PPDER      REG_ACCESS(WoReg, 0x400E1294U) //!< (PIOC) Pad Pull-down Enable Register */
#define REG_PIOC_PPDSR      REG_ACCESS(RoReg, 0x400E1298U) //!< (PIOC) Pad Pull-down Status Register */
#define REG_PIOC_OWER       REG_ACCESS(WoReg, 0x400E12A0U) //!< (PIOC) Output Write Enable */
#define REG_PIOC_OWDR       REG_ACCESS(WoReg, 0x400E12A4U) //!< (PIOC) Output Write Disable */
#define REG_PIOC_OWSR       REG_ACCESS(RoReg, 0x400E12A8U) //!< (PIOC) Output Write Status Register */
#define REG_PIOC_AIMER      REG_ACCESS(WoReg, 0x400E12B0U) //!< (PIOC) Additional Interrupt Modes Enable Register */
#define REG_PIOC_AIMDR      REG_ACCESS(WoReg, 0x400E12B4U) //!< (PIOC) Additional Interrupt Modes Disables Register */
#define REG_PIOC_AIMMR      REG_ACCESS(RoReg, 0x400E12B8U) //!< (PIOC) Additional Interrupt Modes Mask Register */
#define REG_PIOC_ESR        REG_ACCESS(WoReg, 0x400E12C0U) //!< (PIOC) Edge Select Register */
#define REG_PIOC_LSR        REG_ACCESS(WoReg, 0x400E12C4U) //!< (PIOC) Level Select Register */
#define REG_PIOC_ELSR       REG_ACCESS(RoReg, 0x400E12C8U) //!< (PIOC) Edge/Level Status Register */
#define REG_PIOC_FELLSR     REG_ACCESS(WoReg, 0x400E12D0U) //!< (PIOC) Falling Edge/Low Level Select Register */
#define REG_PIOC_REHLSR     REG_ACCESS(WoReg, 0x400E12D4U) //!< (PIOC) Rising Edge/ High Level Select Register */
#define REG_PIOC_FRLHSR     REG_ACCESS(RoReg, 0x400E12D8U) //!< (PIOC) Fall/Rise - Low/High Status Register */
#define REG_PIOC_LOCKSR     REG_ACCESS(RoReg, 0x400E12E0U) //!< (PIOC) Lock Status */
#define REG_PIOC_WPMR       REG_ACCESS(RwReg, 0x400E12E4U) //!< (PIOC) Write Protect Mode Register */
#define REG_PIOC_WPSR       REG_ACCESS(RoReg, 0x400E12E8U) //!< (PIOC) Write Protect Status Register */
#define REG_PIOC_SCHMITT    REG_ACCESS(RwReg, 0x400E1300U) //!< (PIOC) Schmitt Trigger Register */
#define REG_PIOC_PCMR       REG_ACCESS(RwReg, 0x400E1350U) //!< (PIOC) Parallel Capture Mode Register */
#define REG_PIOC_PCIER      REG_ACCESS(WoReg, 0x400E1354U) //!< (PIOC) Parallel Capture Interrupt Enable Register */
#define REG_PIOC_PCIDR      REG_ACCESS(WoReg, 0x400E1358U) //!< (PIOC) Parallel Capture Interrupt Disable Register */
#define REG_PIOC_PCIMR      REG_ACCESS(RoReg, 0x400E135CU) //!< (PIOC) Parallel Capture Interrupt Mask Register */
#define REG_PIOC_PCISR      REG_ACCESS(RoReg, 0x400E1360U) //!< (PIOC) Parallel Capture Interrupt Status Register */
#define REG_PIOC_PCRHR      REG_ACCESS(RoReg, 0x400E1364U) //!< (PIOC) Parallel Capture Reception Holding Register */
/* ========== Register definition for RSTC peripheral ========== */
#define REG_RSTC_CR         REG_ACCESS(WoReg, 0x400E1400U) //!< (RSTC) Control Register */
#define REG_RSTC_SR         REG_ACCESS(RoReg, 0x400E1404U) //!< (RSTC) Status Register */
#define REG_RSTC_MR         REG_ACCESS(RwReg, 0x400E1408U) //!< (RSTC) Mode Register */
/* ========== Register definition for SUPC peripheral ========== */
#define REG_SUPC_CR         REG_ACCESS(WoReg, 0x400E1410U) //!< (SUPC) Supply Controller Control Register */
#define REG_SUPC_SMMR       REG_ACCESS(RwReg, 0x400E1414U) //!< (SUPC) Supply Controller Supply Monitor Mode Register */
#define REG_SUPC_MR         REG_ACCESS(RwReg, 0x400E1418U) //!< (SUPC) Supply Controller Mode Register */
#define REG_SUPC_WUMR       REG_ACCESS(RwReg, 0x400E141CU) //!< (SUPC) Supply Controller Wake Up Mode Register */
#define REG_SUPC_WUIR       REG_ACCESS(RwReg, 0x400E1420U) //!< (SUPC) Supply Controller Wake Up Inputs Register */
#define REG_SUPC_SR         REG_ACCESS(RoReg, 0x400E1424U) //!< (SUPC) Supply Controller Status Register */
/* ========== Register definition for RTT peripheral ========== */
#define REG_RTT_MR          REG_ACCESS(RwReg, 0x400E1430U) //!< (RTT) Mode Register */
#define REG_RTT_AR          REG_ACCESS(RwReg, 0x400E1434U) //!< (RTT) Alarm Register */
#define REG_RTT_VR          REG_ACCESS(RoReg, 0x400E1438U) //!< (RTT) Value Register */
#define REG_RTT_SR          REG_ACCESS(RoReg, 0x400E143CU) //!< (RTT) Status Register */
/* ========== Register definition for WDT peripheral ========== */
#define REG_WDT_CR          REG_ACCESS(WoReg, 0x400E1450U) //!< (WDT) Control Register */
#define REG_WDT_MR          REG_ACCESS(RwReg, 0x400E1454U) //!< (WDT) Mode Register */
#define REG_WDT_SR          REG_ACCESS(RoReg, 0x400E1458U) //!< (WDT) Status Register */
/* ========== Register definition for RTC peripheral ========== */
#define REG_RTC_CR          REG_ACCESS(RwReg, 0x400E1460U) //!< (RTC) Control Register */
#define REG_RTC_MR          REG_ACCESS(RwReg, 0x400E1464U) //!< (RTC) Mode Register */
#define REG_RTC_TIMR        REG_ACCESS(RwReg, 0x400E1468U) //!< (RTC) Time Register */
#define REG_RTC_CALR        REG_ACCESS(RwReg, 0x400E146CU) //!< (RTC) Calendar Register */
#define REG_RTC_TIMALR      REG_ACCESS(RwReg, 0x400E1470U) //!< (RTC) Time Alarm Register */
#define REG_RTC_CALALR      REG_ACCESS(RwReg, 0x400E1474U) //!< (RTC) Calendar Alarm Register */
#define REG_RTC_SR          REG_ACCESS(RoReg, 0x400E1478U) //!< (RTC) Status Register */
#define REG_RTC_SCCR        REG_ACCESS(WoReg, 0x400E147CU) //!< (RTC) Status Clear Command Register */
#define REG_RTC_IER         REG_ACCESS(WoReg, 0x400E1480U) //!< (RTC) Interrupt Enable Register */
#define REG_RTC_IDR         REG_ACCESS(WoReg, 0x400E1484U) //!< (RTC) Interrupt Disable Register */
#define REG_RTC_IMR         REG_ACCESS(RoReg, 0x400E1488U) //!< (RTC) Interrupt Mask Register */
#define REG_RTC_VER         REG_ACCESS(RoReg, 0x400E148CU) //!< (RTC) Valid Entry Register */
/* ========== Register definition for GPBR peripheral ========== */
#define REG_GPBR_GPBR0      REG_ACCESS(RwReg, 0x400E1490U) //!< (GPBR) General Purpose Backup Register 0 */
#define REG_GPBR_GPBR1      REG_ACCESS(RwReg, 0x400E1494U) //!< (GPBR) General Purpose Backup Register 1 */
#define REG_GPBR_GPBR2      REG_ACCESS(RwReg, 0x400E1498U) //!< (GPBR) General Purpose Backup Register 2 */
#define REG_GPBR_GPBR3      REG_ACCESS(RwReg, 0x400E149CU) //!< (GPBR) General Purpose Backup Register 3 */
#define REG_GPBR_GPBR4      REG_ACCESS(RwReg, 0x400E14A0U) //!< (GPBR) General Purpose Backup Register 4 */
#define REG_GPBR_GPBR5      REG_ACCESS(RwReg, 0x400E14A4U) //!< (GPBR) General Purpose Backup Register 5 */
#define REG_GPBR_GPBR6      REG_ACCESS(RwReg, 0x400E14A8U) //!< (GPBR) General Purpose Backup Register 6 */
#define REG_GPBR_GPBR7      REG_ACCESS(RwReg, 0x400E14ACU) //!< (GPBR) General Purpose Backup Register 7 */


/** @} */

#endif /* SERIES_MAP_H_ */
