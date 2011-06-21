/*
 * part of GradSoft C++ ToolBox
 * (C) Grad-Soft Ltd, Kiev, Ukraine. 2000, 2001, 2002
 * http://www.gradsoft.com.ua
 * $Id: Thread.cpp,v 1.49 2003-03-14 00:27:42 rssh Exp $
 */

#include <GradSoft/Thread.h>
#include <GradSoft/ThreadEvent.h>
#include <GradSoft/ThreadContext.h>
#include <ThreadContextSingleton.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SCHED_H
#include <sched.h>
#endif

#ifndef __GradSoft_vector_INCLUDED
#include <vector>
#define __GradSoft_vector_INCLUDED
#endif


#ifdef WIN32
#include <windows.h>
#endif

#include <assert.h>


using namespace std;


namespace GradSoft
{


#ifdef HAVE_PTHREAD_H


Thread::Thread() throw (ThreadingExceptions::NoMemory) 
{
  id_ = 0;
  error_code_ = pthread_attr_init(&attr_);
  running_=false;
  in_cleanup_=false;
  ThreadingError::checkError(error_code_,"pthread_attr_init");
  pthread_mutex_init(&idMutex_,0);
  context_=NULL;
}

Thread::~Thread() 
{
 pthread_mutex_lock(&idMutex_);
 if (id_ == pthread_self()) {
   pthread_mutex_unlock(&idMutex_);
   // we are descructed from themself
   // so, it was call "delete this" from run
   if (!in_cleanup_) {
     pthread_detach(id_);
     running_ = false;
     pthread_cleanup_push(thread_cleanup_process,this);
     in_cleanup_ = true;
     pthread_exit(NULL);
     pthread_cleanup_pop(1); //  useless for our purpose, but Solaris require
   }else{
     pthread_attr_destroy(&attr_);
     pthread_mutex_destroy(&idMutex_);
   }
 }else{
   pthread_mutex_unlock(&idMutex_);
   if (is_running()) {
     cancel();
   }
   join(*this);
   pthread_attr_destroy(&attr_);
   pthread_mutex_destroy(&idMutex_);
 }
}


void* Thread::thread_process(void* v)  
{
 Thread* th = (Thread*)(v);
 assert(th!=NULL);
 try {
  {
   WriteLocker wl(th->runningRWLock_);
   th->running_ = true;
  }
  pthread_mutex_lock(&th->idMutex_);  // wait until id_ is set.
  pthread_mutex_unlock(&th->idMutex_);
  th->context_ = new ThreadContext(th);
  th->run();
  {
   WriteLocker wl(th->runningRWLock_);
   th->running_ = false;
  }
  th->context_=NULL; // destructor will be called in thread-destructor.
  pthread_exit(NULL);
 }catch(...){
  {
   th->running_ = false;
   th->error_code_ = -1;
   th->context_ = NULL;
   throw;
  }
  //pthread_exit(th);
 }
 return NULL;
}

void  Thread::thread_cleanup_process(void* v) 
{
  Thread* th = (Thread*)(v);
  delete th;
}

void Thread::start()
                     throw  (ThreadingExceptions::NoResources,
                             ThreadingExceptions::InternalError)  
{
 pthread_mutex_lock(&idMutex_);
 int errcode =  pthread_create(&id_, &attr_, thread_process, this);
 pthread_mutex_unlock(&idMutex_);
 ThreadingError::checkError(errcode,"pthread_create");
}

bool Thread::is_running() const throw(ThreadingExceptions::NoResources,
                                      ThreadingExceptions::InternalError)
{
 ReadLocker rl(runningRWLock_);
 return running_;
}

void Thread::cancel() throw()
{
 error_code_=pthread_cancel(id_);
 if (error_code_==0) {
   WriteLocker wl(runningRWLock_);
   running_=false;
 }
}

void Thread::join(const id_type& id) throw(ThreadingExceptions::Failure)
{
 if (id == pthread_self()) return;
 pthread_join(id,NULL);
}

#ifdef HAVE_PTHREAD_H
void Thread::setDetached()
{
 if (!running_) {
   int rc = pthread_attr_setdetachstate(&attr_,1);
   if (rc!=0) {
      //std::cerr << "setdetachstate failed with rc=" << rc << std::endl;
   }
 } else {
   pthread_detach(id_);
 }
}

#endif

void Thread::yield()
{
 sched_yield();
}

void Thread::exit(void) throw()
{
 pthread_exit(NULL);
}

ThreadContext* Thread::getContext()
{
 return context_;
}

void Thread::sleep(unsigned int nseconds) throw()
{
 sleepCurrent(nseconds);
}

void Thread::sleepCurrent(unsigned int nseconds) throw()
{
#ifdef __linux__
pthread_testcancel();
#endif
yield();
::sleep(nseconds);
#ifdef __linux__
pthread_testcancel();
#endif
}

void Thread::nanosleep(unsigned int nseconds, long nnanoseconds) throw()
{
 nanosleepCurrent(nseconds, nnanoseconds);
}

void Thread::nanosleepCurrent(unsigned int nseconds, long nnanoseconds) throw()
{
#ifdef __linux__
pthread_testcancel();
#endif
yield();
timespec req;
req.tv_sec=nseconds;
req.tv_nsec=nseconds;
::nanosleep(&req,0);
#ifdef __linux__
pthread_testcancel();
#endif
}

void Thread::testcancel(void) throw()
{
pthread_testcancel();
}

Thread::id_type Thread::current_id() throw()
{
 return pthread_self();
}

#else
# if defined(WIN32)
//
// class to support for "delete this" syntax 
// for Thread instance
//
class SuicideHelper: public Thread
{
private:

  // used to say the run(), 
  // there are threads to be deleted or the work must be finished
  ThreadEvent rotate_;

