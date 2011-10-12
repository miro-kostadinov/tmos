/*
 * tmos_stdio.h
 *
 *  Created on: 20.11.2010
 *      Author: Miroslav Kostadinov
 */

#ifndef TMOS_STDIO_H_
#define TMOS_STDIO_H_

#include <stdarg.h>

#ifdef __cplusplus
 extern "C" {
#endif

int tmos_sprintf(char *buf, const char *format, ...);
int tmos_vsprintf(char *buffer, const char *format, va_list arg);

int tmos_sscanf(const char *buf, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* TMOS_STDIO_H_ */
