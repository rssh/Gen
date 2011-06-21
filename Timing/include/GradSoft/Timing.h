#ifndef __GradSoft_Timing_h
#define __GradSoft_Timing_h

/*
 * Part of GradSoft C++ ToolBox
 * (C) GradSoft, Kiev, Ukraine 2001
 * http://www.gradsoft.com.ua
 * $Id: Timing.h,v 1.10 2002-08-01 18:13:07 rin Exp $
 * responsability: Bannij Michael <rin@mail.grad.kiev.ua>
 */

#ifdef WIN32
#include <GradSoft/TimingConfigNT.h>
#endif



#include <string>

#include <GradSoft/Conversion.h>
#include <GradSoft/TimeFetch.h>

#include <GradSoft/TimeStamp.h>
#include <GradSoft/AmountOfTime.h>



namespace GradSoft { // here placed exceptions

/*
 *  Genearal Timing exception class
 */
 class Exception
 {
  public:
    std::string message;
    const char* what() const { return message.c_str(); }
    Exception();
    Exception(const char* msg):message(msg) {}
 };

/*
 *  Exceptions to represent date/time faults
 */
 class InvalidTimestamp : public Ecxeption
 {  
  public:
    InvalidTimestamp():Exception("Invalid time/date parameter. "){}
 };

 class InvalidDate : public InvalidTimestamp
 {  
  public:
    InvalidDate():Exception("Invalid date parameter. "){}
 };

 class InvalidTime : public InvalidTimestamp
 {  
  public:
    InvalidTime():Exception("Invalid time parameter. "){}
 };
 
/*
 *  Exception to indicate inaccessibility of system time
 */
 class  SystemTimeNotAvaliable : public Exception
 {
  public:
    SystemTimeNotAvaliable(): Exception("System time is unavailable"){}
 };
 
/*
 *  TODO: find out responcebility of this class
 *  Bingo! It's used to indicate overflow while computing (toMilliseconds)
 */
 //class MillisecondsExc : public Exception
 //{
 // public:
 //     MillisecondsExc(): Exception ("Overflow of 'long' bounds"){}
 //};



} // of Gradsoft namespace

#endif

