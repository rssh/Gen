
#include <HelloInterface.h>
#include <iostream>

class Hello2: public HelloInterface
{
public:

  Hello2(const char* name)
   :name_(name)
  {}

  virtual ~Hello2() {}

  virtual void hello() 
   { std::cerr << "Hi! I'm Hello2, name was " << name_ << std::endl; }

private:

  std::string name_;

};

class Hello2Module: public HelloModule
{
public:

  const char* name()    const { return "Hello2"; }
  int   versionMajor()  const { return 1; }
  int   versionMinor()  const { return 0; }
  int   versionSubMinor()  const { return 0; }
  const char* author()  const { return "Grad-Soft LTD"; }

  // Hello
  HelloInterface* create(const char* args)
   { return new Hello2(args); }

};

Hello2Module hello2ModuleTag;

EXPORT_OBJECT(hello2ModuleTag)
