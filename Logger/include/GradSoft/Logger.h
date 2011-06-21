#ifndef __GradSoft_Logger_h
#define __GradSoft_Logger_h

/*
 * part of GradSoft C++ ToolBox: Logger
 * (C) GradSoft, Kiev, Ukraine 2000, 2001
 * http://www.gradsoft.com.ua
 * author: Ruslan Shevchenko <Ruslan@Shevchenko.Kiev.UA>, 2000
 * $Id: Logger.h,v 1.27 2008-02-25 09:40:09 rssh Exp $
 */

#include <GradSoft/LoggerPostConfig.h>

#include <GradSoft/Threading.h>

#include <cstddef>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

/**
 *
 **/
namespace GradSoft {


 /**
  * Types of log events:
  *  Debug, Info, Warning, Error, Fatal
  **/  
 enum LogEventType
 {
  ///
  Debug=0,
  ///
  Info=1,
  ///
  Warning=2,
  ///
  Error=3,
  ///
  Fatal=4  
 };

 struct DebugLogEventStruct 
 {
   static const LogEventType tag;
 };
 struct InfoLogEventStruct 
 {
   static const LogEventType tag;
 };
 struct WarningLogEventStruct 
 {
   static const LogEventType tag;
 };
 struct ErrorLogEventStruct 
 {
   static const LogEventType tag;
 };
 struct FatalLogEventStruct {
   static const LogEventType tag;
 };

 template<class EventType, bool enabled>
 class LogStream;

/**
 * lock guard for logger streams.
 **/
template<class EventType, bool enabled>
class LogStreamLocker
{
  Mutex* mutex_p_;
public:

  /**
   * lock logStream to current thread
   **/
  LogStreamLocker(LogStream<EventType,enabled>& logStream)
  {
   if (enabled) {
     mutex_p_ = logStream.getMutexPtr();
     mutex_p_->lock();
   }
  }

  /**
   * unlock logStream
   **/
 ~LogStreamLocker()
  {
   if (enabled)  mutex_p_->unlock();
  }

};


#ifdef HAVE_PARTIAL_SPECIALIZATION

template<class EventType>
class LogStreamLocker<EventType, false>
{
public:
   LogStreamLocker(LogStream<EventType,false>&)  {}
   ~LogStreamLocker()  {}
};

template<class EventType>
class LogStreamLocker<EventType, true>
{
private:
   Mutex* mutex_p_;
public:
   LogStreamLocker(LogStream<EventType,true>& logStream) 
   {
    mutex_p_ = logStream.getMutexPtr();
    mutex_p_->lock();
   }

   ~LogStreamLocker()
   {
    mutex_p_->unlock();
   }

};

#endif


#ifndef _NDEBUG
# ifndef LOG_DEBUG_ENABLE
/** Are we want to log debug output ? [true/false]
 * default: true if _NDEBUG is not defined, otherwise false
 **/
#  define LOG_DEBUG_ENABLE false
# endif 
# ifndef LOG_INFO_ENABLE
/** Are we want to log info output ? [true/false]
 * default: true if _NDEBUG is not defined, otherwise false
 **/
#  define LOG_INFO_ENABLE true
# endif
#else
# ifndef LOG_DEBUG_ENABLE
#  define LOG_DEBUG_ENABLE false
# endif 
# ifndef LOG_INFO_ENABLE
#  define LOG_INFO_ENABLE false
# endif
#endif
#ifndef LOG_WARNING_ENABLE
/** Are we want to log warning output ? [true/false]
 * default: true
 **/
# define LOG_WARNING_ENABLE true
#endif
#ifndef LOG_ERROR_ENABLE
/** Are we want to log error output ? [true/false]
 * default: true
 **/
# define LOG_ERROR_ENABLE true
#endif
#ifndef LOG_FATAL_ENABLE
/** 
 * Are we want to log fatal error output ? [true/false]
 *  default: true
 **/
# define LOG_FATAL_ENABLE true
#endif
 
/**
 * stream for debug output
 **/
typedef LogStream<DebugLogEventStruct, LOG_DEBUG_ENABLE> DebugLogStream;

/**
 * stream for information output
 **/
typedef LogStream<InfoLogEventStruct, LOG_INFO_ENABLE>   InfoLogStream;

/**
 * stream for warning output
 **/
typedef LogStream<WarningLogEventStruct, LOG_WARNING_ENABLE> WarningLogStream;

/**
 * stream for errors output
 **/
typedef LogStream<ErrorLogEventStruct, LOG_ERROR_ENABLE> ErrorLogStream;

/**
 * stream for fatal errors output
 **/
typedef LogStream<FatalLogEventStruct, LOG_FATAL_ENABLE> FatalLogStream;

///

