// time.cpp : Defines the entry point for the console application.
//


#include <GradSoft/timing.h>
#ifdef HAVE_IOSTREAM
#include <iostream>
#else
#ifdef HAVE_IOSTREAM_H
#include <iostream.h>
#else
#error either iostream or iostream.h must be included
#endif
#endif

using namespace GradSoft;
using namespace std;

int main(int argc, char* argv[])
{   

cout<<"****************NEW*****************"<<endl;
	TimeStamp t1,t2;
	AmountOfTime prec;	
	t1.initialize(2001,1,1,0,0,0,0,3,prec);
	
	t2=t2.now();
       
        if(t2<t1)
        cout<<"t2 less than t1"<<endl;
        else
        cout<<"t2 bigger than t1"<<endl;
	cout<<"OS date :\t\t\t\t\t";
	cout<<t2.getDayOfMonth()<<'/'<<t2.getMonth()<<'/'<<t2.getYear()<<"\n\n";

	cout<<"OS time :\t\t\t\t\t";
	cout<<t2.getHoursOfDay()<<':'<<t2.getMinutesOfHour()<<':'<<t2.getSecondsOfMinute()<<':'
		<<t2.getMillisecOfSeconds()<<"\n\n";

	cout<<"Zone difference in hours from UTC :\t\t";
	cout<<t2.getOffsetGMT()<<"\n\n";

	cout<<"Time in seconds since UTC  1/1/1970 :\t\t";
	cout<<t2.getGreenwich()<<"\n\n";

	prec=t2-t1;
    cout<<"Time in seconds and milliseconds (amount\n of time (*)) since 1/1/2001 0:0:0:0 :\t\t";	    
	cout<<prec.toSeconds()<<':'<<prec.getMilliSecondsOfSeconds()<<"\n\n";    	
        	
	t1+=prec;	
	cout<<"1/1/2001 0:0:0:0 plus amount of time (*) :\t";
	cout<<t1.getDayOfMonth()<<'/'<<t1.getMonth()<<'/'<<t1.getYear()<<' '<<
		t1.getHoursOfDay()<<':'<<t1.getMinutesOfHour()<<':'<<t1.getSecondsOfMinute()<<':'
		<<t1.getMillisecOfSeconds()<<"\n\n";

	cout<<"Local time, which you get from "<<t2.getGreenwich()<<
                           " seconds\nelapsed since UTC 1/1/1970 0:0:0 :\t\t";
	t2.initializeFromGreenwich(t2.getGreenwich(),3);
	cout<<t2.getDayOfMonth()<<'/'<<t2.getMonth()<<'/'<<t2.getYear()<<' '<<
		t2.getHoursOfDay()<<':'<<t2.getMinutesOfHour()<<':'<<t2.getSecondsOfMinute()<<':'
		<<t2.getMillisecOfSeconds()<<'\n';         

    cin.get();

	return 0;
}



