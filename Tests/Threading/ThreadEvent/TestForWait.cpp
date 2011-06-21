#include <GradSoft/Threading.h>

// Tasks to be resolved:
//
// 1. Using wait/notify
// 2. Using wait/notify-all
// 3. Using double/wait notificationling.

#include <iostream>
  
using namespace std;
using namespace GradSoft;

ThreadEvent do_exit;
Mutex  coutMutex;

class TestForWait : public Thread
{
private:
  int n_;
public:
  void set_number(int n)
  {
    n_=n;
  }
  void run() 
  { 
    do_exit.wait();
    { MutexLocker ml(coutMutex);
      cout << "instance " << n_ << " finish work" << endl ;
    }
  }
};

#define SLEEP_TIME_1 1
#define SLEEP_TIME_2 2

class TestForMessaging: public Thread
{
  ThreadEvent do_stop_;
  ThreadEvent run_is_used_;
public: 
  void stop()
  {
    do_stop_.notify();
    run_is_used_.wait();
    { MutexLocker ml(coutMutex);
      cout << "stop say: notification received" << endl ;
    }
  }
  void run()
  {
    { MutexLocker ml(coutMutex);
     cout <<"TestForMessaging::run"<<endl;
    }
    do_stop_.wait();
    { MutexLocker ml(coutMutex);
     cout<<"run say: notification received"<<endl;
    }
    sleep(1);
    run_is_used_.notify();
  }
}; 

#define WAITERS_NUM 3

int main(int argc, char** argv )
{
  int i;
  bool step1,step2,step3;
  step1=step2=step3=true;
  bool interactive=false;
  if (argc>1) {
    if (!strcmp(argv[1],"step1")) {
      step1=true;
      interactive=true;
    }else if (!strcmp(argv[1],"step2")) {
      step1=false;
      step2=true;
      interactive=true;
    }else if (!strcmp(argv[1],"step3")) {
      step1=false;
      step2=false;
      step3=true;
      interactive=true;
    }else{
      cerr << "Unknown option" << endl;
      return 1;
    }
  } 
  
  TestForWait waiters[WAITERS_NUM];
  for(int i=0; i<WAITERS_NUM; ++i) {
    waiters[i].set_number(i);
  }

  { MutexLocker ml(coutMutex);
   cout << "OK" << endl;
  }
  char buff[255];
  cin.getline(buff,255);
  if (step1) {
     for(i=0; i<WAITERS_NUM; ++i) {
	waiters[i].start();
     }
     {MutexLocker ml(coutMutex);
      cout<<"waiters started"<<endl;
     }
     Thread::sleep(1);
     for(i=0; i<WAITERS_NUM; ++i) {
       if (interactive) 
         cin.getline(buff,255);
       else{
         Thread::sleep(1);
       }
       do_exit.notify();
     }
     if (interactive) {
      bool running=true;
      while(running) {
A:
       for(int i=0; i<WAITERS_NUM; ++i) {
         if (waiters[i].is_running()) {
           running=true;
           Thread::yield();
           goto A;
         }
       }
       running=false;
      }

      {MutexLocker ml(coutMutex); 
       cout << "Exiting" << endl;
      }
       return 0;
     }
  }

  if (step2) {
    {MutexLocker ml(coutMutex);
     cout<<"step 2"<<endl;
    }
    for(i=0; i<WAITERS_NUM; ++i) {
	waiters[i].start();
    }
    {MutexLocker ml(coutMutex);
     cout<<"waiters started"<<endl;
    }
    if (interactive) {
      cin.getline(buff,255);
    }
    Thread::sleep(1);
    do_exit.notifyAll();
    if (interactive) {
      cin.getline(buff,255);
    }else{
      Thread::sleep(1);
    }
    if (interactive) return 0;
  }

  if (step3) {
    {MutexLocker ml(coutMutex);
     cout<<"step 3"<<endl;
    }
    TestForMessaging conversation;
    conversation.start();
    if (interactive) {
       cin.getline(buff,255);
    }
    Thread::sleep(1);
    conversation.stop();
    if (interactive) {
       cin.getline(buff,255);
    }
    Thread::sleep(1);
  }

  {MutexLocker ml(coutMutex);
   cout<<"main() has finised work:END"<<endl;
  }
  return 0;
}

