/**************************************************************************//**
 * @ingroup	 hardware_stm32f2_flash
 * @{
 * @file     hardware/stm32/stm32f2/flash_f2.cpp
 * @brief    STM32F2 Flash
 * @version  V3.00
 * @date     13. December 2013
 * @author	 Miroslav Kostadinov
 * @}
 */

#include <tmos.h>
#include <flash_h7.h>
#include <mcu_cpp.h>


uint32_t flash_wait_ready(FLASH_BankDef* bank)
{
	uint32_t status;

	do
	{
		status = bank->FLASH_SR;
	} while (status & (FLASH_SR_QW | FLASH_SR_BSY | FLASH_SR_CRC_BUSY));

	status &= FLASH_SR_ERRORS;
	if (status)
		bank->FLASH_CCR = status;

	return status;
}


uint32_t flash_wait_ready()
{
	uint32_t status;

	status = flash_wait_ready(&FLASH->fl_bank[0]);
	if(!status)
		status = flash_wait_ready(&FLASH->fl_bank[1]);

	return status;
}


uint32_t flash_unlock(FLASH_BankDef* bank)
{
	uint32_t status;


	status = bank->FLASH_CR & FLASH_CR_LOCK;
	if (status)
	{
		bank->FLASH_KEYR = FLASH_KEYR_KEYR_1;
		bank->FLASH_KEYR = FLASH_KEYR_KEYR_2;
		status = bank->FLASH_CR & FLASH_CR_LOCK;
	}
	return status;
}


void flash_lock(FLASH_BankDef* bank)
{
	bank->FLASH_CR |= FLASH_CR_LOCK;
}

uint32_t flash_erase_sector(uint32_t address)
{
	uint32_t status;
	FLASH_BankDef* bank = nullptr;

	if(address & (FLASH_SECTOR_SIZE-1))
		return FLASH_SR_ERRORS;
	if(address >= BASE_FLASH_BANK1)
	{
		if( address < BASE_FLASH_BANK2)
		{
			bank = &FLASH->fl_bank[0];
		} else
		{
			if(address < (BASE_FLASH + FLASH_SIZE))
				bank = &FLASH->fl_bank[0];
		}
	}
	if(bank == nullptr)
		return FLASH_SR_ERRORS;


	/* Wait for last operation to be completed */
	status = flash_wait_ready(bank);
	if(status)
		return status;

    /* reset Program/erase  */
	address >>= 17-8;
	address &= FLASH_CR_SNB;
	address |= FLASH_CR_SER | (bank->FLASH_CR & ~(FLASH_CR_SNB));
	bank->FLASH_CR = address;
	bank->FLASH_CR = address | FLASH_CR_START;

    	/* Wait for last operation to be completed */
	status = flash_wait_ready(bank);

    /* If the erase operation is completed, disable the SER Bit */
    bank->FLASH_CR &= (~(FLASH_CR_SER | FLASH_CR_SNB));

	return status;
}

