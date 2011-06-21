#include <GradSoft/ThreadEvent.h>

/*
 * part of GradSoft C++ ToolBox
 * Threading: ThreadEvent
 * (C) GradSoft, 2001
 * http://www.gradsoft.com.ua
 * $Id: ThreadEvent.cpp,v 1.11 2001-10-31 03:13:50 rssh Exp $
 */


#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

namespace GradSoft {

ThreadEvent::ThreadEvent() 
                      throw (ThreadingExceptions::NoResources,
                             ThreadingExceptions::InternalError) 
{
#ifdef WIN32
  StrExTranslator se;
  se.touch();
  // Create an auto-reset event.
  events_[SIGNAL] = CreateEvent (NULL,  // no security
                                FALSE, // auto-reset event
                                FALSE, // non-signaled initially
                                NULL); // unnamed
  if (events_[SIGNAL]==NULL){
    ThreadingError::lastErrorNotification();
  }
  // Create a manual-reset event.
  events_[BROADCAST] = CreateEvent (NULL,  // no security
                                TRUE,  // manual-reset
                                FALSE, // non-signaled initially
                                NULL); // unnamed
  if (events_[BROADCAST]==NULL){
    ThreadingError::lastErrorNotification();
  }
  ThreadingWrapper::InitializeCriticalSection(&send_lock_);
  ThreadingWrapper::InitializeCriticalSection(&waiters_count_lock_);
  waiters_count_ = 0;
#else
  int retcode = 0;
  retcode = pthread_cond_init(&condition_, NULL);
  ThreadingError::checkError(retcode, "pthread_cond_init");
  retcode = pthread_mutex_init(&internal_mutex_, NULL);
  try {
    ThreadingError::checkError(retcode,"pthread_mutex_init");
  }catch(...){
    pthread_cond_destroy(&condition_);
    throw;
  }
#endif
}

ThreadEvent::~ThreadEvent()
                             throw(ThreadingExceptions::ResourceBusy,
                                   ThreadingExceptions::InternalError)
{
#ifdef WIN32
  StrExTranslator se;
  se.touch();
  if(!CloseHandle(events_[SIGNAL])){
    ThreadingError::lastErrorNotification();
  }
  if(!CloseHandle(events_[BROADCAST])){
    ThreadingError::lastErrorNotification();
  }
  DeleteCriticalSection(&send_lock_);
  DeleteCriticalSection(&waiters_count_lock_);
#else
  ThreadingError::checkError(pthread_cond_destroy(&condition_),"pthread_cond_destroy");
  ThreadingError::checkError(pthread_mutex_destroy(&internal_mutex_),"pthread_mutex_destroy");
#endif
}

void ThreadEvent::wait()
                       throw(ThreadingExceptions::PossibleDeadlock,
                             ThreadingExceptions::InternalError)
{
#ifdef WIN32
  wait(INFINITE);
#else
  ThreadingError::checkError(pthread_mutex_lock(&internal_mutex_),"pthread_mutex_lock");
  int err = pthread_cond_wait(&condition_, &internal_mutex_);
  pthread_mutex_unlock(&internal_mutex_);
  if (err==EINVAL)
      throw ThreadingExceptions::InternalError(err,"pthread_mutex_unlock");
#endif
}

void ThreadEvent::wait(long timeout)
                       throw(ThreadingExceptions::PossibleDeadlock,
                             ThreadingExceptions::InternalError)
{
#ifdef WIN32
  StrExTranslator se;
  se.touch();
  EnterCriticalSection(&send_lock_);
   EnterCriticalSection(&waiters_count_lock_);
    waiters_count_++;
   LeaveCriticalSection(&waiters_count_lock_);
  LeaveCriticalSection(&send_lock_); 
  WaitForMultipleObjects(
      2,      // Wait on both <events_>
      events_,
      FALSE,  // Wait for either event to be signaled
      timeout // Wait milisec
  ); 
  EnterCriticalSection(&waiters_count_lock_);
  waiters_count_--;
  LeaveCriticalSection(&waiters_count_lock_);
#else
  timespec abs_timeout;
  abs_timeout.tv_sec = time(NULL) + timeout/100;
  abs_timeout.tv_nsec = (timeout % 100)*10;
  ThreadingError::checkError(pthread_mutex_lock(&internal_mutex_),"pthread_mutex_lock");
  int err = pthread_cond_timedwait(&condition_, &internal_mutex_, &abs_timeout);
  pthread_mutex_unlock(&internal_mutex_);
  if (err==EINVAL)
      throw ThreadingExceptions::InternalError(err,"pthread_cond_timedwait");
#endif
}

void ThreadEvent::notify() throw()
{
#ifdef WIN32
  StrExTranslator se;
  se.touch();
  PulseEvent(events_[SIGNAL]);
#else
  pthread_cond_signal(&condition_);
#endif
}

void ThreadEvent::notifyAll() throw()
{
#ifdef WIN32
  StrExTranslator se;
  se.touch();
  EnterCriticalSection(&send_lock_);
  if (waiters_count_>0) {
    SetEvent(events_[BROADCAST]); 
    while (waiters_count_> 0) Sleep(0); 
    ResetEvent(events_[BROADCAST]); 
  }
  LeaveCriticalSection(&send_lock_);
#else
  pthread_cond_broadcast(&condition_);
#endif
}

}

