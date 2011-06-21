#ifndef __GradSoft_ThreadServices_SingleThreadReactive_h
#define __GradSoft_ThreadServices_SingleThreadReactive_h

/*
 * part of GradSoft C++ ToolBox: Threading
 * (C) GradSoft 2001
 * (C) Ruslan Shevchenko 2001
 * $Id: SingleThreadReactive.h,v 1.7 2004-03-13 09:53:27 rssh Exp $
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
 * Single-Threaded Reactor: i. e. requests are passed
 * to queue, single thread retryive requests from queue
 * and process it;s one-by-one
 **/
class __dll_threading SingleThreadReactive: public ThreadService, public Thread
{
public:

  enum Mode { Blocked, Checked, CheckedWithTimeout };

private:

  QueueOfRunnables   queue_;
  Mode               mode_;
  long               timeout_;

public:

  SingleThreadReactive(int queueSize, Mode mode, long timeout);
  ~SingleThreadReactive();
                       

  void process(Runnable* runnable);

protected:

  void  run();

public:

  void activate();

  void deactivate(bool shutdown);

};

}

#endif
