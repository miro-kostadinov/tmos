/*
 * ec.cpp
 *
 *  Created on: Jul 14, 2017
 *     License: see LICENSE file
 */

#include <tmos.h>
#include <ec.h>
#include <asn1.h>
#include <x509.h>
#include <pem.h>

#if EC_SUPPORT

//EC Public Key OID (1.2.840.10045.2.1)
const uint8_t EC_PUBLIC_KEY_OID[7] = {0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01};


void EcPoint::ecp_free()
{
	x.mpi_free();
	y.mpi_free();
	z.mpi_free();
}

/**
 * @brief Copy EC point
 * @param[out] r Destination EC point
 * @param[in] s Source EC point
 * @return Error code
 **/
RES_CODE EcPoint::ecCopy(const EcPoint* s)
{
	RES_CODE res;

	//R and S are the same instance?
	if (this == s)
		return RES_OK;

	//Copy coordinates
	res = x.mpiCopy(&s->x);
	if(res == RES_OK)
	{
		res = y.mpiCopy(&s->y);
		if(res == RES_OK)
			res = z.mpiCopy(&s->z);
	}

	return res;
}

/**
 * @brief Compute projective representation
 * @param[in] params EC domain parameters
 * @param[out] r Projective representation of the point
 * @param[in] s Affine representation of the point
 * @return Error code
 **/
RES_CODE EcPoint::ecProjectify(const EcPoint* s)
{
	RES_CODE res;

	//Copy point
	res = ecCopy(s);
	if(res == RES_OK)
	{
		//Map the point to projective space
		res = z.mpiSetValue(1);
	}

	return res;
}





void EcDomainParameters::ecdp_free()
{
	type = EC_CURVE_TYPE_NONE;
	p.mpi_free();
	pa.mpi_free();
	pb.mpi_free();
	q.mpi_free();
	g.ecp_free();
	mod = nullptr;
}

RES_CODE EcDomainParameters::ecLoadDomainParameters(const EcCurveInfo* curveInfo)
{
	RES_CODE res;

	//Debug message
	TRACE_TLS("Loading %s EC domain parameters...\r\n", curveInfo->name);

	//Curve type
	type = curveInfo->type;

	//Import prime modulus
	res = p.mpiReadRaw(curveInfo->p, curveInfo->pLen);
	if (res == RES_OK)
	{
		//Import parameter a
		res = pa.mpiReadRaw(curveInfo->a, curveInfo->aLen);
		if (res == RES_OK)
		{
			//Import parameter b
			res = pb.mpiReadRaw(curveInfo->b, curveInfo->bLen);
			if (res == RES_OK)
			{
				//Import the x-coordinate of the base point G
				res = g.x.mpiReadRaw(curveInfo->gx, curveInfo->gxLen);
				if (res == RES_OK)
				{
					//Import the y-coordinate of the base point G
					res = g.y.mpiReadRaw(curveInfo->gy, curveInfo->gyLen);
					if (res == RES_OK)
					{
						//Import base point order q
						res = q.mpiReadRaw(curveInfo->q, curveInfo->qLen);
						if (res == RES_OK)
						{
							//Normalize base point G
							res = g.z.mpiSetValue(1);
							if (res == RES_OK)
							{
								//Fast modular reduction
								mod = curveInfo->mod;

								//Debug message
								TRACE1_TLS("  p:\r\n");
								TRACE_MPI("    ", &p);
								TRACE1_TLS("  a:\r\n");
								TRACE_MPI("    ", &pa);
								TRACE1_TLS("  b:\r\n");
								TRACE_MPI("    ", &pb);
								TRACE1_TLS("  Gx:\r\n");
								TRACE_MPI("    ", &g.x);
								TRACE1_TLS("  Gy:\r\n");
								TRACE_MPI("    ", &g.y);
								TRACE1_TLS("  q:\r\n");
								TRACE_MPI("    ", &q);
							}
						}
					}
				}
			}
		}
	}

	return res;
}

/**
 * @brief Point doubling
 * @param[in] params EC domain parameters
 * @param[out] r Resulting point R = 2S
 * @param[in] s Point S
 * @return Error code
 **/
