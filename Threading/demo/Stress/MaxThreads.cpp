#include <GradSoft/Threading.h>

#include <iostream>
# include <vector>

using namespace std;
using namespace GradSoft;

class LightThread: public Thread
{
  int n_;

public:

  LightThread(int n):n_(n){}

  void run()
  {
    cerr<<n_<<endl;
    for(;;) sleep(0);
  }
};

class HeavyThread: public Thread
{
public:

  void run()
  {
    unsigned long x;
    for(;;) {
       ++x;
    }
  }

};

int main(int argc, char** argv)
{
  vector<Thread*> threads;
  int threadCount = 0;
  for(;;){
    LightThread* newThread_p;
    try {
      newThread_p = new LightThread(threadCount);
      ++threadCount;
    }catch(...){
      cerr<<"can not create a Thread heir instance"<<endl;
      break;
    }
    try{
      threads.push_back(newThread_p);
    }catch(...){
      cerr<<"can not store a pointer to Thread heir instance"<<endl;
	  break;
    }
    try {
      newThread_p->start();
      while(!newThread_p->is_running()) Thread::yield();
    }catch(const ThreadingExceptions::SystemError& ex){
      cerr<<"can not start a thread : system error"<<endl;
      cerr<<ex.getErrorMessage()<<endl;
      break;
    }catch(const ThreadingExceptions::Failure& ex){
      cerr<<"can not start a thread"<<endl;
      cerr<<ex.getErrorMessage()<<endl;
      break;
    }
  }

  cerr << endl;
  cerr << "threadCount = " << threadCount << endl;
  cerr << endl;

  while (threadCount>0) {
    try{
      delete threads[--threadCount];
    }catch(const ThreadingExceptions::Failure& ex){
      cerr<<"Failure:"<< ex.what() << endl;
    }catch(...){
      cerr<<"unrecognized exception"<<endl;
    }
  } 

  threads.erase(threads.begin(),threads.end());

  return 0;
}

