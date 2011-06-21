#ifndef __GradSoft_safe_auto_ptr_h
#define __GradSoft_safe_auto_ptr_h

/*
 * part of Grad-Soft C++ ToolBox.
 * (C) Grad-Soft Ltd, Kiev, Ukraine. 2002
 * http://www.gradsoft.kiev.ua
 * $Id: safe_auto_ptr.h,v 1.5 2002-10-16 09:00:58 rssh Exp $
 */

#ifndef __GradSoft_ptrsPostConfig_h
#include <GradSoft/ptrsPostConfig.h>
#endif

#ifndef __GradSoft_memory_INCLUDED
#include <memory>
#ifndef __GradSoft_memory_INCLUDED
#define __GradSoft_memory_INCLUDED
#endif
#endif

/**
 *
 **/
namespace GradSoft
{

template <typename T> 
class safe_auto_ptr ;

template <typename T>
struct safe_auto_ptr_traits
{
 typedef safe_auto_ptr<T>  ptr_type;
};

/**
 * safe_auto_ptr: safe auto_ptr;
 * requirements for T type are the same, as in auto_ptr template.
 **/
template <typename T> 
class safe_auto_ptr:public internal::ptr_base<T,safe_auto_ptr_traits<T> >
{
private:
  mutable T* p_;

public:

  ///
  typedef T element_type;

public:

  ///
  explicit safe_auto_ptr(T* p = 0) throw() : p_(p) {}
  ///
  safe_auto_ptr(safe_auto_ptr& x) throw() : p_(x.release()) {}
  ///
  safe_auto_ptr(std::auto_ptr<T>& x) throw() : p_(x.release()) {}

  ///
  safe_auto_ptr& operator=(safe_auto_ptr& x) throw() 
  {
    if (&x != this) {
      delete p_;
      p_ = x.release();
    }
    return *this;
  }

  ///
  safe_auto_ptr& operator=(std::auto_ptr<T>& x) throw() 
  {
   if (x.get()!=p_) {
    delete p_;
    p_ = x.release();
   }
   return *this;
  }

  ///
  ~safe_auto_ptr() throw() 
    { if (p_) delete[] p_; }

  ///
  T* operator->() const throw(NullPointerException)
    { return call_get(); }

  ///
  T& operator*() const throw(NullPointerException)
    { return call_get(); }
   
  ///
  T* get() const throw() {
    return p_;
  }

  ///
  T* call_get() const throw(NullPointerException)
    { if (!p_) throw NullPointerException();
      return p_; }

  ///
  T* release()  throw() {
    T* tmp = p_;
    p_ = 0;
    return tmp;
  }


};

}

#endif
