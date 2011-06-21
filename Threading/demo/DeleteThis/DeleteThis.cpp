#include <GradSoft/Threading.h> 

// Demonstrates a heir of Thread may delete itself
// NB: This program works over the time of 6 minutes 

#include <iostream>

using namespace std;
using namespace GradSoft;

class Suicide: public Thread
{
 int number_;
 char*  str_;

public:

 Suicide(int n)
  :number_(n)
 {
   cerr << "instance "<< n <<" of Suicide is created " << endl;
 }

 virtual ~Suicide()
 {
   delete[] str_;
   cerr << "instance "<< number_ <<" of Suicide is being deleted" << endl;
 }

 void run()
 {
   cerr << "run:" << number_ << endl;
   str_ = new char[222222];
   nanosleep(0,number_%234567);
   delete this;
 }

};

class ThreadGenerator: public Thread
{
  bool work_;
public:
  ThreadGenerator():work_(true){}
  void run();
  void deactivate();

};

void ThreadGenerator::run()
{
 long i=0;
 while(work_) { 
   Suicide* suicide_ptr;
   try {
     suicide_ptr = new Suicide(i);
     suicide_ptr->start();
     cerr << "instance "<< i <<" of Suicide is started"<< endl;
   }catch(ThreadingExceptions::TemporaryNoResources){
     delete suicide_ptr; // ignore exception
     cerr << "temporal absence of resources for starting Suicide instance (ignored)" << endl;
   }
   ++i;
   nanosleep(0,i%567890);
 }
}

void ThreadGenerator::deactivate()
{
  work_ = false;
}

int main(int argc, char** argv)
{
 ThreadGenerator generator;
 generator.start();
 Thread::sleepCurrent(300);
 generator.deactivate();
 while(generator.is_running()) Thread::sleepCurrent(1);
 return 0;
}

