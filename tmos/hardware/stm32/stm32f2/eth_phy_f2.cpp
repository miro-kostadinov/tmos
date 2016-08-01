/*
 * eth_phy_f2.cpp
 *
 *  Created on: Jul 27, 2016
 *      Author: miro
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
	res = HAL_ETH_PHY_Init_LAN8720(mac, cfg);

	return res;
}

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

WEAK RES_CODE HAL_ETH_PHY_Init_TLK110(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg)
{
	RES_CODE res;
	uint32_t tmpreg, phyreg;

	res = HAL_ETH_ReadPHYRegister(mac, cfg, PHY_TLK110_SWSCR1, &phyreg);
	if(res == RES_OK)
	{
		if(!(phyreg & PHY_TLK110_SWSCR1_SWCFGDNE))
		{
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
		}
	}
	return res;



}
