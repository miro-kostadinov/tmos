/**************************************************************************//**
 * @ingroup	 libs
 * @defgroup lib_cstring CSTRING
 * @{
 * @file     str_list.h
 * @brief    CSTRING header
 * @version  V3.00
 * @date     17. May 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/
#ifndef STR_LIST_H_
#define STR_LIST_H_

/**
 * @brief String list
 *	The string list is a list of non-empty strings terminated by empty string.
 *
 *	Examples:
 *	 -	0, 0  - empty list ("")
 *	 -	'h', 'e', 'l', 'l', 'o', 0, 0 - list with one element ("hello", "")
 */
typedef char * STR_LIST;


/**
 * Macro to create STR_LIST item
 *
 * Examples:
 * @code
 * 	STR_LIST my_list = SZ(first) SZ(second);
 *  //This is equivalent to:
 * const char * my_list = "first\0second\0";
 * @endcode
 */
#define SZ(item) # item "\0"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 					 CString
//	or something alike...
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
 *
 *
 *
 */

struct str_storage
{
	volatile short refs;	/**< refference counter	*/
	unsigned short len;		/**< string length	*/
	char	buf[];			/**< string data	*/
};

/**
 * Internal macro
 * returns the allocated memory size for str_storage
 */
#define STR_SIZEOF(x) 	(dyn_sizeof(x) - sizeof(str_storage))

/**
 * Internal macro
 * it is true for pointers in the RAM
 */
#define RAM_ADR(x)   (((unsigned int)(x)) >> 29)

/**
 * Internal macro
 * it is true for pointers in the ROM
 */
#define ROM_ADR(x) (!(((unsigned int)(x)) >> 29))

/** CSTRING class
*
* CSTRING has only one data member, so it is very safe to pass it by VALUE
*
* NEVER allocate CSTRING directly, because this may lead to this = NULL and this
* is not checked. Use CSTRING as local variable or part of other structures.
*
* The actual data is str_storage, which is subject to memory allocation. If the
* allocation fails, str_storagy ptr will be NULL as for empty string. This will
* not cause crashes, only the string will stay empty.
* A reference count is supported, so it is really fast to copy a cstring and
* does not waste memory.
*/
class CSTRING
{
public:

	union
	{
		str_storage *	ram;
		const char *	rom;
		unsigned int	adr;
	} storage;		/**< pointer to the data	*/

	static char dummy_char; /**< dummy char returned as reference for out of bounds	*/

	//Constructors
	CSTRING(const CSTRING& cstr);
	CSTRING(const char *str);
	CSTRING(const char *str, unsigned int len);
	CSTRING(unsigned int size);
	CSTRING();
	~CSTRING();

	//Assign
	CSTRING& assign( const CSTRING& s );
	CSTRING& assign( const char* s );
	CSTRING& assign( const char* s, unsigned int len );
	CSTRING& assign( char ch );
	/**
	 * Assign CSTRING operator
	 * @param s
	 * @return
	 */
	CSTRING& operator=( const CSTRING& s ) 	{ return this->assign(s); }
	/**
	 * Assign string operator
	 * @param s
	 * @return
	 */
	CSTRING& operator=( const char* s ) 	{ return this->assign(s); }
	/** Assign char operator
	 *
	 * @param c
	 * @return
	 */
	CSTRING& operator=( char c )			{ return this->assign(c); }


	//Append
	CSTRING& append(const CSTRING& s);
	CSTRING& append(const char* s);
	CSTRING& append(const char* s, unsigned int l);
	CSTRING& append(char ch);
	/**
	 * Append CSTRING operator
	 * @param s
	 * @return
	 */
	CSTRING& operator+=(const CSTRING& s)	{ return this->append(s); }
	/**
	 * Append string operator
	 * @param s
	 * @return
	 */
	CSTRING& operator+=(const char* s)		{ return this->append(s); }
	/** Append char operator
	 *
	 * @param c
	 * @return
	 */
	CSTRING& operator+=(const char  c)		{ return this->append(c); }


	//Compare
    int compare( const CSTRING& str ) const;
    int compare( const char* str ) const;
    int compare( const char* str, unsigned int len ) const;
    bool find(unsigned int& pos, char val) const;

    /**
     * [] operator
     * @param index
     * @return
     */
	char& operator[]( unsigned int index )
	{
		if(RAM_ADR(storage.adr))
		{
			return storage.ram->buf[index];
		}
		return dummy_char;
	}

