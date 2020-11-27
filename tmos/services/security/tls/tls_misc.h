/*
 * tls_misc.h
 *
 *  Created on: Aug 4, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_TLS_TLS_MISC_H_
#define SERVICES_SECURITY_TLS_TLS_MISC_H_

#include <tmos.h>
#include <tls.h>
#include <x509.h>

RES_CODE tlsVerifyRsaSignature(const RsaPublicKey* key, const uint8_t* digest,
		const TlsDigitalSignature* signature);

RES_CODE tlsVerifyDsaSignature(DsaPublicKey* key, const uint8_t* digest,
		size_t digestLength, const TlsDigitalSignature* signature);

bool tlsIsCertificateAcceptable(const TlsCertDesc *cert,
		const uint8_t* certTypes, size_t numCertTypes,
		const tls_sign_algos_t* signHashAlgos,
		const tls_elliptic_curve_list_t* curveList,
		const TlsCertAuthorities* certAuthorities);

const EcCurveInfo* tlsGetCurveInfo(tls_ecnamedcurve_t namedCurve);

const hash_info_t* tlsGetHashAlgo(tls_hash_algo_t hash_id);

RES_CODE tlsGetCertificateType(const X509CertificateInfo *certInfo,
		TlsCertificateType *certType, tls_sign_algo_t *certSignAlgo,
		tls_hash_algo_t *certHashAlgo, tls_ecnamedcurve_t *namedCurve);


const tls_extension_t* tlsGetExtension(const uint8_t* data, size_t length, uint16_t type);

#endif /* SERVICES_SECURITY_TLS_TLS_MISC_H_ */
