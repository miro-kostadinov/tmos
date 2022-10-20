/*
 * cache.cpp
 *
 *  Created on: 2010-8-4
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <cache.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+			CCache
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void CCache::set_cache_pos(uint32_t pos)
{
	if(pos < cache_offset)
	{
		// pos is too old
		cache_offset =  pos;
		cache_pos = 0;
		buf.clear();
		next.free();
	} else
	{
		pos -= cache_offset;
		if(pos < buf.length())
		{
			// pos is inside the buf
			cache_pos = pos;
		} else
		{
			cache_offset += buf.length();
			pos -= buf.length();
			if(!next.empty())
			{
				buf = next;
				next.free();
			} else
			{
				buf.clear();
			}

			if(pos < buf.length())
			{
				cache_pos = pos;
			} else
			{
				cache_offset +=  pos;
				cache_pos = 0;
				buf.clear();
			}
		}
	}
}

/**
 * Reads single character from the cached object
 * @return  : 0, success - symbol
 */
char CCache::getc()
{
	if(cache_pos >= buf.length())
		if (load_next() != RES_OK)
			return 0;

	return buf.c_str()[cache_pos++];
}

/**
 * Returns the character from top of the cached object
 * @return : 0, success - symbol
 */
char CCache::topc()
{
	if(cache_pos >= buf.length())
		if (load_next() != RES_OK)
			return 0;

	return buf.c_str()[cache_pos];
}

/**
 * Reads single character from the cached object in reverse direction
 * @return
 */
char CCache::getc_rev()
{
	if (cache_pos)
	{
		return buf.c_str()[--cache_pos];
	} else
	{
		if(cache_offset)
		{
			uint32_t rev_seek;

			rev_seek = min(16u, cache_offset);
			cache_offset -= rev_seek;
			buf.clear();
			next.free();
			if (load_next() == RES_OK && rev_seek <= buf.length())
			{
				cache_pos = rev_seek;
				return buf.c_str()[--cache_pos];
			}
			cache_pos = rev_seek;
		}
	}

	return 0;
}

/**
 * Reads single character from the cached object
 * @param c - reference to store the character
 * @return RES_OK or error returned from load()
 */
RES_CODE CCache::getc(char& c)
{
	RES_CODE res;

	if(cache_pos >= buf.length())
		res = load_next();
	else
		res = RES_OK;

	if (res == RES_OK)
		c = buf.c_str()[cache_pos++];

	return (res);
}

/**
 * Reads print character (skip white spaces) from the cached object
 * @param c - reference to store the character
 * @return RES_OK or error returned from load_next()
 */
RES_CODE CCache::get_pc(char&c)
{
	RES_CODE res;

	while (1)
	{
		res = getc(c);
		if (res != RES_OK)
			break;

		if (c > ' ')
			break;
	}

	return (res);
}

/**
 * Skip white spaces
 * @return RES_OK or error returned from load_next()
 */
RES_CODE CCache::skip_ws()
{
	RES_CODE res;
	const char* ptr;
	uint32_t len;

	do
	{
		ptr = buf.c_str();
		len = buf.length();
		while(cache_pos < len)
		{
			if (ptr[cache_pos] > ' ')
				return RES_OK;
			cache_pos++;
		}

		res = load_next();
	} while (res == RES_OK);

	return (res);

}

/**
 * Skips everything up to and including c
 * @param c
 * @return
 */
RES_CODE CCache::skip_char(char c)
{
	RES_CODE res;
	const char* ptr;
	uint32_t len;

	do
	{
		ptr = buf.c_str();
		len = buf.length();
		while(cache_pos < len)
		{
			if (ptr[cache_pos++] == c)
				return RES_OK;
		}
		res = load_next();
	} while (res == RES_OK);

	return (res);
}

/**
 *  Skips XML comments
 */
void CCache::skip_xml_comments()
{
	bool found;

	do
	{
		found = false;

		if(match("<!--") == RES_OK)
		{
			found = true;
			while(match("-->") == RES_INVALID_DATA)
			{
				if(!getc())
					break;
			}
		}
		else
		{
			if(match("<?") == RES_OK)
			{
				found = true;
				while(match("?>") == RES_INVALID_DATA)
				{
					if(!getc())
						break;
				}
			}
		}

	} while(found);
}

/**
 * Undo last read (if possible)
 * @return true if the symbol was restored
 */
bool CCache::ungetc()
{
	if (cache_pos)
	{
		cache_pos--;
		return (true);
	} else
	{
		if(cache_offset)
		{
			if(!buf.empty())
			{
				buf += next;
				next = buf;
				buf.clear();
			}
			cache_offset--;
			return (true);
		}
	}

	return (false);
}

