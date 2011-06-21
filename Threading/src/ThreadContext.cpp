
/*
 * part of GradSoft C++ ToolBox.
 * (C) Grad-Soft Ltd, Kiev, Ukraine.
 * http://www.gradsoft.kiev.ua
 * (C) Ruslan Shevchenko, Kiev, Ukraine
 * <ruslan@shevchenko.kiev.ua>
 * $Id: ThreadContext.cpp,v 1.6 2003-03-11 14:24:35 rssh Exp $
 */

#ifndef __GradSoft_ThreadContext_h
#include <GradSoft/ThreadContext.h>
#endif
#ifndef __GradSoft_Thread_h
#include <GradSoft/Thread.h>
#endif

#include <ThreadContextSingleton.h>
#include <deque>


using namespace std;

namespace GradSoft {


ThreadContextSingleton ThreadContextSingleton::singleton_;


  /**
   * get current thread.
   * return NULL, is this thread is not created
   * by our ToolBox.
   **/
Thread* ThreadContext::getThread()
{
 return thread_;
}

/**
 * get Context of current thread.
 * (note, that ThreadContext is automatically created on this call.)
 *
 **/
ThreadContext* ThreadContext::current()
{
#ifdef WIN32
 void* pv = TlsGetValue(ThreadContextSingleton::getKey());
#else
 void* pv = pthread_getspecific(ThreadContextSingleton::getKey());
#endif
 if (pv==NULL) {
   ThreadContext* retval = new ThreadContext(NULL);
#ifdef WIN32
   TlsSetValue(ThreadContextSingleton::getKey(),retval);
   ThreadContextSingleton::registerNewContext(retval);
#else
   pthread_setspecific(ThreadContextSingleton::getKey(),retval);
#endif
   return retval;
 }else{
   return static_cast<ThreadContext*>(pv);
 }
} 

/**
 * alloc slot in current context and return it's index.
 **/
unsigned int    ThreadContext::allocSlot(Slot* slot)
{
 unsigned int retval = (unsigned int)slots_.size();
 slots_.push_back(slot);
 return retval;
}

/**
 * alloc slot on specified index, if it yet not exists.
 **/
bool   ThreadContext::allocSlot(Slot* slot, unsigned int index)
{
 if (index < slots_.size()) {
   if (slots_[index]==NULL) {
     slots_[index]=slot;
   }else{
     return false;
   }
 }else {
   while(index != slots_.size()) {
     slots_.push_back(NULL);
   }
   slots_.push_back(slot);
 }
 return true;
} 

/**
 * return slot on specified index or NULL, if slot does not exists.
 **/
ThreadContext::Slot*  ThreadContext::getSlot(unsigned int index)
{
 if (index < slots_.size()) {
   return slots_[index];
 }else{
   return NULL;
 }
} 

ThreadContext::ThreadContext(Thread* thread)
 :slots_(),
  thread_(thread)
{
#ifdef WIN32
 TlsSetValue(ThreadContextSingleton::getKey(),this);
#else
 pthread_setspecific(ThreadContextSingleton::getKey(),this);
#endif
}

ThreadContext::~ThreadContext()
{
 for(deque<Slot*>::iterator it=slots_.begin(); it!=slots_.end(); ++it) {
   if ((*it)!=NULL) delete *it;
 }
#ifdef WIN32
	TlsSetValue(ThreadContextSingleton::getKey(),NULL);
#endif
}


#ifdef WIN32
void ThreadContextSingleton::registerNewContext_(ThreadContext* context)
{
   cleanup();
   HANDLE handle;
   BOOL ret = DuplicateHandle(GetCurrentProcess(),
                                GetCurrentThread(),
                               GetCurrentProcess(),
                               &handle,
                               0,
                               FALSE,
                               DUPLICATE_SAME_ACCESS);
   if ( !ret)
     ThreadingError::lastErrorNotification();
   contextListLock_.write_lock();
   contextList_.insert(std::map<HANDLE,ThreadContext*>::value_type(handle,context));
   contextListLock_.write_unlock(); 	
}


void ThreadContextSingleton::cleanup_()
{
 unsigned long code;
 std::map<HANDLE,ThreadContext*>::iterator it, tmp_it;
 contextListLock_.write_lock();
 for(it=contextList_.begin(); it!=contextList_.end(); ++it) {
 	
		if ( GetExitCodeThread((*it).first,&code) ){
        	if(code != STILL_ACTIVE){
        		if ( CloseHandle((*it).first) ){
            		if((*it).second != NULL) delete (*it).second;
            		tmp_it = it--;
            		contextList_.erase(tmp_it); //presuming that iterator is still validated        
                }
        	}
        }
 }
 contextListLock_.write_unlock();

}

#else
extern "C"
void destroyThreadContext(void* v)
{

 if (v!=NULL) {
   ThreadContext* ctx=static_cast<ThreadContext*>(v); 
   delete ctx;
   pthread_setspecific(ThreadContextSingleton::getKey(),NULL);
 }
} 
#endif

}
