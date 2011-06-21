/*
 * part of GradSoft C++ ToolBox.
 * Threading.
 * (C) GradSOft 2000, 2001
 * $Id: Hairdresser.cpp,v 1.7 2001-12-26 09:00:05 kav Exp $
 */

#include <GradSoft/Threading.h>

#include <iostream>

using namespace std;
using namespace GradSoft;

int client_no = 0;
RWLock client_no_rw;

int client_count = 0;
RWLock client_count_rw;

RWLock client_rw;
RWLock busy_rw;

RWLock cerr_rw;

class Client;
class Hairdresser;

/**
*  N_MAXCLIENTS - maximum number of clients in waiting room
**/
const int N_MAXCLIENTS = 4;

const int HAIRCAT_TIME = 2;

ThreadEvent condition;
ThreadEvent condition2;

Mutex mutex;

bool bedtime=false;
RWLock bedtimeRWLock;


class Hairdresser: public Thread
{
public:
  Hairdresser();
  void run();
  Client* client;
  bool busy;
};

class Client: public Thread
{
  Hairdresser* hairdresser_;
public:
  Client(); 
  ~Client();
  int number;
  void run();
  void chooseHairdresser(Hairdresser* hairdresser);
};

Hairdresser::Hairdresser()
{
  cerr_rw.write_lock();
  cerr<<"Hairdresser created"<<endl;
  cerr_rw.write_unlock();
  client_rw.write_lock();
  client = NULL; 
  client_rw.write_unlock();
  busy_rw.write_lock();
  busy = false;
  busy_rw.write_unlock();
}

void Hairdresser::run()
{
  cerr_rw.write_lock();
  cerr<<"Hairdresser begin work"<<endl;
  cerr_rw.write_unlock();

  while (true) {
    client_rw.write_lock();
    if (client != NULL){
      // Hairdresser make haircut for client
      cerr_rw.write_lock();
      cerr<<"Hairdresser makes haircut for client "<<client->number<<endl;
      cerr_rw.write_unlock();

      sleep(HAIRCAT_TIME);

      // client leave hairdressing salon
      cerr_rw.write_lock();
      cerr<<"Client "<<client->number<<" : leave hairdressing salon"<<endl;
      cerr_rw.write_unlock();

      client_rw.write_unlock();
      {
       WriteLocker wl(client_count_rw);
       client_count--;
      }
  
      // Hairdresser looks at watch
      testcancel();
      {
        ReadLocker rl(bedtimeRWLock);
        if (bedtime) break;
      }

      // Hairdresser invite next client
      cerr_rw.write_lock();
      cerr<<"Hairdresser: invite next client"<<endl;
      cerr_rw.write_unlock();

      busy_rw.write_lock();
      busy = false;
      busy_rw.write_unlock();
      condition.notify();
      // time to get next client
      sleep(0);
    }else{
      // Hairdresser wait for clients
      client_rw.write_unlock();
      cerr_rw.write_lock();
      cerr<<"Hairdresser sleep while in waiting room no one client"<<endl;
      cerr_rw.write_unlock();
      condition2.wait();
    }
  }
  {
    ReadLocker rl(client_count_rw);
    if (client_count>0) condition.notifyAll();
  }
}

Client::Client()
{
  client_no_rw.write_lock();
  number = ++client_no;
  cerr_rw.write_lock();
  cerr<<"Client "<<number<<" created"<<endl;
  cerr_rw.write_unlock();
  client_no_rw.write_unlock();
}

void Client::chooseHairdresser(Hairdresser* hairdresser)
{
  hairdresser_ = hairdresser;
}

Client::~Client()
{
  cerr_rw.write_lock();
  cerr<<"Client "<<number<<" destroyed"<<endl;
  cerr_rw.write_unlock();
}

void Client::run()
{
  busy_rw.read_lock();

  if (hairdresser_->busy){
    busy_rw.read_unlock();

    // client wait until Hairdresser make haircut for another client
    cerr_rw.write_lock();
    cerr<<"Client "<<number<<" : wait"<<endl;
    cerr_rw.write_unlock();
    condition.wait();
    // client enter to Hairdresser room
    busy_rw.write_lock();
    hairdresser_->busy = true;

    cerr_rw.write_lock();
    cerr<<"Client "<< number <<" : enter to Hairdresser room"<<endl;
    cerr_rw.write_unlock();

    client_rw.write_lock();
    hairdresser_->client = this;
    client_rw.write_unlock();

    busy_rw.write_unlock();

  }else{
    busy_rw.read_unlock();

    // client enter to Hairdresser room
    busy_rw.write_lock();
    hairdresser_->busy = true;
    busy_rw.write_unlock();

    client_rw.write_lock();
    hairdresser_->client = this;
    client_rw.write_unlock();


    cerr_rw.write_lock();
    cerr<<"Client "<<number<<" wake up Hairdresser"<<endl;
    cerr_rw.write_unlock();
    condition2.notify();
  }
}

#define CLIENT_NUMBER 10
#define MAXIMUM_INTERVAL 3
#ifdef WIN32
#define RANDOM_VALUE rand()
#else
#define RANDOM_VALUE random()
#endif

int main(void)
{
  cerr << "Workday started" << endl;

  Hairdresser hairdresser;
  hairdresser.start();
  Thread::sleepCurrent(2);

  Client clients[CLIENT_NUMBER];
  for (int i=0; i<CLIENT_NUMBER;){
    {
      WriteLocker wl(client_count_rw);
      if (client_count < N_MAXCLIENTS){
        clients[i].chooseHairdresser(&hairdresser);
        if(hairdresser.is_running()){
          ++client_count;
          clients[i].start();
        }else break;
        ++i;
      }
    }//this is essential
    Thread::yield();
    Thread::sleepCurrent(RANDOM_VALUE % MAXIMUM_INTERVAL);
  }
  Thread::sleepCurrent(4);
  {
    WriteLocker wl(bedtimeRWLock);
    bedtime=true;
  }

  Thread::sleepCurrent(5);

  cerr_rw.write_lock();
  cerr<<"End of work "<<endl;
  cerr_rw.write_unlock();
  return 0;
}

