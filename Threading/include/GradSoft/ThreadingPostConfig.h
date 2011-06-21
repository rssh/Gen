#ifndef __GradSoft_ThreadingPostConfig_h
#define __GradSoft_ThreadingPostConfig_h

/*
 * part of GradSoft C++ ToolBox: Threading.
 * (C) GradSoft 2000, 2001
 * http://www.gradsoft.com.ua
 * $Id: ThreadingPostConfig.h,v 1.7 2004-03-13 09:53:25 rssh Exp $
 */

#ifdef WIN32
# include <GradSoft/ThreadingConfigNT.h>
# if defined(SHARED_THREADINFG)
#  if defined(COMPILE_THREADING) 
#   define __dll_threading  __declspec(dllexport)
#  else
#   define __dll_threading  __declspec(dllimport)
#  endif
# else
#  define __dll_threading
# endif
#else
# include <GradSoft/ThreadingConfig.h>
# define __dll_threading
#endif

#ifndef _POSIX_PTHREAD_SEMANTICS
//for Sun Solaris
#define _POSIX_PTHREAD_SEMANTICS
#endif

#ifndef _REENTRANT
#define _REENTRANT
#endif

#ifdef __linux__
/* #define _XOPEN_SOURCE 500 */
 // bug or feature (?) in Linux system headers.
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 
#endif
#ifndef __USE_UNIX98
#define __USE_UNIX98 1
#endif
#endif

#ifndef HAVE_NO_ARRAY_INITIALIZER
#ifdef __SUNPRO_CC
# if __SUNPRO_CC <= 0x700
#  define HAVE_NO_ARRAY_INITIALIZER
# endif
#endif
#endif



#endif
