/**************************************************************************//**
 * @ingroup lib_cstring
 * @{
 * @file     str_list.cpp
 * @brief    CSTRING class implementation
 * @version  V3.00
 * @date     18. May 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include "str_list.h"
#include <stdarg.h>


#define str_malloc1		(str_storage*)tsk_malloc
#define str_free 		tsk_free
#define STR_MIN_SIZE 	(sizeof(str_storage)+1)

#define storage_realloc(size) 	(storage.ram = (str_storage*)tsk_realloc(storage.ram, size))
#define storage_malloc(size) 	(storage.ram = (str_storage*)tsk_malloc(size))

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 					 CString
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
char CSTRING::dummy_char;

/** Default constructor
 *
 * @return
 */
CSTRING::CSTRING()
{
	storage.adr = NULL;
}

/** Constructor
 *
 * @param size
 * @return
 */
CSTRING::CSTRING(unsigned int size)
{
	if( storage_malloc(size + STR_MIN_SIZE) )
	{
		storage.ram->refs = 1;
		storage.ram->len = 0;
		storage.ram->buf[0] = 0;
	}
	else
	{
		ASSERT(storage.ram);
	}
}

/**
 * Copy constructoor
 * @param cstr
 * @return
 */
CSTRING::CSTRING(const CSTRING& cstr)
{
	if(RAM_ADR(storage.adr = cstr.storage.adr))
	{
		locked_inc_short(&storage.ram->refs);
	}
}


/**
 * Constructor
 * @param str
 * @return
 */
CSTRING::CSTRING(const char *str)
{
	storage.rom = str;
	if(RAM_ADR(storage.adr))
	{
		unsigned int len;

		len = strlen(str);
		if( storage_malloc(len + STR_MIN_SIZE) )
		{
			storage.ram->refs = 1;
			storage.ram->len = len;
			strcpy(storage.ram->buf, str);
		}
		else
		{
			ASSERT(storage.ram);
		}
	}
}

/** Constructor
 *
 * @param str
 * @param len
 * @return
 */
CSTRING::CSTRING(const char *str, unsigned int len)
{
	if( storage_malloc(len + STR_MIN_SIZE) )
	{
		storage.ram->refs = 1;
		storage.ram->len = len;
		memcpy(storage.ram->buf, str, len);
		storage.ram->buf[len]=0;
	}
	else
	{
		ASSERT(storage.ram);
	}
}

/** Destructor
 *
 * @return
 */
CSTRING::~CSTRING()
{
	if(RAM_ADR(storage.adr))
		if(locked_dec_short(&storage.ram->refs) <= 0)
			str_free(storage.ram);
}

/** free allocated memory
 *
 * @return
 */
void CSTRING::free()
{
	if(RAM_ADR(storage.adr))
		if(locked_dec_short(&storage.ram->refs) <= 0)
			str_free(storage.ram);
	storage.rom = "";
}


//Assign

/** Assign CSTRING
 *
 * @param s
 * @return
 */
CSTRING& CSTRING::assign( const CSTRING& s )
{
	if(storage.adr != s.storage.adr)
	{
		if(RAM_ADR(storage.adr))
			if(locked_dec_short(&storage.ram->refs) <= 0)
				str_free(storage.ram);

		if(RAM_ADR((storage.adr = s.storage.adr)))
		{
			locked_inc_short(&storage.ram->refs);
		}
	}
	return (*this);
}

/** Assign substring
 *
 * @param s
 * @param len
 * @return
 */
CSTRING& CSTRING::assign( const char* s, unsigned int len )
{
	clear();
	if(reserve(len))
	{
		strncpy(storage.ram->buf, s, len);
		storage.ram->buf[len] = 0;
		storage.ram->len = len;
	}
	return (*this);
}

/** Assign string
 *
 * @param s
 * @return
 */
