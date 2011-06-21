#include <GradSoft/Signaling.h>

#ifdef HAVE_NAMESPACES
using namespace std;
#endif

#ifdef HAVE_NAMESPACES
namespace GradSoft {
#endif
SignalHandler*    Signaling::currentSignalHandlers_[ SignalSet::sNSIG ];
SignalSet         Signaling::currentSignalMask_;


int   SignalSet::signal2bitNumber( const unsigned long insignum )
{
   int i, one=1;
   for( i = 0; !( insignum & one ) && ( i < sNSIG ); i++ )
       one <<= 1;
   return i;
}

unsigned long   SignalSet::system2internal( const int sig_num )
{
   switch ( sig_num  ) {
#ifdef WIN32
      case SIGINT  : return sSIGINT  ;
      case SIGILL  : return sSIGILL  ;
      case SIGFPE  : return sSIGFPE  ;
      case SIGSEGV : return sSIGSEGV ;
      case SIGTERM : return sSIGTERM ;
      case SIGABRT : return sSIGABRT ;
      case SIGBREAK : return sSIGQUIT ;
#else
      case SIGHUP  : return sSIGHUP  ;
      case SIGINT  : return sSIGINT  ;
      case SIGQUIT : return sSIGQUIT ;
      case SIGILL  : return sSIGILL  ;
      case SIGABRT : return sSIGABRT ;
      case SIGFPE  : return sSIGFPE  ;
      case SIGKILL : return sSIGKILL ;
      case SIGSEGV : return sSIGSEGV ;
      case SIGPIPE : return sSIGPIPE ;
      case SIGALRM : return sSIGALRM ;
      case SIGTERM : return sSIGTERM ;
      case SIGUSR1 : return sSIGUSR1 ;
      case SIGUSR2 : return sSIGUSR2 ;
      case SIGCHLD : return sSIGCHLD ;
      case SIGCONT : return sSIGCONT ;
      case SIGSTOP : return sSIGSTOP ;
      case SIGTSTP : return sSIGTSTP ;
      case SIGTTIN : return sSIGTTIN ;
      case SIGTTOU : return sSIGTTOU ;
#endif
   }
   return 0;
}

int   SignalSet::internal2system( const unsigned long insig_num )
{
   switch ( insig_num  ) {
#ifdef WIN32
      case sSIGINT  : return SIGINT  ;
      case sSIGILL  : return SIGILL  ;
      case sSIGFPE  : return SIGFPE  ;
      case sSIGSEGV : return SIGSEGV ;
      case sSIGTERM : return SIGTERM ;
      case sSIGABRT : return SIGABRT ;
      case sSIGQUIT : return SIGBREAK;
#else
      case sSIGHUP  : return SIGHUP  ;
      case sSIGINT  : return SIGINT  ;
      case sSIGQUIT : return SIGQUIT ;
      case sSIGILL  : return SIGILL  ;
      case sSIGABRT : return SIGABRT ;
      case sSIGFPE  : return SIGFPE  ;
      case sSIGKILL : return SIGKILL ;
      case sSIGSEGV : return SIGSEGV ;
      case sSIGPIPE : return SIGPIPE ;
      case sSIGALRM : return SIGALRM ;
      case sSIGTERM : return SIGTERM ;
      case sSIGUSR1 : return SIGUSR1 ;
      case sSIGUSR2 : return SIGUSR2 ;
      case sSIGCHLD : return SIGCHLD ;
      case sSIGCONT : return SIGCONT ;
      case sSIGSTOP : return SIGSTOP ;
      case sSIGTSTP : return SIGTSTP ;
      case sSIGTTIN : return SIGTTIN ;
      case sSIGTTOU : return SIGTTOU ;
#endif
   }
   return 0;
}

bool SignalSet::isSysSet(const int signum)
{
   return isBitSet(signal2bitNumber(system2internal(signum)));
}

bool   SignalSet::isBitSet( const int bitNumber )
{
   bool retval;
   if ( bitNumber > sNSIG)  return false;
   retval = ( (sigmask_ & bitNumber2signal(bitNumber)  ) ? (true) : (false) );
   return retval;
}

void Signaling::set_handler( const unsigned long  insignum, 
                             void (*handler)(int))
                                       throw(Signaling::Error)
{
   int signum = SignalSet::internal2system( insignum );
   int indx   = SignalSet::signal2bitNumber( insignum );
   if ( indx <= 0 ) return;
#ifndef _MSC_VER

#ifdef HAVE_SIGACTION
   struct sigaction action;
   sigemptyset(&action.sa_mask);
   action.sa_handler =  handler;
   action.sa_flags = 0;
   if ( sigaction(signum, &action, &oldHandlers_[indx]) == -1 ) 
       throw( Error( strerror(errno) ) );
#elif HAVE_SIGNAL
   oldHandlers_[indx] =  signal( signum, handler );
   if ( oldHandlers_[indx] == SIG_ERR )
       throw Error("Signal: SIG_ERR");
#endif

#else // For WINDOWS 
      oldHandlers_[indx] =  signal( signum, handler );
   if ( oldHandlers_[indx] == SIG_ERR )
       throw Error("Signal: SIG_ERR");
#endif
}

void  Signaling::unset_handler(const unsigned long insignum)
                                       throw(Signaling::Error)
{
   int  signum = SignalSet::internal2system( insignum );
   int  indx = SignalSet::signal2bitNumber( insignum );
   if ( indx < 0 ) return;
#ifndef _MSC_VER

#ifdef HAVE_SIGACTION
   struct sigaction action;
   if ( sigaction(signum, &oldHandlers_[indx], &action ) == -1 ) 
      throw( Error(strerror(errno)) );
#elif HAVE_SIGNAL
   oldHandlers_[indx] = signal( signum, (void(*) (int)) oldHandlers_[indx] );
   if ( oldHandlers_[indx] == SIG_ERR ) 
      throw Error("Signal: SIG_ERR");
#endif

#else // For WINDOWS
   oldHandlers_[indx] = signal( signum, (void(*) (int)) oldHandlers_[indx] );
   if ( oldHandlers_[indx] == SIG_ERR ) 
      throw Error("Signal: SIG_ERR");
#endif
} 

bool  Signaling::isSysSet(const int sig_num)
{
      return ( currentSignalMask_.isSysSet(sig_num) );
}

bool  Signaling::isBitSet(const int bitnum)
{
      return ( currentSignalMask_.isBitSet( bitnum ));
}

Signaling::Signaling(SignalSet signalSet, SignalHandler& handler )
                                       throw(Signaling::Error) 
{
   memcpy(oldSignalHandlers_, currentSignalHandlers_, 
                              sizeof(currentSignalHandlers_)); 
   oldSignalMask_ = currentSignalMask_;
   currentSignalMask_ = signalSet;
   for( int i = 0; i < SignalSet::sNSIG; i++ ) {
      if ( currentSignalMask_.isBitSet( i ) ) {
          currentSignalHandlers_[i] = &handler;
          set_handler( SignalSet::bitNumber2signal(i), call_handler );
      }
   }
   currentSignalMask_ |= oldSignalMask_;
}

Signaling::~Signaling()
{
   for( int i = 0; i < SignalSet::sNSIG; i++ ) {
      if ( currentSignalMask_.isBitSet( i ) ) {
          try { unset_handler( SignalSet::signal2bitNumber(i) ); }
          catch( const Signaling::Error& ) {};
      }
   }
   memcpy(currentSignalHandlers_, oldSignalHandlers_, 
   sizeof(oldSignalHandlers_));
   currentSignalMask_ = oldSignalMask_;
   memset(oldSignalHandlers_, 0, sizeof(oldSignalHandlers_));
}

void  Signaling::call_handler(int sig_num)
{
#ifdef HAVE_SIGNAL
    signal( sig_num, call_handler );
#endif
    unsigned long  insig_num = SignalSet::system2internal( sig_num );
    int indx = SignalSet::signal2bitNumber( insig_num );
    if ( currentSignalMask_.isBitSet( indx ) )
         currentSignalHandlers_[ indx ]->handler( insig_num );
}

#ifdef HAVE_NAMESPACES
}
#endif
