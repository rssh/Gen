#ifndef __GradSoft_ptrExceptions_h
#define __GradSoft_ptrExceptions_h

/*
 * ptrExceptions.h -- exceptions  
 * (C) Grad-Soft Ltd, <http://www.gradsoft.com.ua>, 2002
 * $Id: ptrExceptions.h,v 1.3 2002-03-27 20:02:34 rssh Exp $
 */


#ifndef __STDEXCEPT_
#include <stdexcept>
#endif


/**
 *
 **/
namespace GradSoft
{

/**
 * those exception is thrown, when we try acess to null pointer
 * throught safe pointer wrapper class.
 **/
struct NullPointerException: public std::runtime_error 
{
 NullPointerException()
  :std::runtime_error(std::string("Null pointer access"))
 {}
};


}

#endif
