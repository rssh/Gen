#include <GradSoft/AmountOfTime.h>


 /*
  *
  *  Part of GradCToolbox.
  *  Implementation of the class AmountOfTime.
  *  $Id: AmountOfTime.cpp,v 1.3 2002-08-01 18:13:07 rin Exp $
  *
  */


namespace GradSoft {

 /*
  *  Constructors
  */

 AmountOfTime::AmountOfTime()
 {
    seconds_=0;
    milliseconds_=0;  
 }

 void AmountOfTime::AmountOfTime(
    short milliseconds,
    short seconds = 0,
    short minutes = 0,
    short hours = 0,
    short days = 0)
 {
    if(hours<-23||hours>23||
       minutes<-59||minutes>59||
       seconds<-59||seconds>59||
       milliseconds<-999||milliseconds>999)
          throw InvalidTime();
    if (milliseconds<0) { // This to ensure that milliseconds are positive
       milliseconds_ = 1000 + milliseconds;
       seconds_ = seconds - 1;
    } else {
       milliseconds_ = milliseconds;
       seconds_ = seconds;
    };
    seconds_ += minutes*60 + hours*3600 + days*86400 + weeks*604800
 }

 /*
  *  nearest-high boundary dependent access functions
  */ 

 // read functions

 short AmountOfTime::getMilliseconds() const
 {
    return milliseconds_;
 }
 
 short AmountOfTime::getSeconds() const
 {
    return (short)(seconds_%60);
 }
 
 short AmountOfTime::getMinutes() const
 {
    return (short)(seconds_%3600/60);
 }
 
 short AmountOfTime::getHours() const
 {
    return (short)(seconds_%86400/3600);
 }
 
 long AmountOfTime::getDays() const 
 {
    return (short)(seconds_/86400);
 }

 // modify functions

 void AmountOfTime::setMilliseconds(short milliseconds)
 {
    if(milliseconds<-999||milliseconds>999)
       throw InvalidTime();
    if (milliseconds<0) { // This to ensure that milliseconds are positive
       milliseconds_ = 1000 + milliseconds;
       seconds_ = seconds_ - 1;
    } else {
       milliseconds_ = milliseconds;
    };
 }
 
 void AmountOfTime::setSeconds(short seconds)
 {
    if(seconds<-59||seconds>59)
       throw InvalidTime();
    seconds_ -= seconds_%60;
    seconds_ += seconds;
 }
 
 void AmountOfTime::setMinutes(short minutes)
 {
    if(minutes<-59||minutes>59)
       throw InvalidTime();
    seconds_ -= seconds_%3600 - seconds_%60;
    seconds_ += minutes*60;
 }
 
 void AmountOfTime::setHours(short hours)
 {
    if(hours<-23||mours>23)
       throw InvalidTime();
    seconds_ -= seconds_%86400 - seconds_%3600;
    seconds_ += hours*3600;
 }
 
 void AmountOfTime::setDays(long days)
 { 
    seconds_ = seconds_%86400 + days*86400;
 }
 
 /*
  *  Absolute access functions
  */ 

 // read functions
 
 long AmountOfTime::toMilliseconds() const
 {
    //if(seconds_<-2000000||seconds_>2000000)
    // {
    //    throw MillisecondsExc();
    // }  TODO: Find out how to process such situations? 
    return (seconds_*1000 + milliseconds_);
 }

 long AmountOfTime::toSeconds() const
 {
    return seconds_;
 }
 
 long AmountOfTime::toMinutes() const
 {
    return seconds/60;
 }
 
 long AmountOfTime::toHours() const
 {
    return minutes/3600;
 }
 
 long AmountOfTime::toDays() const
 {
    return minutes/86400;
 }
 
 // modify functions

 long AmountOfTime::asMilliseconds(long milliseconds) const
 {
    milliseconds_ = milliseconds % 1000;
    seconds_ = milliseconds/1000;
    if (milliseconds<0) { // This to ensure that milliseconds are positive
       if (milliseconds_) { // It's all non-mathematical C behavor of operator "/"
          seconds_ = seconds_ - 1; // NOTE: that "-5%3" evaluates to  "-2"
          milliseconds += 1000; // NOTE: also "-5/3" evaluates to  "-1"
       }
    }
 }

 long AmountOfTime::asSeconds(long seconds) const
 {
    seconds_ = seconds;
    milliseconds_ = 0;
 }
 
 long AmountOfTime::asMinutes(long minutes) const
 {
    seconds_ = minutes*60;
    milliseconds_ = 0;
 }
 
 long AmountOfTime::asHours(long hours) const
 {
    seconds_ = hours*3600;
    milliseconds_ = 0;
 }
 
 long AmountOfTime::asDays(long days) const
 {
    seconds_ = minutes*86400;
    milliseconds_ = 0;
 }
 
