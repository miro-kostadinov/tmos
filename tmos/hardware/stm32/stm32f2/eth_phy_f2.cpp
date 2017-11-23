/**************************************************************************//**
 * @ingroup	 hardware_stm32f2_eth_phy
 * @{
 * @file     hardware/stm32/stm32f2/eth_phy_f2.cpp
 * @brief    STM32F2 ETH PHY
 * @version  V3.00
 * @date     27. July 2016
 * @author	 Miroslav Kostadinov
 * @}
 */

#include <tmos.h>
#include <fam_cpp.h>
#include <net_err.h>
#include <eth_phy_f2.h>

/*******************************************************************************
 * 			PHY initialization and configuration
 *
 * Replace with the actual routine
 *
 ******************************************************************************/
WEAK RES_CODE HAL_ETH_Init_PHY(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg)
{
	RES_CODE res;

	// TO DO - read the PHY id register and call the right function
	// for now just use LAN8720 by default
#if ETH_PHY_LAN8700
	res = HAL_ETH_PHY_Init_LAN8720(mac, cfg);
#else
#if ETH_PHY_TLK110
	res = HAL_ETH_PHY_Init_TLK110(mac, cfg);
#else
	res = RES_ERROR;
#endif
#endif
	return res;
}

#if ETH_PHY_LAN8700
WEAK RES_CODE HAL_ETH_PHY_Init_LAN8720(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg)
{
	RES_CODE res;
	uint32_t tmpreg, phyreg;

	/*--------------------  ----------------*/
	/* Put the PHY in reset mode */
	//Per the IEEE 802.3u standard, clause 22 (22.2.4.1.1) the reset
	// process will be completed within 0.5s from the setting of this bit
	res = HAL_ETH_WritePHYRegister(mac, cfg, PHY_REG_BCR, PHY_REG_BCR_RESET);
	if(res == RES_OK)
	{
		tmpreg = 0;
		do
		{
			/* Delay to assure PHY reset */
			tsk_sleep(5);

			if (++tmpreg >= 250)
			{
				res = NET_ERR_NO_LINK;
				break;
			}
			res = HAL_ETH_ReadPHYRegister(mac, cfg, PHY_REG_BCR, &phyreg);
		} while ( (res != RES_OK) || (phyreg & PHY_REG_BCR_RESET) );
	}

	cfg->mac_data->mac_cfg = cfg->mac_cfg_flags;
	if(res == RES_OK)
	{
		if ( cfg->mac_cfg_flags & MAC_CFG_FLG_AUTONEGOTIATION )
		{
			/* We wait for linked status */
			tmpreg = 0;
			do
			{
				tsk_sleep(10);
				if (++tmpreg >= PHY_READ_TOT)
				{
					res = NET_ERR_NO_LINK;
					TRACEPHY(mac, cfg, "ETH NO LINK!!");
					break;
				}
				res = HAL_ETH_ReadPHYRegister(mac, cfg, PHY_REG_BSR, &phyreg);
			} while ( (res != RES_OK) || !(phyreg & PHY_REG_BSR_LINKED_STATUS) );


			/* Enable Auto-Negotiation */
			if(res == RES_OK)
			{
				res = HAL_ETH_WritePHYRegister(mac, cfg, PHY_REG_BCR, PHY_REG_BCR_AUTONEGOTIATION | PHY_REG_BCR_FULLDUPLEX_100M);
				if(res == RES_OK)
				{
					/* Wait until the auto-negotiation will be completed */
					tmpreg = 0;
					do
					{
						if (++tmpreg >= PHY_READ_TOT)
						{
							res = NET_ERR_NO_LINK;
							TRACEPHY(mac, cfg, "ETH NEGO TOT!!");
							break;
						}
						tsk_sleep(4);
						res = HAL_ETH_ReadPHYRegister(mac, cfg, PHY_REG_BSR, &phyreg);
					} while ( (res != RES_OK) || !(phyreg & PHY_REG_BSR_AUTONEGO_COMPLETE) );
				}
			}

		    /* Read the result of the auto-negotiation */
			if(res == RES_OK)
			{
				if(HAL_ETH_ReadPHYRegister(mac, cfg, PHY_REG_SR, &phyreg) == RES_OK)
				{
					cfg->mac_data->mac_cfg &= ~(MAC_CFG_FLG_DUPLEX_MODE | MAC_CFG_FLG_100M);

				    /* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
				    if(phyreg & PHY_REG_SR_DUPLEX_STATUS)
				    {
				    	cfg->mac_data->mac_cfg |= MAC_CFG_FLG_DUPLEX_MODE;
				    	TRACEPHY(mac, cfg, "ETH full");
				    }

				    /* Configure the MAC with the speed fixed by the auto-negotiation process */
				    if( !(phyreg & PHY_REG_SR_SPEED_STATUS))
				    {
				    	cfg->mac_data->mac_cfg |= MAC_CFG_FLG_100M;
				    	TRACEPHY(mac, cfg, "ETH 100M");
				    }
				}
			}


		} else /* AutoNegotiation Disable */
		{

			/* Set MAC Speed and Duplex Mode */
			tmpreg = 0;
			if ( cfg->mac_cfg_flags & MAC_CFG_FLG_DUPLEX_MODE )
				tmpreg = PHY_REG_BCR_FULLDUPLEX_10M;
			if( cfg->mac_cfg_flags & MAC_CFG_FLG_100M)
				tmpreg |= PHY_REG_BCR_HALFDUPLEX_100M;
			res = HAL_ETH_WritePHYRegister(mac, cfg, PHY_REG_BCR, tmpreg);

			/* Delay to assure PHY configuration */
			tsk_sleep(PHY_CONFIG_DELAY);
		}
	}

	return res;
}

