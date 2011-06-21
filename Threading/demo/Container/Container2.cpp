#include <GradSoft/Threading.h>


#include <iostream>
#include <set>

using namespace GradSoft;
using namespace std;

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif



typedef threadsafe_uac<set<int, less<int> > > StorageType;

StorageType storage;


class Putter:public Thread
{
public:

 void run(void)
 {
  for(;;) {
#ifdef WIN32
     storage.insert(rand()%10);
#else
     storage.insert(random()%10);
#endif
    sleep(1);
  }
 }

};


class Eraser: public Thread
{
 void run(void)
 {
  for(;;) {
#ifdef WIN32
    storage.erase(rand()%10);
#else
    storage.erase(random()%10);
#endif
    sleep(1);
  }
 }
};

class Printer: public Thread
{
 void run(void)
 {
  for(;;) {
    cout << '(' ;
    storage.read_lock();
    for(StorageType::iterator it = storage.begin_(); it!=storage.end_();++it){
      cout << (*it) << " ";
    }
    storage.read_unlock();
    cout << ')' << endl;
    sleep(1);
  }
 }
};

int main(int argc, char** argv)
{
 Putter putter;
 Eraser eraser;
 Printer printer;

 putter.start();
 eraser.start();
 printer.start();

 Thread::sleepCurrent(10);

 return 0;
}



