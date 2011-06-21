#ifndef __GradSoft_AmountOfTime_h
#define __GradSoft_AmountOfTime_h

/*
 * Part of GradSoft C++ ToolBox
 * (C) GradSoft, Kiev, Ukraine 2001
 * http://www.gradsoft.com.ua
 * $Id: AmountOfTime.h,v 1.4 2002-08-01 18:13:06 rin Exp $
 *
 * Hierarchy of default includings:
 * millisecond -> second -> minute -> hour -> day -> month -> year
 *                                                     ^
 *                                                     |
 *                                                    week
 *
 */

#ifdef WIN32
#include <GradSoft/TimingConfigNT.h>
#endif


#include <string>
#include <GradSoft/Conversion.h>
#include <GradSoft/TimeFetch.h>



namespace GradSoft {

/*
 * class, which represents amount of time
 */

 class AmountOfTime
 {
  public:
    
    /*
     * Constructors
     */

    AmountOfTime();

    // NOTE: Reverse order of parameters!

    AmountOfTime(short milliseconds,
                 short seconds = 0,
                 short minutes = 0,
                 short hours = 0,
                 short days = 0) throw (InvalidTimeStamp);


    /*
     * Property access functions
     */

    // nearest high boundary dependent functions

    // get milliseconds of second
    short getMilliseconds() const throw();
    // get seconds of minute
    short getSeconds() const throw();
    // get minute of hour
    short getMinutes() const throw();
    // get hour of day
    short getHours() const throw();
    // get days
    long  getDays() const throw();

    // set miliseconds of second
    void setMilliseconds(short milliseconds) throw (InvalidTimeStamp);
    // set seconds of minute
    void setSeconds(short seconds) throw(InvalidTimeStamp);
    // set minutes of hour
    void setMinutes(short minutes) throw(InvalidTimeStamp);
    // set hours of day
    void setHours(short hours) throw(InvalidTimeStamp);
    // set days
    void setDays(long days) throw(InvalidTimeStamp);

    // absolute functions

    // get length in milliseconds
    long toMilliseconds() const throw();
    // get length in seconds
    long toSeconds() const throw(); // unix-like function
    // get length in minutes
    long toMinutes() const throw();
    // get length in hours
    long toHours() const throw();
    // get length in days
    long toDays() const throw();

    // set length in milliseconds
    void asMilliseconds(long milliseconds) const throw();
    // set length in seconds
    void asSeconds(long seconds) const throw(); // unix-like function
    // set length in minutes
    void asMinutes(long minutes) const throw();
    // set length in hours
    void asHours(long hours) const throw();
    // set length in days
    void asDays(long days) const throw();

    /*
     * Redefine operators to simplify use
     */

    AmountOfTime  operator+  (const AmountOfTime& anotherAmount) const throw();
    AmountOfTime& operator+= (const AmountOfTime& anotherAmount) throw();
    AmountOfTime  operator-  (const AmountOfTime& anotherAmount) const throw();
    AmountOfTime& operator-= (const AmountOfTime& anotherAmount) throw();

    bool operator<  (const AmountOfTime& anotherAmount) const throw();
    bool operator<= (const AmountOfTime& anotherAmount) const throw();
    bool operator>  (const AmountOfTime& anotherAmount) const throw();
    bool operator>= (const AmountOfTime& anotherAmount) const throw();    
    bool operator== (const AmountOfTime& anotherAmount) const throw();
    bool operator!= (const AmountOfTime& anotherAmount) const throw();


    /*
     * Other functions
     */

    // NOTE: Reverse order of parameters!

    void set(short milliseconds = 0,
               short seconds = 0,
               short minutes = 0,
               short hours = 0,
               long  days = 0) throw (InvalidTimeStamp);

    void as(long milliSeconds = 0, long seconds = 0) throw();


  private:

	 long  seconds_;
	 short milliseconds_;

 }; // of AmountOfTime


 /*
  *  Output to stream functions
  */

 ostream& operator<< (ostream& oStream, const AmountOfTime& amount) const;




} // of GradSoft namespace


#endif
