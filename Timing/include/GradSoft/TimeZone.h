#ifndef __GradSoft_TimeZone_h
#define __GradSoft_TimeZone_h

/*
 * Part of GradSoft C++ ToolBox
 * (C) GradSoft, Kiev, Ukraine 2001
 * http://www.gradsoft.com.ua
 * $Id: TimeZone.h,v 1.1 2002-08-01 18:13:07 rin Exp $
 *
 */

#ifdef WIN32
#include <GradSoft/TimingConfigNT.h>
#endif


#include <string>
#include <GradSoft/Conversion.h>
#include <GradSoft/TimeFetch.h>
#include <GradSoft/AmountOfTime.h>



namespace GradSoft : private AmountOfTime{

/*
 * class, which represents TimeZone.
 * Extended in extra class because of possible
 * unknowness of timezone. Also keep in mind DST(Daylight Saving Time).
 * Hours is from {-99, ..., 99} and minutes is from {0, ..., 59},
 * otherwise InvalidOffsetTime will be raised.
 *
 * IF realized addition - InvalidOffsetTime will also be raised while owerflow occurs.
 *
 */

 class TimeZone
 {
  public:
    
    /*
     * Time Zone Exceptions
     */

    class TimeZoneUnknown : public Exception
    {  
     public:
       InvalidTime():Exception("Time zone unknown. "){}
    };

    class InvalidOffsetTime : public InvalidTime
    {  
     public:
       InvalidTime():Exception("Invalid offset time parameter. "){}
    };

    /*
     * Static members
     */

    // TODO: static TimeZone&  systemTimeZone();

    /*
     * Constructors
     */

    TimeZone();

    TimeZone(short hours, short minutes = 0) throw (InvalidTime);

    /*
     * Property access functions
     */

    // get minutes offset
    short getMinutes() const throw(TimeZoneUnknown);
    // get hours offset
    short getHours() const throw(TimeZoneUnknown);

    // set minutes offset, when to be called
    // on the object with uncnown timezone,
    // causes hours set to zero
    void setMinutes(short minutes) throw(InvalidOffsetTime);
    // set hours offset, when to be called
    // on the object with uncnown timezone,
    // causes minutes set to zero
    void setHours(short hours) throw(InvalidOffsetTime);

    /*
     * Redefine operators to simplify use
     */

    bool operator<  (const AmountOfTime& anotherAmount) const throw(InvalidOffsetTime);
    bool operator<= (const AmountOfTime& anotherAmount) const throw(InvalidOffsetTime);
    bool operator>  (const AmountOfTime& anotherAmount) const throw(InvalidOffsetTime);
    bool operator>= (const AmountOfTime& anotherAmount) const throw(InvalidOffsetTime);    
    bool operator== (const AmountOfTime& anotherAmount) const throw(InvalidOffsetTime);
    bool operator!= (const AmountOfTime& anotherAmount) const throw(InvalidOffsetTime);

  private:

	 bool typeOfZone; // False means that zine is not defined

 }; // of TimeZone



} // of GradSoft namespace


#endif
