#ifndef __GradSoft_ThreadingUtils_h
#define __GradSoft_ThreadingUtils_h

/*
 * part of GradSoft C++ ToolBox
 * (C) GradSoft 2000, 2001
 * http://www.gradsoft.com.ua
 * $Id: ThreadingUtils.h,v 1.8 2004-03-13 09:53:25 rssh Exp $
 */

#ifndef __GradSoft_ThreadingExceptions_h
#include <GradSoft/ThreadingExceptions.h>
#endif

#ifdef WIN32
#include <EH.H>
#include <windows.h>
#else
#include <stdlib.h>
  // for NULL
#endif



namespace GradSoft {

/*
 * This class ThreadingError provides checking 
 * of service function's error code 
 * and throwing a ThreadingExceptions "member" class
 */
class __dll_threading ThreadingError
{
protected:
  friend class Mutex;
#ifndef WIN32
  friend class RWLock;
#endif
  friend class Thread;
  friend class ThreadEvent;
  friend class ThreadContextSingleton;
  friend class ThreadContext;
#ifdef WIN32
  static void lastErrorNotification();
#else
  static void checkError(int errorCode=0, 
                         const char* name=NULL, bool ownMessage=false );
#endif
};

#ifdef WIN32

/*
 * This class StrExTranslator provides translation
 * of structured C exceptions to typed C++ exceptions
 */
class __dll_threading StrExTranslator{

private: 

  // pointer to SE translation function 
  // registered by _set_se_translator:

  _se_translator_function previous_;

public:

 /* 
  * SE translation function 
  */
  static void 
  se_translator(unsigned int errCode, _EXCEPTION_POINTERS* pExp);

 /*
  * install a new SE translation function 
  * with saving a pointer to previous one
  */
  StrExTranslator();
 

 /*
  * restore the previous SE translator function
  */
  ~StrExTranslator();

 /* 
  * do nothing 
  * (invoke this method after creation of the object
  * to ensure destructor was called when exception occured )
  */
  void touch(void){} 

};

namespace ThreadingWrapper
{
 /*
  * This is wrapper function for InitializeCriticalSection() 
  * service function defined in <winbase.h>;
  * This wrapper function provides catching of structured 
  * "C" exceptions and mapping ones into typed "C++" exceptions.
  */
  void InitializeCriticalSection(LPCRITICAL_SECTION parameter);

}//namespace ThreadingWrapper

#endif//WIN32

}//namespace GradSoft 

#endif//__GradSoft_ThreadingUtils_h
