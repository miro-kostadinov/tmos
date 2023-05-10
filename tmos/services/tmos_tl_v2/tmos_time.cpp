/*
 * tmos_time.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: miro
 */

#include <tmos.h>
#include <tmos_time.h>

const char time_t::day_name[8][4] = {"???", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char time_t::mon_name[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const uint8_t time_t::month_lengths[] = {0,	  31,   28,    31,    30,    31,    30,    31,     31,   30,     31,   30,   31 };
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

const time_t time_t::MAX_TIME = {0,59,59,23,31,12,2100 }; // 2100-31-12 23:59:59/591110778765654784ULL

WEAK time_t get_current_time()
{
	time_t tm;
	tm = 0ULL;
	return tm;
}

int time_t::is_leap_year (int year)
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
				if(hour <= 23 && minute <= 59 && second < 60)
					return true;
			}
		}
	}

	return false;
}

/**
 *
 * @param change_hour - offset from UTC at which time change occurs
 *   1 for all Europe, for America change is at 2am local time
 * @return true when this is in summer time
 */
bool time_t::is_DST(unsigned int change_hour) const
{
	time_t tmp;

	tmp.year = year;    // DST start 1:59:59
	tmp.mon = 3;
	tmp.mday = 31 - ((year * 5) / 4 + 4) % 7;
	tmp.hour = change_hour;
	tmp.minute = 0;
	tmp.second = 0;

	if(*this > tmp)
	{
		tmp.mon = 10;	// DST end 2:59:59
		tmp.mday = 31 - ((year * 5) / 4 + 1) % 7;
		if(*this < tmp)
			return true;
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

int time_t::sscanf(const char* buf, const char* format, ... )
{
	int scaned = 0, new_pos, tmp, pos=0;
	va_list lst;
	va_start(lst, format);
	bool stop = false;
	if(buf && format)
	{
		while(format[0] && buf[0] && !stop)
		{
			if(format[0]== '%')
			{
				format++;
				switch(format[0])
				{
				case 't':
					format++;
					if(format[0] == 'M')
					{
						for(int i=0; i<12; i++)
						{
							if(!strncmp(buf, mon_name[i], 3))
							{
								new_pos = 3;
								mon = i+1;
								break;
							}
						}
						if(new_pos)
							break;
					}
					stop = true;
					continue;
					break;

				case 'Y':
					if( 2 != tmos_sscanf(buf, "%4u%n", &tmp, &new_pos))
					{
						stop = true;
						continue;
					}
					year = tmp;
					break;
				case 'y':
				case 'M':
				case 'D':
				case 'h':
				case 'm':
				case 's':
					if( 2 != tmos_sscanf(buf, "%02u%n", &tmp, &new_pos))
					{
						stop = true;
						continue;
					}
					switch(format[0])
					{
					case 'y':
						year = tmp + 2000;
						break;
					case 'M':
						mon = tmp;
						if(tmp <1 || tmp > 12)
							stop = true;
						break;
					case 'D':
						mday = tmp;
						if(tmp <1 || tmp > 31)
							stop = true;
						break;
					case 'h':
						hour = tmp;
						if(tmp < 0 || tmp >= 24 )
							stop = true;
						break;
					case 'm':
						minute = tmp;
						if(tmp < 0 || tmp > 59)
							stop = true;
						break;
					case 's':
						second = tmp;
						if(tmp < 0 || tmp > 59)
							stop = true;
						break;
					}
					if(stop)
						continue;
					break;

				case 'n':
				{
					int* arg;
					arg = va_arg(lst, int* );
					*arg = pos;
					break;
				}
				default:
					stop = true;
					continue;
				}
				scaned++;
				format++;

				while(buf[0]&& new_pos)
				{
					buf++; pos++; new_pos--;
				}
				if(!buf[0])
					continue;
			}
			else
			{
				if(buf[0] != format[0])
				{
					stop = true;
					continue;
				}
				if(buf[0] == ' ')
				{
					while(buf[0] == ' ')
					{
						buf++; pos++;
					}
					while(format[0] == ' ')
						format++;
				}
				else
				{
					buf++; pos++; format++;
				}
			}
		}
	}
	if(!stop && buf[0]== 0 && format[0])
	{
		if(format[0] == '%' && format[1] == 'n')
		{
			int* arg;
			arg = va_arg(lst, int* );
			*arg = pos;
		}
	}
	va_end(lst);
	return scaned;
}

static unsigned int read2XX(const char* str, unsigned int& pos, bool&res)
{
	if(!str[pos] || !IS_DIGIT(str[pos]) || !IS_DIGIT(str[pos+1]) || !res )
	{
		res = false;
		return 0;
	}

	unsigned value = (str[pos++]-'0')*10;
	return (value + str[pos++] - '0');
}

unsigned int time_t::set_from_xml_datetime(const char* str)
{
	//YYYY-MM-DDThh:mm:ss
	bool res=true;
	unsigned int pos=0;

	year = read2XX(str, pos, res);
	mon = read2XX(str, pos, res);
	year = year*100 +mon;
	if(str[pos++] != '-')
		return 0;

	mon = read2XX(str, pos, res);
	if(str[pos++] != '-')
		return 0;

	mday = read2XX(str, pos, res);
	if(str[pos++] != 'T')
		return 0;

	hour = read2XX(str, pos, res);
	if(str[pos++] != ':')
		return 0;

	minute = read2XX(str, pos, res);
	if(str[pos++] != ':')
		return 0;

	second = read2XX(str, pos, res);

	//skip any fractional part
	if(str[pos]=='.')
	{
		while(IS_DIGIT(str[++pos]))
			;
	}

	//skip time zone
	if(str[pos]=='Z')
	{
		pos++;
	} else
	{
		if((str[pos]=='+') || (str[pos]=='-'))
		{
			pos++;
			read2XX(str, pos, res);
			if(str[pos++] != ':')
				res=false;
			read2XX(str, pos, res);
		}
	}

	if(res && is_valid())
		return pos;
	return 0;
}

unsigned int time_t::set_from_xml_date(const char* str)
{
	//YYYY-MM-DDThh:mm:ss
	bool res=true;
	unsigned int pos=0;

	year = read2XX(str, pos, res);
	mon = read2XX(str, pos, res);
	year = year*100 +mon;
	if(str[pos++] != '-')
		return CTIME_INVALID_YEAR;

	mon = read2XX(str, pos, res);
	if(str[pos++] != '-')
		return CTIME_INVALID_MONTH;

	mday = read2XX(str, pos, res);
	if(str[pos])
	{
		if(str[pos++] != 'T')
			return CTIME_INVALID_HOUR;

		hour = read2XX(str, pos, res);
		if(str[pos] != '.' && str[pos] != ':')
			return CTIME_INVALID_MINUTE;

		minute = read2XX(str, ++pos, res);
		if(str[pos] != '.' && str[pos] != ':')
			return 0;

		second = read2XX(str, ++pos, res);
	}else
	{
		hour =0;
		minute =0;
		second = 0;
	}

	//skip any fractional part
	if(str[pos]=='.')
	{
		while(IS_DIGIT(str[++pos]))
			;
	}

	//skip time zone
	if(str[pos]=='Z')
	{
		pos++;
	} else
	{
		if((str[pos]=='+') || (str[pos]=='-'))
		{
			pos++;
			read2XX(str, pos, res);
			if(str[pos] != '.' && str[pos] != ':')
				res=false;
			pos++;
			read2XX(str, pos, res);
		}
	}

	if(res && is_valid())
		return pos;
	return 0;
}

/** Set from exactly YYMMDDHHMMSS
 *  	no spacer allowed
 *
 * @param val
 * @return
 */
unsigned int time_t::set_from_YYMMDDHHMMSS(const char* str)
{
	bool res=true;
	unsigned int pos=0;

	year = 2000 + read2XX(str, pos, res);
	mon = read2XX(str, pos, res);
	mday = read2XX(str, pos, res);
	hour = read2XX(str, pos, res);
	minute = read2XX(str, pos, res);
	second = read2XX(str, pos, res);

	if( res && is_valid() )
		return pos;
	return 0;
}


static int read(const char* &val)
{
	int res = 0;
	while( (val[0] >='0') && (val[0] <= '9'))
	{
		res *= 10;
		res += val[0] - '0';
		val++;
	}
	if(val[0])
		val++;
	return res;
}

/** Set from various formats such as:
 * 		 "00/02/02,16:06:11"
 * 		 "2000*2*2,16:06:11"
 *
 * @param val
 */
void time_t::set_from_YYMMDDHHMMSSZZ(const char* val)
{

	year = read(val);
	if(year <100)
	{
		year += 2000;
	}
	mon = read(val);
	mday = read(val);
	hour = read(val);
	minute = read(val);
	second = read(val);
}

CSTRING time_t::sprintf( const char* format) const
{
	CSTRING str;
	sprintf(str, format);
	return str;
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
				str.appendf("%04u", year );
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
			case 'b':
				if(mon && mon < 13)
					str += mon_name[mon-1];
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

CSTRING time_t::xml_date_time(void) const
{
	CSTRING DateTime;

	DateTime.format("%04.4d-%02.2u-%02.2uT%02.2u:%02.2u:%02.2u",
			year, mon, mday, hour, minute, second);
	return DateTime;
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

uint64_t time_t::get_atomic() const
{
	uint64_t time;
	__disable_irq();
	time = time64;
	__enable_irq();
	return time;
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


