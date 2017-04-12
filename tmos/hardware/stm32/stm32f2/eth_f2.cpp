/**************************************************************************//**
 * @ingroup	 hardware_stm32f2_eth
 * @{
 * @file     hardware/stm32/stm32f2/eth_f2.cpp
 * @brief    STM32F2 ETH
 * @version  V3.00
 * @date     20. May 2015
 * @author	 Miroslav Kostadinov
 * @}
 */

#include <tmos.h>
#include <fam_cpp.h>
#include <net_err.h>
#include <eth_phy_f2.h>

WEAK_C void EthernetMACAddrGet(const eth_mac_cfg_t* cfg)
{
	cfg->mac_data->mac_adr[0] =  0x50;
	cfg->mac_data->mac_adr[1] =  0xa5;
	cfg->mac_data->mac_adr[2] =  0x00;
	cfg->mac_data->mac_adr[3] =  0x12;
	cfg->mac_data->mac_adr[4] =  0x34;
	cfg->mac_data->mac_adr[5] =  0x56;
}

RES_CODE HAL_ETH_WritePHYRegister(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg, uint16_t PHYReg, uint32_t RegValue)
{
  uint32_t tmpreg;

  /* Get the ETHERNET MACMIIAR value */
  tmpreg = mac->ETH_MACMIIAR;

  /* Keep only the CSR Clock Range CR[2:0] bits value */
  tmpreg &= ETH_MACMIIAR_CR;

  /* Prepare the MII register address value */
  tmpreg |= cfg->phy_address<<11;		/* Set the PHY device address */
  tmpreg |= PHYReg<<6;                 	/* Set the PHY register address */
  tmpreg |= ETH_MACMIIAR_MW;            /* Set the write mode */
  tmpreg |= ETH_MACMIIAR_MB;            /* Set the MII Busy bit */

  /* Give the value to the MII data register */
  mac->ETH_MACMIIDR = RegValue;

  /* Write the result value into the MII Address register */
  mac->ETH_MACMIIAR = tmpreg;

  /* Check for the Busy flag */
  tmpreg = 0;
  while(mac->ETH_MACMIIAR & ETH_MACMIIAR_MB)
  {
	  if(++tmpreg >= PHY_WRITE_TOT)
		  return NET_ERR_PHY_NOT_READY;
	  tsk_sleep(1);
  }

  /* Return function status */
  return RES_OK;
}

RES_CODE HAL_ETH_ReadPHYRegister(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg, uint16_t PHYReg, uint32_t *RegValue)
{
  uint32_t tmpreg;

  /* Get the ETHERNET MACMIIAR value */
  tmpreg = mac->ETH_MACMIIAR;

  /* Keep only the CSR Clock Range CR[2:0] bits value */
  tmpreg &= ETH_MACMIIAR_CR;

  /* Prepare the MII address register value */
  tmpreg |= cfg->phy_address<<11;		/* Set the PHY device address */
  tmpreg |= PHYReg<<6;                 	/* Set the PHY register address */
  tmpreg |= ETH_MACMIIAR_MB;            /* Set the MII Busy bit */


  /* Write the result value into the MII Address register */
  mac->ETH_MACMIIAR = tmpreg;

  /* Check for the Busy flag */
  tmpreg = 0;
  while(mac->ETH_MACMIIAR & ETH_MACMIIAR_MB)
  {
	  if(++tmpreg >= PHY_READ_TOT)
		  return NET_ERR_PHY_NOT_READY;
	  tsk_sleep(1);
  }

  /* Get MACMIIDR value */
  *RegValue = mac->ETH_MACMIIDR;

  return RES_OK;
}

static void ETH_MACAddressConfig(__IO uint32_t *mac_reg, uint8_t *Addr)
{
  uint32_t tmpreg;


  /* Calculate the selected MAC address high register */
  tmpreg = ((uint32_t)Addr[5] << 8) | (uint32_t)Addr[4];
  /* Load the selected MAC address high register */
  mac_reg[0] = tmpreg;

  /* Calculate the selected MAC address low register */
  tmpreg = ((uint32_t)Addr[3] << 24) | ((uint32_t)Addr[2] << 16) | ((uint32_t)Addr[1] << 8) | Addr[0];

  /* Load the selected MAC address low register */
  mac_reg[1] = tmpreg;
}

