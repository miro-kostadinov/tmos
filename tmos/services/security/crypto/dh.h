/*
 * dh.h
 *
 *  Created on: Aug 3, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_DH_H_
#define SERVICES_SECURITY_CRYPTO_DH_H_

#include <tmos.h>
#include <crypto.h>
#include <mpi.h>

/// Diffie-Hellman parameters
struct DhParameters
{
	Mpi p; //!< Prime modulus
	Mpi g; //!< Generator

	RES_CODE dhCheckPublicKey(const Mpi* publicKey);
};

/// Diffie-Hellman context
struct DhContext
{
	DhParameters params; //!< Diffie-Hellman parameters
	Mpi xa;              //!< One's own private value
	Mpi ya;              //!< One's own public value
	Mpi yb;              //!< Peer's public value

	RES_CODE dhGenerateKeyPair(prng_algo_t* prngAlgo);
	RES_CODE dhComputeSharedSecret(uint8_t* output, size_t outputSize, size_t* outputLength);

};


#endif /* SERVICES_SECURITY_CRYPTO_DH_H_ */