CSTRING& CSTRING::assign( const char* s )
{
	unsigned int len;

	if(RAM_ADR(s))
	{
		if( (len = strlen(s)) )
		{
			//assign non empty string
			if(RAM_ADR(storage.adr))
			{
				if(locked_dec_short(&storage.ram->refs) > 0)
					storage.ram = NULL;
				else
				{
					if(len < STR_SIZEOF(storage.ram))
					{
						strcpy(storage.ram->buf, s);
						storage.ram->refs = 1;
						storage.ram->len = len;
						return (*this);
					}
				}
			} else
				storage.rom = NULL;

			if(storage_realloc(len + STR_MIN_SIZE))
			{
				strcpy(storage.ram->buf, s);
				storage.ram->refs = 1;
				storage.ram->len = len;
			}
			else
			{
				ASSERT(storage.ram);
			}
			return (*this);
		}
		s="";
	}

	if(RAM_ADR(storage.adr))
		if(locked_dec_short(&storage.ram->refs) <= 0)
			str_free(storage.ram);
	storage.rom = s;

	return (*this);
}

/** Assign char
 *
 * @param ch
 * @return
 */
CSTRING& CSTRING::assign( char ch )
{

	if(!RAM_ADR(storage.adr) || (locked_dec_short(&storage.ram->refs) > 0))
		storage_malloc(1 + STR_MIN_SIZE);
	if(storage.adr)
	{
		storage.ram->refs = 1;
		storage.ram->len = ch?1:0;
		storage.ram->buf[0] = ch;
		storage.ram->buf[1] = 0;
	}
	return (*this);
}

//Append
/**
 * Append CSTRING
 * @param s
 * @return
 */
CSTRING& CSTRING::append(const CSTRING& s)
{
	const char * src;

	if( RAM_ADR(src = s.storage.rom) )
	{
		src = s.storage.ram->buf;

		if(s.storage.adr == storage.adr)
		{
			//special case e.g.   str.append(str)
			//we need to make sure that the realloc does not change our source pointer

			locked_inc_short(&storage.ram->refs);
			append(src);
			locked_dec_short(&storage.ram->refs);
			return (*this);
		}
	}
	return (append(src));
}

/** Append string
 *
 * @param s
 * @return
 */
CSTRING& CSTRING::append(const char* s)
{
	unsigned int len;
	str_storage *	tmp;

	if(s)
	{
		if( (len = strlen(s)) )
		{
			if(storage.adr)
			{
				if(ROM_ADR(storage.adr))
				{
					unsigned int current_len = strlen(storage.rom);

					len += current_len;
					tmp = str_malloc1(len + STR_MIN_SIZE);
					if(tmp)
					{
						strcpy(tmp->buf, storage.rom);
						tmp->len = current_len;
						tmp->refs = 1;
					}
					else
					{
						ASSERT(tmp);
					}
					storage.ram = tmp;

				} else
				{
					len += storage.ram->len;

					if( storage.ram->refs > 1)
					{
						// we need to make a new storage

						tmp = str_malloc1(len + STR_MIN_SIZE);
						if(tmp)
						{
							strcpy(tmp->buf, storage.ram->buf);
							tmp->len = storage.ram->len;
							tmp->refs = 1;
						}
						else
						{
							ASSERT(tmp);
						}
						if(locked_dec_short(&storage.ram->refs) <= 0)
							str_free(storage.ram);	//oops someone released the storage
						storage.ram = tmp;

					} else
					{
						// we are the only owner of the storage
						if(len >= STR_SIZEOF(storage.ram))
						{
							//but it is too small -> realloc
							storage_realloc(len + STR_MIN_SIZE);
							ASSERT(storage.ram);
						}
					}
				}

			} else
			{
				if(storage_malloc(len + STR_MIN_SIZE))
				{
					storage.ram->len = 0;
					storage.ram->refs = 1;
				}
				else
				{
					ASSERT(storage.ram);
				}
			}

			//we are done with storage
			if(RAM_ADR(storage.adr))
			{
				// append now...
				strcpy(storage.ram->buf + storage.ram->len, s);
				storage.ram->len = len;
			}

		}
	}
	return (*this);
}

/** Append substrng
 *
 * @param s
 * @param l
 * @return
 */