/**** Configure PHY to generate an interrupt when Eth Link state changes ****/
WEAK RES_CODE HAL_ETH_PHY_INTs(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg)
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
#endif

#if ETH_PHY_TLK110
WEAK RES_CODE HAL_ETH_PHY_Init_TLK110(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg)
{
	RES_CODE res;
	uint32_t tmpreg, phyreg;

	res = HAL_ETH_ReadPHYRegister(mac, cfg, PHY_TLK110_SWSCR1, &phyreg);
	if(res == RES_OK)
	{
		if(!(phyreg & PHY_TLK110_SWSCR1_SWCFGDNE))
		{
			// phyreg &= 0xE7FF;  // Force 10Mbit
			phyreg |= PHY_TLK110_SWSCR1_SWCFGDNE;
			res = HAL_ETH_WritePHYRegister(mac, cfg, PHY_TLK110_SWSCR1, phyreg);
		}
		if(res == RES_OK)
		{
			/*--------------------  ----------------*/
			/* Put the PHY in reset mode */
			//Per the IEEE 802.3u standard, clause 22 (22.2.4.1.1) the reset
			// process will be completed within 0.5s from the setting of this bit
			res = HAL_ETH_WritePHYRegister(mac, cfg, PHY_TLK110_BMCR, PHY_REG_BCR_RESET);
			HAL_ETH_WritePHYRegister(mac, cfg, PHY_TLK110_PHYRCR, 0xC000);
			if(res == RES_OK)
			{
				tmpreg = 0;
				do
				{
					/* Delay to assure PHY reset */
					tsk_sleep(5);

					if (++tmpreg >= 250)
					{
						res = NET_ERR_NO_LINK;
						break;
					}
					res = HAL_ETH_ReadPHYRegister(mac, cfg, PHY_TLK110_BMCR, &phyreg);
				} while ( (res != RES_OK) || (phyreg & PHY_REG_BCR_RESET) );
			}

			cfg->mac_data->mac_cfg = cfg->mac_cfg_flags;
			if(res == RES_OK)
			{
				if ( cfg->mac_cfg_flags & MAC_CFG_FLG_AUTONEGOTIATION )
				{
					/* We wait for linked status */
					tmpreg = 0;
					do
					{
						if(tmpreg == 1)
						{
							if(HAL_ETH_ReadPHYRegister(mac, cfg, PHY_TLK110_BMCR, &phyreg) == RES_OK)
							{
								HAL_ETH_WritePHYRegister(mac, cfg, PHY_TLK110_BMCR, phyreg | PHY_REG_BCR_RESTART_AUTONEGO);
							}
						}
						tsk_sleep(10);
						if (++tmpreg >= PHY_READ_TOT)
						{
							res = NET_ERR_NO_LINK;
							TRACEPHY(mac, cfg, "ETH NO LINK!!");
							break;
						}
						res = HAL_ETH_ReadPHYRegister(mac, cfg, PHY_REG_BSR, &phyreg);
					} while ( (res != RES_OK) || !(phyreg & PHY_REG_BSR_LINKED_STATUS) );


					/* Enable Auto-Negotiation */
					if(res == RES_OK)
					{
						res = HAL_ETH_WritePHYRegister(mac, cfg, PHY_REG_BCR, PHY_REG_BCR_AUTONEGOTIATION | PHY_REG_BCR_FULLDUPLEX_100M);
						if(res == RES_OK)
						{
							/* Wait until the auto-negotiation will be completed */
							tmpreg = 0;
							do
							{
								if (++tmpreg >= PHY_READ_TOT)
								{
									res = NET_ERR_NO_LINK;
									TRACEPHY(mac, cfg, "ETH NEGO TOT!!");
									break;
								}
								tsk_sleep(4);
								res = HAL_ETH_ReadPHYRegister(mac, cfg, PHY_REG_BSR, &phyreg);
							} while ( (res != RES_OK) || !(phyreg & PHY_REG_BSR_AUTONEGO_COMPLETE) );
						}
					}

				    /* Read the result of the auto-negotiation */
					if(res == RES_OK)
					{
						if(HAL_ETH_ReadPHYRegister(mac, cfg, PHY_TLK110_PHYSTS, &phyreg) == RES_OK)
						{
							cfg->mac_data->mac_cfg &= ~(MAC_CFG_FLG_DUPLEX_MODE | MAC_CFG_FLG_100M);

						    /* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
						    if(phyreg & PHY_TLK110_PHYSTS_DUPLEX)
						    {
						    	cfg->mac_data->mac_cfg |= MAC_CFG_FLG_DUPLEX_MODE;
						    	TRACEPHY(mac, cfg, "ETH full");
						    }

						    /* Configure the MAC with the speed fixed by the auto-negotiation process */
						    if( !(phyreg & PHY_TLK110_PHYSTS_SPEED))
						    {
						    	cfg->mac_data->mac_cfg |= MAC_CFG_FLG_100M;
						    	TRACEPHY(mac, cfg, "ETH 100M");
						    }
						}
					}


				} else /* AutoNegotiation Disable */
				{

					/* Set MAC Speed and Duplex Mode */
					tmpreg = 0;
					if ( cfg->mac_cfg_flags & MAC_CFG_FLG_DUPLEX_MODE )
						tmpreg = PHY_REG_BCR_FULLDUPLEX_10M;
					if( cfg->mac_cfg_flags & MAC_CFG_FLG_100M)
						tmpreg |= PHY_REG_BCR_HALFDUPLEX_100M;
					res = HAL_ETH_WritePHYRegister(mac, cfg, PHY_REG_BCR, tmpreg);

					/* Delay to assure PHY configuration */
					tsk_sleep(PHY_CONFIG_DELAY);
				}
			}
		}
	}
	return res;



}

