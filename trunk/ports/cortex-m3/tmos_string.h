/*
 * tmos_string.h
 *
 *  Created on: 10.11.2010
 *      Author: Miroslav Kostadinov
 */

#ifndef TMOS_STRING_H_
#define TMOS_STRING_H_

#ifdef __cplusplus
 extern "C" {
#endif

size_t strlen(const char *dst);
void memclr(void *dst, size_t length);
void* memcpy(void *dst, const void *src, size_t length);
void* memmove(void *dst, const void *src, size_t length);
void* memset(void *dst, int c, size_t length);
char* strcpy(char *dst, const char *src);
char* strncpy(char *dst, const char *src, size_t len);
char* strchr(const char *dst, int i);
char* strrchr(const char *dst, int i);
const char* strpbrk(const char * str1, const char * str2);
int memcmp(const void * ptr1, const void * ptr2, size_t num);
int strcmp(const void *dst, const void *src);
int strncmp(const void *dst, const void *src, size_t len);
int strcasecmp(const char *dst, const char *src);
int strncasecmp(const char *dst, const char *src, size_t len);

extern char G_char_2_type_[256];
int islower(unsigned int c);
int isupper(unsigned int c);
int tolower(int c);
int toupper(int c);

#define	FLAG_IS_UPPER			0x80
#define FLAG_IS_LOWER			0x01
#define	FLAG_IS_NUMBER			0x04
#define	FLAG_IS_SPACE			0x08
#define FLAG_IS_PUNC			0x10
#define FLAG_IS_CONTROL			0x20
#define FLAG_IS_HEX				0x40
#define	FLAG_IS_BLANK			0x02
#define FLAG_IS_ALPHA			(FLAG_IS_UPPER | FLAG_IS_LOWER)
#define FLAG_IS_ALPHANUM		(FLAG_IS_ALPHA | FLAG_IS_NUMBER)
#define IS_LOWER(c) 	(G_char_2_type_[(unsigned)c] & FLAG_IS_LOWER)
#define IS_UPPER(c) 	(G_char_2_type_[(unsigned)c] & FLAG_IS_UPPER)
#define IS_ALPHA(c) 	(G_char_2_type_[(unsigned)c] & FLAG_IS_ALPHA)
#define IS_ALPHANUM(c) 	(G_char_2_type_[(unsigned)c] & FLAG_IS_ALPHANUM)
#define IS_DIGIT(c) 	(G_char_2_type_[(unsigned)c] & FLAG_IS_NUMBER)
#define IS_XDIGIT(c) 	(G_char_2_type_[(unsigned)c] & FLAG_IS_HEX)

#define XDIGIT_TO_NUM(h) ((h) < 'A' ? (h) - '0' : (h) + 10 - ((h)<'a' ? 'A' : 'a'))
#define X2DIGITS_TO_NUM(h1, h2) ((XDIGIT_TO_NUM (h1) << 4) + XDIGIT_TO_NUM (h2))
unsigned int XNUM_TO_DIGIT(unsigned int c);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	language strings
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define D_LL(name, l1, l2)    const char name[] = l1 "\0" l2 "\0"
#define LL(l1, l2) l1 "\0" l2 "\0"

#ifdef __cplusplus
}
#endif

#endif /* TMOS_STRING_H_ */
