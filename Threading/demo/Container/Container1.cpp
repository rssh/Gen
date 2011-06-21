#include <GradSoft/Threading.h>


#include <vector>
#include <iostream>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

using namespace std;
using namespace GradSoft;



class Counter: public RWLocked<int>
{
public:

  Counter(int v)
    :RWLocked<int>(v)
  {
  }

  Counter& operator++()
  {
   WriteLocker wl(rwlock_); 
   ++get_value_(); 
   return *this;
  }

  Counter& operator--()
  {
   WriteLocker wl(rwlock_); 
   --get_value_(); 
   return *this;
  }

  bool zero() { return v_ == 0; }
  bool zero_less() { return v_ < 0; }

  void print(ostream& os)
  {
   ReadLocker rl(rwlock_);
   os << get_value_();
  }

};

typedef threadsafe_biseq<vector<Counter> > StorageType;

StorageType storage;


class Putter:public Thread
{
public:

 void run(void)
 {
  for(;;) {
    storage.push_back(4);
    sleep(2);
  }
 }

};

class Mutator: public Thread
{
public:

 void run(void)
 {
  for(;;) {
    storage.read_lock();
    for(StorageType::iterator it = storage.begin_(); it!=storage.end_(); ++it){
#ifdef WIN32
      if ((rand() % 2)==1) {
#else
      if ((random() % 2)==1) {
#endif
        --(*it);
      }else{
        ++(*it);
      }
    }
    storage.read_unlock();
    sleep(1);
  } 
 }

};

class Eraser: public Thread
{
 void run(void)
 {
  for(;;) {
    storage.read_lock();
    for(StorageType::iterator it = storage.begin_(); it!=storage.end_();){
      if ((*it).zero()) {
         storage.read_unlock();
         it = storage.erase(it);
         storage.read_lock();
      }else{
         ++it;
      }
    }
    storage.read_unlock();
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
      (*it).print(cout);
      cout << " ";
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
 Mutator mutator;
 Eraser eraser;
 Printer printer;

 putter.start();
 mutator.start();
 eraser.start();
 printer.start();

 Thread::sleepCurrent(100);

 return 0;
}



