#ifndef __GradSoft_ThreadServices_SingleThreadBlocking_h
#define __GradSoft_ThreadServices_SingleThreadBlocking_h

/*
 * part of GradSoft C++ ToolBox.
 * (C) GradSoft 2000, 2001
 * http://www.gradsoft.com.ua
 * $Id: SingleThreadBlocking.h,v 1.8 2004-03-13 09:53:26 rssh Exp $
 */

#ifndef __GradSoft_ThreadService_h
#include <GradSoft/ThreadService.h>
#endif

#ifndef __GradSoft_Thread_h
#include <GradSoft/Thread.h>
#endif


namespace GradSoft {

/**
 * SingleThreadBlocking ThreadService.
 * (i. e. this is 'non-thread' ThreadService: each message is
 * processed in the same thread as caller, without
 * any dispatching.
 **/
class __dll_threading SingleThreadBlocking: public ThreadService
{
private:

  Runnable* current_;
  Thread::id_type threadId_;

public:

  SingleThreadBlocking()
    :current_(NULL), threadId_(Thread::current_id())
  {}

  ~SingleThreadBlocking()
  {
    if (current_ != NULL) {
      if (threadId_ == Thread::current_id()){
        delete[] current_;
      }else{
        // else it now evaluated, don't warry about this.
      }
    }
  }

  ///
  void process(Runnable* runnable);

};

}


#endif
