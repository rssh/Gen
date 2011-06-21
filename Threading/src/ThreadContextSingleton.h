#ifndef __GradSoft_ThreadContextSingleton_h
#define __GradSoft_ThreadContextSingleton_h

/*
 * part of Grad-Soft C++ ToolBox.
 * (C) Grad-Soft Ltd, Kiev, Ukraine.
 * http://www.gradsoft.kiev.ua
 * $I$
 */

#ifndef __GradSoft_map_INCLUDED
#include <map>
#ifndef __GradSoft_map_INCLUDED
#define __GradSoft_map_INCLUDED
#endif
#endif

namespace GradSoft {

#ifndef WIN32
extern "C" void destroyThreadContext(void*);
#endif



class ThreadContextSingleton
{
public:

#ifdef WIN32
 static void registerNewContext(ThreadContext* context) 
 { singleton().registerNewContext_(context); }

 static void cleanup() { singleton().cleanup_(); }

 static unsigned int getKey() { return singleton().getKey_(); }
#else
 static pthread_key_t getKey() { return singleton().getKey_(); }
#endif


protected:

 static ThreadContextSingleton& singleton()
  { return singleton_; }

#ifdef WIN32
 unsigned int getKey_() { return key_; }

 void registerNewContext_(ThreadContext* context);

 void cleanup_();
#else
 pthread_key_t getKey_() { return key_; }
#endif

public:

 ThreadContextSingleton()
 {
#ifdef WIN32
	key_ = TlsAlloc();
#else
   pthread_key_create(&key_,destroyThreadContext);
#endif
 }

 ~ThreadContextSingleton()
 {
#ifdef WIN32
	TlsFree(key_);
//	unsigned long code;
    contextListLock_.write_lock();       
    for(std::map<HANDLE,ThreadContext*>::iterator it=contextList_.begin(); it!=contextList_.end(); ++it) {
     	if( !CloseHandle((*it).first) ){
    		ThreadingError::lastErrorNotification();
            contextListLock_.write_unlock();
        }
        if((*it).second != NULL) delete (*it).second;
    }
     contextList_.erase(contextList_.begin(),contextList_.end());
    contextListLock_.write_unlock();
#else
  pthread_key_delete(key_);
#endif
}
 

private:
 static ThreadContextSingleton singleton_;

#ifdef WIN32
 unsigned int key_;

 std::map<HANDLE,ThreadContext*> contextList_;

 RWLock contextListLock_;
#else
 pthread_key_t key_;
#endif

}; 

}

#endif
