
#include <HelloInterface.h>
#include <iostream>

class Hello1: public HelloInterface
{
public:

  Hello1(const char* x) {}

  virtual ~Hello1() {}

  virtual void hello() 
   { std::cerr << "Hi! I'm Hello1" << std::endl; }

};

class Hello1Module: public HelloModule
{
public:

  const char* name()    const { return "Hello1"; }
  int   versionMajor() const { return 1; }
  int   versionMinor() const { return 0; }
  int   versionSubMinor() const { return 0; }
  const char* author()  const { return "Grad-Soft LTD"; }

  // Hello1
  HelloInterface* create(const char* args)
   { return new Hello1(args); }

};

Hello1Module hello1ModuleTag;

EXPORT_OBJECT(hello1ModuleTag)