  // this is list of threads to be deleted
  vector<Thread*> threads_;
  
  // used to lock/unlock threads_ handling
  Mutex threads_lock_;

public:
 
  ///
  SuicideHelper();

  ///
  ~SuicideHelper();

  ///
  void run();

  // method to be invoked by ~Thread() destructor
  void deleteThread(Thread* thread_ptr);
};

SuicideHelper::SuicideHelper(){}

SuicideHelper::~SuicideHelper()
{
  if(is_running()){
  	cancel();
    rotate_.notify();
  }
}

void SuicideHelper::deleteThread(Thread* thread_ptr)
{
  threads_lock_.lock();
  threads_.push_back(thread_ptr);
  threads_lock_.unlock();
  if(is_running()) rotate_.notify();
  else start();
}

void SuicideHelper::run()
{
  do{
    threads_lock_.lock();
    for(unsigned int i=0;i<threads_.size();++i)
    {
      delete threads_[i];
    }
    threads_.clear();
    threads_lock_.unlock();
    rotate_.wait();
    testcancel();
  }while(true);
}

static SuicideHelper cleaner;

Thread::Thread()
{
  hndl_ = 0;
  error_code_ = 0;
  running_=false;
  do_break_=false;
}

Thread::~Thread()
{
 
 do_break_=true;
  { 
    MutexLocker ml(idMutex_);
    if (hndl_==0) return;
  }
  StrExTranslator se;
  se.touch();
  if( (ThreadContext*)(TlsGetValue(ThreadContextSingleton::getKey())) != NULL) {
  	  delete (ThreadContext*)(TlsGetValue(ThreadContextSingleton::getKey()));
      TlsSetValue(ThreadContextSingleton::getKey(),NULL);
  }
  if(threadID_ == GetCurrentThreadId()){
    cleaner.deleteThread(this);
    exit();
  }else{
    // if (is_running()) cancel();
    join(*this);
    if (!CloseHandle(hndl_)){
      ThreadingError::lastErrorNotification();
    }
  }
}

void* Thread::thread_process(void* v)
{
  Thread* th = (Thread*)v;
  try{
    {
      WriteLocker wl(th->runningRWLock_);
      th->running_ = true;
      MutexLocker ml(th->idMutex_);
    }
	th->context_ = new ThreadContext(th);
    if(!th->do_break_) th->run();
    {
      WriteLocker wl(th->runningRWLock_);
      th->running_ = false;
    }
  }catch(...){
   th->running_ = false;
   th->error_code_ = -1;
   throw;
  }
  StrExTranslator se;
  se.touch();
  if(th->context_ != NULL)
  	delete th->context_;
  ExitThread(0);
  return NULL;
}

void Thread::start()
{
  {
  StrExTranslator se;
  se.touch();
  MutexLocker ml(idMutex_);
  hndl_=CreateThread(
       NULL, // the handle cannot be inherited 
       0,    // default initial size of the stack, 
             // the same as the calling thread
       (LPTHREAD_START_ROUTINE)thread_process, 
             // pointer to the application-defined function 
             // being executed by the thread 
       (void*)this, // parameter value passed to the thread
       0,    // means the thread runs immediately after creation
       &threadID_ 
             // NB: variable that receives the thread identifier. 
     ); 
  }
  if (hndl_ == NULL){
    ThreadingError::lastErrorNotification();
  }
}

void Thread::cancel()
{
  do_break_=true;
}

bool Thread::is_running() const throw (ThreadingExceptions::NoResources,
                                  ThreadingExceptions::InternalError)
{
   ReadLocker rl(runningRWLock_);
   return running_;
}

void Thread::exit() 
{
  {
    WriteLocker wl(runningRWLock_);
    running_=false;
  }
  {
    StrExTranslator se;
    se.touch();
  if( (ThreadContext*)(TlsGetValue(ThreadContextSingleton::getKey())) != NULL)
  	  delete (ThreadContext*)(TlsGetValue(ThreadContextSingleton::getKey()));
    ExitThread(0);
  }
}

void Thread::testcancel() 
{
  StrExTranslator se;
  se.touch();
  if(threadID_==GetCurrentThreadId()){
    if(do_break_){
       exit();
    }
  }
}

void Thread::sleep(unsigned int sec)
{
   ThreadContextSingleton::cleanup();			
   Sleep(sec*1000);
   testcancel(); 
}

void Thread::nanosleep(unsigned int nseconds, long nnanoseconds) throw()
{
   ThreadContextSingleton::cleanup();			
   Sleep(nseconds*1000 + nnanoseconds%1000);
   testcancel(); 
}

void Thread::sleepCurrent(unsigned int sec)
{
   ThreadContextSingleton::cleanup();			
   Sleep(sec*1000);
}

void Thread::nanosleepCurrent(unsigned int nseconds, long nnanoseconds) throw()
{
   ThreadContextSingleton::cleanup();			
   Sleep(nseconds*1000 + nnanoseconds%1000);
}

void Thread::yield()
{
  StrExTranslator se;
  se.touch();
  ThreadContextSingleton::cleanup();			
  Sleep(0);
}

void Thread::join(HANDLE id)
{
  StrExTranslator se;
  se.touch();
  if (id == GetCurrentThread()) return;

  if (WaitForSingleObject(id,INFINITE)==WAIT_FAILED) {
    ThreadingError::lastErrorNotification();
  }
}

Thread::id_type Thread::current_id() throw()
{
  //StrExTranslator se;
  //se.touch();
  //  GetCurrentThreadId always work ;)
  return GetCurrentThreadId();
}


# else
#  error "Non HAVE_PTHREAD_H non WIN32 are defined"
# endif
#endif

}

