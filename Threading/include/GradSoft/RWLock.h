#ifndef __GradSoft_RWLock_h
#define __GradSoft_RWLock_h

/*
 * part of GradSoft C++ ToolBox
 * (C) GradSoft 2000, 2001
 * http://www.gradsoft.com.ua
 */

#ifndef __GradSoft_Mutex_h
#include <GradSoft/Mutex.h>
#endif

#ifdef HAVE_RWLOCK_INIT
#include <synch.h>
#endif


/**
 *
 **/
namespace GradSoft {

 ///

/**
 * Read/Write lock 
 *  allow multiple readers/single writer.
 *  access to object with such lock must be 
 *  sequence of atomic read-only or write-only
 *  operations.
 *
 *  (i. e. rwlock.read_lock(), rwlock.write_lock() is
 * the fastest way to deadlock state.
 **/
class __dll_threading RWLock
{
#ifdef HAVE_PTHREAD_RWLOCK_INIT
 mutable pthread_rwlock_t lock_;
#elif defined(HAVE_RWLOCK_INIT)
 mutable rwlock_t lock_;
#else
 mutable int nReaders_;
 mutable Mutex nReadersMutex_; 
 mutable Mutex writeMutex_; 
#endif
public:

   /**
    * constructor. Create rwlock in ulocked state.
    **/
   RWLock() throw(ThreadingExceptions::NoResources, 
                  ThreadingExceptions::InternalError);

   /**
    * destructor
    **/
   virtual ~RWLock()  throw(ThreadingExceptions::ResourceBusy,
                            ThreadingExceptions::InternalError);

   /**
    * obtaine read lock. If now rwlock is write-locked, than wait until
    * lock will be obtained.
    **/
   void  read_lock() const
                      throw(ThreadingExceptions::NoResources,
                            ThreadingExceptions::PossibleDeadlock,
                            ThreadingExceptions::InternalError);

   /**
    * try to obtain read lock. 
    *@returns true, if lock obtained, false otherwise.
    **/
   bool  try_read_lock() const
                      throw(ThreadingExceptions::NoResources,
                            ThreadingExceptions::PossibleDeadlock,
                            ThreadingExceptions::InternalError);

   /**
    * unlock read lock. (safe, only if lock was previously read-locked).
    **/
   void  read_unlock() const
                        throw (ThreadingExceptions::NoPermission,
                               ThreadingExceptions::InternalError);

   /**
    * obtaine write lock. If now rwlock is locked, than wait until
    * lock will be obtained.
    **/
   void  write_lock()
                       throw(ThreadingExceptions::NoMemory,
                             ThreadingExceptions::PossibleDeadlock,
                             ThreadingExceptions::InternalError);

   /**
    * try to obtain write lock. 
    *@returns true, if lock obtained, false otherwise.
    **/
   bool  try_write_lock()
                       throw(ThreadingExceptions::NoMemory,
                             ThreadingExceptions::PossibleDeadlock,
                             ThreadingExceptions::InternalError);

   /**
    * unlock write lock. (safe, only if lock was previously write-locked).
    **/
   void  write_unlock() const
                        throw (ThreadingExceptions::NoPermission,
                               ThreadingExceptions::InternalError);

};


/**
 * ReadLocker : wrap read locking in constructor/destructor.
 * Usage:
 *
 * {
 *  ReadLocker rlocker(rwlock);
 *  ...
 *  read-only operations here
 *  ...
 * }
 **/
class ReadLocker
{
  const RWLock& owner_;
public:
  /**
   * constructor. Lock owner.
   *@param owner - rwlock to lock.
   **/
  ReadLocker(const RWLock& owner)
                      throw(ThreadingExceptions::PossibleDeadlock,
                            ThreadingExceptions::NoResources,
                            ThreadingExceptions::InternalError)
   :owner_(owner)  { owner_.read_lock(); }

  /**
   * destructor. Unlock owner.
   **/
  ~ReadLocker() throw ()
         { owner_.read_unlock(); }
};

/**
 * WriteLocker: wrap write locking in constructor/destructor
 *
 * Usage:
 * { WriteLocker wlocker(rwlock);
 *   ... write-operations here ..
 * }
 **/
class WriteLocker
{
  RWLock& owner_;
public:
  /**
   * Constructor. Lock owner.
   *@param owner - rwlock to lock.
   **/
  WriteLocker(RWLock& owner)
                      throw(ThreadingExceptions::PossibleDeadlock,
                            ThreadingExceptions::NoResources,
                            ThreadingExceptions::InternalError)
   :owner_(owner)  { owner_.write_lock(); }

  /**
   * Destructor. Unlock owner.
   **/
  ~WriteLocker() throw()
                   { owner_.write_unlock(); }
};


/**
 * Threadsafe adapter for DefaultConstructable Assignable and Comparable
 * objects.
 **/
template<class T>
class RWLocked
{
public:

 ///
 typedef T locked_type;

protected:

 T v_;
 RWLock rwlock_;

public:
 
 ///
 RWLocked()
  :v_(),rwlock_() {}

 ///
 RWLocked(const T& v)
  :v_(v),rwlock_() {} 

 ///
 RWLocked(const RWLocked& x)
 {
  x.read_lock();
  v_ = x.v_; 
  x.read_unlock();
 }

 ///
 virtual ~RWLocked() {}

 ///
 RWLocked& operator=(const RWLocked& x)
 {
  if (&x!=this) {
   write_lock();
   x.read_lock();
   v_ = x.v_; 
   x.read_unlock();
   write_unlock();
  }
  return *this;
 }

 ///
 bool  operator==(const RWLocked& x)
 {
   if (&x==this) return true;
   {
     ReadLocker rlt(this.rwlocker_);
     ReadLocker rlx(x.rwlocker_);
     return v_ == x.v_;
   }
 }

 ///
 bool  operator==(const T& x)
 {
  ReadLocker rl(rwlock_);
  return v_ == x;
 }

 ///
 bool  operator!=(const RWLocked& x)
 {
   return !((*this)==x);
 }

 ///
 bool  operator!=(const T& x)
 {
  ReadLocker rl(rwlock_);
  return v_ != x;
 }
 
public:

 ///
 RWLock&  get_rwlock()  throw() { return rwlock_; }

 ///
 T&       get_value_() throw()  { return v_; }
 ///
 const T& get_value_() const throw() { return v_; }

 ///
 void  read_lock() const
                      throw(ThreadingExceptions::PossibleDeadlock,
                            ThreadingExceptions::NoResources,
                            ThreadingExceptions::InternalError)
 { rwlock_.read_lock(); }
 ///
 void  read_unlock() const
                      throw(ThreadingExceptions::InternalError)
 { rwlock_.read_unlock(); }

 ///
 void  write_lock()
                      throw(ThreadingExceptions::PossibleDeadlock,
                            ThreadingExceptions::NoResources,
                            ThreadingExceptions::InternalError)
 { rwlock_.write_lock(); }

 ///
 void  write_unlock() const
                      throw(ThreadingExceptions::InternalError)
 { rwlock_.write_unlock(); }

};

/**
 * thread-safe pointer to shared resource.
 **/
template<class T>
class RWLockedPtr:public RWLocked<T*>
{
};


}

#endif
