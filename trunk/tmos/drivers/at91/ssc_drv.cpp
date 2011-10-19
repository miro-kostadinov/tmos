/**************************************************************************//**
 * @file     ssc_drv.cpp
 * @ingroup  DRV_AT91_SSC
 * @brief    SSC driver implementation
 * @version  V3.00
 * @date     06. February 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include <ssc_drv.h>
#include <platform_drv.h>
#include <fam_cpp.h>

/** Enable SSC
 *
 * @param drv_info
 * @param mode
 */
static void SSC_ENABLE(SSC_INFO drv_info, const SSC_MODE_STRU* mode)
{
	Ssc* pSSC;

	pSSC = drv_info->hw_base;

//	PIO_CfgPeriph(&drv_info->ssc_pins);

	//enable periph clock
	drv_pmc_enable(&drv_info->info);

	//* Disable interrupts
	pSSC->SSC_IDR = (unsigned int) -1;

	//* Reset receiver and transmitter
	pSSC->SSC_CR = SSC_CR_SWRST | SSC_CR_RXDIS | SSC_CR_TXDIS;

	//* Define the Clock Mode Register
	pSSC->SSC_CMR = 1+ (system_clock_frequency / mode->baudrate )/2;;

	//* Write the Receive Clock Mode Register
	pSSC->SSC_RCMR = SSC_RCMR_CKO_TRANSFER | SSC_RCMR_CKS_MCK
			|SSC_RCMR_START_TRANSMIT;

	//* Write the Transmit Clock Mode Register
	pSSC->SSC_TCMR = SSC_TCMR_CKO_TRANSFER | SSC_TCMR_CKS_MCK
			|SSC_TCMR_CKG_TRANSFER | SSC_TCMR_START_CONTINUOUS;

	//* Write the Receive Frame Mode Register
	pSSC->SSC_RFMR = SSC_RFMR_DATLEN(7);

	//* Write the Transmit Frame Mode Register
	pSSC->SSC_TFMR = SSC_TFMR_DATLEN(7);

	//* Clear Transmit and Receive Counters
	pSSC->SSC_PTCR = SSC_PTCR_TXTEN;

	//* Enable  transmitter
	pSSC->SSC_CR = SSC_CR_TXEN | SSC_CR_RXEN;

	//* Enable interrupts
	drv_isr_enable(&drv_info->info);
}

/** SSC Driver DCR routine  **/
void SSC_DCR(SSC_INFO drv_info, unsigned int reason, HANDLE param)
{
	switch (reason)
	{
	case DCR_RESET:

		//enable periph clock
		drv_pmc_disable(&drv_info->info);
		break;

	case DCR_OPEN:
		const SSC_MODE_STRU * mode;
		mode = (const SSC_MODE_STRU *) param->mode.as_cvoidptr;
		if (mode)
		{
			if (!drv_info->drv_data->cnt++)
			{
				SSC_ENABLE(drv_info, mode);
			}
			PIO_CfgOutput1(&mode->cs_pin);
			param->res = RES_OK;
		}
		break;
	}
}

/** SSC Driver DSR routine **/
void SSC_DSR(SSC_INFO drv_info, HANDLE hnd)
{
	SSC_DRIVER_DATA drv_data = drv_info->drv_data;

	if (drv_data->pending)
	{
		//but is busy
		hnd->list_add(drv_data->waiting);
	}
	else
	{
		Ssc* pSSC;

		drv_data->pending = hnd;
		PIO_ClrOutput(&((const SSC_MODE_STRU *) hnd->mode.as_cvoidptr)->cs_pin);
		pSSC = drv_info->hw_base;
		pSSC->SSC_TPR = hnd->src.as_int;
		pSSC->SSC_TCR = hnd->len;
		pSSC->SSC_IER = SSC_IER_TXEMPTY;
	}

}

/** SSC Driver interrupt **/
void SSC_ISR(SSC_INFO drv_info)
{
	Ssc* pSSC;
	SSC_DRIVER_DATA drv_data;
	HANDLE hnd;

	pSSC = drv_info->hw_base;
	drv_data = drv_info->drv_data;

	//read SSC_SR
	if (pSSC->SSC_SR & SSC_SR_TXEMPTY)
	{
		if ((hnd = drv_data->pending))
		{
			PIO_SetOutput(
					&((const SSC_MODE_STRU *) hnd->mode.as_cvoidptr)->cs_pin);
			hnd->len = 0;
			drv_data->pending = NULL;
			usr_HND_SET_STATUS(hnd, RES_SIG_OK);
		}

		if ((hnd = drv_data->waiting))
		{
			drv_data->waiting = hnd->next;
			drv_data->pending = hnd;
			PIO_ClrOutput(
					&((const SSC_MODE_STRU *) &hnd->mode.as_cvoidptr)->cs_pin);
			pSSC->SSC_TPR = hnd->src.as_int;
			pSSC->SSC_TCR = hnd->len;
		}
		else
		{
			pSSC->SSC_IDR = SSC_IDR_TXEMPTY;
		}
	}
}
