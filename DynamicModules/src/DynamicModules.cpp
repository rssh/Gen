#include <GradSoft/DynamicModules.h>

/*
 * (C) Grad-Soft Ltd, Kiev, Ukraine. 2001-2002
 * http://www.gradsoft.kiev.ua
 * $Id: DynamicModules.cpp,v 1.12 2002-04-05 20:25:47 rssh Exp $
 */

#ifdef WIN32
#include <windows.h>
#include <lmerr.h>
#include <algorithm>
#else
#include <dlfcn.h>
#endif

#include <iostream>

namespace GradSoft
{

#ifdef WIN32
typedef DynamicModule* (*__GET_DYNAMIC_MODULE)(void);

void fillErrorText(DWORD dwLastError, std::string& errorText)
{
    HMODULE hModule = NULL; // default to system source
    LPSTR MessageBuffer;
    DWORD dwBufferLength;
    DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM ;
    if (dwLastError >= NERR_BASE && dwLastError <= MAX_NERR) {
        hModule = LoadLibraryEx(TEXT("netmsg.dll"), NULL
                , LOAD_LIBRARY_AS_DATAFILE);
        if (hModule != NULL) dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
    }
    if (dwBufferLength = FormatMessageA(dwFormatFlags, hModule, dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &MessageBuffer, 0,
        NULL)) {
        errorText.assign(MessageBuffer, dwBufferLength);
        LocalFree(MessageBuffer);
    }
    if(hModule != NULL) FreeLibrary(hModule);
}

#endif

DynamicModule& DynamicModules::load(const char* name,const char* path)
                                          
{
  std::string sPath(path);
#ifdef WIN32
  sPath += ".dll";
  std::replace(sPath.begin(), sPath.end(), '/', '\\');
  HINSTANCE dlDescriptor = LoadLibrary(sPath.c_str());
  if (dlDescriptor == NULL) {
   std::string message;
   fillErrorText(GetLastError(), message);
   throw Error(message.c_str()); 
  }
  __GET_DYNAMIC_MODULE f = (__GET_DYNAMIC_MODULE)GetProcAddress(dlDescriptor, "__getDynamicModule");
  if (f == NULL) {
   throw Error("Dynamic modules object not exported"); 
  }
  DynamicModule* m = f();
#else
  sPath += ".so";
  void * dlDescriptor = dlopen(sPath.c_str(), RTLD_LAZY|RTLD_GLOBAL);
  if (dlDescriptor==NULL) {
   std::cerr << "dlDescriptor==NULL" << std::endl;
   const char* message = dlerror();
   std::cerr << message << std::endl;
   throw Error(message); 
  }
  void* sObject = dlsym(dlDescriptor,name);
  if (sObject==NULL) {
   std::cerr << "sObject==NULL" << std::endl;
   const char* message = dlerror();
   throw Error(message); 
  }
  DynamicModule* m = (DynamicModule*)sObject;
#endif
  std::string strName(name);
  Entry e(dlDescriptor,m);
  dlMap()[strName]=e;
  return *m;
}

void  DynamicModules::unload(const char* name)
{
  std::string strName(name);
  StorageType& dlmap = dlMap();
  StorageIteratorType it = dlmap.find(strName);
  if (it==dlmap.end()) {
    std::cerr << "Can't find loaded module with name " << name << std::endl;
    return;
  }
#ifdef WIN32
  FreeLibrary((*it).second.descriptor);
#else
  dlclose((*it).second.descriptor);
#endif
  dlmap.erase(it);
}

DynamicModule& DynamicModules::getModule(const char* name, const char* path)
{
  std::string strName(name);
  StorageType& dlmap = dlMap();
  StorageIteratorType it = dlmap.find(strName);
  if (it==dlmap.end()) {
    return load(name,path);
  }else{
    return *((*it).second.module_p); 
  }
}

DynamicModules::Enumerator::Enumerator()
{
 it_=DynamicModules::dlMap().begin();
}

DynamicModule& DynamicModules::Enumerator::next()
{
 if (it_==DynamicModules::dlMap().end()) 
  throw DynamicModules::Error("DynamicModules:Enumerator:OutOfRange");
 return *((*it_).second.module_p);
}

bool DynamicModules::Enumerator::more() const
{
 return (it_!=DynamicModules::dlMap().end());
}

DynamicModules::Enumerator DynamicModules::createEnumerator()
{
 Enumerator retval;
 retval.it_ = dlMap().begin();
 return retval;
}

DynamicModules DynamicModules::singleton_;

}