 /**
  * Main class for events log
  **/
 class Logger
 {
   public:

     /**
      * Struct, which indicate input/output exception
      **/
     struct IOException 
     {
       std::string message_;

       IOException(const char* message)
         :message_(message) {}

       ///
       const char* what() const { return message_.c_str(); }

     };

     ///
     Logger();

     ///
     Logger(const char* fname, bool dup_to_stderr=false) 
                      throw (IOException);

     ///
     ~Logger();

     ///
     void setOutputFile(const char* fname) throw (IOException);

     ///
     void  setCallback(LogEventType evTag, void (*callback)(const char* msg));

     /**
      * set property:  dup output to standart error stream for all streams
      **/
     void  setDuppedToStderr(bool x);

     /**
      * set output to syslog: if true we log to syslog, if false - no.
      * by default, output to syslog is enabled.
      **/
     void  setSyslogOutput(bool x)
       { syslogEnabled_ = x; }
          
     ///
     void  put_event(LogEventType evTag, 
                     const char* value, 
                     bool dup_to_stderr = false);

     ///
     DebugLogStream&   debugs() { return *debugStream_mp_; }
     ///
     const DebugLogStream&   debugs() const { return *debugStream_mp_; }


     ///
     InfoLogStream&   infos() { return *infoStream_mp_; }
     ///
     const InfoLogStream&   infos() const { return *infoStream_mp_; }

     ///
     WarningLogStream&   warnings() { return *warningStream_mp_; }
     ///
     const WarningLogStream&   warnings() const { return *warningStream_mp_; }

     ///
     ErrorLogStream&   errors() { return *errorStream_mp_; }
     ///
     const ErrorLogStream&   errors() const { return *errorStream_mp_; }

     ///
     FatalLogStream&   fatals() { return *fatalStream_mp_; }
     ///
     const FatalLogStream&   fatals() const { return *fatalStream_mp_; }

     ///
     static const char* getStringEventType(LogEventType ev);


    /**
     * type for Debug LogStream lock guard.
     **/
    typedef LogStreamLocker<DebugLogEventStruct,LOG_DEBUG_ENABLE> DebugLocker;

    /**
     * type for Info LogStream lock guard.
     **/
    typedef LogStreamLocker<InfoLogEventStruct, LOG_INFO_ENABLE> InfoLocker;

    /**
     * type for Warnings LogStream lock guard.
     **/
    typedef LogStreamLocker<WarningLogEventStruct,LOG_WARNING_ENABLE>  
                                                              WarningLocker;

    /**
     * type for Errors LogStream lock guard.
     **/
    typedef LogStreamLocker<ErrorLogEventStruct,LOG_ERROR_ENABLE> ErrorLocker;

    /**
     * type for Fatals LogStream lock guard
     **/
    typedef LogStreamLocker<FatalLogEventStruct,LOG_FATAL_ENABLE>  FatalLocker;


   private:

     void (*callbacks_[5])(const char* msg);
     char* fname_;
     std::ofstream  ofs_;
     bool   syslogEnabled_;

     DebugLogStream*   debugStream_mp_;
     InfoLogStream*   infoStream_mp_;
     WarningLogStream*   warningStream_mp_;
     ErrorLogStream*   errorStream_mp_;
     FatalLogStream*   fatalStream_mp_;

     Mutex   mutex_;

   private:

     void init(bool dup_to_stderr=false);
    
   private:

     Logger(const Logger&);
     Logger& operator=(const Logger&);

 };

 class LogStreamBase;

 class LogStreamBuff: public std::streambuf
 {
  private:
    Logger&      logger_;
    bool         duppedToStderr_;
    LogEventType tag_;
    bool         enabled_;
    std::string   str_;  
    
    bool         touched_;
    
    
  public:

    LogStreamBuff(Logger& logger, LogEventType tag,
                  bool enabled, bool duppedToStderr);

    ~LogStreamBuff();

  protected:

    friend class LogStreamBase;

    std::streamsize xsputn(const char* str, std::streamsize size);
    int overflow(int c);
    int sync();

    bool  isDuppedToStderr() const  { return duppedToStderr_; }
    void  setDuppedToStderr(bool x) { duppedToStderr_ = x; }

  private:

    LogStreamBuff();
    LogStreamBuff(const LogStreamBuff&);
    LogStreamBuff& operator=(const LogStreamBuff&);

 };


 class LogStreamBase
 {
  public:

  LogStreamBuff buff_;
  LogStreamBase(Logger& logger, LogEventType tag,
                bool enabled, bool duppedToStderr)
   :buff_(logger,tag,enabled,duppedToStderr) {}