 /*
  *  Redefined operators
  */
 
 AmountOfTime AmountOfTime::operator+ (const AmountOfTime& anotherAmount) const
 {
    AmountOfTime temp;
    temp.seconds_ = seconds_ + anotherAmount.seconds_;
    temp.milliseconds_ = milliseconds_ + anotherAmount.milliseconds_;
    if (temp.milliseconds_>999) {
       temp.milliseconds_ = temp.milliseconds_ - 1000;
       temp.seconds_++;
    }
    return temp;
 }
 
 AmountOfTime& AmountOfTime::operator+= (const AmountOfTime& anotherAmount)
 {
    seconds_ += anotherAmount.seconds_;
    milliseconds_ += anotherAmount.milliseconds_;
    if (milliseconds_>999) {
       milliseconds_ = milliseconds_ - 1000;
       seconds_++;
    }
    return *this;
 }
 
 AmountOfTime AmountOfTime::operator- (const AmountOfTime& anotherAmount) const
 {
    AmountOfTime temp;
    temp.seconds_ = seconds_ - anotherAmount.seconds_;
    temp.milliseconds_ = milliseconds_ - anotherAmount.milliseconds_;
    if (temp.milliseconds_<0) {
       temp.milliseconds_ = temp.milliseconds_ + 1000;
       temp.seconds_--;
    }
    return temp;
 }
 
 AmountOfTime& AmountOfTime::operator-= (const AmountOfTime& anotherAmount)
 {
    seconds_ -= anotherAmount.seconds_;
    milliseconds_ -= anotherAmount.milliseconds_;
    if (milliseconds_<0) {
       milliseconds_ = milliseconds_ + 1000;
       seconds_ --;
    }
    return *this;
 }
 
 bool AmountOfTime::operator< (const AmountOfTime& anotherAmount) const
 {
    if (seconds_<anotherAmount.seconds_) {
       return true;
    } else if (seconds_==anotherAmount.seconds_) {
       return (milliseconds_<anotherAmount.milliseconds_);
    } else {
       return false;
    }
 }
 
 bool AmountOfTime::operator<=(const AmountOfTime& anotherAmount) const
 {
    if (seconds_<anotherAmount.seconds_) {
       return true;
    } else if (seconds_==anotherAmount.seconds_) {
       return (milliseconds_<=anotherAmount.milliseconds_);
    } else {
       return false;
    }
 }
 
 bool AmountOfTime::operator>(const AmountOfTime& anotherAmount) const
 {
    if (seconds_>anotherAmount.seconds_) {
       return true;
    } else if (seconds_==anotherAmount.seconds_) {
       return (milliseconds_>anotherAmount.milliseconds_);
    } else {
       return false;
    }
 }
 
 bool AmountOfTime::operator>=(const AmountOfTime& anotherAmount) const
 {
    if (seconds_>anotherAmount.seconds_) {
       return true;
    } else if (seconds_==anotherAmount.seconds_) {
       return (milliseconds_>=anotherAmount.milliseconds_);
    } else {
       return false;
    }
 }
 
 bool AmountOfTime::operator==(const AmountOfTime& anotherAmount) const
 {
    return ((seconds_==anotherAmount.seconds_)&&
            (milliseconds_==anotherAmount.milliseconds_));
 }
 
 bool AmountOfTime::operator!=(const AmountOfTime& anotherAmount) const
 {
    return ((seconds_!=anotherAmount.seconds_)||
            (milliseconds_!=anotherAmount.milliseconds_));
 }

 /*
  *  Other functions
  */
 
 // NOTE: Reverse order of parameters!

 void AmountOfTime::set(short milliseconds = 0,
                          short seconds = 0,
                          short minutes = 0,
                          short hours = 0, 
                          long  days = 0)
 {
    if(hours<-23||hours>23||
       minutes<-59||minutes>59||
       seconds<-59||seconds>59||
       milliseconds<-999||milliseconds>999)
          throw InvalidTime();
    if (milliseconds<0) { // This to ensure that milliseconds are positive
       milliseconds_ = 1000 + milliseconds;
       seconds_ = seconds - 1;
    } else {
       milliseconds_ = milliseconds;
       seconds_ = seconds;
    };
    seconds_ += minutes*60 + hours*3600 + days*86400 + weeks*604800
 }
 
 void AmountOfTime::as(long milliseconds = 0, long seconds = 0)
 {
    milliseconds_ = (short)(milliseconds%1000);
    seconds_ = milliseconds/1000 + seconds;
    if(milliseconds_<0)
    {
       milliseconds_ += 1000;
       seconds_--;
    }
 }

 /*
  *  Output to stream functions
  */

 ostream& operator<< (ostream& oStream, const &AmountOfTime amount) const
 {
    oStream << (seconds_ + milliseconds_/1000.0);
 }
  
 

};






