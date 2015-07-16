/**************************************************************************//**
 * @ingroup	 libs
 * @defgroup lib_tmos_time TMOS time
 * @{
 * @file     tmos_time.h
 * @brief    tmos_time header
 * @version  V3.00
 * @date     15. Feb 2013
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/
#ifndef TMOS_TIME_H_
#define TMOS_TIME_H_

#include <tmos.h>


struct time_t
{
	union
	{
		struct
		{
			uint8_t subsec;		//!< sub seconds 00-99
			uint8_t second;		//!< seconds 00-59
			uint8_t minute;		//!< minutes 00-59
			uint8_t hour; 		//!< hours 00-23

			uint8_t mday;		//!< day 01-31
			uint8_t mon;		//!< month 01-12
			uint16_t year; 		//!< year since 2000
		};
		uint64_t time64;
	};

	bool is_valid() const;
	uint32_t get_week_day() const;
	void sprintf(CSTRING& str, const char* format) const;

	time_t& operator= (const time_t& T);
	time_t& operator= (unsigned int seconds);

	operator unsigned int() const;

	bool operator< (const time_t& T) const
	{
		return (time64 < T.time64);
	}

	bool operator> (const time_t& T) const
	{
		return (time64 > T.time64);
	}

	bool operator<= (const time_t& T) const
	{
		return (time64 <= T.time64);
	}

	bool operator>= (const time_t& T) const
	{
		return (time64 >= T.time64);
	}

	bool operator== (const time_t& T) const
	{
		return (time64 == T.time64);
	}

	bool operator!= (const time_t& T) const
	{
		return (time64 != T.time64);
	}

};


#endif /* TMOS_TIME_H_ */
/** @} defgroup lib_tmos_time  */
