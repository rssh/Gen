#include <GradSoft/Logger.h>


#include <sys/types.h>
#include <time.h>

#ifdef HAVE_SYSLOG
#include <syslog.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef HAVE_CSTRING
#include <cstring>
#endif

#ifndef STREAMBUF_HAVE_PUBSEEKPOS
#define pubseekpos seekpos
#endif

using namespace std;

namespace GradSoft {


void Logger::setCallback(LogEventType evTag, 
                         void (*callback)(const char* msg))
{
 MutexLocker ml(mutex_);
 callbacks_[evTag]=callback;
}


void  Logger::put_event(LogEventType evType, const char* value, 
                        bool dup_to_stderr)
{
 char tmbuff[26];
 time_t t = time(NULL);

#ifdef HAVE_CTIME_R
#ifdef CTIME_R_HAVE_3_ARGS
 ctime_r(&t,tmbuff,26);
#else
 ctime_r(&t,tmbuff);
#endif
#elif defined(HAVE_CTIME)
 {
  MutexLocker ml(mutex_); 
  char *staticLine=ctime(&t);
  memcpy(tmbuff,staticLine,26*sizeof(char));
 }
#else
#error neither HAVE_CTIME_R nor HAVE_CTIME is defined
#endif
 
 tmbuff[strlen(tmbuff)-1]='\0'; // change \n to \0
 const char* szEvType = getStringEventType(evType);
 
 {
  // serialize output
  MutexLocker ml(mutex_);
  if (dup_to_stderr) {
    std::cerr << szEvType << ":" << tmbuff << ":" << value << std::flush;
  }
  if (ofs_) {
    ofs_ << szEvType << ":" << tmbuff << ":" << value << std::flush;
  }
 }
 if (callbacks_[evType]!=NULL) {
   callbacks_[evType](value);
 }

#ifdef HAVE_SYSLOG
 if (syslogEnabled_) {
  int syslogType;
  switch(evType) {
    case Debug:
              syslogType = LOG_DEBUG;          
              break;
    case Info:
              syslogType = LOG_INFO;          
              break;
    case Warning:
              syslogType = LOG_WARNING;          
              break;
    case Error:
              syslogType = LOG_ERR;          
              break;
    case Fatal:
              syslogType = LOG_ALERT;          
              break;
    default:
              // internal error  (i.e. near impossible)
              syslogType = LOG_CRIT;          
             break;
  }
  syslog(syslogType,"%s",value);
 }
#endif
}


void Logger::setOutputFile(const char* fname)
                                      throw (Logger::IOException)
{
 MutexLocker ml(mutex_);
#ifdef HAVE_IOSBASE_FAILURE
 try {
#endif
  if (ofs_.is_open()) {
    ofs_.close();
    if (ofs_.bad()) {
      ostringstream ostr;
      ostr << "Can't close file " << fname << ":" << strerror(errno) << '\0';
      throw IOException(ostr.str().c_str());
    }
  }
  ofs_.clear();
  ofs_.open(fname);
  if (ofs_.fail()) {
     ostringstream ostr;
     ostr << "Can't open file " << fname << ":" << strerror(errno) << '\0';
     throw IOException(ostr.str().c_str());
  }
#ifdef HAVE_IOSBASE_FAILURE
 }catch(const iosbase_failure& ex){
   throw IOException(ex.what());
 }
#endif
}


const char* Logger::getStringEventType(LogEventType evType)
{
 switch(evType)
 {
   case Debug:
          return "Debug";
   case Info:
          return "Info";
   case Warning:
          return "Warning";
   case Error:
          return "Error";
   case Fatal:
          return "Fatal";
   default:
          return "UnknownEventType";
 }
}


const LogEventType DebugLogEventStruct::tag=Debug;
const LogEventType InfoLogEventStruct::tag=Info;
const LogEventType WarningLogEventStruct::tag=Warning;
const LogEventType ErrorLogEventStruct::tag=Error;
const LogEventType FatalLogEventStruct::tag=Fatal;


LogStreamBuff::LogStreamBuff(Logger& logger, LogEventType tag,
                             bool enabled, bool duppedToStderr)
      :logger_(logger),
       tag_(tag),
       enabled_(enabled),
       duppedToStderr_(duppedToStderr),
       str_(),
       touched_(false)
{
 setp(0,0);
 setg(0,0,0);
} 

LogStreamBuff::~LogStreamBuff()
{
  sync();
  enabled_=false;
}


streamsize LogStreamBuff::xsputn(const char* str, streamsize size)
                                 
{
 touched_ = true;
 if (!enabled_) return size;
 str_.append(str,size);
 return size;
}
 
int LogStreamBuff::overflow(int c)
{
 touched_ = true;
 if (!enabled_) return c;
 if (c!=EOF) str_.push_back((char)c);
 return c;
}

int LogStreamBuff::sync()
{
 if (!touched_) {
     return 0;
 }
 if (!enabled_) {
    return 0;
 }
 logger_.put_event(tag_,str_.c_str(),isDuppedToStderr());
 str_.clear();
 //str_=std::string("");
 touched_ = false;
 return 0;
}


}

