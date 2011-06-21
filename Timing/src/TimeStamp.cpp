#include <GradSoft/TimeStamp.h>


 /*
  *
  *  Part of GradCToolbox.
  *  Implementation of the class TimeStamp.
  *  $Id: TimeStamp.cpp,v 1.4 2002-08-02 18:52:05 rin Exp $
  *
  */


namespace GradSoft {

 /*
  *  Constructors
  */

 TimeStamp::TimeStamp()
 {
    year_ = 1970;
    month_ = 1;
    day_ = 1;
    hour_ = 0;
    minute_ = 0;
    second_ = 0;

    milseconds_ = 0;

    daySeconds_ = 0;
    julianDay_ = 2440550;
    // zone_ should be created by default constructor
 }

 TimeStamp::TimeStamp(const TimeStamp& another)
 {
    year_ = another.year_;
    month_ = another.month_;
    day_ = another.day_;
    hour_ = another.hour_;
    minute_ = another.minute_;
    second_ = another.second_;

    milseconds_ = another.milseconds_;

    daySeconds_ = another.daySeconds_;
    julianDay_ = another.julianDay_;

    zone_ = another.zone_;
 }

 TimeStamp::TimeStamp(
              const TimeZone& zone,
              long year,
              short month,
              short day,
              short hour = 0,
              short minute - 0,
              short second = 0,
              short millisecond = 0
             ) throw (InvalidDate,InvalidTime,InvalidTimeStamp)
 {
    if badDate(day,month,year) throw InvalidDate();
    if (hours<0||hours>23||minutes<0||minutes>59||
       seconds<0||seconds>59||milliseconds<0||milliseconds>999)
          throw InvalidTime();
    year_ = year;
    month_ = month;
    day_ = day;
    hour_ = hour;
    minute_ = minute;
    second_ = second;

    milseconds_ = milseconds;

    daySeconds_ = hour*3600 + minute*60 +second;
    gregorianDateToJulianDay(julianDay_,day,month,year);

    zone_ = zone;
 }

 TimeStamp::TimeStamp(
              const TimeZone& zone,
              long julianDay,
              long seconds // Seconds since beginning of that day
             ) throw (InvalidDate,InvalidTime,InvalidTimeStamp)
 {
    julianDay_ = julianDay + div(seconds,86400);
    julianDayToGregorianDate(julianDay_,day_,month_,year_);
    daySeconds_ = seconds;
    hour_ = div(seconds,3600);
    minute_ = div(seconds,60);
    second_ = seconds;
    milliseconds_ = 0;
    zone_ = zone;
 }

 TimeStamp::TimeStamp(
              const TimeZone& zone,
              long seconds // Seconds since beginning of the UNIX epoch
             ) throw (InvalidDate,InvalidTime,InvalidTimeStamp)
 {
    julianDay_ = 2440550 + div(seconds,86400);
    julianDayToGregorianDate(julianDay_,day_,month_,year_);
    daySeconds_ = seconds;
    hour_ = seconds/3600; // Note: seconds are now positive so we could use here operator "/".
    seconds = seconds%3600;
    minute_ = seconds/60;;
    second_ = seconds%60;
    milliseconds_ = 0;
    zone_ = zone;
 }


 /*
  *  Property access functions
  */

 // Read type functions

 short TimeStamp::getHours() const
 {
    return hour_;
 }
 
 short TimeStamp::getMinutes() const
 {
    return minute_;
 }
 
 short TimeStamp::getSeconds() const
 {
    return second_;
 }

 short TimeStamp::getMilliseconds() const
 {
    return milliseconds_;
 }

 short TimeStamp::getDay() const
 {
    return day_;
 }
 
 short TimeStamp::getDayOfYear() const
 {
    if (month>2 && isLapYear(Y)) {
       return( daysSinceXmasInShortYear[month_] + day_ + 1 );
    } else {
       return( daysSinceXmasInShortYear[month_] + day_ );
    }
 }

 short TimeStamp::getJulianDay() const
 {
    return day_;
 }
 
 short TimeStamp::getMonth() const
 {
    return month_;
 }
 
 long TimeStamp::getYear() const
 {
    return year_;
 }

 TimeZone& TimeStamp::getTimeZone() const
 {
    return  new TimeZone(zone_);
 }


 // Modify methods

 void TimeStamp::setHours(short hours)
 {
    if (hours<0||hours>23)  throw InvalidTime();   
    hour_ = hours;
    daySeconds_ = hours_*3600 + minutes_*60 + seconds_;
 }
 
 void TimeStamp::setMinutes(short minutes)
 {
    if (minutes<0||minutes>59)  throw InvalidTime();   
    minutes_ = minutes;
    daySeconds_ = hours_*3600 + minutes_*60 + seconds_;
 }
 
