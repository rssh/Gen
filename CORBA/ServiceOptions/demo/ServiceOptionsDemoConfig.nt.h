#ifdef WIN32

/*
 * ORB-related setings: 
 *
 */

#ifdef _MSC_VER

#ifdef ORBACUS

#ifndef CORBA_H
#define CORBA_H <OB/CORBA.h>
#endif

#ifndef COSNAMING_H
#define COSNAMING_H <OB/CosNaming.h>
#endif

#ifndef CORBA_MODULE_NAMESPACE_MAPPING
#define CORBA_MODULE_NAMESPACE_MAPPING 1
#endif

#ifndef CORBA_HAVE_POA 
#define CORBA_HAVE_POA 1
#endif

#ifndef CORBA_SYSTEM_EXCEPTION_IS_STREAMBLE
#define CORBA_SYSTEM_EXCEPTION_IS_STREAMBLE
#endif

#ifndef IDL_CLN_H_SUFFIX
#define IDL_CLN_H_SUFFIX .h
#endif

#ifndef IDL_SRV_H_SUFFIX
#define IDL_SRV_H_SUFFIX _skel.h
#endif

#elif defined(OMNIORB)

#ifndef CORBA_H
#define CORBA_H <omniORB3/CORBA.h>
#endif

#ifndef COSNAMING_H
#define COSNAMING_H <omniORB3/Naming.hh>
#endif

#ifndef CORBA_MODULE_NAMESPACE_MAPPING
#define CORBA_MODULE_NAMESPACE_MAPPING 1
#endif

#ifndef CORBA_HAVE_POA
#define CORBA_HAVE_POA 1
#endif

#ifndef CORBA_ORB_INIT_HAVE_3_ARGS
#define CORBA_ORB_INIT_HAVE_3_ARGS 1
#endif

#ifndef CORBA_ORB_INIT_THIRD_ARG
#define CORBA_ORB_INIT_THIRD_ARG "omniORB3"
#endif

# ifdef CORBA_SYSTEM_EXCEPTION_IS_STREAMBLE
# undef CORBA_SYSTEM_EXCEPTION_IS_STREAMBLE
#endif

#ifndef IDL_CLN_H_SUFFIX
#define IDL_CLN_H_SUFFIX .hh
#endif

#ifndef IDL_SRV_H_SUFFIX
#define IDL_SRV_H_SUFFIX .hh
#endif

#elif defined(RSSH_TAO)

#ifndef CORBA_H
#define CORBA_H <tao/corba.h>
#endif

#ifndef COSNAMING_H
#define COSNAMING_H <orbsvcs/CosNamingC.h>
#endif

#ifndef CORBA_MODULE_NAMESPACE_MAPPING
#define CORBA_MODULE_NAMESPACE_MAPPING 1
#endif

#ifndef CORBA_HAVE_POA
#define CORBA_HAVE_POA 1
#endif

#ifndef CORBA_SYSTEM_EXCEPTION_IS_STREAMBLE
#define CORBA_SYSTEM_EXCEPTION_IS_STREAMBLE
#endif

#ifndef IDL_CLN_H_SUFFIX
#define  IDL_CLN_H_SUFFIX C.h
#endif

#ifndef IDL_SRV_H_SUFFIX
#define  IDL_SRV_H_SUFFIX S.h
#endif

#elif defined(MICO)

#ifndef CORBA_H
#define CORBA_H <CORBA.h>
#endif

#ifndef COSNAMING_H
#define COSNAMING_H <coss/CosNaming.h>
#endif

#ifndef CORBA_MODULE_NAMESPACE_MAPPING
#define CORBA_MODULE_NAMESPACE_MAPPING 1
#endif

#ifndef CORBA_HAVE_POA
#define CORBA_HAVE_POA 1
#endif


# ifdef CORBA_SYSTEM_EXCEPTION_IS_STREAMBLE
# undef CORBA_SYSTEM_EXCEPTION_IS_STREAMBLE
#endif

#ifndef IDL_CLN_H_SUFFIX
#define IDL_CLN_H_SUFFIX .h
#endif

#ifndef IDL_SRV_H_SUFFIX
#define IDL_SRV_H_SUFFIX .h
#endif


#else
#error ORB model is not defined 
#endif

#endif

/*
 * Compiler & libraries 
 *
 */

#ifdef _MSC_VER

#ifndef STREAMBUF_HAVE_PUBSEEKPOS
#define STREAMBUF_HAVE_PUBSEEKPOS 1
#endif

#endif
#endif