/*
 * mpi.h
 *
 *  Created on: Jul 14, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_MPI_H_
#define SERVICES_SECURITY_CRYPTO_MPI_H_

#include <tmos.h>
#include <crypto.h>

//Size of the sub data type
#define MPI_INT_SIZE sizeof(uint32_t)

#define MPI_MAX_SIZE  2048

//Error code checking
#define MPI_CHECK(f) if((res = f) != RES_OK) goto end

enum MpiFormat
{
   MPI_FORMAT_LITTLE_ENDIAN = 0,
   MPI_FORMAT_BIG_ENDIAN    = 1
};


/// Arbitrary precision integer

struct Mpi
{
   uint32_t* 	mpi_data;		//!< integer data
   int8_t 		mpi_sign;		//!< sign
   uint8_t		mpi_refs;		//!< refs: 0=none, 1=dynamic
   uint16_t 	mpi_size;

   Mpi(): mpi_data(nullptr), mpi_sign(1), mpi_refs(0), mpi_size(0) {}
   Mpi(int val, uint32_t* storage):
	   mpi_data(storage),
	   mpi_sign((val >= 0) ? 1 : -1),
	   mpi_refs(0),
	   mpi_size(1)
   {
		*storage = (val >= 0) ? val : -val;
   }
   ~Mpi()
   {
	   mpi_free();
   }

   void mpi_free();

   RES_CODE mpiGrow(uint32_t size);

   RES_CODE mpiReadRaw(const uint8_t* data, uint32_t length);
   RES_CODE mpiWriteRaw(uint8_t *data, uint32_t length) const;

   uint32_t mpiGetLength() const;
   uint32_t mpiGetByteLength() const;
   uint32_t mpiGetBitLength() const;

   RES_CODE mpiSetBitValue(uint32_t index, uint32_t value);
   uint32_t mpiGetBitValue(uint32_t index) const;

   int mpiComp(const Mpi* b) const;
   int mpiCompInt(int b) const;
   int mpiCompAbs(const Mpi* b) const;

   RES_CODE mpiCopy(const Mpi *a);
   RES_CODE mpiSetValue(int b);

   RES_CODE mpiRand(uint32_t length, prng_algo_t* prngAlgo);

   RES_CODE mpiAdd(const Mpi *a, const Mpi *b);
   RES_CODE mpiAddInt(const Mpi *a, int b);

   RES_CODE mpiSub(const Mpi *a, const Mpi *b);
   RES_CODE mpiSubInt(const Mpi *a, int b);

   RES_CODE mpiAddAbs(const Mpi *a, const Mpi *b);
   RES_CODE mpiSubAbs(const Mpi *a, const Mpi *b);

   RES_CODE mpiShiftLeft(uint32_t n);
   RES_CODE mpiShiftRight(uint32_t n);

   RES_CODE mpiMul(const Mpi *a, const Mpi *b);
   RES_CODE mpiMulInt(const Mpi *a, int b);

   RES_CODE mpiDiv(Mpi *r, const Mpi *a, const Mpi *b);
   RES_CODE mpiDivInt(Mpi *r, const Mpi *a, int b);

   RES_CODE mpiMod(const Mpi *a, const Mpi *p);
   RES_CODE mpiAddMod(const Mpi *a, const Mpi *b, const Mpi *p);
   RES_CODE mpiSubMod(const Mpi *a, const Mpi *b, const Mpi *p);
   RES_CODE mpiMulMod(const Mpi *a, const Mpi *b, const Mpi *p);
   RES_CODE mpiInvMod(const Mpi *a, const Mpi *p);
   RES_CODE mpiExpMod(const Mpi *a, const Mpi *e, const Mpi *p);

   RES_CODE mpiMontgomeryMul(const Mpi *a, const Mpi *b, uint32_t k, const Mpi *p, Mpi *t);
   RES_CODE mpiMontgomeryRed(const Mpi *a, uint32_t k, const Mpi *p, Mpi *t);

   RES_CODE tlsReadMpi(const uint8_t* data, size_t size, size_t* length);
   RES_CODE tlsWriteMpi(uint8_t* data, size_t* length) const;

   RES_CODE mpiImport(const uint8_t *data, size_t length, MpiFormat format);

   RES_CODE asn1WriteMpi(bool reverse, uint8_t* data, size_t* written) const;
   RES_CODE mpiCheckProbablePrime() const;
};

void mpiDump(const char* prepend, const Mpi* a);


#endif /* SERVICES_SECURITY_CRYPTO_MPI_H_ */
