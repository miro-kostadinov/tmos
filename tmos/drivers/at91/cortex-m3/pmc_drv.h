/**
 * @ingroup	 drv_at91_cm3
 * @defgroup DRV_AT91_PMC Power Management Controller Driver
 * Interface for Power Management Controller (PMC) driver.
 * @{
 *
 *
 * @file     drivers/at91/cortex-m3/pmc_drv.h
 * @ingroup	 DRV_AT91_PMC
 * @brief    PMC driver header
 *
 * @date     29. November 2010
 * @author	 Miroslav Kostadinov
 * @}
*/
#ifndef PMC_DRV_H_
#define PMC_DRV_H_


#include <tmos.h>
#include <pmc.h>

#ifdef __cplusplus
 extern "C" {
#endif

struct PMC_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	Pmc*				hw_base;		//!< pointer to the hardware peripheral
	unsigned int 	CFG_CKGR_MOR;		//!< slow clock source and settings (PMC_MOR)
	unsigned int 	CFG_CKGR_PLLAR;		//!< PLLA clock settings )
	unsigned int 	CFG_FREQUENCIES[4]; //!< Base frequency for each clock source
	unsigned int	CFG_BOOT_CLK;		//!< boot clock
};

/** PMC Driver Info */
typedef const PMC_DRIVER_INFO* PMC_INFO;

void PMC_DCR(PMC_INFO drv_info, unsigned int reason, HANDLE hnd);
void PMC_DSR(PMC_INFO drv_info, HANDLE hnd);
void PMC_ISR(PMC_INFO drv_info );

extern volatile __no_init unsigned int sysdrv_clock_frequency;

#ifdef __cplusplus
}
#endif


#endif /* PMC_DRV_H_ */
