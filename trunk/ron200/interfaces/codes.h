/*
 * codes.h
 *
 *  Created on: 03.05.2011
 */

#ifndef CODES_H_
#define CODES_H_

#define EINVAL           22    /* Invalid argument */


enum SystemErrors
{
        EX_SUCCESS=0,           // no error -> all is OK
        EX_GENERIC=0x201,       // 0x01: generic error
//        EX_INVPARAM,            // 0x02: invalid parameter
//        EX_INVOBJECT,           // 0x03: invalid object or object reference
//        EX_NOTFREE,             // 0x04: the requested object is currently allocated
//        EX_NOTUSED,             // 0x05: the requested object is currently unused
//        EX_NOTEXIST,            // 0x06: the object does not exist
//        EX_NOTPRESENT,          // 0x07: the requested object or function is not present
//        EX_CONTEXT,             // 0x08: the requested action is not valid in this context
//        EX_LIMIT,               // 0x09: the given limit was exceeded
//        EX_NOSUPPORT,           // 0x0A: no support for the requested action
//        EX_TIME,                // 0x0B: timeout occurred
//        EX_NOMEM,               // 0x0C: out of memory
//        EX_SATUR,               // 0x0D: object is saturated
//        EX_ALLOC_ERR,           // 0x0E: memory allocation error
//        EX_BAD_CALL,            // 0x0F: bad function call
//        EX_NORDY,               // 0x10: object was not ready
//        EX_DELETE,              // 0x11: the associated object was deleted
//        EX_ALRDY_FREE,          // 0x12: object is already free
//        EX_ALRDY_EXIST,         // 0x13: object already exists
//        EX_TESTFAIL,            // 0x14: test failed
//        EX_SHADOW,              // 0x15: shadow memory not consistent
//        EX_TRANS,               // 0x16: illegal transaction state
//        EX_NOHARDWARE,          // 0x17: hardware not present
//        EX_NOCREATE,            // 0x18: cannot create requested object
//        EX_OSINTERNAL,          // 0x19: OS internal error
//        EX_MSGUNKNOWN,          // 0x1A: unknown message
//        EX_CRCFAIL,             // 0x1B: CRC check failed
//        EX_MEMCORRUPT,          // 0x1C: Memory structure corrupted
//        EX_SERCOMMERR,          // 0x1D: Serial communication error
//        EX_FATAL,               // 0x1E: Fatal Error
//        EX_INTERNAL,            // 0x1F: internal error
//        EX_INTUNKNOWN,          // 0x20: unknown interrupt
//        EX_STACKOVER,           // 0x21: stack overrun
//        EX_I2CERROR,            // 0x22: MBUS Error
//        EX_MODULENOTINIT,       // 0x23: Module is not initialized
//        EX_FLASHERROR,          // 0x24: Flash error
//
//        EX_VERIFYERROR,         // 0x25: device write/verify error
//        EX_DISKFULL,            // 0x26: disk/media full
//        EX_NOMATCH,             // 0x27: no matching item found
//        EX_INCOMPLETE,          // 0x28: incomplete command sequence
//        EX_CMDCORRUPT,          // 0x29: corrupted command sequence
//        EX_EMPTY,               // 0x2A: empty result
//        EX_INHIBIT,             // 0x2B: operation blocked for safety reasons
//        EX_KILL,                // 0x2C: killed during operation by other thread
//        EX_BADCONFIG,	        // 0x2D: bad configuration record
//        EX_TOOBIG,    	        // 0x2E: object too big to handle
//        EX_NETWORK_ERR,			// 0x2F: general network error
//		EX_SERVER_ERR,			// 0x30: general network error or server unreachable
//		EX_USER_ERR,			// 0x31: user not found/authenticated
//		EX_FILE_ERR,			// 0x32: file not found
//		EX_PATH_ERR,			// 0x33: file name error
//		EX_NOTCONNECTED,		// 0x34
//		EX_NOPOWER,				// 0x35
//		EX_IO_ERR,				// 0x36: I/O error
//		EX_NOCARD,				// 0x37: No Card in Slot
//		EX_INVARGNUM,			// invalid number of arguments
//		EX_NOTALIGNED,			// If not aligned
//		EX_MOUNT,				// unable to mount media
//		EX_NOPRINT,				// error is handled
//		EX_MULTI_0000,			// Multi command status
//		EX_MULTI_0001,			//
//		EX_MULTI_0010,			//
//		EX_MULTI_0011,			//
//		EX_MULTI_0100,			//
//		EX_MULTI_0101,			//
//		EX_MULTI_0110,			//
//		EX_MULTI_0111,			//
//		EX_MULTI_1000,			//
//		EX_MULTI_1001,			//
//		EX_MULTI_1010,			//
//		EX_MULTI_1011,			//
//		EX_MULTI_1100,			//
//		EX_MULTI_1101,			//
//		EX_MULTI_1110,			//
		EX_MULTI_1111			//
};

#endif /* CODES_H_ */