/**
 * Read a line from the cached object
 * @param var - CSTRING where to store the line
 * @return RES_OK or error returned from load_next()
 */
RES_CODE CCache::readline(CSTRING& var)
{
	RES_CODE res;

	do
	{
		int len = buf.length() - cache_pos;

		if(len > 0)
		{
			const char* ptr = buf.c_str()+ cache_pos;
			int i = 0;

			do
			{
				if (!ptr[i] || ptr[i] == '\n')
				{
					//we found \n or \0
					var.append(ptr, i);
					cache_pos += i + 1;
					return RES_OK;
				}
			} while(++i < len);

			var.append(ptr, len);
		}

		res = load_next();
	} while (res == RES_OK);

	return (res);
}

/**
 * Read a line from the cached object for a given time
 * @param str - CSTRING where to store the line
 * @param time - timeout limit
 * @return
 */
RES_CODE CCache::readline(CSTRING& str, unsigned int time)
{
	CURRENT_TASK->time = CURRENT_TIME + time;
	return (readline(str));
}

/**
 *  Default cache load_next method
 *
 * @return
 */
RES_CODE CCache::load_next()
{
	cache_offset += buf.length();
	cache_pos = 0;

	if(next.empty())
	{
		buf.clear(); // avoid memory release

		return RES_EOF;
	} else
	{
		buf = next;
		next.clear();
	}

	return RES_OK;
}

/**
 * Reads a name from the cache
 *
 * Description:
 * 		Name	   		::=   	NameStartChar (NameChar)*
 * 		NameStartChar	::=   	":" | [A-Z] | "_" | [a-z]	//add "?"
 * 		NameChar	   	::=   	NameStartChar | "-" | "." | [0-9]
 * @param var
 * @return RES_OK if the char was skipped, RES_EOF if no more tags, errors..
 */
RES_CODE CCache::get_name(CSTRING& var)
{
	char ch;
	RES_CODE res;

	res = get_pc(ch);
	if (res == RES_OK)
	{
		if (ch == ':' || ch == '_' || ch == '?' || IS_ALPHA(ch))
		{
			var += ch;

			while (!var.empty())
			{
				res = getc(ch);
				switch (res)
				{
				case RES_OK:
					if (ch == ':' || ch == '_' || ch == '-' || ch == '.'
							|| IS_ALPHANUM(ch))
					{
						var += ch;
					}
					else
					{
						ungetc();
						return RES_OK;
					}
					break;

				case RES_EOF:
					return RES_OK;

				default:
					return (res);
				}

			}
			return RES_OUT_OF_MEMORY;
		}
		ungetc();
		res = RES_INVALID_DATA;
	}
	return (res);
}

/**
 * Repeat string from position
 * @param str
 * @param from
 * @return
 */
RES_CODE CCache::repeat_from(CSTRING& str, uint32_t from)
{
	uint32_t pos;
	RES_CODE res= RES_OK;
	int start, len;

	pos = cache_offset + cache_pos;
	while(from < pos)
	{
		start = from - cache_offset;
		len = buf.length() - start;
		if(start < 0 || len <= 0)
		{
			// it's not in the cache anymore
			set_cache_pos(from);
			res = load_next();
			if(res != RES_OK)
				break;
		} else
		{
			// cache_offset <= from < pos  copy [start.. pos-offset]
			len = min(pos-from, (unsigned)len);
			from += len;
			str.append(buf.c_str()+start, len);
		}
	}
	set_cache_pos(pos);
	return res;
}

const char* CCache::get_cptr(uint32_t len)
{
	if(cache_pos >= buf.length())
		if (load_next() != RES_OK)
			return nullptr;

	// not enough in the cache?
	if((cache_pos + len) > buf.length())
	{
		//nope, free some space
		if(cache_pos)
		{
			buf.erase(0, cache_pos);
			cache_offset += cache_pos;
			cache_pos = 0;
		}

		if(!next.empty())
		{
			buf += next;
			next.free();
		}

		//check again
		if(len > buf.length())
		{
			CSTRING s;

			s = buf;
			len = cache_offset;
			load_next();
			s += buf;
			buf = s;
			cache_offset = len;
		}

	}
	return buf.c_str()+ cache_pos;
}

/**
 * Puts a string in the cache line
 * @param var
 * @return
 */
RES_CODE CCache::buffer(CSTRING &var)
{
	if (!var.empty())
	{
		if(cache_pos < buf.length())
		{
			if(cache_pos)
			{
				cache_offset += cache_pos;
				buf.erase(0, cache_pos);
			}
			buf += next;
			next = buf;
		} else
			cache_offset += buf.length();

		cache_pos = 0;
		buf = var;
	}
	return RES_OK;
}