 void TimeStamp::setSeconds(short seconds)
 {
    if (seconds<0||seconds>59)  throw InvalidTime();   
    seconds_ = seconds;
    daySeconds_ = hours_*3600 + minutes_*60 + seconds_;
 }

 void TimeStamp::setMillisec(short milliseconds)
 {
    if (milliseconds<0||milliseconds>999)  throw InvalidTime();
    milliseconds_ = milliseconds;
 }
 
 void TimeStamp::setDay(short day)
 {
    if (badDate(day,month_,year_)) throw InvalidDate();
    day_ = day;
 }
 
 void TimeStamp::setDayOfYear(short day)
 {
    if (day<1||day>366) {
       if (day!=366 || !isLapYear(year_))  throw InvalidDate();
    }
    month_ = (day-1)/31 + 1; // Approximate month. Note: day is positive or zero.
    day -= daysSinceXmathInShortYear[month_];
    if (daysInMonth[month_] < day) { // Check approximation
       day -= daysInMonth[month_];   // and possibly correct month.
       month++;
    }
    if (month>2isLapYear(year_)) day--;
    gregorianDateToJulianDay(julianDay_,day_,month_,year_);
 }

 void TimeStamp::setJulianDay(long jd)
 {
    julianDay_ = jd;
    julianDayToGregorianDate(jd,day_,month_,year_);
 }
 
 void TimeStamp::setMonth(short month)
 {
    if (badDate(day_,month,year_)) throw InvalidDate();
    month_ = month;
 }

 void TimeStamp::setYear(long year)
 {
    year_ = year;
 }
 
 void TimeStamp::setTimeZone(const TimeZone& zone)
 {
    zone_ = zone;
 }

 /*
  *  UNIX-like time functions
  */

 long TimeStamp::toSeconds() const
 {
    return ( (julianDay_-2440550)*86400 + daySeconds_ );
 }

 void TimeStamp::asSeconds(long seconds) const
 {
    julianDay_ = 2440550 + div(seconds,86400);
    JulianDayToGregorianDate(julianDay_,day_,month_,year_);
    daySeconds_ = seconds;
    hour_ = seconds/3600; // Note: seconds are positive there so we could use here operator "/".
    seconds = seconds%3600;
    minute_ = seconds/60;;
    second_ = seconds%60;
    milliseconds_ = 0;
 }

 /*
  *  Here goes operator redefinitions
  */
 
 TimeStamp TimeStamp::operator+ (const AmountOfTime& amount) const
 {
    long amountSeconds = amount.asSeconds() + daySeconds_;
    short milliseconds = milliseconds_ + amount.getMilliseconds();
    while (milliseconds>999) { amountSeconds++; milliseconda-=1000; }
    while (milliseconds<0) { amountSeconds--; milliseconda+=1000; }
    long amountDays = div(amountSeconds,86400);
    return TimeStamp(zone_, julianDay_ + amountDays, amountSeconds);
 }
 













 TimeStamp& TimeStamp::operator +=(const AmountOfTime& amount) 
 {
    long amountSeconds = amount.asSeconds() + daySeconds_;
    short milliseconds = milliseconds_ + amount.getMilliseconds();
    while (milliseconds>999) { amountSeconds++; milliseconds-=1000; }
    while (milliseconds<0) { amountSeconds--; milliseconds+=1000; }
    long amountDays = div(amountSeconds,86400);
    return new TimeStamp(zone_, julianDay_ + amountDays, amountSeconds);
 }  // TODO: handle here milliseconds!!!!!!!!!
 
 AmountOfTime TimeStamp::operator -(const TimeStamp& another) const
 {
    long days = julianDays_ - another.julianDays_;
    long seconds = daySeconds_ - another.daySeconds_;
    short milliseconds = milliseconds_ - amount.getMilliseconds();
    while (milliseconds>999) { seconds++; milliseconds-=1000; }
    while (milliseconds<0) { seconds--; milliseconds+=1000; }
    while (seconds>86400) { days++; seconds-=86400; }
    while (seconds<0) { days--; seconds+=86400; }
    return AmountOfTime( days, seconds);
 }  // TODO: handle here milliseconds !!!!!!!!!!!
 
 TimeStamp TimeStamp::operator -(const AmountOfTime& amount) const
 {
 }

 TimeStamp& TimeStamp::operator -=(const AmountOfTime& amount)
 {
   TimeStamp temp=*this;
   *this=temp-amount;
   return *this;
 }

 bool TimeStamp::operator <=(const TimeStamp& anotherTime) const
 {
    AmountOfTime amount1,amount2;
    amount1.initialize(getGreenwich(),milsec_);
    amount1.setPrecision(precision_.toMilliSeconds());
    amount2.initialize(anotherTime.getGreenwich(),anotherTime.milsec_);
   amount2.setPrecision(precision_.toMilliSeconds());
   return (amount1<=amount2);
 }

