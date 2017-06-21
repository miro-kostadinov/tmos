/*
 * url.c
 *
 *  Created on: 2009-2-24
 *      Author: Miroslav Kostadinov
 */

//	URL format:
//		scheme://network/path;parameters?query#fragment
//		scheme://userinfo@host:port/path;parameters?query#fragment

#include <tmos.h>
#include "url.h"

/* Table of "reserved" and "unsafe" characters.  Those terms are
   rfc1738-speak, as such largely obsoleted by rfc2396 and later
   specs, but the general idea remains.

   A reserved character is the one that you can't decode without
   changing the meaning of the URL.  For example, you can't decode
   "/foo/%2f/bar" into "/foo///bar" because the number and contents of
   path components is different.  Non-reserved characters can be
   changed, so "/foo/%78/bar" is safe to change to "/foo/x/bar".  The
   unsafe characters are loosely based on rfc1738, plus "$" and ",",
   as recommended by rfc2396, and minus "~", which is very frequently
   used (and sometimes unrecognized as %7E by broken servers).

   An unsafe character is the one that should be encoded when URLs are
   placed in foreign environments.  E.g. space and newline are unsafe
   in HTTP contexts because HTTP uses them as separator and line
   terminator, so they must be encoded to %20 and %0A respectively.
   "*" is unsafe in shell context, etc.

   We determine whether a character is unsafe through static table
   lookup.  This code assumes ASCII character set and 8-bit chars.  */

enum {
  /* rfc1738 reserved chars + "$" and ",".  */
  urlchr_reserved = 1,

  /* rfc1738 unsafe chars, plus non-printables.  */
  urlchr_unsafe   = 2
};

#define urlchr_test(c, mask) (urlchr_table[(unsigned char)(c)] & (mask))
#define URL_RESERVED_CHAR(c) urlchr_test(c, urlchr_reserved)
#define URL_UNSAFE_CHAR(c) urlchr_test(c, urlchr_unsafe)

/* Shorthands for the table: */
#define R  urlchr_reserved
#define U  urlchr_unsafe
#define RU R|U

static const unsigned char urlchr_table[256] =
{
  U,  U,  U,  U,   U,  U,  U,  U,   /* NUL SOH STX ETX  EOT ENQ ACK BEL */
  U,  U,  U,  U,   U,  U,  U,  U,   /* BS  HT  LF  VT   FF  CR  SO  SI  */
  U,  U,  U,  U,   U,  U,  U,  U,   /* DLE DC1 DC2 DC3  DC4 NAK SYN ETB */
  U,  U,  U,  U,   U,  U,  U,  U,   /* CAN EM  SUB ESC  FS  GS  RS  US  */
  U,  0,  U, RU,   R,  U,  R,  0,   /* SP  !   "   #    $   %   &   '   */
  0,  0,  0,  R,   R,  0,  0,  R,   /* (   )   *   +    ,   -   .   /   */
  0,  0,  0,  0,   0,  0,  0,  0,   /* 0   1   2   3    4   5   6   7   */
  0,  0, RU,  R,   U,  R,  U,  R,   /* 8   9   :   ;    <   =   >   ?   */
 RU,  0,  0,  0,   0,  0,  0,  0,   /* @   A   B   C    D   E   F   G   */
  0,  0,  0,  0,   0,  0,  0,  0,   /* H   I   J   K    L   M   N   O   */
  0,  0,  0,  0,   0,  0,  0,  0,   /* P   Q   R   S    T   U   V   W   */
  0,  0,  0, RU,   U, RU,  U,  0,   /* X   Y   Z   [    \   ]   ^   _   */
  U,  0,  0,  0,   0,  0,  0,  0,   /* `   a   b   c    d   e   f   g   */
  0,  0,  0,  0,   0,  0,  0,  0,   /* h   i   j   k    l   m   n   o   */
  0,  0,  0,  0,   0,  0,  0,  0,   /* p   q   r   s    t   u   v   w   */
  0,  0,  0,  U,   U,  U,  0,  U,   /* x   y   z   {    |   }   ~   DEL */

  U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,
  U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,
  U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,
  U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,

  U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,
  U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,
  U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,
  U, U, U, U,  U, U, U, U,  U, U, U, U,  U, U, U, U,
};
#undef R
#undef U
#undef RU

