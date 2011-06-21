#ifndef __GradSoft_Time_h
#define __GradSoft_Time_h

/**
 * class, which represent amount of time
 **/
class AmountOfTime
{
public:

    long getSecondsOfMinute() ;

    long getMinutesOfHour() ;

    long getHoursOfDay() ;

    long getDays() ;

    long getWeeks() ;

    void setSecondsOfMinute(long newSeconds) ;

    void setMinutesOfHour(long newMinutes) ;

    void setHoursOfDay(long newHours) ;

    void setDays(long newDays) ;

    void setWeeks(long newWeeks) ;

    long long toSeconds(); 

    void setNSeconds(long long nSeconds);

    long toMinutes() ;

    long toHours() ;

    long toDays() ;

    long toWeeks() ;

    AmountOfTime operator+(cosnt AmountOfTime& anotherAmount);

    AmountOfTime& operator+=(const AmountOfTime& anotherAmount);

    AmountOfTime operator-(const AmountOfTime& anotherAmount);

    AmountOfTime& operator-=(const AmountOfTime& anotherAmount);

    boolean operator<(const AmountOfTime& anotherAmount);
    boolean operator<=(const AmountOfTime& anotherAmount);
    boolean operator>(const AmountOfTime& anotherAmount);
    boolean operator>=(const AmountOfTime& anotherAmount);
    boolean operator<(const AmountOfTime& anotherAmount);
    boolean operator==(const AmountOfTime& anotherAmount);
    boolean operator!=(const AmountOfTime& anotherAmount);

    void initialize(long weeks,
                    long days,
                    long hours,
                    long minutes,
                    long seconds);

    void initialize(long nSeconds);

};


/**
 * class, which represents Time
 **/
class Timestamp
{
public:

    Time();

    Time(const Time&);

    /*
     * return hour of day
     */
    long getHoursOfDay();

    long getMinutesOfHour() ;

    long getSecondsOfMinute() ;

    long getDayOfMonth() ;

    long getDayOfYear() ;

    short getMonth();

    long getOffsetGMT() ;

    long getGreenwich(); // number of seconds from 0:0:0 1970 GMT

    AmountOfTime getPrecision();

    void setHours(long hours) ;

    void setMinutes(long minutes) ;

    void setSeconds(long seconds) ;

    void setDayOfMonth(long dayOfMonth) ;

    void setDayOfYear(long dayOfYear) ;

    void setMonth(long month) ;

    void setYear(long year) ;

    long getGreenwich(void);

    Time& operator=(const Time& anotherTime);

    Time& operator+=(const AmountOfTime& amount);
    Time  operator+(const AmountOfTime& amount);
    Time& operator-=(const AmountOfTime& amount);

    AmountOfTime operator-(const Time& anotherTime) const ;

    bool   operator <= (const Time& anotherTime) const;
    bool   operator < (const Time& anotherTime)  const;
    bool   operator >= (const Time& anotherTime) const;
    bool   operator > (const Time& anotherTime)  const;
    bool   operator != (const Time& anotherTime) const;
    bool   operator == (const Time& anotherTime) const;

    void initialize(long year,
                    long month,
                    long day,
                    long hour,
                    long minute,
                    long second,
                    long offsetGMT,
                    AmountOfTime precision);

    void initializeFromGreenwich(long nSeconds);

    static  Time  now();

};


#endif