CSTRING& CSTRING::append(const char* s, unsigned int l)
{
	unsigned int len;
	str_storage *	tmp;

	if(s)
	{
		if( (len = l) )
		{
			if(storage.adr)
			{
				if(ROM_ADR(storage.adr))
				{
					unsigned int current_len = strlen(storage.rom);

					len += current_len;
					tmp = str_malloc1(len + STR_MIN_SIZE);
					if(tmp)
					{
						strcpy(tmp->buf, storage.rom);
						tmp->len = current_len;
						tmp->refs = 1;
					}
					else
					{
						ASSERT(tmp);
					}
					storage.ram = tmp;

				} else
				{
					len += storage.ram->len;

					if( storage.ram->refs > 1)
					{
						// we need to make a new storage

						tmp = str_malloc1(len + STR_MIN_SIZE);
						if(tmp)
						{
							strcpy(tmp->buf, storage.ram->buf);
							tmp->len = storage.ram->len;
							tmp->refs = 1;
						}
						else
						{
							ASSERT(tmp);
						}
						if(locked_dec_short(&storage.ram->refs) <= 0)
							str_free(storage.ram);	//oops someone released the storage
						storage.ram = tmp;

					} else
					{
						// we are the only owner of the storage
						if(len >= STR_SIZEOF(storage.ram))
						{
							//but it is too small -> realloc
							storage_realloc(len + STR_MIN_SIZE);
							ASSERT(storage.ram);
						}
					}
				}

			} else
			{
				if(storage_malloc(len + STR_MIN_SIZE))
				{
					storage.ram->len = 0;
					storage.ram->refs = 1;
				}
				else
				{
					ASSERT(storage.ram);
				}
			}

			//we are done with storage
			if(RAM_ADR(storage.adr))
			{
				// append now...
				char * ptr;
				ptr = strncpy(storage.ram->buf + storage.ram->len, s, l);
				*ptr = 0;
				storage.ram->len = ptr - storage.ram->buf;
			}

		}
	}
	return (*this);
}

/** Append char
 *
 * @param ch
 * @return
 */
CSTRING& CSTRING::append(char ch)
{
	unsigned int len=0;
	str_storage *	tmp;

	if(ch)
	{
		if(storage.adr)
		{
			if(ROM_ADR(storage.adr))
			{
				len = strlen(storage.rom) +1;
				tmp = str_malloc1(len + STR_MIN_SIZE);

				if(tmp)
				{
					strcpy(tmp->buf, storage.rom);
					tmp->refs = 1;
				}
				else
				{
					ASSERT(tmp);
				}
				storage.ram = tmp;

			} else
			{
				len = storage.ram->len +1;

				if( storage.ram->refs > 1)
				{
					// we need to make a new storage

					tmp = str_malloc1(len + STR_MIN_SIZE);
					if(tmp)
					{
						strcpy(tmp->buf, storage.ram->buf);
						tmp->refs = 1;
					}
					else
					{
						ASSERT(tmp);
					}
					if(locked_dec_short(&storage.ram->refs) <= 0)
						str_free(storage.ram);	//oops someone released the storage
					storage.ram = tmp;

				} else
				{
					// we are the only owner of the storage
					if(len >= STR_SIZEOF(storage.ram))
					{
						//but it is too small -> realloc
						storage_realloc(len + STR_MIN_SIZE);
						ASSERT(storage.ram);
					}
				}
			}
		} else
		{
			if(storage_malloc(1 + STR_MIN_SIZE))
			{
				storage.ram->len = 0;
				storage.ram->refs = 1;
				len = 1;
			}
			else
			{
				ASSERT(storage.ram);
			}
		}

		//we are done with ptr
		if(RAM_ADR(storage.adr))
		{
			// append now...
			storage.ram->buf[len-1] = ch;
			storage.ram->buf[len] = 0;
			storage.ram->len = len;
		}
	}

	return (*this);
}

