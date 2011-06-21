
#include <HelloInterface.h>
#include <memory>
#include <iostream>
#include <typeinfo>

using namespace GradSoft;

int main()
{
 try {

  DynamicModule& dmHello1 = DynamicModules::load("hello1ModuleTag","./Hello1");
  DynamicModule& dmHello2 = DynamicModules::load("hello2ModuleTag","./Hello2");

  std::string s1=typeid(dmHello1).name();
  std::string s2=typeid(dmHello2).name();
  
  if (s1.find("Hello1Module")==std::string::npos || s2.find("Hello2Module")==std::string::npos) {
    throw std::bad_typeid();
  }

  HelloModule& hello1Module = static_cast<HelloModule&>(dmHello1);
  HelloModule& hello2Module = static_cast<HelloModule&>(dmHello2);
  
  {
   std::auto_ptr<HelloInterface> h1 ( hello1Module.create("xxx") );
   std::auto_ptr<HelloInterface> h2 ( hello2Module.create("xxx") );

   h1->hello();
   h2->hello();
  }

  DynamicModules::unload("hello1ModuleTag");
  DynamicModules::unload("hello2ModuleTag");

 }catch(const DynamicModules::Error& ex){
  
  std::cerr << "Error during loading DynamicModule" << std::endl;
  std::cerr << ex.what() << std::endl;
 } catch (std::bad_typeid&) {
  std::cerr << "Wrong type in dynamic module" << std::endl;
 }

 return 0;
}
