#include <GradSoft/DynamicModules.h>

using namespace GradSoft;

class HelloInterface
{
public:


  virtual ~HelloInterface()  {}

  virtual void hello() = 0;

};

class HelloModule: public DynamicModule
{
public:

  virtual HelloInterface* create(const char* name) = 0;

};
