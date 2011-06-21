#ifndef __GradSoft_AmountOfTime_h
#define __GradSoft_AmountOfTime_h

/*
 * Part of GradSoft C++ ToolBox
 * (C) GradSoft, Kiev, Ukraine 2001
 * http://www.gradsoft.com.ua
 * $Id: TimeStamp.h,v 1.4 2002-08-02 18:52:04 rin Exp $
 *
 * "toSeconds();" 
 * Returns unix-like timestamp.
 * This means representation of date/time
 * as number of seconds since 1 january 1970
 * 0 hours 0 seconds.
 * Also it's common to use "set" when changing object properties
 * and get while reading ones.
 */

#ifdef WIN32
#include <GradSoft/TimingConfigNT.h>
#endif


#include <string>
#include <GradSoft/Conversion.h>
#include <GradSoft/TimeFetch.h>
#include <GradSoft/TimeZone.h>



namespace GradSoft {

/*
 * class, which represents Time
 */

 class TimeStamp
 {

  public:

    /*
     * Static members
     */

    // TODO: static  TimeStamp&  now() throw(SystemTimeExc);

	
    /*
     * Constructors
     */   

    // current time constructor
    TimeStamp(); // Set's date/time to the beginning of the so-called
                 // UNIX-epoch. 1st january of 1970 A.D. 0 minutes 0 seconds
                 // Set's time zone to default (unknown)
    // copy constructor
    TimeStamp(const TimeStamp& anotherTime);
    // initialize constructor
    TimeStamp(const TimeZone& zone,
              long year,
              short month,
              short day,
              short hour = 0,
              short minute = 0,
              short second = 0,
              short millisecond = 0
             ) throw (InvalidDate,InvalidTime,InvalidTimeStamp);
    // initialize julian day and ajust by seconds (seconds since beginning of that day)
    TimeStamp(const TimeZone& zone, long julianDay, long seconds) throw();
    // initialize by unix-like timestamp. Seconds starts count at 00:00:00 01.01.1970
    TimeStamp(const TimeZone& zone, long seconds) throw();

    /*
     * Primary functions to access object properties
     */   

    // Get properties

    // returns hour of day
    short getHours() const throw();
    // get minutes of hour
    short getMinutes() const throw();
    // get seconds of minute
    short getSeconds() const throw();
    // get milliseconds of second
    short getMilliseconds() const throw();
    // get number of day of month
    short getDay() const throw();
    // get number of day of year
    short getDayOfYear() const throw();
    // get julian day for this date - number of days passed since 01.01.-4712
    short getJulianDay() const throw();
    // get number of month of year
    short getMonth() const throw();
    // get year.
    long  getYear() const throw();
    // returns object's time zone 
    TimeZone& getTimeZone() const throw();

    // Set properties

    // set hours of a day
    void setHours(short hours) throw(InvalidTime,InvalidTimeStamp) ;
    // set minutes of an hour
    void setMinutes(short minutes) throw(InvalidTime,InvalidTimeStamp);
    // set seconds of a minute
    void setSeconds(short seconds) throw(InvalidTime,InvalidTimeStamp);
    // set milliseconds of second
    void setMillisec(short milliseconds) throw(InvalidTime,InvalidTimeStamp); 
    // set day of a month
    void setDay(short dayOfMonth) throw(InvalidDate,InvalidTimeStamp);
    // set day of year
    void setDayOfYear(short dayOfYear) throw(InvalidDate,InvalidTimeStamp);
    // get julian day
    void setJulianDay(long jd) const throw(InvalidDate,InvalidTimeStamp);
    // set month of year
    void setMonth(short month) throw(InvalidDate,InvalidTimeStamp);
    // set year "anno Domini"
    void setYear(short year) throw(InvalidDate,InvalidTimeStamp); 
    // set time zone (offset to UTC)
    void setTimeZone (const TimeZone& zone);

    /*
     *  UNIX-like timestamp functions.
     */

    // returns number of seconds since 1970-01-01T00:00:00 in the timezone
    long toSeconds() const throw(); 
    // set date/time as number of seconds since 1970-01-01T00:00:00 in the timezone
    long asSeconds(long nSeconds,
                     short hOffsetGMT = 0,
                     short mOffsetGMT = 0) const throw(InvalidTimeStamp); 

    /*
     * Redefine operations to simplify use
     */   

    // arithmetics

    TimeStamp    operator+ (const AmountOfTime& amount) const throw(TimeZone::UnknownTimeZone);
    TimeStamp&   operator+=(const AmountOfTime& amount) throw(TimeZone::UnknownTimeZone);
    TimeStamp    operator- (const AmountOfTime& amount) const throw(TimeZone::UnknownTimeZone);
    TimeStamp&   operator-=(const AmountOfTime& amount) throw(TimeZone::UnknownTimeZone);

    AmountOfTime operator- (const TimeStamp& another) const throw(TimeZone::UnknownTimeZone);

    // comparison

    bool   operator <= (const TimeStamp& anotherTime) const throw(TimeZone::UnknownTimeZone);
    bool   operator <  (const TimeStamp& anotherTime)  const throw(TimeZone::UnknownTimeZone);
    bool   operator >= (const TimeStamp& anotherTime) const throw(TimeZone::UnknownTimeZone);
    bool   operator >  (const TimeStamp& anotherTime)  const throw(TimeZone::UnknownTimeZone);
    bool   operator != (const TimeStamp& anotherTime) const throw(TimeZone::UnknownTimeZone);
    bool   operator == (const TimeStamp& anotherTime) const throw(TimeZone::UnknownTimeZone);

    // TODO: char*   operator << (const TimeStamp& anotherTime) const throw();


    /*
     * More complex functions
     */   

    // set by given date/time
    void set(short year,
             short month,
             short day,
             short hour,
             short minute,
             short second,
             short milsec,
             const TimeZone& zone)
          throw (InvalidDate,InvalidTime,InvalidTimeStamp);

    // set date/time by system time
    void setNow() throw(SystemTimeNotAvaliable);

  private:

    // NOTE: There  exist lack of big integers while using Microsoft C++

    // Traditional human representation of date/time
    long year_; // Grigorian year
    short month_; // Grigorian month
    short day_; // Grigorian day
    short hour_; // Grigorian hour
    short minute_; // Grigorian minute
    short second_; // Grigorian second

    // This part belongs to both types of representation, used here.
    short millisecond_; // I's milliseconds.

    // Second type of representation
    long daySeconds_; // Seconds passed since the midnight.
    long julianDay_; // Julian day for a Grigorian date
                     // - nice thing to do a lot of kinds of computation.

    TimeZone zone_; // Time zone.


 }; // of class TimeStamp



} // of GradSoft namespace



#endif
