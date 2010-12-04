/*
 * Driver System
 *
 *  Created on: 2010-4-9
 *      Author: stanly
 */

#ifndef HANDLE_H_
#define HANDLE_H_

#ifndef TMOS_H
#error "use tmos.h"
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 HANDLE
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct CHandle *HANDLE;
typedef void (*HND_CALLBACK)(HANDLE hnd, unsigned int status);

union HND_CLIENTS {
	Task *task; /**< as task pointer	*/
	unsigned int drv_index; /**< as driver	*/
	HND_CALLBACK callback; /**< as callback	*/
};

#ifndef usr_drv_hcontrol_swi
#error "this swi is required"
#endif

struct CHandle {
	//----- members  ---------------------------//
	DRIVER_INDEX drv_index; /**< Driver index	*/
	unsigned char signal; /**< Signal allocated from the client	*/
	unsigned char mode0; /**< driver private field	*/
	unsigned char mode1; /**< driver private field	*/

	HANDLE next; /**< driver private field	*/
	HND_CLIENTS client; /**< pointer to the client	*/

	unsigned short len; /**< length for the requested operation	*/
	unsigned char cmd; /**< requested operation	*/
	unsigned char res; /**< status/response code	*/

	word_reg dst; /**< destination data pointer	*/
	word_reg src; /**< source data pointer	*/
	word_reg mode; /**< driver mode	*/

	//----------------------    ctors etc   ---------------------------//
	CHandle();
	~CHandle();

	//----------------------    common      ---------------------------//
	/**
	 * Request driver DCR() call
	 * @param reason
	 */
	void hcontrol(unsigned int volatile reason) {
		HANDLE _hnd asm("r0") = this;
		register unsigned int _reason asm("r1") = reason;

		asm volatile ("swi %0"
				:
				: "I" (usr_drv_hcontrol_swi), "r"(_hnd), "r"(_reason)
				: );
	}

	/**
	 * helper function (make private?)
	 * @param val
	 */
	void set_res_cmd(unsigned int val) {
		((unsigned short *) (this))[7] = (val);
	}

	void close();

	//----------------------   task related ---------------------------//

	/**
	 * Request DCR()
	 * @param reason
	 */
	void tsk_hcontrol(unsigned int volatile reason) {
		register HANDLE _hnd asm("r0") = this;
		register unsigned int _reason asm("r1") = reason;

		asm volatile ("swi %0"
				:
				: "I" (usr_drv_hcontrol_swi), "r"(_hnd), "r"(_reason)
				: );
	}

	/**
	 * Starts handle operation and return immediately
	 */
	void tsk_start_handle() {
		register HANDLE _hnd asm("r0") = this;

		asm volatile ("swi %0"
				:
				: "I" (tsk_start_handle_swi), "r"(_hnd)
				: );
	}

	/**
	 * Starts and wait the handle operation
	 * @return
	 */
	RES_CODE tsk_start_and_wait() //send DSR request and wait for signal
	{
		register HANDLE _hnd asm("r0") = this;

		asm volatile ("swi %1"
				: "=r"(_hnd)
				: "I" (tsk_start_and_wait_swi), "0"(_hnd)
				: "r1");

		return res &= ~FLG_SIGNALED;
	}

	void tsk_cancel();
	bool tsk_open(DRIVER_INDEX index, const void * m);
	void tsk_safe_open(DRIVER_INDEX index, const void * m);
	bool tsk_start_read(void * buf, unsigned int l);
	RES_CODE tsk_read(void * buf, unsigned int l);
	RES_CODE tsk_read(void * buf, unsigned int l, unsigned int time);
	RES_CODE tsk_read_locked(void * buf, unsigned int l);
	RES_CODE tsk_read_locked(void * buf, unsigned int l, unsigned int time);
	RES_CODE tsk_resume_read(void * buf, unsigned int l);

	bool tsk_start_write(const void * buf, unsigned int l);
	RES_CODE tsk_write(const void * buf, unsigned int l);
	RES_CODE tsk_write(const void * buf, unsigned int l, unsigned int time);

	/**
	 * Write string
	 * @param buf
	 * @return
	 */
	RES_CODE tsk_write(const char * buf) {
		return tsk_write(buf, strlen(buf));
	}
#if USE_CSTRING
	RES_CODE tsk_write(CSTRING& str) {return tsk_write(str.c_str(), str.length());}
#endif
	RES_CODE tsk_write_locked(const void * buf, unsigned int l);
	RES_CODE tsk_write_locked(const void * buf, unsigned int ln,
			unsigned int time);
	RES_CODE tsk_resume_write(const void * buf, unsigned int l);

	RES_CODE tsk_read_write(void *dst, const void *src, unsigned int l);

	RES_CODE tsk_command(void * cmd, void *ptr);
	RES_CODE tsk_command_locked(void * cmd, void *ptr);

	//----------------------   task related ---------------------------//
	bool svc_list_cancel(HANDLE& base);
	void list_add(HANDLE& base);
	bool list_remove(HANDLE& base);

	void usr_hnd_signal();
};

//HANDLE tsk_open_new_hnd(DRIVER_INDEX index, const void * mode );
//HANDLE tsk_safe_open_new_hnd(DRIVER_INDEX index, const void * mode );


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 Driver related
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//void usr_HND_SET_STATUS(HANDLE hnd, RES_CODE result);
RES_CODE svc_init_handle(HANDLE hnd, unsigned char own_index,
		unsigned int drv_indx, void * mode);

extern "C" void svc_HND_SET_STATUS(HANDLE hnd, RES_CODE result);
#ifdef usr_HND_SET_STATUS_swi
inline static void usr_HND_SET_STATUS(HANDLE volatile hnd, RES_CODE volatile result)
{
	register HANDLE _hnd asm("r0") = hnd;
	register unsigned int _result asm("r1") = result;

	asm volatile ("swi %0"
			:
			: "I" (usr_HND_SET_STATUS_swi), "r"(_hnd), "r"(_result)
			: );
}
inline static void tsk_HND_SET_STATUS(HANDLE volatile hnd, RES_CODE volatile result)
{
	register HANDLE _hnd asm("r0") = hnd;
	register unsigned int _result asm("r1") = result;

	asm volatile ("swi %0"
			:
			: "I" (usr_HND_SET_STATUS_swi), "r"(_hnd), "r"(_result)
			: );
}
#endif

#endif /* HANDLE_H_ */
