/*
 * tls_cache.h
 *
 *  Created on: Jul 13, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_TLS_TLS_CACHE_H_
#define SERVICES_SECURITY_TLS_TLS_CACHE_H_

#include <tmos.h>
#include <tls.h>

RES_CODE tls_remove_from_cache(tls_context_t* context);
RES_CODE tlsSaveToCache(tls_context_t* context);
RES_CODE tlsFindAndRestoreCache(tls_context_t* context, const uint8_t* sessionId,
		size_t sessionIdLen);

#endif /* SERVICES_SECURITY_TLS_TLS_CACHE_H_ */
