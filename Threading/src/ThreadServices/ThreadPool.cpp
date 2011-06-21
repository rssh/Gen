
#ifndef __GradSoft_ThreadServices_ThreadPool_h
#include <GradSoft/ThreadServices/ThreadPool.h>
#endif

/*
 * GradSoft C++ ToolBox: Threading
 * (C) GradSoft 2001
 * (C) Ruslan Shevchenko 2001
 * http://www.gradsoft.com.ua
 * $Id: ThreadPool.cpp,v 1.10 2004-06-12 10:53:01 rin Exp $
 */


namespace GradSoft {

//
//  ThreadServiceQueue    queue_;
//  ThreadPoolElement*    threads_;
//  int                   nThreads_;
//  Mode                  mode_;
//  long                  timeout_;
//


void ThreadPool::ThreadPoolElement::run()
{
 try {
  while(owner_->is_active()) {
   Runnable* current = owner_->queue_.get();
   current->run();
   yield();
  }
 }catch(const QueueOfRunnables::Destructed& /*ex*/) {
   //we are in destructor, do nothing
 }
}

void ThreadPool::ThreadPoolElement::set_owner(ThreadPool* owner)
{
 owner_=owner;
}

ThreadPool::ThreadPool(int nThreads, int queueSize,  Mode mode, long timeout)
    :queue_(queueSize),
     threads_(NULL),
     nThreads_(nThreads),
     mode_(mode),
     timeout_(timeout)
{} 


void ThreadPool::process(Runnable* runnable)
{
  if (!is_active()) throw NotActive();
  switch(mode_){
     case Blocked:
            queue_.put(runnable);
            break;
     case Checked: 
            if (!queue_.try_put(runnable)) {
              throw ThreadingExceptions::TemporaryNoResources();
            }
            break;
     case CheckedWithTimeout:
            if (!queue_.put_with_timeout(runnable,timeout_)){
              throw ThreadingExceptions::TemporaryNoResources();
            }
            break;
      default:
            throw ThreadingExceptions::InternalError();
   }
}

void ThreadPool::activate()
{
 if (threads_ == NULL) {

 //#ifdef HAVE_NO_ARRAY_INITIALIZER
    threads_ = new ThreadPoolElement[nThreads_];
    for (int j=0; j<nThreads_; ++j) threads_[j].set_owner(this);
 //#else
 //   threads_ = new ThreadPoolElement[nThreads_](this);
 //#endif
 }
 for(int i=0; i<nThreads_; ++i) {
    if (!threads_[i].is_running()) threads_[i].start();
 }
 ThreadService::activate();
} 

void ThreadPool::deactivate(bool shutdown)
{
 ThreadService::deactivate(shutdown);
 if (shutdown) {   
      delete[] threads_;
      threads_ = NULL;
 }
}

}

