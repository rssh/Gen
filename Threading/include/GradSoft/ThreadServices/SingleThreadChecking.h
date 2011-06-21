#ifndef __GradSoft_ThreadService_SingleThreadChecking_h
#define __GradSoft_ThreadService_SingleThreadChecking_h

/*
 * part of GradSoft C++ ToolBox: Threading
 * (C) GradSoft 2001
 * http://www.gradsoft.com.ua
 * $Id: SingleThreadChecking.h,v 1.7 2004-03-13 09:53:27 rssh Exp $
 */

#ifndef __GradSoft_ThreadService_h
#include <GradSoft/ThreadService.h>
#endif

#ifndef __GradSoft_Thread_h
#include <GradSoft/Thread.h>
#endif

#ifndef __GradSoft_ThreadEvent_h
#include <GradSoft/ThreadEvent.h>
#endif

namespace GradSoft {

/**
 * Single thread service wich run in own thread and check
 * aviability of service before processing of runnable
 **/
class __dll_threading SingleThreadChecking: public ThreadService, public Thread
{
private:

  Runnable*   current_;
  Mutex       currentMutex_;
  ThreadEvent runnableArrived_;

public:

  SingleThreadChecking();

  ~SingleThreadChecking();
 
  /**
   * process runnable, if worker thread is aviable, than
   * process it immediatly, otherwise throw ResourceTemporaryNotAviable
   * exception.
   **/
  void process(Runnable* runnable);

  void  activate();
  
  void  deactivate(bool shutdown);

protected:

  void run();

};

}



#endif