CSTRING& CSTRING::insert (char ch, unsigned int index)
{
	CSTRING tmp (c_str(), index);
	tmp.append (ch);
	tmp.append (substr(index, length() - index));
	return (*this = tmp);
}

CSTRING& CSTRING::insert (const char* s, unsigned int index)
{
	CSTRING tmp (c_str(), index);
	tmp.append (s);
	tmp.append (substr(index, length() - index));
	return (*this = tmp);
}


/**
 * Concatenate two c-strings
 * @param s1
 * @param s2
 * @return CSTRING object
 */
CSTRING operator+(const CSTRING& s1, const CSTRING& s2 )
{
	unsigned int len1;
	unsigned int len2;

	len1 = s1.length();
	len2 = len1+ s2.length();

	CSTRING s(len2);

	if(s.storage.adr)
	{
		s.storage.ram->len = len2;
		strcpy(s.storage.ram->buf, s1.c_str());
		strcpy(s.storage.ram->buf + len1, s2.c_str());
	}
	return (s);
}

/**
 * Concatenates char* and CSTRING
 * @param s
 * @param s2
 * @return CSTRING object
 */
CSTRING operator+(const char* s, const CSTRING& s2 )
{
	unsigned int len1;
	unsigned int len2;

	len1 = strlen(s);
	len2 = len1+ s2.length();

	CSTRING res(len2);

	if(res.storage.adr)
	{
		res.storage.ram->len = len2;
		strcpy(res.storage.ram->buf, s);
		strcpy(res.storage.ram->buf + len1, s2.c_str());
	}
	return (res);
}

/**
 * Concatenates char and CSTRING
 * @param c
 * @param s2
 * @return CSTRING object
 */
CSTRING operator+( char c, const CSTRING& s2 )
{
	unsigned int len1;
	unsigned int len2;

	len1 = c?1:0;
	len2 = len1 + s2.length();

	CSTRING res(len2);

	if(res.storage.adr)
	{
		res.storage.ram->len = len2;
		res.storage.ram->buf[0] = c;
		strcpy(res.storage.ram->buf + len1, s2.c_str());
	}
	return (res);
}

/**
 * Concatenates CSTRING and char*
 * @param s1
 * @param s
 * @return CSTRING object
 */
CSTRING operator+( const CSTRING& s1, const char* s )
{
	unsigned int len1;
	unsigned int len2;

	len1 = s1.length();
	len2 = len1 + strlen(s);

	CSTRING res(len2);

	if(res.storage.adr)
	{
		res.storage.ram->len = len2;
		strcpy(res.storage.ram->buf, s1.c_str());
		strcpy(res.storage.ram->buf+ len1, s);
	}
	return (res);
}

/**
 * Concatenates CSTRING and char
 * @param s1
 * @param c
 * @return CSTRING object
 */
CSTRING operator+( const CSTRING& s1, char c )
{
	unsigned int len1;
	unsigned int len2;

	len1 = s1.length();
	len2 = len1 + (c?1:0);

	CSTRING res(len2);

	if(res.storage.adr)
	{
		res.storage.ram->len = len2;
		strcpy(res.storage.ram->buf, s1.c_str());
		if(c)
		{
			res.storage.ram->buf[len2--] = 0;
			res.storage.ram->buf[len2] = c;
		}
	}
	return (res);
}

/**
 * Compare with CSTRING
 * @param str
 * @return
 */
int CSTRING::compare( const CSTRING& str ) const
{
	const char* s1;
	const char* s2;

	s1 = RAM_ADR(storage.adr)?storage.ram->buf: storage.rom;
	s2 = RAM_ADR(str.storage.adr)?str.storage.ram->buf: str.storage.rom;

	return (strcmp(s1, s2));
}

/** Compare with string
 *
 * @param str
 * @return
 */
int CSTRING::compare( const char* str ) const
{
	return (strcmp(RAM_ADR(storage.adr)?storage.ram->buf: storage.rom, str));
}

/** Compare with substring
 *
 * @param str
 * @param len
 * @return
 */
