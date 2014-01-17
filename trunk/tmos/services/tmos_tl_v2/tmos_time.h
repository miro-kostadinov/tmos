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
	bool is_DST(unsigned int change_hour);

	uint32_t get_week_day() const;

	int sscanf(const char* buf, const char* format, ... );
	unsigned int set_from_xml_datetime(const char* str);
	void set_from_YYMMDDHHMMSSZZ(const char* val);

	void sprintf(CSTRING& str, const char* format) const;
	CSTRING xml_date_time(void) const;

	inline time_t& operator= (const time_t& T)
	{
		time64 = T.time64;
		return *this;
	}
	inline time_t& operator= (uint64_t time)
	{
		time64 = time;
		return *this;
	}

	time_t& operator= (unsigned int seconds);

	uint64_t get_atomic();

	operator unsigned int() const;

	inline bool operator< (const time_t& T) const
	{
		return (time64 < T.time64);
	}

	inline bool operator> (const time_t& T) const
	{
		return (time64 > T.time64);
	}

	inline bool operator<= (const time_t& T) const
	{
		return (time64 <= T.time64);
	}

	inline bool operator>= (const time_t& T) const
	{
		return (time64 >= T.time64);
	}

	inline bool operator== (const time_t& T) const
	{
		return (time64 == T.time64);
	}

	inline bool operator!= (const time_t& T) const
	{
		return (time64 != T.time64);
	}

};


#endif /* TMOS_TIME_H_ */
/** @} defgroup lib_tmos_time  */
