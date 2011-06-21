#ifndef __GradSoft_ThreadContext_h
#define __GradSoft_ThreadContext_h

/*
 * part of GradSoft C++ ToolBox.
 * (C) Grad-Soft Ltd, Kiev, Ukraine.
 * http://www.gradsoft.kiev.ua
 * (C) Ruslan Shevchenko, Kiev, Ukraine
 * <ruslan@shevchenko.kiev.ua>
 * $Id: ThreadContext.h,v 1.7 2004-03-13 09:53:24 rssh Exp $
 */

#ifndef __GradSoft_ThreadingPostConfig_h
#include <GradSoft/ThreadingPostConfig.h>
#endif


#ifndef __GradSoft_deque_INCLUDED
#include <deque>
#ifndef __GradSoft_deque_INCLUDED
#define __GradSoft_deque_INCLUDED
#endif
#endif

#ifdef WIN32
#include <windows.h>
#endif

/**
 *
 **/
namespace GradSoft {

class Thread;
#ifndef WIN32
extern "C" void destroyThreadContext(void*);
#endif

/**
 * Thread-Specific context.
 **/
class __dll_threading ThreadContext
{
public:

  /**
   * Abstract class for one slot in ThreadContext.
   * Subclasses must be difined by user.
   **/
  class __dll_threading Slot
  {
   public:
     virtual ~Slot() {}
  };

public:

  /**
   * get current thread.
   * return NULL, is this thread is not created
   * by our ToolBox.
   **/
  Thread* getThread();

  /**
   * get Context of current thread.
   * (note, that ThreadContext is automatically created on this call.)
   **/
  static ThreadContext* current();

  /**
   * alloc slot in current context and return it's index.
   **/
  unsigned int    allocSlot(Slot* slot);

  /**
   * alloc slot on specified index.
   **/
  bool   allocSlot(Slot* slot, unsigned int index);

  /**
   * return slot on specified index or NULL, if slot does not exists.
   **/
  Slot*  getSlot(unsigned int index);

private:

  std::deque<Slot*> slots_;
  Thread*           thread_;
  
 friend class Thread;
 friend class ThreadContextSingleton;

#ifndef WIN32
 friend void destroyThreadContext(void*);
#endif


  ThreadContext(Thread*);
  ~ThreadContext();

};

}

#endif