 bool TimeStamp::operator <(const TimeStamp& anotherTime) const
 {
    AmountOfTime amount1,amount2;
   amount1.initialize(getGreenwich(),milsec_);
    amount1.setPrecision(precision_.toMilliSeconds());
    amount2.initialize(anotherTime.getGreenwich(),anotherTime.milsec_);
    amount2.setPrecision(precision_.toMilliSeconds());
    return (amount1<amount2);
 }
 
 bool TimeStamp::operator >=(const TimeStamp& anotherTime) const
 {
   AmountOfTime amount1,amount2;
   amount1.initialize(getGreenwich(),milsec_);
   amount1.setPrecision(precision_.toMilliSeconds());
   amount2.initialize(anotherTime.getGreenwich(),anotherTime.milsec_);
   amount2.setPrecision(precision_.toMilliSeconds());
   return (amount1>=amount2);   
 }
 
 bool TimeStamp::operator >(const TimeStamp& anotherTime) const
 {
    AmountOfTime amount1,amount2;
    amount1.initialize(getGreenwich(),milsec_);
    amount1.setPrecision(precision_.toMilliSeconds());
    amount2.initialize(anotherTime.getGreenwich(),anotherTime.milsec_);
    amount2.setPrecision(precision_.toMilliSeconds());
   return (amount1>amount2);   
 }

 bool TimeStamp::operator !=(const TimeStamp& anotherTime) const
 {
   AmountOfTime amount1,amount2;
    amount1.initialize(getGreenwich(),milsec_);
   amount1.setPrecision(precision_.toMilliSeconds());
   amount2.initialize(anotherTime.getGreenwich(),anotherTime.milsec_);
    amount2.setPrecision(precision_.toMilliSeconds());
    return (amount1!=amount2);   
 }
 
 bool TimeStamp::operator ==(const TimeStamp& anotherTime) const
 {
   AmountOfTime amount1,amount2;
   amount1.initialize(getGreenwich(),milsec_);
   amount1.setPrecision(precision_.toMilliSeconds());
   amount2.initialize(anotherTime.getGreenwich(),anotherTime.milsec_);
   amount2.setPrecision(precision_.toMilliSeconds());
    return (amount1==amount2);   
 }
 
 void TimeStamp::initialize(unsigned short year, unsigned short month, unsigned short day, unsigned short hour, unsigned short minute, unsigned short second, unsigned short milsec, unsigned short offsetGMT, AmountOfTime precision) 
 {
     if(year<1970||year>2038||month<1||month>12||hour>23||minute>59||second>59||milsec>999||offsetGMT>24)
 		throw InvalidDateParameter();
 	year_=year;
 	month_=month;
 	setDayOfMonth(day);
 	hour_=hour;
 	minute_=minute;
 	second_=second;
 	milsec_=milsec;
 	offsetGMT_=offsetGMT;
 	precision_=precision;	
 }

 void TimeStamp::initializeFromGreenwich(unsigned long nSeconds,unsigned short offsetGMT)
 {
	if(offsetGMT>24)
		throw InvalidDateParameter();
 
     precision_.setNSeconds(0);
	 precision_.setMillisecondsOfSecond(0);
 	 year_=(short)(nSeconds/31536000)+1970;
 	 nSeconds-=(year_-1970)*31536000+(year_-1969)/4*86400;
 	 setDayOfYear((short)(nSeconds/86400)+1);
 	 offsetGMT_=offsetGMT;
 	 hour_=nSeconds%86400/3600;
     hour_+=offsetGMT_;	 
	 minute_=nSeconds%3600/60;
	 second_=nSeconds%60;
 }

 TimeStamp TimeStamp::now()
 {	
    TimeStamp temp;	
     time_t t,*t_ptr=&t;	
     t=time(t_ptr); 
     if(t==-1)     
        throw SystemTimeExc();
 	
     tm *local=localtime(t_ptr);        
    
	temp.year_=local->tm_year+1900;
	temp.month_=local->tm_mon+1;
	temp.day_=local->tm_mday;
	temp.hour_=local->tm_hour;
    temp.minute_=local->tm_min;
    temp.second_=local->tm_sec;
	tm *greenw=gmtime(t_ptr);
	if(greenw==NULL)
		throw SystemTimeExc();
    temp.offsetGMT_=temp.hour_-greenw->tm_hour;
    _timeb  t2,*timebuf=&t2;   
    _ftime(timebuf);
	temp.milsec_=timebuf->millitm;
    temp.precision_.setMillisecondsOfSecond(0);
    temp.precision_.setNSeconds(0)