/* Decide whether the char at position P needs to be encoded.  (It is
   not enough to pass a single char *P because the function may need
   to inspect the surrounding context.)

   Return true if the char should be escaped as %XX, false otherwise.  */

static bool inline char_needs_escaping (const char *p)
{
  if (*p == '%')
    {
      if (IS_XDIGIT (p[1]) && IS_XDIGIT (p[2]))
        return false;
      else
        /* Garbled %.. sequence: encode `%'. */
        return true;
    }
  else if (URL_UNSAFE_CHAR (*p) && !URL_RESERVED_CHAR (*p))
    return true;
  else
    return false;
}

/* Translate a %-escaped (but possibly non-conformant) input string S
   into a %-escaped (and conformant) output string.  If no characters
   are encoded or decoded, return the same string S; otherwise, return
   a freshly allocated string with the new contents.

   After a URL has been run through this function, the protocols that
   use `%' as the quote character can use the resulting string as-is,
   while those that don't can use url_unescape to get to the intended
   data.  This function is stable: once the input is transformed,
   further transformations of the result yield the same output.

   Let's discuss why this function is needed.

   Imagine Wget is asked to retrieve `http://abc.xyz/abc def'.  Since
   a raw space character would mess up the HTTP request, it needs to
   be quoted, like this:

       GET /abc%20def HTTP/1.0

   It would appear that the unsafe chars need to be quoted, for
   example with url_escape.  But what if we're requested to download
   `abc%20def'?  url_escape transforms "%" to "%25", which would leave
   us with `abc%2520def'.  This is incorrect -- since %-escapes are
   part of URL syntax, "%20" is the correct way to denote a literal
   space on the Wget command line.  This leads to the conclusion that
   in that case Wget should not call url_escape, but leave the `%20'
   as is.  This is clearly contradictory, but it only gets worse.

   What if the requested URI is `abc%20 def'?  If we call url_escape,
   we end up with `/abc%2520%20def', which is almost certainly not
   intended.  If we don't call url_escape, we are left with the
   embedded space and cannot complete the request.  What the user
   meant was for Wget to request `/abc%20%20def', and this is where
   reencode_escapes kicks in.

   Wget used to solve this by first decoding %-quotes, and then
   encoding all the "unsafe" characters found in the resulting string.
   This was wrong because it didn't preserve certain URL special
   (reserved) characters.  For instance, URI containing "a%2B+b" (0x2b
   == '+') would get translated to "a%2B%2Bb" or "a++b" depending on
   whether we considered `+' reserved (it is).  One of these results
   is inevitable because by the second step we would lose information
   on whether the `+' was originally encoded or not.  Both results
   were wrong because in CGI parameters + means space, while %2B means
   literal plus.  reencode_escapes correctly translates the above to
   "a%2B+b", i.e. returns the original string.

   This function uses a modified version of the algorithm originally
   proposed by Anon Sricharoenchai:

   * Encode all "unsafe" characters, except those that are also
     "reserved", to %XX.  See urlchr_table for which characters are
     unsafe and reserved.

   * Encode the "%" characters not followed by two hex digits to
     "%25".

   * Pass through all other characters and %XX escapes as-is.  (Up to
     Wget 1.10 this decoded %XX escapes corresponding to "safe"
     characters, but that was obtrusive and broke some servers.)

   Anon's test case:

   "http://abc.xyz/%20%3F%%36%31%25aa% a?a=%61+a%2Ba&b=b%26c%3Dc"
   ->
   "http://abc.xyz/%20%3F%25%36%31%25aa%25%20a?a=%61+a%2Ba&b=b%26c%3Dc"

   Simpler test cases:

   "foo bar"         -> "foo%20bar"
   "foo%20bar"       -> "foo%20bar"
   "foo %20bar"      -> "foo%20%20bar"
   "foo%%20bar"      -> "foo%25%20bar"       (0x25 == '%')
   "foo%25%20bar"    -> "foo%25%20bar"
   "foo%2%20bar"     -> "foo%252%20bar"
   "foo+bar"         -> "foo+bar"            (plus is reserved!)
   "foo%2b+bar"      -> "foo%2b+bar"  */