static void ETH_MACDMAConfig(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg)
{
  uint32_t tmpreg;


  	/*------------------------ ETHERNET MACCR Configuration --------------------*/
	tmpreg = mac->ETH_MACCR;
	/* Clear WD, PCE, PS, TE and RE bits */
	tmpreg &= ~(ETH_MACCR_WD | ETH_MACCR_JD | ETH_MACCR_IFG | ETH_MACCR_CSD |
	ETH_MACCR_FES | ETH_MACCR_ROD | ETH_MACCR_LM |
	ETH_MACCR_DM | ETH_MACCR_IPCO | ETH_MACCR_RD |
	ETH_MACCR_APCS | ETH_MACCR_BL | ETH_MACCR_DC);

	tmpreg |= cfg->ETH_MACCR;
	if (cfg->mac_data->mac_cfg & MAC_CFG_FLG_DUPLEX_MODE)
		tmpreg |= ETH_MACCR_DM;
	if (cfg->mac_data->mac_cfg & MAC_CFG_FLG_100M)
		tmpreg |= ETH_MACCR_FES;

	/* Write to ETHERNET MACCR */
	mac->ETH_MACCR = tmpreg;

	/* Wait until the write operation will be taken into account:
     at least four TX_CLK/RX_CLK clock cycles */
	tsk_sleep(ETH_REG_WRITE_DELAY);


  	/*----------------------- ETHERNET MACFFR Configuration --------------------*/
	mac->ETH_MACFFR = cfg->ETH_MACFFR;

	tsk_sleep(ETH_REG_WRITE_DELAY);

	/*--------------- ETHERNET MACHTHR and MACHTLR Configuration --------------*/
	mac->ETH_MACHTHR = 0;

	mac->ETH_MACHTLR = 0;
	/*----------------------- ETHERNET MACFCR Configuration -------------------*/

	tmpreg = mac->ETH_MACFCR;
	tmpreg &= ~(ETH_MACFCR_PT | ETH_MACFCR_ZQPD | ETH_MACFCR_PLT |
	ETH_MACFCR_UPFD | ETH_MACFCR_RFCE | ETH_MACFCR_TFCE);
	tmpreg |= cfg->ETH_MACFCR;

	mac->ETH_MACFCR = tmpreg;

	tsk_sleep(ETH_REG_WRITE_DELAY);

	/*----------------------- ETHERNET MACVLANTR Configuration ----------------*/
	/* Set the ETV bit according to ETH VLANTagComparison value */
	/* Set the VL bit according to ETH VLANTagIdentifier value */
	mac->ETH_MACVLANTR = 0;

    tsk_sleep(ETH_REG_WRITE_DELAY);

    /* Ethernet DMA default initialization ************************************/

    /* Get the ETHERNET DMAOMR value */
    tmpreg = mac->ETH_DMAOMR;
    /* Clear xx bits */
    tmpreg &= ~(ETH_DMAOMR_DTCEFD | ETH_DMAOMR_RSF | ETH_DMAOMR_DFRF |
    		ETH_DMAOMR_TSF | ETH_DMAOMR_TTC |
			ETH_DMAOMR_FEF | ETH_DMAOMR_FUGF | ETH_DMAOMR_RTC | ETH_DMAOMR_OSF);
    tmpreg |= cfg->ETH_DMAOMR;

    mac->ETH_DMAOMR = tmpreg;

    // Wait until the write operation will be taken into account
    tsk_sleep(ETH_REG_WRITE_DELAY);

    /*----------------------- ETHERNET DMABMR Configuration ------------------*/
    mac->ETH_DMABMR = cfg->ETH_DMABMR | ETH_DMABMR_USP; // Enable use of separate PBL for Rx and Tx

    // Wait until the write operation will be taken into account
    tsk_sleep(ETH_REG_WRITE_DELAY);

	/* Enable the Ethernet Rx Interrupt */
	mac->ETH_DMAIER |= ETH_DMAIER_NISE | ETH_DMAIER_RIE;

     /* Initialize MAC address in ethernet MAC */
     ETH_MACAddressConfig(&mac->ETH_MACA0HR, cfg->mac_data->mac_adr);
}

void TRACEPHY(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg, const char* err)
{
	RES_CODE res;
	uint32_t phyreg;

	TRACELN("");
	TRACE1(err);
	for(uint32_t i=0; i<32; i++)
	{
		res = HAL_ETH_ReadPHYRegister(mac, cfg, i, &phyreg);
		TRACELN("PHY %02u %u %04X", i, res, phyreg);
	}
}


