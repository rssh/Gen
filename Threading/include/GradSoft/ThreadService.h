#ifndef __GradSoft_ThreadService_h
#define __GradSoft_ThreadService_h

/*
 * part of GradSoft C++ ToolBox
 * (C) GradSoft 2000, 2001
 * http://www.gradsoft.com.ua
 * $Id: ThreadService.h,v 1.11 2007-04-24 07:12:07 rssh Exp $
 */


#ifndef __GradSoft_Runnable_h
#include <GradSoft/Runnable.h>
#endif

namespace GradSoft {

/**
 * ThreadService: entity which process Runnable
 * (Runnable may be events, network connections, etc)
 * Typical usage pattern:
 *		  1. Generator generates Runnable
 *  2. this Runnables are passed to ThreadServices,
 *    with help of call ThreadService::process 
 *  3. ThreadService process this runnable, asynchronicly or
 *   synchronisly.
 *
 * ThreadService can be in active or inactive state.
 * When it in active state, it can process requests.
 * When in inactive - can't.
		     * 
 **/
class __dll_threading ThreadService
{
public:

   /**
    * This exception is throwed, when we try to use
    * not-activated ThreadService
    **/
   struct NotActive {int dummy;};

private:

   volatile bool active_;

public:

   ///
   ThreadService();

   ///
   virtual ~ThreadService();

   ///
   virtual void  process(Runnable* runnable)=0;

   ///
   bool is_active() const { return active_; }

   ///
   virtual void  activate();

   ///
   virtual void  deactivate(bool shutdown);

protected:

   virtual void mark_deactivate();

   class DeactivateRunnable: public Runnable  
   {
    ThreadService* owner_p_;
   public:
      DeactivateRunnable(ThreadService* owner_p) 
        :owner_p_(owner_p)
      {}
        
      void run() 
       { owner_p_->mark_deactivate(); delete this; }

   };

   friend class DeactivateRunnable;

private:

   ThreadService(const ThreadService&);
   ThreadService& operator=(const ThreadService&);

};

}

#endif
