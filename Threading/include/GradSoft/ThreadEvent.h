#ifndef __GradSoft_ThreadCondition_h
#define __GradSoft_ThreadCondition_h

/*
 * ThreadCondition
 * part of GradSoft C++ Threading toolbox
 * (C) GradSoft 2000, 2001
 * $Id: ThreadEvent.h,v 1.10 2004-03-13 09:53:25 rssh Exp $
 */

#ifndef __GradSoft_ThreadingExceptions_h
#include <GradSoft/ThreadingExceptions.h>
#endif

#ifndef __GradSoft_ThreadingUtils_h
#include <GradSoft/ThreadingUtils.h>
#endif

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

/**
 *
 **/
namespace GradSoft
{

#ifdef WIN32
const int SIGNAL = 0;
const int BROADCAST = 1;
#endif

/**
 * Thread Event class
 * (or in other terminology, ThreadCondition)
 **/
class __dll_threading ThreadEvent
{
public:
   ///
   ThreadEvent() throw(ThreadingExceptions::NoResources,
                       ThreadingExceptions::InternalError);

   ///
   ~ThreadEvent() throw(ThreadingExceptions::ResourceBusy,
                            ThreadingExceptions::InternalError);

   ///
   void wait()
                  throw(ThreadingExceptions::PossibleDeadlock,
                        ThreadingExceptions::InternalError);

   ///
   void wait(long timeout)
                  throw(ThreadingExceptions::PossibleDeadlock,
                        ThreadingExceptions::InternalError);

   ///
   void notify() throw();

   ///
   void notifyAll() throw();

private:

#ifdef WIN32
  int waiters_count_;
  CRITICAL_SECTION waiters_count_lock_;
  CRITICAL_SECTION send_lock_;
  HANDLE events_[2];
#else
  pthread_cond_t condition_;
  pthread_mutex_t internal_mutex_;
#endif
};

}

#endif
