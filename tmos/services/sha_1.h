//Copyright (C) 1998, 2009
//Paul E. Jones <paulej@packetizer.com>
//
//Freeware Public License (FPL)
//
//This software is licensed as "freeware."  Permission to distribute
//this software in source and binary forms, including incorporation
//into other products, is hereby granted without a fee.  THIS SOFTWARE
//IS PROVIDED 'AS IS' AND WITHOUT ANY EXPRESSED OR IMPLIED WARRANTIES,
//INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
//AND FITNESS FOR A PARTICULAR PURPOSE.  THE AUTHOR SHALL NOT BE HELD
//LIABLE FOR ANY DAMAGES RESULTING FROM THE USE OF THIS SOFTWARE, EITHER
//DIRECTLY OR INDIRECTLY, INCLUDING, BUT NOT LIMITED TO, LOSS OF DATA
//OR DATA BEING RENDERED INACCURATE.

/*
 *  sha1.h
 *
 *  Copyright (C) 1998, 2009
 *  Paul E. Jones <paulej@packetizer.com>
 *  All Rights Reserved.
 *
 *****************************************************************************
 *  $Id: sha1.h 12 2009-06-22 19:34:25Z paulej $
 *****************************************************************************
 *
 *  Description:
 *      This class implements the Secure Hashing Standard as defined
 *      in FIPS PUB 180-1 published April 17, 1995.
 *
 *      Many of the variable names in this class, especially the single
 *      character names, were used because those were the names used
 *      in the publication.
 *
 *      Please read the file sha1.cpp for more information.
 *
 */


#ifndef SHA_1_H_
#define SHA_1_H_

#include <tmos.h>

class SHA1
{

    public:

        SHA1();
        virtual ~SHA1();

        /*
         *  Re-initialize the class
         */
        void Reset();

        /*
         *  Returns the message digest
         */
        bool Result(unsigned *message_digest_array);
        bool Result(CSTRING& message_digest);
        bool Result(char* message_digest);

        /*
         *  Provide input to SHA1
         */
        void Input( const unsigned char *message_array,
                    unsigned            length);
        void Input( const char  *message_array,
                    unsigned    length);
        void Input(const CSTRING& message);

    private:

        /*
         *  Process the next 512 bits of the message
         */
        void ProcessMessageBlock();

        /*
         *  Pads the current message block to 512 bits
         */
        void PadMessage();

        /*
         *  Performs a circular left shift operation
         */
        static inline unsigned CircularShift(int bits, unsigned word);

        unsigned H[5];                      // Message digest buffers

        unsigned Length_Low;                // Message length in bits
        unsigned Length_High;               // Message length in bits

        unsigned char Message_Block[64];    // 512-bit message blocks
        int Message_Block_Index;            // Index into message block array

        bool Computed;                      // Is the digest computed?
    protected:
        bool Corrupted;                     // Is the message digest corruped?
        
};

#endif /* SHA_1_H_ */