static CSTRING reencode_escapes (const char* src)
{
	CSTRING dst;

	if(src)
	{
		while(*src)
		{
		    if (char_needs_escaping (src))
		      {
		        unsigned char c = *src++;
		        dst += '%';
		        dst += XNUM_TO_DIGIT (c >> 4);
		        dst += XNUM_TO_DIGIT (c & 0xf);
		      }
		    else
		    	dst += *src++;
		}
	}
	return dst;
}


CSTRING CEncode::encode_utf_8( const char *src, const char *separator )
{
	CSTRING dst;
	if(src)
	{
		while(*src)
		{
			unsigned char c = *src++;
			if( c >= 0xC0 )
			{
				int w = 0xD080;
				c -=0xB0;
				w |= (c & 0x3F);
				w |= (c&0xC0) << 2;
				dst += separator;
				dst += XNUM_TO_DIGIT (w >> 12);
				dst += XNUM_TO_DIGIT ((w >> 8)&0x0F);
				dst += separator;
				dst += (XNUM_TO_DIGIT (w >> 4)&0x0F);
				dst += XNUM_TO_DIGIT (w & 0x0F);
		        continue;
			}
		    if (URL_RESERVED_CHAR(c))
		      {
				dst += separator;
				dst += XNUM_TO_DIGIT (c >> 4);
				dst += XNUM_TO_DIGIT (c & 0x0F);
		      }
		    else
		    	dst += c;
		}
	}
	return dst;
}

CSTRING CEncode::encode_escapes(const char *src)
{
	CSTRING dst;
	if(src)
	{
		while(*src)
		{
	        unsigned char c = *src++;
		    if (URL_RESERVED_CHAR(c))
		      {
		        dst += '%';
		        dst += XNUM_TO_DIGIT (c >> 4);
		        dst += XNUM_TO_DIGIT (c & 0xf);
		      }
		    else
		    	dst += c;
		}
	}
	return dst;
}

static CSTRING uencode_escapes(const char *src)
{
	CSTRING dst;
	if(src)
	{
		while(*src)
		{
	        unsigned char c = *src++;
			if (c == '%')
			{
		          // Do nothing if '%' is not followed by two hex digits.
		          if (IS_XDIGIT(src[0]) && IS_XDIGIT(src[1]))
		          {
		              c = X2DIGITS_TO_NUM(src[0], src[1]);
		              src+=2;
		          }
			}
	    	dst += c;
		}
	}
	return dst;
}

static CSTRING uencode_escapes(const char *src, unsigned int len)
{
	CSTRING dst;
	if(src)
	{
		while(*src && len)
		{
	        unsigned char c = *src++;
	        len--;
			if ((c == '%') && (len>1))
			{
		          // Do nothing if '%' is not followed by two hex digits.
		          if (IS_XDIGIT(src[0]) && IS_XDIGIT(src[1]))
		          {
		              c = X2DIGITS_TO_NUM(src[0], src[1]);
		              src+=2;
		              len-=2;
		          }
			}
	    	dst += c;
		}
	}
	return dst;
}

static CSTRING encode_quotes(const char *src)
{
	CSTRING dst;
	if(src)
	{
    	dst += '\"';
		while(*src)
		{
	        unsigned char c = *src++;
	    	dst += c ;
		    if ( c == '\"')
		    	dst +=  c;
		}
    	dst += '\"';
	}
	return dst;
}

CSTRING CEncode::uencode_quotes(const char *src)
{
	CSTRING dst;
	if(src)
	{
		if(*src == '\"')
			src++;
		while(*src)
		{
	        unsigned char c = *src++;
		    if (c == '\"')
			    if (*src != '\"')
			    	break;
		    dst += c;
		}
	}
	return dst;
}

