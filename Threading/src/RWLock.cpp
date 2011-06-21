#include <GradSoft/RWLock.h>

/*
 * part of GradSoft C++ ToolBox
 * (C) GradSoft 2000, 20001
 * http://www.gradsoft.com.ua
 * $Id: RWLock.cpp,v 1.12 2003-03-11 14:24:35 rssh Exp $
 */

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif


namespace GradSoft {


RWLock::RWLock() throw(ThreadingExceptions::NoResources, 
                       ThreadingExceptions::InternalError)
{
 int err = 0;
#ifdef HAVE_PTHREAD_H
 const char* fun;
#endif
#ifdef HAVE_PTHREAD_RWLOCK_INIT
 err = pthread_rwlock_init(&lock_,NULL);
 fun = "pthread_rwlock_init";
#elif defined(HAVE_RWLOCK_INIT)
 err = rwlock_init(&lock_,NULL,NULL);
 fun = "rwlock_init";
#else
 nReaders_ = 0;
#endif

#if defined(HAVE_PTHREAD_RWLOCK_INIT)||defined(HAVE_RWLOCK_INIT) 
 try {
  ThreadingError::checkError(err, fun);
 }catch(const ThreadingExceptions::ResourceBusy& ex){
  throw ThreadingExceptions::InternalError(ex.getErrorCode(),
                                           ex.getErrorMessage());
 }
#endif
}

RWLock::~RWLock() throw (ThreadingExceptions::ResourceBusy,
                         ThreadingExceptions::InternalError)
{
#ifdef HAVE_PTHREAD_RWLOCK_INIT
 ThreadingError::checkError( pthread_rwlock_destroy(&lock_),
                            "pthread_rwlock_destroy" );
#elif defined(HAVE_RWLOCK_INIT)
 ThreadingError::checkError( rwlock_destroy(&lock_), "rwlock_destroy"  );
#else
 if (nReaders_ > 0 || nReaders_==-1) writeMutex_.unlock();
#endif
}

void RWLock::read_lock() const throw(ThreadingExceptions::NoResources,
                                     ThreadingExceptions::PossibleDeadlock,
                                     ThreadingExceptions::InternalError)
{
#ifdef HAVE_PTHREAD_RWLOCK_INIT
 pthread_rwlock_rdlock(&lock_);
#elif defined(HAVE_RWLOCK_INIT)
 rw_rdlock(&lock_);
#else
 nReadersMutex_.lock();
 if (nReaders_ <= 0) 
    writeMutex_.lock();
 ++nReaders_;
 nReadersMutex_.unlock();
#endif
}

bool RWLock::try_read_lock() const 
                               throw (ThreadingExceptions::NoResources,
                                      ThreadingExceptions::PossibleDeadlock,
                                      ThreadingExceptions::InternalError)
{
#if defined(HAVE_PTHREAD_RWLOCK_INIT) || defined(HAVE_RWLOCK_INIT)
 int err = 0;
 const char* fun;
#ifdef HAVE_PTHREAD_RWLOCK_INIT
 err = pthread_rwlock_tryrdlock(&lock_);
 fun = "pthread_rwlock_tryrdlock(&lock_)";
#else
 err = rw_tryrdlock(&lock_)!=0;
 fun = "rw_tryrdlock";
#endif
 if (err == EBUSY) {
    return false;
 }else{
    ThreadingError::checkError(err, fun);
    return true;
 }
#else
 nReadersMutex_.lock();
 if (nReaders_>=0) {
   if (nReaders_==0)
      writeMutex_.lock();
   ++nReaders_;
   nReadersMutex_.unlock();
   return true;
 }else{
   nReadersMutex_.unlock();
   return false;
 }
#endif
}

void RWLock::read_unlock() const 
                            throw(ThreadingExceptions::NoPermission,
                                  ThreadingExceptions::InternalError) 
{
#ifdef HAVE_PTHREAD_RWLOCK_INIT
 ThreadingError::checkError(pthread_rwlock_unlock(&lock_), "pthread_rwlock_unlock");
#elif defined(HAVE_RWLOCK_INIT)
#ifdef HAVE_MUTABLE
 ThreadingError::checkError(rw_unlock(&lock_),"rw_unlock");
#else
 ThreadingError::checkError(rw_unlock(const_cast<rwlock_t*>(&lock_)),"rw_unlock");
#endif
#else
 nReadersMutex_.lock();
 --nReaders_;
 if (nReaders_==0) 
   writeMutex_.unlock();
 nReadersMutex_.unlock();
#endif
}

void RWLock::write_lock() 
                          throw (ThreadingExceptions::NoMemory,
                                 ThreadingExceptions::PossibleDeadlock,
                                 ThreadingExceptions::InternalError)
{
#ifdef HAVE_PTHREAD_RWLOCK_INIT
 ThreadingError::checkError(pthread_rwlock_wrlock(&lock_),"pthread_rwlock_wrlock");
#elif defined(HAVE_RWLOCK_INIT)
 ThreadingError::checkError(rw_wrlock(&lock_),"rw_wrlock");
#else
 writeMutex_.lock();
 nReadersMutex_.lock();
 nReaders_=-1;
 nReadersMutex_.unlock();
#endif
}

bool RWLock::try_write_lock()
                          throw (ThreadingExceptions::NoMemory,
                                 ThreadingExceptions::PossibleDeadlock,
                                 ThreadingExceptions::InternalError)
{
 int err = 0;
#ifdef HAVE_PTHREAD_RWLOCK_INIT
 err = pthread_rwlock_trywrlock(&lock_);
 if (err == EBUSY) return false;
 ThreadingError::checkError(err,"pthread_rwlock_trywrlock");
 return true;
#elif defined(HAVE_RWLOCK_INIT)
 err = rw_trywrlock(&lock_)!=0;
 if (err == EBUSY) return false;
 ThreadingError::checkError(err,"rw_trywrlock");
 return true;
#else
 return writeMutex_.try_lock();
#endif
}

void RWLock::write_unlock() const
                            throw(ThreadingExceptions::NoPermission,
                                 ThreadingExceptions::InternalError)
{
#ifdef HAVE_PTHREAD_RWLOCK_INIT
 ThreadingError::checkError(pthread_rwlock_unlock(&lock_),
                            "pthread_rwlock_unlock");
#elif defined(HAVE_RWLOCK_INIT)
 ThreadingError::checkError(rw_unlock(&lock_),"rw_unlock");
#else
 writeMutex_.unlock();
 nReadersMutex_.lock();
 nReaders_=0;
 nReadersMutex_.unlock();
#endif
}


}