int CSTRING::compare( const char* str, unsigned int len ) const
{
	return (strncmp(RAM_ADR(storage.adr)?storage.ram->buf: storage.rom, str, len));
}

/** Check if this starts with str
 *
 * @param str
 * @return
 */
int CSTRING::start_with( const char* str ) const
{
	unsigned int len;

	len = strlen(str);
	if(length() >= len)
	{
		if(!strncmp(c_str(), str, len))
			return len;
	}
	return 0;
}

/** Check if this starts with str (case not sensitive)
 *
 * @param str
 * @return
 */
int CSTRING::start_casewith( const char* str ) const
{
	unsigned int len;

	len = strlen(str);
	if(length() >= len)
	{
		if(!strncasecmp(c_str(), str, len))
			return len;
	}
	return 0;
}

/**
 * Find char
 * @param pos
 * @param val
 * @return
 */
bool CSTRING::find(unsigned int& pos, char val) const
{
	const char* ptr;

	pos =0;
	ptr = c_str();
	if(ptr)
	{
		while(*ptr)
		{
			if(*ptr == val)
				return (true);
			ptr++;
			pos++;
		}

	}
	return (false);
}

/** Get data pinter
 * returns a non-modifiable standard C character array version of the string
 *
 * @return
 */
const char* CSTRING::c_str() const
{
	/*
	 * 	NOTE: NULL can be returned!
	 */
	return (IS_NOT_NULL(this)?(RAM_ADR(storage.adr)?storage.ram->buf: storage.rom): "");
}

/**
 * Capacity
 * @return	the number of characters that the string can hold
 */
unsigned int CSTRING::capacity() const
{
	/*
	 *	NOTE: Capacity can be 0, even when the string is NOT empty! (e.g. it is shared)
	 */

	if(RAM_ADR(storage.adr))
		if(storage.ram->refs == 1)
			return (STR_SIZEOF(storage.ram)-1);

	return (0);
}

/** removes all characters from the string
 *
 */
void CSTRING::clear()
{
	if(RAM_ADR(storage.adr))
	{
		if( (storage.ram->refs <= 1) || !locked_dec_short(&storage.ram->refs)  )
		{
			storage.ram->refs = 1;
			storage.ram->len = 0;
			storage.ram->buf[0] = 0;
			return;
		}
	}
	storage.adr = NULL;
}

/**
 * copies characters from a string into an array
 * @param dst
 * @param num
 * @param index
 * @return
 */
unsigned int CSTRING::copy( char* dst, unsigned int num, unsigned int index ) const
{
	unsigned int res=0;

	if(dst)
	{
		if(index < length())
		{
			const char* src = c_str() +index;

			while( num-- && (dst[res] = src[res]) )
				res++;
		} else
			*dst = 0;
	}

	return (res);
}

/** Empty test
 *
 * @return true if the string has no characters
 */
bool CSTRING::empty() const
{
	if(storage.adr)
	{
		if(RAM_ADR(storage.adr))
			return (storage.ram->len == 0);
		else
			return (!storage.rom[0]);

	}
	return (true);
}

/**
 * sets the minimum capacity of the string
 * @param size
 * @return string capacity
 */