CSTRING CEncode::wap_encode(char conversion, CSTRING & src)
{
	switch(conversion)
	{
	case 'e':
		return encode_escapes(src.c_str());

	case 'q':
		return encode_quotes(src.c_str());

	case 'u':
		return uencode_escapes( src.c_str());

	case 's':
		return uencode_quotes( src.c_str());
	}
	return src;
}

//*----------------------------------------------------------------------------
//								url_scheme()
//								-----------
//	Parameter:
//
//	Return: scheme
//
//	Description:
// 		Parse a URL scheme.
//*----------------------------------------------------------------------------
static const char* url_skip_slashes(const char* url)
{
	if(url[0]=='/')
	{
		url++;
		if(url[0]=='/')
			url++;
	}
	return url;
}

static const char* url_scheme(const char* url, unsigned short * url_flags )
{
	int i=0;

	*url_flags=0;
	// search for ':'
	while(1)
	{
		switch(url[i])
		{
			case 0:
				return url;

			case ':':
				switch(i)
				{
					case 3:// ftp - not supported
						if(!strncasecmp (url, "ftp",3))
						{
							*url_flags = URL_FLAG_SCHEME_FTP;
							return url_skip_slashes(url+i+1);
						}
						break;

					case 4:
						if(!strncasecmp (url, "file",4))
						{
							*url_flags = URL_FLAG_SCHEME_FILE;
							return url_skip_slashes(url+i+1);
						}
						if(!strncasecmp (url, "http",4))
						{
							*url_flags = URL_FLAG_SCHEME_HTTP;
							return url_skip_slashes(url+i+1);
						}
						break;

					case 5:
						if(!strncasecmp (url, "https",5))
						{
							*url_flags = URL_FLAG_SCHEME_HTTPS;
							return url_skip_slashes(url+i+1);
						}
						break;

				}
				return url;

			default:
				i++;
				break;
		}
	}

	return url;
}

//*----------------------------------------------------------------------------
//								url_parse_fragment()
//								------------------
//	Description:
// 		host:port/path;parameters?query#fragment
//*----------------------------------------------------------------------------
static CSTRING url_parse_fragment(char *p)
{
	CSTRING s;
    p = strchr (p, '#');
    if(p)
    {
    	*p++ = 0;
    	s = p;
    }
	return s;
}

//*----------------------------------------------------------------------------
//								url_parse_query()
//								------------------
//	Description:
// 		host:port/path;parameters?query
//*----------------------------------------------------------------------------
static CSTRING url_parse_query(char *p)
{
	CSTRING s;
    p = strchr (p, '?');
    if(p)
    {
    	s = p;
    	*p = 0;
    }
	return s;
}

//*----------------------------------------------------------------------------
//								url_parse_parameters()
//								------------------
//	Description:
// 		host:port/path;parameters
//*----------------------------------------------------------------------------
static CSTRING url_parse_parameters(char *p)
{
	CSTRING s;
    p = strchr (p, ';');
    if(p)
    {
    	*p++ = 0;
    	s = p;
    }
	return s;
}

CURL& CURL::operator= (const CURL& url)
{
	host = url.host;
	port = url.port;
	path = url.path;
	fragment = url.fragment;
	params = url.params;
	query = url.query;
	url_flags = url.url_flags;
	user = url.user;
	password = url.password;
	return *this;
}

/* Skip the username and password, if present in the URL.  The
   function should *not* be called with the complete URL, but with the
   portion after the scheme.

   If no username and password are found, return URL.  */

const char* CURL::url_get_userinfo (const char *url)
{
	password.clear();
	user.clear();

	/* Look for '@' that comes before terminators, such as '/', '?', '#', ';' */
	const char *p = strpbrk(url, "@/?#;");
	if (!p || *p != '@')
	{
		return url;
	}

	if(url != p)
	{
		const char* pp = url;

		while( (pp != p) && (*pp != ':') )
		{
			pp++;
		}

		user = uencode_escapes(url, pp - url);
		if(++pp < p)
		{
			password = uencode_escapes(pp, p - pp);
		}
	}
	return p + 1;
}



