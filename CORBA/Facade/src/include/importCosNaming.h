#ifndef __importCosNaming_h
#define __importCosNaming_h

#ifndef __importCORBA_h
#include <importCORBA.h>
#endif

#ifndef _MSC_VER
#ifdef COSNAMING_H
#include COSNAMING_H
#else
#error COSNAMING_H undefinig: some strange with configure
#endif
#else
 // for ORBACUS only.
#ifndef __CosNaming_h
#include <OB/CosNaming.h>
#endif
#endif

#ifdef CORBA_MODULE_NAMESPACE_MAPPING
using namespace CosNaming;
#elif defined(CORBA_MODULE_CLASS_MAPPING)

typedef CosNaming::NamingContext  NamingContext;
typedef CosNaming::NamingContext_ptr  NamingContext_ptr;
typedef CosNaming::NamingContext_var  NamingContext_var;

typedef CosNaming::Name Name;
typedef CosNaming::Name_var Name_var;

#else

typedef CosNaming_NamingContext NamingContext;
typedef CosNaming_NamingContext_ptr NamingContext_ptr;
typedef CosNaming_NamingContext_var NamingContext_var;

typedef CosNaming_Name Name;
typedef CosNaming_Name_var Name_var;

#endif
//NAMESPACE_MAPPING


#endif