unsigned int CSTRING::reserve( unsigned int size)
{
	str_storage *	tmp;

	if(RAM_ADR(storage.adr))
	{
		if( storage.ram->refs > 1)
		{
			// we need to make a new storage

			if(size < storage.ram->len)
				size = storage.ram->len;
			tmp = str_malloc1(size + STR_MIN_SIZE);
			if(tmp)
			{
				memcpy(&tmp->len, &storage.ram->len, storage.ram->len + 3);
//				tmp->len = storage.ram->len;
				tmp->refs = 1;
			}
			else
			{
				ASSERT(tmp);
			}
			if(locked_dec_short(&storage.ram->refs) <= 0)
				str_free(storage.ram);	//oops someone released the storage
			storage.ram = tmp;

		} else
		{
			// we are the only owner of the storage
			if(size >= STR_SIZEOF(storage.ram ))
			{
				//but it is too small -> realloc
				storage_realloc(size + STR_MIN_SIZE);
				ASSERT(storage.ram);
			}
		}

	} else
	{
		if(storage.adr)
		{
			unsigned int current_len;

			current_len = strlen(storage.rom);
			if(size < current_len)
				size = current_len;

			tmp = str_malloc1(size + STR_MIN_SIZE);
			if(tmp)
			{
				strcpy(tmp->buf, storage.rom);
				tmp->len = current_len;
				tmp->refs = 1;
			}
			else
			{
				ASSERT(tmp);
			}
			storage.ram = tmp;
		} else
		{
			if( storage_malloc(size + STR_MIN_SIZE) )
			{
				storage.ram->refs = 1;
				storage.ram->len = 0;
				storage.ram->buf[0] = 0;
			}
			else
			{
				ASSERT(storage.ram);
			}
		}
	}

	if(storage.ram)
		return (STR_SIZEOF(storage.ram)-1);
	return (0);
}

/** swap the contents of this string with another
 *
 * @param from
 */
void CSTRING::swap( CSTRING& from )
{
	unsigned int	tmp;

	tmp = storage.adr;
	storage.adr = from.storage.adr;
	from.storage.adr = tmp;
}


/**
 * returns a certain substring
 * @param index
 * @param size
 * @return
 */
CSTRING CSTRING::substr( unsigned int index, unsigned int size ) const
{
	unsigned int len;

	len = length();
	if(index < len)
	{
		len -= index;		//available
		if(len > size)
			len = size;		//requested
	} else
		len =0;

	if(len)
	{
		CSTRING res(c_str() +index, len);
		return (res);

	} else
	{
		CSTRING res;
		return (res);
	}


}

/**
 * Test for substring
 * @param sub
 * @param pos
 * @return true if begins with sub
 */
unsigned int CSTRING::contains(CSTRING& sub, unsigned int pos) const
{
	unsigned int len;

	len = sub.length();
	if( pos+len <= length())
		if( !memcmp(c_str()+pos, sub.c_str(), len))
			return (len);
	return (0);
}

/** Length()
 *
 * @return the actual string length
 */
unsigned int CSTRING::length() const
{
	if(RAM_ADR(storage.adr))
		return (storage.ram->len);
	return (strlen(storage.rom));
}

/**
 * Make the storage writable
 * @note Does nothing if the string is in ROM or NULL
 * @return
 */
str_storage * CSTRING::get_storage()
{
	if( RAM_ADR(storage.adr) )
	{
		if( storage.ram->refs > 1)
		{
			str_storage * tmp;

			tmp = str_malloc1(storage.ram->len +12+ STR_MIN_SIZE);
			if(tmp)
			{
				strcpy(tmp->buf, storage.ram->buf);
				tmp->len = storage.ram->len;
				tmp->refs = 1;
			}
			else
			{
				ASSERT(tmp);
			}
			if(locked_dec_short(&storage.ram->refs) <= 0)
				str_free(storage.ram);	//oops someone released the storage
			storage.ram = tmp;

		}
		return (storage.ram);
	}
	return (NULL);
}

/**
 * Erase substring
 * @bug wont work for ROM...
 * @param index
 * @param count
 * @return
 */
CSTRING& CSTRING::erase(unsigned int index, unsigned int count)
{
	str_storage * tmp;
	if(count && length() > index)
	{
		tmp = get_storage();
		if(!tmp)
		{
			CSTRING str(c_str() ,index);

			if(count < length() - index)
			{
				str.append(c_str() + index + count);
			}
			*this = str;
		}
		else
		{
			if(index < tmp->len)
			{
				if(tmp->len > count)
				{
					tmp->len -= count;

					while(index < tmp->len)
					{
						tmp->buf[index] = tmp->buf[index+count];
						index++;
					}
				}

				//cut rightmost
				tmp->len = index;
				tmp->buf[index] = 0;
			}
		}
	}

	return (*this);
}

