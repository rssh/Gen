#ifndef __GradSoft_LoggerConfigNT_h
#define __GradSoft_LoggerConfigNT_h

/*
 * part of GradSoft C++ ToolBox: Logger
 * (C) GradSoft, 2000, 2001
 * author: Ruslan Shevchenko <Ruslan@Shevchenko.Kiev.UA>, 
 * 2000, 2001
 * $Id: LoggerConfigNT.h,v 1.5 2002-01-17 16:22:19 kav Exp $
 */

#ifdef _MSC_VER

#ifndef STREAMBUF_HAVE_PUBSEEKPOS
#define STREAMBUF_HAVE_PUBSEEKPOS
#endif

#ifndef HAVE_CTIME
#define HAVE_CTIME
#endif

#else
#error "Only MSVC on Win 32 is supported"
#endif

#endif
