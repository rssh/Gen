#ifndef __GradSoft_Mutex_h
#define __GradSoft_Mutex_h

/*
 * GradSoft C++  ToolBox Threading Library.
 * Mutex definitions
 * (C) GradSoft 2000, 2001
 * http://www.gradsoft.kiev.ua
 * Ukraine, Kiev
 * $Id: Mutex.h,v 1.34 2004-03-13 09:53:23 rssh Exp $
 */

#ifndef __GradSoft_ThreadingPostConfig_h
#include <GradSoft/ThreadingPostConfig.h>
#endif

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
namespace GradSoft {


/**
 * Mutual Exclusive Lock (mutex)
 **/
class __dll_threading Mutex
{

public:

  ///
  Mutex()  throw (ThreadingExceptions::Failure);

  ///
 ~Mutex()  throw (ThreadingExceptions::Failure); 

  /**
   * lock mutex
   **/
  void lock(void)  throw (ThreadingExceptions::Failure);

  /**
   * try lock mutex
   *@return true, if mutex is succesfully locked, false otherwise
   **/
  bool try_lock(void) throw() ;

  /**
   * unlock mutex
   *@precondition mutex must be locked
   **/
  void unlock(void)  throw (ThreadingExceptions::Failure);

private:

#ifdef HAVE_PTHREAD_H
  pthread_mutex_t mutex_;
#else
# if defined(WIN32)
  HANDLE mutex_;
  int waiters_count_;
  CRITICAL_SECTION waiters_count_lock_;
# else
#  error "Unknown platform, may be WIN32 or HAVE_PTHREAD_H undefined ?"
# endif
#endif

  Mutex(const Mutex& m);
  Mutex& operator=(const Mutex& m);

};

/**
 * MutexLocker : 
 *  lock mutex on creation, unlock on desctruction.
 * Usage:
 *<code>
 * {
 *  MutexLocker locker(mutex);
 *  ....
 *  critical section operations is here
 *  ....
 * }
 *</code>
 **/
class MutexLocker
{
 Mutex& mutex_;
public:

 ///
 MutexLocker(Mutex& mutex) throw(ThreadingExceptions::Failure)
  :mutex_(mutex)
 { mutex.lock(); }

 ///
 ~MutexLocker() throw(ThreadingExceptions::Failure)
 { bool flg=std::uncaught_exception();
   try { mutex_.unlock(); }
   catch(const ThreadingExceptions::Failure&)
   {if (!flg) throw;} 
 }

};

}

#endif