RES_CODE HAL_ETH_Init(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg)
{
	uint32_t tmpreg;
	uint32_t res;
	RCC_ClocksTypeDef  clocks;


	TRACELN("ETH Init");
	/* Select MII or RMII Mode*/
	if(cfg->mac_cfg_flags & MAC_CFG_FLG_IFCE_MII_RMII)
	{
		tmpreg = 0;
		do
		{
			mac->ETH_DMABMR |= ETH_DMABMR_SR;
			SYSCFG->SYSCFG_PMC |= SYSCFG_PMC_MII_RMII;
			if(++tmpreg > PHY_CONFIG_DELAY)
			{
				TRACEPHY(mac, cfg, "ETH NO PHY CLK!!");
				break;
			}
			tsk_sleep(1);
		} while (!(SYSCFG->SYSCFG_PMC & SYSCFG_PMC_MII_RMII));
	}
	else
		SYSCFG->SYSCFG_PMC &= ~SYSCFG_PMC_MII_RMII;

	/* Ethernet Software reset */
	/* Set the SWR bit: resets all MAC subsystem internal registers and logic */
	/* After reset all the registers holds their respective reset values */
	mac->ETH_DMABMR |= ETH_DMABMR_SR;

	/* Wait for software reset */
	tmpreg = 0;
	while (mac->ETH_DMABMR & ETH_DMABMR_SR)
	{
		tsk_sleep(1);
		if(++tmpreg > PHY_CONFIG_DELAY)
		{
			TRACEPHY(mac, cfg, "ETH MAC RST!!");
			break;
		}
	}

	/*-------------------------------- MAC Initialization ----------------------*/

	TRACEPHY(mac, cfg, "ETH PHY:");

	/* Get the ETHERNET MACMIIAR value */
	tmpreg = mac->ETH_MACMIIAR;
	/* Clear CSR Clock Range CR[2:0] bits */
	tmpreg &= ~ETH_MACMIIAR_CR;

	/* Get hclk frequency value */
	RCC_GetClocksFreq (&clocks);

	/* Set CR bits depending on hclk value */
	if ((clocks.HCLK_Frequency >= 20000000) && (clocks.HCLK_Frequency < 35000000))
	{
		/* CSR Clock Range between 20-35 MHz */
		tmpreg |= ETH_MACMIIAR_CR_Div16;
	}
	else if ((clocks.HCLK_Frequency >= 35000000) && (clocks.HCLK_Frequency < 60000000))
	{
		/* CSR Clock Range between 35-60 MHz */
		tmpreg |= ETH_MACMIIAR_CR_Div26;
	}
	else if ((clocks.HCLK_Frequency >= 60000000) && (clocks.HCLK_Frequency < 100000000))
	{
		/* CSR Clock Range between 60-100 MHz */
		tmpreg |= ETH_MACMIIAR_CR_Div42;
	}
	else /* ((hclk >= 100000000)&&(hclk < 120000000)) */
	{
		/* CSR Clock Range between 100-120 MHz */
		tmpreg |= ETH_MACMIIAR_CR_Div62;
	}

	/* Write to ETHERNET MAC MIIAR: Configure the ETHERNET CSR Clock Range */
	mac->ETH_MACMIIAR = tmpreg;

	/*-------------------- PHY initialization and configuration ----------------*/
	res = HAL_ETH_Init_PHY(mac, cfg);

	/* Config MAC and DMA */
	ETH_MACDMAConfig(mac, cfg);

	return res;
}

void HAL_ETH_DMATxDescListInit(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg)
{
	ETH_DMADescTypeDef* dmatxdesc = cfg->DMATxDescTab;


	/* Set the DMATxDescToSet pointer with the first one of the DMATxDescTab list */
	cfg->mac_data->TxDesc = dmatxdesc;

	/* Fill each DMATxDesc descriptor with the right values */
	for (int i = 0; i < cfg->ETH_TXBUFNB; i++)
	{
		/* Set Second Address Chained bit */
		dmatxdesc->Status = ETH_DMATXDESC_TCH;

		/* Set Buffer1 address pointer */
		dmatxdesc->Buffer1Addr = dmatxdesc->Buff;

		if(cfg->ETH_MACCR & ETH_MACCR_IPCO) // IPv4 Checksum offload
		{
			/* Set the DMA Tx descriptors checksum insertion */
			dmatxdesc->Status |= ETH_DMATXDESC_CIC_TCPUDPICMP_FULL;
		}

		/* Initialize the next descriptor with the Next Descriptor Polling Enable */
		if (i < (cfg->ETH_TXBUFNB - 1))
		{
			/* Set next descriptor address register with next descriptor base address */
			dmatxdesc = dmatxdesc->Buffer2NextDescAddr = dmatxdesc + 1;
		}
		else
		{
			/* For last descriptor, set next descriptor address register equal to the first descriptor base address */
			dmatxdesc->Buffer2NextDescAddr = cfg->DMATxDescTab;
		}
	}

	/* Set Transmit Descriptor List Address Register */
	mac->ETH_DMATDLAR =  (uint32_t)cfg->DMATxDescTab;

}

