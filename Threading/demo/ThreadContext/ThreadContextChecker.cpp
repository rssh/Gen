/*
 * part of GradSoft C++ ToolBox.
 * illustrate usage of ThreadContext
 */

#include <GradSoft/Threading.h>
#include <GradSoft/ThreadServices/SingleThreadBlocking.h>
#include <GradSoft/ThreadServices/ThreadPerClient.h>

#include <iostream>
#include <string>


#ifndef WIN32
#include <pthread.h>
#endif

using namespace GradSoft;
using namespace std;

class MyThreadContextSlot: public ThreadContext::Slot
{
public:

  MyThreadContextSlot(const char* info)
    :info_(info) 
  { cerr << "ThreadContextSlot::ThreadContextSlot" << endl; }

  ~MyThreadContextSlot()
  { cerr << "ThreadContextSlot::~ThreadContextSlot" << endl; }

  const char*  getInfo() const 
    { return info_.c_str(); }

  static MyThreadContextSlot* getOrCreate(ThreadContext* ctx, const char* info)
  {
   ThreadContext::Slot* slot=ctx->getSlot(myIndex_);
   MyThreadContextSlot* retval=dynamic_cast<MyThreadContextSlot*>(slot);
   if (slot==NULL) {
      retval=new MyThreadContextSlot(info);
      ctx->allocSlot(retval,myIndex_);
   }
   return retval;
  }
 

private:

  static int myIndex_;
  string info_;

};

int MyThreadContextSlot::myIndex_ = 0;

class TestRunnable: public Runnable
{
public:

  void  check();

  void run()
  {
   check();
  };

};

void  TestRunnable::check()
{
 MyThreadContextSlot* slot = MyThreadContextSlot::getOrCreate(
                              ThreadContext::current(),"from Runnable::check");
 if (slot!=NULL) {
   cerr << "slot info: " << slot->getInfo() << endl;
 }else{
   cerr << "Impossible: invalid slot type " << endl;
 }
}



int main(int argc, char** argv)
{
  ThreadService* s1=new  SingleThreadBlocking();
  ThreadService* s2=new  ThreadPerClient();
   MyThreadContextSlot* slot = MyThreadContextSlot::getOrCreate(
                                         ThreadContext::current(),
                                         "from main");
  TestRunnable test;

  s1->activate();
  s2->activate();
  cerr << 1 << endl;
  s1->process(&test);
  Thread::sleepCurrent(1);
  cerr << 2 << endl;
  s2->process(&test);
  Thread::sleepCurrent(1);
  cerr << 3 << endl;
  s1->process(&test);
  Thread::sleepCurrent(1);
  cerr << 4 << endl;
  s2->process(&test);
  Thread::sleepCurrent(1);

  return 0;
}