//*----------------------------------------------------------------------------
//								url_parse()
//								-----------
//	Parameter:
//
//	Return:
//
//	Description:
// 		Parse a URL. URL_LINK is assumed to be cleared
//*----------------------------------------------------------------------------
NET_CODE CURL::url_parse(const char* url, bool path_only)
{
	CSTRING s;
	char* surl;

	if(!url || !url[0])
		return NET_ERR_URL_INVALID;

	host.clear();
#ifdef URL_CUSTOM_ROUTING
	if(url[0] == '/' && url[1] == '~')
	{
		surl = strchr(url+2, '/');
		if(surl++)
		{
			host.assign(url, surl-url);
			url = surl;
		}
	}
#endif

	//parse the scheme
	url = url_scheme(url, &url_flags);
	if((url_flags & URL_FLAG_SCHEME_MASK) > URL_FLAG_SCHEME_HTTP)
		return NET_ERR_URL_SCHEME;
	if(!url[0] && host.empty())
		return NET_ERR_URL_INVALID;

	// default port
	switch( url_flags & URL_FLAG_SCHEME_MASK)
	{
	case URL_FLAG_SCHEME_FTP:
		port = DEFAULT_FTP_PORT;
		break;

	case URL_FLAG_SCHEME_HTTP:
		port = DEFAULT_HTTP_PORT;
		break;

	case URL_FLAG_SCHEME_HTTPS:
		port = DEFAULT_HTTPS_PORT;
		break;

	default:
		port = 0;
		break;

	}

	path.clear();
	//re-encode
	s = reencode_escapes(url_get_userinfo(url));

	surl = (char*)s.c_str();
	fragment = url_parse_fragment(surl);
	query = url_parse_query(surl);
	params = url_parse_parameters(surl);
	if(*surl == '[')
	{
#ifdef ENABLE_IPV6
		surl = strchr(surl, ']');
		if(!p)
			return NET_ERR_URL_INVALID;
		/* Check if the IPv6 address is valid. */
		if (!is_valid_ipv6_address(s->buf+1, surl))
		return NET_ERR_URL_INVALID;

		/* Continue parsing after the closing ']'. */
		host += CSTRING(s->buf, surl - s->buf + 1);
		surl++;

		/* The closing bracket must be followed by a separator or by the null char.  */
		/* http://[::1]... */
		/*             ^   */
		if (!strchr (":/;?#", *surl))
		{
			/* Trailing garbage after []-delimited IPv6 address. */
			return NET_ERR_URL_INVALID;//PE_INVALID_HOST_NAME;
		}
#else
		return NET_ERR_URL_INVALID;//PE_IPV6_NOT_SUPPORTED;
#endif
	}

	if(s.length())
	{
		surl = strchr (s.c_str(), ':');
		if(surl)
		{
			*surl++ = 0;
			if(s[0])
				host += s.c_str();

			/* Allow empty port, as per rfc2396. */
			port = 0;
			while(IS_DIGIT(*surl))
			{
				port *= 10;
				port += *surl - '0';
				surl++;
			}
			if(!*surl)
				return NET_OK;

			/* http://host:12randomgarbage/blah */
			/*               ^                  */
			if(*surl != '/')
			{
				return NET_ERR_URL_INVALID;
			}
			if(surl[1])
				path = surl+1;
			return NET_OK;
		}

		if(s[0] == '.')
		{
			path = s.c_str();
		} else
			if(path_only || (s[0] == '/' && (url_flags & (URL_FLAG_SCHEME_FTP | URL_FLAG_SCHEME_HTTP))))
			{
				path = s.c_str();
			} else
			{
				surl = strchr (s.c_str()+1, '/');
				if(surl)
				{
					*surl = 0;
					if(surl[1])
						path = surl+1;
					if(s[0])
						host += s.c_str();
				} else
				{
					host += s.c_str();
				}
			}
	}
	return NET_OK;
}

