#ifndef _EMP_HDR_H_
#define _EMP_HDR_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <signal.h>
#include <errno.h>
#include <dirent.h>


#ifdef TRUE
#undef TURE
#endif /*TRUE*/

#ifdef FALSE
#undef FALSE
#endif /*FALSE*/

typedef enum {FALSE, TRUE} Boolean;

#define min(m, n) ((m) < (n) ? (m) : (n))
#define max(m, n) ((m) > (n) ? (m) : (n))

#define PI 	3.14159265358
#define HPI	1.57079632679	// Half PI 
#define DPI	6.28318530716	// Double PI

#if defined(__sgi)
typedef int socklen_t;
#endif

#if defined(__sun)
#include <sys/file.h>
#endif

#if ! defined(O_ASYNC) && defined(FASYNC)
#define O_ASYNC FASYNC
#endif

#if ! defined(MAP_ANON) && ! defined(MAP_ANONYMOUS)
#define MAP_ANONYMOUS MAP_ANON
#endif

#if ! defined(O_SYNC) && defined(O_FSYNC)
#define O_SYNC O_FSYNC
#endif

#if defined(__FreeBSD__)
#define sival_int sigval_int
#define sival_ptr sigval_ptr
#endif

#endif /*_EMP_HDR_H_*/
