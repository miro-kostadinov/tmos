/*
 * cache.h
 *
 *  Created on: 2010-8-4
 *      Author: Miroslav Kostadinov
 */

#ifndef CACHE_H_
#define CACHE_H_

#define CACHE_DEFAULT_SIZE	40

/** Cache base class
 *
 */
struct CCache
{
	//-- members
	CSTRING buf; 		//!< buffer holding the cached content
	CSTRING next; 		//!< buffer holding queued content
	const char* pos; 	//!< position to read from buf
	unsigned int size;  //!< cache line size

	/** Default constructor	**/
	CCache() :
			buf(), next(), pos(NULL), size(CACHE_DEFAULT_SIZE)
	{
	}
	;

	/** Construct from CSTRING **/
	CCache(const CSTRING& s) :
		buf(s), next(), pos(buf.c_str()), size(CACHE_DEFAULT_SIZE)
	{
	}
	;
	/** Construct from char* 	**/
	CCache(const char *str) :
		buf(str), next(), pos(buf.c_str()), size(CACHE_DEFAULT_SIZE)
	{
	}
	;
	/** Construct with custom buffer size **/
	CCache(unsigned int sz) :
#if USE_CSTRING
		buf(sz), next(), pos(0), size(sz)
#else
		buf(), next(), pos(0), size(sz)
#endif
	{
	}
	;
	virtual ~CCache(){};

	char getc(); // get 1 char
	RES_CODE getc(char& c);
	RES_CODE get_pc(char&c);
	RES_CODE skip_ws();
	RES_CODE skip_char(char c);
	bool ungetc();
	RES_CODE readline(CSTRING& var);
	RES_CODE readline(CSTRING& str, unsigned int time);
	RES_CODE buffer(CSTRING& var);
	RES_CODE get_name(CSTRING& var);

	RES_CODE match(const char* str);
	RES_CODE match_name(const char* str);

protected:
	virtual RES_CODE load();

};

/** Cached handle **/
struct CCachedHandle: CCache
{
	HANDLE hnd;		//!< handle to be cached
	/// Constructor
	CCachedHandle(HANDLE h, unsigned int sz) :
		CCache(sz), hnd(h)
	{
	}
	;

protected:
	RES_CODE load();

};

#endif /* CACHE_H_ */
