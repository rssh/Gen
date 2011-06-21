
#define _REENTRAT

#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include <stdio.h>
#include <strstream>

#include <iostream>
using namespace std;

pthread_cond_t condition;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;
pthread_mutex_t g_mutex;
pthread_attr_t attr;
pthread_t id1;
pthread_t id2;
pthread_t id3;

int my_pthread_mutex_lock(pthread_mutex_t *pmutex)
{
  int retcode = pthread_mutex_lock(pmutex);
  if (retcode != 0) {
    cerr << pthread_self() << ":pthread_mutex_lock failed " << retcode << endl;
  }else{
    cerr << pthread_self() << ":pthread_mutex_lock success " << retcode << endl;
  }
  return retcode;
}

int my_pthread_cond_wait(pthread_cond_t* pcond, pthread_mutex_t *pmutex)
{
  int retcode = pthread_cond_wait(pcond,pmutex);
  if (retcode != 0) {
    cerr << pthread_self() << ":pthread_cond_wait failed " << retcode << endl;
  }else{
    cerr << pthread_self() << ":pthread_cond_wait success " << retcode << endl;
  }
  return retcode;
}

int my_pthread_cond_init(pthread_cond_t* pcond, pthread_condattr_t* pcondattr)
{
  int retcode = pthread_cond_init(pcond,pcondattr);
  if (retcode != 0) {
    cerr << pthread_self() << ":pthread_cond_init failed " << retcode << endl;
  }else{
    cerr << pthread_self() << ":pthread_cond_init success " << retcode << endl;
  }
  return retcode;
}

int my_pthread_mutex_init(pthread_mutex_t* pmutex, pthread_mutexattr_t* pmutexattr)
{
  int retcode = pthread_mutex_init(pmutex,pmutexattr);
  if (retcode != 0) {
    cerr << pthread_self() << ":pthread_mutex_init failed " << retcode << endl;
  }else{
    cerr << pthread_self() << ":pthread_mutex_init success " << retcode << endl;
  }
  return retcode;
}

int my_pthread_attr_init(pthread_attr_t* pattr)
{
  int retcode = pthread_attr_init(pattr);
  if (retcode != 0) {
    cerr << pthread_self() << ":pthread_attr_init failed " << retcode << endl;
  }else{
    cerr << pthread_self() << ":pthread_attr_init success " << retcode << endl;
  }
  return retcode;
}

int my_pthread_create(pthread_t* thr,
                      pthread_attr_t* pattr,
                      void* (*fun)(void*),
                      void * data)  
{
  int retcode = pthread_create(thr, pattr, fun, data);
  if (retcode != 0){
    cerr << "pthread_create fail with code " << retcode << endl;
    return 1;
  }  
}

void* process1(void* data)
{
  pthread_mutex_t* pmutex = (pthread_mutex_t*)data;
  cerr << pthread_self() << ": process1 started." << endl;
  my_pthread_mutex_lock(pmutex);
  cerr << pthread_self() << ": mutex accuired" << endl;
  my_pthread_cond_wait(&condition, pmutex);
  cerr << pthread_self() << ": end" << endl;
  pthread_mutex_unlock(pmutex);
  return NULL;
}


int main(void)
{
  int retcode = 0;

  retcode = my_pthread_cond_init(&condition, NULL);


  retcode = my_pthread_mutex_init(&mutex1, NULL);
  retcode = my_pthread_mutex_init(&mutex2, NULL);
  retcode = my_pthread_mutex_init(&mutex3, NULL);

  retcode = my_pthread_attr_init(&attr);

  retcode = my_pthread_create(&id1, NULL /*&attr*/, process1, &mutex1);
  retcode = my_pthread_create(&id2, NULL /*&attr*/, process1, &mutex2);
  retcode = my_pthread_create(&id3, NULL /*&attr*/, process1, &mutex3);

  sleep(1); 
  retcode = pthread_cond_broadcast(&condition);
  if (retcode != 0){
    cerr << "pthread_cond_broadcast(&condition) fail with code " << retcode << endl;
    return 1;
  }else{
    cerr << "pthread_cond_broadcast success" << endl;
  }

  sleep(10);
  return 0;
}

