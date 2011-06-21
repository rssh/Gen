
#ifndef __GradSoft_ThreadServices_SingleThreadChecking_h
#include <GradSoft/ThreadServices/SingleThreadChecking.h>
#endif

/*
 * part of GradSoft C++ ToolBox: Threading
 * (C) GradSoft 2001
 * http://www.gradsoft.com.ua
 * $Id: SingleThreadChecking.cpp,v 1.8 2001-10-31 03:13:52 rssh Exp $
 */


namespace GradSoft {

/**
 * Single thread service wich run in own thread and check
 * aviability of service before processing of runnable
 **/

//  Runnable*   current_;
//  Mutex       currentMutex_;
//  ThreadEvent runnableArrived_;


SingleThreadChecking::SingleThreadChecking()
     :
      ThreadService(),
      Thread(),
      current_(NULL),
      currentMutex_(),
      runnableArrived_()
{
}

SingleThreadChecking::~SingleThreadChecking()
{
 if (current_ != NULL) {
     deactivate(true);
 }
}

 
  /**
   * process runnable, if worker thread is aviable, than
   * process it immediatly, otherwise throw ResourceTemporaryNotAviable
   * exception.
   **/
void SingleThreadChecking::process(Runnable* runnable)
{
 if (!is_active()) 
      throw ThreadService::NotActive();
 if (current_ == NULL) {
    {
     MutexLocker ml(currentMutex_);
     if (current_ != NULL) {
       throw ThreadingExceptions::TemporaryNoResources();
     }else{
        current_ = runnable;
     }
    }
    runnableArrived_.notify();
 }else{
    Thread::yield();
    throw ThreadingExceptions::TemporaryNoResources();
 }
}

void  SingleThreadChecking::activate()
{
  ThreadService::activate(); 
  if (!is_running()) start();
}
  
void  SingleThreadChecking::deactivate(bool shutdown)
{
 if (shutdown) {
   cancel();
   if (current_) {
     delete current_;
     current_ = NULL;
   }
 }else{
   if (current_!=NULL) {
     ThreadService::deactivate(shutdown);
   }else{
     process(new DeactivateRunnable(this));
   }
 }
}

void SingleThreadChecking::run()
{
 while(is_active()) {
     runnableArrived_.wait();
     current_->run();
     {
      MutexLocker ml(currentMutex_);
      current_=NULL;
     }
     yield();
 } 
}

}


