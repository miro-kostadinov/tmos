/*
 * oid.h
 *
 *  Created on: Jul 14, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_OID_H_
#define SERVICES_SECURITY_CRYPTO_OID_H_

#include <tmos.h>
#include <crypto.h>

#define OID_MORE_FLAG  0x80
#define OID_VALUE_MASK 0x7F

int32_t oidComp(const uint8_t* oid1, size_t oidLen1, const uint8_t* oid2, size_t oidLen2);


#endif /* SERVICES_SECURITY_CRYPTO_OID_H_ */
