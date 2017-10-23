/*
 * ecdh.h
 *
 *  Created on: Aug 4, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_ECDH_H_
#define SERVICES_SECURITY_CRYPTO_ECDH_H_

#include <tmos.h>
#include <crypto.h>
#include <ec.h>

/// brief ECDH context
struct EcdhContext
{
   EcDomainParameters params; //!< EC domain parameters
   Mpi da;                    //!< One's own private key
   EcPoint qa;                //!< One's own public key
   EcPoint qb;                //!< Peer's public key


   RES_CODE tlsReadEcPoint(const uint8_t* data, size_t size, size_t* length);
   RES_CODE tlsWriteEcPoint(uint8_t *data, size_t *length) const;
   RES_CODE ecdhCheckPublicKey();
   RES_CODE ecdhGenerateKeyPair(prng_algo_t* prngAlgo);
   RES_CODE ecdhComputeSharedSecret(uint8_t* output, size_t outputSize, size_t* outputLength);

};


#endif /* SERVICES_SECURITY_CRYPTO_ECDH_H_ */