static void strip_after_last_slash(CSTRING& s)
{
	char *last_slash;
	last_slash = strrchr(s.c_str(), '/');

	if (last_slash)
	{
		s.erase(last_slash - s.c_str()+1, s.length());
	}
	else
	{
		s.clear();
	}

}
//*----------------------------------------------------------------------------
//								url_resolve()
//								-----------
//	Parameter:
//
//	Return:
//
//	Description:  RFC 1808
//
//Step 1: The base URL is established according to the rules of
//        Section 3.  If the base URL is the empty string (unknown),
//        the embedded URL is interpreted as an absolute URL and
//        we are done.
//
//Step 2: Both the base and embedded URLs are parsed into their
//        component parts as described in Section 2.4.
//
//        a) If the embedded URL is entirely empty, it inherits the
//           entire base URL (i.e., is set equal to the base URL)
//           and we are done.
//
//
//Step 7: The resulting URL components, including any inherited from
//        the base URL, are recombined to give the absolute form of
//        the embedded URL.
//
//*----------------------------------------------------------------------------

NET_CODE CURL::url_resolve(const CURL & old_link)
{
	bool host_empty;

	//If the relative URL has a scheme, it's interpreted as a complete absolute URL by itself.
	//Step 2
	//        b) If the embedded URL starts with a scheme name, it is
	//           interpreted as an absolute URL and we are done.
	if(url_flags & URL_FLAG_SCHEME_MASK)
	{
		return NET_OK;
	}
	else
	{
	//        c) Otherwise, the embedded URL inherits the scheme of
	//           the base URL.
	//
#ifdef URL_CUSTOM_ROUTING
		// if "~/" do not inherit
		if(host[0] == '~')
		{
			host.erase(0, 1);
			if(host.empty())
				host += '/';
			return NET_OK;
		}

#endif
		url_flags |= old_link.url_flags & URL_FLAG_SCHEME_MASK;
		port = old_link.port;
	}
	host_empty = host.empty();
	if(!host_empty && host == "/")
		host_empty = true;
#ifdef URL_CUSTOM_ROUTING
	// copy interface
	if(old_link.host.start_with("/~") && !host.start_with("/~"))
	{
		const char* ptr;
		CSTRING s(old_link.host);

		ptr = s.c_str();
		ptr = strchr(ptr +2, '/');
		if(ptr)
		{
			s.erase(ptr - s.c_str()+1, -1u);
		} else
			s += '/';
		if(!host_empty)
		{
			if(host[0] == '/')
				s+= host.c_str()+1;
			else
				s += host;
		}
		host = s;
	}
#endif

	//Step 3: If the embedded URL's <net_loc> is non-empty, we skip to
	//        Step 7.  Otherwise, the embedded URL inherits the <net_loc>
	//        (if any) of the base URL.
	if(!host_empty)
	{
		if(url_flags & (URL_FLAG_SCHEME_FTP | URL_FLAG_SCHEME_HTTP))
			return NET_OK;

		if(host[0] == '/' )
			return NET_OK;

		if(old_link.host == "/")
			host =  "/" + host;
		else
		{
			if(path.empty())
				path = host;
			else
				path = host + "/" + path;
			host = old_link.host;
		}

	} else
	{
		host = old_link.host;
	}
	if(user.empty())
		user = old_link.user;

	if(password.empty())
		password = old_link.password;
	//Step 4: If the embedded URL path is preceded by a slash "/", the
	//        path is not relative and we skip to Step 7.

	if(!path.empty())
	{
		if(path[0] == '/')
			return NET_OK;
	} else
	{

		//Step 5: If the embedded URL path is empty (and not preceded by a
		//        slash), then the embedded URL inherits the base URL path,
		//        and
		path = old_link.path;

		//        a) if the embedded URL's <params> is non-empty, we skip to
		//           step 7; otherwise, it inherits the <params> of the base
		//           URL (if any) and
		if(!params.empty())
			return NET_OK;
		params = old_link.params;

		//        b) if the embedded URL's <query> is non-empty, we skip to
		//           step 7; otherwise, it inherits the <query> of the base
		//           URL (if any) and we skip to step 7.
		if(query.empty())
			query = old_link.query;
		return NET_OK;
	}

	//Step 6: The last segment of the base URL's path (anything
	//        following the rightmost slash "/", or the entire path if no
	//        slash is present) is removed and the embedded URL's path is
	//        appended in its place.
	CSTRING s;

	if(url_flags & (URL_FLAG_SCHEME_FTP | URL_FLAG_SCHEME_HTTP))
	{
		s = old_link.path;
		strip_after_last_slash(s);
	} else
	{
		s = host;
		if(!old_link.path.empty())
			s += "/" + old_link.path;
	}

	//	  The following operations are then applied, in order, to the new path:
	for (unsigned int i=0; i< path.length(); i++ )
	{
		if(path[i] == '.')
		{
			switch(path[i+1])
			{
			//        a) All occurrences of "./", where "." is a complete path
			//           segment, are removed.
			case '/':
				path.erase(i, 2);
				i--;
				break;

			//        b) If the path ends with "." as a complete path segment,
			//           that "." is removed.
			case 0:
				path.erase(i, 1);
				break;

			//        c) All occurrences of "<segment>/../", where <segment> is a
			//           complete path segment not equal to "..", are removed.
			//           Removal of these path segments is performed iteratively,
			//           removing the leftmost matching pattern on each iteration,
			//           until no matching pattern remains.
			//        d) If the path ends with "<segment>/..", where <segment> is a
			//           complete path segment not equal to "..", that
			//           "<segment>/.." is removed.
			case '.':
				if ((path[i+2] == '/' || path[i+2] == 0))
				{
					//s->buf[--s->len] = 0;
					s.erase(s.length() - 1, 1);
					strip_after_last_slash(s);
					s.erase(s.length() - 1, 1);
					path.erase(i, 3);
					i--;
				}
				break;
			}
		}
	}

	if(url_flags & (URL_FLAG_SCHEME_FTP | URL_FLAG_SCHEME_HTTP))
	{
		if (!s.empty())
		{
			s += path;
			if (s.empty())
				return NET_ERR_OUT_OF_MEMORY;
			path = s;
		}
	} else
	{
//		s.erase(s.length() - 1, 1);
//		if(s.length() < host.length())
			host = s;
	}

	return NET_OK;
}

