#ifndef __GradSoft_DynamicModules_h
#define __GradSoft_DynamicModules_h

/*
 * DynamicModules implementation for GradSoft C++ ToolBox
 * (C) GradSoft 2001
 * http://www.gradsoft.com.ua
 * $Id: DynamicModules.h,v 1.11 2003-03-25 18:41:56 rin Exp $
 */

#ifndef WIN32
#define EXPORT_OBJECT(x)
#else
#include <windows.h>
#define EXPORT_OBJECT(x) extern "C" __declspec(dllexport) GradSoft::DynamicModule* __getDynamicModule() { return &x; };
#endif

#include <string>
#include <functional>
#include <map>
#include <stdexcept>

namespace GradSoft
{

/**
 * interface, which must implement DynamicModule
 * (we load modules by name)
 **/
class DynamicModule
{
 public:

    /**
     * must return name of the module
     **/
    virtual  const char* name() const = 0;

    /**
     * must return the author of the module
     **/
    virtual  const char* author() const = 0;

    /**
     * must return major version of the module 
     **/
    virtual  int   versionMajor() const = 0;

    /**
     * must return minor version of the module
     **/
    virtual  int   versionMinor() const = 0;
    
    /**
     * must return sub-minor version of the module.
     **/
    virtual  int   versionSubMinor() const = 0;

};


/**
 * Program interface to Dynamic Module Manager
 **/
class DynamicModules
{
public:

    ///
    struct Error : public std::runtime_error
    {
      Error(const char* message)
       :std::runtime_error(std::string(message)) {}
      Error(const std::string& message)
       :std::runtime_error(message) {}
    };


public:

    class Enumerator;
    friend class DynamicModules::Enumerator;

public:

    ///
    static   DynamicModule&  load(const char* name, const char* path);
    ///
    static   void            unload(const char* name);

    ///
    static   DynamicModule&  getModule(const char* name, const char* path);

    ///
    static   Enumerator      createEnumerator();

private:

    struct Entry {
#ifdef WIN32
      HINSTANCE descriptor;
#else
      void*           descriptor;
#endif
      DynamicModule*  module_p;

      Entry() {}
#ifdef WIN32
      Entry(HINSTANCE& d, DynamicModule* m)
#else
      Entry(void* d, DynamicModule* m)
#endif
       :descriptor(d),module_p(m) {}
    };

    typedef  std::map<std::string,Entry,std::less<std::string> > StorageType;
    typedef  StorageType::iterator                  StorageIteratorType;

    StorageType  dlMap_;

    // singleton infrastructure.

    static DynamicModules singleton_;
    static DynamicModules& singleton() { return singleton_; }
    static StorageType& dlMap() { return singleton().dlMap_; }

public:
    /**
     * enumerator over dynamic modules.
     **/
    class Enumerator
    {
     public:
      //constructor.
      Enumerator();
      /// give us current dynamic module and move to next state
      /// at the end of all dynamic modules return last.
      DynamicModule&  next();
      /// true, if it is safe to call next.
      bool            more() const;
     private:
      friend  class DynamicModules;
      StorageIteratorType it_;
    };


    DynamicModules():dlMap_(){}

private:

    DynamicModules(const DynamicModules&);
    DynamicModules& operator=(const DynamicModules&);

};

/**
 * Simple concrete tag.
 *Usage:
 * DynamicModuleTag("ModuleName","MyCompany",1,0,0);
 **/
class DynamicModuleTag: public DynamicModule
{
  DynamicModuleTag(const char* name, const char* author,
                      int vMajor, int vMinor, int vSubMinor)
    :name_(name),author_(author),
     vMajor_(vMajor),vMinor_(vMinor),vSubMinor_(vSubMinor)
  {}
private:
  const char* name_;
  const char* author_;
  int vMajor_;
  int vMinor_;
  int vSubMinor_;
public:

  const char* name() const { return name_; }
  const char* author() const { return author_; }
  int   versionMajor() const { return vMajor_; }
  int   versionMinor() const { return vMinor_; }
  int   versionSubMinor() const { return vSubMinor_; }

};


}

#endif