/**
 *
 * return Link status and clear interrupts (for TLK110 )
 */
WEAK RES_CODE HAL_ETH_PHY_INT_LINK_STATUS(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg, uint32_t *Reg)
{
	RES_CODE res;
	uint32_t status;

	/* Clear interrupt status () */
	HAL_ETH_ReadPHYRegister(mac, cfg,	PHY_TLK110_MISR1, &status);
	res = HAL_ETH_ReadPHYRegister(mac, cfg, PHY_REG_BSR, &status);
	if(res == RES_OK && Reg)
	{
		*Reg = (status & PHY_REG_BSR_LINKED_STATUS);
	}

	return res;
}

/**** Configure PHY to generate an interrupt when Eth Link state changes for TLK110 ****/
WEAK RES_CODE HAL_ETH_PHY_INTs(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg)
{
	uint32_t regvalue;
	RES_CODE res;

	res = HAL_ETH_ReadPHYRegister(mac, cfg, PHY_TLK110_PHYSCR, &regvalue);
	if(res == RES_OK)
	{
		regvalue |= PHY_TLK110_PHYSCR_INT_OE | PHY_TLK110_PHYSCR_INT_EN | PHY_TLK110_PHYSCR_INT_POL;

		/* Enable Interrupt on change of link status */
		res = HAL_ETH_WritePHYRegister(mac, cfg, PHY_TLK110_PHYSCR, regvalue);
		if(res == RES_OK)
		{
			res = HAL_ETH_ReadPHYRegister(mac, cfg, PHY_TLK110_MISR1, &regvalue);
			if(res == RES_OK)
			{
				regvalue |= PHY_TLK110_MISR1_LINK_EN;
				res = HAL_ETH_WritePHYRegister(mac, cfg, PHY_TLK110_MISR1, regvalue);
			}
		}
	}
	return res;
}

#endif