void CURL::url_print(CSTRING& str)
{
	unsigned int i, host_pos=0;

#ifdef URL_CUSTOM_ROUTING
	if(host.start_with("/~"))
	{
		const char* ptr;

		ptr = strchr(host.c_str()+2, '/');
		if(ptr)
		{
			host_pos = ptr+1 - host.c_str();
			str.append(host.c_str(), host_pos);
		} else
		{
			str += host;
			str += '/';
			host_pos = host.length();
		}
	}
#endif

	switch(url_flags & URL_FLAG_SCHEME_MASK)
	{
	case URL_FLAG_SCHEME_FILE:
		str += "file://";
		break;

	case URL_FLAG_SCHEME_FTP:
		str += "ftp://";
		break;

	case URL_FLAG_SCHEME_HTTP:
		str += "http://";
		break;

	}

	if(!user.empty())
	{
		str += CEncode::encode_escapes(user.c_str());
		if(!password.empty())
		{
			str += ':';
			str += CEncode::encode_escapes(password.c_str());
		}
		str += '@';
	}
	str += host.c_str() + host_pos;

	switch(url_flags & URL_FLAG_SCHEME_MASK)
	{
	case URL_FLAG_SCHEME_FTP:
		i = DEFAULT_FTP_PORT;
		break;

	case URL_FLAG_SCHEME_HTTP:
		i = DEFAULT_HTTP_PORT;
		break;
	default:
		i = 0;
		break;
	}

	if(port != i)
	{
		str.appendf(":%u/", port);
	} else
	{
		if(!host.empty())
			if(!path.empty())
				str += '/';

	}
	str += path;

	if(!params.empty())
	{
		str += ';';
		str += params;
	}

	if(!query.empty())
	{
		str += '?';
		str += query;
	}
	if(!fragment.empty())
	{
		str += '#';
		str += fragment;
	}
}
