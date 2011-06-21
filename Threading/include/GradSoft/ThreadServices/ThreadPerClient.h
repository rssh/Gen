#ifndef __GradSoft_ThreadServices_ThreadPerClient_h
#define __GradSoft_ThreadServices_ThreadPerClient_h

/*
 * Part of GradSoft C++ ToolBox
 * (C) GradSoft, 2001
 * (C) Ruslan Shevchenko, 2001
 * $Id: ThreadPerClient.h,v 1.5 2004-03-13 09:53:27 rssh Exp $
 */

#ifndef __GradSoft_ThreadService_h
#include <GradSoft/ThreadService.h>
#endif

#ifndef __GradSoft_Thread_h
#include <GradSoft/Thread.h>
#endif

namespace GradSoft {

/**
 *
 **/
class __dll_threading ThreadPerClient: public ThreadService
{
private:

  class ClientThread: public Thread
  {
     Runnable* client_;
   public:

   ClientThread(Runnable* client)
    :client_(client)
   {}
    
   void run();

  };

public:

  void process(Runnable* runnable);

};

}

#endif
