#include <GradSoft/ptrs.h>
#include <iostream>
#include <utility>


using namespace GradSoft;

int main(int argc, char** argv)
{

 owned_ptr<std::pair<int,int>,ptr::safe>  x(new std::pair<int,int>(5,5),true);
 owned_ptr<std::pair<int,int>,ptr::safe>  y(new std::pair<int,int>(4,4),true);
 owned_ptr<std::pair<int,int>,ptr::unsafe>  z(new std::pair<int,int>(4,4),true);

 x.set(y,true);
 y.release();

 try {
  std::cout << y->first << std::endl;
 }catch(const NullPointerException& ex){
  std::cout << "Null pointer exception handled" << std::endl;
 }

 try {
  std::cout << x->first << std::endl;
 }catch(const NullPointerException& ex){
  std::cout << "Null pointer exception handled" << std::endl;
 }

 return 0;

}
