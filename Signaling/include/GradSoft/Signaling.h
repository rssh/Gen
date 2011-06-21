#ifndef _Signaling_h_ 
#define _Signaling_h_ 1


#include <csignal>
#include <cerrno>
#include <string>

#ifdef _MSC_VER
#define WIN32
#define HAVE_SIGNAL
#endif


#ifdef HAVE_NAMESPACES
namespace GradSoft {
#endif

///
class SignalHandler 
{
public: 

      ///
      SignalHandler() {};

      /**
       * method which handle handler: must be implemented
       * by library client.
       * @param signal internal signal number in SignalSet
       **/
      virtual void handler(unsigned long signal) = 0;

      ///
      virtual ~SignalHandler() {}

private:
      SignalHandler(const SignalHandler&) {};
      SignalHandler&  operator=(const SignalHandler&);
};


///
class SignalSet 
{
   friend   class Signaling;
public:

// every signal has its own bit in "unsigned long" (internal number)
// signal and its bit  
enum SigNames {
     sSIGHUP  = 0x1     ,
     sSIGINT  = 0x2     ,
     sSIGQUIT = 0x4     ,
     sSIGILL  = 0x8     ,
     sSIGABRT = 0x10    ,
     sSIGFPE  = 0x20    ,
     sSIGKILL = 0x40    ,
     sSIGSEGV = 0x80    ,
     sSIGPIPE = 0x100   ,
     sSIGALRM = 0x200   ,
     sSIGTERM = 0x400   ,
     sSIGUSR1 = 0x800   ,
     sSIGUSR2 = 0x1000  ,
     sSIGCHLD = 0x2000  ,
     sSIGCONT = 0x4000  ,
     sSIGSTOP = 0x8000  ,
     sSIGTSTP = 0x10000 ,
     sSIGTTIN = 0x20000 ,
     sSIGTTOU = 0x40000 
};


// all signals count.
// max value of this contant = 32 
#ifdef WIN32
enum NSignals {
     sNSIG = 7,
     sENABLE_ALL = 0x2 | 0x8 | 0x80 | 0x400 | 0x10 | 0x4
};
#else
enum NSignals {
     sNSIG = 19,
     sENABLE_ALL = 0x7FFFF - 0x40 - 0x8000
};
#endif

private:

  unsigned long     sigmask_;

public: 

  SignalSet( unsigned long sig_mask = 0L ): sigmask_(sig_mask)  
  {}

  /**
   * Operator "OR"
   **/
  SignalSet& operator|=( const SignalSet& sigset  ) {
     sigmask_ |= sigset.sigmask_; 
     return *this;
  }

  /**
   * Operator "OR"
   **/
  SignalSet operator|( const SignalSet& sigset  ) {
     return SignalSet(sigmask_|sigset.sigmask_);
  }


  /**
   * Operator "AND"
   **/
  SignalSet& operator&=( const SignalSet& sigset  ) {
     sigmask_ &= sigset.sigmask_; 
     return *this;
  }

  /**
   * Operator "AND"
   **/
  SignalSet operator&( const SignalSet& sigset  ) {
     return SignalSet(sigmask_ & sigset.sigmask_);
  }

private:
  /**
   * Get internal signal number correspondent to bit with number bitnum
   * @param bitnum number of bit in unsigned long
   * @return internal signal number
   **/
  static unsigned long  bitNumber2signal( const int bitnum )
  {
      return ( 1 << bitnum );
  }

  /**
   * Get bit number correspondent to internal signal with number insignum
   * @param signum internal signal number
   * @return bit number
   **/
  static int     signal2bitNumber( const unsigned long insignum );

public:
  /**
   * Translate system signal number (System V, POSIX) to internal signal number
   * @param sig_num number of system signal
   * @return internal signal number
   **/
  static unsigned long      system2internal( const int sig_num );

  /**
   * Translate from internal signal number to system signal number
   * @param insig_num number of internal signal
   * @return system signal number
   **/
  static int   internal2system( const unsigned long insig_num );

  /**
   *  Check is system signal in signal mask
   *  @param sig_num system signal number
   *  @return true if signal is set
   **/
  bool isSysSet( const int sig_num );

  /**
   * Check bit is set or not in signal mask.
   * @param bitnum number bit.
   * @return true if set
   **/
  bool   isBitSet( const int bitnum );
 

};

///
class Signaling 
{
public:
  ///
  class Error 
  {
    public:
    ///
    std::string   why;
    ///
    Error(const char* str ) { why = str;}
    Error(const std::string& str ) { why = str; }
    const char* what() const { return why.c_str(); }
  };

private:

  // saved previous function-handler which returned by "signal" or "sigaction".
#ifndef _MSC_VER

# ifdef HAVE_SIGACTION
     struct sigaction       oldHandlers_[ SignalSet::sNSIG ];
# elif HAVE_SIGNAL
     void*                  oldHandlers_[ SignalSet::sNSIG ];
# endif

#else 
     void*                  oldHandlers_[ SignalSet::sNSIG ];
#endif

   SignalHandler*           oldSignalHandlers_[ SignalSet::sNSIG ];
   SignalSet                oldSignalMask_;

   static SignalHandler*    currentSignalHandlers_[ SignalSet::sNSIG ];
   static SignalSet         currentSignalMask_;

public:

  ///
  Signaling( SignalSet signalSet, SignalHandler& handler ) 
                                             throw(Error);

  ~Signaling();

  /**
   * The same as SignalSet::isSysSet( int )
   * @param  signalNumber system signal number
   * @return true if set
   **/
  bool  isSysSet( const  int signalNumber );

  /*
   * The same as SignalSet::isBitSet( int )
   * @param bitnum system signal number
   * @return true if set
   **/
  bool  isBitSet( const  int bitnum );

private:

  /**
   * Set handler for concrete signal
   * @param  insignum  internal signal number 
   * @param  handler  function handler
   * @return old handler
   **/
  void set_handler( const unsigned long  insignum, void (*handler)(int))
                                           throw(Signaling::Error);

  /**
   * Opposite to previous method.
   **/
  void  unset_handler( const unsigned long insignum )
                                           throw(Signaling::Error);

  static void  call_handler(int sig_num);
};
 
#ifdef HAVE_NAMESPACES
}
#endif

#endif

