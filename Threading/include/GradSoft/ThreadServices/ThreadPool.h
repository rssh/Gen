#ifndef __GradSoft_ThreadServices_ThreadPool_h
#define __GradSoft_ThreadServices_ThreadPool_h

/*
 * GradSoft C++ ToolBox: Threading
 * (C) GradSoft 2001
 * (C) Ruslan Shevchenko 2001
 * http://www.gradsoft.com.ua
 * $Id: ThreadPool.h,v 1.8 2004-03-13 09:53:27 rssh Exp $
 */

#ifndef __GradSoft_ThreadService_h
#include <GradSoft/ThreadService.h>
#endif

#ifndef __GradSoft_Thread_h
#include <GradSoft/Thread.h>
#endif

#ifndef __GradSoft_ThreadServices_QueueOfRunnables_h
#include <GradSoft/ThreadServices/QueueOfRunnables.h>
#endif

namespace GradSoft {

/**
 * ThreadPool service.
 **/
class __dll_threading ThreadPool: public ThreadService
{
public:

  ///
  enum Mode { Blocked, Checked, CheckedWithTimeout };

private:


  class ThreadPoolElement: public Thread
  {
   ThreadPool* owner_;
   public:
    
    ThreadPoolElement(ThreadPool* owner)
      :owner_(owner)
    {}

    void run();

  
    ThreadPoolElement() // used, when C++ compiler does not support 
                        // array initializer
      :owner_(NULL)
    {}

    void set_owner(ThreadPool* owner);

  };

  friend class ThreadPoolElement;

  QueueOfRunnables      queue_;
  ThreadPoolElement*    threads_;
  int                   nThreads_;
  Mode                  mode_;
  long                  timeout_;

public:

  ThreadPool(int nThreads, int queueSize,  Mode mode, long timeout);

  void process(Runnable* runnable);

  void activate();

  void deactivate(bool shutdown);

};

}

#endif
