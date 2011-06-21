#ifndef __ServiceOptionsDemoPostConfig_h
#define __ServiceOptionsDemoPostConfig_h

/*
 * ServiceOptionsDemoPostConfig.h
 * part of implementation of CORBA query service.
 * (C) Ruslan Shevchenko <Ruslan@Shevchenko.Kiev.UA>, 1998,1999
 * $Id: ServiceOptionsDemoPostConfig.h,v 1.4 2008-06-12 10:52:16 rssh Exp $
 */

#ifdef WIN32
#include <ServiceOptionsDemoConfig.nt.h>
#elif defined(HAVE_CONFIG_H)
#include <ServiceOptionsDemoConfig.h>
#else
#error : please, run autoconf
#endif

#ifdef HAVE_TYPENAME
#define TYPENAME typename
#else
#define TYPENAME
#endif


#ifndef __CAT1_PP
#define __CAT1_PP(x) x
#endif

#ifndef __CAT2_PP
#define __CAT2_PP(x,y) x##y
#endif

#ifndef __CAT3_DOT_FF
#define __CAT3_DOT_FF(x,y,z)  <__CAT2_PP(x,y).__CAT1_PP(z)>
#endif

#ifndef __CAT3_DOT_F
#define __CAT3_DOT_F(x,y,z)  __CAT3_DOT_FF(x,y,z)
#endif

#define CORBA_STUB_HEADER(x) __CAT3_DOT_F(x,IDL_CLN_H_BEFOREDOT_SUFFIX,IDL_CLN_H_EXT)
#define CORBA_SKELETON_HEADER(x) __CAT3_DOT_F(x,IDL_SRV_H_BEFOREDOT_SUFFIX,IDL_SRV_H_EXT)


#endif