void HAL_ETH_DMARxDescListInit(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg)
{
	ETH_DMADescTypeDef* dmarxdesc = cfg->DMARxDescTab;


	/* Set the Ethernet RxDesc pointer with the first one of the DMARxDescTab list */
	cfg->mac_data->RxDesc = dmarxdesc;

	/* Fill each DMARxDesc descriptor with the right values */
	for (int i = 0; i < cfg->ETH_RXBUFNB; i++)
	{

		/* Set Own bit of the Rx descriptor Status */
		dmarxdesc->Status = ETH_DMARXDESC_OWN;

		/* Set Buffer1 size and Second Address Chained bit */
		dmarxdesc->ControlBufferSize = ETH_DMARXDESC_RCH | ETH_RX_BUF_SIZE;

		/* Set Buffer1 address pointer */
		dmarxdesc->Buffer1Addr = dmarxdesc->Buff;

		/* Initialize the next descriptor with the Next Descriptor Polling Enable */
		if (i < (cfg->ETH_RXBUFNB - 1))
		{
			/* Set next descriptor address register with next descriptor base address */
			dmarxdesc = dmarxdesc->Buffer2NextDescAddr = dmarxdesc+1;
		}
		else
		{
			/* For last descriptor, set next descriptor address register equal to the first descriptor base address */
			dmarxdesc->Buffer2NextDescAddr = cfg->DMARxDescTab;
		}
	}

	/* Set Receive Descriptor List Address Register */
	mac->ETH_DMARDLAR = (uint32_t)cfg->DMARxDescTab;
}

void HAL_ETH_Start(ETH_TypeDef* mac)
{
	/* Enable transmit state machine of the MAC for transmission on the MII */
	mac->ETH_MACCR |= ETH_MACCR_TE;
	tsk_sleep(ETH_REG_WRITE_DELAY);

	/* Enable receive state machine of the MAC for reception from the MII */
	mac->ETH_MACCR |= ETH_MACCR_RE;
	tsk_sleep(ETH_REG_WRITE_DELAY);

	/* Flush Transmit FIFO */
	mac->ETH_DMAOMR |= ETH_DMAOMR_FTF;
	tsk_sleep(ETH_REG_WRITE_DELAY);

	/* Start DMA transmission */
	mac->ETH_DMAOMR |= ETH_DMAOMR_ST;

	/* Start DMA reception */
	mac->ETH_DMAOMR |= ETH_DMAOMR_SR;
}

/**** Configure PHY to generate an interrupt when Eth Link state changes ****/
RES_CODE HAL_ETH_PHY_INTs(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg)
{
	uint32_t regvalue;
	RES_CODE res;

	// mask 30.4
	// Source flag 29.4 Link Down
	// Interrupt source 1.2 Link Status
	// Event to assert Falling
	// Event to de-assert 1.2 Reading register 1 or	Reading register 29

	  /* Read Register Configuration */
//	  HAL_ETH_ReadPHYRegister(mac, cfg, PHY_MICR, &regvalue);
//	  regvalue |= (PHY_MICR_INT_EN | PHY_MICR_INT_OE);
	  /* Enable Interrupts */
//	  HAL_ETH_WritePHYRegister(mac, cfg, PHY_MICR, regvalue );

	  /* Read Register Configuration */
	  res = HAL_ETH_ReadPHYRegister(mac, cfg, PHY_REG_IMR, &regvalue);
	  if(res == RES_OK)
	  {
		  regvalue |= PHY_REG_IMR_LINK_STATUS;

		  /* Enable Interrupt on change of link status */
		  res = HAL_ETH_WritePHYRegister(mac, cfg, PHY_REG_IMR, regvalue);
	  }
	  return res;
}

