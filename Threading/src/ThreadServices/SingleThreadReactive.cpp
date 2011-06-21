
#ifndef __GradSoft_ThreadServices_SingleThreadReactive_h
#include <GradSoft/ThreadServices/SingleThreadReactive.h>
#endif

/*
 * part of GradSoft C++ ToolBox: Threading
 * (C) GradSoft 2001
 * (C) Ruslan Shevchenko 2001
 * $Id: SingleThreadReactive.cpp,v 1.8 2003-03-11 14:24:36 rssh Exp $
 */

#include <assert.h>


namespace GradSoft {

/**
 * Single-Threaded Reactor: i. e. requests are passed
 * to queue, single thread retryive requests from queue
 * and process it;s one-by-one
 **/

//  enum Mode { Blocked, Checked, CheckedWithTimeout }

//  QueueOfRunnables   queue_;
//  Mode               mode_;
//  long               timeout_;


SingleThreadReactive::SingleThreadReactive(int queueSize, 
                            SingleThreadReactive::Mode mode, long timeout)
   :queue_(queueSize),
    mode_(mode),
    timeout_(timeout)
{}
                       
SingleThreadReactive::~SingleThreadReactive()
{}

void SingleThreadReactive::process(Runnable* runnable)
{
 if (!is_active()) throw ThreadService::NotActive();
 switch(mode_) {
      case Blocked:
          queue_.put(runnable);
          break;
      case Checked:
          if (!queue_.try_put(runnable)) 
             throw ThreadingExceptions::TemporaryNoResources();
          break;
      case CheckedWithTimeout:
          if (!queue_.put_with_timeout(runnable, timeout_)) {
              throw ThreadingExceptions::TemporaryNoResources();
          }  
          break;
      default:
          assert(0);
 }
}

void  SingleThreadReactive::run()
{
  try {
   while(is_active()) {
     Runnable* current = queue_.get();
     current->run();
     yield();
   }
  } catch (QueueOfRunnables::Destructed ) {
    // do nothing, this is called from destructor.
  }
}

void SingleThreadReactive::activate()
{
  ThreadService::activate();
  if (!is_running()) start();
}

void SingleThreadReactive::deactivate(bool shutdown)
{
  if (shutdown) {
    cancel();
  }else{
    queue_.put(new DeactivateRunnable(this));
  }
}

}

