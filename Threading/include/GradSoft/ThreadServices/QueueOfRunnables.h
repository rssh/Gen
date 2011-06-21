#ifndef __GradSoft_ThreadServices_QueueOfRunnables_h
#define __GradSoft_ThreadServices_QueueOfRunnables_h

/*
 * part of GradSoft C++ ToolBox: Threading
 * http://www.gradsoft.com.ua
 * (C) GradSoft, 2001
 * $Id: QueueOfRunnables.h,v 1.10 2004-03-13 09:53:26 rssh Exp $
 */

#ifndef __GradSoft_Runnable_h
#include <GradSoft/Runnable.h>
#endif

#ifndef __GradSoft_Mutex_h
#include <GradSoft/Mutex.h>
#endif

#ifndef __GradSoft_Thread_h
#include <GradSoft/Thread.h>
#endif

#ifndef __GradSoft_ThreadEvent_h
#include <GradSoft/ThreadEvent.h>
#endif


namespace GradSoft {


/**
 * Fixed-size queue of Runnable-s, often used in Thread-Services
 * use hairdressen algorithm
 **/
class __dll_threading QueueOfRunnables
{
public:

  /**
   * this exception is thrown from get .. family of functions,
   * when Queue is destructed.
   **/
   struct Destructed {};

private:

  Runnable** arrived_;
  mutable Mutex     arrivedMutex_;
  int       head_;
  int       tail_;
  int       nElements_;
  int       arrivedSize_;
  bool      destructed_;

  ThreadEvent  oneElementAppear_;
  ThreadEvent  freePlaceAppear_;

public:

  ///
  QueueOfRunnables(int maxSize);
  //
  ~QueueOfRunnables();

  /**
   * put runnable to queue; 
   * if queue is full, then wait until place will be freeded.
   **/
  void put(Runnable* runnable); 

  /**
   * try put \verb|runnable| to queue.
   * on success return true, otherwise (i. e. when queue is full) return false
   **/
  bool try_put(Runnable* runnable);


  /**
   * put runnable to queue 
   * if queue is full, then wait until place will be freeded during
   * timeout <code> timeout </code> (in seconds).
   *@return false, if timeout expired by queue is not smalled
   **/
  bool put_with_timeout(Runnable* runnable, long timeout);

  /**
   * get item from queue. If queue is empty, than wait until element
   * will be arrived.
   **/
  Runnable* get(); 

  /**
   * try get runnable from queue. on success return true, otherwise
   * (i. e. if queue is empty) immediatly return false
   **/
  Runnable* try_get();

  /**
   * get runnable from queue.
   **/
  Runnable* get_with_timeout(long timeout);

  ///
  bool  empty() const
  { MutexLocker ml(arrivedMutex_); return (nElements_ == 0); }

  ///
  bool  full() const
  { MutexLocker ml(arrivedMutex_); return nElements_ == arrivedSize_; }

  ///
  int  size() const
  { MutexLocker ml(arrivedMutex_); return nElements_; }

  ///
  int  max_size() const
  { return arrivedSize_; }

private:

  bool  empty_() const
  { return nElements_ == 0; }

  bool  full_() const
  { return nElements_ == arrivedSize_; }

  Runnable* real_get_();
  void      real_put_(Runnable*);

  QueueOfRunnables(const QueueOfRunnables&);
  QueueOfRunnables& operator=(const QueueOfRunnables&);

};

}


#endif
