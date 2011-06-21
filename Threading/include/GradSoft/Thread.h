#ifndef __GradSoft_Thread_h
#define __GradSoft_Thread_h

/*
 * part of GradSoft C++ ToolBox
 * (C) GradSoft 2000 - 2010
 * http://www.gradsoft.com.ua
 */

#ifndef __GradSoft_RWLock_h
#include <GradSoft/RWLock.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

/**
 *
 **/
namespace GradSoft
{

class ThreadContext;

/**
 * Base class for thread.
 * Library users create own threads by inherienting from this class
 * and overriding run() method. 
 **/
class __dll_threading Thread
{
public:

#ifdef HAVE_PTHREAD_H
  typedef pthread_t id_type;
#endif

#ifdef WIN32
  typedef DWORD id_type;
#endif

private:

  int error_code_;
  
  volatile bool running_;
  RWLock runningRWLock_;

#ifdef HAVE_PTHREAD_H
  pthread_t      id_;
  pthread_attr_t attr_;
  bool in_cleanup_;
  pthread_mutex_t idMutex_;
#endif

#ifdef WIN32
  HANDLE hndl_;
  DWORD threadID_;
  bool do_break_;
  Mutex idMutex_;
#endif

  ThreadContext* context_;

public:


  /**
   * join current thread with argument.
   * note, that all thread must be joined
   * with main thread after completition.
   * By default this is done in thread
   * destructor
   **/
#ifdef WIN32
  static void join(HANDLE id) throw(ThreadingExceptions::Failure);
#else
  static void join(const id_type& id) throw(ThreadingExceptions::Failure);
#endif


  /**
   * join current thread with thread <code> th </code>
   **/
  static void join(const Thread& th) throw(ThreadingExceptions::Failure) 
#ifdef WIN32
   { join(th.hndl_); }
#else
   { join(th.id_); }
#endif


  ///
  Thread() throw(ThreadingExceptions::NoMemory);

  ///
  virtual ~Thread() ;

  /**
   * return system-depended id of started thread
   **/
  id_type id() const  throw()       
  {
#ifdef WIN32
    return threadID_;
#else
    return id_; 
#endif
  }

  /**
   * return id of current thread
   **/
  static id_type current_id()  throw();

  /**
   * return error code of last thread operation;
   * 0 if all is good, otherwise - system depended 
   * nonzero value
   **/
  int error_code() const throw () { return error_code_; }

  /**
   * library user must put all work of thread here.
   **/
  virtual void run()=0;

  /**
   * Is thread running ?
   *@return true, if thread is running; false otherwise.
   **/
  bool  is_running() const throw (ThreadingExceptions::NoResources,
                                  ThreadingExceptions::InternalError);

  /**
   * start thread.
   **/
  void start() 
            throw (ThreadingExceptions::NoResources,
                   ThreadingExceptions::InternalError);

  /**
   * cancel thread
   **/
  void cancel() throw();

  /**
   * get context of thread. automatically create one if it does not exists.
   **/
  ThreadContext*  getContext();


  /**
   * mark thread to be detached
   **/
#ifdef HAVE_PTHREAD_H
   void  setDetached();
#endif


  /**
   * sleep, yield and test cancelation points
   **/
  static void sleepCurrent(unsigned int sec) throw();

  /**
   * sleep, yield and test cancelation points
   **/
  static void nanosleepCurrent(unsigned int sec, long nsec) throw();

  /**
   * yield
   **/
  static void yield();

protected:

  /**
   * exit from thread
   **/
  void exit(void) throw();

  /**
   * test for cancelation point
   **/
  void testcancel(void) throw();

  /**
   * sleep, yield and test for cancelation point
   **/
  void sleep(unsigned int sec) throw();

  /**
   * sleep, yield and test for cancelation point
   **/
  void nanosleep(unsigned int sec, long nsec) throw();

private:

  static void* thread_process(void* v);
  static void  thread_cleanup_process(void* v) ;

  // block copying
  Thread(const Thread& x);
  Thread& operator=(const Thread& x);

};

}

#endif
