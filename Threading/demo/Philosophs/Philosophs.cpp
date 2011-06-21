
#include <GradSoft/Threading.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <stdlib.h>

#include <iostream>
using namespace std;


#define N_PHILOSOPHS  5
#define N_FORKS  (N_PHILOSOPHS)

#define MAX_SLEEP_TIME 3
#define EAT_TIME       5

using namespace GradSoft;

Mutex forks[N_FORKS];

class Philosoph: public Thread
{
private:
 int n_;
public:

 Philosoph(int n=0)
  :n_(n)
 {}

 void  set_number(int n) { n_ = n; }

 Mutex* left_fork() { return &forks[n_]; }
 Mutex* right_fork() { return &forks[(n_+1)%N_FORKS]; }

 void run(void);

 void random_sleep(void);
 void eat(void);

};

int main(void)
{
  Philosoph philosophs[N_PHILOSOPHS];
  int i=0;
  for(i=0; i<N_PHILOSOPHS; ++i) {
    philosophs[i].set_number(i);
    philosophs[i].start();
  }

// need to philosophs have time to start
  Thread::sleepCurrent(2); 

// wait until they are running:
WAIT:
  for(i=0; i<N_PHILOSOPHS; ++i){
    if (philosophs[i].is_running()) {
      Thread::sleepCurrent(1);
      goto WAIT;
    }
  }

  cout<<endl
      <<"lunch is finished"<<endl
      <<endl;
  return 0;
}

Mutex cerr_mutex;

void Philosoph::run(void)
{
 do{  
   if (left_fork()->try_lock()) 
   {
	 if (right_fork()->try_lock()) goto EAT;
     else left_fork()->unlock();
   } 
   random_sleep();
 }while(true);

EAT:
  eat();
  left_fork()->unlock();
  right_fork()->unlock();
}

void Philosoph::random_sleep(void)
{
  int random_value;
#ifdef WIN32
  random_value = rand();
#else
  random_value = random();
#endif
  long sleep_time = (random_value % MAX_SLEEP_TIME); 
  cerr_mutex.lock();
    cerr << n_ << ": sleep for " << sleep_time <<" sec"<< endl;
  cerr_mutex.unlock();
  sleep(sleep_time);
  cerr_mutex.lock();
    cerr << n_ << ": sleep for " << sleep_time <<" sec"<< endl;
  cerr_mutex.unlock();
}

void Philosoph::eat(void)
{
  cerr_mutex.lock();
    cerr << n_ << ": begin feed" << endl;
  cerr_mutex.unlock();
  sleep(EAT_TIME);
  cerr_mutex.lock();
    cerr << n_ << ": end feed" << endl;
  cerr_mutex.unlock();
}

