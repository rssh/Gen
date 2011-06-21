#ifndef __GradSoft_counted_ptr_h
#define __GradSoft_counted_ptr_h

/*
 * counted_ptr.h --- definition of counted pointers .
 * (C) Ruslan Shevchenko, <ruslan@shevchenko.kiev.ua>, 1997
 * (C) Grad-Soft Ltd, <http://www.gradsoft.com.ua>, 2002
 * $Id: counted_ptr.h,v 1.7 2004-06-11 18:08:07 rin Exp $
 */

#ifndef __GradSoft_ptrsPostConfig_h
#include <GradSoft/ptrsPostConfig.h>
#endif

/**
 *
 **/
namespace GradSoft
{

///
template<typename T, ptr::SafeProperty property>
class counted_ptr;

///
template<typename T, ptr::SafeProperty property>
struct counted_ptr_traits
{
 ///
 typedef counted_ptr<T,property> ptr_type;
 ///
 typedef typename internal::safe_traits<T,property>::safe_helper_type safe_helper_type;
};

/**
 * shared counted pointer, (not thread-safe);
 *\begin{itemize}
 *  \item we use this pointer in case, when we need share one pointer
 * between multiple owners; pointer is released, when
 * last owner (counted_ptr ) is released;
 *  \item second template paramer (template<T,ptr::SafeProperty>) means safety:
 * when safe_property is true, we throw NullPointerException on NULL access.
 * otherwise - this is unsafe counted pointer wrapper.
 *\end{itemize}
 **/
template<class T,ptr::SafeProperty property>
class counted_ptr: public internal::ptr_base<T,counted_ptr_traits<T,property> >
{
private:

  class Value {

   protected:
     friend class counted_pptr;

     T* ptr;
     mutable int refCount_;

     Value(T* _ptr) 
       :ptr(_ptr),refCount_(1)
     {}
  };

  Value* pv_;

public:
   
   /**
    * initialize to NULL value.
    **/
   counted_ptr()  throw()
    :pv_(NULL)
   {
   }

   /**
    * copy constructror.
    **/
   counted_ptr(const counted_ptr& x) throw ()
    :pv_(x.pv_)
   { 
    if (x.pv_!=NULL) ++x.pv_->refCount_; 
   }

   /**
    * explicit constructor with pointer;
    *\begin{verbatim}
    * Usage:
    *   counted_ptr<My> my(new My());
    *\end{verbatim}
    **/
   explicit counted_ptr(T* pT) throw (std::bad_alloc)
     :pv_((pT==NULL) ?  NULL : new Value(pT))
   {
   }

   /**
    * destructor.
    **/
   ~counted_ptr()
   {
    decRef();
   }

   ///
   counted_ptr operator=(const counted_ptr& x) throw ();

   /**
    * return pointer to T*.
    *\begin{itemize}
    *  \item when safe: throw exception NullPointerException on NULL pointer.
    *  \item when unsafe: return NULL pointer as NULL pointer ;)
    *\end{itemize}
    **/
   T* operator->() throw (NullPointerException)
    { return call_get(); }

   /**
    * const version of operator->
    **/
   const T* operator->() const throw(NullPointerException)
    { return call_get(); }

   /**
    *reference operator
    *\begin{itemize}
    * \item when safe:
    *   return internal pointer by reference or throw NullPointerException if
    *   internal pointer is NULL 
    * \item when unsafe:
    *   return internal pointer by reference or do undefiend behaviour if 
    *   internal pointer is NULL.
    *\end{itemize}
    **/
   T& operator*()  
    { return *call_get(); }

   /**
    * const version of *() 
    **/
   const T& operator*() const 
    { return *call_get(); }

   /**
    * return internal pointer.
    **/
   T* get() const throw()
       { return (pv_==NULL) ? NULL : pv_->ptr_; }

   /**
    * set current wrapper to another pointer; analog of
    *\begin{verbatim}
    * x = new counted_ptr<T,safety>(pt);
    *\end{verbatim}
    **/
   void set(T* pT) throw(std::bad_alloc);

   /**
    *\begin{itemize}
    *  \item assign another value to internal shared value;
    *  \item all counted\_ptr-s, which was pointed on old T,
    *      will pointed to newT;
    *\end{itemize}
    **/
   void assign(T* newT) throw(std::bad_alloc);

   ///
   int getRefCount(void) const  throw() 
        { return pv_==NULL ? 0 : pv_->refCount_; }

   ///
   bool operator==(const counted_ptr& x) const throw()
      { return pv_ == x.pv_; }

   bool operator==(const T* x) const
      { return (pv_==NULL) ? (x==NULL) : (pv_->ptr_ == x); }

   ///
   bool isNULL(void) const throw()
      { return pv_ == NULL; }

   ///
   bool isNull(void) const throw()
      { return pv_ == NULL; }

   ///
   bool operator!=(const counted_ptr& x) const
      { return pv_ != x.pv_; }

   ///
   bool operator!=(const T* x) const
      { return (pv_==NULL) ? x!=NULL : pv_->ptr_ != x; }

protected:

   void decRef()
   {
    if (pv_!=NULL) {
      if ( --(pv_->refCount_) == 0 ) {
         if (pv_->ptr_) delete pv_->ptr_;
         delete pv_;
         pv_=NULL;
      }
    }
   }


   T* call_get() throw(NullPointerException)
    {
      Value* pv = internal::safe_traits<Value,property>::safe_helper_type::call_get(pv_);
      return internal::ptr_base<T,counted_ptr_traits<T,property> >::traits_type::safe_helper_type::call_get(pv->ptr_);
    };

};

template<typename T, ptr::SafeProperty S>
inline
counted_ptr<T,S> counted_ptr<T,S>::operator=(const counted_ptr<T,S>& x) throw()
{
 if (pv_!=x.pv_) {
   if (x.pv_!=NULL) ++x.pv_->refCount_;
   decRef();
   pv_=x.pv_;
 }
 return *this;
}

template<typename T, ptr::SafeProperty S>
void counted_ptr<T,S>::set(T* newT) throw(std::bad_alloc)
{
 if (pv_!=NULL) {
  if (pv_->ptr_==newT) return;
  if (--pv_->refCount_==0) {
    pv_->ptr_=newT;
    pv_->refCount_=1;
    return;
  }
 }
 pv_ = ((newT==NULL) ? NULL : new Value(newT));
}
 

template<typename T, ptr::SafeProperty S>
void counted_ptr<T,S>::assign(T* newT) throw(std::bad_alloc)
{
 if (pv_!=NULL) {
  if (pv_->ptr_==newT){
    return;
  }else{
    if ( --pv_->refCount_ == 0 ) delete pv_->ptr_;
    if (newT !=NULL) {
      pv_->ptr_=newT;
      pv_->refCount_=1;
      return;
    }else{
      pv_=NULL;
    }
  }
 }else{
  pv_ = ((newT==NULL) ? NULL : new Value(newT));
 }
}


}

#endif
