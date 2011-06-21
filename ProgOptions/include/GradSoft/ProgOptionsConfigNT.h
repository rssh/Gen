#ifndef __GradSoft_ProgOptionsConfigNT_h
#define __GradSoft_ProgOptionsConfigNT_h

/*
 * part of GradSoft C++ ToolBox: Logger
 * (C) GradSoft, 2000, 2001
 * author: Ruslan Shevchenko <Ruslan@Shevchenko.Kiev.UA>, 
 * 2000, 2001
 * $Id: ProgOptionsConfigNT.h,v 1.5 2003-04-05 11:09:32 kav Exp $
 */

#ifdef _MSC_VER

#ifdef IOS_HAVE_NOCREATE 
#undef IOS_HAVE_NOCREATE // MCVS 6, 7 does not seem to have nocreate 
#endif

#ifndef IFSTREAM_HAVE_ISOPEN
#define IFSTREAM_HAVE_ISOPEN
#endif

#else
#error "Only MSVC on Win 32 is supported"
#endif

#endif 
