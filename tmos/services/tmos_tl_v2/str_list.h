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

#include <brd_cfg.h>
#include <trace.h>
#include <tmos_stdlib.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 					 CString
//	or something alike...
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/** storage for CSTRING **/
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
	CSTRING(const CSTRING& cstr);		// copy constructor
	CSTRING(CSTRING&& cstr)				// move constructor
	{
		if(RAM_ADR(storage.adr = cstr.storage.adr))
		{
			cstr.storage.adr = NULL;
		}
	}
	CSTRING(const char *str);
	CSTRING(const char *str, unsigned int len);
	CSTRING(unsigned int size);
	CSTRING();
	~CSTRING();
	// free allocated memory
	void free(void);
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
	CSTRING& operator=( const CSTRING& s ) 	{ return (this->assign(s)); }
	/**
	 * Assign string operator
	 * @param s
	 * @return
	 */
	CSTRING& operator=( const char* s ) 	{ return (this->assign(s)); }
	/** Assign char operator
	 *
	 * @param c
	 * @return
	 */
	CSTRING& operator=( char c )			{ return (this->assign(c)); }


	//Append
	CSTRING& append(const CSTRING& s);
	CSTRING& append(const char* s);
	CSTRING& append(const char* s, unsigned int l);
	CSTRING& append(char ch);
	CSTRING& insert(char ch, unsigned int index = 0);
	CSTRING& insert(const char* s, unsigned int index = 0);
	/**
	 * Append CSTRING operator
	 * @param s
	 * @return
	 */
	CSTRING& operator+=(const CSTRING& s)	{ return (this->append(s)); }
	/**
	 * Append string operator
	 * @param s
	 * @return
	 */
	CSTRING& operator+=(const char* s)		{ return (this->append(s)); }
	/** Append char operator
	 *
	 * @param c
	 * @return
	 */
	CSTRING& operator+=(const char  c)		{ return (this->append(c)); }


	//Compare
    int compare( const CSTRING& str ) const;
    int compare( const char* str ) const;
    int compare( const char* str, unsigned int len ) const;
    int start_with( const char* str ) const;
    int start_casewith( const char* str ) const;
    bool find(unsigned int& pos, char val) const;

	char& operator[]( unsigned int index );
	const char& operator[]( unsigned int index ) const;

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
	void trim_right();

	int atoi() const;
	long long atoll() const;
	int itoa(int num);
	unsigned int find_in_list(STR_LIST sl, unsigned int* dwRead) const;
	size_t find_first_not_of(const char* s, size_t pos = 0) const;
	size_t find_first_of(const char* s, size_t pos = 0) const;
	int format(const char *fmt, ...);
	int appendf(const char *fmt, ...);

	// Use with care (when this is in RAM, e.g. after reserve() )
	void m_set_size(unsigned int n)
	{
		storage.ram->buf[n] = 0;
		storage.ram->len = n;
	}

protected:
	void* storage_realloc(unsigned int size);
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
	return (s1.compare(s2) == 0);
}

/** CSTRING == char*
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator==(const CSTRING& s1, const char* s2) {
	return (s1.compare(s2) == 0);
}

/** char* == CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator==(const char* s1, const CSTRING& s2) {
	return (s2.compare(s1) == 0);
}

/** CSTRING != CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator!=(const CSTRING& s1, const CSTRING& s2) {
	return (s1.compare(s2) != 0);
}

/** CSTRING != char*
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator!=(const CSTRING& s1, const char* s2) {
	return (s1.compare(s2) != 0);
}

/** char* != CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator!=(const char* s1, const CSTRING& s2) {
	return (s2.compare(s1) != 0);
}

/** CSTRING < CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator<(const CSTRING& s1, const CSTRING& s2) {
	return (s1.compare(s2) < 0);
}

/** CSTRING < char*
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator<(const CSTRING& s1, const char* s2) {
	return (s1.compare(s2) < 0);
}

/** char* < CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator<(const char* s1, const CSTRING& s2) {
	return (s2.compare(s1) >= 0);
}

/** CSTRING > CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator>(const CSTRING& s1, const CSTRING& s2) {
	return (s1.compare(s2) > 0);
}

/** CSTRING > char*
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator>(const CSTRING& s1, const char* s2) {
	return (s1.compare(s2) > 0);
}

/** char* > CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator>(const char* s1, const CSTRING& s2) {
	return (s2.compare(s1) <= 0);
}

/** CSTRING <= CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator<=(const CSTRING& s1, const CSTRING& s2) {
	return (s1.compare(s2) <= 0);
}

/** CSTRING <= char*
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator<=(const CSTRING& s1, const char* s2) {
	return (s1.compare(s2) <= 0);
}

/** char* <= CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator<=(const char* s1, const CSTRING& s2) {
	return (s2.compare(s1) > 0);
}

/** CSTRING >= CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator>=(const CSTRING& s1, const CSTRING& s2) {
	return (s1.compare(s2) >= 0);
}

/** CSTRING >= char*
 *
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator>=(const CSTRING& s1, const char* s2) {
	return (s1.compare(s2) >= 0);
}

/** char* >= CSTRING
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool operator>=(const char* s1, const CSTRING& s2) {
	return (s2.compare(s1) < 0);
}


#endif /* STR_LIST_H_ */
/** @} defgroup lib_cstring  */
