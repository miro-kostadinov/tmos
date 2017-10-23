/*
 * oid.cpp
 *
 *  Created on: Jul 14, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <oid.h>

int32_t oidComp(const uint8_t* oid1, size_t oidLen1, const uint8_t* oid2, size_t oidLen2)
{
	size_t i;

	//Perform lexicographical comparison
	for (i = 0; i < oidLen1 && i < oidLen2; i++)
	{
		//Compare current byte
		if (oid1[i] < oid2[i])
			return -1;
		else if (oid1[i] > oid2[i])
			return 1;
	}

	//Compare length
	if (oidLen1 < oidLen2)
		return -1;
	else if (oidLen1 > oidLen2)
		return 1;

	//Object identifiers are equal
	return 0;
}

