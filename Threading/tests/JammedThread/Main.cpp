#include <string>
#include <iostream>
#include <stdio.h>

#include <GradSoft/Threading.h>

/*
 *  Test example to show how ThreadEvent->wait() jamms program
 *  $Id: Main.cpp,v 1.1 2002-08-27 13:59:59 rin Exp $
 *
 */

namespace GradSoft {


 class TicksThread : public Thread
 {
  private:
    bool repeat_;
    ThreadEvent core_;

  public:
    TicksThread() : Thread()
    {  std::cerr << "TicksThread::  Constructor complete." << std::endl;  };
    ~TicksThread()
    {  std::cerr << "TicksThread::  Destructor called." << std::endl;  };

    void notify(void) { core_.notifyAll(); };
    void wait(void) { core_.wait(); };
    void run(void)
    {
       repeat_ = true;
       do {
          std::cout << ".";
          Thread::sleep(1);
          this->notify();
       } while (repeat_);
    };
 }; // of class


 class JammedThread : public Thread
 {
  private:
    bool doRepeat;
    TicksThread eventManager_;

  public:
    JammedThread() : Thread()
    {
       eventManager_.start();
       std::cerr << "JammedThread::  Constructor complete." << std::endl;
    };

    ~JammedThread()
    {
       std::cerr << "JammedThread::  Destructor called." << std::endl;
       doRepeat = false;
    };

    void run(void)
    {
       long i = 0;
       doRepeat = true;
       do {
          // Do some work
          i++;  std::cerr << ' ' << i << ' ';
          // Than wait               
          std::cerr << '[';
          eventManager_.wait();  // NOTE: This wait call jams the destructor
          std::cerr << ']' << std::endl;
       } while (doRepeat);
    };
  
 }; // of class


}; // of namespace



int main(int argc, char** argv)
{
   std::string str;
   GradSoft::JammedThread thread;

   std::cerr << " {STARTING... ";
   thread.start();
   std::cerr << " DONE} " << std::endl;

   std::cerr << " {PRESS <ENTER> TO TERMINATE... ";
   getchar();
   std::cerr << " DONE} " << std::endl;

   std::cerr << std::endl << " {PROGRAM COMPLETE AND IS JAMMED, USE <CTRL>+C} " << std::endl;
   return 0;
}


