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
#include <flash_f4.h>
#include <mcu_cpp.h>


static uint32_t flash_wait_ready()
{
	uint32_t status;

	do
	{
		status = FLASH->FLASH_SR;
	} while (status & FLASH_SR_BSY);

	if (status)
		FLASH->FLASH_SR = status;
	return status;
}

uint32_t flash_unlock()
{
	uint32_t status;

	//disable and reset caches
	status = FLASH->FLASH_ACR;
	status &= ~(FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN);
	FLASH->FLASH_ACR = status;
	FLASH->FLASH_ACR = status | FLASH_ACR_ICRST | FLASH_ACR_DCRST;
	FLASH->FLASH_ACR = status;

	status = FLASH->FLASH_CR & FLASH_CR_LOCK;
	if (status)
	{
		FLASH->FLASH_KEYR = FLASH_KEYR_KEY_1;
		FLASH->FLASH_KEYR = FLASH_KEYR_KEY_2;
		status = FLASH->FLASH_CR & FLASH_CR_LOCK;
	}
	return status;
}

void flash_lock()
{
	FLASH->FLASH_CR |= FLASH_CR_LOCK;
}

uint32_t flash_erase_page(uint32_t address)
{
	uint32_t status;

	status = flash_wait_ready();
	if ( !(status & FLASH_SR_ERRORS))
	{
		// start erasing

		// sector must be aligned to 16K
		if(address & 0x3FFF)
			return FLASH_SR_BSY;

		address >>= 14;
		if(address > 3)
		{
			// sectors 4-11 must be aligned to 64K
			if(address & 3)
				return FLASH_SR_BSY;

			if(address > 4)
			{
				// sectors 5-11 must be aligned to 128K
				if((address & 7) || (address > 0x38))
					return FLASH_SR_BSY;
				address >>= 3;
				address += 4;
			}
		}
		address <<= 3;	// convert sector 0-11 to FLASH_CR_SNB

		address |= FLASH_CR_PSIZE_x32 | FLASH_CR_SER;
		FLASH->FLASH_CR = address;
		FLASH->FLASH_CR = address | FLASH_CR_STRT;

		status = flash_wait_ready();

		// stop erasing
		FLASH->FLASH_CR = 0;
	}
	return status;

}

uint32_t flash_erase_all()
{
	uint32_t status;

	status = flash_wait_ready();
	if ( !(status & FLASH_SR_ERRORS))
	{
		// start erasing

		FLASH->FLASH_CR = FLASH_CR_PSIZE_x32| FLASH_CR_MER;
		FLASH->FLASH_CR = FLASH_CR_PSIZE_x32| FLASH_CR_MER | FLASH_CR_STRT;

		status = flash_wait_ready();

		// stop erasing
		FLASH->FLASH_CR = 0;
	}
	return status;
}

uint32_t flash_write_byte(uint32_t address, uint32_t value)
{
	uint32_t status;

	status = flash_wait_ready();
	if ( !(status & FLASH_SR_ERRORS))
	{
		// start programming
		FLASH->FLASH_CR = FLASH_CR_PSIZE_x8 | FLASH_CR_PG;

		*(volatile uint8_t*) address = value;

		status = flash_wait_ready();

		// stop programming
		FLASH->FLASH_CR = 0;
	}

	return status;
}

uint32_t flash_write_hword(uint32_t address, uint32_t value)
{
	uint32_t status;

	status = flash_wait_ready();
	if ( !(status & FLASH_SR_ERRORS))
	{
		// start programming
		FLASH->FLASH_CR = FLASH_CR_PSIZE_x16 | FLASH_CR_PG;

		*(volatile uint16_t*) address = value;

		status = flash_wait_ready();

		// stop programming
		FLASH->FLASH_CR = 0;
	}

	return status;
}

uint32_t flash_write_word(uint32_t address, uint32_t value)
{
	uint32_t status;

	status = flash_wait_ready();
	if ( !(status & FLASH_SR_ERRORS))
	{
		// start programming
		FLASH->FLASH_CR = FLASH_CR_PSIZE_x32 | FLASH_CR_PG;

		*(volatile uint32_t*) address = value;

		status = flash_wait_ready();

		// stop programming
		FLASH->FLASH_CR = 0;
	}

	return status;
}

template <typename T>
volatile T& dst_inc(uint32_t& a)
{
	return *reinterpret_cast<volatile T*&>(a)++;
}

template <typename T>
T src_inc(const void*& a)
{
	return *reinterpret_cast<const T*&>(a)++;
}


uint32_t flash_write(uint32_t address, const void* data, uint32_t len)
{
	uint32_t status;

	status = flash_wait_ready();
	while(len && !status)
	{
		if(len>=4 && !(address & 3) )
		{
			//write words
			FLASH->FLASH_CR = FLASH_CR_PSIZE_x32 | FLASH_CR_PG;
			do
			{
				dst_inc<uint32_t>(address) = src_inc<uint32_t>(data);

				len -= 4;
				status = flash_wait_ready();
			} while(len >=8 && !status);
		} else
		{
			if( len==1 || (address & 1))
			{
				//write byte
				FLASH->FLASH_CR = FLASH_CR_PSIZE_x8 | FLASH_CR_PG;
				dst_inc<uint8_t>(address) = src_inc<uint8_t>(data);

				len -= 1;
				status = flash_wait_ready();
			} else
			{
				//write half
				FLASH->FLASH_CR = FLASH_CR_PSIZE_x16 | FLASH_CR_PG;
				dst_inc<uint16_t>(address) = src_inc<uint16_t>(data);

				len -= 2;
				status = flash_wait_ready();
			}
		}
	}

	// stop programming
	FLASH->FLASH_CR = 0;

	return status;
}


uint32_t flash_ob_read()
{
	return FLASH->FLASH_OPTCR;
}

uint32_t flash_ob_write(uint32_t value)
{
	uint32_t status;

	// unlock
	status = FLASH->FLASH_OPTCR & FLASH_OPTCR_OPTLOCK;
	if (status)
	{
		FLASH->FLASH_OPTKEYR = FLASH_OPTKEYR_KEY_1;
		FLASH->FLASH_OPTKEYR = FLASH_OPTKEYR_KEY_2;
		status = FLASH->FLASH_OPTCR & FLASH_OPTCR_OPTLOCK;
	}

	if(!status)
	{
		status = flash_wait_ready() & FLASH_SR_ERRORS;
		if ( !status )
		{
			value &= !(FLASH_OPTCR_OPTLOCK | FLASH_OPTCR_OPTSTRT);
			FLASH->FLASH_OPTCR = value;
			FLASH->FLASH_OPTCR = value | FLASH_OPTCR_OPTSTRT;

			status = flash_wait_ready();
		}
	}

	// lock
	FLASH->FLASH_OPTCR |= FLASH_OPTCR_OPTLOCK;
	return status;
}

