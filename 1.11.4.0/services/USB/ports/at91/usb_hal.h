
#ifndef USB_HAL_H_
#define USB_HAL_H_

#include <tmos.h>
#include <usb_opt.h>

/** PKTRDY must be set to enable transmission of the packet that was loaded
 *
 * AT91 - only one packet can be confirmed...
 */
#define SET_TXPKTRDY(hw_base, eptnum) 							\
	SET_CSR(&hw_base->UDP_CSR[eptnum], UDP_CSR_TXPKTRDY)


/** Enable interrupt for endpoint
 *
 */
#define ENTPOINT_ENABLE_INT(hw_base, eptnum) 					\
	    hw_base->UDP_IER |= 1 << eptnum

/** Enable interrupt for endpoint
 *
 * Not implemented from LM3S ????
 */
#define ENTPOINT_IS_PING_PONG(hw_base, eptnum) 					\
	(CHIP_USB_ENDPOINTS_BANKS(eptnum) > 1)

/** Force Stall on endpoint  */
#define ENTPOINT_FIFO(hw_base, eptnum) (unsigned char*)&hw_base->UDP_FDR[eptnum]

#define FORCE_STALL(hw_base, eptnum) SET_CSR(&hw_base->UDP_CSR[eptnum], UDP_CSR_FORCESTALL);

void usb_ack_packet(Udp* hw_base, Endpoint* endpoint, unsigned int eptnum);


#endif /* USB_HAL_H_ */
