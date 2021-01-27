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
protected:
	//-- members
	CSTRING buf; 			//!< buffer holding the cached content
	CSTRING next; 			//!< buffer holding queued content
	uint32_t cache_pos; 	//!< position to read from buf
	uint32_t cache_offset;

public:
	/** Default constructor	**/
	CCache() :
			buf(), next(), cache_pos(0), cache_offset(0)
	{
	}

	/** Construct from CSTRING **/
	CCache(const CSTRING& s) :
		buf(s), next(), cache_pos(0), cache_offset(0)
	{
	}

	/** Construct from char* 	**/
	CCache(const char *str) :
		buf(str), next(), cache_pos(0), cache_offset(0)
	{
	}

	virtual ~CCache(){};

	void reset()
	{
		buf.clear();
		next.clear();
		cache_pos =0;
		cache_offset =0;
	}

	uint32_t get_cache_pos()
	{
		return cache_offset + cache_pos;
	}
	void set_cache_pos(uint32_t pos);

	char getc(); // get 1 char
	char topc();
	char getc_rev(); // get 1 char in reverse
	RES_CODE getc(char& c);
	RES_CODE get_pc(char&c);
	RES_CODE skip_ws();
	RES_CODE skip_char(char c);
	void skip_xml_comments();
	bool ungetc();
	RES_CODE readline(CSTRING& var);
	RES_CODE readline(CSTRING& str, unsigned int time);
	RES_CODE buffer(CSTRING& var);
	RES_CODE get_name(CSTRING& var);
	RES_CODE repeat_from(CSTRING& str, uint32_t from);
	const char* get_cptr(uint32_t len);

	RES_CODE match(const char* str);
	RES_CODE match_name(const char* str);
	RES_CODE matchc(char c);
	bool match_uint();
	bool match_ulong();
	bool match_long();
	bool match_uamount();
	bool match_amount();

protected:
	virtual RES_CODE load_next();

};

/** Cached handle **/
struct CCachedHandle: CCache
{
	HANDLE hnd;		//!< handle to be cached
	unsigned int size;  //!< cache line size

	/// Constructor
	CCachedHandle(HANDLE h, unsigned int sz) :
		CCache(), hnd(h), size(sz)
	{
		buf.reserve(sz);
	}

protected:
	RES_CODE load_next() override;

};

#endif /* CACHE_H_ */
