#ifndef __GradSoft_ptrsPostConfig_h
#define __GradSoft_ptrsPostConfig_h

/*
 * part of Grad-Soft C++ ToolBox.
 * (C) Grad-Soft Ltd, Kiev, Ukraine, 2002
 * http://www.gradsoft.kiev.ua
 * $Id: ptrsPostConfig.h,v 1.3 2002-03-27 20:02:34 rssh Exp $
 */

#ifdef HAVE_CONFIG_H
#include <GradSoft/ptrsConfig.h>
#endif

#ifndef __GradSoft_ptrExceptions_h
#include <GradSoft/ptrExceptions.h>
#endif

/**
 *
 **/
namespace GradSoft {

///

/**
 * this is namespace for ptr categories.
 **/
namespace ptr
{
 ///
 typedef bool SafeProperty;
 ///
 const SafeProperty safe = true;
 ///
 const SafeProperty unsafe = false;
}

/**
 * Member function, binded with object.
 */
template<typename ClassType, typename ReturnType, typename Signature>
class MemberFunctionCall
{
public:
  ///
  typedef ClassType  class_type;
  ///
  typedef ReturnType return_type;
  ///
  typedef Signature  member_function_type;
private:
  ClassType* object_;
  member_function_type fun_;
public:

  ///
  MemberFunctionCall(ClassType* obj, Signature fun)
   :object_(obj),fun_(fun) {}

  ///
  ReturnType operator()() const
   { return (object_->*fun_)(); }

 //TODO:
 // enable when compiler will suppopt this.
 //template<class Param>
 //ReturnType operator()(Param p) const
 //  { return (object_->*fun_)(p); }

};

/**
 * void member function, binded with object.
 **/
template<typename ClassType, typename Signature>
class VoidMemberFunctionCall
{
public:
  ///
  typedef ClassType  class_type;
  ///
  typedef Signature  member_function_type;
private:
  ClassType* object_;
  member_function_type fun_;
public:

  ///
  VoidMemberFunctionCall(ClassType* obj, Signature fun)
   :object_(obj),fun_(fun) {}

  ///
  void operator()() const
   { (object_->*fun_)(); }

};

/**
 *
 **/
namespace internal {

template<typename T, typename PtrTraits>
class ptr_base
{
public:
  typedef  T element_type; 
  typedef  PtrTraits  traits_type;

  //
  // TODO:
  //   analyze inlining and put -> and * operators here. 


  ///
  VoidMemberFunctionCall<T,void(T::*)(void)>operator->*(void (T::*f)(void)) 
   {return VoidMemberFunctionCall<T,void(T::*)(void)>(
          (static_cast<typename PtrTraits::ptr_type*>(this))->call_get(),f); }

  ///
  MemberFunctionCall<T,bool,bool (T::*)()> operator->*(bool (T::*f)()) 
  { return MemberFunctionCall<T,bool,bool(T::*)(void)>(
          (static_cast<typename PtrTraits::ptr_type*>(this))->call_get(),f); }
  // todo: template must be here, when MFVC will work.


};

// now specialize for primitive types.
//
#ifdef HAVE_PARTIAL_SPECIALIZATION
 // WIN32 does not support partial specialization.
template<typename PtrTraits>
class ptr_base<char,PtrTraits>
{
public:
  typedef char element_type; 
};
template<typename PtrTraits>
class ptr_base<unsigned char,PtrTraits>
{
public:
  typedef unsigned char element_type; 
};
template<typename PtrTraits>
class ptr_base<int,PtrTraits>
{
public:
  typedef int element_type; 
};
template<typename PtrTraits>
class ptr_base<unsigned int,PtrTraits>
{
public:
  typedef unsigned int element_type; 
};
template<typename PtrTraits>
class ptr_base<double,PtrTraits>
{
public:
  typedef double element_type; 
};
#endif

#ifdef HAVE_PARTIAL_SPECIALIZATION
template<typename T, ptr::SafeProperty property>
struct safe_helper;
template<typename T>
struct safe_helper<T,ptr::safe>
{
  static T* call_get(T* x) throw(NullPointerException)
    { if (x==NULL) throw NullPointerException();
      return x;}
};
template<typename T>
struct safe_helper<T,ptr::unsafe>
{
  static T* call_get(T* x) throw()
  { return x; }
};
#endif

///
template<typename T, ptr::SafeProperty property>
struct safe_traits
{
#ifndef HAVE_PARTIAL_SPECIALIZATION
 template<ptr::SafeProperty>
 struct safe_helper
 {};
  // MSVC allow explicit specialization in struct scope instead.
 template<>
 struct safe_helper<ptr::safe>
 {
  static T* call_get(T* x) throw(NullPointerException)
    { if (x==NULL) throw NullPointerException();
      return x;}
 };
 template<>
 struct safe_helper<ptr::unsafe>
 {
  static T* call_get(T* x) throw()
   { return x; }
 };
 typedef safe_helper<property> safe_helper_type; 
#else
 typedef safe_helper<T,property> safe_helper_type; 
#endif
};

}// end namespace internal

}

#endif
