#ifndef __importCORBA_H
#define __importCORBA_H

/**
 * import CORBA definitions to program 'namespace'
 * when namespaces are not supported.
 */

#ifndef __TestPostConfig_h
#include <TestPostConfig.h>
#endif

#ifdef CORBA_H
#include CORBA_H
#endif

#ifdef _WIN32
#ifdef ORBACUS
# include <OB/CORBA.h>
# ifndef OB_INTEGER_VERSION
#  include <OB/ORB.h>
# endif
#else
# error "for now we can work only with orbacus"
#endif
#endif

#if defined(ORBACUS)
#if !defined(OB_INTEGER_VERSION) || (OB_INTEGER_VERSION < 3999951L)
#define CORBA_SEQ_HAVE_NO_REPLACE
#endif
#endif

#ifdef _MSC_VER
#ifdef ORBACUS
#if (OB_INTEGER_VERSION < 3999951L)
#define HAVE_OB_UTIL_H
#define HAVE_OBPRINTEXCEPTION
#endif
#endif
#endif

#if (ORBACUS) && (OB_INTEGER_VERSION >= 3999952L)
#define CORBA_HAVE_POA 1
#endif

#ifdef CORBA_MODULE_NAMESPACE_MAPPING
using namespace CORBA;

#elif defined (CORBA_MODULE_CLASS_MAPPING)

typedef CORBA::Long Long;
typedef CORBA::ULong       ULong;
typedef CORBA::Short       Short;
typedef CORBA::UShort      UShort;
typedef CORBA::Boolean     Boolean;
typedef CORBA::Octet       Octet;
typedef CORBA::Char        Char;
typedef CORBA::Float       Float;
typedef CORBA::Double      Double;
typedef CORBA::String_var  String_var;
typedef CORBA::Any         Any;
typedef CORBA::Any_var     Any_var;

typedef CORBA::Object      Object;
typedef CORBA::Object_ptr      Object_ptr;
typedef CORBA::Object_var      Object_var;


typedef CORBA::ORB         ORB;
typedef CORBA::ORB_ptr     ORB_ptr;
typedef CORBA::ORB_var     ORB_var;

#define ORB_init   CORBA::ORB_init

inline char* string_dup(const char* s) { return  CORBA::string_dup(s); }
#define string_alloc CORBA::string_alloc
#define string_free CORBA::string_free

inline CORBA::Boolean is_nil(CORBA::Object_ptr x)
 { return CORBA::is_nil(x); }


#else

#ifndef Long
# define Long        CORBA_Long
#endif

#define ULong       CORBA_ULong
#define Short       CORBA_Short
#define UShort      CORBA_UShort
#define Boolean     CORBA_Boolean
#define Octet       CORBA_Octet
#define Char        CORBA_Char
#define Float       CORBA_Float
#define Double      CORBA_Double
#ifndef TRUE
# define TRUE        CORBA_TRUE
#endif
#ifndef FALSE
# define FALSE       CORBA_FALSE
#endif
#define String      CORBA_String
#define String_var  CORBA_String_var
#define string_dup  CORBA_string_dup
#define string_alloc CORBA_string_alloc
#define string_free CORBA_string_free

#define Any     CORBA_Any
#define Any_var CORBA_Any_var

#define TypeCode     CORBA_TypeCode
#define TypeCode_ptr CORBA_TypeCode_ptr
#define TypeCode_var CORBA_TypeCode_var

#define Object       CORBA_Object
#define Object_ptr       CORBA_Object_ptr
#define Object_var       CORBA_Object_var

#define is_nil(x)    CORBA_is_nil(x)

#define tk_null      CORBA_tk_null
#define tk_void      CORBA_tk_void
#define tk_short      CORBA_tk_short
#define tk_long      CORBA_tk_long
#define tk_ushort      CORBA_tk_ushort
#define tk_ulong      CORBA_tk_ulong
#define tk_float      CORBA_tk_float
#define tk_double      CORBA_tk_double
#define tk_boolean      CORBA_tk_boolean
#define tk_char      CORBA_tk_char
#define tk_octet      CORBA_tk_octet
#define tk_any      CORBA_tk_any
#define tk_TypeCode      CORBA_tk_TypeCode
#define tk_Principal      CORBA_tk_Principal
#define tk_objref      CORBA_tk_objref
#define tk_struct      CORBA_tk_struct
#define tk_union      CORBA_tk_union
#define tk_enum      CORBA_tk_enum
#define tk_string      CORBA_tk_string
#define tk_sequence      CORBA_tk_sequence
#define tk_array      CORBA_tk_array
#define tk_alias     CORBA_tk_alias
#define tk_except     CORBA_tk_except


#ifndef ORB_Defined
typedef CORBA_ORB   ORB;
typedef CORBA_ORB_var   ORB_var;
typedef CORBA_ORB_ptr   ORB_ptr;
#define ORB_init   CORBA_ORB_init
#define ORB_Defined
#endif

#ifndef InterfaceDef_Defined
#define InterfaceDef CORBA_InterfaceDef
#define InterfaceDef_Defined
#endif

#ifndef ImplementationDef_Defined
#define ImplementationDef CORBA_ImplementationDef
#define ImplementationDef_ptr CORBA_ImplementationDef_ptr
#define ImplementationDef_var CORBA_ImplementationDef_var
#endif

#ifndef Container_Defined
#define Container  CORBA_Container
#define Container_var  CORBA_Container_var
#define Container_Defined
#endif

#ifndef BOA_Defined
#define BOA CORBA_BOA
#define BOA_ptr CORBA_BOA_ptr
#define BOA_var CORBA_BOA_var
#endif

#define SystemException CORBA_SystemException
#define BAD_OPERATION CORBA_BAD_OPERATION

#endif
//CORBA_MODULE_NAMESPACE_MAPPING

#endif
