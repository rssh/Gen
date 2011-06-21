#include <iostream>

#include <Conversion.h>


namespace GradSoft
{


int test1()
{

   long begin_year=-5000,end_year=5000;
   long jd,jd1;
   short months[] = {0,31,28, 31,30,31, 30,31,31, 30,31,30, 31};
   bool bad = false;
   long y,y1;
   short m,m1,d,d1;

   // Part 1 - by-hand test
   //d = 1; m = 1; y = 10;
   //GradSoft::gregorianDateToJulianDay(jd,d,m,y);
   //std::cout << "jd(" << d << "." << m << "." << y << ") = " << jd << std::endl;
   //GradSoft::julianDayToGregorianDate(jd,d,m,y);
   //std::cout << "date(" << jd << ") = " << d << "." << m << "." << y << std::endl;

   //do {
   //   std::cout << "Enter jd d m y: ";
   //   std::cin >> jd;
   //   std::cin >> d; std::cin >> m; std::cin >> y;
   //   GradSoft::gregorianDateToJulianDay(jd1,d,m,y);
   //   std::cout << "jd(" << d << "." << m << "." << y << ") = " << jd1 << std::endl;
   //   GradSoft::julianDayToGregorianDate(jd,d1,m1,y1);
   //   std::cout << "date(" << jd << ") = " << d1 << "." << m1 << "." << y1 << std::endl;
   //} while (jd!=0||d!=0||m!=0||y!=0);


   // Part 2 - Automatic test
   //std::cout << "Enter year to start from: ";
   //std::cin >> begin_year;
   //std::cout << "Enter year to end at: ";
   //std::cin >> end_year;

   GradSoft::gregorianDateToJulianDay(jd,31,12,begin_year-1);

   std::cout << std::endl << "Processing -5000..+5000 test";
   for (y=begin_year; y<end_year&&!bad; y++) {
      if (!(y%400)) {std::cout << ".";}
      //std::cout << "Year " << y;
      if ( y%4==0 && (y%100!=0 || y%400==0)) {
         //std::cout << " is lap" << std::endl;
         months[2] = 29;
      } else {
         //std::cout << " is short" << std::endl;
         months[2] = 28;
      }
 
      for(m=1; m<13&&!bad; m++){
         //std::cout << "Month " << m << " has " << months[m] << " days" << std::endl;
         for(d=1; d<(months[m]+1)&&!bad; d++){
            GradSoft::gregorianDateToJulianDay(jd1,d,m,y);
            GradSoft::julianDayToGregorianDate(jd1,d1,m1,y1);
            //std::cout << "jd(" << d << "." << m << "." << y << ") = " << jd1 << std::endl;
            if( (jd1-jd)!=1 || d!=d1 || m!=m1 || y!=y1 ) {
               std::cout << "[" << d << "." << m << "." << y << "] is BAD!!!" << std::endl;
               std::cout << "jd = " << jd << std::endl;
               std::cout << "jd1 = " << jd1 << std::endl;
               std::cout << "d = " << d << std::endl;
               std::cout << "m = " << m << std::endl;
               std::cout << "y = " << y << std::endl;
               std::cout << "d1 = " << d1 << std::endl;
               std::cout << "m1 = " << m1 << std::endl;
               std::cout << "y1 = " << y1 << std::endl;
               bad = true;
               break;
            };
            jd=jd1;
         }
      }
   }

   if (bad) {
      std::cout << "TEST FAILED" << std::endl;
      return 1;
   } else {
      std::cout << "Test passed" << std::endl;
      return 0;
   }
}


}

int main(int argc, char** argv)
{
   long i=0;
   std::cout << std::endl << "Passing testes:" << std::endl;

   if (GradSoft::test1()) {i++;}

   if (i!=0) {
      std::cout << std::endl << "TESTS FAILED: There is "
                << i << " failed tests." << std::endl;
   } else {
      std::cout << std::endl << "TESTS PASSED: All ok." << std::endl;
   }
   return -1;
}