/**
 * Strips the white space and then tries to read and match.
 * @param str
 * @return RES_OK if str matches. Otherwise the cache is restored
 */
RES_CODE CCache::match(const char* str)
{
	RES_CODE res;

	res = skip_ws();

	if (res == RES_OK && str)
	{
		uint32_t start_pos = get_cache_pos();
		const char* ptr;

		if(cache_pos >= buf.length())
		{
			res = load_next();
		}
		if(res == RES_OK)
		{
			ptr = buf.c_str();
			while(*str)
			{
				if(*str == ptr[cache_pos])
				{
					str++;
					cache_pos++;
				} else
				{
					if(cache_pos >= buf.length())
					{
						res = load_next();
						if(res != RES_OK)
							break;
						ptr = buf.c_str();
					} else
					{
						res = RES_INVALID_DATA;
						break;
					}
				}
			}
			if(res != RES_OK)
				set_cache_pos(start_pos);
		}
	}

	return (res);
}

/**
 * Match name
 * @param str
 * @return
 */
RES_CODE CCache::match_name(const char* str)
{
	RES_CODE res;
	uint32_t pos;

	pos = get_cache_pos();
	res = match(str);

	if (res == RES_OK)
	{
		char c = topc();

		if (IS_ALPHANUM(c) || c == ':' || c == '_' || c == '-' || c == '.')
		{
			//this is not a name
			set_cache_pos(pos);
			res = RES_INVALID_DATA;
		}
	}

	return (res);
}

RES_CODE CCache::matchc(char c)
{
	RES_CODE res;

	if(cache_pos >= buf.length())
		res = load_next();
	else
		res = RES_OK;

	if (res == RES_OK)
	{
		if(c == buf.c_str()[cache_pos])
			cache_pos++;
		else
			res = RES_INVALID_DATA;
	}
	return (res);
}

bool CCache::match_uint()
{
	char c, sign;

	c = topc();
	if(c == '+')
	{
		sign = c;
		getc();
		c = topc();
	} else
		sign = 0;

	if(IS_DIGIT(c))
	{
		unsigned int old_val, new_val;

		new_val=0;
		do
		{
			old_val = new_val;
			new_val *= 10;
			new_val += c - '0';
			if(new_val < old_val)
				break; //overflow
			getc();
			c = topc();
		} while ( IS_DIGIT(c) );
		return true;
	}
	if(sign)
		ungetc();
	return false;

}

bool CCache::match_long()
{
	char c, sign;

	c = topc();
	if(c == '-'|| c == '+')
	{
		sign = c;
		getc();
		c = topc();
	} else
		sign = 0;

	if(IS_DIGIT(c))
	{
		unsigned long long val;

		val=0;
		do
		{
			val *= 10;
			val += c - '0';
			if(val >> 63)
				break; //overflow

			getc();
			c = topc();
		} while ( IS_DIGIT(c) );
		return true;
	}
	if(sign)
		ungetc();
	return false;
}

bool CCache::match_ulong()
{
	char c, sign;

	c = topc();
	if(c == '+')
	{
		sign = c;
		getc();
		c = topc();
	} else
		sign = 0;

	if(IS_DIGIT(c))
	{
		unsigned long long val;

		val=0;
		do
		{
			val *= 10;
			val += c - '0';
			if(val >> 63)
				break; //overflow

			getc();
			c = topc();
		} while ( IS_DIGIT(c) );
		return true;
	}
	if(sign)
		ungetc();
	return false;
}

bool CCache::match_uamount()
{
	if(match_ulong())
	{
		if(topc() == '.')
		{
			getc();
			if(IS_DIGIT(topc()))
			{
				getc();
				if(IS_DIGIT(topc()))
					getc();
			}
		}
		return true;
	}
	return false;
}

bool CCache::match_amount()
{
	if(match_long())
	{
		if(topc() == '.')
		{
			getc();
			if(IS_DIGIT(topc()))
			{
				getc();
				if(IS_DIGIT(topc()))
					getc();
			}
		}
		return true;
	}
	return false;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+			CCachedHandle
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/**
 * Load (virtual) from handle
 * @return
 */
RES_CODE CCachedHandle::load_next()
{
	RES_CODE res;
	unsigned int len;

	res = CCache::load_next();
	if(res == RES_EOF)
	{
		len = buf.reserve(size);
		if (len)
		{
			res = hnd->tsk_resume_read((void*) buf.c_str(), len);
			buf.m_set_size(len - hnd->len);
		} else
			res = RES_OUT_OF_MEMORY;
	}
	return res;
}

