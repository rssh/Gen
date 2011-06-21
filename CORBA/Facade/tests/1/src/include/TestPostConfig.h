#ifndef __TestPostConfig_h
#define __TestPostConfig_h

/*
 * ServiceOptionsDemoPostConfig.h
 * part of implementation of CORBA query service.
 * (C) Ruslan Shevchenko <Ruslan@Shevchenko.Kiev.UA>, 1998,1999
 * $Id: TestPostConfig.h,v 1.1 2000-08-02 09:07:22 alik Exp $
 */

#ifdef HAVE_CONFIG_H
# include <TestConfig.h>
#else
# ifndef _MSC_VER
#  error : please, run autoconf
# else
/* this is windows. */
#ifndef HAVE_NAMESPACES
#  define HAVE_NAMESPACES
#endif
# ifdef ORBACUS
#  define IDL_CLN_H_SUFFIX .h
#  define IDL_SRV_H_SUFFIX _skel.h
# endif
# endif
#endif

#ifdef HAVE_TYPENAME
#define TYPENAME typename
#else
#define TYPENAME
#endif

#ifndef _MSC_VER
#ifdef CORBA_H
#include CORBA_H
#endif
#endif
 
#ifndef __CAT2_FF
#define __CAT2_FF(x,y) <##x##y##>
#endif

#ifndef __CAT2_F
#define __CAT2_F(x,y) __CAT2_FF(x,y)
#endif

#define CORBA_STUB_HEADER(x)  __CAT2_F(x,IDL_CLN_H_SUFFIX)
#define CORBA_SKELETON_HEADER(x)  __CAT2_F(x,IDL_SRV_H_SUFFIX)

#ifndef _MSC_VER
#ifdef ORBACUS
#if (OB_INTEGER_VERSION < 3999951L)
#define HAVE_OB_UTIL_H
#define HAVE_OBPRINTEXCEPTION
#endif
#endif
#endif

#endif