RES_CODE EcDomainParameters::ecDouble(EcPoint* r, const EcPoint* s) const
{
	RES_CODE res;
	Mpi t1;
	Mpi t2;
	Mpi t3;
	Mpi t4;
	Mpi t5;


	//Point at the infinity?
	if (!s->z.mpiCompInt(0))
	{
		//Set R = (1, 1, 0)
		res = r->x.mpiSetValue(1);
		if(res == RES_OK)
		{
			res = r->y.mpiSetValue(1);
			if(res == RES_OK)
				res = r->z.mpiSetValue(0);
		}
	}
	else
	{
		//Set t1 = Sx
		res = t1.mpiCopy(&s->x);
		if(res == RES_OK)
		{
			//Set t2 = Sy
			res = t2.mpiCopy(&s->y);
			if(res == RES_OK)
			{
				//Set t3 = Sz
				res = t3.mpiCopy(&s->z);
				if(res == RES_OK)
				{
					//SECP K1 elliptic curve?
					if (type == EC_CURVE_TYPE_SECP_K1)
					{
						//Compute t5 = t1^2
						res = ecSqrMod(&t5, &t1);
						if(res == RES_OK)
						{
							//Compute t4 = 3 * t5
							res = ecAddMod(&t4, &t5, &t5);
							if(res == RES_OK)
							{
								res = ecAddMod(&t4, &t4, &t5);
							}
						}
					}
					//SECP R1 elliptic curve?
					else if (type == EC_CURVE_TYPE_SECP_R1)
					{
						//Compute t4 = t3^2
						res = ecSqrMod(&t4, &t3);
						if(res == RES_OK)
						{
							//Compute t5 = t1 - t4
							res = ecSubMod(&t5, &t1, &t4);
							if(res == RES_OK)
							{
								//Compute t4 = t1 + t4
								res = ecAddMod(&t4, &t1, &t4);
								if(res == RES_OK)
								{
									//Compute t5 = t4 * t5
									res = ecMulMod(&t5, &t4, &t5);
									if(res == RES_OK)
									{
										//Compute t4 = 3 * t5
										res = ecAddMod(&t4, &t5, &t5);
										if(res == RES_OK)
										{
											res = ecAddMod(&t4, &t4, &t5);
										}
									}
								}
							}
						}
					}
					else
					{
						//Compute t4 = t3^4
						res = ecSqrMod(&t4, &t3);
						if(res == RES_OK)
						{
							res = ecSqrMod(&t4, &t4);
							if(res == RES_OK)
							{
								//Compute t4 = a * t4
								res = ecMulMod(&t4, &t4, &pa);
								if(res == RES_OK)
								{
									//Compute t5 = t1^2
									res = ecSqrMod(&t5, &t1);
									if(res == RES_OK)
									{
										//Compute t4 = t4 + 3 * t5
										res = ecAddMod(&t4, &t4, &t5);
										if(res == RES_OK)
										{
											res = ecAddMod(&t4, &t4, &t5);
											if(res == RES_OK)
											{
												res = ecAddMod(&t4, &t4, &t5);
											}
										}
									}
								}
							}
						}
					}

					if(res == RES_OK)
					{
						//Compute t3 = t3 * t2
						res = ecMulMod(&t3, &t3, &t2);
						if(res == RES_OK)
						{
							//Compute t3 = 2 * t3
							res = ecAddMod(&t3, &t3, &t3);
							if(res == RES_OK)
							{
								//Compute t2 = t2^2
								res = ecSqrMod(&t2, &t2);
								if(res == RES_OK)
								{
									//Compute t5 = t1 * t2
									res = ecMulMod(&t5, &t1, &t2);
									if(res == RES_OK)
									{
										//Compute t5 = 4 * t5
										res = ecAddMod(&t5, &t5, &t5);
										if(res == RES_OK)
										{
											res = ecAddMod(&t5, &t5, &t5);
											if(res == RES_OK)
											{
												//Compute t1 = t4^2
												res = ecSqrMod(&t1, &t4);
											}
										}
									}
								}
							}
						}
						if(res == RES_OK)
						{
							//Compute t1 = t1 - 2 * t5
							res = ecSubMod(&t1, &t1, &t5);
							if(res == RES_OK)
							{
								res = ecSubMod(&t1, &t1, &t5);
								if(res == RES_OK)
								{
									//Compute t2 = t2^2
									res = ecSqrMod(&t2, &t2);
									if(res == RES_OK)
									{
										//Compute t2 = 8 * t2
										res = ecAddMod(&t2, &t2, &t2);
										if(res == RES_OK)
										{
											res = ecAddMod(&t2, &t2, &t2);
											if(res == RES_OK)
											{
												res = ecAddMod(&t2, &t2, &t2);
												if(res == RES_OK)
												{
													//Compute t5 = t5 - t1
													res = ecSubMod(&t5, &t5, &t1);
													if(res == RES_OK)
													{
														//Compute t5 = t4 * t5
														res = ecMulMod(&t5, &t4, &t5);
														if(res == RES_OK)
														{
															//Compute t2 = t5 - t2
															res = ecSubMod(&t2, &t5, &t2);
															if(res == RES_OK)
															{
																//Set Rx = t1
																res = r->x.mpiCopy(&t1);
																if(res == RES_OK)
																{
																	//Set Ry = t2
																	res = r->y.mpiCopy(&t2);
																	if(res == RES_OK)
																	{
																		//Set Rz = t3
																		res = r->z.mpiCopy(&t3);
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return res;
}

/**
 * @brief Point addition (helper routine)
 * @param[in] params EC domain parameters
 * @param[out] r Resulting point R = S + T
 * @param[in] s First operand
 * @param[in] t Second operand
 * @return Error code
 **/
RES_CODE EcDomainParameters::ecAdd(EcPoint* r, const EcPoint* s, const EcPoint* t) const
{
	RES_CODE res;
	Mpi t1;
	Mpi t2;
	Mpi t3;
	Mpi t4;
	Mpi t5;
	Mpi t6;
	Mpi t7;

	//Set t1 = Sx
	MPI_CHECK(t1.mpiCopy(&s->x));
	//Set t2 = Sy
	MPI_CHECK(t2.mpiCopy(&s->y));
	//Set t3 = Sz
	MPI_CHECK(t3.mpiCopy(&s->z));
	//Set t4 = Tx
	MPI_CHECK(t4.mpiCopy(&t->x));
	//Set t5 = Ty
	MPI_CHECK(t5.mpiCopy(&t->y));

	//Check whether Tz != 1
	if (t->z.mpiCompInt(1))
	{
		//Compute t6 = Tz
		MPI_CHECK(t6.mpiCopy(&t->z));
		//Compute t7 = t6^2
		EC_CHECK(ecSqrMod(&t7, &t6));
		//Compute t1 = t1 * t7
		EC_CHECK(ecMulMod(&t1, &t1, &t7));
		//Compute t7 = t6 * t7
		EC_CHECK(ecMulMod(&t7, &t6, &t7));
		//Compute t2 = t2 * t7
		EC_CHECK(ecMulMod(&t2, &t2, &t7));
	}

	//Compute t7 = t3^2
	EC_CHECK(ecSqrMod(&t7, &t3));
	//Compute t4 = t4 * t7
	EC_CHECK(ecMulMod(&t4, &t4, &t7));
	//Compute t7 = t3 * t7
	EC_CHECK(ecMulMod(&t7, &t3, &t7));
	//Compute t5 = t5 * t7
	EC_CHECK(ecMulMod(&t5, &t5, &t7));
	//Compute t4 = t1 - t4
	EC_CHECK(ecSubMod(&t4, &t1, &t4));
	//Compute t5 = t2 - t5
	EC_CHECK(ecSubMod(&t5, &t2, &t5));

	//Check whether t4 == 0
	if (!t4.mpiCompInt(0))
	{
		//Check whether t5 == 0
		if (!t5.mpiCompInt(0))
		{
			//Set R = (0, 0, 0)
			MPI_CHECK(r->x.mpiSetValue(0));
			MPI_CHECK(r->y.mpiSetValue(0));
			MPI_CHECK(r->z.mpiSetValue(0));
		}
		else
		{
			//Set R = (1, 1, 0)
			MPI_CHECK(r->x.mpiSetValue(1));
			MPI_CHECK(r->y.mpiSetValue(1));
			MPI_CHECK(r->z.mpiSetValue(0));
		}
	}
	else
	{
		//Compute t1 = 2 * t1 - t4
		EC_CHECK(ecAddMod(&t1, &t1, &t1));
		EC_CHECK(ecSubMod(&t1, &t1, &t4));
		//Compute t2 = 2 * t2 - t5
		EC_CHECK(ecAddMod(&t2, &t2, &t2));
		EC_CHECK(ecSubMod(&t2, &t2, &t5));

		//Check whether Tz != 1
		if (t->z.mpiCompInt(1))
		{
			//Compute t3 = t3 * t6
			EC_CHECK(ecMulMod(&t3, &t3, &t6));
		}

		//Compute t3 = t3 * t4
		EC_CHECK(ecMulMod(&t3, &t3, &t4));
		//Compute t7 = t4^2
		EC_CHECK(ecSqrMod(&t7, &t4));
		//Compute t4 = t4 * t7
		EC_CHECK(ecMulMod(&t4, &t4, &t7));
		//Compute t7 = t1 * t7
		EC_CHECK(ecMulMod(&t7, &t1, &t7));
		//Compute t1 = t5^2
		EC_CHECK(ecSqrMod(&t1, &t5));
		//Compute t1 = t1 - t7
		EC_CHECK(ecSubMod(&t1, &t1, &t7));
		//Compute t7 = t7 - 2 * t1
		EC_CHECK(ecAddMod(&t6, &t1, &t1));
		EC_CHECK(ecSubMod(&t7, &t7, &t6));
		//Compute t5 = t5 * t7
		EC_CHECK(ecMulMod(&t5, &t5, &t7));
		//Compute t4 = t2 * t4
		EC_CHECK(ecMulMod(&t4, &t2, &t4));
		//Compute t2 = t5 - t4
		EC_CHECK(ecSubMod(&t2, &t5, &t4));

		//Compute t2 = t2 / 2
		if (!t2.mpiGetBitValue(0))
		{
			MPI_CHECK(t2.mpiShiftRight(1));
		}
		else
		{
			MPI_CHECK(t2.mpiAdd(&t2, &p));
			MPI_CHECK(t2.mpiShiftRight(1));
		}

		//Set Rx = t1
		MPI_CHECK(r->x.mpiCopy(&t1));
		//Set Ry = t2
		MPI_CHECK(r->y.mpiCopy(&t2));
		//Set Rz = t3
		MPI_CHECK(r->z.mpiCopy(&t3));
	}

end:
	return res;
}

/**
 * @brief Point addition
 * @param[in] params EC domain parameters
 * @param[out] r Resulting point R = S + T
 * @param[in] s First operand
 * @param[in] t Second operand
 * @return Error code
 **/
RES_CODE EcDomainParameters::ecFullAdd(EcPoint* r, const EcPoint* s, const EcPoint* t) const
{
	RES_CODE res;

	//Check whether Sz == 0
	if (!s->z.mpiCompInt(0))
	{
		//Set R = T
		MPI_CHECK(r->x.mpiCopy(&t->x));
		MPI_CHECK(r->y.mpiCopy(&t->y));
		MPI_CHECK(r->z.mpiCopy(&t->z));
	}
	//Check whether Tz == 0
	else if (!t->z.mpiCompInt(0))
	{
		//Set R = S
		MPI_CHECK(r->x.mpiCopy(&s->x));
		MPI_CHECK(r->y.mpiCopy(&s->y));
		MPI_CHECK(r->z.mpiCopy(&s->z));
	}
	else
	{
		//Compute R = S + T
		EC_CHECK(ecAdd(r, s, t));

		//Check whether R == (0, 0, 0)
		if (!r->x.mpiCompInt(0) && !r->y.mpiCompInt(0)	&& !r->z.mpiCompInt(0))
		{
			//Compute R = 2 * S
			EC_CHECK(ecDouble(r, s));
		}
	}

end:
	return res;
}

/**
 * @brief Point subtraction
 * @param[in] params EC domain parameters
 * @param[out] r Resulting point R = S - T
 * @param[in] s First operand
 * @param[in] t Second operand
 * @return Error code
 **/
RES_CODE EcDomainParameters::ecFullSub(EcPoint* r, const EcPoint* s, const EcPoint* t) const
{
	RES_CODE res;
	EcPoint u;

	//Set Ux = Tx and Uz = Tz
	MPI_CHECK(u.x.mpiCopy(&t->x));
	MPI_CHECK(u.z.mpiCopy(&t->z));
	//Set Uy = p - Ty
	MPI_CHECK(u.y.mpiSub(&p, &t->y));

	//Compute R = S + U
	EC_CHECK(ecFullAdd(r, s, &u));

end:
	return res;
}

/**
 * @brief Scalar multiplication
 * @param[in] params EC domain parameters
 * @param[out] r Resulting point R = d.S
 * @param[in] d An integer d such as 0 <= d < p
 * @param[in] s EC point
 * @return Error code
 **/
RES_CODE EcDomainParameters::ecMult(EcPoint* r, const Mpi* d, const EcPoint* s) const
{
	RES_CODE res;
	int i;
	Mpi h;


	//Check whether d == 0  or whether Sz == 0
	if (!d->mpiCompInt(0) || !s->z.mpiCompInt(0))
	{
		//Set R = (1, 1, 0)
		MPI_CHECK(r->x.mpiSetValue(1));
		MPI_CHECK(r->y.mpiSetValue(1));
		MPI_CHECK(r->z.mpiSetValue(0));
	}
	//Check whether d == 1
	else if (!d->mpiCompInt(1))
	{
		//Set R = S
		MPI_CHECK(r->x.mpiCopy(&s->x));
		MPI_CHECK(r->y.mpiCopy(&s->y));
		MPI_CHECK(r->z.mpiCopy(&s->z));
	}
	else
	{
		//Check whether Sz != 1
		if (s->z.mpiCompInt(1))
		{
			//Normalize S
			EC_CHECK(ecAffinify(r, s));
			EC_CHECK(ecProjectify(r, r));
		}
		else
		{
			//Set R = S
			MPI_CHECK(r->x.mpiCopy(&s->x));
			MPI_CHECK(r->y.mpiCopy(&s->y));
			MPI_CHECK(r->z.mpiCopy(&s->z));
		}

		//Fast left-to-right binary method
		//Precompute h = 3 * d
		MPI_CHECK(h.mpiAdd(d, d));
		MPI_CHECK(h.mpiAdd(&h, d));

		//Scalar multiplication
		for (i = h.mpiGetBitLength() - 2; i >= 1; i--)
		{
			//Point doubling
			EC_CHECK(ecDouble(r, r));

			//Check whether h(i) == 1 and k(i) == 0
			if (h.mpiGetBitValue(i) && !d->mpiGetBitValue(i))
			{
				//Compute R = R + S
				EC_CHECK(ecFullAdd(r, r, s));
			}
			//Check whether h(i) == 0 and k(i) == 1
			else if (!h.mpiGetBitValue(i) && d->mpiGetBitValue(i))
			{
				//Compute R = R - S
				EC_CHECK(ecFullSub(r, r, s));
			}
		}
	}

end:
	return res;
}

/**
 * @brief An auxiliary function for the twin multiplication
 * @param[in] t An integer T such as 0 <= T <= 31
 * @return Output value
 **/

uint32_t ecTwinMultF(uint32_t t)
{
   if(18 <= t && t < 22)
      return 9;
   else if(14 <= t && t < 18)
      return 10;
   else if(22 <= t && t < 24)
      return 11;
   else if(4 <= t && t < 12)
      return 14;
   else
      return 12;
}

/**
 * @brief Twin multiplication
 * @param[in] params EC domain parameters
 * @param[out] r Resulting point R = d0.S + d1.T
 * @param[in] d0 An integer d such as 0 <= d0 < p
 * @param[in] s EC point
 * @param[in] d1 An integer d such as 0 <= d1 < p
 * @param[in] t EC point
 * @return Error code
 **/
RES_CODE EcDomainParameters::ecTwinMult(EcPoint* r, const Mpi* d0, const EcPoint* s,
		const Mpi* d1, const EcPoint* t) const
{
	RES_CODE res;
	int k, u0, u1;
	uint32_t m, m0, m1, c0, c1, h0, h1;
	EcPoint spt;
	EcPoint smt;

	//Precompute SpT = S + T
	EC_CHECK(ecFullAdd(&spt, s, t));
	//Precompute SmT = S - T
	EC_CHECK(ecFullSub(&smt, s, t));

	//Let m0 be the bit length of d0
	m0 = d0->mpiGetBitLength();
	//Let m1 be the bit length of d1
	m1 = d1->mpiGetBitLength();
	//Let m = MAX(m0, m1)
	m = max(m0, m1);

	//Let c be a 2 x 6 binary matrix
	c0 = d0->mpiGetBitValue(m - 4);
	c0 |= d0->mpiGetBitValue(m - 3) << 1;
	c0 |= d0->mpiGetBitValue(m - 2) << 2;
	c0 |= d0->mpiGetBitValue(m - 1) << 3;
	c1 = d1->mpiGetBitValue(m - 4);
	c1 |= d1->mpiGetBitValue(m - 3) << 1;
	c1 |= d1->mpiGetBitValue(m - 2) << 2;
	c1 |= d1->mpiGetBitValue(m - 1) << 3;

	//Set R = (1, 1, 0)
	MPI_CHECK(r->x.mpiSetValue(1));
	MPI_CHECK(r->y.mpiSetValue(1));
	MPI_CHECK(r->z.mpiSetValue(0));

	//Calculate both multiplications at the same time
	for (k = m; k >= 0; k--)
	{
		//Compute h(0) = 16 * c(0,1) + 8 * c(0,2) + 4 * c(0,3) + 2 * c(0,4) + c(0,5)
		h0 = c0 & 0x1F;
		//Check whether c(0,0) == 1
		if (c0 & 0x20)
			h0 = 31 - h0;

		//Compute h(1) = 16 * c(1,1) + 8 * c(1,2) + 4 * c(1,3) + 2 * c(1,4) + c(1,5)
		h1 = c1 & 0x1F;
		//Check whether c(1,0) == 1
		if (c1 & 0x20)
			h1 = 31 - h1;

		//Compute u(0)
		if (h0 < ecTwinMultF(h1))
			u0 = 0;
		else if (c0 & 0x20)
			u0 = -1;
		else
			u0 = 1;

		//Compute u(1)
		if (h1 < ecTwinMultF(h0))
			u1 = 0;
		else if (c1 & 0x20)
			u1 = -1;
		else
			u1 = 1;

		//Update c matrix
		c0 <<= 1;
		c0 |= d0->mpiGetBitValue(k - 5);
		c0 ^= u0 ? 0x20 : 0x00;
		c1 <<= 1;
		c1 |= d1->mpiGetBitValue(k - 5);
		c1 ^= u1 ? 0x20 : 0x00;

		//Point doubling
		EC_CHECK(ecDouble(r, r));

		//Check u(0) and u(1)
		if (u0 == -1 && u1 == -1)
		{
			//Compute R = R - SpT
			EC_CHECK(ecFullSub(r, r, &spt));
		}
		else if (u0 == -1 && u1 == 0)
		{
			//Compute R = R - S
			EC_CHECK(ecFullSub(r, r, s));
		}
		else if (u0 == -1 && u1 == 1)
		{
			//Compute R = R - SmT
			EC_CHECK(ecFullSub(r, r, &smt));
		}
		else if (u0 == 0 && u1 == -1)
		{
			//Compute R = R - T
			EC_CHECK(ecFullSub(r, r, t));
		}
		else if (u0 == 0 && u1 == 1)
		{
			//Compute R = R + T
			EC_CHECK(ecFullAdd(r, r, t));
		}
		else if (u0 == 1 && u1 == -1)
		{
			//Compute R = R + SmT
			EC_CHECK(ecFullAdd(r, r, &smt));
		}
		else if (u0 == 1 && u1 == 0)
		{
			//Compute R = R + S
			EC_CHECK(ecFullAdd(r, r, s));
		}
		else if (u0 == 1 && u1 == 1)
		{
			//Compute R = R + SpT
			EC_CHECK(ecFullAdd(r, r, &spt));
		}
	}

end:
	return res;
}

/**
 * @brief Fast modular addition
 * @param[in] params EC domain parameters
 * @param[out] r Resulting integer R = (A + B) mod p
 * @param[in] a An integer such as 0 <= A < p
 * @param[in] b An integer such as 0 <= B < p
 * @return Error code
 **/
RES_CODE EcDomainParameters::ecAddMod(Mpi *r, const Mpi *a, const Mpi *b) const
{
	RES_CODE res;

	//Compute R = A + B
	res = r->mpiAdd(a, b);
	if(res == RES_OK)
	{
		//Compute R = (A + B) mod p
		if (r->mpiComp(&p) >= 0)
		{
			res = r->mpiSub(r, &p);
		}
	}

	return res;
}

/**
 * @brief Fast modular subtraction
 * @param[in] params EC domain parameters
 * @param[out] r Resulting integer R = (A - B) mod p
 * @param[in] a An integer such as 0 <= A < p
 * @param[in] b An integer such as 0 <= B < p
 * @return Error code
 **/
RES_CODE EcDomainParameters::ecSubMod(Mpi *r, const Mpi *a, const Mpi *b) const
{
	RES_CODE res;

	//Compute R = A - B
	res = r->mpiSub(a, b);
	if(res == RES_OK)
	{
		//Compute R = (A - B) mod p
		if (r->mpiCompInt(0) < 0)
		{
			res = r->mpiAdd(r, &p);
		}
	}

	return res;
}

/**
 * @brief Fast modular multiplication
 * @param[in] params EC domain parameters
 * @param[out] r Resulting integer R = (A * B) mod p
 * @param[in] a An integer such as 0 <= A < p
 * @param[in] b An integer such as 0 <= B < p
 * @return Error code
 **/
RES_CODE EcDomainParameters::ecMulMod(Mpi *r, const Mpi *a, const Mpi *b) const
{
	RES_CODE res;

	//Compute R = A * B
	res = r->mpiMul(a, b);
	if(res == RES_OK)
	{
		//Compute R = (A * B) mod p
		if (mod)
		{
			res = mod(r, &p);
		}
		else
		{
			res = r->mpiMod(r, &p);
		}
	}

	return res;
}

/**
 * @brief Fast modular squaring
 * @param[in] params EC domain parameters
 * @param[out] r Resulting integer R = (A ^ 2) mod p
 * @param[in] a An integer such as 0 <= A < p
 * @return Error code
 **/
RES_CODE EcDomainParameters::ecSqrMod(Mpi *r, const Mpi *a) const
{
	RES_CODE res;

	//Compute R = A ^ 2
	res = r->mpiMul(a, a);
	if(res == RES_OK)
	{
		//Compute R = (A ^ 2) mod p
		if (mod)
		{
			res = mod(r, &p);
		}
		else
		{
			res = r->mpiMod(r, &p);
		}
	}

	return res;
}

/**
 * @brief Convert an octet string to an EC point
 * @param[in] params EC domain parameters
 * @param[out] r EC point resulting from the conversion
 * @param[in] data Pointer to the octet string
 * @param[in] length Length of the octet string
 * @return Error code
 **/

RES_CODE EcDomainParameters::ecImport(EcPoint* r, const uint8_t* data, size_t length) const
{
   RES_CODE res;
   size_t k;

   //Get the length in octets of the prime
   k = p.mpiGetByteLength();

   //Check the length of the octet string
   if(length != (k * 2 + 1))
      return RES_TLS_DECODING_FAILED;

   //Compressed point representation is not supported
   if(data[0] != 0x04)
      return RES_TLS_ILLEGAL_PARAMETER;

   //Convert the x-coordinate to a multiple precision integer
   res = r->x.mpiReadRaw(data + 1, k);
   if(res != RES_OK)
      return res;

   //Convert the y-coordinate to a multiple precision integer
   res = r->y.mpiReadRaw(data + k + 1, k);

   return res;
}

/**
 * @brief Convert an EC point to an octet string
 * @param[in] params EC domain parameters
 * @param[in] a EC point to be converted
 * @param[out] data Pointer to the octet string
 * @param[out] length Length of the resulting octet string
 * @return Error code
 **/
RES_CODE EcDomainParameters::ecExport(const EcPoint* a, uint8_t* data, size_t* length) const
{
	RES_CODE res;
	size_t k;

	//Get the length in octets of the prime
	k = p.mpiGetByteLength();

	if(data)
	{
		//Point compression is not used
		data[0] = 0x04;

		//Convert the x-coordinate to an octet string
		res = a->x.mpiWriteRaw(data + 1, k);
		if (res != RES_OK)
			return res;

		//Convert the y-coordinate to an octet string
		res = a->y.mpiWriteRaw(data + k + 1, k);
		if (res != RES_OK)
			return res;
	}

	//Return the total number of bytes that have been written
	*length = k * 2 + 1;
	//Successful processing
	return RES_OK;
}

/**
 * @brief Compute projective representation
 * @param[in] params EC domain parameters
 * @param[out] r Projective representation of the point
 * @param[in] s Affine representation of the point
 * @return Error code
 **/
RES_CODE EcDomainParameters::ecProjectify(EcPoint* r, const EcPoint* s) const
{
	RES_CODE res;

	//Copy point
	EC_CHECK(r->ecCopy(s));
	//Map the point to projective space
	MPI_CHECK(r->z.mpiSetValue(1));

end:
	return res;
}

/**
 * @brief Recover affine representation
 * @param[in] params EC domain parameters
 * @param[out] r Affine representation of the point
 * @param[in] s Projective representation of the point
 * @return Error code
 **/
RES_CODE EcDomainParameters::ecAffinify(EcPoint* r, const EcPoint* s) const
{
	RES_CODE res;
	Mpi a;
	Mpi b;

	//Point at the infinity?
	if (!s->z.mpiCompInt(0))
		return RES_TLS_INVALID_PARAMETER;

	//Compute a = 1/Sz mod p
	MPI_CHECK(a.mpiInvMod(&s->z, &p));

	//Set Rx = a^2 * Sx mod p
	EC_CHECK(ecSqrMod(&b, &a));
	EC_CHECK(ecMulMod(&r->x, &b, &s->x));

	//Set Ry = a^3 * Sy mod p
	EC_CHECK(ecMulMod(&b, &b, &a));
	EC_CHECK(ecMulMod(&r->y, &b, &s->y));

	//Set Rz = 1
	MPI_CHECK(r->z.mpiSetValue(1));

end:
	return res;
}

/**
 * @brief Check whether the affine point S is on the curve
 * @param[in] params EC domain parameters
 * @param[in] s Affine representation of the point
 * @return TRUE if the affine point S is on the curve, else false
 **/
RES_CODE EcDomainParameters::ecIsPointAffine(const EcPoint *s) const
{
	RES_CODE res;
	Mpi t1;
	Mpi t2;

	//Compute t1 = (Sx^3 + a * Sx + b) mod p
	EC_CHECK(ecSqrMod(&t1, &s->x));
	EC_CHECK(ecMulMod(&t1, &t1, &s->x));
	EC_CHECK(ecMulMod(&t2, &pa, &s->x));
	EC_CHECK(ecAddMod(&t1, &t1, &t2));
	EC_CHECK(ecAddMod(&t1, &t1, &pb));

	//Compute t2 = Sy^2
	EC_CHECK(ecSqrMod(&t2, &s->y));

	//Check whether the point is on the elliptic curve
	if (t1.mpiComp(&t2))
		res = RES_ERROR;

end:
	return res;
}

RES_CODE EcDomainParameters::ecGeneratePrivateKey(prng_algo_t* prngAlgo, Mpi *privateKey) const
{
	RES_CODE res;
	uint32_t n;

	//Check parameters
	if (prngAlgo == nullptr ||  privateKey == nullptr)
	{
		return RES_TLS_INVALID_PARAMETER;
	}

	//Let N be the bit length of q
	n = q.mpiGetBitLength();

	//Generated a pseudorandom number
	MPI_CHECK(privateKey->mpiRand(n, prngAlgo));

	//Make sure that 0 < d < q
	if (privateKey->mpiComp(&q) >= 0)
	{
		EC_CHECK(privateKey->mpiShiftRight(1));
	}

	//Debug message
	TRACE1_TLS("  Private key:");
	TRACE_MPI("    ", privateKey);

end:
	//Return status code
	return res;
}

RES_CODE EcDomainParameters::ecGeneratePublicKey(const Mpi* privateKey, EcPoint* publicKey) const
{
	RES_CODE res;

	//Check parameters
	if (privateKey == nullptr || publicKey == nullptr)
		return RES_TLS_INVALID_PARAMETER;

	//Compute Q = d.G
	EC_CHECK(ecMult(publicKey, privateKey, &g));

	//Convert the public key to affine representation
	EC_CHECK(ecAffinify(publicKey, publicKey));

	//Debug message
	TRACE1_TLS("  Public key X:");
	TRACE_MPI("    ", &publicKey->x);
	TRACE1_TLS("  Public key Y:");
	TRACE_MPI("    ", &publicKey->y);

end:
	//Return status code
	return res;
}

RES_CODE EcDomainParameters::ecGenerateKeyPair(prng_algo_t* prngAlgo, Mpi* privateKey,
		EcPoint* publicKey) const
{
	RES_CODE res;

	//Generate a private key
	res = ecGeneratePrivateKey(prngAlgo, privateKey);

	//Check status code
	if (res == RES_OK)
	{
		//Derive the public key from the private key
		res = ecGeneratePublicKey(privateKey, publicKey);
	}

	//Return status code
	return res;
}

RES_CODE pkcs8FormatEcPrivateKey(const EcCurveInfo* curveInfo,
   const Mpi* privateKey, const EcPoint* publicKey, uint8_t* output,
   size_t* written)
{
   RES_CODE res;
   size_t n;
   size_t length;
   uint8_t *p;
   Asn1Tag tag;

   //Point to the buffer where to write the ASN.1 structure
   p = output;
   //Length of the ASN.1 structure
   length = 0;

   //Format Version field (refer to RFC 5915, section 3)
   res = asn1WriteInt32(1, false, p, &n);
   //Any error to report?
   if(res != RES_OK)
      return res;

   //Update the length of the ECPrivateKey structure
   length += n;

   //If the output parameter is NULL, then the function calculates the
   //length of the resulting PEM file without copying any data
   if(output != nullptr)
   {
      //Advance data pointer
      p += n;

      //Write the EC private key
      res = privateKey->mpiWriteRaw(p, curveInfo->pLen);
      //Any error to report?
      if(res != RES_OK)
         return res;
   }

   //Format PrivateKey field
   tag.constructed = false;
   tag.objClass = ASN1_CLASS_UNIVERSAL;
   tag.objType = ASN1_TYPE_OCTET_STRING;
   tag.length = curveInfo->pLen;
   tag.value = p + (p?0:1);

   //Write the corresponding ASN.1 tag
   res = tag.asn1WriteTag(false, p, &n);
   //Any error to report?
   if(res != RES_OK)
      return res;

   //Update the length of the ECPrivateKey structure
   n = tag.totalLength;
   length += n;

   //Advance data pointer
   if(output != nullptr)
      p += n;

   //The public key is optional
   if(publicKey != nullptr)
   {
      //Format PublicKey field
      res = pkcs8FormatEcPublicKey(curveInfo, publicKey, p, &n);
      //Any error to report?
      if(res != RES_OK)
         return res;

      //Update the length of the ECPrivateKey structure
      length += n;

      //Advance data pointer
      if(output != nullptr)
         p += n;
   }

   //Format ECPrivateKey field
   tag.constructed = true;
   tag.objClass = ASN1_CLASS_UNIVERSAL;
   tag.objType = ASN1_TYPE_SEQUENCE;
   tag.length = length;
   tag.value = output + (output?0:1);

   //Write the corresponding ASN.1 tag
   res = tag.asn1WriteTag(false, output, &n);
   //Any error to report?
   if(res != RES_OK)
      return res;

   //Get the length of the ECPrivateKey structure
   n = tag.totalLength;

   //The PrivateKey field is an octet string whose contents are the value
   //of the private key
   tag.constructed = false;
   tag.objClass = ASN1_CLASS_UNIVERSAL;
   tag.objType = ASN1_TYPE_OCTET_STRING;
   tag.length = n;
   tag.value = output + (output?0:1);

   //Write the corresponding ASN.1 tag
   res = tag.asn1WriteTag(false, output, &n);
   //Any error to report?
   if(res != RES_OK)
      return res;

   //Total number of bytes that have been written
   *written = tag.totalLength;

   //Successful processing
   return RES_OK;
}

#endif

RES_CODE pkcs8FormatEcPublicKey(const EcCurveInfo* curveInfo,
   const EcPoint* publicKey, uint8_t* output, size_t* written)
{
#if EC_SUPPORT
   RES_CODE res;
   size_t n;
   EcDomainParameters params;
   Asn1Tag tag;


   //The bit string shall contain an initial octet which encodes the number
   //of unused bits in the final subsequent octet
   if(output)
	   output[0] = 0;

   //Load EC domain parameters
   res = params.ecLoadDomainParameters(curveInfo);

   //Check status code
   if(res == RES_OK)
   {
      //Format ECPublicKey structure
      res = params.ecExport(publicKey, output + (output?1:0), &n);
   }

   //Check status code
   if(res == RES_OK)
   {
      //The public key is encapsulated within a bit string
      tag.constructed = false;
      tag.objClass = ASN1_CLASS_UNIVERSAL;
      tag.objType = ASN1_TYPE_BIT_STRING;
      tag.length = n + 1;
      tag.value = output + (output?0:1);

      //Write the corresponding ASN.1 tag
      res = tag.asn1WriteTag(false, output, &n);
   }

   //Check status code
   if(res == RES_OK)
   {
      //Explicit tagging shall be used to encode the public key
      tag.constructed = true;
      tag.objClass = ASN1_CLASS_CONTEXT_SPECIFIC;
      tag.objType = (Asn1Type)1;
      tag.length = n;
      tag.value = output + (output?0:1);

      //Write the corresponding ASN.1 tag
      res = tag.asn1WriteTag(false, output, written);
   }

   //Return status code
   return res;
#else
   //Not implemented
   return RES_TLS_NOT_IMPLEMENTED;
#endif
}

RES_CODE pemExportEcPrivateKey(const EcCurveInfo* curveInfo,
   const Mpi* privateKey, const EcPoint* publicKey, char *output,
   size_t* written)
{
#if EC_SUPPORT
   RES_CODE res;
   size_t n;
   size_t length;
   uint8_t *p;
   Asn1Tag tag;
   X509SubjectPublicKeyInfo publicKeyInfo;

   //Check parameters
   if(curveInfo == nullptr || privateKey == nullptr || written == nullptr)
      return RES_TLS_INVALID_PARAMETER;

   //Clear the SubjectPublicKeyInfo structure
   memclr(&publicKeyInfo, sizeof(X509SubjectPublicKeyInfo));

   //Point to the buffer where to write the PrivateKeyInfo structure
   p = (uint8_t *) output;
   //Total length of the PrivateKeyInfo structure
   length = 0;

   //Format Version field (refer to RFC 5208, section 5)
   res = asn1WriteInt32(0, false, p, &n);
   //Any error to report?
   if(res != RES_OK)
      return res;

   //Update the length of the PrivateKeyInfo structure
   length += n;

   //Advance data pointer
   if(output != nullptr)
      p += n;

   //The PrivateKeyAlgorithm identifies the private-key algorithm
   publicKeyInfo.oid = EC_PUBLIC_KEY_OID;
   publicKeyInfo.oidLen = sizeof(EC_PUBLIC_KEY_OID);
   publicKeyInfo.ecParams.namedCurve = curveInfo->oid;
   publicKeyInfo.ecParams.namedCurveLen = curveInfo->oidSize;

   //Format PrivateKeyAlgorithm field
   res = publicKeyInfo.x509FormatAlgorithmIdentifier(nullptr, p, &n);
   //Any error to report?
   if(res != RES_OK)
      return res;

   //Update the length of the PrivateKeyInfo structure
   length += n;

   //Advance data pointer
   if(output != nullptr)
      p += n;

   //Format PrivateKey field
   res = pkcs8FormatEcPrivateKey(curveInfo, privateKey, publicKey, p, &n);
   //Any error to report?
   if(res != RES_OK)
      return res;

   //Update the length of the PrivateKeyInfo structure
   length += n;

   //The PrivateKeyInfo structure is encapsulated within a sequence
   tag.constructed = true;
   tag.objClass = ASN1_CLASS_UNIVERSAL;
   tag.objType = ASN1_TYPE_SEQUENCE;
   tag.length = length;
   tag.value = (uint8_t *) output + (output?0:1);

   //Write the corresponding ASN.1 tag
   res = tag.asn1WriteTag(false, (uint8_t *) output, &n);
   //Any error to report?
   if(res != RES_OK)
      return res;

   //Get the length of the PrivateKeyInfo structure
   n = tag.totalLength;

   //PKCS#8 private keys are encoded using the "PRIVATE KEY" label
   res = pemEncodeFile(output, n, "PRIVATE KEY", output, &n);
   //Any error to report?
   if(res != RES_OK)
      return res;

   //Total number of bytes that have been written
   *written = n;

   //Successful processing
   return RES_OK;
#else
   //Not implemented
   return RES_TLS_NOT_IMPLEMENTED;
#endif
}