uint32_t flash_mass_erase(uint32_t banks)
{
	uint32_t status;
	FLASH_BankDef* bank0;
	FLASH_BankDef* bank1;

	bank0 = &FLASH->fl_bank[0];
	bank1 = &FLASH->fl_bank[1];
	switch (banks)
	{
	case 1:
		status = flash_wait_ready(bank0);
		break;
	case 2:
		status = flash_wait_ready(bank1);
		break;
	case 3:
		status = flash_wait_ready();
		break;
	default:
		status = FLASH_SR_ERRORS;
	}

	if (!status)
	{
		// start erasing

		if (banks & 1)
			bank0->FLASH_CR &= (~FLASH_CR_PSIZE);

		if (banks & 2)
			bank1->FLASH_CR &= (~FLASH_CR_PSIZE);

		if (banks == 3)
		{
			/* Set voltage range */
			bank0->FLASH_CR |= FLASH_CR_PSIZE_x32;
			bank1->FLASH_CR |= FLASH_CR_PSIZE_x32;

			/* Set Mass Erase Bit */
			bank0->FLASH_OPTCR |= FLASH_OPTCR_MER;

		}
		else
		{
			if (banks & 1)
			{
				/* Bank1 will be erased, and set voltage range */
				bank0->FLASH_CR |= FLASH_CR_BER | FLASH_CR_PSIZE_x32;
				bank0->FLASH_CR |= FLASH_CR_START;
			}
			if (banks & 2)
			{
				/* Bank2 will be erased, and set voltage range */
				bank1->FLASH_CR |= FLASH_CR_BER | FLASH_CR_PSIZE_x32;
				bank1->FLASH_CR |= FLASH_CR_START;
			}

		}

		if (banks & 1)
		{
			status = flash_wait_ready(bank0);
			// stop erasing
			bank0->FLASH_CR &= (~FLASH_CR_BER);
		}
		if (banks & 2)
		{
			status |= flash_wait_ready(bank1);
			// stop erasing
			bank1->FLASH_CR &= (~FLASH_CR_BER);
		}

	}
	return status;
}



uint32_t flash_write256b(uint32_t dst,	uint32_t src)
{
	uint32_t status;
	FLASH_BankDef* bank = nullptr;

	if(dst >= BASE_FLASH_BANK1)
	{
		if( dst < BASE_FLASH_BANK2)
		{
			bank = &FLASH->fl_bank[0];
		} else
		{
			if(dst < (BASE_FLASH + FLASH_SIZE))
				bank = &FLASH->fl_bank[0];
		}
	}
	if(bank == nullptr)
		return FLASH_SR_ERRORS;


	/* Wait for last operation to be completed */
	status = flash_wait_ready(bank);
	if(status)
		return status;

	/* Set PG bit */
	bank->FLASH_CR |= FLASH_CR_PG;

	__ISB();
	__DSB();

	/* Program the 256 bits flash word */
	for(uint32_t i=0; i<FLASH_NB_32BITWORD_IN_FLASHWORD; i++)
	{
		*(uint32_t*)dst = *(uint32_t*)src;
		dst += 4;
		src += 4;
	}

	__ISB();
	__DSB();

	/* Wait for last operation to be completed */
	status = flash_wait_ready(bank);

	/* disable the PG */
	bank->FLASH_CR &= ~FLASH_CR_PG;

	return status;
}


/**
 * Prepare for ob write
 *
 * To modify user option bytes, follow the sequence below
 * 	1. call flash_ob_chage_start()
 * 	2. change some FLASH_XXX_PRG
 * 	3. call flash_ob_chage_end()
 * @return
 */
uint32_t flash_ob_chage_start()
{
	uint32_t status;
	FLASH_BankDef* bank0;


	// both banks must be ready
	status = flash_wait_ready();
	if(!status)
	{
		bank0 = &FLASH->fl_bank[0];

		// unlock FLASH_OPTCR register
		status = bank0->FLASH_OPTCR & FLASH_OPTCR_OPTLOCK;
		if (!status)
		{
			bank0->FLASH_OPTKEYR = FLASH_OPTKEYR_OPTKEYR_1;
			bank0->FLASH_OPTKEYR = FLASH_OPTKEYR_OPTKEYR_2;
			status = bank0->FLASH_OPTCR & FLASH_OPTCR_OPTLOCK;
		}

	}
	return status;
}

uint32_t flash_ob_chage_end()
{
	uint32_t status;

	status = flash_wait_ready();
	if(!status)
	{
		// start change
		FLASH->fl_bank[0].FLASH_OPTCR |= FLASH_OPTCR_OPTSTART;

		// what for change
		status = flash_wait_ready();
	}

	// ob lock
	FLASH->fl_bank[0].FLASH_OPTCR |= FLASH_OPTCR_OPTLOCK;

	return status;
}

