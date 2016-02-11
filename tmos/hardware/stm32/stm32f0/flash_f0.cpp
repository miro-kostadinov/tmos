/*
 * flash_f0.cpp
 *
 *  Created on: Feb 11, 2016
 *      Author: miro
 */

#include <tmos.h>
#include <flash_f0.h>
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

	//disable prefetch
	FLASH->FLASH_ACR &= ~FLASH_ACR_PRFTEN;

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

		// sector must be aligned to 2K
		if(address & 0x7FF)
			return FLASH_SR_BSY;

		FLASH->FLASH_CR = FLASH_CR_PER;

		FLASH->FLASH_AR = address;
		FLASH->FLASH_CR |=  FLASH_CR_STRT;

		status = flash_wait_ready() ^ FLASH_SR_EOP;

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

		FLASH->FLASH_CR = FLASH_CR_MER;
		FLASH->FLASH_CR = FLASH_CR_MER | FLASH_CR_STRT;

		status = flash_wait_ready() ^ FLASH_SR_EOP;

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
		value &= 0xFF;
		if(address & 1)
		{
			value <<= 8;
			address--;
		}
		value &= reinterpret_cast<volatile uint16_t*>(address)[0];


		// start programming
		FLASH->FLASH_CR = FLASH_CR_PG;

		reinterpret_cast<volatile uint16_t*>(address)[0] = value;

		status = flash_wait_ready() ^ FLASH_SR_EOP;

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
		FLASH->FLASH_CR =  FLASH_CR_PG;

		*(volatile uint16_t*) address = value;

		status = flash_wait_ready() ^ FLASH_SR_EOP;

		// stop programming
		FLASH->FLASH_CR = 0;
	}

	return status;
}


template <typename T>
T src_inc(const void*& a)
{
	return *reinterpret_cast<const T*&>(a)++;
}

uint32_t flash_write(uint32_t address, const void* data, uint32_t len)
{
	uint32_t status, value;

	status = flash_wait_ready();
	while(len && !status)
	{
		if( len==1 || (address & 1))
		{
			//write byte
			value = src_inc<uint8_t>(data);
			if(address & 1)
			{
				value <<= 8;
				value &= reinterpret_cast<volatile uint16_t*>(address-1)[0];
			} else
				value &= reinterpret_cast<volatile uint16_t*>(address)[0];

			FLASH->FLASH_CR =  FLASH_CR_PG;
			if(address & 1)
			{
				reinterpret_cast<volatile uint16_t*>(address-1)[0] = value;
			} else
				reinterpret_cast<volatile uint16_t*>(address)[0] = value;
			address++;

			len -= 1;
		} else
		{
			//write half
			FLASH->FLASH_CR = FLASH_CR_PG;
			reinterpret_cast<volatile uint16_t*>(address)[0] = src_inc<uint16_t>(data);
			address +=2;

			len -= 2;
		}

		status = flash_wait_ready() ^ FLASH_SR_EOP;

		// stop programming
		FLASH->FLASH_CR = 0;
	}

	// stop programming
	FLASH->FLASH_CR = 0;

	return status;
}





