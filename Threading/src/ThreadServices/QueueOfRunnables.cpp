#ifndef __GradSoft_ThreadServices_QueueOfRunnables_h
#include <GradSoft/ThreadServices/QueueOfRunnables.h>
#endif

#include <assert.h>

#include <memory>

namespace GradSoft {

//
//private variables:
//
//  Runnable* arrived_;
//  Mutex_    arrivedMutex_;
//  ThreadEvent  oneElementAppear_;
//  ThreadEvent  freePlaceAppear_;
//  int       head_;
//  int       tail_;
//  int       nElements_;
//  int       arrivedSize_;
//  bool      destructed_;
//


QueueOfRunnables::QueueOfRunnables(int maxSize)
  :arrived_(NULL),
   arrivedMutex_(),
   head_(0),
   tail_(0),
   nElements_(0),
   arrivedSize_(maxSize),
   destructed_(false)
{
#ifdef __SUNPRO_CC
# if __SUNPRO_CC <= 0x420
#  define HAVE_NO_BAD_ALLOC
# endif
#else
# if defined(WIN32) 
#  define HAVE_NO_BAD_ALLOC
# endif
#endif
#if defined(HAVE_NO_BAD_ALLOC)
 arrived_ = new Runnable*[maxSize];
 if (arrived_==NULL) throw ThreadingExceptions::NoMemory();
#else
 try {
   arrived_ = new Runnable*[maxSize];
 }catch(const std::bad_alloc& ex){
   throw ThreadingExceptions::NoMemory();
 }
#endif
}

QueueOfRunnables::~QueueOfRunnables()
{
 // wait, when we in clean state;
 {
  MutexLocker ml(arrivedMutex_);
  destructed_ = true;
  // notify all getters about destruction.
  oneElementAppear_.notifyAll();
 }
 // wait until all threads will be exiting. 
 // TODO: think to refine in non-sleep termins
 Thread::nanosleepCurrent(0,1000);
}

void QueueOfRunnables::real_put_(Runnable* runnable) 
{
 assert(nElements_<arrivedSize_);
 arrived_[tail_++]=runnable;
 tail_%=arrivedSize_;
 ++nElements_;
 if (nElements_ == 1)
     oneElementAppear_.notify();
}

void QueueOfRunnables::put(Runnable* runnable)
{
 assert(runnable!=NULL);
 while(full()) {
   freePlaceAppear_.wait();
 }
 MutexLocker guard(arrivedMutex_);
 real_put_(runnable);
}

bool QueueOfRunnables::try_put(Runnable* runnable)
{
 assert(runnable!=NULL);
 MutexLocker guard(arrivedMutex_);
 if (full_()) {
   return false;
 }
 real_put_(runnable);
 return true;
}

bool QueueOfRunnables::put_with_timeout(Runnable* runnable, long timeout)
{
 assert(runnable!=NULL);
 if (full()) {
   freePlaceAppear_.wait(timeout);
 }
 if (full()) {
   return false;
 }
 MutexLocker guard(arrivedMutex_);
 real_put_(runnable);
 return true;
}

Runnable* QueueOfRunnables::real_get_()
{
 Runnable* retval = arrived_[head_];
 ++head_;
 head_%=arrivedSize_;
 --nElements_;
 if (nElements_ == arrivedSize_ - 1)
   freePlaceAppear_.notify();
 return retval;
}

Runnable* QueueOfRunnables::get()
{
 while(empty() && !destructed_) {
    oneElementAppear_.wait();
 }
 if (destructed_) {
    throw Destructed();
 }
 MutexLocker guard(arrivedMutex_);
 return real_get_();
}
 
Runnable* QueueOfRunnables::try_get()
{
 MutexLocker guard(arrivedMutex_);
 return  empty_() ? NULL : real_get_();
}

Runnable* QueueOfRunnables::get_with_timeout(long timeout)
{
 if(empty() && !destructed_) {
    oneElementAppear_.wait(timeout);
 }
 if (destructed_) {
   throw Destructed();
 }
 MutexLocker guard(arrivedMutex_);
 return  empty_() ? NULL : real_get_();
}

}

