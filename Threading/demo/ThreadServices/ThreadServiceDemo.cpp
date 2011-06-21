#ifndef __GradSoft_Threading_h
#include <GradSoft/Threading.h>
#endif

#include <GradSoft/ThreadServices/SingleThreadBlocking.h>
#include <GradSoft/ThreadServices/SingleThreadChecking.h>
#include <GradSoft/ThreadServices/SingleThreadReactive.h>
#include <GradSoft/ThreadServices/ThreadPerClient.h>
#include <GradSoft/ThreadServices/ThreadPool.h>

#include <iostream>
#include <cstring>

#ifdef WIN32
#include <stdlib.h>
#endif
 


using namespace GradSoft;
using namespace std;



class EventGenerator;

class Event: public Runnable
{
private:

  int index_;
  long duration_;

protected:

  friend class EventGenerator;

  Event(int index, long duration) 
    :index_(index),duration_(duration % 99999999L) 
  {}

  ~Event() 
  { cerr << "event destructor" << endl; }

public:

  void run(void)
  {
    cerr << "event " << index_ << " start" << endl;
    Thread::nanosleepCurrent(0,duration_);
    cerr << "event " << index_ << " stop" << endl;
    delete this;
  }

};


class EventGenerator: public Thread
{
 ThreadService* threadService_; 
public:

  EventGenerator(ThreadService* threadService)
   :threadService_(threadService)
  {
   cerr << "EventGenerator constructor" << endl;
  }

  void run(void);

};

void EventGenerator::run(void)
{
 cerr << "EventGenerator running" << endl;
 for(int n=0; n<10000; ++n) {
   #ifdef WIN32
    Event* event = new Event(n,rand());
    nanosleep(0,rand());
   #else
    Event* event = new Event(n,random());
    nanosleep(0,random() % 999999999L);
   #endif
   bool success=false;
   while(!success) {
     try {
       threadService_->process(event);
       success = true;
     }catch(ThreadService::NotActive ex){
       cerr << "Thread Service is not active" << endl;
       return;
     }catch(ThreadingExceptions::TemporaryNoResources){
       cerr << "can't process Event " << n <<" , retrying" << endl;
       #ifdef WIN32
        nanosleep(0,rand());
       #else
        long l = random() % 999999999L;
        cerr << "nanosleep(0," << l << ");" << endl;
        nanosleep(0,l); 
       #endif
       yield();
     }catch(ThreadingExceptions::Failure){
       cerr << "Threading failure, exiting" << endl;
       return;
     }catch(...){
       cerr << "catched unknown exception" << endl;
       return;
     }
   }
 }
}

int readQuerySize(int argc, char** argv, int index);
int readThreadProcessingMode(int argc, char** argv, int index);
long  readTimeout(int argc, char** argv, int index);
int  readNumberOfThreads(int argc, char** argv, int index);

int main(int argc, char** argv)
{
  ThreadService* ts;
  if (argc < 2) {
    cerr << "Usage: ThreadServiceDemo threading-type params" << endl;
    cerr << "where  threading-type params are:" << endl;
    cerr << "  single-thread-blocking" << endl;
    cerr << "  single-thread-checked" << endl;
    cerr << "  single-thread-reactive qSize [blocked|checked|with-timeout] [timeout]" << endl;
    cerr << "  thread-per-client " << endl;
    cerr << "  thread-pool nThreads qSize [blocked|checked|with-timeout] [timeout]" << endl;
    return 1;
  }
  if (!strcmp(argv[1],"single-thread-blocking")) {
      ts=new SingleThreadBlocking();
  }else if (!strcmp(argv[1], "single-thread-checked")) {
      ts=new SingleThreadChecking();
  }else if (!strcmp(argv[1], "single-thread-reactive")) {
      int qSize = readQuerySize(argc,argv,2);
      int imode = readThreadProcessingMode(argc,argv,3);
      SingleThreadReactive::Mode mode;
      switch(imode) {
         case 0: mode = SingleThreadReactive::Blocked;
                 break;
         case 1: mode = SingleThreadReactive::Checked;
                 break;
         case 2: mode = SingleThreadReactive::CheckedWithTimeout;
                 break;
         default:
                 cerr << "Invalid mode" << endl;
                 return 1;
      }
      long timeout = 0;
      if (mode==SingleThreadReactive::CheckedWithTimeout) {
         timeout = readTimeout(argc,argv,4);
      }
      ts = new SingleThreadReactive(qSize,mode,timeout);
  }else if (!strcmp(argv[1], "thread-per-client")) {
      ts = new ThreadPerClient();
  }else if (!strcmp(argv[1], "thread-pool")) {
      int nThreads = readNumberOfThreads(argc,argv,2);
      int qSize = readQuerySize(argc,argv,3);
      int imode = readThreadProcessingMode(argc,argv,4);
      ThreadPool::Mode mode;
      switch(imode) {
         case 0: mode = ThreadPool::Blocked;
                 break;
         case 1: mode = ThreadPool::Checked;
                 break;
         case 2: mode = ThreadPool::CheckedWithTimeout;
                 break;
         default:
                 cerr << "Invalid mode" << endl;
                 return 1;
      }
      long timeout = 0;
      if (mode==ThreadPool::CheckedWithTimeout) {
         timeout = readTimeout(argc,argv,5);
      }
      ts = new ThreadPool(nThreads,qSize,mode,timeout);
  }else{
      cerr << "Invalid first parameter, please, run program without arguments\n"
              "to see help screen " << endl;
      return 1; 
  }

  
  try {
    ts->activate();
  }catch(const ThreadingExceptions::Failure){
    cerr << argv[0] << ": can not activate service" << endl;
    return 1;
  }
      
  EventGenerator eg(ts);

  cerr << "before eg starting" << endl;
  eg.start();
  cerr << "after eg starting" << endl;

  Thread::sleepCurrent(100);

  ts->deactivate(false);

  Thread::sleepCurrent(10);
  delete ts;

  return 0;
}

int readInt(int argc, char** argv, int index)
{
 if (argc <= index) {
   cerr << "Parameter " << index << " needed" << endl;
   exit(1);
 }
 char* strtolend;
 int retval = (int)strtol(argv[index],&strtolend,10);
 if (strtolend==argv[index]) {
   cerr << "Parameter " << index << " : number required" << endl;
   exit(1);
 }
 return retval;
}

int readQuerySize(int argc, char** argv, int index)
{
 return readInt(argc,argv,index);
}

int readNumberOfThreads(int argc, char** argv, int index)
{
 return readInt(argc,argv,index);
}


int readThreadProcessingMode(int argc, char** argv, int index)
{
 if (argc <= index) {
   cerr << "Parameter " << index << " needed" << endl;
   exit(1);
 }
 if (strcmp(argv[index],"blocked")==0) {
   return 0;
 }else if(strcmp(argv[index],"checked")==0) {
   return 1;
 }else if(strcmp(argv[index],"with-timeout")==0) {
   return 2;
 }else{
   cerr << "Parameter " << index << 
           " must be one of blocked,checked,with-timeout" << endl;
   exit(1);
 }
}

long readLong(int argc, char** argv, int index)
{
 if (argc <= index) {
   cerr << "Parameter " << index << " needed" << endl;
   exit(1);
 }
 char* strtolend;
 long retval = strtol(argv[index],&strtolend,10);
 if (strtolend==argv[index]) {
   cerr << "Parameter " << index << " : number required" << endl;
   exit(1);
 }
 return retval;
}

long  readTimeout(int argc, char** argv, int index)
{
  return readLong(argc,argv,index);
}


