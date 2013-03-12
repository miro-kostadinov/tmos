/*
 * tmos_time.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: miro
 */

#include <tmos.h>
#include <tmos_time.h>
#include <cmsis_cpp.h>

const char day_name[8][4] = {"???", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const uint8_t month_lengths[] = {0,	  31,   28,    31,    30,    31,    30,    31,     31,   30,     31,   30,   31 };
const uint8_t month_start[]   = {0,	   0,    3,     3,     6,     1,     4,     6,      2,    5,      0,    3,   5 };
const uint16_t month_base_day[12] =
{
   0,     // 0 January base
   31,    // 1 February base
   31+28, // 2 March base (+28 or +29)
   31+28+31,
   31+28+31+30,
   31+28+31+30+31,
   31+28+31+30+31+30,
   31+28+31+30+31+30+31,
   31+28+31+30+31+30+31+31,
   31+28+31+30+31+30+31+31+30,
   31+28+31+30+31+30+31+31+30+31,
   31+28+31+30+31+30+31+31+30+31+30,
};


static int is_leap_year (int year)
{
    return (year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0);
}


bool time_t::is_valid() const
{
	unsigned int days;

	if(year >= 1900 && year <= 2100)
	{
		if(mon >=1 && mon <=12)
		{
			days = month_lengths[mon];
			if(mon == 2)
				days += is_leap_year(year);
			if(mday >= 1 && mday <= days )
			{
				if(hour <= 23 && minute <= 59)
					return true;
			}
		}
	}

	return false;
}

uint32_t time_t::get_week_day() const
{
	if(is_valid())
	{
		uint32_t wday;

		wday = year %100;

		// years  + leap years  + mon_start         +  day    + century
		wday += (wday /4) + month_start[mon] + mday + 6;
		//correct for leap
		if(mon < 3)
			wday -= is_leap_year(year);

		return (wday % 7) +1;
	}
	return 0;
}

void time_t::sprintf(CSTRING& str, const char* format) const
{
	while(format[0])
	{
		if(format[0] == '%')
		{
			format++;
			switch (format[0])
			{
			case 0:
				return;
			case 'y':
				str.appendf("%02u", year %100);
				break;
			case 'Y':
				str.appendf("%u", year );
				break;
			case 'M':
				str.appendf("%02u", mon);
				break;
			case 'D':
				str.appendf("%02u", mday);
				break;
			case 'h':
				str.appendf("%02u", hour);
				break;
			case 'm':
				str.appendf("%02u", minute);
				break;
			case 's':
				str.appendf("%02u", second);
				break;
			case 'w':
				str += day_name[get_week_day()];
				break;
			default:
				str += format[0];
				break;
			}
		} else
		{
			str += format[0];
		}
		format++;
	}
}

time_t& time_t::operator= (const time_t& T)
{
	__disable_irq();
	time64 = T.time64;
	__enable_irq();
	return *this;
}

time_t& time_t::operator= (unsigned int seconds)
{
	unsigned int delta;
	bool is_leap;

	// There is one leap day for each 4 years.
	// Calculate how many "4 year" chunks that has happened
	// since 2004.
	//
	delta = (365*4+1)* 86400ul; 		//delta = 4 years
	year = seconds / delta;
	seconds -= year*delta;				//seconds = seconds in 4 years
	year = year*4 + 2000;

	if ( year >= 2100 ) // 2100 is NOT a leap year
		seconds+= 86400ul;
	delta = (366l * 86400l); // 1st chunk is a leap year.

	if ( year == 2100 ) // Inside the 2100..2103 area
		delta = (365l * 86400l); // Then NOT a leap year.

	while ( seconds >= delta )
	{
		seconds-= delta;
		year++;
		delta = (365l * 86400l); // Then NOT a leap year.
	};

	is_leap = ( delta >= (366l * 86400l) ); // 1st chunk is a leap year.

	for(mon=1; mon<13; mon++)
	{
		mday = month_lengths[mon];
		if ( is_leap && (mon == 2) ) // February
			mday++;
		delta = mday * 86400l;

		if ( seconds < delta )
			break; // Month is known.

		seconds-= delta;
	}

	mday = seconds / 86400l;
	seconds-= (mday * 86400l);			//seconds = seconds of the day
	mday++;

	hour = (seconds / 3600);
	seconds-= (hour * 3600l);			//seconds = seconds of the hour

	minute = (seconds / 60 );
	seconds-= (minute*60l);				// seconds = seconds

	second = seconds;
	subsec = 0;

	return *this;
}

time_t::operator unsigned int() const
{
	if(is_valid())
	{
		unsigned int y, m, d;

		y = year;
		if(y >= 2000)
			y -= 2000;
		m = mon-1;
		d = mday-1;
		if ( m >= 2 ) // >= March
			if ( (y & 0x03) == 0 && y != 100) // Possible leap year?
					d++;

		d += y*365l + month_base_day[m];
		if(y)
			d+= ((y-1)/4) + 1;

		if(y > 100)
			d--;

		return  second + minute*60 + hour*3600 + d*86400;
	}
	return 0;
}


