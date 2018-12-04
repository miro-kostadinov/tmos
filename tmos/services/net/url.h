/*
 * url.h
 *
 *  Created on: 2009-2-24
 *      Author: Miroslav Kostadinov
 */

#ifndef URL_H_
#define URL_H_

#include <tmos.h>
#include <csocket.h>

/* Default port definitions */
#define DEFAULT_HTTP_PORT 		80
#define DEFAULT_FTP_PORT 		21
#define DEFAULT_HTTPS_PORT 		443

/* Specifies how, or whether, user auth information should be included
 * in URLs regenerated from URL parse structures. */
#define URL_FLAG_AUTH_MODE_SHOW				0x00
#define URL_FLAG_AUTH_MODE_HIDE				0x01
#define URL_FLAG_AUTH_MODE_HIDE_PASSWD		0x02
#define URL_FLAG_AUTH_MODE_MASK				0x03

/* Note: the ordering here is related to the order of elements in
   `supported_schemes' in url.c.  */
#define URL_FLAG_SCHEME_FILE				0x08
#define URL_FLAG_SCHEME_FTP					0x10
#define URL_FLAG_SCHEME_HTTP				0x20
#define URL_FLAG_SCHEME_HTTPS				0x30
#define URL_FLAG_SCHEME_INVALID				0x80
#define URL_FLAG_SCHEME_MASK				0xF8

// Support for "~route/url"
#ifndef URL_CUSTOM_ROUTING
#define URL_CUSTOM_ROUTING	0
#endif

class CURL
{
	const char * url_get_userinfo (const char *url);
public:
	CSTRING			host;			// Extracted hostname
	CSTRING			path;
	CSTRING			params;
	CSTRING			query;
	CSTRING			fragment;
	CSTRING 		user;
	CSTRING			password;

	unsigned short	url_flags;		//URL_FLAG_XXXX
	unsigned short	port;			// Port number
	CURL() :
		host(), path(), params(), query(), fragment(), user(), password(),
				url_flags(0), port(0)
	{
	}
	CURL& operator= (const CURL& url);
	NET_CODE url_parse(const char* url, bool path_only=false);
	NET_CODE url_resolve(const CURL& old_link);
	void url_print(CSTRING& str);
};



class CEncode
{
public:
static CSTRING encode_utf_8(const char *src, const char *separator);
static CSTRING encode_escapes(const char *src);
static CSTRING uencode_quotes(const char *src);
static CSTRING wap_encode(char conversion, CSTRING & src);

static CSTRING encode_utf_8(CSTRING & src, const char *separator){return encode_utf_8(src.c_str(), separator);};
static CSTRING encode_escapes(CSTRING & src){return encode_escapes(src.c_str());};
static CSTRING uencode_quotes(CSTRING & src){return uencode_quotes(src.c_str());};
};

struct iface_map_t
{
	const char* 		iface_name;
	const sock_mode_t* 	iface_mode;
};

struct mapped_sock_mode_t
{
	CSTRING 			mapped_url;
	const sock_mode_t*	mapped_mode;
	redir_sock_mode_t	mapped_redir;

	mapped_sock_mode_t(): mapped_mode(nullptr) {};

	RES_CODE resolve_mode(const iface_map_t* iface_map);
};
#endif /* URL_H_ */
