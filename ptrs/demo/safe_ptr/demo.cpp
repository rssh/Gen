#include <GradSoft/ptrs.h>
#include <iostream>

using namespace GradSoft;

class X
{
private:
  int x_;
public:
  X(int x=0):x_(x){};

  void print(void)
  { std::cout << x_ << std::endl; }

  void inc(void)
  { ++x_; }

  bool isZero(void)
  { return x_==0; }

};

int main(int argc, char** argv)
{

 safe_ptr<X>  x;
 safe_ptr<int>  ix;

 try {
  std::cout << *ix << std::endl;
 }catch(const NullPointerException& ex){
  std::cout << "Null pointer exception handled" << std::endl;
 }

 void (X::*method)() = &X::print;

 try {
   (x->*method)();
 }catch(const NullPointerException& ex){
   std::cout << "Null pointer exception handled" << std::endl;
 }

 try {
  x=new X();
  try {
   (x->*method)();
  }catch(const NullPointerException& ex){
   std::cout << "Null pointer exception handled" << std::endl;
  }

  bool (X::*method1)() = &X::isZero;
  std::cout << (x->*method1)() << std::endl;

  delete x.get();
 }catch(const std::exception& ex){
  std::cerr << ex.what() << std::endl;
 }

 safe_auto_ptr<X> x1(new X());

 x1->print();
 x1->inc();

 x1.release();

 try {
  x1->print();
 }catch(const std::exception& ex){
  std::cerr << ex.what() << std::endl;
 }

 return 0;

}