void CSTRING::trim_right()
{
	unsigned int pos;

	pos = length();
	while(pos--)
	{
		if(RAM_ADR(storage.adr))
		{
			if(storage.ram->buf[pos] != ' ')
				break;
		} else
		{
			if(!storage.adr)
				break;
			if(storage.rom[pos] != ' ')
				break;
		}
		erase(pos, 1);
	}
}
/** Convert to integer
 *
 * @return
 */
unsigned int CSTRING::atoi() const
{
	unsigned int res=0;
	const char *ptr = RAM_ADR(storage.adr)?storage.ram->buf: storage.rom;

	if(ptr)
	{
		tmos_sscanf(ptr, "%d", &res);
	}
	return (res);
}

long long CSTRING::atoll() const
{
	long long res=0;
	const char *ptr = RAM_ADR(storage.adr)?storage.ram->buf: storage.rom;

	if(ptr)
	{
		tmos_sscanf(ptr, "%ld", &res);
	}
	return (res);
}

int CSTRING::itoa(int num)
{
	return format("%d", num);
}

/**
 * Search for word from list
 *
 * @param sl - list of words
 * @param dwRead - start position (incremented with the found word length)
 * @return index+1 of the word that matches Or zero if nothing matches
 */
unsigned int CSTRING::find_in_list(STR_LIST sl, unsigned int* dwRead) const
{
	const char * buf;
	buf = c_str();
	if(dwRead)
		buf += *dwRead;
	return (::find_in_list(buf, sl, dwRead));
}

size_t CSTRING::find_first_not_of(const char* s, size_t pos) const
{
	size_t sz = length();
	const char *ptr = RAM_ADR(storage.adr)?storage.ram->buf: storage.rom;

    while(pos < sz)
    {
		if(!strchr(s,ptr[pos]))
			return pos;
		++pos;
    }
    return -1u;
}

size_t CSTRING::find_first_of(const char* s, size_t pos) const
{
	size_t sz = length();
	const char *ptr = RAM_ADR(storage.adr)?storage.ram->buf: storage.rom;
    while(pos < sz)
    {
		if(strchr(s,ptr[pos]))
			return pos;
		++pos;
    }
    return -1u;
}

/**
 * [] operator
 * @param index
 * @return
 */
char& CSTRING::operator[]( unsigned int index )
{
	if(!RAM_ADR(storage.adr))
		reserve();
	if(RAM_ADR(storage.adr))
	{
		ASSERT(index <= storage.ram->len);
		return (storage.ram->buf[index]);
	}
	ASSERT(0);
	return (dummy_char);
}

/**
 * [] operator
 * @param index
 * @return
 */
const char& CSTRING::operator[]( unsigned int index ) const
{
	if(RAM_ADR(storage.adr))
	{
		ASSERT(index <= storage.ram->len);
		return (storage.ram->buf[index]);
	} else
	{
		if(storage.adr)
		{
			return (storage.rom[index]);
		}
	}
	ASSERT(storage.adr);
	return (dummy_char);
}

/**
 * Appends formatted data string
 *
 * @param fmt
 * @return
 */
int CSTRING::appendf(const char *fmt, ...)
{
	va_list args;
	int len1, len2;

	va_start(args, fmt);
	len1 = length();
	len2 = tmos_vprintf_len(fmt, args);
	if(reserve(len1 + len2 ))
	{
		len2 = tmos_vsprintf(storage.ram->buf + len1, fmt, args);
		storage.ram->len = len1 + len2;
	} else
	{
		len2 = 0;
	}
	va_end(args);

	return len2;
}

/**
 * Assigns formated data string
 *
 * @param fmt
 * @return
 */
int CSTRING::format(const char *fmt, ...)
{
	va_list args;
	int len;

	va_start(args, fmt);
	clear();
	len = tmos_vprintf_len(fmt, args);
	if(reserve(len))
	{
		len = tmos_vsprintf(storage.ram->buf, fmt, args);
		storage.ram->len = len;
	} else
	{
		len = 0;
	}
	va_end(args);

	return len;
}


/** @} ingroup lib_cstring  */