void HAL_ETH_TransmitFrame(ETH_TypeDef* mac, eth_mac_data_t* mac_data, uint32_t FrameLength)
{
	uint32_t status, size;

	status = ETH_DMATXDESC_FS;
	while(FrameLength)
	{
		if (FrameLength > ETH_TX_BUF_SIZE)
		{
			size = ETH_TX_BUF_SIZE;
		} else
		{
			status |= ETH_DMATXDESC_LS;
			size = FrameLength;
		}
		FrameLength -= size;

		/* FIRST and LAST segment bits */
		mac_data->TxDesc->Status &= ~(ETH_DMATXDESC_FS | ETH_DMATXDESC_LS);
		mac_data->TxDesc->Status |= status;
		status = 0;

		/* Set frame size */
		mac_data->TxDesc->ControlBufferSize = size;

		/* Set Own bit of the Tx descriptor Status: gives the buffer back to ETHERNET DMA */
		mac_data->TxDesc->Status |= ETH_DMATXDESC_OWN;

		/* Point to next descriptor */
		mac_data->TxDesc = mac_data->TxDesc->Buffer2NextDescAddr;
	}

	/* When Tx Buffer unavailable flag is set: clear it and resume transmission */
	if (mac->ETH_DMASR & ETH_DMASR_TBUS)
	{
		/* Clear TBUS ETHERNET DMA flag */
		mac->ETH_DMASR = ETH_DMASR_TBUS;
	}
	/* Resume DMA transmission*/
	mac->ETH_DMATPDR = 0;
}

void HAL_ETH_ReleaseFrame(eth_mac_data_t* mac_data, uint32_t descriptors)
{
	ETH_DMADescTypeDef* DmaRxDesc;

	DmaRxDesc = mac_data->RxDesc;
	while(descriptors)
	{
		descriptors--;
		DmaRxDesc->Status = ETH_DMARXDESC_OWN;
		DmaRxDesc = DmaRxDesc->Buffer2NextDescAddr;
	}
	mac_data->RxDesc = DmaRxDesc;
}

uint32_t HAL_ETH_GetReceivedFrame_IT(const eth_mac_cfg_t* cfg)
{
	ETH_DMADescTypeDef* DmaRxDesc;
	uint32_t descriptor_count = 0;
	uint32_t status;

	DmaRxDesc = cfg->mac_data->RxDesc;
	while(1)
	{
		// try to get a frame
		do
		{
			descriptor_count++;
			status = DmaRxDesc->Status;
			if(status & ETH_DMARXDESC_OWN)
				return 0;	//no or incomplete frame

			if(status & (ETH_DMARXDESC_AFM | ETH_DMARXDESC_ES))
				break;		// error or filter fail -> dump it

			if(cfg->ETH_MACCR & ETH_MACCR_IPCO) // IPv4 Checksum offload
			{
				if( (status & ETH_DMARXDESC_FT) && (status & (ETH_DMARXDESC_IPHCE | ETH_DMARXDESC_PCE)))
				{
					if((status & 0x1FF) == (ETH_DMARXDESC_LS | ETH_DMARXDESC_FT | ETH_DMARXDESC_PCE))
					{
						// extended status available, check it
						if (!(DmaRxDesc->ExtendedStatus & ETH_DMAPTPRXDESC_IPCB) && (DmaRxDesc->ExtendedStatus & (ETH_DMAPTPRXDESC_IPPE | ETH_DMAPTPRXDESC_IPHE)))
							break;

					} else
						break; //check sum error
				}
			}

			if(descriptor_count == 1 && !(status & ETH_DMARXDESC_FS))
				break;  //not a first descriptor -> dump it

			if(status & ETH_DMARXDESC_LS)
			{
				// valid frame found
				cfg->mac_data->rx_len = (status & ETH_DMARXDESC_FL) >> 16;
				if(!cfg->mac_data->rx_len)
					break;
				TRACELN("ETH: rx %x %u * %u ", DmaRxDesc, descriptor_count, cfg->mac_data->rx_len);
				return descriptor_count;
			}
		} while( descriptor_count < cfg->ETH_RXBUFNB);

		// bad frame -> dump it
		DmaRxDesc = cfg->mac_data->RxDesc;
		while(descriptor_count)
		{
			TRACELN("ETH: rx drop %u res %x", DmaRxDesc, DmaRxDesc->Status);
			descriptor_count--;
			DmaRxDesc->Status = ETH_DMARXDESC_OWN;
			DmaRxDesc = DmaRxDesc->Buffer2NextDescAddr;
		}
		cfg->mac_data->RxDesc = DmaRxDesc;
	}
	return 0;
}
