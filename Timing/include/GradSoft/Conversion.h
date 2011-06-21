#ifndef __GradSoft_Conversion_h
#define __GradSoft_Conversion_h

/*
 * Part of GradSoft C++ ToolBox
 * (C) GradSoft, Kiev, Ukraine 2001
 * http://www.gradsoft.com.ua
 * $Id: Conversion.h,v 1.3 2002-08-02 18:52:04 rin Exp $
 */

//#ifdef WIN32
//#include <GradSoft/TimingConfigNT.h>
//#endif





namespace GradSoft {

 // Days in month(index),
 // 1 - January, 2 - February, ... 12 - December.
 static short daysInMonth[] =
    {0, 31,29, 31,30,31, 30,31,31, 30,31,30, 31};
 static short daysSinceXmasInShortYear[] =
    {0, 0,31, 59,90,120, 151,181,212, 243,273,304,334,365};


 // Days, passed from 1 of March to 1 of corresponding month(index),
 // 1 - March, 2-April, ... 12 - February.
 static short daysSinceMarch[] =
    {0,  0,31,61, 92,122,153, 184,214,245, 275,306,337,365}; // NOTE: I call it also Km.
 // Days in month(index) begginning from March,
 // 1 - January, 2 - February, ... 12 - December.
 static short daysInMonthSinceMarch[] =
    {0,  31,30,31, 30,31,31, 30,31,30, 31,31,29}; // NOTE: I call it also Nm.
 // Days in four centuries                                                  
 static long daysInFourCenturies = 365*400 + 97; // =146097. NOTE: I call it also Kq.
 // Days in not lap century
 static long daysInShortCentury = 365*100 + 24; // =36524. NOTE: I call it also Kc.
 // Days in lap four years
 static long daysInLapFourYears = 365*4 + 1; // =1461. NOTE: I call it also K4.
 // Days in not lap year
 static long daysInShortYear = 365; // NOTE: I call it also Ky.

 // Mathematical integer divide: a = (a - floor(a/b));//<-Reminder. Returns quot.
 static long  div(long & a, long b);

 // Converts gregorian date to corresponding julian day
 static void  gregorianDateToJulianDay(long & JD, short D, short M, long Y);
 // Converts julian day to corresponding gregorian date
 static void  julianDayToGregorianDate(long JD, short & D, short & M, long & Y);
 // Checks date
 static bool  badDate(short D, short M, long Y);
 // Checks year if it is lap
 static bool  isLapYear(long Y);


} // of Gradsoft namespace






// Here goes definition of static inline functions:

static long  GradSoft::div(long & a, long b)
{
   long c = a/b;
   a %= b;
   if (a<0) { // Wheither C creators know mathematics? So (-5)/3 should be equal -2 with reminder 1!
      a += b;
      c--;
   };
   return(c);
}

// This converts gregorian date to corresponding julian day
void  GradSoft::gregorianDateToJulianDay(long & JD, short D, short M, long Y)
{
   short m;
   long y;
   if (M<3) {        // Correct month: m-=2, optional
      m=M+10; y=Y-1; // y-=1;m+=12; this to make begiinning  
   } else {          // on 1st of March so the possible lap  
      m=M-2; y=Y;    // day will be at the end of year.      
   }                 // Christmass should be at 1st of March!

   // So the julian day is:
   // jd = Kq*div(y,400) + Kc*div(mod(y,400),100) + 365*mod(y,100) + div(mod(y,100),4) + Km[m] + d + 1721081; //1721081 - For real julian day   
   JD  = 1721081 + D + daysSinceMarch[m] + daysInFourCenturies*div(y,400);
   JD += daysInShortCentury*div(y,100);
   JD += daysInShortYear*y;
   JD += div(y,4);
} // of GradSoft::gregorianDateToJulianDay

// This converts julian day to corresponding gregorian date
void  GradSoft::julianDayToGregorianDate(long JD, short & D, short & M, long & Y)
{
   long jd = JD - 1721082; // Correct julian day to begin count from 1 of March 0 year.
   Y  = div(jd,daysInFourCenturies)*400; // Note that there and further jd is newer negative.
   if ( jd == daysInFourCenturies-1 ) { // Because of that fact 
      Y += 400;                         // that daysInFourCenturies-1=0 (mod daysInCentury).
      M  = 2;
      D  = 29;
      return;
   }
   Y += div(jd,daysInShortCentury) * 100; // There is no last lap year 
   Y += div(jd,daysInLapFourYears) * 4;   // when 100|y and not 400|y. So no check.
   if ( jd == daysInLapFourYears-1 ) { // Because of that fact                             
      Y += 4;                          // that daysInLapFourYears-1=0 (mod daysInShortYear).
      M  = 2;
      D  = 29;
      return;
   }
   Y += div(jd,daysInShortYear);
   M = jd/31 + 1; // Approximate month.
   D = jd - daysSinceMarch[M] + 1;
   if ( daysInMonthSinceMarch[M] < D) { // Check approximation
       D -= daysInMonthSinceMarch[M];   // and possibly correct month.
       M++;
   }
   // Correct date so as to start from 1 of January instead of 1 of March:
   if (M>10) {
      M -= 10; // M+=2-12;
      Y++;
   } else {
      M += 2;
   }
} // of GradSoft::julianDayToGregorianDate

// Checks date
static bool  badDate(short D, short M, long Y)
{
   if (M<1||M>12) { return(false); };
   if (D<1||D>daysInMonth[M]) { return(false); };
   if (M==2&&D==29&& ((Y%4)||((Y%100==0)&&(Y%400))) ) { return(false);};
   return true;
} // of badDate

// Checks year if it is lap
static bool  isLapYear(long Y); // In Gregorian calculus
{
   return (Y%4==0 && Y%100!=0) || (Y%400==0);
} // of isLapYear



#endif
