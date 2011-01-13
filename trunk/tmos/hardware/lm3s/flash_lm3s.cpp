/**************************************************************************//**
 * @ingroup	 lm3s_flash
 * @file     flash_lm3s.cpp
 * @brief    Flash controller functions
 * @version  V3.00
 * @date     06. January 2010
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/

#include <tmos.h>
#include <fam_cpp.h>
#include <flash_lm3s.h>



//*****************************************************************************
//
//! Erases a single 1KB block of internal flash.
//!
//! \param ulAddress is the address of the block of flash to erase.
//!
//! This function erases a single 1KB block of the internal flash, blocking
//! until the erase has completed.
//!
//! \return None
//
//*****************************************************************************
void FLASHC_Type::erase_page(unsigned long ulAddress)
{
    FMA = ulAddress;
    FMC = FLASH_FMC_WRKEY | FLASH_FMC_ERASE;

    //
    // Wait until the flash has been erased.
    //
    while(FMC & FLASH_FMC_ERASE)
    {
    }
}


//*****************************************************************************
//
//! Programs a block of data at a given address in the internal flash.
//!
//! \param ulDstAddr is the address of the first word to be programmed in flash.
//! \param pucSrcData is a pointer to the first byte to be programmed.
//! \param ulLength is the number of bytes to program.  This must be a multiple
//! of 4.
//!
//! This function writes a block of data to the internal flash at a given
//! address.  Since the flash is written a word at a time, the data must be a
//! multiple of 4 bytes and the destination address, ulDstAddr, must be on a
//! word boundary.
//!
//! \return None
//
//*****************************************************************************
void FLASHC_Type::write(unsigned long ulDstAddr, const unsigned char *pucSrcData,
		unsigned long ulLength)
{
    unsigned long ulLoop;

    for(ulLoop = 0; ulLoop < ulLength; ulLoop += 4)
    {
        //
        // Program this word into flash.
        //
        FMA = ulDstAddr + ulLoop;
        FMD = *(unsigned long *)(void*)(pucSrcData + ulLoop);
        FMC = FLASH_FMC_WRKEY | FLASH_FMC_WRITE;

        //
        // Wait until the flash has been programmed.
        //
        while(FMC & FLASH_FMC_WRITE)
        {
        }
    }
}

void FLASHC_Type::fast_write(unsigned long ulDstAddr, const unsigned char *pucSrcData,
		unsigned long ulLength)
{
	while(ulLength > 3)
	{
		unsigned long adr;

		adr = ulDstAddr & 0x7F;

		FMA = ulDstAddr ^ adr;

		adr >>= 2;
		while((ulLength > 3) && (adr < 32))
		{
			FWBN[adr++] = *(unsigned long *)(void*)(pucSrcData);
			pucSrcData += 4;
			ulDstAddr += 4;
			ulLength -= 4;
		}

        FMC2 = FLASH_FMC2_WRKEY | FLASH_FMC2_WRBUF;

        //
        // Wait until the flash has been programmed.
        //
        while(FMC2 & FLASH_FMC2_WRBUF)
        {
        }
	}
}
