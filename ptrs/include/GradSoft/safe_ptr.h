#ifndef __GradSoft_safe_ptr_h
#define __GradSoft_safe_ptr_h

/*
 * part of Grad-Soft C++ ToolBox.
 * (C) Grad-Soft Ltd, Kiev, Ukraine. 2002
 * http://www.gradsoft.kiev.ua
 * $Id: safe_ptr.h,v 1.6 2002-03-27 20:02:34 rssh Exp $
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
template<typename T>
class safe_ptr;

///
template<typename T>
struct safe_ptr_traits
{
 typedef safe_ptr<T> ptr_type;
};

#ifndef HAVE_PARTIAL_SPECIALIZATION
/*@{*/
namespace internal {
 // All primitive types must be here, i. e. if you want correct ptr to
 // primitive type, then just insert empty ptr_base definition here in case
 // if you compiler does not support partial specialization.
template<>
class ptr_base<int,safe_ptr_traits<int> >
{
public:
 typedef int element_type;
};
template<>
class ptr_base<unsigned int,safe_ptr_traits<unsigned int> >
{
public:
 typedef unsigned int element_type;
};
}
/*@}*/
#endif


/**
 * safe_ptr : pointer wrapper, which throw exception on NULL access.
 **/
template <typename T> 
class safe_ptr:public internal::ptr_base<T,safe_ptr_traits<T> >
{
private:
  T* p_;

public:

  /**
   * constructor
   **/
  explicit safe_ptr(T* p = NULL) throw() : p_(p) {}

  ///
  safe_ptr& operator=(T* p)
  { p_=p; return *this; }

  ///
  T* operator->() const throw(NullPointerException)
    { return safe_get(); } 

  ///
  T& operator*() const throw(NullPointerException)
    { return *safe_get(); } 

  /**
   * return pointer.
   **/
  T* get() const throw() {
    return p_;
  }

  /**
   * return wrapped pointer, if one is not Null;
   * otherwise throw NulPointerException.
   **/
  T* call_get() const throw(NullPointerException)
   { return safe_get(); }


protected:

  T*  safe_get() const throw(NullPointerException)
  { if (p_==NULL) throw NullPointerException();
    return p_; }

};

}

#endif
