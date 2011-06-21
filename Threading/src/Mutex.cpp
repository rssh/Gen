#ifndef __GradSoft_Mutex_h
#include <GradSoft/Mutex.h>
#endif

#ifdef HAVE_ERRNO_H
#include <sys/errno.h>
#endif
namespace GradSoft {

#ifdef HAVE_PTHREAD_H

Mutex::Mutex() throw(ThreadingExceptions::Failure)
{
  ThreadingError::checkError( 
         pthread_mutex_init(&mutex_,NULL), "pthread_mutex_init failed" );
}


Mutex::~Mutex() throw(ThreadingExceptions::Failure)
{
  ThreadingError::checkError( pthread_mutex_destroy(&mutex_),
                              "pthread_mutex_destroy failed" );
}

void Mutex::lock() throw(ThreadingExceptions::Failure)
{
  ThreadingError::checkError( pthread_mutex_lock(&mutex_),
                             "pthread_mutex_lock failed" );
}

void Mutex::unlock() throw(ThreadingExceptions::Failure)
{
 ThreadingError::checkError(pthread_mutex_unlock(&mutex_),
                            "pthread_mutex_unlock failed" );
}

bool Mutex::try_lock() throw()
{
  int err = pthread_mutex_trylock(&mutex_);
  if (err==0) return true;
  if (err==EBUSY) return false;
  return false;
}

#else
#ifdef WIN32

Mutex::Mutex()
{
  {
    StrExTranslator se;
    se.touch();
    mutex_ = CreateMutex(NULL,FALSE,NULL);
  }
  if (mutex_==0) {
    ThreadingError::lastErrorNotification();
  } 
  ThreadingWrapper::InitializeCriticalSection(&waiters_count_lock_);
  {
    StrExTranslator se;
    se.touch();
    EnterCriticalSection(&waiters_count_lock_);
    waiters_count_ = 0;
    LeaveCriticalSection(&waiters_count_lock_);
  }
}

Mutex::~Mutex()
{
  StrExTranslator se;
  se.touch();
  if (!CloseHandle(mutex_)){
    ThreadingError::lastErrorNotification();
  }
  EnterCriticalSection(&waiters_count_lock_);
  if (waiters_count_ > 0) {
    LeaveCriticalSection(&waiters_count_lock_);
    throw ThreadingExceptions::ResourceBusy();
  }
  LeaveCriticalSection(&waiters_count_lock_);
  DeleteCriticalSection(&waiters_count_lock_);
}

void Mutex::lock()
{
  StrExTranslator se;
  se.touch();
  EnterCriticalSection(&waiters_count_lock_);
  waiters_count_++;
  LeaveCriticalSection(&waiters_count_lock_);
  switch (WaitForSingleObject(mutex_,INFINITE)) {
  case WAIT_ABANDONED:
    throw ThreadingExceptions::PossibleDeadlock();
  case WAIT_FAILED:
    ThreadingError::lastErrorNotification();
  }
}

bool Mutex::try_lock()
{
  StrExTranslator se;
  se.touch();
  switch(WaitForSingleObject(mutex_,0)) {
//  case WAIT_ABANDONED:
//           impossible
//    throw ThreadingExceptions::PossibleDeadlock();
  case WAIT_OBJECT_0:
    EnterCriticalSection(&waiters_count_lock_);
    waiters_count_++;
    LeaveCriticalSection(&waiters_count_lock_);
    return true;
  case WAIT_TIMEOUT:
    return false;
  case WAIT_FAILED:
    ThreadingError::lastErrorNotification();
  }
  return false; 
}

void Mutex::unlock()
{
  StrExTranslator se;
  se.touch();
  if(!ReleaseMutex(mutex_)) {
    ThreadingError::lastErrorNotification();
  }
  EnterCriticalSection(&waiters_count_lock_);
  waiters_count_--;
  LeaveCriticalSection(&waiters_count_lock_);
}

#else
#error  "platform does not determined (neither HAVE_PTHREAD_H, nor WIN32 is defined)"
#endif
#endif


}