  void setDuppedToStderr(bool x) { buff_.setDuppedToStderr(x); }
  bool isDuppedToStderr() const { return buff_.isDuppedToStderr(); }

 };


#if defined(_MSC_VER) && _MSC_VER < 1300
  using std::ostream;  // bug in MSVC templates implementation
#endif



 /**
  * Stream-like class for output to logger.
  * Example of usage:
  * \begin{verbatim}
  *  logStream << "can't open file " << fname << endl;
  * \end{verbatim}
  * Stream have 2 compile-time attributes:
  *\begin{itemize}
  *  \item EventType - type of events, which this stream can receive.
  *  \item enabled  - true, if we use output of this stream:
  *\end{itemize}
  *   operations with LogStream<evt,true> are really logging to stream,
  *   all operations with LogStream<evt,false> do nothing.
  **/

 template<class EventType, bool enabled>
 class LogStream: private LogStreamBase, 
#if defined(_MSC_VER) && _MSC_VER < 1300
                  public ostream
#else
                  public std::ostream
#endif
 {
  private:
     Mutex mutex_;
  public:
     
     // public, as implementation effects: 
     //   bug in template friends in C++ compilers.
     LogStream(Logger& logger, bool duppedToStderr)
      :LogStreamBase(logger,EventType::tag,enabled, duppedToStderr),
#if defined(_MSC_VER) && _MSC_VER < 1300
       ostream(&buff_),
#else
       std::ostream(&buff_),
#endif
       mutex_()
     {}

  public:

     ///
     void  setDuppedToStderr(bool x)
       { LogStreamBase::setDuppedToStderr(x); }

     ///
     bool  isDuppedToStderr() const
       { return LogStreamBase::isDuppedToStderr(); }

     // public, as implementation effects: 
     //   bug in template friends in C++ compilers.
     Mutex*  getMutexPtr() { return &mutex_; }

  private:
     LogStream();

     LogStream(const LogStream&);
     LogStream& operator=(const LogStream&);
 };

#if defined(HAVE_PARTIAL_SPECIALIZATION) && !defined(__SUNPRO_CC)

class DummyLogStreamBuff: public std::streambuf
{
public:
  DummyLogStreamBuff() {}
  int xsputn(char const *, int ) { return 0; };
  int overflow(int c) { return c; }
  int sync() { return 0; }
};

struct DummyLogStreamBase
{
 DummyLogStreamBuff buff_;
};

template<class EventType>
class LogStream<EventType, false>: private DummyLogStreamBase, 
                                   public std::ostream
{
public:
     LogStream(Logger& logger, bool duppedToStderr)
      :std::ostream(&buff_) {}
     void  setDuppedToStderr(bool x) {}
     bool  isDuppedToStderr() const { return false; }
     Mutex*  getMutexPtr() { return NULL; }
};

#endif

 

inline
Logger::Logger(const char* fname, bool dup_to_stderr) 
                                      throw (Logger::IOException)
{
  // must be defined here, becous it's depend from compile-time settings.
  // note1: we have no code-bloat, becouse usially only one instance of logger
  // is situated in program.
  // note2: all method of logger with touch stream instances must be here.
  setOutputFile(fname);  
  init(dup_to_stderr);
}

inline
void Logger::init(bool dup_to_stderr)
{
  for(int i=0; i<5; ++i)
  {
    callbacks_[i]=NULL;
  } 
  debugStream_mp_ = new DebugLogStream(*this, dup_to_stderr);
  infoStream_mp_ = new InfoLogStream(*this, dup_to_stderr);
  warningStream_mp_ = new WarningLogStream(*this, dup_to_stderr);
  errorStream_mp_ = new ErrorLogStream(*this, dup_to_stderr);
  fatalStream_mp_ = new FatalLogStream(*this, dup_to_stderr);
  syslogEnabled_ = true;
}

inline Logger::Logger()
{
 init();
}

inline     
Logger::~Logger()
{
  if (ofs_) ofs_.close();
  delete debugStream_mp_;
  delete infoStream_mp_;
  delete warningStream_mp_;
  delete errorStream_mp_;
  delete fatalStream_mp_;
}

inline
void  Logger::setDuppedToStderr(bool x)
{
 MutexLocker ml(mutex_);
 debugStream_mp_->setDuppedToStderr(x);
 infoStream_mp_->setDuppedToStderr(x);
 warningStream_mp_->setDuppedToStderr(x);
 errorStream_mp_->setDuppedToStderr(x);
 fatalStream_mp_->setDuppedToStderr(x);
}


}

#endif