	/**
	 * [] operator
	 * @param index
	 * @return
	 */
	const char& operator[]( unsigned int index ) const
	{
		if(RAM_ADR(storage.adr))
		{
			return storage.ram->buf[index];
		} else
		{
			if(storage.adr)
			{
				return storage.rom[index];
			}
		}
		return dummy_char;
	}


	const char* c_str() const;
	unsigned int capacity() const;
	void clear();
	unsigned int copy( char* dst, unsigned int num, unsigned int index = 0 ) const;
	bool empty() const;
	unsigned int reserve( unsigned int size = 0 );
	void swap( CSTRING& from );
	CSTRING substr( unsigned int index = 0, unsigned int size = -1 ) const;
	unsigned int contains(CSTRING& sub, unsigned int pos=0) const;
	unsigned int length() const;
	str_storage * get_storage();
	CSTRING& erase(unsigned int index, unsigned int count);

	unsigned int atoi();
	unsigned int find_in_list(STR_LIST sl, unsigned int* dwRead) const;
};

CSTRING operator+(const CSTRING& s1, const CSTRING& s2 );
CSTRING operator+(const char* s, const CSTRING& s2 );
CSTRING operator+( char c, const CSTRING& s2 );
CSTRING operator+( const CSTRING& s1, const char* s );
CSTRING operator+( const CSTRING& s1, char c );

/** CSTRING == CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator==(const CSTRING& s1, const CSTRING& s2) {
	return s1.compare(s2) == 0;
}

/** CSTRING == char*
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator==(const CSTRING& s1, const char* s2) {
	return s1.compare(s2) == 0;
}

/** char* == CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator==(const char* s1, const CSTRING& s2) {
	return s2.compare(s1) == 0;
}

/** CSTRING != CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator!=(const CSTRING& s1, const CSTRING& s2) {
	return s1.compare(s2) != 0;
}

/** CSTRING != char*
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator!=(const CSTRING& s1, const char* s2) {
	return s1.compare(s2) != 0;
}

/** char* != CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator!=(const char* s1, const CSTRING& s2) {
	return s2.compare(s1) != 0;
}

/** CSTRING < CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator<(const CSTRING& s1, const CSTRING& s2) {
	return s1.compare(s2) < 0;
}

/** CSTRING < char*
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator<(const CSTRING& s1, const char* s2) {
	return s1.compare(s2) < 0;
}

/** char* < CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator<(const char* s1, const CSTRING& s2) {
	return s2.compare(s1) >= 0;
}

/** CSTRING > CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator>(const CSTRING& s1, const CSTRING& s2) {
	return s1.compare(s2) > 0;
}

/** CSTRING > char*
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator>(const CSTRING& s1, const char* s2) {
	return s1.compare(s2) > 0;
}

/** char* > CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator>(const char* s1, const CSTRING& s2) {
	return s2.compare(s1) <= 0;
}

/** CSTRING <= CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator<=(const CSTRING& s1, const CSTRING& s2) {
	return s1.compare(s2) <= 0;
}

/** CSTRING <= char*
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator<=(const CSTRING& s1, const char* s2) {
	return s1.compare(s2) <= 0;
}

/** char* <= CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator<=(const char* s1, const CSTRING& s2) {
	return s2.compare(s1) > 0;
}

/** CSTRING >= CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator>=(const CSTRING& s1, const CSTRING& s2) {
	return s1.compare(s2) >= 0;
}

/** CSTRING >= char*
 *
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator>=(const CSTRING& s1, const char* s2) {
	return s1.compare(s2) >= 0;
}

/** char* >= CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator>=(const char* s1, const CSTRING& s2) {
	return s2.compare(s1) < 0;
}

/** CSTRING format (written in assembly)
 *
 */

extern "C" str_storage * cstring_sprintf( str_storage *buffer, const char *format, ... );

/** Format - using sprintf style
 *
 * @param cstr
 * @param fmt
 * @param args
 * @return
 *  No capacity is required. The function allocates/reallocates the storage
 *
 *  @note format() works as append function. Call clear() for assign behavior
 *  Examples:
 *  @code
 *	CSTRING s;						//s = ""
 *
 *	cstr_format(s, "%d", 2);  		//s = "2"
 *  cstr_format(s, " + %d", 3);		//s = "2 + 3"
 *  @endcode
 */

#define cstr_format(cstr, fmt, args...) cstr.storage.ram = cstring_sprintf(cstr.get_storage(), fmt, ## args)


#endif /* STR_LIST_H_ */
/** @} defgroup lib_cstring  */
