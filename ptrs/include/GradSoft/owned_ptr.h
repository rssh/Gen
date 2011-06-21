#ifndef __GradSoft_owned_ptr_h
#define __GradSoft_owned_ptr_h

/*
 * part of Grad-Soft C++ ToolBox.
 * (C) Grad-Soft Ltd, Kiev, Ukraine. 2002
 * http://www.gradsoft.kiev.ua
 * $Id: owned_ptr.h,v 1.8 2002-10-16 09:00:57 rssh Exp $
 */

#ifndef __GradSoft_ptrsPostConfig_h
#include <GradSoft/ptrsPostConfig.h>
#endif


/**
 *
 **/
namespace GradSoft
{

template <typename T, ptr::SafeProperty property>
class owned_ptr;


///
template <typename T, ptr::SafeProperty property>
struct owned_ptr_traits
{
 typedef owned_ptr<T,property> ptr_type;
 typedef typename internal::safe_traits<T,property>::safe_helper_type safe_helper_type;
};


/**
 * owned_ptr: combination of pointer and explicit ownity flag;
 * \begin{itemize}
 *   \item we use this pointer, when we want explicity set memory ownership 
 *         of pointer during transfer or copying;
 *   \item second template parameter means safety.
 * \end{itemize}
 **/
template <typename T, ptr::SafeProperty property> 
class owned_ptr:
     public internal::ptr_base<T,owned_ptr_traits<T,property> >
{
public:

  ///
  typedef owned_ptr_traits<T,property> traits_type;

private:
  T* p_;
  bool ownity_;

public:

  ///
  owned_ptr(T* p = NULL, bool ownity=false) throw() : p_(p),ownity_(ownity) {}

  ///
  owned_ptr& set(owned_ptr& x, bool passOwnership)
  {
    if (&x!=this){
      if (ownity_) delete p_;
      if (passOwnership) {
        ownity_=x.ownity_;
        p_ = x.p_;
        x.ownity_ = false;
      }else{
        ownity_ = false;
        p_ = x.p_;
      }
    }
    return *this;
  }

  ///
  owned_ptr& set(T* x, bool passOwnership)
  {
    if (x!=p_){
      if (ownity_ && p_!=NULL) delete p_;
      ownity_ = passOwnership;
      p_ = x;
    }
  }

  ///
  ~owned_ptr() throw() 
    { if (ownity_) delete p_; }

  ///
  T* operator->() const throw(NullPointerException)
    { return traits_type::safe_helper_type::call_get(p_);  }

  ///
  T& operator*() const throw(NullPointerException)
    { return traits_type::safe_helper_type::call_get(p_);  }
    
  ///
  T* get() const throw() 
    { return p_; }

  ///
  bool get_ownity() const throw()
    { return ownity_; }

  ///
  T* call_get() const throw(NullPointerException) 
    { return traits_type::safe_helper_type::call_get(p_);  }
 
  ///
  T* release()  throw() {
    T* tmp = p_;
    p_ = 0;
    return tmp;
  }

  ///
  void reset(T* p) throw() {
    if (p_ && ownity_) delete p_;
    p_ = p;
    ownity_ = true;
  }

private:

  owned_ptr(const owned_ptr&);
  owned_ptr& operator=(const owned_ptr&);
  
};


}

#endif
