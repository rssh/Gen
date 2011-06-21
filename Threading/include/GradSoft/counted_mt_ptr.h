#ifndef __GradSoft_counted_mt_ptr_h
#define __GradSoft_counted_mt_ptr_h

/*
 * counted_ptr.h --- definition of counted pointers .
 * (C) Ruslan Shevchenko, <ruslan@shevchenko.kiev.ua>, 1997
 * (C) Grad-Soft Ltd, <http://www.gradsoft.com.ua>, 2002
 * $Id: counted_mt_ptr.h,v 1.9 2004-06-12 10:53:01 rin Exp $
 */

#ifndef __GradSoft_ptrs_h
#include <GradSoft/ptrs.h>
#endif


/**
 *
 **/
namespace GradSoft
{


template<typename T, ptr::SafeProperty property>
class counted_mt_ptr;

template<typename T, ptr::SafeProperty property>
struct counted_mt_ptr_traits
{
 typedef counted_ptr<T,property> ptr_type;
 typedef typename internal::safe_traits<T,property>::safe_helper_type safe_helper_type;
};



/**
 * shared counted pointer (thread-safe);
 * \begin{itemize}
 *  \item we use this pointer in case, when we need share one pointer
 *  between multiple owners and this pointer must be accessible from parallel 
 *  threads;
 *  \item pointer is released, when last owner (counted_mt_ptr to one) is done;
 *\end{itemize} 
 **/
template<class T, ptr::SafeProperty property>
class counted_mt_ptr: public internal::ptr_base<T,counted_mt_ptr_traits<T,property> >
{
private:


  class Value {
   protected:
     friend class counted_mt_ptr;

     T* ptr_;
     mutable int refCount_;
     mutable Mutex  m_;

     Value(T* ptr) 
       :ptr_(ptr),refCount_(1),m_()
     {}
  
     static Value* inc(Value* pv) throw()
        { if (pv) { MutexLocker ml(pv->m_);  ++pv->refCount_; return pv; }
          else return pv;
        }

  };

  Value* pv_;

public:
   
   /**
    * initialize to NULL value.
    **/
   counted_mt_ptr()  throw()
    :pv_(NULL)
   {
   }

   /**
    * copy constructror.
    **/
   counted_mt_ptr(const counted_mt_ptr& x) throw ()
    :pv_(Value::inc(x.pv_))
   { 
   }

   /**
    * explicit constructor with pointer
    *\begin{verbatim}
    * Usage:
    *   counted_mt_ptr<My> my(new My());
    *\end{verbatim}
    **/
   explicit counted_mt_ptr(T* pT) throw (std::bad_alloc)
     :pv_((pT==NULL) ?  NULL : new Value(pT))
   {
   }

   /**
    * destructor.
    **/
   ~counted_mt_ptr()
   {
    decRef();
   }

   ///
   counted_mt_ptr operator=(const counted_mt_ptr& x);

   /**
    * return pointer to holded value 
    * Note: throw NullPointerException on NULL when safety is set to true.
    **/
   T* operator->() throw (NullPointerException)
       { return call_get(); }

   ///
   const T* operator->() const throw(NullPointerException)
    {  return call_get(); } 

   ///
   T& operator*()  throw (NullPointerException)
    { return call_get(); } 

   const T& operator*() const 
       { return call_get(); } 

   ///
   T* get() const throw()
       { 
         if (pv_==NULL) { return NULL; }
         else { MutexLocker ml(pv_->m_); return pv_->ptr_; }
       }


   T* call_get() throw(NullPointerException)
    {
      Value* pv = internal::safe_traits<Value,property>::safe_helper_type::call_get(pv_);
      {
       MutexLocker ml(pv_->m);
       return internal::ptr_base<T,counted_mt_ptr_traits<T,property> >::traits_type::safe_helper_type::call_get(pv->ptr_);
      }
    };


   /**
    * unsafe get
    **/
   T* get_() const throw()
       { return (pv_==NULL) ? NULL : pv_->ptr_; }

   void set(T* newT) throw(std::runtime_error);

   ///
   void assign(T* newT) throw(std::runtime_error);

   /**
    * unsafe assign
    **/
   void assign_(T* newT) throw(std::bad_alloc);

   ///
   int getRefCount(void) const  
        { if (pv_==NULL) return 0;
          { MutexLocker ml(pv_->mutex_); return pv_->refCount_; }
        }

   ///
   int getRefCount_(void) const throw() 
        { return (pv_==NULL) ? 0 : pv_->refCount_ ; }

   ///
   Mutex&  getMutex()
   {
     if (pv_==NULL) {
       Value* tmp = new Value(NULL); 
       pv_ = tmp;
     }
     return this.tmp_->mutex_;
   }

   ///
   bool operator==(const counted_ptr<T,property>& x) const
      { return pv_ == x.pv_; }

   ///
   bool operator==(const counted_mt_ptr& x) const
      { return pv_ == x.pv_; }

   ///
   bool operator==(const T* x) const
      { if (pv_ == NULL) return x==NULL; 
        else { MutexLocker ml(pv_->m_);
               return pv_->ptr_ == x; }
      }

   ///
   bool equal(const T* x) const
     { return (*this)==x; }
   
   /**
    * equal without mt checking.
    **/
   bool equal_(const T* x) const;

   
   ///
   bool isNULL() const //throw(ThreadingExceptions::Failure)
      { if (pv_==NULL) return true; 
        else { MutexLocker ml(pv_->m_); return pv_->ptr_==NULL; }
      }

   ///
   bool isNULL_() const //trow()
      { return pv_ == NULL || pv_->ptr_==NULL; }

   ///
   bool operator!=(const counted_mt_ptr& x) const
      { return pv_ != x.pv_; }

protected:

   void decRef()
   {
    Value* toDelete=NULL;
    if (pv_!=NULL) {
      MutexLocker ml(pv_->m_);
      if (--(pv_->refCount_)==0) {
         toDelete=pv_;
         pv_=NULL;
      }
    }
    if (toDelete!=NULL) {
      MutexLocker ml(toDelete->m_);
      if (toDelete->refCount_ == 0) {
         if (toDelete->ptr_) delete toDelete->ptr_;
      }else{
         return; /* it's owned by other pointer */
      }
    }
    delete toDelete;
   }

};

template<typename T, ptr::SafeProperty s>
counted_mt_ptr<T,s> counted_mt_ptr<T,s>::operator=(
                                             const counted_mt_ptr<T,s>& x)
{
 if (pv_!=x.pv_) {
   if (x.pv_!=NULL) {
       MutexLocker xml(x.pv_->m_);
       ++x.pv_->refCount_;
   }
   decRef();
   MutexLocker xml(x.pv_->m_);
   pv_=x.pv_;
 }
 return *this;
}

template<typename T, ptr::SafeProperty s>
void counted_mt_ptr<T,s>::set(T* newT) throw(std::runtime_error)
{
 if (pv_!=NULL) {
   if (pv_->ptr==newT) {
     MutexLocker ml(pv_->m_);
     if (pv_->ptr==newT) return;
   }
   decRef();
 }
 pv_ = ((newT!=NULL) ? new Value(newT) : NULL);
}

template<typename T, ptr::SafeProperty s>
void counted_mt_ptr<T,s>::assign(T* newT) throw(std::runtime_error)
{
 if (pv_!=NULL) {
  if (pv_->ptr_!=newT){
    MutexLocker ml(pv_->m_);
    pv_->ptr_=newT;
  }
 }else{
  pv_ = ((newT==NULL) ? NULL : new Value(newT));
 }
}


}

#endif
