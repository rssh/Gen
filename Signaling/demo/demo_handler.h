#ifndef _demo_handler_h_ 
#define _demo_handler_h_ 1


#include <GradSoft/Signaling.h>

#include <iostream>
#ifndef WIN32
#include <unistd.h>
#else
#include <windows.h>
namespace {
    HANDLE __ctrl_evnt_for_pause = CreateEvent(NULL, true, false, NULL);
}
#endif

class demo_handler : public GradSoft::SignalHandler {
public:
      demo_handler() {};
      virtual void handler(unsigned long insignal) {
          int sig_num = GradSoft::SignalSet::internal2system(insignal);
          std::cerr << "Ctrl+C has catched" << std::endl;
#ifdef WIN32
          PulseEvent(__ctrl_evnt_for_pause);
#endif
      }
private:
      demo_handler(const demo_handler&) {};
};

class demo_handlerNew : public GradSoft::SignalHandler {
public : 
      /**
       * My handler.
       **/
      demo_handlerNew() {};
      virtual void handler(unsigned long insignal) 
      {
        using namespace GradSoft;
        std::cerr << "Catch signal |" ;
        switch ( insignal  ) {
          case SignalSet::sSIGHUP  : std::cerr << "SIGHUP"  ; break;
          case SignalSet::sSIGINT  : std::cerr << "SIGINT"  ; break; 
          case SignalSet::sSIGQUIT : std::cerr << "SIGQUIT" ; break;
          case SignalSet::sSIGILL  : std::cerr << "SIGILL"  ; break;
          case SignalSet::sSIGABRT : std::cerr << "SIGABRT" ; break;
          case SignalSet::sSIGFPE  : std::cerr << "SIGFPE"  ; break;
          case SignalSet::sSIGKILL : std::cerr << "SIGKILL" ; break;
          case SignalSet::sSIGSEGV : std::cerr << "SIGSEGV" ; break;
          case SignalSet::sSIGPIPE : std::cerr << "SIGPIPE" ; break;
          case SignalSet::sSIGALRM : std::cerr << "SIGALRM" ; break;
          case SignalSet::sSIGTERM : std::cerr << "SIGTERM" ; break;
          case SignalSet::sSIGUSR1 : std::cerr << "SIGUSR1" ; break;
          case SignalSet::sSIGUSR2 : std::cerr << "SIGUSR2" ; break;
          case SignalSet::sSIGCHLD : std::cerr << "SIGCHLD" ; break;
          case SignalSet::sSIGCONT : std::cerr << "SIGCONT" ; break;
          case SignalSet::sSIGSTOP : std::cerr << "SIGSTOP" ; break;
          case SignalSet::sSIGTSTP : std::cerr << "SIGTSTP" ; break;
          case SignalSet::sSIGTTIN : std::cerr << "SIGTTIN" ; break;
          case SignalSet::sSIGTTOU : std::cerr << "SIGTTOU" ; break;
        } 
        std::cerr << "| with system number <" 
                  << SignalSet::internal2system(insignal) << "> " ;
        std::cerr << "and internal number <" << insignal << ">" << std::endl;
#ifdef WIN32
        PulseEvent(__ctrl_evnt_for_pause);
#endif
      }
private:
      demo_handlerNew(const demo_handler&) {};
};

#endif
