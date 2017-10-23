/*
 * base64.cpp
 *
 *  Created on: Aug 1, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <base64.h>


#if BASE64_SUPPORT

//Base64 encoding table
const char base64EncTable[64] =
{
   'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
   'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
   'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
   'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

//Base64 decoding table
const uint8_t base64DecTable[128] =
{
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
   0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
   0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
   0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
   0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};


/**
 * @brief Base64 encoding algorithm
 * @param[in] input Input data to encode
 * @param[in] inputLength Length of the data to encode
 * @param[out] output NULL-terminated string encoded with Base64 algorithm
 * @param[out] outputLength Length of the encoded string (optional parameter)
 **/
void base64Encode(const void* input, size_t inputLength, char* output,
		size_t* outputLength)
{
	size_t n;
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	const uint8_t* p;

	//Point to the first byte of the input data
	p = (const uint8_t *) input;

	//Divide the input stream into blocks of 3 bytes
	n = inputLength / 3;

	//A full encoding quantum is always completed at the end of a quantity
	if (inputLength == (n * 3 + 1))
	{
		//The final quantum of encoding input is exactly 8 bits
		if (input != NULL && output != NULL)
		{
			//Read input data
			a = (p[n * 3] & 0xFC) >> 2;
			b = (p[n * 3] & 0x03) << 4;

			//The final unit of encoded output will be two characters followed
			//by two "=" padding characters
			output[n * 4] = base64EncTable[a];
			output[n * 4 + 1] = base64EncTable[b];
			output[n * 4 + 2] = '=';
			output[n * 4 + 3] = '=';
			output[n * 4 + 4] = '\0';
		}

		//Length of the encoded string (excluding the terminating NULL)
		if (outputLength != NULL)
			*outputLength = n * 4 + 4;
	}
	else if (inputLength == (n * 3 + 2))
	{
		//The final quantum of encoding input is exactly 16 bits
		if (input != NULL && output != NULL)
		{
			//Read input data
			a = (p[n * 3] & 0xFC) >> 2;
			b = ((p[n * 3] & 0x03) << 4) | ((p[n * 3 + 1] & 0xF0) >> 4);
			c = (p[n * 3 + 1] & 0x0F) << 2;

			//The final unit of encoded output will be three characters followed
			//by one "=" padding character
			output[n * 4] = base64EncTable[a];
			output[n * 4 + 1] = base64EncTable[b];
			output[n * 4 + 2] = base64EncTable[c];
			output[n * 4 + 3] = '=';
			output[n * 4 + 4] = '\0';
		}

		//Length of the encoded string (excluding the terminating NULL)
		if (outputLength != NULL)
			*outputLength = n * 4 + 4;
	}
	else
	{
		//The final quantum of encoding input is an integral multiple of 24 bits
		if (output != NULL)
		{
			//The final unit of encoded output will be an integral multiple of 4
			//characters with no "=" padding
			output[n * 4] = '\0';
		}

		//Length of the encoded string (excluding the terminating NULL)
		if (outputLength != NULL)
			*outputLength = n * 4;
	}

	//If the output parameter is NULL, then the function calculates the
	//length of the resulting Base64 string without copying any data
	if (input != NULL && output != NULL)
	{
		//The input data is processed block by block
		while (n-- > 0)
		{
			//Read input data
			a = (p[n * 3] & 0xFC) >> 2;
			b = ((p[n * 3] & 0x03) << 4) | ((p[n * 3 + 1] & 0xF0) >> 4);
			c = ((p[n * 3 + 1] & 0x0F) << 2) | ((p[n * 3 + 2] & 0xC0) >> 6);
			d = p[n * 3 + 2] & 0x3F;

			//Map each 3-byte block to 4 printable characters using the Base64
			//character set
			output[n * 4] = base64EncTable[a];
			output[n * 4 + 1] = base64EncTable[b];
			output[n * 4 + 2] = base64EncTable[c];
			output[n * 4 + 3] = base64EncTable[d];
		}
	}
}

/**
 * @brief Base64 decoding algorithm
 * @param[in] input Base64 encoded string
 * @param[in] inputLength Length of the encoded string
 * @param[out] output Resulting decoded data
 * @param[out] outputLength Length of the decoded data
 * @return Error code
 **/
RES_CODE base64Decode(const char* input, size_t inputLength, void* output,
		size_t* outputLength)
{
	size_t i;
	size_t j;
	uint32_t value;
	uint8_t *p;

	//Check parameters
	if (input == NULL && inputLength != 0)
		return RES_TLS_INVALID_PARAMETER;
	if (outputLength == NULL)
		return RES_TLS_INVALID_PARAMETER;

	//Point to the buffer where to write the decoded data
	p = (uint8_t *) output;
	//Length of the decoded data
	i = 0;

	//The length of the string to decode must be a multiple of 4
	if ((inputLength % 4) != 0)
		return RES_TLS_INVALID_LENGTH;

	//Process the Base64 encoded string
	while (inputLength >= 4)
	{
		//Divide the input stream into blocks of 4 characters
		for (value = 0, j = 0; j < 4; j++)
		{
			//The "==" sequence indicates that the last block contains only 1 byte
			if (inputLength == 2 && input[0] == '=' && input[1] == '=')
			{
				//Decode the last byte
				if (p != NULL)
					p[i] = (value >> 4) & 0xFF;

				//Return the length of the decoded data
				*outputLength = i + 1;

				//Decoding is now complete
				return RES_OK;
			}
			//The "=" sequence indicates that the last block contains only 2 bytes
			else if (inputLength == 1 && input[0] == '=')
			{
				//Decode the last two bytes
				if (p != NULL)
				{
					p[i] = (value >> 10) & 0xFF;
					p[i + 1] = (value >> 2) & 0xFF;
				}

				//Return the length of the decoded data
				*outputLength = i + 2;

				//Decoding is now complete
				return RES_OK;
			}

			//Ensure the current character belongs to the Base64 character set
			if (((uint8_t) *input) > 127
					|| base64DecTable[(uint8_t) *input] > 63)
			{
				//Decoding failed
				return RES_TLS_INVALID_CHARACTER;
			}

			//Decode the current character
			value = (value << 6) | base64DecTable[(uint8_t) *input];

			//Point to the next character to decode
			input++;
			//Remaining bytes to process
			inputLength--;
		}

		//Map each 4-character block to 3 bytes
		if (p != NULL)
		{
			p[i] = (value >> 16) & 0xFF;
			p[i + 1] = (value >> 8) & 0xFF;
			p[i + 2] = value & 0xFF;
		}

		//Next block
		i += 3;
	}

	//Return the length of the decoded data
	*outputLength = i;

	//Decoding is now complete
	return RES_OK;
}


#endif // BASE64_SUPPORT

